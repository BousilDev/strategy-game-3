# Meeting Notes

In this file, you should keep notes of your project meetings.
In each meeting, you are required to discuss:

1. What each member has done?
2. Are there challenges or problems? Discuss the possible solutions
3. Plan for the next steps for everyone
4. Deviations and changes to the project plan, if any

# Meeting dd.mm.2021 HH::MM

**Participants**:

1. Member 1
2. Member 2
3. Member 3
4. Member 4

## Summary of works

1. Member 1

   Implementing the class XX. Tested the class XX.
   Results are in `tests/<class-xx-tests>`. Resolved the identified problems.

2. Member 2

   Same as above

3. ...

## Challenges

1. The integration of UI with the monsters requires an abstract interface.
2. ...

## Actions

1. Member 1 is going to look into defining an abstract interface for monsters
   to enable easy UI integration.
2. Member 2 is going to work with Member 1 to use abstract interface in derived
   monster classes.
3. Member 2 is going to test the interface.
4. Member 3 is going to use ...

> Please reflect these action decisions in your git commit messages so that
> your group members and advisor can follow the progress.

## Project status

Short summary of current project status.

### TODOs

1. Member 1: Write an action.
2. ...


# Meeting 6.10.2025 10::00 - 11::45

**Participants**:

1. Oliver Niemi
2. Konsta Suuriniemi
3. Aleksi Sarja
4. Topi Varuskivi
5. Atte Inki

## Summary of works

1. First meeting, no summary

## Challenges

1. Not sure yet which features to implement in which sprints and how much time the implementation takes.

## Actions

1. Created issues to Issue Board which reflect the tasks each of us is doing
2. Scheduled a time for the next meeting

## Project status

A rough outline for the project scope, frameworks and a sketch for an UML diagram exist. Work and importance of some features evaluated. Everyone knows what they are supposed to do for the next meeting.

### TODOs

1. Prioritize and estimate project features
2. Create initial UML class diagram
3. Research how SFML works and share remarks/notes
4. Finish documenting the project scope
5. Research how CMake is going to be used in the project


# Meeting 10.10.2025 12::00 - 12::30 @Teams

**Participants**:

1. Oliver Niemi
2. Konsta Suuriniemi
3. Aleksi Sarja
4. Topi Varuskivi
5. Atte Inki

## Summary of works

1. Features estimated roughly
2. Initial UML diagram created
3. Basics of SFML under research
4. Project scope clarified
5. CMake basics researched

## Challenges

1. No notable challenges

## Actions

1. Discussed the deliverables of sprint 0 (the project scope and practices)

## Project status

The project scope and features are looking quite good already, some polishing needed. The basics of SFML and CMake have been researched.

### TODOs

1. Document project practices
2. Finalize the UML diagram
3. Research SFML more and report findings
4. Specify the project scope further
5. Research CMake further and report findings


# Meeting 14.10.2025 16::00 - 17::15 @Jaakko (108)

**Participants**:

1. Oliver Niemi
2. Konsta Suuriniemi
3. Aleksi Sarja
4. Topi Varuskivi
5. Atte Inki
6. Rami Ghoniem (TA)

## Summary of works

Initial project plan is committed to the repository. 
Initial CMake for the project exists.

## Challenges

In which part of the development should we link the UI to the game logic?
Can we implement all the things in our current aim?

## Actions

1. Discussed the general project idea & scope with the coach.
2. Recommendation to use some cloud service to log implemented features.
3. Scheduled time for next meeting, 23. thursday at 16 or 17 on site.
4. Scheduled time for "daily" meeting, 17. friday at 16 remotely
5. Added items to be implemented in the first sprint to the issue board.
6. Assigned an issue board task to each developer.

## Project status

Project plan ready along with CMake for the project.
Sprint 1 tasks defined in the issue board.

### TODOs

1. Research testing more.
2. Remember to add exception handling in the project (part of testing plan)
3. Think about establishing a log for implemented features
4. Each of the developers will try to implement their own task in the issue board.


# Meeting 17.10.2025 16::00 - 16::30 @Teams
Sprint 1, "daily meeting" 1

**Participants**:

1. Oliver Niemi
2. Konsta Suuriniemi
3. Aleksi Sarja
4. Topi Varuskivi
5. Atte Inki

## Summary of works

1. Initial sketch of a working main menu exists, SFML researched more.
2. Header files for basic classes exist.
3. Game and Player classes initialized and basic functionalities implemented.
4. The implementation of Map and Tile classes is progressing 

## Challenges

No notable new challenges

## Actions

1. Discussed the progress of the sprint items and reviewed what each of us had done.
2. Agreed on some design details 
3. Scheduled next "daily" meeting, 22.10 at 16 on site

## Project status

Sprint 1 is progressing so far as expected.
The project contains basic header files and some game logic functionalities.

### TODOs

1. Begin sketching the game state save & loading
2. Add the rest of the header files
3. Implement game option selection to SFML main menu
4. Finish the initial Map class


# Meeting 22.10.2025 16::00 - 16::40 @On site
Sprint 1, "daily meeting" 2

**Participants**:

1. Oliver Niemi
2. Konsta Suuriniemi
3. Aleksi Sarja
4. Topi Varuskivi
5. Atte Inki

## Summary of works

1. Main menu with interactivity (Aleksi & Topi)
2. Header files quite ready (Konsta)
3. Game class testing isolated to its own file and better testing output formatting (Oliver)
4. Map functional (Atte)

