#include "cards/card.hpp"

namespace cards { 

bool BuildingCard::Play(world::Tile& target) {
    return !(target.get_terrain()->get_name() == "water" || !target.place_building(building_->Clone()));
}

bool UnitCard::Play(world::Tile& target) {
    return !(target.get_terrain()->get_name() == "water" || !target.place_unit(unit_->Clone()));
}

} // namespace cards