#include "cards/hand.hpp"
#include "cards/deck.hpp"

namespace cards {

std::ostream& operator<<(std::ostream& out, const std::shared_ptr<Hand>& other) {
    // Write hand size
    out << other->size_ << '\n';
    // Write number of cards in hand
    out << other->contents_.size() << '\n';
    // Write each card
    for (const auto& card : other->contents_) {
        out << card;
    }
    return out;
}

std::istream& operator>>(std::istream& in,  std::shared_ptr<Hand>& other) {
    // Read hand size
    other->size_ = core::GetIntFromLine(in);
    // Read number of cards in hand
    int num_cards = core::GetIntFromLine(in);
    // Read each card
    other->contents_.clear();
    for (int i = 0; i < num_cards; ++i) {
        std::shared_ptr<Card> card;
        in >> card;
        other->contents_.push_back(card);
    }
    return in;
}

const std::shared_ptr<Card>& Hand::GetCard(int i) {
    if (i < 0 || i >= static_cast<int>(contents_.size()))
        throw std::out_of_range("Hand::GetCard index out of range.");

    return contents_[i];
}

bool Hand::PlayCard(std::shared_ptr<Card> card, std::shared_ptr<world::Tile> target, std::shared_ptr<core::Player> player) {
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