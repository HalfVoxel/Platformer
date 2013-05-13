
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Here is a small helper for you ! Have a look.
//#include "ResourcePath.hpp"

#include "ResourcePath.hpp"
#include "world.h"
#include "PhysicsSprite.h"
#include <string>
#include <sstream>

int main(int, char const**)
{
    // Create the main window
	sf::ContextSettings ctx;
	ctx.antialiasingLevel = 4;
	ctx.majorVersion = 3;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Platformer", sf::Style::Default, ctx);
	
	//window.setVerticalSyncEnabled(true);
	
    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile(resourcePath() + "cute_image.jpg")) {
        return EXIT_FAILURE;
    }
    //sf::Sprite sprite(texture);

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text text("FPS: ", font, 50);
    text.setColor(sf::Color::Black);

    // Load a music to play
    sf::Music music;
    if (!music.openFromFile(resourcePath() + "nice_music.ogg")) {
        return EXIT_FAILURE;
    }
	
    // Play the music
    music.play();

    world* w = new world();
	world::activeWorld = w;
	
    w->CreateWorld();

	
	PhysicsSprite ps(texture);// = new PhysicsSprite(texture);
	
	sf::Clock clock;
	
	sf::Time prevTime = clock.getElapsedTime();
	
	int counter = 0;
    // Start the game loop
    while (window.isOpen())
    {
		counter++;
		sf::Time time = clock.getElapsedTime();
		sf::Time delta = time - prevTime;
		prevTime = time;
		
		double deltaTime = delta.asMicroseconds()/1000000.0;
		
		if (counter % 1000 == 0) {
			std::ostringstream strs;
			strs.precision(0);
			strs.setf(std::ios::fixed, std::ios::floatfield);
			strs << "FPS: " << (1.0/deltaTime);
			text.setString(strs.str());
		}
		
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Espace pressed : exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
		
		w->activeRenderTarget = &window;
        // Clear screen
        window.clear();

        // Draw the sprite
        //window.draw(sprite);
        
		ps.drawVisual();
		
		w->root.setPosition(time.asMilliseconds()*0.001, (time.asMilliseconds()*7)*0.001 );
		window.draw(*world::activeWorld);
		
        // Draw the string
        window.draw(text);
		
        // Update the window
        window.display();
    }
    
    return EXIT_SUCCESS;
}
