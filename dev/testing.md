Testing Notes
================

Debug Controls
--------------

* `F9` - Manipulate switches and variables
* `Ctrl` - Hold to walk through walls.

Debug Mode
----------

Enable switch `115:DEBUG MENU` to spawn the debug menu. After accessing
it the first time, a `Debugger` item will be added to your inventory.
This item can be used instead of manually activating the switch to
access the menu again.

Debug Checks
------------

When testing the game we should always enable switch `33:DEBUG CHECKS`.
This will add additional checks and graphical overlays which tell us
the state of some hidden variables in game.

* After every battle should see "Battle Win" or "Battle Loss" animation
    * "Battle Win" if you won the battle
    * "Battle Loss" if you died (scripted no game over), or ran away
        * Note: This does not occur if you run away on first turn
    * Should always work for random encounters
    * Check that it happens always immediately after boss battles

* Should see debug boxes in upper right
    * Outside or Inside, depending on map
    * Menu, Regen, Enemies normally on, off for cutscenes and scripted movement.
    * Day / Night, depending on events

Save the game anywhere
----------------------

Call the debug menu (see above) and choose option `22` to save the game
anywhere. Please provide save game files with bug reports.
