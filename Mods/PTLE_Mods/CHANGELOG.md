# Gizmod Changelog

## v0.2.1

Additions :

- Added "speedrunFriendly" option (disables all mod features we're not sure are affecting gameplay or not)
- Randomizer : Insta-dash option (grant the player dash at the start for quicker games)
- CheatLab : Added a simple top-down camera mode
- CheatLab : "Teleport to explorer" cheat + "Give 5 idols" cheat

Fixes :

- Bushes not moving along with bush ninjas
- Incorrect reported size for `EUIObjectNode` type
- Randomizer : Starting area systematically had Valley of Spirits and Mama-Oullo Tower connected

## v0.2.0

Additions :

- Gizmod configuration : Bypass legal notice and splash screens on game startup (add `"skipSplashScreens=true"`)
- Save interface
	- Tampering with the game's own save variables
	- Adding custom, arbitrary save data to a game save
- Events
  - Override the music played upon starting the level ("LevelLoaded" event)
  - "BeastStateSwitch" event (everytime an enemy or NPC changes its AI state)
  - "BreakableBreak" event (stone pillars, pots, hay stacks, etc...)
  - "CinematicPlay" event (anytime a cutscene starts playing)
  - "CollectIdol" event
  - "PlantRustle" event (tallgrass and other plants moving when colliding with Harry or a projectile)
  - "ProjectileShoot" event (sling rocks, TNT, mangos, coconuts, porcupine spikes, etc...)
- New interfaces
  - Barebones HUD manipulation
  - Inventory interface (changing hotbar items, item in hand, unlocking shaman notes)


Fixes :

- Fixed typo "`MOUTAIN_NOTES`" in `ShamanShop`
- Fixed crashes with getting current level and listing entities



## v0.1.1

Fixes :

- Mole no longer fires at you (tied to Monkey Temple crash, this was due to type infos not being registered, I just forgot)
- Grabbing an artifact (spirit temples, St.Claire's camp) no longer crashes
- Load times are faster and more consistent (at least on my end)
- Explorers no longer hardlock **(Hurray, finally!)**
	Note : The method used for this fix still needs to be proved speedrun-friendly, that is, not giving advantage to players and not breaking other parts of the game.

## v0.1.0

Additions :

- Plugin interface
	- Dedicated configuration directory
- Inventory item interface
	- Unlocking, getting model ID, name, etc...
- Shaman shop interface
	- Overridable prices
- World interface
	- Getting all entities / entities of a specific type
	- Arbitrary projectile spawning
- Event-driven API for hooking actions to gameplay events
	- LoadLevelEvent : Called when a new level is requested to be loaded
	- LevelLoadedEvent : Called when a level has just finished loading (even after a respawn)
	- CollectItemEvent : Called when Harry collects a floating item (idol, explorer idol, temple artifact...)
	- ShamanPurchaseEvent : Called when the player selects an item in the shaman shop. The purchase might fail.
- Mod loader configuration :
	- Enabling / disabling mod loading altogether, without uninstalling Gizmod.
	- Showing a console for real-time info and error logging.
- Huge base of knowledge of the game's internal types, useable to alter object states and implement complex gameplay behavior seamlessly within the game.

Fixes :

- Native game bug : Sometimes, coming back from Monkey Temple to Flooded Courtyard causes a game crash.



## Pre-versioning

Before Gizmod, we used another pre-built mod loader.