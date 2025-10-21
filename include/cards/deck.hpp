#pragma once

/**
 * @file deck.hpp
 * @brief Declares the Deck class which manages and stores the playable cards of a player.
 */

#include "cards/hand.hpp"

namespace cards {
  
class Deck {
  public:
    Deck() {};
    Hand& GetHand() { return hand_; };
    void drawHand() {};
  private:
    Hand hand_;
};
  
} // namespace cards
