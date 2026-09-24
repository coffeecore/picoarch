#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <utime.h>

#include <inttypes.h>
#include <sys/stat.h>

#include "content_cache.h"
#include "main.h"

bool content_cache_enabled(void)
{
	const char *root = getenv("PICOARCH_CACHE_ROOT");

	return root && root[0];
}

bool content_cache_use_ram(uint32_t size)
{
	struct statvfs stat;
	uint64_t available;

	if (size > CONTENT_RAM_THRESHOLD)
		return false;

	if (statvfs("/tmp", &stat) != 0)
		return false;

	available = (uint64_t)stat.f_bavail * stat.f_frsize;

	return available >= (uint64_t)size + CONTENT_TMP_RESERVE;
}

static const char *content_extension(const char *filename)
{
	const char *name;
	const char *extension;

	name = strrchr(filename, '/');
	name = name ? name + 1 : filename;

	extension = strrchr(name, '.');
	if (!extension || extension == name)
		return "";

	return extension;
}

static void content_cache_fix_fifo_time(
	const struct content_cache_target *target)
{
	const char *root = getenv("PICOARCH_CACHE_ROOT");
	DIR *dir;
	struct dirent *entry;
	struct stat target_stat;
	struct stat st;
	struct utimbuf times;
	char directory[MAX_PATH];
	char path[MAX_PATH];
	time_t newest = 0;

	if (!root || !root[0] || target->oversize)
		return;

	if (snprintf(
			directory,
			sizeof(directory),
			"%s/normal",
			root) >= (int)sizeof(directory)) {
		return;
	}

	if (stat(target->path, &target_stat) != 0)
		return;

	dir = opendir(directory);
	if (!dir)
		return;

	while ((entry = readdir(dir))) {
		if (!strcmp(entry->d_name, ".") ||
			!strcmp(entry->d_name, "..")) {
			continue;
		}

		if (snprintf(
				path,
				sizeof(path),
				"%s/%s",
				directory,
				entry->d_name) >= (int)sizeof(path)) {
			continue;
		}

		if (!strcmp(path, target->path))
			continue;

		if (stat(path, &st) != 0 || !S_ISREG(st.st_mode))
			continue;

		if (st.st_mtime > newest)
			newest = st.st_mtime;
	}

	closedir(dir);

	if (newest && target_stat.st_mtime <= newest) {
		times.actime = target_stat.st_atime;
		times.modtime = newest + 2;

		if (utime(target->path, &times) != 0) {
			PA_ERROR(
				"Couldn't update cache FIFO timestamp for %s: %s\n",
				target->path,
				strerror(errno)
			);
		}
	}
}

static int content_cache_path(
	const struct unzip_info *info,
	char *path,
	size_t path_size,
	bool part)
{
	const char *root = getenv("PICOARCH_CACHE_ROOT");
	const char *directory;
	const char *extension;
	int length;

	if (!root || !root[0])
		return -1;

	directory = info->uncompressed_size > CONTENT_CACHE_MAX
		? "oversize"
		: "normal";

	extension = content_extension(info->filename);

	length = snprintf(
		path,
		path_size,
		"%s/%s/%08" PRIx32 "-%" PRIu32 "%s%s",
		root,
		directory,
		info->crc32,
		info->uncompressed_size,
		extension,
		part ? ".part" : ""
	);

	if (length < 0 || (size_t)length >= path_size) {
		PA_ERROR("Content cache path is too long\n");
		return -1;
	}

	return 0;
}

static int cache_mkdir(const char *path)
{
	if (mkdir(path, 0755) != 0 && errno != EEXIST) {
		PA_ERROR("Couldn't create cache directory %s: %s\n",
			path, strerror(errno));
		return -1;
	}

	return 0;
}

static int content_cache_directories(
	char *normal,
	size_t normal_size,
	char *oversize,
	size_t oversize_size)
{
	const char *root = getenv("PICOARCH_CACHE_ROOT");

	if (!root || !root[0])
		return -1;

	if (snprintf(normal, normal_size, "%s/normal", root) >= normal_size)
		return -1;

	if (snprintf(oversize, oversize_size, "%s/oversize", root) >= oversize_size)
		return -1;

	if (cache_mkdir(root) != 0 ||
		cache_mkdir(normal) != 0 ||
		cache_mkdir(oversize) != 0) {
		return -1;
	}

	return 0;
}

static void content_cache_clean_parts(const char *directory)
{
	DIR *dir;
	struct dirent *entry;
	char path[MAX_PATH];

	dir = opendir(directory);
	if (!dir)
		return;

	while ((entry = readdir(dir))) {
		size_t len = strlen(entry->d_name);

		if (len < 5 || strcmp(entry->d_name + len - 5, ".part"))
			continue;

		if (snprintf(path, sizeof(path), "%s/%s",
				directory, entry->d_name) >= sizeof(path)) {
			continue;
		}

		remove(path);
	}

	closedir(dir);
}

