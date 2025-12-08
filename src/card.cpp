#include "cards/card.hpp"
#include "constants/constants.hpp"
#include "constants/deck_and_card_options.hpp"
#include "core/player.hpp"
#include "core/utils.hpp"

namespace cards { 


std::ostream& operator<<(std::ostream& out, const std::shared_ptr<Card>& other) {
    if (other) {
        out << other->GetName() << '\n';
    } else {
        out << "<null>\n";
    }
    return out;
}

std::istream& operator>>(std::istream& in, std::shared_ptr<Card>& other) {
    std::string card_name = core::GetStringFromLine(in);

    bool found = false;
    for (const auto& c : card_constants::kAllAvailableCards) {
        if (card_name == c->GetName()) {
            other = c->Clone();
            found = true;
            break;
        }
    }

    if (!found) {
        other.reset();                      // optional
        in.setstate(std::ios::failbit);     // recommended
    }
    return in;
}


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