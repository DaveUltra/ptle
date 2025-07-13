# Pitfall : The Lost Expedition, PC Mods

## Downloading

You can also check out the [latest automated builds](/../../actions/workflows/build-mods.yml?query=event%3Apush+is%3Asuccess+branch%3Amaster) (requires a GitHub account)
(If you don't have a GitHub account, you can try [nightly.link](https://nightly.link/DaveUltra/ptle/workflows/build-mods/master))

## Installing / Using

### How to Setup

To set up the mod loader, you only need four steps :

- Locate your game installation directory. It should be in `C:\Program Files (x86)\Aspyr\PITFALL The Lost Expedition` by default,
- Extract the contents of the downloaded `PTLE_Mods.zip` wherever you want,
- Run `install.bat` as administrator, and give it the path to your Pitfall installation directory.
- Remove `KBhook.dll` to avoid the game stealing focus from other programs. The game will use the default system DLL.

### Enabling / Disabling the mods

- To enable / disable the mod loader without uninstalling fully, you can use the provided `config.ini` in the `Game/` directory with the option `enabled=true/false`.
- To disable individual mods, move or delete the `.asi` file out of the `Game/mods/` directory.

### Uninstalling

- To uninstall the mod loader, execute `uninstall.bat` located in your install directory, as administrator.



**Note :** You might be aware of PC's version many flaws : the taskbar gets locked, the game constantly steals focus, responds to any input even outside of its window, and freezes on Alt+Tabs. DxWnd does allow playing in windowed mode, but it does not address any of the other issues. It is recommended to use the `WindowedMode.asi` mod to counter all of these effects and be able to multitask.



## Building the mods

You can edit and build the mods yourself using the Visual Studio solution. You'll need at least MSBuildTools v142 for C++ (as that's the targetted platform toolset, comes with Visual Studio 2019), which can be installed with the Visual Studio Installer.

Mods will be automatically saved under `$(SolutionDir)bin\$(Configuration)` (where `$(Configuration)` is `Release`|`Debug`). You only want the `.asi` files and `config` folder.
