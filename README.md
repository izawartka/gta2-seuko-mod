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
- Quicksaving
- Custom quick actions on key press
- Saving and spawning saved vehicles
- Spawning objects (including power-ups and secret objects cut out from the game)
- Native cheats (each cheat can be force disabled, force enabled or unmodified to not interfere with cheat codes and registry flags)
- Configurable menu controls
- Shadows distance fix

more features coming soon...

## Compatibility

- This mod is intended for [GTA2 PC version 11.44](https://gtamp.com/GTA2/gta2-installer.exe) only
- Widescreenfix is not supported at the moment, i'm planning to include by own version of it in the future

## Installation

1. Make sure you have [VC++ Redistributables](https://www.techpowerup.com/download/visual-c-redistributable-runtime-package-all-in-one/) installed
2. Download the [latest release](https://github.com/izawartka/gta2-seuko-mod/releases/latest)
3. Extract the contents of the downloaded archive to your GTA2 installation folder
4. Launch the game

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
3. Open `gta2-seuko-mod.sln` in Visual Studio.
4. Select one of the build configurations - Debug, Release or ReleaseWithLogging.
5. Build the solution.
6. After the build, output files will be automatically copied to your GTA2 installation folder and the game will be launched.

## Support

If you like this mod and want to support its development, you can donate via [PayPal](https://www.paypal.me/maseuko) :)

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
