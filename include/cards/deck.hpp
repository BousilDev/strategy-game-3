#pragma once

/**
 * @file deck.hpp
 * @brief Declares the Deck class which manages and stores the playable cards of a player.
 */

#include <list>

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
   * @param cards The cards initially in the deck. Go in the draw pile.
   * @param hand_size The initial hand size of the deck.
   */
  Deck(std::list<Card> cards, unsigned int hand_size);

  /**
   * @brief Get the Hand of the deck.
   * 
   * @return The hand of the deck.
   */
  Hand& GetHand() { return hand_; };

  /**
   * @brief Get the draw pile of the deck.
   * 
   * @return A list of cards in the draw pile.
   */
  const std::list<Card>& GetDraw() const;

  /**
   * @brief Get the discard pile of the deck.
   * 
   * @return A list of cards in the discard pile.
   */
  const std::list<Card>& GetDiscard() const;

  /**
   * @brief Get all cards in the deck.
   * 
   * @return A list of all the cards the deck contains.
   */
  const std::list<Card>& GetCards() const;

  /**
   * @brief Returns a pseudo-randomly picked card from the entire deck.
   * 
   * Used to give pseudo-randomized deck alteration options to the player. For example, choose one of these
   * three cards to remove from your deck.
   * 
   * @return A random card from the deck.
   */
  const Card& GetRandomCard() const;

  /**
   * @brief Draws a card from the draw pile of the deck into the hand. 
   * 
   * If the draw pile is empty, tries to shuffle the discard pile and make a new draw pile from that. 
   * If successful, draws a card from this new draw pile. If the discard pile is also empty, simply 
   * returns false. Also returns false and does not draw a card if the hand is full.
   * 
   * @return true if the card was successfully drawn, false otherwise.
   */
  bool DrawCard();

  /**
   * @brief Draws cards to the hand until it is full or there are no cards to draw.
   */
  void DrawHand();

  /**
   * @brief Discards the card at index i from the hand and places it in the discard pile. If there is no
   * card at index i in the hand, throws an exception.
   * 
   * @param i The index of the card to discard.
   */
  void DiscardCard(int i);

  /**
   * @brief Discards the entire hand, placing all cards in the discard pile.
   */
  void DiscardHand();

  /**
   * @brief Pseudo-randomizes the order of the cards in the discard pile.
   */
  void ShuffleDiscard();

  /**
   * @brief Places all cards from the discard pile into the draw pile.
   */
  void NewDraw();

  /**
   * @brief Adds the given card to the draw pile of the deck.
   * 
   * @param card The card to add.
   */
  void AddCard(Card& card);

  /**
   * @brief Removes the given card from the deck. If the card is not in the draw pile, discard pile, or
   * hand, throws an exception.
   * 
   * @param card The card to remove from the hand.
   */
  void RemoveCard(Card& card);
  
private:
  Hand hand_; ///< The hand of the deck
  std::list<Card> draw_; ///< The draw pile of the deck.
  std::list<Card> discard_; ///< The discard pile of the deck.
};
  
} // namespace cards
