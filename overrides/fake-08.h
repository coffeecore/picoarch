#include "overrides.h"

me_bind_action fake08_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "O        ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "X        ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "START    ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ NULL,       0 }
};

#define fake08_overrides {                              \
	.core_name = "fake-08",                               \
	.actions = fake08_ctrl_actions,                       \
	.action_size = array_size(fake08_ctrl_actions),       \
	.defer_frames = 3,                                    \
}
