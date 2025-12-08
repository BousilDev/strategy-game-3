#pragma once

/**
 * @file hand.hpp
 * @brief Declares the Hand class which is the set of cards playable this turn by the player 
 * who owns the deck.
 */

#include <iostream>
#include <vector>

#include <cards/card.hpp>
#include <world/tile.hpp>

namespace cards {

class Deck;
  
/**
 * @brief Class representing a hand of playable cards a player has on their turn.
 * 
 * The hand is a part of every deck. At the beginning of a turn, a player draws cards to their hand until
 * the hand has cards equal to the hand size. Cards can be played from the hand to influence the game state.
 * At the end of the turn, the cards which have not been played are discarded so that the hand is empty.
 */
class Hand {
public:
  /**
   * @brief Construct a new Hand object. The hand is initially empty.
   * 
   * @param deck The deck the hand draws cards from and discards them to.
   * @param starting_size The starting size of the hand.
   */
  Hand(Deck* deck, unsigned int starting_size)
    : deck_(deck), contents_(std::vector<std::shared_ptr<Card>>()) , size_(starting_size) {}

  /**
   * @brief Copies the contents of another hand into this hand.
   * 
   * @param other The other hand to copy contents from.
   */
  void CopyContentsFrom(const Hand& other) {
    contents_ = other.contents_;
  }


  /**
   * @brief The size of the hand tells how many cards are drawn to the hand at the start of a turn.
   * 
   * @return The size of the hand.
   */
  unsigned int Size() const { return size_; }

  /**
   * @brief Increases the size of the hand by n.
   * 
   * @param n how much larger the hand will become.
   */
  void IncreaseSize(unsigned int n) { size_ += n; }

  /**
   * @brief Decreases the size of the hand by n. Hand size cannot be less than 0.
   * 
   * @param n how much smaller the hand will become.
   */
  void DecreaseSize(unsigned int n) { size_ = (n > size_) ? 0 : size_ - n; }

  /**
   * @brief Checks if the hand is full, meaning there are as many cards in the contents of the hand as
   * the size of the hand.
   * 
   * @return true 
   * @return false 
   */
  bool IsFull() const { return contents_.size() == size_; }

  /**
   * @brief Get the Card in the hand at index i. Throws std::out_of_range if the index is out of bounds.
   * 
   * @param i The index of the card in the hand.
   * @return The card at the index.
   */
  const std::shared_ptr<Card>& GetCard(int i); 

  /**
   * @brief Get the cards contained in the hand.
   * 
   * @return The cards in a vector.
   */
  const std::vector<std::shared_ptr<Card>>& GetCards() const { return contents_; }

  /**
   * @brief Plays the card at index i in the hand on the target if possible. Throws std::out_of_range 
   * if the card index is out of bounds. Discards the card if it was successfully played.
   * 
   * @param card The card to be played.
   * @param target The tile the card will attempt to be played on.
   * @return true if the card is successfully played on the target tile, false otherwise.
   */
  bool PlayCard(std::shared_ptr<Card> card, std::shared_ptr<world::Tile> target, std::shared_ptr<core::Player> player);

  /**
   * @brief Adds a card from the deck to the current hand if there is sufficient room. If there are no cards
   * to draw, does nothing.
   * 
   * @return true if drawing the card was successful, false otherwise.
   */
  bool DrawCard();

  /**
   * @brief Removes the card at index i from the hand. Throws std::out_of_range if the index is out of 
   * bounds.
   * 
   * @param card The card to be discarded.
   */
  void DiscardCard(std::shared_ptr<Card> card);

  /**
   * @brief Removes all cards from the hand so that the contents_ of the hand becomes empty. Places the cards
   * in the discard pile of the deck.
   */
  void DiscardHand();

  friend std::istream& operator>>(std::istream& in,  std::shared_ptr<Hand>& other);
  friend std::ostream& operator<<(std::ostream& out, const std::shared_ptr<Hand>& other);

private:
  unsigned int size_; ///< The size of the hand.
  std::vector<std::shared_ptr<Card>> contents_; ///< The cards in the hand.
  Deck* deck_; ///< The deck of cards the hand draws from and discards to.
};
  
} // namespace cards
