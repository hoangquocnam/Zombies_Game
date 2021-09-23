#pragma once
#ifndef TEXTUREHOLDER_H
#define TEXTUREHOLDER_H

#include <SFML/Graphics.hpp>
#include<map>

using namespace sf;
using namespace std;


class TextureHolder {
private:
	map <string, Texture> maptextures;
	static TextureHolder* m_Instance;

public:
	TextureHolder();
	static Texture& getTexture(string const& filename);

};


#endif // !TEXTUREHOLDER_H

