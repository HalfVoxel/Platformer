//
//  ResourceManager.h
//  Platformer
//
//  Created by Aron Granberg on 2013-05-14.
//
//

#ifndef __Platformer__ResourceManager__
#define __Platformer__ResourceManager__

#include <iostream>
#include <map>
#include<string>
#include<SFML/Graphics.hpp>
#include "ResourcePath.hpp"

class ResourceManager {
	
	static std::map<std::string,sf::Texture> textures;
	
public:
	
	static void init () {
		
	}
	
	static sf::Texture getTexture (std::string id) {
		return textures[id];
	}
};

#endif /* defined(__Platformer__ResourceManager__) */
