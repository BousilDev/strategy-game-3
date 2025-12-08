#include <algorithm>

#include "cards/deck.hpp"

namespace cards {

std::ostream& operator<<(std::ostream& out, const std::shared_ptr<Deck>& other) {
    // Write draw pile size
    out << other->draw_.size() << '\n';
    // Write each card in draw pile
    for (const auto& card : other->draw_) {
        out << card;
    }
    // Write discard pile size
    out << other->discard_.size() << '\n';
    // Write each card in discard pile
    for (const auto& card : other->discard_) {
        out << card;
    }
    out << other->hand_;
    return out;
}

std::istream& operator>>(std::istream& in,  std::shared_ptr<Deck>& other) {
    // Read draw pile size
    int draw_size = core::GetIntFromLine(in);
    other->draw_.clear();
    // Read each card in draw pile
    for (int i = 0; i < draw_size; ++i) {
        std::shared_ptr<Card> card;
        in >> card;
        other->draw_.push_back(card);
    }
    // Read discard pile size
    int discard_size = core::GetIntFromLine(in);
    other->discard_.clear();
    // Read each card in discard pile
    for (int i = 0; i < discard_size; ++i) {
        std::shared_ptr<Card> card;
        in >> card;
        other->discard_.push_back(card);
    }
    in >> other->hand_;
    return in;
}

Deck::Deck(const std::vector<std::shared_ptr<Card>>& cards, unsigned int hand_size) {
    draw_ = cards;
    hand_ = std::make_unique<Hand>(this, hand_size);
    gen_ = std::mt19937(rd_());
    ShuffleDraw();
}

std::shared_ptr<Deck> Deck::Clone() const {
    // Create a fresh deck with the same draw pile initial state and same hand size.
    // Note: The constructor performs ShuffleDraw() so we will need to overwrite after.
    auto cloned = std::make_unique<Deck>(draw_, hand_->Size());

    // Overwrite draw and discard to match the exact state
    cloned->draw_ = draw_;
    cloned->discard_ = discard_;

    // Copy RNG state
    cloned->gen_ = gen_;

    // Copy hand contents
    cloned->hand_->CopyContentsFrom(*hand_);

    return cloned;
}


// Does not work (reference to temporary). Fix if needed in the future.
// std::shared_ptr<Card>& Deck::GetRandomCard() const {
//     if (draw_.empty() && discard_.empty()) {
//         throw std::out_of_range("Deck is empty, cannot get a random card.");
//     }

//     std::vector<std::shared_ptr<Card>> combined;
//     combined.insert(combined.end(), draw_.begin(), draw_.end());
//     combined.insert(combined.end(), discard_.begin(), discard_.end());

//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dis(0, combined.size() - 1);

//     auto it = combined.begin();
//     std::advance(it, dis(gen));
//     return *it;
// }

void Deck::ShuffleDraw() {
    std::shuffle(draw_.begin(), draw_.end(), gen_);
}

void Deck::NewDraw() {
    draw_.insert(draw_.end(), discard_.begin(), discard_.end());
    discard_.clear();
    ShuffleDraw();
}

Hand* Deck::DrawHand() {
    hand_->DiscardHand();
    while (hand_->DrawCard()) {
        // Keep drawing until the hand is full.
    }
    return hand_.get();
}

void Deck::AddCard(const std::shared_ptr<Card>& card) {
    discard_.push_back(card);
}

std::shared_ptr<Card> Deck::DrawCard() {
    if (draw_.empty()) {
        NewDraw();
    }
    if (draw_.empty() && discard_.empty()) {
        return nullptr;
    }
    std::shared_ptr<Card> drawn_card = draw_.back();
    draw_.pop_back();
    return drawn_card;
}

void Deck::DiscardCard(const std::shared_ptr<Card>& card) {
    discard_.push_back(card);
}

void Deck::RemoveCard(const std::shared_ptr<Card>& card) {
    auto remove_from_vector = [&card](std::vector<std::shared_ptr<Card>>& vec) {
        auto it = std::remove_if(vec.begin(), vec.end(),
                                 [&card](const std::shared_ptr<Card>& c) { return c.get() == card.get(); });
        vec.erase(it, vec.end());
    };

    remove_from_vector(draw_);
    remove_from_vector(discard_);
    // Note: Hand removal not implemented here.
}

} // namespace cards
