#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "libretro.h"

void video_set_pixel_format(enum retro_pixel_format format);
void video_set_geometry(struct retro_game_geometry *geometry);
void video_process(const void *data, unsigned width, unsigned height, size_t pitch);
void video_deinit(void);

#endif
