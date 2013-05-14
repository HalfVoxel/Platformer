
#include "box2d.h"
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
	
	root.setScale(worldScale, worldScale);
	
	PhysicsSprite *s1 = new PhysicsSprite(*texture);
	s1->body = groundBody;
	
	world::activeWorld->root.addChild(s1);
}

world* world::activeWorld = NULL;

world::world () {
}

world::~world () {

}

void world::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(root);
}