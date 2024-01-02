#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "ResourcesIds.h"
#include "Unit.h"

#include <string>
#include <iostream>
#include "Vec.h"
#include "Map.h"
#include "ImageStore.h"
#include <set>
#include <SDL2/SDL_render.h>

namespace Renderer {
	extern SDL_Window* window;
	extern SDL_Renderer* renderer;
	extern ImageStore* images;

	extern Vec2<int> tile_size;

	void Init();
	void loadFont(int font_size);
	void update();
	void clear();
	void Quit();
	Vec2<int> map_to_cartesian(int i, int j);
	Vec2<int> map_to_cartesian(Vec2<int> pos);
	void renderImg(ImageStore::Image * img, int i, int j);

	template<typename T_ID>
	void render(T_ID tid, int i, int j) {
		if (tid == T_ID::None) {
			return;
		}
		renderImg(images->get_image(tid), i, j);
	}
	
	void renderMap(Map &carte);
	void zoom(float zoom_added, Vec2<int> centre = Vec2(0,0));
	void move(int x, int y);
	void draw_contour(const std::set<std::tuple<int, int> >& squares, float ratio = .2);
	std::vector<SDL_FPoint> contour_triangule(const std::set<std::tuple<int, int> >& squares, float ratio = .2);

}
