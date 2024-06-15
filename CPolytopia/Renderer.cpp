#include "Renderer.h"
#include <set>
#include <unordered_map>
#include <cassert>
#include "Player.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include "TechTree.h"

namespace Renderer {
	constexpr int DEFAULT_WIN_WIDTH = 900;
	constexpr int DEFAULT_WIN_HEIGHT = 600;
	constexpr int DEFAULT_FONT_SIZE = 10;

	SDL_Window* window;
	SDL_Renderer* renderer;
	ImageStore* images;
	Image* cloud_image;
	const static std::string font_file = "Fonts/Swansea.ttf";
	static TTF_Font* font;

	Vec2<double> shift;
	Vec2<double> cloud_shift(-12, 133);
	// parameters
	Vec2<int> tile_size(2*415, 2*250);
	Vec2<int> img_sizes(841, 1000);

	float zoom_factor = 0.2f;
	Vec2<int> position(DEFAULT_WIN_WIDTH / 2 - (int)(zoom_factor*tile_size.x() / 2), 15);

	 
	void loadImages (){
		images = new ImageStore( renderer ,"./images/");
		
		images->add_image("Grass.png"		, TerrainID::Field);
		images->add_image("Mountain.png"	, TerrainID::Mountain);
		images->add_image("Water.png"		, TerrainID::Water);
		images->add_image("Ocean.png"		, TerrainID::Ocean);
		
		images->add_image("Forest.png"		, TerrainAlterationID::Forest);
		
		images->add_image("Fruit.png"		, ResourceID::Fruit);
		images->add_image("Crop.png"		, ResourceID::Crop);
		images->add_image("WildAnimal.png"	, ResourceID::WildAnimal);
		images->add_image("Metal.png"		, ResourceID::Metal);
		images->add_image("Fish.png"		, ResourceID::Fish);
		images->add_image("Whale.png"		, ResourceID::Whale);
		
		images->add_image("Farm.png"		, BuildingID::Farm);
		images->add_image("Forge.png"		, BuildingID::Forge);
		images->add_image("Lumber_hut.png"	, BuildingID::LumberHut);
		images->add_image("Mine.png"		, BuildingID::Mine);
		images->add_image("Port.png"		, BuildingID::Port);
		images->add_image("Sawmill.png"		, BuildingID::Sawmill);
		images->add_image("Windmill.png"	, BuildingID::Windmill);

		images->add_image("Warrior.png"		, UnitID::Warrior);
		images->add_image("Archer.png"		, UnitID::Archer);
		images->add_image("Catapult.png"	, UnitID::Catapult);
		images->add_image("Rider.png"		, UnitID::Rider);
		images->add_image("Knight.png"		, UnitID::Knight);
		images->add_image("Defender.png"	, UnitID::Defender);
		images->add_image("Cloak.png"		, UnitID::Cloak);
		images->add_image("Swordsman.png"	, UnitID::Swordsman);
		images->add_image("MindBender.png"	, UnitID::MindBender);
		images->add_image("Giant.png"		, UnitID::Giant);

		cloud_image = new Image(renderer, "./images/Clouds.png");
	}
	void loadFont(int font_size) {
		font = TTF_OpenFont(font_file.c_str(), font_size);
	}
	void Init() {
		SDL_Init(SDL_INIT_VIDEO);
		SDL_CreateWindowAndRenderer(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window, &renderer);
		TTF_Init();
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		
		loadImages();
		loadFont(DEFAULT_FONT_SIZE);

		SDL_RenderPresent(renderer);
	}
	
	
	void unloadImages() {
		delete images;
		delete cloud_image;
	}
	void unloadFont() {
		TTF_CloseFont(font);
	}
	void Quit() {
		unloadImages();
		unloadFont();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		IMG_Quit();
	}

	
	void update() {
		SDL_RenderPresent(renderer);
	}
	void clear() {
		Uint8 r, g, b, a; 
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a); 
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	
	void zoom(float zoom_scale, Vec2<int> centre) {
		float zoom_factor_preview = zoom_factor * zoom_scale;
		if (zoom_factor_preview >= 0.01) {
			shift = shift + (Vec2<float>(centre- position) - shift) * (1 - zoom_scale);
			zoom_factor = zoom_factor_preview;
		}
	}
	void move(int x, int y) {
		shift = shift + Vec2<double>(x, y);
	}
	
	
	Vec2<int> map_to_cartesian(int i, int j) {
		return position + (Vec2<int>(j, j) + Vec2<int>(-i, i)) * tile_size * (zoom_factor/2) + Vec2<int>(shift);
	}
	Vec2<int> map_to_cartesian(Vec2<int> pos) {
		return map_to_cartesian(pos.x(), pos.y());
	}
	Vec2<int> cartesian_to_map(int x, int y) {
		Vec2<double> orig(422.*zoom_factor, 208.*zoom_factor);
		Vec2<double> res = Vec2<double>(-1, 1) * ((double)(x - position.x() - shift.x() - orig.x()) / ((double)tile_size.x() * zoom_factor))
			+ Vec2<double>(1, 1) * ((double)(y - position.y() - shift.y()-orig.y()) / ((double)tile_size.y() * zoom_factor));
		return {(int)std::floor(res.x()), (int)std::floor(res.y())};
	}
	Vec2<int> cartesian_to_map(Vec2<int> pos) {
		return cartesian_to_map(pos.x(), pos.y());
	}
	
	
	bool cannot_be_visible(int i , int j) {
		Vec2<int> coord = map_to_cartesian(i, j);
		int x = coord.x();
		int y = coord.y();
		return x > DEFAULT_WIN_WIDTH || x + img_sizes.x() * zoom_factor < 0 || y > DEFAULT_WIN_HEIGHT || img_sizes.y() * zoom_factor + y < 0;
	}
	void renderImg(Image * img, int i, int j) {
		img->draw(
			map_to_cartesian(i, j),
			zoom_factor
		);
	}
	void renderCloud(int i, int j) {
		cloud_image->draw(
			map_to_cartesian(i, j) + cloud_shift*zoom_factor,
			zoom_factor
		);
	}
	void renderUnits(const Map& carte) {
		for (int i = 0; i < carte.getSize(); i++) {
			for (int j = 0; j < carte.getSize(); j++) {
				//if (cannot_be_visible(i, j)) {continue;}
				const Tile* tile = carte.getTileAt(i, j);
				render(tile->getUnit().m_type, i, j);
			}
		}
	}
	void renderAll(const Map& carte, const Player& p, const std::set<point>& squares_selected, bool map_vs_ttree) {
		if (!map_vs_ttree) {
			renderTestTree();
			return;
		}
		renderMap(carte, p);
		draw_contour(squares_selected);
		renderUnits(carte);

		int h = 90;
		SDL_SetRenderDrawColor(renderer, 0, 0, 40, 255);
		draw_rectangle(Vec2(0, DEFAULT_WIN_HEIGHT - h), DEFAULT_WIN_WIDTH, h);
	}
	void renderMap(const Map& carte, const Player& p) {
		for (int i = 0; i < carte.getSize(); i++) {
			for (int j = 0; j < carte.getSize(); j++){
				//if (cannot_be_visible(i, j)) {continue;}
				if (!p.isDiscovered(i,j) ) {
					renderCloud(i, j);
					continue;
				}
				const Tile* tile = carte.getTileAt(i, j);
				render(tile ->getTerrain(), i, j);
				render(tile->getAlteration(), i, j);
				if (tile->getBuilding() == BuildingID::None) {
					render(tile->getResource(), i, j);
				}
				else {
					render(tile->getBuilding(), i, j);
				}
			}
		}
	}
	

