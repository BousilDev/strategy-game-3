#include "cards/card.hpp"

namespace cards { 

bool BuildingCard::Play(world::Tile& target) {
    return !(target.get_terrain()->get_name() == "water" || !target.place_building(building_->CreateEmptyFromCopy()));
}

bool UnitCard::Play(world::Tile& target) {
    return !(target.get_terrain()->get_name() == "water" || !target.place_unit(unit_->CreateEmptyFromCopy()));
}

} // namespace cards