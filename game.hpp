#ifndef GAME
#define GAME

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "texturemanager.hpp"

class Game {
private:
    sf::Clock clock;
    sf::RenderWindow window;
    TextureManager textures;

    void loadTextures();

public:
	Game();
	~Game();

	void loop();
};

#endif // GAME
