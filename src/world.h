
#ifndef _WORLD_H
#define _WORLD_H

#include "SFML/Graphics.hpp"
#include "PhysicsSprite.h"

enum RenderMode {
	RenderVisual,
	RenderMeta
};

class world : public sf::Drawable
{
public:
	static world* activeWorld;
	
	float worldScale;
	sf::RenderTarget* activeRenderTarget;
	
	RenderMode renderMode;
	
	PhysicsSprite root;
	
	world();

	~world();

	void CreateWorld();
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif