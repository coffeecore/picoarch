#include "main.h"
#include "overrides.h"

static const struct core_override_option dosbox_pure_core_option_overrides[] = {
	{
		.key = "dosbox_pure_advanced",
		.desc = "Show Advanced",
	},
	{
		.key = "dosbox_pure_force60fps",
		.desc = "Force 60FPS",
	},
	{
		.key = "dosbox_pure_perfstats",
		.desc = "Show Performance",
		.options = {
			{ "none",     "Disabled" },
			{ "simple",   "Simple" },
			{ "detailed", "Detailed" },
		},
	},
	{
		.key = "dosbox_pure_savestate",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_conf",
		.desc = "Load dosbox.conf",
		.info = "Load legacy dosbox.conf files. 'inside' loads files in the loaded zip or folder. 'outside' loads files next to the zip or folder.",
		.options = {
			{ "false", "Off" },
			{ "inside", "Inside" },
			{ "outside", "Outside" }
		}
	},
	{
		.key = "dosbox_pure_menu_time",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_latency",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_auto_target",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_bind_unused",
		.desc = "Bind unused buttons",
		.info = "Bind all unused controller buttons to keyboard keys.",
	},
	{
		.key = "dosbox_pure_on_screen_keyboard",
		.desc = "Enable OSK",
	},
	{
		.key = "dosbox_pure_mouse_wheel",
		.default_value = "none",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_mouse_speed_factor",
		.desc = "Mouse Sensitivity",
	},
	{
		.key = "dosbox_pure_mouse_speed_factor_x",
		.desc = "Horz. Sensitivity",
	},
	{
		.key = "dosbox_pure_mouse_input",
		.blocked = true,
		.default_value = "false",
	},
	{
		.key = "dosbox_pure_auto_mapping",
		.desc = "Auto Button Map",
		.info = "Automatically apply a gamepad control mapping scheme when it detects a game, provided by the Keyb2Joypad Project (by Jemy Murphy and bigjim)",
		.options = {
			{ "true", "On" },
			{ "notify", "On (notify)" },
			{ "false", "Off" }
		}
	},
	{
		.key = "dosbox_pure_keyboard_layout",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_menu_transparency",
		.desc = "Transparency",
	},
	{
		.key = "dosbox_pure_joystick_analog_deadzone",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_joystick_timed",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_cycles",
		.desc = "Performance",
		.options = {
			{ "auto",    "AUTO" },
			{ "max",     "MAX" },
			{ "315",     "8086/8088" },
			{ "1320",    "286 slow" },
			{ "2750",    "286 fast" },
			{ "4720",    "386" },
			{ "7800",    "386DX" },
			{ "13400",   "486DX" },
			{ "26800",   "486DX2" },
			{ "77000",   "Pentium" },
			{ "200000",  "PII" },
			{ "500000",  "PIII" },
			{ "1000000", "Athlon" },
		}
	},
	{
		.key = "dosbox_pure_cycles_scale",
		.desc = "Perf. Scale",
	},
	{
		.key = "dosbox_pure_cycle_limit",
		.desc = "CPU Limit",
		.info = "When emulating DOS as fast as possible, how much time per frame should be used by the emulation.",
		.default_value = "0.8",
	},
	{
		.key = "dosbox_pure_machine",
		.desc = "Graphics",
		.info = "The type of graphics chip that DOSBox will emulate. Requires restart.",
		.options = {
			{ "svga",     "SVGA" },
			{ "vga",      "VGA" },
			{ "ega",      "EGA" },
			{ "cga",      "CGA" },
			{ "tandy",    "Tandy" },
			{ "hercules", "Hercules" },
			{ "pcjr",     "PCjr" },
		}
	},
	{
		.key = "dosbox_pure_cga",
		.desc = "CGA Mode",
		.options = {
			{ "early_auto", "Early, comp. auto" },
			{ "early_on",   "Early, comp. on" },
			{ "early_off",  "Early, comp. off" },
			{ "late_auto", "Late, comp. auto" },
			{ "late_on",   "Late, comp. on" },
			{ "late_off",  "Late, comp. off" },
		}
	},
	{
		.key = "dosbox_pure_hercules",
		.desc = "Hercules Color",
		.options = {
			{ "white", "white" },
			{ "amber", "amber" },
			{ "green", "green" },
		}
	},
	{
		.key = "dosbox_pure_svga",
		.desc = "SVGA Mode",
		.info = "The SVGA variation that is being emulated. Try changing this if you encounter graphical glitches. Requires restart.",
		.options = {
			{ "svga_s3",       "Trio64" },
			{ "vesa_nolfb",    "Trio64 (FB hack)" },
			{ "vesa_oldvbe",   "Trio64 (VESA 1.3)" },
			{ "svga_et3000",   "ET3000" },
			{ "svga_et4000",   "ET4000" },
			{ "svga_paradise", "PVGA1A" },
		}
	},
	{
		.key = "dosbox_pure_svgamem",
		.desc = "SVGA Memory",
		.info = "The amount of memory available to the emulated SVGA card. Requires restart.",
		.options = {
			{ "0",  "512KB" },
			{ "1", "1MB" },
			{ "2", "2MB" },
			{ "3", "3MB" },
			{ "4", "4MB" },
			{ "8", "8MB" },
		}
	},
	{
		.key = "dosbox_pure_voodoo",
		.blocked = true,
		.default_value = "off",
	},
	{
		.key = "dosbox_pure_voodoo_perf",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_aspect_correction",
		.desc = "Correct Aspect",
		.options = {
			{ "false", "Off" },
			{ "true", "On" },
		},
	},
	{
		.key = "dosbox_pure_memory_size",
		.desc = "Memory Size",
		.info = "The amount of (high) memory that the emulated machine has. You can also disable extended memory (EMS/XMS). Requires restart.",
		.options = {
			{ "none", "no EMS/XMS" },
			{ "4",  "4 MB" },
			{ "8",  "8 MB" },
			{ "16", "16 MB" },
			{ "24", "24 MB" },
			{ "32", "32 MB" },
			{ "48", "48 MB" },
			{ "64", "64 MB" },
			{ "96", "96 MB" },
			{ "128", "128 MB" },
			{ "224", "224 MB" },
		}
	},
	{
		.key = "dosbox_pure_cpu_type",
		.desc = "CPU Type",
		.info = "Emulated CPU type. Auto is the fastest choice. Requires restart.",
		.options = {
			{ "auto", "Auto" },
			{ "386", "386" },
			{ "386_slow", "386 (slow)" },
			{ "386_prefetch", "386 (prefetch)" },
			{ "486_slow", "486 (slow)" },
			{ "pentium_slow", "Pentium (slow)" },
		}
	},
	{
		.key = "dosbox_pure_cpu_core",
		.desc = "CPU Core",
		.info = "Emulation method (DOSBox CPU core) used. Dynamic uses a dynarec implementation. Simple uses an interpreter optimized for old games.",
		.options = {
			{ "auto", "Auto" },
			{ "dynamic", "Dynamic" },
			{ "normal", "Normal" },
			{ "simple", "Simple" },
		},
		.default_value = "dynamic",
	},
	{
		.key = "dosbox_pure_bootos_ramdisk",
		.desc = "Ignore Disk Changes",
		.info = "When running an installed operating system, modifications to the C: drive will not be saved permanently.",
		.options = {
			{ "false", "Off" },
			{ "true", "On" },
		},
	},
	{
		.key = "dosbox_pure_bootos_dfreespace",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_bootos_forcenormal",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_audiorate",
		.desc = "Sample rate",
		.info = "Sets the audio sample rate. Requires restart.",
	},
	{
		.key = "dosbox_pure_sblaster_conf",
		.desc = "SoundBlaster Conf",
		.options = {
			{ "A220 I7 D1 H5",  "A220 I7 D1 H5" },
			{ "A220 I5 D1 H5",  "A220 I5 D1 H5" },
			{ "A240 I7 D1 H5",  "A240 I7 D1 H5" },
			{ "A240 I7 D3 H7",  "A240 I7 D3 H7" },
			{ "A240 I2 D3 H7",  "A240 I2 D3 H7" },
			{ "A240 I5 D3 H5",  "A240 I5 D3 H5" },
			{ "A240 I5 D1 H5",  "A240 I5 D1 H5" },
			{ "A240 I10 D3 H7", "A240 I10 D3 H7" },
			{ "A280 I10 D0 H6", "A280 I10 D0 H6" },
			{ "A280 I5 D1 H5",  "A280 I5 D1 H5" },
		},
	},
	{
		.key = "dosbox_pure_midi",
		.blocked = true,
	},
	{
		.key = "dosbox_pure_sblaster_type",
		.desc = "SoundBlaster Type",
		.options = {
			{ "sb16", "SB16" },
			{ "sbpro2", "SB Pro 2" },
			{ "sbpro1", "SB Pro" },
			{ "sb2", "SB 2.0" },
			{ "sb1", "SB 1.0" },
			{ "gb", "GameBlaster" },
			{ "none", "none" },
		},
	},
	{
		.key = "dosbox_pure_sblaster_adlib_mode",
		.desc = "SB Adlib Mode",
		.options = {
			{ "auto",     "Auto" },
			{ "cms",      "CMS" },
			{ "opl2",     "OPL-2" },
			{ "dualopl2", "Dual OPL-2" },
			{ "opl3",     "OPL-3" },
			{ "opl3gold", "OPL-3 Gold" },
			{ "none",     "Disabled" },
		},
	},
	{
		.key = "dosbox_pure_sblaster_adlib_emu",
		.desc = "Adlib Provider",
		.options = {
			{ "default", "Default" },
			{ "nuked", "Nuked OPL3" },
		},
	},
	{
		.key = "dosbox_pure_gus",
		.desc = "Gravis Ultrasound",
		.info = "Enable Gravis Ultrasound emulation. Requires restart.",
		.options = {
			{ "false", "Off" },
			{ "true", "On" },
		},
	},
	{ NULL }
};

