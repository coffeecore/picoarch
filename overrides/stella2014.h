#include "overrides.h"

static const struct core_override_option stella2014_core_option_overrides[] = {
	{
		.key = "stella2014_color_depth",
		.desc = "Color Depth",
		.info = "Specifies number of colors to display on-screen. 24-bit may increase performance overheads on some platforms. Requires restart."
	},
	{
		.key = "stella2014_mix_frames",
		.info = "Simulates CRT phosphor ghosting effects. May be used to alleviate screen flicker when games toggle sprites on alternate frames.",
	},
	{
		.key = "stella2014_paddle_digital_sensitivity",
		.desc = "Paddle Sensitivity",
	},
	{
		.key = "stella2014_paddle_analog_sensitivity",
		.blocked = true,
	},
	{
		.key = "stella2014_paddle_analog_response",
		.blocked = true,
	},
	{
		.key = "stella2014_paddle_analog_deadzone",
		.blocked = true,
	},
	{
		.key = "stella2014_stelladaptor_analog_sensitivity",
		.blocked = true,
	},
	{
		.key = "stella2014_stelladaptor_analog_center",
		.blocked = true,
	},
	{ NULL }
};

me_bind_action stella2014_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "L DIFF A ",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "R DIFF A ",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "L DIFF B ",  1 << RETRO_DEVICE_ID_JOYPAD_L2 },
	{ "R DIFF B ",  1 << RETRO_DEVICE_ID_JOYPAD_R2 },
	{ "FIRE     ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "PDL FIRE ",  1 << RETRO_DEVICE_ID_JOYPAD_Y },
	{ "SELECT   ",  1 << RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ "RESET    ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ NULL,       0 }
};

#define stella2014_overrides {                              \
	.core_name = "stella2014",                                \
	.actions = stella2014_ctrl_actions,                       \
	.action_size = array_size(stella2014_ctrl_actions),       \
	.options = stella2014_core_option_overrides               \
}
