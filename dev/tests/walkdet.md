Test Detect Traps and Walk Silently
===================================

Basic Detect Traps tests

* Try detect -> walk slow, icon on
* Exit dungeon when detect on -> detect off, reset walk speed
* Try detect when have caution -> do nothing
* Learn caution when detect is on -> fix walk speed to normal, icon stays on
* Learn caution when detect is off -> turns on icon
* Party change when detect is on, party no caution -> detect off
* Party change add member with caution -> icon on after party change
* Cast FindTraps while Detecting -> continue detecting after done.

Basic Walk Silently tests

* Try walk -> walk slow, icon on
* Exit dungeon with walk on -> walk off, reset walk speed
* Try walk when have run -> icon on, fast walk speed
* Learn run when walk is on -> fix walk speed to normal
* Learn run when walk is off -> nothing
* Party change when walk is on -> turn off
* Verify initial encounter rate is doubled when walk is on
* Verify #steps doubles when walk enabled
* Verify #steps halves when walk disabled

Combined tests

* Try walk when detect is on no caution -> detect off, walk on, slow
* Try walk when detect is on with caution -> walk on, slow
* Try walk when detect is on no caution, run -> detect off, walk on, fast
* Try walk when detect is on with caution, run -> walk on, fast
* Try detect when walk is on -> walk off, detect on, slow
* Try detect when walk is on, run -> walk off, detect on, slow
* Try detect when walk is on, caution -> do nothing
* Try detect when walk is on, run, caution -> do nothing
