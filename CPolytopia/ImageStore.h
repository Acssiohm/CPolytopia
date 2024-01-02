#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <string>

#include "ResourcesIds.h"
#include "Vec.h"
#include <array>
//#include <assert.h>
 

class ImageStore {
public:
	ImageStore(SDL_Renderer* renderer, std::string directory) : directory(directory), M_renderer(renderer), id_image{ nullptr } {}

	class Image {
	public:
		Image();
		Image(SDL_Renderer* renderer, std::string file);

		bool is_valid() const ;

		void draw(int x, int y, double scale = 1) const ;
		void draw(int x, int y, int w, int h, double scale = 1) const ;
		void draw(Vec2<int> p, double scale = 1) const ;
		
		~Image();

		SDL_Renderer* m_renderer;
		SDL_Texture* m_texture;
		int m_height;
		int m_width;
	};

	template < typename T_ID >
	void add_image(std::string file, T_ID  id) {
		if (idof(id) < 0) {
			std::cout << "Invalid id" << std::endl;
			return;
		}
		SDL_assert(id_image[idof(id)] == nullptr);
		id_image[idof(id)] = new Image(M_renderer, directory + file);
	}

	template < typename T_ID >
	Image* get_image(T_ID id) {
		SDL_assert(id_image[idof(id)] != nullptr);
		return id_image[idof(id)];
	}

	~ImageStore();

private:
	std::array<Image*, numberOfIds> id_image;
	std::string directory;
	SDL_Renderer* M_renderer;

};

