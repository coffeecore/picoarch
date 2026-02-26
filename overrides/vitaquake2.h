#include "overrides.h"

static const struct core_override_option vitaquake2_core_option_overrides[] = {
	{
		.key = "vitaquakeii_resolution",
		.blocked = true,
	},
	{
		.key = "vitaquakeii_gamma",
		.desc = "Gamma Level",
		.info = "Set the overall gamma offset of the rendered image. Requires restart.",
	},
	{
		.key = "vitaquakeii_sw_dithered_filtering",
		.desc = "Dithering",
	},
	{
		.key = "vitaquakeii_xhair",
		.desc = "Crosshair",
	},
	{
		.key = "vitaquakeii_cin_force43",
		.desc = "4:3 Cinematics",
		.info = "Show all videos (introductions, cutscenes) at the correct aspect ratio of 4:3. If disabled, videos will be stretched.",
	},
	{
		.key = "vitaquakeii_cdaudio_enabled",
		.info = "Enable playback of original CD audio tracks at the expense of increased performance requirements.",
	},
	{
		.key = "vitaquakeii_aimfix",
		.info = "Weapons target the exact centre of the aiming crosshair. This changes the slightly inaccurate original shooting.",
	},
	{
		.key = "vitaquakeii_invert_y_axis",
		.blocked = true,
	},
	{ NULL }
};

/* DrUm78 build */
me_bind_action vitaquake2_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "LOOK     ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "CROUCH   ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "FIRE     ",  1 << RETRO_DEVICE_ID_JOYPAD_X },
	{ "STRAFE   ",  1 << RETRO_DEVICE_ID_JOYPAD_Y },
	{ "MENU     ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ "OBJECTIVE",  1 << RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ "NEXT WPN ",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "JUMP     ",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "NEXT INV ",  1 << RETRO_DEVICE_ID_JOYPAD_L2 },
	{ "USE INV  ",  1 << RETRO_DEVICE_ID_JOYPAD_R2 },
	{ NULL,       0 }
};

me_bind_action vitaquake2_emu_actions[] =
{
	{ "Toggle FPS/CPU%  ", 1 << EACTION_TOGGLE_HUD },
	{ "Toggle FF        ", 1 << EACTION_TOGGLE_FF },
	{ "Take Screenshot  ", 1 << EACTION_SCREENSHOT },
	{ NULL,                0 }
};

#define vitaquake2_overrides {                           \
	.core_name = "vitaquake2",                             \
	.actions = vitaquake2_ctrl_actions,                    \
	.action_size = array_size(vitaquake2_ctrl_actions),    \
	.emu_actions = vitaquake2_emu_actions,                 \
	.emu_action_size = array_size(vitaquake2_emu_actions), \
	.options = vitaquake2_core_option_overrides,           \
	.needs_reopen = 1                                      \
}
