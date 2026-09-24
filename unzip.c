#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "unzip.h"
#include "util.h"
#include "zlib.h"
#include <unistd.h>

#define HEADER_SIZE 30
#define CHUNK 65536

#define LE_READ16(buf) ((uint16_t)(((uint8_t *)(buf))[1] << 8 | ((uint8_t *)(buf))[0]))
#define LE_READ32(buf) ((uint32_t)(((uint8_t *)(buf))[3] << 24 | ((uint8_t *)(buf))[2] << 16 | ((uint8_t *)(buf))[1] << 8 | ((uint8_t *)(buf))[0]))

struct unzip_write_context {
	const struct unzip_info *info;
	unzip_progress_cb progress;
	void *userdata;

	uint64_t written;
	uLong crc;
};

struct file_info {
	struct unzip_info info;

	int (*write)(
		FILE *zip,
		FILE *dest,
		struct unzip_write_context *context
	);
};

static int write_chunk(
	FILE *dest,
	const uint8_t *data,
	size_t size,
	struct unzip_write_context *context)
{
	if (!size)
		return 0;

	/*
	 * A corrupt ZIP must not be allowed to produce more data than
	 * announced in its header.
	 */
	if (context->written + size > context->info->uncompressed_size) {
		PA_ERROR("ZIP content exceeds expected uncompressed size\n");
		return -1;
	}

	if (fwrite(data, 1, size, dest) != size || ferror(dest))
		return -1;

	/*
	 * Calculate CRC while the block is already in memory.
	 * No second read of the extracted ROM is necessary.
	 */
	context->crc = crc32(
		context->crc,
		data,
		(uInt)size
	);

	context->written += size;

	if (context->progress) {
		context->progress(
			context->written,
			context->info->uncompressed_size,
			context->userdata
		);
	}

	return 0;
}

static int write_uncompressed(
	FILE *zip,
	FILE *dest,
	struct unzip_write_context *context)
{
	uint8_t buf[CHUNK];
	uint32_t remaining = context->info->compressed_size;

	while (remaining) {
		size_t size = MIN(remaining, CHUNK);

		if (fread(buf, 1, size, zip) != size)
			return -1;

		if (write_chunk(dest, buf, size, context) != 0)
			return -1;

		remaining -= size;
	}

	return 0;
}

static int write_inflate(
	FILE *zip,
	FILE *dest,
	struct unzip_write_context *context)
{
	z_stream stream = {0};
	uint8_t in[CHUNK];
	uint8_t out[CHUNK];
	uint32_t remaining = context->info->compressed_size;
	int ret;

	ret = inflateInit2(&stream, -MAX_WBITS);
	if (ret != Z_OK)
		return -1;

	ret = Z_OK;

	while (remaining && ret != Z_STREAM_END) {
		size_t input_size = MIN(remaining, CHUNK);

		if (fread(in, 1, input_size, zip) != input_size) {
			ret = Z_ERRNO;
			break;
		}

		remaining -= input_size;

		stream.avail_in = input_size;
		stream.next_in = in;

		do {
			size_t produced;

			stream.avail_out = CHUNK;
			stream.next_out = out;

			ret = inflate(&stream, Z_NO_FLUSH);

			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;
				break;

			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
			case Z_STREAM_ERROR:
				goto finish;

			default:
				break;
			}

			produced = CHUNK - stream.avail_out;

			if (write_chunk(
					dest,
					out,
					produced,
					context) != 0) {
				ret = Z_ERRNO;
				goto finish;
			}

		} while (stream.avail_out == 0 && ret != Z_STREAM_END);
	}

finish:
	inflateEnd(&stream);

	return ret == Z_STREAM_END ? 0 : -1;
}

static int extract_entry(
	FILE *zip,
	FILE *dest,
	struct file_info *info,
	unzip_progress_cb progress,
	void *userdata)
{
	struct unzip_write_context context = {
		.info = &info->info,
		.progress = progress,
		.userdata = userdata,
		.written = 0,
		.crc = crc32(0L, Z_NULL, 0),
	};

	if (progress) {
		progress(
			0,
			info->info.uncompressed_size,
			userdata
		);
	}

	if (info->write(zip, dest, &context) != 0) {
		PA_ERROR("Error decompressing file\n");
		return -1;
	}

	if (context.written != info->info.uncompressed_size) {
		PA_ERROR(
			"ZIP size mismatch: expected %u bytes, got %llu\n",
			info->info.uncompressed_size,
			(unsigned long long)context.written
		);
		return -1;
	}

	if ((uint32_t)context.crc != info->info.crc32) {
		PA_ERROR(
			"ZIP CRC mismatch: expected %08x, got %08x\n",
			(unsigned int)info->info.crc32,
			(unsigned int)context.crc
		);
		return -1;
	}

	return 0;
}

