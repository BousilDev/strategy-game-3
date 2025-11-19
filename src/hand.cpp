#include "cards/hand.hpp"
#include "cards/deck.hpp"

namespace cards {

const std::shared_ptr<Card>& Hand::GetCard(int i) {
    if (i < 0 || i >= static_cast<int>(contents_.size()))
        throw std::out_of_range("Hand::GetCard index out of range.");

    return contents_[i];
}

bool Hand::PlayCard(int i, world::Tile& target) {
    if (i < 0 || i >= static_cast<int>(contents_.size())) throw std::out_of_range( "Index " + std::to_string(i) 
        + " is out of bounds for a hand with " + std::to_string(contents_.size()) + " cards." );
    auto it = contents_.begin();
    std::advance(it, i);
    bool played = (*it)->Play(target);
    if (played) {
        DiscardCard(i);
    }
    return played;
}

bool Hand::DrawCard() {
    if (contents_.size() >= size_) return false;
    auto card = deck_->DrawCard();
    if (card == nullptr) return false;
    contents_.push_back(card);
    return true;
}

void Hand::DiscardCard(int i) {
    if (i < 0 || i >= static_cast<int>(contents_.size())) throw std::out_of_range( "Index " + std::to_string(i) 
        + " is out of bounds for a hand with " + std::to_string(contents_.size()) + " cards." );
    auto it = contents_.begin();
    std::advance(it, i);
    std::shared_ptr<Card> discarded_card = *it;
    contents_.erase(it);
    deck_->DiscardCard(discarded_card);
}

void Hand::DiscardHand() {
    std::vector<std::shared_ptr<Card>> discarded_cards = contents_;
    contents_.clear();
    for (auto& card : discarded_cards) {
        deck_->DiscardCard(card);
    }
}

} // namespace cards