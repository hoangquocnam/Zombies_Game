#include"TextureHolder.h"
#include<map>
#include<assert.h>

using namespace sf;

TextureHolder* TextureHolder::m_Instance = nullptr;

TextureHolder::TextureHolder() {
	assert(m_Instance == nullptr);
	m_Instance = this;
}

Texture& TextureHolder::getTexture(string const& filename) {
	auto & m = m_Instance->maptextures;
	auto kvp = m.find(filename);

	if (kvp != m.end()) {
		return kvp->second;
	}
	else {
		auto& txture = m[filename];
		txture.loadFromFile(filename);
		return txture;
	}

}