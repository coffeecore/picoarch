#include "overrides.h"

static const struct core_override_option prboom_core_option_overrides[] = {
	{
		.key = "prboom-resolution",
		.blocked = true,
	},
	{
		.key = "prboom-mouse_on",
		.blocked = true,
	},
	{
		.key = "prboom-find_recursive_on",
		.desc = "Scan for IWADs",
		.info = "Scans parent folders for IWADs. NOTE: You need to disable this if you want to run SIGIL."
	},
	{
		.key = "prboom-rumble",
		.blocked = true,
	},
	{
		.key = "prboom-analog_deadzone",
		.blocked = true,
	},
	{ NULL }
};

#ifdef FUNKEY_S
/* DrUm78 build */
me_bind_action prboom_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "USE      ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "TGL RUN  ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "FIRE     ",  1 << RETRO_DEVICE_ID_JOYPAD_X },
	{ "STRAFE   ",  1 << RETRO_DEVICE_ID_JOYPAD_Y },
	{ "MENU     ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ "MAP      ",  1 << RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ "PREV WPN ",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "NEXT WPN ",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "RUN      ",  1 << RETRO_DEVICE_ID_JOYPAD_L3 },
	{ NULL,       0 }
};
#else
me_bind_action prboom_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "USE      ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "STRAFE   ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "FIRE     ",  1 << RETRO_DEVICE_ID_JOYPAD_X },
	{ "RUN      ",  1 << RETRO_DEVICE_ID_JOYPAD_Y },
	{ "MENU     ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ "MAP      ",  1 << RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ "STRAFE L ",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "STRAFE R ",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "PREV WPN ",  1 << RETRO_DEVICE_ID_JOYPAD_L2 },
	{ "NEXT WPN ",  1 << RETRO_DEVICE_ID_JOYPAD_R2 },
	{ "TGL RUN  ",  1 << RETRO_DEVICE_ID_JOYPAD_L3 },
	{ "180 TURN ",  1 << RETRO_DEVICE_ID_JOYPAD_R3 },
	{ NULL,       0 }
};

#endif

#define prboom_overrides {                          \
	.core_name = "prboom",                            \
	.actions = prboom_ctrl_actions,                   \
	.action_size = array_size(prboom_ctrl_actions),   \
	.options = prboom_core_option_overrides,          \
	.needs_reopen = 1                                 \
}
