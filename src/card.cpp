#include "cards/card.hpp"
#include "core/player.hpp"

namespace cards { 

bool BuildingCard::Play(world::Tile& target, std::shared_ptr<core::Player> player) {
    auto building = building_->CreateEmptyFromCopy();
    bool successful = !(target.get_terrain()->get_name() == "water" || !target.place_building(building));
    if (successful) {
        building->setTile(std::make_shared<world::Tile>(target));
        building->setPlayer(player);
        player->AddBuilding(building);
    }
    return successful;
}

bool UnitCard::Play(world::Tile& target, std::shared_ptr<core::Player> player) {
    auto unit = unit_->CreateEmptyFromCopy();
    bool successful = !(target.get_terrain()->get_name() == "water" || !target.place_unit(unit));
    if (successful) {
        unit->setTile(std::make_shared<world::Tile>(target));
        unit->setPlayer(player);
        player->AddUnit(unit);
    }
    return successful;
}

} // namespace cards