static int content_cache_scan(
	const char *directory,
	uint64_t *total,
	char *oldest,
	size_t oldest_size,
	uint64_t *oldest_size_bytes)
{
	DIR *dir;
	struct dirent *entry;
	struct stat st;
	time_t oldest_time = 0;
	char path[MAX_PATH];

	*total = 0;
	oldest[0] = '\0';
	*oldest_size_bytes = 0;

	dir = opendir(directory);
	if (!dir)
		return -1;

	while ((entry = readdir(dir))) {
		size_t len;

		if (!strcmp(entry->d_name, ".") ||
			!strcmp(entry->d_name, "..")) {
			continue;
		}

		len = strlen(entry->d_name);

		if (len >= 5 &&
			!strcmp(entry->d_name + len - 5, ".part")) {
			continue;
		}

		if (snprintf(path, sizeof(path), "%s/%s",
				directory, entry->d_name) >= sizeof(path)) {
			continue;
		}

		if (stat(path, &st) != 0 || !S_ISREG(st.st_mode))
			continue;

		*total += (uint64_t)st.st_size;

		if (!oldest[0] || st.st_mtime < oldest_time) {
			strncpy(oldest, path, oldest_size - 1);
			oldest[oldest_size - 1] = '\0';

			oldest_time = st.st_mtime;
			*oldest_size_bytes = (uint64_t)st.st_size;
		}
	}

	closedir(dir);

	return 0;
}

static int content_cache_free_space(
	const char *path,
	uint64_t *available)
{
	struct statvfs stat;

	if (statvfs(path, &stat) != 0) {
		PA_ERROR("Couldn't get free space for %s: %s\n",
			path, strerror(errno));
		return -1;
	}

	*available =
		(uint64_t)stat.f_bavail *
		(uint64_t)stat.f_frsize;

	return 0;
}

static int content_cache_prepare_normal(
	const char *directory,
	uint64_t size)
{
	uint64_t total;
	uint64_t available;
	uint64_t oldest_size;
	char oldest[MAX_PATH];

	if (content_cache_scan(
			directory,
			&total,
			oldest,
			sizeof(oldest),
			&oldest_size) != 0) {
		return -1;
	}

	if (content_cache_free_space(directory, &available) != 0)
		return -1;

	/*
	 * Even deleting the whole normal cache would not leave enough
	 * free space while preserving the SD reserve.
	 *
	 * Check this before deleting anything.
	 */
	if (available + total < size + CONTENT_SD_RESERVE) {
		PA_ERROR("Not enough free SD space for cached content\n");
		return -1;
	}

	while (
		total + size > CONTENT_CACHE_MAX ||
		available < size + CONTENT_SD_RESERVE
	) {
		if (!oldest[0]) {
			PA_ERROR("Couldn't free enough content cache space\n");
			return -1;
		}

		if (remove(oldest) != 0) {
			PA_ERROR("Couldn't evict cache file %s: %s\n",
				oldest, strerror(errno));
			return -1;
		}

		if (oldest_size <= total)
			total -= oldest_size;
		else
			total = 0;

		if (content_cache_scan(
				directory,
				&total,
				oldest,
				sizeof(oldest),
				&oldest_size) != 0) {
			return -1;
		}

        if (content_cache_free_space(directory, &available) != 0)
            return -1;
	}

	return 0;
}

static int content_cache_prepare_oversize(
	const char *directory,
	uint64_t size)
{
	uint64_t total;
	uint64_t available;
	uint64_t file_size;
	char file[MAX_PATH];

	if (content_cache_scan(
			directory,
			&total,
			file,
			sizeof(file),
			&file_size) != 0) {
		return -1;
	}

	if (content_cache_free_space(directory, &available) != 0)
		return -1;

	/*
	 * Check before deleting the current oversize entry.
	 */
	if (available + total < size + CONTENT_SD_RESERVE) {
		PA_ERROR("Not enough free SD space for oversize content\n");
		return -1;
	}

	while (file[0]) {
		if (remove(file) != 0) {
			PA_ERROR("Couldn't remove oversize cache file %s: %s\n",
				file, strerror(errno));
			return -1;
		}

		if (content_cache_scan(
				directory,
				&total,
				file,
				sizeof(file),
				&file_size) != 0) {
			return -1;
		}
	}

	return 0;
}

int content_cache_prepare(
	const struct unzip_info *info,
	struct content_cache_target *target)
{
	struct stat st;
	char normal[MAX_PATH];
	char oversize[MAX_PATH];
	const char *directory;

	if (!info || !target || !content_cache_enabled())
		return -1;

	memset(target, 0, sizeof(*target));

	target->oversize =
		info->uncompressed_size > CONTENT_CACHE_MAX;

	if (content_cache_path(
			info,
			target->path,
			sizeof(target->path),
			false) != 0) {
		return -1;
	}

	if (content_cache_path(
			info,
			target->part_path,
			sizeof(target->part_path),
			true) != 0) {
		return -1;
	}

	if (content_cache_directories(
			normal,
			sizeof(normal),
			oversize,
			sizeof(oversize)) != 0) {
		return -1;
	}

	directory = target->oversize ? oversize : normal;

	/*
	 * Remove interrupted extractions before doing anything else.
	 */
	content_cache_clean_parts(directory);

	/*
	 * Cache hit: no eviction and no metadata update.
	 */
	if (stat(target->path, &st) == 0 &&
		S_ISREG(st.st_mode) &&
		(uint64_t)st.st_size == info->uncompressed_size) {
		target->hit = true;
		return 0;
	}

	if (target->oversize) {
		return content_cache_prepare_oversize(
			directory,
			info->uncompressed_size);
	}

	return content_cache_prepare_normal(
		directory,
		info->uncompressed_size);
}

int content_cache_commit(struct content_cache_target *target)
{
	if (rename(target->part_path, target->path) != 0) {
		PA_ERROR(
			"Couldn't commit cached content %s: %s\n",
			target->path,
			strerror(errno)
		);
		return -1;
	}

	content_cache_fix_fifo_time(target);

	return 0;
}

void content_cache_abort(struct content_cache_target *target)
{
	if (target->part_path[0])
		remove(target->part_path);
}
