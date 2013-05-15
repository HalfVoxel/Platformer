//
//  PhysicsSprite.cpp
//  Platformer
//
//  Created by Aron Granberg on 2013-05-13.
//
//

#include "PhysicsSprite.h"
#include "world.h"

void PhysicsSprite::drawVisual() {

	if (body != NULL) {
		b2Transform tr = body->GetTransform();
		tr.p *= world::activeWorld->worldScale;
		sprite.setPosition(tr.p.x, tr.p.y);
		sprite.setRotation(tr.q.GetAngle());
	}
	
	world::activeWorld->activeRenderTarget->draw(sprite);
}

void PhysicsSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	
	sf::Transform m = states.transform;
	
	states.transform *= getTransform();
	
	sf::Transform m2 = states.transform;
	states.transform.scale(1.0f/world::activeWorld->worldScale, 1.0f/world::activeWorld->worldScale);
	if (world::activeWorld->renderMode == RenderVisual) {
		target.draw(sprite, states);
	} else {
		target.draw(metaSprite, states);
	}
	
	states.transform = m2;
	for (int i=0;i<children.size();i++) {
		target.draw(*children[i], states);
	}
	
	states.transform = m;
	
}