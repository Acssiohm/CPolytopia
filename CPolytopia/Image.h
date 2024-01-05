#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <string>

#include "IDs.h"
#include "Vec.h"
#include <array>


class Image {
public:
	Image();
	Image(SDL_Renderer* renderer, std::string file);

	bool is_valid() const;

	void draw(int x, int y, double scale = 1) const;
	void draw(int x, int y, int w, int h, double scale = 1) const;
	void draw(Vec2<int> p, double scale = 1) const;

	~Image();

	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;
	int m_height;
	int m_width;
};
