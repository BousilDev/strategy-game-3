#pragma once
#include <SFML/Graphics.hpp>

// Helper function for creating UI. Takes a drawable shape as input and sets 
// the local origin to the center of the shape.

template<typename T>
void centerOrigin(T& shape) {
    sf::FloatRect bounds = shape.getLocalBounds();
    shape.setOrigin(bounds.left + bounds.width / 2.0f,
                    bounds.top + bounds.height / 2.0f);
}

