#include "cards/card.hpp"
#include "core/player.hpp"

namespace cards { 

bool BuildingCard::Play(std::shared_ptr<world::Tile> target, std::shared_ptr<core::Player> player) {
    if (player->HasResource(this->GetCost()) == false) {
        return false;
    }
    auto building = building_->CreateEmptyFromCopy();
    bool successful = !(target->get_terrain()->get_name() == "water" || !target->place_building(building));
    if (successful) {
        building->setTile(target);
        building->setPlayer(player);
        player->AddBuilding(building);
        player->RemoveResource(this->GetCost());
    }
    return successful;
}

bool UnitCard::Play(std::shared_ptr<world::Tile> target, std::shared_ptr<core::Player> player) {
    if (player->HasResource(this->GetCost()) == false) {
        return false;
    }
    auto unit = unit_->CreateEmptyFromCopy();
    bool successful = !(target->get_terrain()->get_name() == "water" || !target->place_unit(unit));
    if (successful) {
        unit->setTile(target);
        unit->setPlayer(player);
        player->AddUnit(unit);
        player->RemoveResource(this->GetCost());
    }
    return successful;
}

} // namespace cards