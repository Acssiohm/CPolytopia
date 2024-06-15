#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <SDL2/SDL_render.h>

#include <string>
#include <iostream>
#include <set>

#include "IDs.h"
#include "Unit.h"
#include "Vec.h"
#include "Map.h"
#include "ImageStore.h"
#include "Image.h"
#include "TechTree.h"

namespace Renderer {
	extern SDL_Window* window;
	extern SDL_Renderer* renderer;
	extern ImageStore* images;

	extern Vec2<int> tile_size;
	extern Vec2<double> cloud_shift;

	// Load and unload game resources
	void Init();
	void Quit();
	
	// Update the screen with what has been drawn 
	void update();
	// Clear the screen ( make it all black ) without modifying the drawing color
	void clear();
	
	// Conversions between the position on the map and the position on the screen ( according to the top point of the tile ) 
	Vec2<int> map_to_cartesian(int i, int j);
	Vec2<int> map_to_cartesian(Vec2<int> pos);
	Vec2<int> cartesian_to_map(int x, int y);
	Vec2<int> cartesian_to_map(Vec2<int> pos);

	// Zooms with a factor of zoom_scale and centered on centre
	void zoom(float zoom_scale, Vec2<int> centre = Vec2<int>(0, 0));
	// Moves by (x,y) in pixel coordinates ( not map coordinates ) 
	void move(int x, int y);

	// Renders the image pointed by img the map position (i, j) , reference id at top left of the image and top of the tile 
	void renderImg(Image * img, int i, int j);

	// Render the different types of tile content ( Resources, Terrain, Units...etc. )
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

	/* Representaion of a border in the map
	* A border of a tile can be identified by the intersection and the square it borders/follows 
	* so we can store it as two positions on the map: 
	* 
	*	     0  0  1  1  2  2  3  3  4  4  5  5  6
	*		0+-----+-----+-----+-----+-----+-----+
	*		 |     |     * ooo |     |     |     |
	*		 |     |     * ooo |     |     |     |
	*		1+-----+-----@***--+-----+-----+-----+
	*		 |     |     |     |     |     |     |
	*		 |     |     |     |     |     |     |
	*		2+-----+-----+-----+-----+-----+-----+
	*		 |     |     |     |     |     |     |
	*		 |     |     |     |     |     |     |
	*		3+-----+-----+-----+-----+-----+-----+
	* 
	* The border with stars "*" can be identified by the intersection @ and the square with "o"s 
	* @ is at (2,1) and the "o"s are at (2,0) so the border is ((2,1), (2,0)) it was more practicle to store them in two 
	* different arrays as we will be dealing with an array of borders but the algorithm using was simpler if we could 
	* change intersection and center square independently  
	* 
	* PS : another way could've been an intersection and a relative position : TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT
	* but it wasn't as practicle
	*/
	struct border_list {
		path intersections;
		path centers;

		void verify_sizes() const ;
		size_t size() const;
	};

	// Renders the map carte on the the player p perspective  
	void renderMap(const Map &carte, const Player& p);
	// Renders units on the map
	void renderUnits(const Map& carte);
	// Renders the technologie tree  in "tree" with the central point being at "pos"
	void drawTechTree(const TechTree* ttree, Vec2<double> pos);
	TechTree generateTestTechTree();
	void renderTestTree();
	// Renders the outline of the squares with map coordinates in "squares" with a border width of ratio*(size of a tile)
	void draw_contour(const std::set<point>& squares, float ratio = .2f);
	// Combine all of the above with the map is map_vs_tree is true and the tree if it is false 
	void renderAll(const Map &carte, const Player& p, const std::set<point>& squares_selected = {}, bool map_vs_ttree = true);
	
	/* Find the borders of the shape described by the set of squares and push them in out_borders in the order of a path on the outlines
	* The outline of a connected shape end when a border is repeated :
	* example : b1 - b2 - b3 - b1 - b4 - b5 - b6 - b4 means we have 
	* outline 1 : b1-b2-b3-b1 and outline 2 : b4-b5-b6-b4 , notice an outline is like a closed path, it has to end where in began.  
	*/
	void find_contour(const std::set<point>& squares, border_list& out_borders);
	
	/* Takes an array of borders and makes a thick path ( the area between limit_int and limit_ext ) wich connects those borders
	* with a thikness of bord_width_ratio*(size of a square tile)
	* The path doesnt cross itself so when a border repeats we start again at the next border without connecting them, like what you would 
	* expect with the output of find_contour
	*/ 
	void convert_coins_to_bord_limits(const border_list& borders, Fpath& limit_ext, Fpath& limit_int, float bord_width_ratio);
	
	// Takes a thick path i.e. two paths that are the side limits and converts it to triangles to be rendered efficiently with the GPU 
	std::vector<SDL_FPoint> generate_triangles_thick_paths(const Fpath& path_side1, const Fpath& path_side2);

	// Renders efficiently a set of triangles 
	void draw_triangles(const std::vector<SDL_FPoint>& triangles);

	// Renders a rectangle ( using 2 triangles )
	void draw_rectangle(Vec2<int> pos, int w, int h);
}