	void renderTestTree() {
		TechTree ttree = generateTestTechTree();
		drawTechTree(&ttree, Vec2(400, 300));
	}
	TechTree generateTestTechTree() {
		TechTree ttree;
		auto* pt = &ttree.m_parentNode;

		auto c2 = pt->addChild("Organization");
		c2->addChild("Farming")->addChild("Construction");
		c2->addChild("Strategy")->addChild("Diplomacy");

		auto c3 = pt->addChild("Climbing");
		c3->addChild("Mining")->addChild("Smithery");
		c3->addChild("Meditation")->addChild("Philosophy");

		auto c4 = pt->addChild("Sailing");
		c4->addChild("Fishing")->addChild("Navigation");
		c4->addChild("Aquaculture")->addChild("Aquation");

		auto c5 = pt->addChild("Hunting");
		c5->addChild("Archery")->addChild("Spiritualism");
		c5->addChild("Forestry")->addChild("Mathematics");

		auto c1 = pt->addChild("Riding");
		c1->addChild("Roads")->addChild("Trade");
		c1->addChild("Free Spirit")->addChild("Chivalry");

		return ttree;
		//drawTechTree(&ttree, Vec2(400, 300 ));
	}
	// Draws tech node and his children recursively in a star shape as evenly spaced as possible
	void drawTechNode(const TechTree::TechNode * node, double min_angle, double max_angle, bool horaire, Vec2<double> pos) {
		const double rod_length = 100;
		const int rad = 35;
		double init_angle = (horaire ? max_angle : min_angle);
		int n = node->m_children.size();
		assert(max_angle > min_angle);
		double step_angle =  (max_angle - min_angle)/( std::max(n-1., 1/2.));
		step_angle = (horaire ? -step_angle : step_angle);
		for (size_t i = 0; i < n; i++){
			double a = init_angle + i * step_angle;
			double next_min = std::max(min_angle, a - abs(step_angle) / 2);
			double next_max = std::min(max_angle, a + abs(step_angle) / 2);
			Vec2 next_pos = pos + Vec2(std::cos(a), std::sin(a)) * rod_length ;
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			drawTechNode(node->m_children[i],
				next_min,
				next_max,
				a > (max_angle + min_angle)/2,
				next_pos );
		}
		filledCircleRGBA(renderer, pos.x(), pos.y(), rad, 255, 0, 0, 255);
		drawText(node->m_name, pos - Vec2(0, 5), {255, 255, 255, 255}, TextHPos::Center, TextVPos::Center);
	}
	// Draws tech tree in a star shape as evenly spaced as possible
	void drawTechTree(const TechTree * ttree, Vec2<double> pos) {
		const double rod_length = 100;
		//const int rad = 30;
		//filledCircleRGBA(renderer, pos.x(), pos.y(), rad, 255, 0, 0, 255);
		const TechTree::TechNode & const parent = ttree->m_parentNode;
		const int nb_childs = parent.m_children.size();
		double step_angle = 2 * M_PI / nb_childs;
		for (size_t i = 0; i < nb_childs; i++) {
			double a = i * step_angle ;
			drawTechNode(parent.m_children[i],
				(i - 1/2.)*step_angle,
				(i + 1/2.)*step_angle,
				true,
				pos + Vec2(std::cos(a), std::sin(a)) * rod_length); 
		}
	}


