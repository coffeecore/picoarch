# picoarch - a libretro frontend designed for small screens and low power

picoarch runs libretro cores (emulators) for various systems with low overhead and UI designed for small screen, low-powered devices like the Trimui Model S (PowKiddy A66) and FunKey S.

It supports: 

- **Arcade** (mame2000, fbalpha2012)
- **Colecovision** (blueMSX, smsplus)
- **Game Boy / Game Boy Color** (gambatte)
- **Game Boy Advance** (gpsp)
- **Game Gear** (picodrive, smsplus)
- **Genesis** (picodrive)
- **MS-DOS** (dosbox-pure)
- **MSX** (fMSX, blueMSX)
- **NES** (quicknes, fceumm)
- **PCE / TurboGrafx-16** (beetle-pce-fast)
- **PICO-8** (fake-08)
- **PlayStation** (pcsx_rearmed)
- **Pokémon Mini** (pokemini)
- **Sega Master System** (picodrive, smsplus)
- **Super NES** (snes9x2002, snes9x2005, snes9x2005_plus, snes9x2010)
- more to come

picoarch can also play game music (gme).

All emulators have:

- FunKey menu
- fast-forward
- soft scaling options
- START+button combo keybindings
- per-game config
- screenshots

Most have:
- FunKey Instant Play (close / reopen device, requires core with save states)
- autosave and resume (requires core with save states)
- cheat support
- IPS/BPS softpatching
- auto-frameskip for smooth audio

## Install

Mount USB and copy .opk files into Emulators directory. Each core has its own .opk.

picoarch.opk is a standalone app with its own file browser that can be used to run any supported core, and can be used instead of individual core .opk files.

## Notes / extra features

### BIOS

Some emulators require bios files. bios files are placed into `/mnt/FunKey/.picoarch/system`. This directory is created after first launch.

The libretro documentation specifies which bios is required for each core. For example, needed fMSX bios files are listed here: <https://docs.libretro.com/library/fmsx/>

### Cheats

Cheats use RetroArch .cht file format. Many cheat files are here <https://github.com/libretro/libretro-database/tree/master/cht>

Cheat file name needs to match ROM name, and go underneath save directory. For example, `/mnt/FunKey/.picoarch-gambatte/cheats/Super Mario Land (World).cht`. When a cheat file is detected, a "cheats" menu item will appear in advanced menu. Not all cheats work with all cores, may want to clean up files to just the cheats you want.

### IPS / BPS soft-patching

Many cores can apply patches when loading. For example, loading `/roms/game.gba` will apply patches named `/roms/game.ips`, `/roms/game.ips1`, `/roms/game.IPS2`, `/roms/game.bps`, etc. Patching is temporary, original files are unmodified. Patches are loaded in case-insensitive alphabetical order. Note that `.ips12` loads before `.ips2`, but after `.ips02`.
