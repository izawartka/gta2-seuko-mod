# gta2-seuko-mod

An ultimate mod menu (aka. trainer) for Grand Theft Auto 2.

[Video showcase](https://www.youtube.com/watch?v=5gCCe0UtBl4)

[![gta2-seuko-mod](https://maseuko.pl/soft/gta2-seuko-mod/seuko-mod.png)](https://maseuko.pl/soft/gta2-seuko-mod/seuko-mod.png)

## Features

- Player control (money, health, armor, wanted level, wanted level lock, appearance, invulnerability, respect, multiplier, lives)
- Teleport
- Spawning vehicles (selectable model and remap)
- Current/last vehicle control (physics aka. invulnerability, remap, fixing damage, turning the engine off, exploding)
- Advanced game camera position control and 3D mode
- Obtaining weapons (selectable ammo count, including vehicle weapons)
- Obtaining all (handheld) weapons at one key press
- Weapon control (ammo count, infinite ammo, fast reload)
- Obtaining power-ups
- Quicksaving
- Custom quick actions on key press
- Saving and spawning saved vehicles
- Spawning objects (including power-ups and secret objects cut out from the game)
- Native cheats (each cheat can be force disabled, force enabled or unmodified to not interfere with cheat codes and registry flags)
- Configurable menu controls
- Shadows distance fix
- Disabling anti-aliasing

## Compatibility

- This mod is intended for [GTA2 PC version 11.44](https://gtamp.com/GTA2/gta2-installer.exe) only
- [Widescreenfix](https://github.com/ThirteenAG/WidescreenFixesPack/releases/tag/gta2) and [GTA2 radar](https://github.com/gennariarmando/gta2-radar) seem to work fine with this mod (the ASI version), but expect visual glitches and more bugs when using multiple mods together

## Installation
### Standalone version
This version is recommended if you want to use this mod alone without any other mods.

1. Make sure you have [VC++ Redistributables](https://www.techpowerup.com/download/visual-c-redistributable-runtime-package-all-in-one/) installed
2. Open the [latest release](https://github.com/izawartka/gta2-seuko-mod/releases/latest) page
3. Download `gta2-seuko-mod-<version>.zip` archive
4. Extract the contents of the downloaded archive to your GTA2 installation folder
5. Launch the game

### ASI version
This version is recommended if you want to use this mod together with other mods using the ASI Loader.

1. Make sure you have [VC++ Redistributables](https://www.techpowerup.com/download/visual-c-redistributable-runtime-package-all-in-one/) installed
2. Download and install [ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases/latest) (Win32 dinput.dll version) to your GTA2 installation folder
3. Open the [latest release](https://github.com/izawartka/gta2-seuko-mod/releases/latest) page
4. Download `gta2-seuko-mod-<version>-asi.zip` archive
5. Create `scripts` folder in your GTA2 installation folder if it doesn't exist yet
6. Extract the contents of the downloaded archive to the `scripts` folder
7. Launch the game

## Basic controls

- Open/close menu: `F11`
- Navigate menu: `[` (Left bracket) and `]` (Right bracket) keys
- Select menu item / toggle option: `\` (Backslash)

You can modify the controls in `Mod menu` > `Misc` > `Menu controls` section of the menu or reset them by holding `F11` for 5 seconds.

Customizable quick actions can be found in `Mod menu` > `Misc` > `Quick actions` section of the menu.

## Contact & Support

- [seuko mod project discord server](https://discord.gg/crwjQCXRCA) (for support and discussions regarding this mod)
- [h0x91b's discord server](https://discord.gg/7y2Y4Z3) (for general GTA2 modding discussions)

## Building

### Requirements:
- Git
- Microsoft Visual Studio 2022 or newer
- GTA2 PC version 11.44

### Build steps
1. Clone the repository

   ```git clone https://github.com/izawartka/gta2-seuko-mod --recurse-submodules```

2. Open `GTA2.props` in any text editor and set `GTA2_DIR` and `GTA2_EXE` variables to point to your GTA2 installation folder and gta2.exe file respectively.
3. If you plan to build an ASI version, you'll also need to set `GTA2_ASI_DIR` and `GTA2_ASI_FILENAME` variables and install ASI Loader to your GTA2 installation folder (see the [Installation](#asi-version) section).
4. Open `gta2-seuko-mod.sln` in Visual Studio.
5. Select one of the build configurations - `Debug`, `DebugAsi`, `Release`, `ReleaseAsi` or `ReleaseWithLogging`.
6. Build the solution.
7. After the build, output files will be automatically copied to your GTA2 installation folder and the game will be launched.

## Support

If you like this mod and want to support its development, you can donate via [PayPal](https://www.paypal.me/maseuko) :)

## Known issues
- Mission arrows are misaligned when using 3D camera mode and camera position is locked
- The game sometimes crashes on level restart
- Setting large camera height / very low camera zoom crashes the game
- The camera sometimes "jumps" one block higher when locking its position while in 3D mode
- Spawning a lot of explosions (i.e. while using Rocket Launcher with Instant reload cheat) may cause the game to crash
- "Unlock all levels" native cheat does not work

Please remember that this mod is an experimental project and may cause a lot of unexpected bugs, crashes etc.

## Credits

- Original GTA2 game by DMA Design / Rockstar Games (none of original game files are included in this project)
- Mod by [izawartka](https://github.com/izawartka)
- Research support by [h0x91b](https://github.com/h0x91b)

## References
- Own research using Ghidra, Cheat Engine and x64dbg
- [Official scripting documentation](https://gtamp.com/GTA2/gta2script.7z)
- [.MIS decompiler](https://gtamp.com/forum/viewtopic.php?f=4&t=447) by T.M.
- [Memory addresses list](https://gtamp.com/forum/viewtopic.php?t=1123) by BeepBoop
- [DirectInput key codes list](https://gist.github.com/tracend/912308) by [tracend](https://gist.github.com/tracend)


## License

You can modify and distribute this mod freely as long as you credit the original authors and provide a link to [this repository](https://github.com/izawartka/gta2-seuko-mod). You are not allowed to sell this mod or any modified versions of it.
