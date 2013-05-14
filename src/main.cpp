
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
#include <iostream>
int main(int, char const**)
{
    // Create the main window
	sf::ContextSettings ctx;
	ctx.antialiasingLevel = 8;
	ctx.majorVersion = 1;
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
	
	sf::RenderTexture renderTex;
	if (!renderTex.create(window.getSize().x, window.getSize().y)) throw 1;
	
	sf::RenderTexture renderTex2;
	if (!renderTex2.create(window.getSize().x, window.getSize().y)) throw 1;
	
	int counter = 0;
	
	sf::Shader shader;
	shader.loadFromFile(resourcePath() + "generic.vert", sf::Shader::Vertex);
	shader.loadFromFile(resourcePath() + "blur.frag", sf::Shader::Fragment);
	
	sf::Shader shader2;
	shader2.loadFromFile(resourcePath() + "generic.vert", sf::Shader::Vertex);
	shader2.loadFromFile(resourcePath() + "blur2.frag", sf::Shader::Fragment);
	
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
			
			if (event.type == sf::Event::Resized) {
				
				//Recreate buffers
				if (!renderTex2.create(event.size.width, event.size.height)) return 1;
				if (!renderTex.create(event.size.width, event.size.height)) return 1;
				
				window.setView(sf::View(sf::FloatRect(0,0,event.size.width, event.size.height)));
				//window.setSize(sf::Vector2u(event.size.width, event.size.height));
			}
        }
		
		//FIRST PASS - Main Render Pass
		w->activeRenderTarget = &renderTex;
        // Clear screen
        w->activeRenderTarget->clear();

        // Draw the sprite
        //window.draw(sprite);
        
		ps.drawVisual();
		
		w->root.setPosition(window.getSize().x/2, window.getSize().y/2);
		//w->root.setPosition(time.asMilliseconds()*0.001, (time.asMilliseconds()*7)*0.001 );
		w->activeRenderTarget->draw(*world::activeWorld);
		
        // Draw the string
        w->activeRenderTarget->draw(text);
		
        // Update the window
        ((sf::RenderTexture*)(w->activeRenderTarget))->display();
		
		//SECOND PASS
		w->activeRenderTarget = &renderTex2;
		
		sf::RenderStates state;
		state.shader = &shader;
		//shader.setParameter("MainTexture", renderTex.getTexture());
		shader.setParameter("width", renderTex.getSize().x);
		shader.setParameter("sigma", 3);
		shader.setParameter("glowMultiplier", 1.5);
		shader.setParameter("blurSize", 3);
		sf::Sprite fullrect (renderTex.getTexture());
		
		w->activeRenderTarget->draw(fullrect, state);
		renderTex2.display();
		
		//THIRD PASS
		w->activeRenderTarget = &window;
		
		window.clear(sf::Color(0,0,0,0));
		
		state = sf::RenderStates();
		state.shader = &shader2;
		shader2.setParameter("MainTexture", renderTex2.getTexture());
		shader2.setParameter("width", renderTex2.getSize().y);
		shader2.setParameter("sigma", 3);
		shader2.setParameter("glowMultiplier", 1.5);
		shader2.setParameter("blurSize", 3);
		fullrect = sf::Sprite(renderTex2.getTexture());
		
		w->activeRenderTarget->draw(fullrect, state);
		
		window.display();
    }
    
    return EXIT_SUCCESS;
}
