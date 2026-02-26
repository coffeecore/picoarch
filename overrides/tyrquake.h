#include "overrides.h"

static const struct core_override_option tyrquake_core_option_overrides[] = {
	{
		.key = "tyrquake_resolution",
		.blocked = true,
	},
	{
		.key = "tyrquake_colored_lighting",
		.desc = "Colored Lighting",
	},
	{
		.key = "tyrquake_rumble",
		.blocked = true,
	},
	{
		.key = "tyrquake_invert_y_axis",
		.blocked = true,
	},
	{
		.key = "tyrquake_analog_deadzone",
		.blocked = true,
	},
	{ NULL }
};

#ifdef FUNKEY_S
/* DrUm78 build */
me_bind_action tyrquake_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "LOOK     ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "JUMP     ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "FIRE     ",  1 << RETRO_DEVICE_ID_JOYPAD_X },
	{ "STRAFE   ",  1 << RETRO_DEVICE_ID_JOYPAD_Y },
	{ "MENU     ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ "PREV WPN ",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "NEXT WPN ",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "LOOK UP  ",  1 << RETRO_DEVICE_ID_JOYPAD_L2 },
	{ "LOOK DOWN",  1 << RETRO_DEVICE_ID_JOYPAD_R2 },
	{ "MOVE DOWN",  1 << RETRO_DEVICE_ID_JOYPAD_L3 },
	{ "SWIM UP  ",  1 << RETRO_DEVICE_ID_JOYPAD_R3 },
	{ NULL,       0 }
};

#else
me_bind_action tyrquake_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "CHG WPN  ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "JUMP     ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "LOOK     ",  1 << RETRO_DEVICE_ID_JOYPAD_X },
	{ "FIRE     ",  1 << RETRO_DEVICE_ID_JOYPAD_Y },
	{ "MENU     ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ "TGL RUN  ",  1 << RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ "STRAFE L ",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "STRAFE R ",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "LOOK UP  ",  1 << RETRO_DEVICE_ID_JOYPAD_L2 },
	{ "LOOK DOWN",  1 << RETRO_DEVICE_ID_JOYPAD_R2 },
	{ "MOVE DOWN",  1 << RETRO_DEVICE_ID_JOYPAD_L3 },
	{ "SWIM UP  ",  1 << RETRO_DEVICE_ID_JOYPAD_R3 },
	{ NULL,       0 }
};

#endif

me_bind_action tyrquake_emu_actions[] =
{
	{ "Toggle FPS/CPU%  ", 1 << EACTION_TOGGLE_HUD },
	{ "Toggle FF        ", 1 << EACTION_TOGGLE_FF },
	{ "Take Screenshot  ", 1 << EACTION_SCREENSHOT },
	{ NULL,                0 }
};

#define tyrquake_overrides {                           \
	.core_name = "tyrquake",                             \
	.actions = tyrquake_ctrl_actions,                    \
	.action_size = array_size(tyrquake_ctrl_actions),    \
	.emu_actions = tyrquake_emu_actions,                 \
	.emu_action_size = array_size(tyrquake_emu_actions), \
	.options = tyrquake_core_option_overrides,           \
	.needs_reopen = 1                                    \
}
