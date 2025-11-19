#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

//TODO: currently unused
namespace ui {

// Base class for a generic renderer object.
class Renderer {
public:

    // Draw the sprites etc
    void DrawTo(sf::RenderWindow& window);

    //TODO: implement if you have time, this would improve performance by reducing draw calls
    // return a VertexArray containing the vertices that will be drawn
    //sf::VertexArray VerticesToDraw();

private:

};

} // namespace ui