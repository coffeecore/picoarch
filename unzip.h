#ifndef __UNZIP_H_
#define __UNZIP_H_

#include <stdint.h>
#include <stdio.h>

#include "main.h"

struct unzip_info {
	uint32_t crc32;
	uint32_t compressed_size;
	uint32_t uncompressed_size;
	char filename[MAX_PATH];
};

typedef void (*unzip_progress_cb)(
	uint64_t current,
	uint64_t total,
	void *userdata
);

int unzip_get_info(
	FILE *zip,
	const char **extensions,
	struct unzip_info *info
);

int unzip_tmp_progress(
	FILE *zip,
	const char **extensions,
	char *filename,
	size_t len,
	unzip_progress_cb progress,
	void *userdata
);

int unzip_tmp(
	FILE *zip,
	const char **extensions,
	char *filename,
	size_t len
);

int unzip_progress(
	FILE *zip,
	const char **extensions,
	FILE *dest,
	unzip_progress_cb progress,
	void *userdata
);

int unzip(
	FILE *zip,
	const char **extensions,
	FILE *dest
);

#endif
