#include "tests.hpp"
#include "constants/constants.hpp"
#include <iostream>
#include <assert.h>
#include <fstream>
#include <filesystem>

// Sends the file and line number to the AssertWithMessageFull function
#define AssertWithMessage(condition, message) AssertWithMessageFull(condition, message, __FILE__, __LINE__)

// Constructs and returns a test game instance
core::Game tests::CreateTestGame(unsigned int player_count, unsigned int map_size) {
    core::Game game;
    std::vector<core::Game::PlayerInit> players;
    std::shared_ptr<cards::Card> test_card = std::make_shared<cards::BuildingCard>("Test card", "This is a test card", core::Resource(core::ResourceType::kGold, 1), buildings::FarmBuilding::CreateEmpty(10));
    std::vector<std::shared_ptr<cards::Card>> test_cards = {};
    int card_count = 10;
    for (int i = 0; i < 10; i++) {
        test_cards.push_back(test_card->Clone());
    }
    cards::Deck test_deck = cards::Deck(test_cards, card_count);
    for (unsigned int i = 0; i < player_count; ++i) {
        players.emplace_back(core::Game::PlayerInit{
            "Player " + std::to_string(i + 1),
            test_deck.Clone() // TODO: add custom starter decks
        });
    }
    game.Initialize(players, map_size);
    return game;
}

// Testing the Game class
void tests::TestGameInitializationAndTurns() {
  std::cout << "Testing Game class..." << std::endl;
  core::Game game = CreateTestGame(4, 5);
  AssertWithMessage(game.IsInitialized() && !game.IsOver(), "Game should be initialized and not over after initialization.");
  AssertWithMessage(game.GetCurrentPlayer().GetName() == "Player 1", "Current player should be 'Player 1' after initialization.");
  for (int i = 0; i < 5; i++) {
    game.NextTurn();
    AssertWithMessage(game.GetCurrentTurn() == i + 1, "Turn number should be " + std::to_string(i + 1) + " after " + std::to_string(i) + " turns.");
  }
  for (int i = 0; i < 3; i++) {
    const std::shared_ptr<buildings::Building> capital = game.GetCurrentPlayer().GetBuildings().front();
    game.GetCurrentPlayer().RemoveBuilding(capital);
    game.NextTurn();
    AssertWithMessage(game.GetNofPlayers() == 4 - (i + 1), "Number of players should be " + std::to_string(4 - (i + 1)) + " after removing a player.");
  }
  AssertWithMessage(game.IsOver(), "Game should be over when only one player is left.");
  std::cout << "Game class tests completed successfully." << std::endl;
};

void tests::DebugGameState(const core::Game& game) {
    std::cout << "Game State Debug" << std::endl;
    core::PrintTestMsg("Is Initialized: ", game.IsInitialized());
    core::PrintTestMsg("Number of Players: ", game.GetNofPlayers());
    core::PrintTestMsg("Current Turn: ", game.GetCurrentTurn());
    core::PrintTestMsg("Current Player: ", game.GetCurrentPlayer().GetName());
}

/**
 * @brief Tests loading a game from a faulty file.
 * 
 * This function tests whether an exception is thrown when loading a game
 * from a faulty file. It opens the file, attempts to load the game from the file,
 * and checks whether the expected exception is thrown.
 * 
 * @param filename The name of the file to load from.
 */
void TestFaultyFile(std::string filename) {
    core::Game faultyGame;
    std::ifstream faultyFile(filename);
    if (faultyFile.is_open()) {
        try {
            faultyGame.Load(faultyFile);
            tests::AssertWithMessage(false, "Loading from a faulty file should fail: " + filename);
        } catch (std::exception& e) {
            core::PrintTestMsg("Correctly caught exception when loading faulty file '", filename, "': ", e.what());
        }
        faultyFile.close();
    }
}

