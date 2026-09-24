#ifndef CONTENT_CACHE_H
#define CONTENT_CACHE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <dirent.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <time.h>

#include "main.h"
#include "unzip.h"

#define CONTENT_RAM_THRESHOLD (8ULL * 1024 * 1024)
#define CONTENT_CACHE_MAX     (128ULL * 1024 * 1024)
#define CONTENT_SD_RESERVE    (256ULL * 1024 * 1024)
#define CONTENT_TMP_RESERVE   (1ULL * 1024 * 1024)

struct content_cache_target {
	char path[MAX_PATH];
	char part_path[MAX_PATH];
	bool hit;
	bool oversize;
};

bool content_cache_enabled(void);
bool content_cache_use_ram(uint32_t size);

int content_cache_prepare(
	const struct unzip_info *info,
	struct content_cache_target *target
);

int content_cache_commit(struct content_cache_target *target);
void content_cache_abort(struct content_cache_target *target);

#endif
