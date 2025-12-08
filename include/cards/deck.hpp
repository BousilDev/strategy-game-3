#pragma once

/**
 * @file deck.hpp
 * @brief Declares the Deck class which manages and stores the playable cards of a player.
 */

#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "cards/card.hpp"
#include "cards/hand.hpp"

namespace cards {

/**
 * @brief Class representing a deck of cards a player has. Each player has one deck which can be 
 * modified during the game.
 * 
 * The deck holds all the cards of a single player. At the start of each turn a hand is drawn from the
 * draw pile of the deck. At the end of the turn the remaining cards are discarded to the discard pile.
 * When the draw pile runs out of cards, the discard pile is shuffled to obtain a new draw pile. Cards
 * can be added to and removed from the deck during the game.
 */
class Deck {
public:
  /**
   * @brief Construct a new Deck object.
   * 
   * @param cards The cards initially in the deck. They start in the draw pile in pseudo-random order.
   * @param hand_size The initial hand size of the deck.
   */
  Deck(const std::vector<std::shared_ptr<Card>>& cards, unsigned int hand_size);

  /**
   * @brief Clone the deck including its current draw and discard piles. The hand is not cloned.
   * 
   * @return A copy of the deck.
   */
  std::shared_ptr<Deck> Clone() const;

  /**
   * @brief Discard the previous hand and draw a new full hand.
   * 
   * @return A pointer to the new hand of the deck.
   */
  Hand* DrawHand();

  Hand* GetHand() const { return hand_.get(); }

  /**
   * @brief Get the Hand of the deck.
   * 
   * @return The hand of the deck.
   */
  Hand* GetHand() { return hand_.get(); }

  /**
   * @brief Get the draw pile of the deck.
   * 
   * @return A vector of cards in the draw pile.
   */
  const std::vector<std::shared_ptr<Card>>& GetDraw() const { return draw_; }

  /**
   * @brief Get the discard pile of the deck.
   * 
   * @return A vector of cards in the discard pile.
   */
  const std::vector<std::shared_ptr<Card>>& GetDiscard() const { return discard_; }

  /**
   * @brief Returns a pseudo-randomly picked card from the entire deck.
   * 
   * Used to give pseudo-randomized deck alteration options to the player. For example, choose one of these
   * three cards to remove from your deck.
   * 
   * @return A random card from the deck.
   */
  // Unimplemented for now.
  // std::shared_ptr<Card>& GetRandomCard() const;

  /**
   * @brief Pseudo-randomizes the order of the cards in the draw pile.
   */
  void ShuffleDraw();

  /**
   * @brief Places all cards from the discard pile into the draw pile and shuffles the draw pile.
   */
  void NewDraw();

  /**
   * @brief Adds the given card to the discard pile of the deck.
   * 
   * @param card The card to add.
   */
  void AddCard(const std::shared_ptr<Card>& card);

  /**
   * @brief Draws a card from the draw pile. If the draw pile is empty, shuffles the discard pile
   * into the draw pile first.
   * 
   * @return The drawn card. If both the draw and discard piles are empty, returns nullptr.
   */
  std::shared_ptr<Card> DrawCard();

  /**
   * @brief Discards the given card to the discard pile.
   * 
   * @param card The card to discard.
   */
  void DiscardCard(const std::shared_ptr<Card>& card);

  /**
   * @brief Removes the given card from the deck. If the card is not in the draw pile or discard pile does nothing.
   * Does not remove a card from the hand.
   * 
   * @param card The card to remove from the hand.
   */
  void RemoveCard(const std::shared_ptr<Card>& card);

  friend std::istream& operator>>(std::istream& in,  std::shared_ptr<Deck>& other);
  friend std::ostream& operator<<(std::ostream& out, const std::shared_ptr<Deck>& other);

private:
  std::shared_ptr<Hand> hand_; ///< The hand of the deck
  std::vector<std::shared_ptr<Card>> draw_; ///< The draw pile of the deck.
  std::vector<std::shared_ptr<Card>> discard_; ///< The discard pile of the deck.
  std::random_device rd_;
  std::mt19937 gen_; 
};
  
} // namespace cards
