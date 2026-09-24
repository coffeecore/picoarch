#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "core.h"
#include "content.h"
#include "content_cache.h"
#include "patch.h"
#include "unzip.h"
#include "util.h"
#include <inttypes.h>
#include "plat.h"

static int alloc_readfile(const char *path, void **buf, size_t *size) {
	int ret = -1;
	FILE *file = fopen(path, "r");

	if (!file) {
		goto finish;
	}

	fseek(file, 0, SEEK_END);
	*size = ftell(file);
	rewind(file);

	if (!*size) {
		ret = 0;
		goto finish;
	}

	*buf = malloc(*size);
	if (!buf) {
		PA_ERROR("Couldn't allocate memory for file: %s\n", path);
		goto finish;
	}

	if (*size != fread(*buf, sizeof(uint8_t), *size, file)) {
		PA_ERROR("Error reading file: %s\n", path);
		goto finish;
	}
	ret = 0;

finish:
	if (ret) {
		free(*buf);
		*buf = NULL;
		*size = 0;
	}

	if (file)
		fclose(file);

	return ret;
}

static void content_clear_file(struct content *content) {
	if (!content->file)
		return;

	if (content->file_is_temporary)
		remove(content->file);

	free(content->file);
	content->file = NULL;
	content->file_is_temporary = false;
}

struct content_progress {
	unsigned last_percent;
};

static void content_unzip_progress(
	uint64_t current,
	uint64_t total,
	void *userdata)
{
	struct content_progress *progress = userdata;
	unsigned percent;

	if (!progress)
		return;

	if (!total)
		percent = 100;
	else
		percent = (unsigned)(current * 100 / total);

	if (percent > 100)
		percent = 100;

	/*
	 * unzip.c calls us once with current == 0.
	 */
	if (current == 0) {
		progress->last_percent = 0;
		plat_video_progress("Extracting ROM...", 0);
		return;
	}

	/*
	 * Updating SDL for every 64 KiB block is unnecessary.
	 * Refresh every 2%, plus always show 100%.
	 */
	if (percent != 100 &&
		percent < progress->last_percent + 2) {
		return;
	}

	progress->last_percent = percent;

	plat_video_progress(
		"Extracting ROM...",
		percent
	);
}

