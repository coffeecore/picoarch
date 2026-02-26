#include "overrides.h"

static const struct core_override_option mednafen_wswan_core_option_overrides[] = {
	{
		.key = "wswan_rotate_display",
		.info = "Choose the orientation of the console screen. 'Manual' enables rotation via the controller.",
	},
	{
		.key = "wswan_rotate_keymap",
		.desc = "Rotate Buttons",
	},
	{
		.key = "wswan_gfx_colors",
		.desc = "Color Depth",
		.info = "24-bit is slower and not available on all platforms. Restart required.",
	},
	{
		.key = "wswan_frameskip",
		.info = "Skip frames to avoid audio crackling. Improves performance at the expense of visual smoothness.",
	},
	{
		.key = "wswan_frameskip_threshold",
		.desc = "FS Threshold (%%)",
		.info = "When 'Frameskip' is set to 'Threshold', sets how low the audio buffer can get before frames will be skipped.",
	},
	{
		.key = "wswan_60hz_mode",
		.info = "Update the display at 60Hz instead of the native 75Hz WonderSwan refresh rate by dropping every fifth frame. Reduces video smoothness, but avoids screen tearing on 60Hz screens.",
	},
	{
		.key = "wswan_sound_sample_rate",
		.desc = "Sample Rate",
	},
	{ NULL }
};

me_bind_action mednafen_wswan_ctrl_actions[] =
{
	{ "X UP     ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "X DOWN   ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "X LEFT   ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "X RIGHT  ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "Y UP     ",  1 << RETRO_DEVICE_ID_JOYPAD_R2 },
	{ "Y DOWN   ",  1 << RETRO_DEVICE_ID_JOYPAD_L2 },
	{ "Y LEFT   ",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "Y RIGHT  ",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "A        ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "B        ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "START    ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ "ROTATE   ",  1 << RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ NULL,       0 }
};

const struct core_override_fast_forward mednafen_wswan_fast_forward = {
	.type_key = "wswan_frameskip",
	.type_value = "auto",
};

#define mednafen_wswan_overrides {                              \
	.core_name = "mednafen_wswan",                                \
	.fast_forward = &mednafen_wswan_fast_forward,                 \
	.actions = mednafen_wswan_ctrl_actions,                       \
	.action_size = array_size(mednafen_wswan_ctrl_actions),       \
	.options = mednafen_wswan_core_option_overrides               \
}
