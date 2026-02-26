#include "main.h"
#include "overrides.h"

static const struct core_override_option fbalpha2012_core_option_overrides[] = {
	{
		.key = "fbneo-frameskip-type",
		.info = "Skip frames to avoid audio crackling. Improves performance at the expense of visual smoothness.",
		.default_value = "auto",
		.retro_var_value = "Frameskip ; disabled|auto|threshold"
	},
	{
		.key = "fbneo-threshold",
		.info = "When 'Frameskip' is set to 'Threshold', sets how low the audio buffer can get before frames will be skipped.",
		.retro_var_value = "FS Threshold (%%); 30|40|50|60",
	},
	{
		.key = "fbneo-interval",
		.info = "The maximum number of frames that can be skipped before a new frame is rendered.",
		.default_value = "2",
		.retro_var_value = "FS Interval; 1|2|3|4|5|6|7|8|9",
	},
	{
		.key = "fbneo-allow-depth-32",
		.info = "Use 32-bits color depth when available.",
		.retro_var_value = "32-bit color; disabled|enabled",
	},
	{
		.key = "fbneo-samplerate",
		.info = "Change the audio sample rate. Requires restart.",
		.retro_var_value = "Sample Rate; 48000|44100|22050|11025"
	},
	{
		.key = "fbneo-sample-interpolation",
		.retro_var_value = "Sample Interp.; 4-point|2-point|disabled"
	},
	{
		.key = "fbneo-fm-interpolation",
		.retro_var_value = "FM Interpolation; 4-point|disabled"
	},
	{
		.key = "fbneo-cyclone",
		.info = "Requires restart. Changes savestate format. Use at your own risk.",
		.retro_var_value = "Cyclone; disabled|enabled"
	},
	{
		.key = "fbneo-neogeo-mode",
		.retro_var_value = "Neo Geo Mode; MVS|AES|UNIBIOS|DIPSWITCH"
	},
	{ NULL }
};

me_bind_action fbalpha2012_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "BUTTON 1 ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "BUTTON 2 ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "BUTTON 3 ",  1 << RETRO_DEVICE_ID_JOYPAD_Y },
	{ "BUTTON 4 ",  1 << RETRO_DEVICE_ID_JOYPAD_X },
	{ "BUTTON 5 ",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "BUTTON 6 ",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "START    ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ "COIN     ",  1 << RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ NULL,       0 }
};

const struct core_override_fast_forward fbalpha2012_fast_forward = {
	.type_key = "fbneo-frameskip-type",
	.type_value = "auto",
	.interval_key = "fbneo-interval"
};

#define fbalpha2012_overrides {                           \
	.core_name = "fbalpha2012",                             \
	.fast_forward = &fbalpha2012_fast_forward,              \
	.actions = fbalpha2012_ctrl_actions,                    \
	.action_size = array_size(fbalpha2012_ctrl_actions),    \
	.options = fbalpha2012_core_option_overrides,           \
	.needs_reopen = 1,                                      \
}
