
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
#include <algorithm>
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
	
	sf::Text fpstext("FPS: ", font, 18);
	
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

	
	//PhysicsSprite ps(texture);// = new PhysicsSprite(texture);
	
	sf::Clock clock;
	
	sf::Time prevTime = clock.getElapsedTime();
	
	sf::RenderTexture *renderTex= new sf::RenderTexture();
	if (!renderTex->create(window.getSize().x, window.getSize().y)) throw 1;
	
	sf::RenderTexture *renderTex2 = new sf::RenderTexture();
	if (!renderTex2->create(window.getSize().x, window.getSize().y)) throw 1;
	
	int counter = 0;
	
	sf::Shader shader;
	shader.loadFromFile(resourcePath() + "generic.vert", sf::Shader::Vertex);
	shader.loadFromFile(resourcePath() + "heat.frag", sf::Shader::Fragment);
	
	sf::Shader shader2;
	shader2.loadFromFile(resourcePath() + "generic.vert", sf::Shader::Vertex);
	shader2.loadFromFile(resourcePath() + "heatVisual.frag", sf::Shader::Fragment);
	
    sf::Vector2f p1 (0.5,0.5);
    sf::Vector2f p2 (1,0);

    float lambda1 = 0.1f;
    float lambda2 = 0.1f;

    float str1 = 1.0f;
    float str2 = 1.0f;

    float timeScale = 1;
	
	renderTex->clear();
	renderTex2->clear();
	
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
			fpstext.setString(strs.str());
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
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::W:
                        lambda1 += 0.1*deltaTime;
                        break;
                    case sf::Keyboard::Q:
                        lambda1 -= 0.1*deltaTime;
                        break;
                    case sf::Keyboard::S:
                        lambda2 += 0.1*deltaTime;
                        break;
                    case sf::Keyboard::A:
                        lambda2 -= 0.1*deltaTime;
                        break;
                    case sf::Keyboard::T:
                        p1.y += 0.7*deltaTime;
                        break;
                    case sf::Keyboard::G:
                        p1.y -= 0.7*deltaTime;
                        break;
                    case sf::Keyboard::H:
                        p1.x += 0.7*deltaTime;
                        break;
                    case sf::Keyboard::F:
                        p1.x -= 0.7*deltaTime;
                        break;
                }

                str1 += 0.7*deltaTime * (event.key.code == sf::Keyboard::X ? 1 : (event.key.code == sf::Keyboard::Z ? -1 : 0));
                str2 += 0.7*deltaTime * (event.key.code == sf::Keyboard::V ? 1 : (event.key.code == sf::Keyboard::C ? -1 : 0));

                timeScale += 10*deltaTime * (event.key.code == sf::Keyboard::P ? 1 : (event.key.code == sf::Keyboard::O ? -1 : 0));
            }
			
			if (event.type == sf::Event::Resized) {
				
				//Recreate buffers
				if (!renderTex2->create(event.size.width, event.size.height)) return 1;
				if (!renderTex->create(event.size.width, event.size.height)) return 1;
				
				window.setView(sf::View(sf::FloatRect(0,0,event.size.width, event.size.height)));
				//window.setSize(sf::Vector2u(event.size.width, event.size.height));
			}
        }
		
		/*
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
		
		 */
		
		sf::RenderStates state;
		
		std::swap(renderTex,renderTex2);
		
		//FIRST PASS - Main Render Pass
		w->activeRenderTarget = renderTex;
        // Clear screen
        //w->activeRenderTarget->clear();
		w->activeWorld->renderMode = RenderMeta;
		w->root.setPosition(window.getSize().x*p1.x, window.getSize().y*p1.y);
		
		state = sf::RenderStates();
		state.blendMode = sf::BlendAdd;
		//state.shader = &shader3;
		//shader3.setParameter("MainTexture", sf::Shader::CurrentTexture);
		
		w->activeRenderTarget->draw(*world::activeWorld, state);
		
		// Update the window
        ((sf::RenderTexture*)(w->activeRenderTarget))->display();
		
		//SECOND PASS
		w->activeRenderTarget = renderTex2;
		
		state = sf::RenderStates();
		state.shader = &shader;
		shader.setParameter("MainTexture", renderTex->getTexture());
		shader.setParameter("width", renderTex->getSize().x);
		shader.setParameter("height", renderTex->getSize().y);
		shader.setParameter("sigma", 2);
		shader.setParameter("glow", 1.0);
		sf::Sprite fullrect (renderTex->getTexture());
		
		w->activeRenderTarget->draw(fullrect, state);
		renderTex2->display();
		
		//THIRD PASS
		w->activeRenderTarget = &window;
		
		window.clear(sf::Color(0,0,0,0));
		
		state = sf::RenderStates();
		state.shader = &shader2;
		shader2.setParameter("MainTexture", renderTex2->getTexture());
		fullrect = sf::Sprite(renderTex2->getTexture());
		
		w->activeRenderTarget->draw(fullrect, state);
		
        std::ostringstream strs;
        strs.precision(3);
        strs.setf(std::ios::fixed, std::ios::floatfield);
        strs << "P1: (" << p1.x << ", " << p1.y << ")\nP2: (" << p2.x << ", " << p2.y << ")\nF1: " << lambda1 << "\nF2: " << lambda2;
        strs << "\nMagn1: " << str1 << ", Magn2: " << str2 << "\n";
        text.setString(strs.str()); 
        text.setColor(sf::Color::Red);
        w->activeRenderTarget->draw(text);
		fpstext.setPosition(window.getSize().x-100, 5);
		w->activeRenderTarget->draw(fpstext);
		
		window.display();
    }
    
    return EXIT_SUCCESS;
}
