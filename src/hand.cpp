#include "cards/hand.hpp"
#include "cards/deck.hpp"

namespace cards {

const std::shared_ptr<Card>& Hand::GetCard(int i) {
    if (i < 0 || i >= static_cast<int>(contents_.size()))
        throw std::out_of_range("Hand::GetCard index out of range.");

    return contents_[i];
}

bool Hand::PlayCard(std::shared_ptr<Card> card, world::Tile& target, std::shared_ptr<core::Player> player) {
    auto it = std::find(contents_.begin(), contents_.end(), card);
    if (it == contents_.end()) {
        throw std::out_of_range("Hand::PlayCard card not found in hand.");
    }
    bool played = (*it)->Play(target, player);
    if (played) {
        DiscardCard(card);
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

void Hand::DiscardCard(std::shared_ptr<Card> card) {
    auto it = std::find(contents_.begin(), contents_.end(), card);
    if (it == contents_.end()) {
        throw std::out_of_range("Hand::DiscardCard card not found in hand.");
    }
    contents_.erase(it);
    deck_->DiscardCard(card);
}

void Hand::DiscardHand() {
    std::vector<std::shared_ptr<Card>> discarded_cards = contents_;
    contents_.clear();
    for (auto& card : discarded_cards) {
        deck_->DiscardCard(card);
    }
}

} // namespace cards