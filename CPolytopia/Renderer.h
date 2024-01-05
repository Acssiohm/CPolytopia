#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "IDs.h"
#include "Unit.h"

#include <string>
#include <iostream>
#include "Vec.h"
#include "Map.h"
#include "ImageStore.h"
#include "Image.h"
#include <set>
#include <SDL2/SDL_render.h>
#include "TechTree.h"

namespace Renderer {
	extern SDL_Window* window;
	extern SDL_Renderer* renderer;
	extern ImageStore* images;

	extern Vec2<int> tile_size;
	extern Vec2<double> cloud_shift;

	void Init();
	void loadFont(int font_size);
	void update();
	void clear();
	void Quit();
	Vec2<int> map_to_cartesian(int i, int j);
	Vec2<int> map_to_cartesian(Vec2<int> pos);
	Vec2<int> cartesian_to_map(int x, int y);
	Vec2<int> cartesian_to_map(Vec2<int> pos);
	void renderImg(Image * img, int i, int j);

	template<typename T_ID>
	void render(T_ID tid, int i, int j) {
		if (tid == T_ID::None) {
			return;
		}
		renderImg(images->get_image(tid), i, j);
	}
	
	typedef std::tuple<int, int> point;
	typedef std::tuple<Vec2<int>, Vec2<int>> segment;
	typedef std::vector<Vec2<int>> path;
	typedef std::vector<Vec2<float>> Fpath;

	void renderMap(const Map &carte, const Player& p, const std::set<point>& squares_selected = {});
	void zoom(float zoom_scale, Vec2<int> centre = Vec2<int>(0, 0));
	void move(int x, int y);
	void find_contour(const std::set<point>& squares, path& out_bords_coin, path& out_cases_coin);
	void convert_coins_to_bord_limits(const path& bords_coin, const path& cases_coin, Fpath& limit_ext, Fpath& limit_int, float bord_width_ratio);
	std::vector<SDL_FPoint> generate_triangles_thick_paths(const Fpath& path_side1, const Fpath& path_side2);
	void draw_triangles(const std::vector<SDL_FPoint>& triangles);
	void draw_contour(const std::set<point>& squares, float ratio = .2f);
	void drawTechTree(const TechTree* ttree, Vec2<double> pos);
}
