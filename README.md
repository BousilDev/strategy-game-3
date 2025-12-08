# Strategy Game 3

The game is a turn-based Civilization-like strategy game where structures are built and units are deployed with playing cards from a deck. The initial aim of the game was to have modifiable decks, so that the players would make strategic deck-building choices throughout the game, but deck modifications were left unimplemented due to time constraints. In the game, the player must manage their resources through the use of different buildings providing them, and use units to attack and defend from other players. Playing cards cost resources. In one turn, the player plays down cards on the map and moves and attacks with their placed down units. The goal of the game is to beat other players by destroying their capital building and thus defeating their empire.

The player can interact with the game using their mouse through a visual interface. The game includes a simple main menu from which a game can be created or loaded. A created game can have 2-4 players, different map sizes and different starting decks. During a game, the player can click on cards, buildings and units to manipulate them. The map can be panned with the arrow keys. By pressing the ESC key, the game is saved and the player is returned to the main menu. A game state can be saved to a file and loaded from it to save the player’s progress between sessions.

The world is constructed from single color hexagonal tiles while units and buildings are represented by simple shapes. Units are circles and buildings are rectangles, and a health bar is visualized next to them. Multiplayer gameplay is implemented locally with 2-4 players. There is no single player option. The player can only see tiles that are nearby to their already placed buildings or units. Other tiles are shown as only grey and cannot be accessed before they are approached with units or buildings are constructed close to them.


# Group

- Oliver Niemi
- Konsta Suuriniemi
- Aleksi Sarja
- Topi Varuskivi
- Atte Inki

# Repository organization

Your project implementation should follow the organization in this repository.
See readme.md files in each folder.

# Project Implementation

You must use git repository for the work on the project, making frequent enough commits so
that the project group (and course staff) can follow the progress.

The course staff should be able to easily compile the project work using makefile and related
instructions provided in the git repository. The final output should be in the **master branch** of the git repository.

# Source code documentation

It is recommended to use Doxygen to document your source code.
Please go over the _Project Guidelines_ for details.