	enum class TextHPos {
		Left = 0,
		Center = 1,
		Right = 2
	};
	enum class TextVPos {
		Top = 0,
		Center = 1,
		Bottom = 2
	};
	void drawCharChain(const char* text, int x, int y, SDL_Color color, TextHPos hp, TextVPos vp) {
		SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, color);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_rect{};
		SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
		text_rect.x = x - ((int)vp*text_rect.w)/2;
		text_rect.y = y - ((int)hp * text_rect.h) / 2;
		SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
		SDL_FreeSurface(text_surface);
	}
	void drawText(const std::string& text, Vec2<int> pos, SDL_Color color, TextHPos hp, TextVPos vp) {
		drawCharChain(text.c_str(), pos.x(), pos.y(), color, hp, vp);
	}
	
	
	void border_list::verify_sizes() const {
		assert(intersections.size() == centers.size());
	}
	int border_list::size() const {
		verify_sizes();
		return intersections.size();
	}
	
	
	void draw_triangles(const std::vector<SDL_FPoint>& triangles) { 
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a); 
		SDL_Color color = { r, g, b, a };
		std::vector<SDL_Vertex> res;
		res.reserve(triangles.size());
		for (SDL_FPoint pos : triangles) {
			res.push_back({pos, color, {0., 0.}});
		}
		SDL_RenderGeometry(renderer, NULL, res.data(), (int)res.size(), NULL, 0);
	}
	void draw_rectangle(Vec2<int> pos, int w, int h) {
		std::vector<SDL_FPoint> triangles = {
			to_FPoint(pos), to_FPoint(pos + Vec2(w, 0)), to_FPoint(pos + Vec2(0, h)) ,
			to_FPoint(pos + Vec2(w, h)), to_FPoint(pos + Vec2(w, 0)), to_FPoint(pos + Vec2(0, h)) 
		};
		draw_triangles(triangles);
	}
	

	Vec2<int> rotation(Vec2<int> v, bool horaire) {
		int r = (horaire ? 1 : -1);
		return { -r* v.y() ,  r * v.x()};
	}
	void find_contour(const std::set<point>& squares, border_list& out_borders){
		const int directions_x[4] = {1,-1, 0, 0};
		const int directions_y[4] = {0, 0, 1, -1};
		std::set<segment> segments_vu; 
		for (const auto& [i, j] : squares) {
			for (int u = 0; u < 4; u++) {
				int x = directions_x[u], y = directions_y[u];
				if (squares.contains({ i + x, j + y })) {
					continue;
				}
				//  c1 = pos +1/2 + (dir-rot)/2
				// x : i + (1 + x + y )/2
				// y : j + (1 + y - x )/2
				//  c2 = pos +1/2 + (dir+rot)/2
				// x : i + (1 + x - y)/2
				// y : j + (1 + y + x)/2

				Vec2<int> c1(i + (1 + x - y) / 2, j + (1 + y - x) / 2);
				Vec2<int> c2(i + (1 + x + y) / 2, j + (1 + y + x) / 2);
				/*Vec2 c1(i + (1 + x + y) / 2, j + (1 + y - x) / 2);
				Vec2 c2(i + (1 + x - y) / 2, j + (1 + y + x) / 2);
				*/
				if (segments_vu.contains({ c1, c2 }) || segments_vu.contains({ c2, c1 })) {
					continue;
				}
				segments_vu.insert({c1,c2});
				if (y) {
					std::tie(c1, c2) = { c2, c1 }; // echange pour que la case soit à droite du vecteur c1 -> c2
				}
				bool square_is_left = false;
				size_t pos_first = out_borders.size();
				out_borders.centers.emplace_back(0,0); // on garde la place pour ne pas avoir à insérer
				out_borders.intersections.push_back(c1);
				out_borders.intersections.push_back(c2);
				Vec2<int> curr = out_borders.intersections.back();
				Vec2<int> init_dir = c2 - c1;
				Vec2 curr_dir = init_dir;
				int side = (square_is_left ? -1 : 1);
				while( true ) {
					// Rotation horaire ssi side = 1 ssi la case est à droite , car on veut commencer par la direction plus proche de la case
					Vec2 new_dir = rotation(curr_dir, !square_is_left);
					Vec2 double_vect_to_center = curr_dir + new_dir;
					
					// case : pos + double_vect_to_center/2 - (1,1)/2
					int d = 0;
					while (squares.contains(curr + (double_vect_to_center - Vec2(1,1))*(1./2) ) ) {
						d++;
						new_dir = rotation(new_dir, square_is_left);
						double_vect_to_center = rotation(double_vect_to_center , square_is_left);
					}
					if (curr == c1 && init_dir == new_dir) {
						break;
					}
					if (curr_dir == new_dir) {
						assert(d == 1);
						out_borders.intersections.pop_back();
					}
					else {
						assert(d == 0 || d == 2);
						int convexite = (d == 0 ? 1 : -1 );
						Vec2 case_cool = curr + ( (new_dir - curr_dir)*convexite - Vec2(1,1))*(1./2);
						out_borders.centers.push_back(case_cool);

					}
					out_borders.intersections.emplace_back(curr + new_dir);
					segments_vu.insert({curr, out_borders.intersections.back()});
					curr = out_borders.intersections.back();
					curr_dir = new_dir;
				}
				if (curr_dir == init_dir) {
					out_borders.intersections.erase(out_borders.intersections.begin() + pos_first);
					out_borders.intersections.back() = out_borders.intersections[pos_first];
					out_borders.centers.erase(out_borders.centers.begin() + pos_first);
					out_borders.centers.emplace_back(out_borders.centers[pos_first]);
				}
				else {
					Vec2 case_cool = curr + ((init_dir - curr_dir) - Vec2(1, 1)) * (1. / 2);
					if (case_cool != Vec2(i,j) ) {
						case_cool = curr + ((init_dir - curr_dir)*(-1) - Vec2(1, 1)) * (1. / 2);
					}
					out_borders.centers.emplace_back(case_cool);
					out_borders.centers[pos_first] = out_borders.centers.back();
				}
			}
		}
		out_borders.verify_sizes();
	}
	void convert_coins_to_bord_limits(const border_list& borders, Fpath& limit_ext, Fpath& limit_int , float bord_width_ratio) {
		const int n = borders.size();
		const Vec2<float> centre(421.f * zoom_factor, 461.f * zoom_factor);
		const Vec2<float> top_to_center(0, zoom_factor * tile_size.y() / 2.f);
		const Vec2<float> coin_haut = centre - top_to_center;

		limit_ext.reserve(n);
		limit_int.reserve(n);
		for (int i = 0; i < n; i++) {
			limit_ext.push_back( coin_haut + map_to_cartesian(borders.intersections[i]) );
			Vec2<float> c = coin_haut + top_to_center + map_to_cartesian(borders.centers[i]);
			limit_int.push_back( (c * bord_width_ratio + limit_ext[i] * (1 - bord_width_ratio)) );
		}
	}
	SDL_FPoint to_FPoint(Vec2<float> v) {
		return { v.x(), v.y() };
	}
	std::vector<SDL_FPoint> generate_triangles_thick_paths(const Fpath& path_side1, const Fpath& path_side2) {
		assert(path_side1.size() == path_side2.size());
		std::vector<SDL_FPoint> triangles; 
		size_t i_gen= 0;
		for (size_t i = 1; i < path_side1.size(); i++){
			if (i_gen != i-1 && path_side1[i_gen] == path_side1[i-1] && path_side2[i_gen] == path_side2[i-1]) {
				i_gen = i;
				continue;
			}
			SDL_FPoint pos1 = to_FPoint(path_side1[i - 1]);
			SDL_FPoint pos2 = to_FPoint(path_side1[i]);

			SDL_FPoint pos1p = to_FPoint(path_side2[i - 1]);
			SDL_FPoint pos2p = to_FPoint(path_side2[i]);
			
			// génère le quadrilataire pos1, pos2, pos1p, pos2p 
			// avec deux triangles ayant [pos2, pos1p] comme coté commun
			triangles.push_back(pos1);
			triangles.push_back(pos2);
			triangles.push_back(pos1p);
			
			triangles.push_back(pos2);
			triangles.push_back(pos1p);
			triangles.push_back(pos2p);
		}
		return triangles;
	}
	void draw_contour(const std::set<point>& squares, float ratio) {
		border_list borders;
		find_contour(squares, borders);
		Fpath p1, p2;
		convert_coins_to_bord_limits(borders, p1, p2, ratio);
		draw_triangles( generate_triangles_thick_paths(p1, p2) );
	}
}