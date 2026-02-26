#include "overrides.h"

static const struct core_override_option pokemini_core_option_overrides[] = {
	{
		.key = "pokemini_video_scale",
		.desc = "Video Scale",
		.info = "Set internal video scale factor. Increasing the scale factor improves the appearance of the internal 'Dot Matrix' LCD filter. Requires restart."
	},
	{
		.key = "pokemini_60hz_mode",
		.info = "Update the display at 60Hz instead of the native 72Hz Pokemon Mini's refresh rate. Reduces video smoothness, but avoids screen tearing.",
	},
	{
		.key = "pokemini_lcdfilter",
		.info = "Select internal screen filter. 'Dot Matrix' gives an LCD effect that mimics real hardware. Filters are disabled when 'Video Scale' is set to '1x'."
	},
	{
		.key = "pokemini_lcdmode",
		.info = "Specify the greyscale 'color' character of the LCD. 'Analog' mimics real hardware. '2 Shades' removes ghosting, but causes flickering.",
	},
	{
		.key = "pokemini_lowpass_range",
		.desc = "Filter level (%%)",
		.info = "Specifies the cut-off frequency of the low pass audio filter. A higher value increases the perceived 'strength' of the filter.",
	},
	{
		.key = "pokemini_rumble_lv",
		.blocked = true,
	},
	{ NULL }
};

me_bind_action pokemini_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "A        ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "B        ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "C        ",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "TURBO A  ",  1 << RETRO_DEVICE_ID_JOYPAD_X },
	{ "SHAKE    ",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "POWER    ",  1 << RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ NULL,       0 }
};

#define pokemini_overrides {                              \
	.core_name = "pokemini",                                \
	.actions = pokemini_ctrl_actions,                       \
	.action_size = array_size(pokemini_ctrl_actions),       \
	.options = pokemini_core_option_overrides,              \
	.defer_frames = 7,                                      \
}