static int content_load_zip(struct content *content)
{
	const char *ext = NULL;
	int i = 0;
	int ret = -1;
	FILE *f = NULL;
	const char **extensions = core_extensions();
	struct unzip_info info = {0};
	struct content_cache_target cache_target = {0};
	struct content_progress progress = {0};

	if (!extensions || !has_suffix_i(content->path, ".zip"))
		return 0;

	while ((ext = extensions[i++])) {
		if (!strcmp(ext, "zip"))
			return 0;
	}

	f = fopen(content->path, "r");
	if (!f)
		goto finish;

	if (unzip_get_info(f, extensions, &info)) {
		PA_ERROR("Couldn't read ZIP content information\n");
		goto finish;
	}

	content_clear_file(content);

	/*
	 * Keep small content in RAM-backed /tmp whenever enough space is
	 * available. If no cache is configured, retain PicoArch's historical
	 * behaviour and use /tmp regardless of size.
	 */
	if (content_cache_use_ram(info.uncompressed_size) ||
		!content_cache_enabled()) {
		content->file = calloc(MAX_PATH, sizeof(*content->file));
		if (!content->file) {
			PA_ERROR("Couldn't allocate memory for unzipped path\n");
			goto finish;
		}

		content->file_is_temporary = true;

		PA_INFO(
			"ZIP content: %s (%" PRIu32 " bytes) -> RAM\n",
			info.filename,
			info.uncompressed_size
		);

		if (unzip_tmp_progress(
				f,
				extensions,
				content->file,
				MAX_PATH,
				content_unzip_progress,
				&progress))
		{
			plat_video_progress_clear();
			content_clear_file(content);
			goto finish;
		}

		ret = 0;
		goto finish;
	}

	/*
	 * Large content uses the persistent cache. At this stage we only
	 * support cache hits: cache creation will be added separately.
	 */
	if (content_cache_prepare(&info, &cache_target)) {
		PA_ERROR("Couldn't prepare content cache\n");
		goto finish;
	}

	if (cache_target.hit) {
		PA_INFO(
			"ZIP content: %s (%" PRIu32 " bytes) -> cache hit: %s\n",
			info.filename,
			info.uncompressed_size,
			cache_target.path
		);
	} else {
		PA_INFO(
			"ZIP content: %s (%" PRIu32 " bytes) -> cache miss: %s\n",
			info.filename,
			info.uncompressed_size,
			cache_target.path
		);
	}

	if (!cache_target.hit) {
		FILE *dest = NULL;
		struct stat st;
		int dest_fd;

		dest = fopen(cache_target.part_path, "wb");
		if (!dest) {
			PA_ERROR(
				"Couldn't create cache file %s: %s\n",
				cache_target.part_path,
				strerror(errno)
			);
			goto finish;
		}

		/*
		* Decompress completely into the .part file.
		*/
		if (unzip_progress(
		f,
		extensions,
		dest,
		content_unzip_progress,
		&progress)) {
			PA_ERROR(
				"Couldn't decompress content into cache file %s\n",
				cache_target.part_path
			);

			fclose(dest);
			dest = NULL;

			content_cache_abort(&cache_target);
			goto finish;
		}

		/*
		* Push stdio's userspace buffer to the kernel first.
		*/
		if (fflush(dest) != 0) {
			PA_ERROR(
				"Couldn't flush cache file %s: %s\n",
				cache_target.part_path,
				strerror(errno)
			);

			fclose(dest);
			dest = NULL;

			content_cache_abort(&cache_target);
			goto finish;
		}

		/*
		* Then ask the kernel to flush the actual file data to storage
		* before we make the cache entry visible under its final name.
		*/
		dest_fd = fileno(dest);

		if (dest_fd < 0 || fsync(dest_fd) != 0) {
			PA_ERROR(
				"Couldn't sync cache file %s: %s\n",
				cache_target.part_path,
				strerror(errno)
			);

			fclose(dest);
			dest = NULL;

			content_cache_abort(&cache_target);
			goto finish;
		}

		/*
		* fclose() can itself report a delayed write error, so check it too.
		*/
		if (fclose(dest) != 0) {
			dest = NULL;

			PA_ERROR(
				"Couldn't close cache file %s: %s\n",
				cache_target.part_path,
				strerror(errno)
			);

			content_cache_abort(&cache_target);
			goto finish;
		}

		dest = NULL;

		/*
		* Validate the resulting size before committing the cache entry.
		*/
		if (stat(cache_target.part_path, &st) != 0) {
			PA_ERROR(
				"Couldn't stat cached content %s: %s\n",
				cache_target.part_path,
				strerror(errno)
			);

			content_cache_abort(&cache_target);
			goto finish;
		}

		if (!S_ISREG(st.st_mode) ||
			(uint64_t)st.st_size != info.uncompressed_size) {
			PA_ERROR(
				"Cached content has invalid size "
				"(expected %" PRIu32 ", got %" PRIu64 " bytes)\n",
				info.uncompressed_size,
				(uint64_t)st.st_size
			);

			content_cache_abort(&cache_target);
			goto finish;
		}

		/*
		* Atomic rename:
		*
		* xxx.gba.part
		*      ↓
		* xxx.gba
		*/
		if (content_cache_commit(&cache_target) != 0) {
			content_cache_abort(&cache_target);
			goto finish;
		}
	}

	content->file = strdup(cache_target.path);
	if (!content->file) {
		PA_ERROR("Couldn't allocate memory for cached content path\n");
		goto finish;
	}

	content->file_is_temporary = false;
	ret = 0;

finish:
	if (f)
		fclose(f);

	return ret;
}

static char *content_patch_pattern;

