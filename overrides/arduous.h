#include "overrides.h"

me_bind_action arduous_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "A        ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "B        ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ NULL,       0 }
};

#define arduous_overrides {                              \
	.core_name = "arduous",                                \
	.actions = arduous_ctrl_actions,                       \
	.action_size = array_size(arduous_ctrl_actions),       \
	.defer_frames = 3,                                     \
}
