#include "overrides.h"

static const struct core_override_option scummvm_core_option_overrides[] = {
	{
		.key = "scummvm_gamepad_cursor_speed",
		.desc = "Cursor speed",
		.info = "Sets the mouse cursor speed multiplier. '1.0' is optimised for '320x200' or '320x240' games. '2.0' is recommended for higher resolution games.",
	},
	{
		.key = "scummvm_gamepad_cursor_acceleration_time",
		.desc = "Cursor acceleration",
	},
	{
		.key = "scummvm_analog_response",
		.blocked = true,
	},
	{
		.key = "scummvm_analog_deadzone",
		.blocked = true,
	},
	{
		.key = "scummvm_mouse_speed",
		.blocked = true,
	},
	{
		.key = "scummvm_speed_hack",
		.desc = "Speed Hack",
		.info = "Enables a safe speed hack that significantly reduces CPU requirements by allowing subtle timing inaccuracies. Requires restart.",
		.default_value = "enabled",
	},
	{ NULL }
};

me_bind_action scummvm_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "LEFT BTN ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "RIGHT BTN",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "ESC      ",  1 << RETRO_DEVICE_ID_JOYPAD_X },
	{ ".        ",  1 << RETRO_DEVICE_ID_JOYPAD_Y },
	{ "MENU     ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ "KEYBOARD ",  1 << RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ "ENTER    ",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "5        ",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "BACKSPACE",  1 << RETRO_DEVICE_ID_JOYPAD_L2 },
	{ "SLOW CUR ",  1 << RETRO_DEVICE_ID_JOYPAD_R2 },
	{ "F10      ",  1 << RETRO_DEVICE_ID_JOYPAD_L3 },
	{ "0        ",  1 << RETRO_DEVICE_ID_JOYPAD_R3 },
	{ NULL,       0 }
};

me_bind_action scummvm_emu_actions[] =
{
	{ "Toggle FPS/CPU%  ", 1 << EACTION_TOGGLE_HUD },
	{ "Toggle FF        ", 1 << EACTION_TOGGLE_FF },
	{ "Take Screenshot  ", 1 << EACTION_SCREENSHOT },
	{ NULL,                0 }
};

#define scummvm_overrides {                           \
	.core_name = "scummvm",                             \
	.actions = scummvm_ctrl_actions,                    \
	.action_size = array_size(scummvm_ctrl_actions),    \
	.emu_actions = scummvm_emu_actions,                 \
	.emu_action_size = array_size(scummvm_emu_actions), \
	.options = scummvm_core_option_overrides,           \
	.needs_reopen = 1                                   \
}