static int content_patch_filter(const struct dirent *ent) {
	const char *p;

	if (content_patch_pattern &&
	    !strncmp(ent->d_name, content_patch_pattern, strlen(content_patch_pattern))) {
		p = ent->d_name + strlen(content_patch_pattern);

		return !strncasecmp(p, ".ips", sizeof(".ips") - 1) ||
			!strncasecmp(p, ".bps", sizeof(".bps") - 1);
	}

	return 0;
}

static int content_patch_compare(const struct dirent **d1, const struct dirent **d2) {
	return strcasecmp((*d1)->d_name, (*d2)->d_name);
}

static int content_patch(const struct content *content, void *data, size_t size, void **out, size_t *out_size) {
	struct dirent **namelist;
	char pattern[MAX_PATH];
	char patch_path[MAX_PATH];
	int n = 0;
	char *path = strdup(content->path);
	char *dir;

	const void *in = data;
	size_t in_size = size;
	void *patch_data = NULL;
	size_t patch_size = 0;
	void *patched = NULL;
	size_t patched_size = 0;

	int i = 0;
	int ret = -1;

	dir = dirname(path);
	content_based_name(content, pattern, sizeof(pattern), NULL, NULL, "");

	content_patch_pattern = basename(pattern);
	n = scandir(dir, &namelist, content_patch_filter, content_patch_compare);
	content_patch_pattern = NULL;

	if (n < 0) {
		PA_ERROR("Error reading directory: %s\n", strerror(errno));
		goto finish;
	}

	if (n == 0) {
		goto finish;
	}

	for (i = 0; i < n; i++) {
		free(patch_data);
		snprintf(patch_path, sizeof(patch_path), "%s%s%s", dir, "/", namelist[i]->d_name);

		if (alloc_readfile(patch_path, &patch_data, &patch_size)) {
			goto finish;
		}

		if (patched) {
			if (in != data)
				free((void *)in);

			in = patched;
			in_size = patched_size;
			patched = NULL;
		}

		if (patch(in, in_size, patch_data, patch_size, &patched, &patched_size))
			goto finish;

		PA_INFO("Applied %s\n", patch_path);
	}

	*out = patched;
	*out_size = patched_size;

	ret = 0;
finish:

	while (n--) {
		free(namelist[n]);
	}
	free(namelist);

	if (in != data)
		free((void *)in);

	free(patch_data);
	free(path);
	return ret;
}

static int content_patch_file(struct content *content, const char *path)
{
	int fd = -1;
	int outfd = -1;
	int ret = -1;
	struct stat st = {0};
	void *addr = MAP_FAILED;
	void *out = NULL;
	size_t out_size = 0;
	char *content_path = NULL;
	char *content_name;
	char *new_file = NULL;
	char patched_path[MAX_PATH] = {0};
	FILE *outfile = NULL;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		PA_ERROR("Couldn't open content file\n");
		goto finish;
	}

	if (fstat(fd, &st) != 0) {
		PA_ERROR("Couldn't read content file size\n");
		goto finish;
	}

	addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (addr == MAP_FAILED) {
		PA_ERROR("Couldn't read content file\n");
		goto finish;
	}

	/*
	 * No patch is not an error for the caller:
	 * content_patch() returns non-zero when there is nothing to apply.
	 */
	if (content_patch(content, addr, st.st_size, &out, &out_size))
		goto finish;

	/*
	 * Use the actual content path, not content->path.
	 *
	 * If the source came from a ZIP/cache, this preserves the real
	 * extension (.gba, .bin, ...), rather than using ".zip".
	 */
	content_path = strdup(path);
	if (!content_path) {
		PA_ERROR("Couldn't allocate patched content path\n");
		goto finish;
	}

	content_name = basename(content_path);

	if (snprintf(
			patched_path,
			sizeof(patched_path),
			"/tmp/pa-XXXXXX%s",
			content_name) >= (int)sizeof(patched_path)) {
		PA_ERROR("Patched content path is too long\n");
		goto finish;
	}

	outfd = mkstemps(patched_path, strlen(content_name));
	if (outfd < 0) {
		PA_ERROR(
			"Error creating temporary file for patching: %s\n",
			strerror(errno)
		);
		goto finish;
	}

	outfile = fdopen(outfd, "wb");
	if (!outfile) {
		PA_ERROR(
			"Error opening temporary file for patching: %s\n",
			strerror(errno)
		);
		goto finish;
	}

	/*
	 * outfile owns outfd from this point.
	 */
	outfd = -1;

	if (out_size != fwrite(out, 1, out_size, outfile)) {
		PA_ERROR(
			"Error writing patched content: %s\n",
			strerror(errno)
		);
		goto finish;
	}

	/*
	 * Close successfully before making this the active content file.
	 */
	if (fclose(outfile) != 0) {
		outfile = NULL;

		PA_ERROR(
			"Error closing patched content: %s\n",
			strerror(errno)
		);
		goto finish;
	}

	outfile = NULL;

	/*
	 * Allocate the new pathname BEFORE releasing the current content.
	 * This way a memory-allocation failure leaves the old content intact.
	 */
	new_file = strdup(patched_path);
	if (!new_file) {
		PA_ERROR("Couldn't allocate patched content path\n");
		goto finish;
	}

	/*
	 * This removes the previous file only if it was temporary.
	 *
	 * A persistent ZIP cache entry has:
	 *     file_is_temporary == false
	 *
	 * so it is never deleted here.
	 */
	content_clear_file(content);

	content->file = new_file;
	content->file_is_temporary = true;
	new_file = NULL;

	/*
	 * Ownership of the temporary file now belongs to content->file.
	 */
	patched_path[0] = '\0';

	ret = 0;

