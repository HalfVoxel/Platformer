//
//  PhysicsSprite.h
//  Platformer
//
//  Created by Aron Granberg on 2013-05-13.
//
//

#ifndef __Platformer__PhysicsSprite__
#define __Platformer__PhysicsSprite__

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "ResourcePath.hpp"
#include "Box2D/Box2D.h"
#include "PhysicsSprite.h"

class PhysicsSprite : public sf::Drawable, public sf::Transformable {
	sf::Sprite sprite;

protected:
	PhysicsSprite *parent;
	std::vector<PhysicsSprite*> children;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
public:
	b2Body* body;
	
	PhysicsSprite() {
		body = NULL;
		parent = NULL;
	}
	
	PhysicsSprite(sf::Texture &tex) : sprite (tex){
		body = NULL;
		parent = NULL;
	}
	
	void addChild (PhysicsSprite *child) {
		if (child->parent != NULL) {
			child->parent->removeChild(child);
		}
		children.push_back(child);
		child->parent = this;
	}
	
	void removeChild (PhysicsSprite *child) {
		if (child->parent != this) return;
		
		child->parent = NULL;
		
		for (int i=0;i<children.size();i++) {
			if (children[i] == child) {
				children.erase(children.begin()+i);
				return;
			}
		}
		assert (false);
	}
	
	void drawVisual();
	
};
#endif /* defined(__Platformer__PhysicsSprite__) */
