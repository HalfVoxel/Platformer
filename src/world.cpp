
#include "Box2D/Box2D.h"
#include "world.h"
#include<iostream>
#include "PhysicsSprite.h"
#include "SFML/Graphics.hpp"

void world::CreateWorld () {
	b2Vec2 gravity(0.0f,-10.0f);

	b2World w(gravity);
	
	b2BodyDef groundDef;
	groundDef.position.Set(0, 0);

	b2Body* groundBody = w.CreateBody(&groundDef);
	
	
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50, 5);
	groundBody->CreateFixture(&groundBox,0.0f);
	
	
	sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(resourcePath() + "ground.png")) {
        throw 1;
    }
	texture->setSmooth(true);
	
	sf::Texture* heatTexture = new sf::Texture();
    if (!heatTexture->loadFromFile(resourcePath() + "heat1.png")) {
        throw 1;
    }
	heatTexture->setSmooth(true);
	
	root.setScale(worldScale, worldScale);
	root.setRotation(180);
	
	PhysicsSprite *s1 = new PhysicsSprite(*texture, *heatTexture);
	s1->body = groundBody;
	
	world::activeWorld->root.addChild(s1);
}

world* world::activeWorld = NULL;

world::world () {
	worldScale = 20.0f;
}

world::~world () {

}

void world::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(root);
}