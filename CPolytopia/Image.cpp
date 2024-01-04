#include "Image.h"
#include <assert.h>
Image::Image() : m_renderer(nullptr), m_texture(nullptr), m_height(0), m_width(0) {}

Image::Image(SDL_Renderer* renderer, std::string file) {
	SDL_Surface* surface = IMG_Load(file.c_str());
	if (surface == nullptr) {
		std::cout << file << " est introuvable" << std::endl;
		assert(false);
	}
	m_height = surface->h;
	m_width = surface->w;
	m_texture = SDL_CreateTextureFromSurface(renderer, surface);
	assert(m_texture != nullptr);
	m_renderer = renderer;
	SDL_FreeSurface(surface);
}

bool Image::is_valid() const {
	return m_texture != nullptr;
}

void Image::draw(int x, int y, int w, int h, double scale) const {
	assert(m_texture != nullptr);
	SDL_Rect dest_rect{};
	dest_rect.x = x;
	dest_rect.y = y;
	dest_rect.w = (int)(w * scale);
	dest_rect.h = (int)(h * scale);
	SDL_RenderCopy(m_renderer, m_texture, NULL, &dest_rect);
}
void Image::draw(int x, int y, double scale) const {
	draw(x, y, m_width, m_height, scale);
}
void Image::draw(Vec2<int> p, double scale) const {
	draw(p.x(), p.y(), scale);
}

Image::~Image() {
	SDL_DestroyTexture(m_texture);
}