## Challenges

No notable

## Actions

1. Agreed on saving/loading strategies
2. Discussed interactive UI

## Project status

All sprint 1 backlog items seem to be on schedule.
Basic classes have their initial implementations and a main menu using SFML works.

### TODOs

1. Merge all changes before sprint review


# Meeting 23.10.2025 16::00 - 17::00 @On site
Sprint 1, review

**Participants**:

1. Oliver Niemi
2. Konsta Suuriniemi
3. Aleksi Sarja
4. Topi Varuskivi
5. Atte Inki
6. Rami Ghoniem (TA)

## Summary of works

Merged all Sprint 1 branches, modified CMake configuration to include SFML

## Challenges

Valgrind problems with SFML

## Actions

1. Reviewed Sprint 1 with TA
2. Discussed the next sprint briefly
3. Added first Sprint 2 items to issue board
4. Next daily meeting scheduled, 29.10 at 12.00 on site
5. Next sprint review scheduled, 6.11 at 16.00 on site

## Project status

Project advancing according to schedule. Main menu, header files, basic game and map logics implemented.

### TODOs

1. Add rest Sprint 2 items to issue board
2. Test whether new CMake configuration with SFML works


# Meeting 29.10.2025 12::00 - 13::00 @On site
Sprint 2, sprint planning

**Participants**:

1. Oliver Niemi
2. Konsta Suuriniemi
3. Aleksi Sarja
4. Topi Varuskivi
5. Atte Inki

## Summary of works

Game saving and loading works for pretty well for the current state of the project.

## Challenges

Current CMake configuration (including SFML) doesn't work in all environments

## Actions

1. Scheduled next meeting, 4.11 Tuesday at 12.00
2. Tried to fix the CMake issue
3. Added Sprint 2 backlog items to issue board and assigned tasks to developers

## Project status

Apart from the CMake issues everything is looking fine. Sprint 2 items have been set to issue board and assigned to developers. 

### TODOs

1. Fix CMake configuration and verify the fix as soon as possible
2. Start implementing Sprint 2 items


# Meeting 4.11.2025 12::00 - 13::00 @On site
Sprint 2, "daily" 1

**Participants**:

1. Oliver Niemi
2. Konsta Suuriniemi
3. Aleksi Sarja
4. Topi Varuskivi
5. Atte Inki (Remotely)

## Summary of works

Building class and its basic methods implemented further. UI shows the map correctly.

## Challenges

1. Problems with inconsistencies in building the project

## Actions

1. Fixed building issues, so that everyone's master branch now builds correctly.
2. Agreed on the todos for the sprint review

## Project status

A working UI with the static map works along with the game saving and loading.

### TODOs

1. Merge all changes to master before sprint review at thursday


# Meeting 6.11.2025 16::00 - 17::00 @On site
Sprint 2, review

**Participants**:

1. Oliver Niemi
2. Konsta Suuriniemi
3. Aleksi Sarja
4. Topi Varuskivi
5. Atte Inki
6. Rami Ghoniem (TA)

## Summary of works

Merged all changes to master.

## Challenges

1. Some segmentation fault issues in UI

## Actions

1. Reviewed Sprint 2 with TA
2. Discussed Sprint 3 deliverables
3. Scheduled time for the next meeting, Monday 10.11 at 10:00

## Project status

Main menu, saving & loading, initial map UI and some tests for Game & Player implemented

### TODOs

1. Add Sprint 3 items to issue board
2. Update NextTurn() and operator overloads for Building, Map, Unit
3. Add some kind of distance function for Tile/Map
4. Implement UI info screen layer and saving & loading UI interface
5. Implement Card, Hand and Deck


# Meeting 10.11.2025 10::00 - 10::45 @On site
Sprint 3, "daily" 1

**Participants**:

1. Oliver Niemi
2. Konsta Suuriniemi
3. Aleksi Sarja
4. Topi Varuskivi
5. Atte Inki

## Summary of works

Added a VS code test configuration to project, implemented initial version of Tile selecting in UI, implemented a rough info layer for UI.

## Challenges

1. When selecting a Tile, the selection is somewhat inaccurate due to SFML shapes, we discussed a possible fix for this

## Actions

1. Discussed what the info layer should include and how it gets the information it needs.

## Project status

Working main menu and game with Tile selection and rough info layer.

### TODOs

1. Implement the info layer further
2. Add helper function(s) to Building to help calculate the gained resources per turn in Game
3. Continue implementing other sprint items


# Meeting 13.11.2025 16::30 - 17::30 @On site
Sprint 3, "daily" 2

**Participants**:

1. Oliver Niemi
2. Konsta Suuriniemi
3. Aleksi Sarja
4. Topi Varuskivi
5. Atte Inki

## Summary of works

UI info layer works, main refactored using User Interface class, initial implementation of Cards, more Building and Unit functionalities

## Challenges

1. Making Buildings and Units compatible with Cards (Cloning or Creating)

## Actions

1. Discussed the UI changes, possible UI improvements and a possible solution to the challenge described above
2. Scheduled a time for next meeting, Tuesday 18.11 at 12.00

## Project status

Info layer exists with a working 'next turn' button and initial Tile info (no actual Tile info yet). The implementation of Cards has begun.

### TODOs

1. Connect separate functionalities into UI and update existing code to manage new changes