#include "overrides.h"

static const struct core_override_option mednafen_ngp_core_option_overrides[] = {
	{ NULL }
};

me_bind_action mednafen_ngp_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "A        ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "B        ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "OPTION   ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ NULL,       0 }
};

#define mednafen_ngp_overrides {                              \
	.core_name = "mednafen_ngp",                                \
	.actions = mednafen_ngp_ctrl_actions,                       \
	.action_size = array_size(mednafen_ngp_ctrl_actions),       \
	.options = mednafen_ngp_core_option_overrides               \
}
