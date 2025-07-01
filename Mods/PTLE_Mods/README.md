# Pitfall : The Lost Expedition, PC Mods

## Downloading

You can also check out the [latest automated builds](/../../../../actions/workflows/build-mods.yml?query=event%3Apush+is%3Asuccess+branch%3Amaster) (requires a GitHub account)\
(If you don't have a GitHub account, you can try [nightly.link](https://nightly.link/DaveUltra/ptle/workflows/build-mods/master))

## Installing / Using

### How to Setup

To set up the mod loader, you only need three steps :

- Locate your game installation directory. It should be in `C:\Program Files (x86)\Aspyr\PITFALL The Lost Expedition` by default.
- Copy the contents of the repository's `patch` directory into the `Game` directory. Don't worry about overwriting the existing `binkw32.dll`, a backed up copy is provided.
- Remove `KBhook.dll` to avoid the game stealing focus from other programs. The game will use the default system DLL.

Then, you are ready to use mods. You can find pre-built versions of the mods in this repository under `mods`. To enable a mod, simply copy the `.asi` file into the `Game` directory with the rest of the patch files. The next time you open the game, any `.asi` file that the mod loader finds will be automatically loaded.

**Note :** You might be aware of PC's version many flaws : the taskbar gets locked, the game constantly steals focus, responds to any input even outside of its window, and freezes on Alt+Tabs. DxWnd does allow playing in windowed mode, but it does not address any of the other issues. It is recommended to use the `WindowedMode.asi` mod to counter all of these effects and be able to multitask.

### How to Operate

By default, the mod loader will be on. If you wish to toggle between playing with or without mods, you can use the two batch files `enable_mod.bat` and `disable_mod.bat`.

To disable a specific mod, delete or move its `.asi` file out of the game's install directory.

## Building the mods

You can edit and build the mods yourself using the Visual Studio solution. You'll need at least MSBuildTools v142 for C++ (as that's the targetted platform toolset, comes with Visual Studio 2019), which can be installed with the Visual Studio Installer.

Mods will be automatically saved under `$(SolutionDir)bin\$(Configuration)` (where `$(Configuration)` is `Release`|`Debug`). You only want the `.adsi` files and `cfg` folder.