me_bind_action dosbox_pure_ctrl_actions[] =
{
	{ "UP       ",  1 << RETRO_DEVICE_ID_JOYPAD_UP},
	{ "DOWN     ",  1 << RETRO_DEVICE_ID_JOYPAD_DOWN },
	{ "LEFT     ",  1 << RETRO_DEVICE_ID_JOYPAD_LEFT },
	{ "RIGHT    ",  1 << RETRO_DEVICE_ID_JOYPAD_RIGHT },
	{ "A BUTTON ",  1 << RETRO_DEVICE_ID_JOYPAD_A },
	{ "B BUTTON ",  1 << RETRO_DEVICE_ID_JOYPAD_B },
	{ "X BUTTON ",  1 << RETRO_DEVICE_ID_JOYPAD_X },
	{ "Y BUTTON ",  1 << RETRO_DEVICE_ID_JOYPAD_Y },
	{ "START    ",  1 << RETRO_DEVICE_ID_JOYPAD_START },
	{ "SELECT   ",  1 << RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ "L BUTTON ",  1 << RETRO_DEVICE_ID_JOYPAD_L },
	{ "R BUTTON ",  1 << RETRO_DEVICE_ID_JOYPAD_R },
	{ "L2 BUTTON",  1 << RETRO_DEVICE_ID_JOYPAD_L2 },
	{ "R2 BUTTON",  1 << RETRO_DEVICE_ID_JOYPAD_R2 },
	{ "SHOW OSK ",  1 << RETRO_DEVICE_ID_JOYPAD_L3 },
	{ NULL,       0 }
};

#define dosbox_pure_overrides {                           \
	.core_name = "dosbox-pure",                             \
	.actions = dosbox_pure_ctrl_actions,                    \
	.action_size = array_size(dosbox_pure_ctrl_actions),    \
	.options = dosbox_pure_core_option_overrides,           \
	.needs_reopen = 1,                                      \
	.prevent_resume = 1,                                    \
}