static int find_entry(FILE *zip, const char **extensions, struct file_info *info) {
	int ret = -1;
	uint8_t header[HEADER_SIZE];
	uint32_t next = 0;
	uint16_t file_name_size = 0;
	char extension[10];

	while(1) {
		bool match = false;

		if (next)
			fseek(zip, next, SEEK_CUR);

		if (HEADER_SIZE != fread(header, 1, HEADER_SIZE, zip))
			break;

		if (header[0] != 0x50 || header[1] != 0x4b || header[2] != 0x03 || header[3] != 0x04)
			break;

		if ((uint16_t)(header[6]) & 0x0008) {
			PA_ERROR("Unsupported zip file\n");
			break;
		}

		file_name_size = LE_READ16(&header[26]);
		if (file_name_size >= MAX_PATH)
			break;

		if (file_name_size != fread(info->info.filename, 1, file_name_size, zip))
			break;

		info->info.filename[file_name_size] = '\0';
		info->info.crc32 = LE_READ32(&header[14]);
		info->info.compressed_size = LE_READ32(&header[18]);
		info->info.uncompressed_size = LE_READ32(&header[22]);

		fseek(zip, LE_READ16(&header[28]), SEEK_CUR);

		next = info->info.compressed_size;

		for (int i = 0; extensions[i]; i++) {
			snprintf(extension, 10, ".%s", extensions[i]);
			if (has_suffix_i(info->info.filename, extension)) {
				match = true;
				break;
			}
		}

		if (!match)
			continue;

		switch (LE_READ16(&header[8])) {
			case 0: /* Uncompressed */
				info->write = write_uncompressed;
				break;

			case 8: /* DEFLATE */
				info->write = write_inflate;
				break;

			default:
				PA_ERROR("Unsupported zip compression method\n");
				goto finish;
			}

		ret = 0;
		break;
	}
	finish:
		return ret;
}

int unzip_get_info(FILE *zip, const char **extensions, struct unzip_info *info) {
	struct file_info file_info = {0};
	int ret;

	ret = find_entry(zip, extensions, &file_info);
	if (!ret) {
		*info = file_info.info;
	}

	rewind(zip);

	return ret;
}

int unzip_tmp_progress(
	FILE *zip,
	const char **extensions,
	char *filename,
	size_t len,
	unzip_progress_cb progress,
	void *userdata)
{
	int ret = -1;
	struct file_info info = {0};
	FILE *dest = NULL;

	if (!find_entry(zip, extensions, &info)) {
		const char *entry_name;
		int fd;

		entry_name = basename(info.info.filename);

		snprintf(
			filename,
			len,
			"/tmp/pa-XXXXXX%s",
			entry_name
		);

		fd = mkstemps(filename, strlen(entry_name));
		if (fd < 0) {
			PA_ERROR("Error creating temporary file for decompression\n");
			goto finish;
		}

		dest = fdopen(fd, "wb");

		if (!dest) {
			close(fd);
			PA_ERROR("Error opening temporary file for decompression\n");
			goto finish;
		}

		if (extract_entry(
				zip,
				dest,
				&info,
				progress,
				userdata) != 0) {
			goto finish;
		}

		ret = 0;
	}

finish:
	if (dest)
		fclose(dest);

	return ret;
}

int unzip_tmp(
	FILE *zip,
	const char **extensions,
	char *filename,
	size_t len)
{
	return unzip_tmp_progress(
		zip,
		extensions,
		filename,
		len,
		NULL,
		NULL
	);
}

int unzip_progress(
	FILE *zip,
	const char **extensions,
	FILE *dest,
	unzip_progress_cb progress,
	void *userdata)
{
	struct file_info info = {0};

	if (find_entry(zip, extensions, &info) != 0)
		return -1;

	return extract_entry(
		zip,
		dest,
		&info,
		progress,
		userdata
	);
}

int unzip(
	FILE *zip,
	const char **extensions,
	FILE *dest)
{
	return unzip_progress(
		zip,
		extensions,
		dest,
		NULL,
		NULL
	);
}
