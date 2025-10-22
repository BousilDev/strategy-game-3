#pragma once

/**
 * @file hand.hpp
 * @brief Declares the Hand class which is the set of cards playable this turn by the player 
 * who owns the deck.
 */

#include <list>

#include <world/tile.hpp>

namespace cards {
  
/**
 * @brief Class representing a hand of playable cards a player has on their turn.
 * 
 * The hand is a part of every deck. At the beginning of a turn, a player draws cards to their hand until
 * the hand has cards equal to the hand size. Cards can be played from the hand to influence the game state.
 * At the end of the turn, the cards which have not been played are discarded so that the hand is empty.
 * A hand can be indexed into with hand[i].
 * 
 * @code
 * cards::Hand hand = deck.GetHand(); // some deck defined earlier
 * hand.DrawHand();
 * cards::Card first = hand[0];
 * hand.PlayCard(0);
 * hand.DiscardHand();
 * @endcode
 */
class Hand {
public:
  /**
   * @brief Construct a new Hand object.
   * 
   * @param starting_size The starting size of the hand.
   */
  Hand(unsigned int starting_size);

  /**
   * @brief Default hand constructor for testing purposes.
   */
  Hand() {}

  /**
   * @brief The size of the hand tells how many cards are drawn to the hand at the start of a turn.
   * 
   * @return The size of the hand.
   */
  int Size() const { return size_; }

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
  void DecreaseSize(unsigned int n) { size_ = std::max(0U, size_ - n); }

  /**
   * @brief Checks if the hand is full, meaning there are as many cards in the contents of the hand as
   * the size of the hand.
   * 
   * @return true 
   * @return false 
   */
  bool IsFull();

  /**
   * @brief Get the Card in the hand at index i. Throws std::out_of_range if the index is out of bounds.
   * 
   * @param i The index of the card in the hand.
   * @return The card at the index.
   */
  const Card& GetCard(int i) const;

  /**
   * @brief Get the cards contained in the hand.
   * 
   * @return The cards in a list.
   */
  const std::list<Card>& GetCards() const;

  /**
   * @brief Plays the card at index i in the hand on the target if possible. Throws std::out_of_range 
   * if the card index is out of bounds. Discards the card if it was successfully played.
   * 
   * @param i The index of the card in the hand.
   * @param target The tile the card will attempt to be played on.
   * @return true if the card is successfully played on the target tile, false otherwise.
   */
  bool PlayCard(int i, world::Tile& target);

  /**
   * @brief Adds the card to the current hand if there is sufficient room.
   * 
   * @param card The card to be added to the hand.
   * @return true if drawing the card was successful, false otherwise.
   */
  bool DrawCard(Card& card);

  /**
   * @brief Removes the card at index i from the hand. Throws std::out_of_range if the index is out of 
   * bounds.
   * 
   * @param i The index of the card to be removed.
   * @return The discarded card.
   */
  Card DiscardCard(int i);

  /**
   * @brief Removes all cards from the hand so that the contents_ of the hand becomes empty.
   * 
   * @return List containing the cards discarded.
   */
  std::list<Card> DiscardHand();

  /**
   * @brief Get the Card in the hand at index i. Throws std::out_of_range if the index is out of bounds.
   * 
   * @param i The index of the card.
   * @return The card at index i in the hand.
   */
  Card& operator[](int i);

private:
  unsigned int size_; ///< The size of the hand.
  std::list<Card> contents_; ///< The cards in the hand.
};
  
} // namespace cards