finish:
	if (outfile)
		fclose(outfile);
	else if (outfd >= 0)
		close(outfd);

	/*
	 * If the patched file was created but never adopted by content,
	 * remove it.
	 */
	if (patched_path[0])
		remove(patched_path);

	if (addr != MAP_FAILED)
		munmap(addr, st.st_size);

	if (fd >= 0)
		close(fd);

	free(new_file);
	free(out);
	free(content_path);

	return ret;
}

struct content *content_init(const char *path) {
	struct content* content = calloc(1, sizeof(struct content));

	if (content) {
		strncpy((char *)content->path, path, sizeof(content->path) - 1);
	}
	return content;
}

void content_based_name(const struct content *content,
                        char *buf, size_t len,
                        const char *basedir, const char *subdir,
                        const char *new_extension) {
	char filename[MAX_PATH];
	char *path = strdup(content->path);
	char *dot;

	if (basedir) {
		if (!subdir)
			subdir = "";

		strncpy(filename, basename(path), sizeof(filename));
	} else {
		basedir = "";
		subdir = "";
		strncpy(filename, path, sizeof(filename));
	}

	filename[sizeof(filename) - 1] = 0;

	dot = strrchr(filename, '.');
	if (dot)
		*dot = 0;

	snprintf(buf, len, "%s%s%s%s", basedir, subdir, filename, new_extension);

	free(path);
}

int content_load_game_info(struct content *content, struct retro_game_info *info, bool needs_fullpath) {
	const char *path;
	int ret = -1;
	PA_INFO("Loading %s\n", content->path);

	if (content_load_zip(content)) {
		PA_ERROR("Error unzipping content file: %s\n", content->path);
		goto finish;
	}

	path = content->file ? content->file : content->path;

	if (needs_fullpath) {
		content_patch_file(content, path);
		path = content->file ? content->file : content->path;
		info->path = path;
	} else {
		void *data = NULL;
		size_t size = 0;
		void *patched_data = NULL;
		size_t patched_size = 0;

		free(content->data);
		content->data = NULL;

		if (alloc_readfile(path, &data, &size)) {
			PA_ERROR("Error reading content file: %s\n", path);
			goto finish;
		}

		if (!content_patch(content, data, size, &patched_data, &patched_size) && patched_data) {
			free(data);
			data = patched_data;
			size = patched_size;
		}

		info->path = path;
		info->data = content->data = data;
		info->size = content->size = size;
	}
	ret = 0;

finish:
	return ret;
}

void content_free(struct content *content) {
	if (!content)
		return;

	content_clear_file(content);

	free(content->data);
	free(content);
}