void tests::TestGameSaveAndLoad() {
    std::cout << "Testing Game Save and Load..." << std::endl;
    core::Game game = CreateTestGame();
    // Advance a few turns
    for (int i = 0; i < game.GetNofPlayers(); i++) {
        game.NextTurn();
        // Add resources to the current player for testing
        game.GetCurrentPlayer().AddResources({core::Resource(core::ResourceType::kGold, 10 + 2 * i)});
        // Make the capital of the players take damage
        game.GetCurrentPlayer().GetBuildings().front()->takeDamage(20 * i);
        // Use the first card from the player's hand
        std::shared_ptr<cards::Card> card_to_play = game.GetCurrentPlayer().GetHand()->GetCards().front();
        game.PlayCardOnTile(card_to_play, game.GetMap().get_tile(i + 1));
        units::Unit::Create(game.GetCurrentPlayer().GetCapitalBuilding()->getTile(), game.GetCurrentPlayerPtr(), 10, units::UnitType::kSoldier);
    }
    // Save the game state
    std::ofstream outFile("testSaveFile.txt");
    if (outFile.is_open()) {
        game.Save(outFile);
        outFile.close();
    } else {
        std::cerr << "Failed to open file for saving." << std::endl;
        return;
    }
    // Test loading with a faulty files
    // Get files in tests/faultySaves/
    try {
        for (const auto& file : std::filesystem::directory_iterator(constants::faultySavesPath)) {
            if (file.is_regular_file() && file.path().extension() == ".txt") {
                TestFaultyFile(file.path().string());
            }
        }
    } catch(const std::exception& e) {
        AssertWithMessage(false, "Could not access faulty saves directory.");
    }
    
    // Load the game state into a new Game instance
    core::Game loadedGame;
    std::ifstream inFile("testSaveFile.txt");
    if (inFile.is_open()) {
        loadedGame.Load(inFile);
        inFile.close();
    } else {
        std::cerr << "Failed to open file for loading." << std::endl;
        return;
    }
    // Check that the loaded game matches the original game
    AssertWithMessage(loadedGame.IsInitialized() == game.IsInitialized(), "Loaded game initialization state should match original.");
    AssertWithMessage(loadedGame.GetNofPlayers() == game.GetNofPlayers(), "Loaded game number of players should match original.");
    AssertWithMessage(loadedGame.GetCurrentTurn() == game.GetCurrentTurn(), "Loaded game current turn should match original.");
    AssertWithMessage(loadedGame.GetCurrentPlayer().GetName() == game.GetCurrentPlayer().GetName(), "Loaded game current player name should match original.");
    // Check each player's state
    for (unsigned int i = 0; i < game.GetNofPlayers(); ++i) {
        const core::Player& originalPlayer = game.GetCurrentPlayer();
        const core::Player& loadedPlayer = loadedGame.GetCurrentPlayer();
        const auto& originalResources = originalPlayer.GetResources();
        const auto& loadedResources = loadedPlayer.GetResources();
        AssertWithMessage(originalPlayer.GetName() == loadedPlayer.GetName(), "Player names for player " + std::to_string(i + 1) + " should match.");
        AssertWithMessage(originalResources.size() == loadedResources.size(), "Resource size for player " + std::to_string(i + 1) + " should match.");
        for (size_t j = 0; j < originalResources.size(); ++j) {
            AssertWithMessage(originalResources[j].type == loadedResources[j].type, "Resource types for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(originalResources[j].amount == loadedResources[j].amount, "Resource amounts for player " + std::to_string(i + 1) + " should match.");
        }
        auto& originalBuildings = originalPlayer.GetBuildings();
        auto& loadedBuildings = loadedPlayer.GetBuildings();
        const auto& originalBuildingsVector = std::vector<std::shared_ptr<buildings::Building>>(originalBuildings.begin(), originalBuildings.end());
        const auto& loadedBuildingsVector = std::vector<std::shared_ptr<buildings::Building>>(loadedBuildings.begin(), loadedBuildings.end());
        for (size_t j = 0; j < originalBuildings.size(); ++j) {
            const auto& originalBuilding = originalBuildingsVector[j];
            const auto& loadedBuilding = loadedBuildingsVector[j];
            AssertWithMessage(originalBuilding->GetType() == loadedBuilding->GetType(), "Building types for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(originalBuilding->getMaxHp() == loadedBuilding->getMaxHp(), "Building max HP for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(originalBuilding->getCurrentHp() == loadedBuilding->getCurrentHp(), "Building current HP for player " + std::to_string(i + 1) + " should match.");
        }
        auto& originalUnits = originalPlayer.GetUnits();
        auto& loadedUnits = loadedPlayer.GetUnits();
        const auto& originalUnitsVector = std::vector<std::shared_ptr<units::Unit>>(originalUnits.begin(), originalUnits.end());
        const auto& loadedUnitsVector = std::vector<std::shared_ptr<units::Unit>>(loadedUnits.begin(), loadedUnits.end());
        for (size_t j = 0; j < originalUnits.size(); ++j) {
            const auto& originalUnit = originalUnitsVector[j];
            const auto& loadedUnit = loadedUnitsVector[j];
            AssertWithMessage(originalUnit->GetType() == loadedUnit->GetType(), "Unit types for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(originalUnit->getMaxHp() == loadedUnit->getMaxHp(), "Unit max HP for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(originalUnit->getCurrentHp() == loadedUnit->getCurrentHp(), "Unit current HP for player " + std::to_string(i + 1) + " should match.");
            AssertWithMessage(loadedPlayer.GetCapitalBuilding()->getTile()->get_unit() != nullptr, "There should be a unit on the capital building tile.");
        }
        game.NextTurn();
        loadedGame.NextTurn();
    }
    constants::debug ? DebugGameState(loadedGame) : void();
    std::cout << "Game Save and Load tests completed successfully." << std::endl;
}

void tests::TestPlayer() {
    std::cout << "Testing Player class..." << std::endl;
    core::Game game = CreateTestGame(2, 5);
    core::Player& player1 = game.GetCurrentPlayer();
    game.NextTurn();
    core::Player& player2 = game.GetCurrentPlayer();

    // Test adding resources
    player1.AddResources({core::Resource(core::ResourceType::kGold, 50)});
    AssertWithMessage(player1.HasResource(core::Resource(core::ResourceType::kGold, 50)), "Player should have 50 Gold after adding.");
    
    // Test removing resources
    player1.RemoveResource(core::Resource(core::ResourceType::kGold, 20));
    AssertWithMessage(player1.HasResource(core::Resource(core::ResourceType::kGold, 30)), "Player should have 30 Gold after removing 20.");
    
    // Test removing more resources than available
    try {
        player1.RemoveResource(core::Resource(core::ResourceType::kGold, 40));
        AssertWithMessage(false, "Removing more resources than available should throw an exception.");
    } catch (const std::exception& e) {
        core::PrintTestMsg("Correctly caught exception when removing too many resources: ", e.what());
    }
    
    // Test IsAlive method
    AssertWithMessage(player1.IsAlive(), "Player should be alive with capital building intact.");
    
    // Destroy capital building
    auto capital = player1.GetCapitalBuilding();
    capital->takeDamage(capital->getCurrentHp());
    game.NextTurn();
    AssertWithMessage(!player1.IsAlive(), "Player should be dead after capital building is destroyed.");
    std::cout << "Player class tests completed successfully." << std::endl;
}

void tests::TestMap() {
    std::cout << "Testing Map class..." << std::endl;
    core::Game game = CreateTestGame(2, 5);
    world::Map& map = game.GetMap();

    // Test included tiles
    AssertWithMessage(map.get_tiles().size() == 25, "Map should have 25 tiles for a 5x5 map.");
    AssertWithMessage(map.get_tile(0) != nullptr, "Tile 0 should not be null.");
    AssertWithMessage(map.get_tile(24) != nullptr, "Tile 24 should not be null.");

    // Test out-of-bounds tile access
    AssertWithMessage(map.get_tile(25) == nullptr, "Tile 25 should be null.");

    for (auto tile : map.get_tiles()) {
        // Test that each tile has terrain set
        AssertWithMessage(tile->get_terrain() != nullptr, "Each tile should have terrain set.");
        AssertWithMessage(tile->get_terrain()->get_terrain_type() >= 0 && tile->get_terrain()->get_terrain_type() <= constants::kNumberOfTerrainTypes, "Tile terrain type should be valid.");
    }

    // Test buildings on a tile
    std::shared_ptr<buildings::Building> capital = game.GetCurrentPlayer().GetCapitalBuilding();
    AssertWithMessage(capital->getTile()->get_building() == capital, "Capital building should be correctly placed on its tile.");
    buildings::FarmBuilding::Create(game.GetCurrentPlayer().GetCapitalBuilding()->getTile(), game.GetCurrentPlayerPtr(), 10);
    AssertWithMessage(capital->getTile()->get_building() == capital, "Capital building should be still on its tile after trying to place another building on the same tile.");

    std::cout << "Map class tests completed successfully." << std::endl;
}

void tests::TestBuildings() {
    std::cout << "Testing Buildings..." << std::endl;
    core::Game game = CreateTestGame(2, 5);
    core::Player& player = game.GetCurrentPlayer();
    std::shared_ptr<world::Tile> tile;
    int i = 0;
    while ((tile == nullptr || tile == player.GetCapitalBuilding()->getTile()) && i < game.GetMap().get_tiles().size()) {
        tile = game.GetMap().get_tile(i);
        i++;
    }

    // Test FarmBuilding creation
    std::shared_ptr<buildings::Building> farm = buildings::FarmBuilding::Create(tile, game.GetCurrentPlayerPtr(), 10);
    AssertWithMessage(farm->GetType() == buildings::BuildingType::kFarm, "Farm building type should be kFarm.");
    AssertWithMessage(farm->getMaxHp() == 10, "Farm building max HP should be 10.");
    AssertWithMessage(farm->getCurrentHp() == 10, "Farm building current HP should be 10.");

    // Test damage application
    farm->takeDamage(3);
    AssertWithMessage(farm->getCurrentHp() == 7, "Farm building current HP should be 7 after taking 3 damage.");

    farm->takeDamage(100);
    AssertWithMessage(farm->getCurrentHp() == 0, "Farm building current HP should be 0 after taking lethal damage.");
    AssertWithMessage(tile->get_building() == nullptr, "Tile should have no building after farm is destroyed.");

    std::cout << "Building tests completed successfully." << std::endl;
}

void tests::TestUnits() {
    std::cout << "Testing Units..." << std::endl;
    core::Game game = CreateTestGame(2, 5);
    core::Player& player = game.GetCurrentPlayer();
    std::shared_ptr<world::Tile> tile = game.GetMap().get_tile(0);
    tile->remove_current_unit(); // Ensure tile is empty

    // Test Unit creation
    std::shared_ptr<units::Unit> soldier = units::Unit::Create(tile, game.GetCurrentPlayerPtr(), 15, units::UnitType::kSoldier);
    AssertWithMessage(soldier->GetType() == units::UnitType::kSoldier, "Unit type should be kSoldier.");
    AssertWithMessage(soldier->getMaxHp() == 15, "Unit max HP should be 15.");
    AssertWithMessage(soldier->getCurrentHp() == 15, "Unit current HP should be 15.");

    // Test damage application
    soldier->takeDamage(5);
    AssertWithMessage(soldier->getCurrentHp() == 10, "Unit current HP should be 10 after taking 5 damage.");

    soldier->takeDamage(20);
    AssertWithMessage(soldier->getCurrentHp() == 0, "Unit current HP should be 0 after taking lethal damage.");
    AssertWithMessage(tile->get_unit() == nullptr, "Tile should have no unit after soldier is destroyed.");

    soldier = units::Unit::Create(tile, game.GetCurrentPlayerPtr(), 15, units::UnitType::kSoldier);
    game.NextTurn();

    core::Player& player2 = game.GetCurrentPlayer();
    std::shared_ptr<world::Tile> tile2 = game.GetMap().get_tile(1);
    tile2->remove_current_unit(); // Ensure tile is empty
    std::shared_ptr<units::Unit> enemy_soldier = units::Unit::Create(tile2, game.GetCurrentPlayerPtr(), 15, units::UnitType::kSoldier);

    // Test attacking
    soldier->moveToTile(tile2);
    AssertWithMessage(enemy_soldier->getCurrentHp() < 15, "Enemy soldier should have taken damage after being attacked.");
    AssertWithMessage(soldier->getCurrentHp() == 15, "Attacking soldier should not have taken damage.");
    std::shared_ptr<world::Tile> correct_tile = enemy_soldier->getCurrentHp() > 0 ? tile : tile2;
    AssertWithMessage(soldier->GetTile() == correct_tile, "Attacking soldier's tile should be correct after attack.");

    std::cout << "Unit tests completed successfully." << std::endl;
}

void tests::TestCards() {
    std::cout << "Testing Cards..." << std::endl;
    // Test BuildingCard creation
    std::shared_ptr<buildings::Building> test_building = buildings::FarmBuilding::CreateEmpty(10);
    cards::BuildingCard building_card("Farm Card", "A card to build a farm.", core::Resource(core::ResourceType::kGold, 5), test_building);
    AssertWithMessage(building_card.GetName() == "Farm Card", "Building card name should be 'Farm Card'.");
    AssertWithMessage(building_card.GetDescription() == "A card to build a farm.", "Building card description should match.");
    AssertWithMessage(building_card.GetCost().type == core::ResourceType::kGold && building_card.GetCost().amount == 5, "Building card cost should be 5 Gold.");

    // Test cloning
    std::shared_ptr<cards::Card> cloned_card = building_card.Clone();
    AssertWithMessage(cloned_card->GetName() == building_card.GetName(), "Cloned card name should match original.");
    AssertWithMessage(cloned_card->GetDescription() == building_card.GetDescription(), "Cloned card description should match original.");

    std::cout << "Card tests completed successfully." << std::endl;
}