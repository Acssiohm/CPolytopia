#include "ImageStore.h"
#include <iostream>
#include <assert.h> 

ImageStore::Image::Image() : m_renderer(nullptr), m_texture(nullptr) , m_height(0), m_width(0) {}

ImageStore::Image::Image(SDL_Renderer* renderer, std::string file) {
	SDL_Surface* surface = IMG_Load(file.c_str());
	m_height = surface->h;
	m_width = surface->w;
	m_texture = SDL_CreateTextureFromSurface(renderer, surface);
	assert(m_texture != nullptr);
	m_renderer = renderer;
	SDL_FreeSurface(surface);
}

bool ImageStore::Image::is_valid() const {
	return m_texture != nullptr;
}

void ImageStore::Image::draw(int x, int y, int w, int h, double scale) const {
	assert(m_texture != nullptr);
	SDL_Rect dest_rect{};
	dest_rect.x = x;
	dest_rect.y = y;
	dest_rect.w = (int)(w*scale);
	dest_rect.h = (int)(h*scale);
	SDL_RenderCopy(m_renderer, m_texture, NULL, &dest_rect);
}
void ImageStore::Image::draw(int x, int y, double scale) const {
	draw(x, y,m_width, m_height, scale);
}
void ImageStore::Image::draw(Vec2<int> p, double scale) const {
	draw(p.x(), p.y(), scale);
}

ImageStore::Image::~Image() {
	SDL_DestroyTexture(m_texture);
}

ImageStore::~ImageStore() {
	for (Image* im : id_image) {
		if (im != nullptr) {
			SDL_assert(im->is_valid());
			delete im;
		}
	}
}