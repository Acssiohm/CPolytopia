#include "Renderer.h"
#include <set>
#include <unordered_map>
#include <cassert>

#include "Player.h"

namespace Renderer {
	constexpr int DEFAULT_WIN_WIDTH = 900;
	constexpr int DEFAULT_WIN_HEIGHT = 600;
	constexpr int DEFAULT_FONT_SIZE = 20;

	SDL_Window* window;
	SDL_Renderer* renderer;
	ImageStore* images;
	Image* cloud_image;
	const static std::string font_file = "Resources/LargeFont.ttf";
	static TTF_Font* font;

	Vec2<double> shift;
	Vec2<double> cloud_shift(-12, 133);
	// parameters
	Vec2<int> tile_size(2*415, 2*250);
	Vec2<int> img_sizes(841, 1000);

	float zoom_factor = 0.2f;
	Vec2<int> position(DEFAULT_WIN_WIDTH / 2 - (int)(zoom_factor*tile_size.x() / 2), 15);

	static void loadImages (){
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
	void unloadImages() {
		delete images;
		delete cloud_image;
	}
	void unloadFont() {
		TTF_CloseFont(font);
	}
	void Init() {
		SDL_Init(SDL_INIT_VIDEO);
		SDL_CreateWindowAndRenderer(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT, 0, &window, &renderer);
		TTF_Init();
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		
		loadImages();
		loadFont(DEFAULT_FONT_SIZE);

		SDL_RenderPresent(renderer);
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
	void renderMap(const Map& carte, const Player& p, const std::set<point>& squares_selected) {
		for (int i = 0; i < carte.getSize(); i++) {
			for (int j = 0; j < carte.getSize(); j++){
				//if (cannot_be_visible(i, j)) {continue;}
				if (!p.discovered[carte.linearise(i,j)]) {
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
		draw_contour(squares_selected);
		for (int i = 0; i < carte.getSize(); i++) {
			for (int j = 0; j < carte.getSize(); j++) {
				//if (cannot_be_visible(i, j)) {continue;}
				const Tile* tile = carte.getTileAt(i, j);
				render(tile->getUnit().m_type, i, j);
			}
		}
		
	}
	void move(int x, int y) {
		shift = shift + Vec2<double>(x, y);
	}
	void zoom(float zoom_scale, Vec2<int> centre) {
		float zoom_factor_preview = zoom_factor * zoom_scale;
		if (zoom_factor_preview >= 0.01) {
			shift = shift + (Vec2<float>(centre- position) - shift) * (1 - zoom_scale);
			zoom_factor = zoom_factor_preview;
		}
	}
	void drawCharChain(const char* text, int x, int y, SDL_Color color) {
		SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, color);
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		SDL_Rect text_rect{};
		SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
		text_rect.x = x;
		text_rect.y = y;
		SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
		SDL_FreeSurface(text_surface);
	}
	void drawText(const std::string& text, Vec2<int> pos, SDL_Color color) {
		drawCharChain(text.c_str(), pos.x(), pos.y(), color);
	}

	Vec2<int> rotation(Vec2<int> v, bool horaire) {
		int r = (horaire ? 1 : -1);
		return { -r* v.y() ,  r * v.x()};
	}

	void find_contour(const std::set<point>& squares, path& out_bords_coin, path& out_cases_coin){
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

				Vec2 c1(i + (1 + x - y) / 2, j + (1 + y - x) / 2);
				Vec2 c2(i + (1 + x + y) / 2, j + (1 + y + x) / 2);
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
				assert(out_bords_coin.size() == out_cases_coin.size());
				size_t pos_first = out_bords_coin.size();
				out_cases_coin.emplace_back(0,0); // on garde la place pour ne pas avoir à insérer
				out_bords_coin.push_back(c1);
				out_bords_coin.push_back(c2);
				Vec2<int> curr = out_bords_coin.back();
				Vec2<int> init_dir = c2 - c1;
				Vec2 curr_dir = init_dir;
				int side = (square_is_left ? -1 : 1);
				while( curr != c1) {
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
					if (curr_dir == new_dir) {
						assert(d == 1);
						out_bords_coin.pop_back();
					}
					else {
						assert(d == 0 || d == 2);
						int convexite = (d == 0 ? 1 : -1 );
						Vec2 case_cool = curr + ( (new_dir - curr_dir)*convexite - Vec2(1,1))*(1./2);
						out_cases_coin.push_back(case_cool);
					}
					out_bords_coin.emplace_back(curr + new_dir);
					segments_vu.insert({curr, out_bords_coin.back()});
					curr = out_bords_coin.back();
					curr_dir = new_dir;
				}
				if (curr_dir == init_dir) {
					out_bords_coin.erase(out_bords_coin.begin() + pos_first);
					out_bords_coin.back() = out_bords_coin[pos_first];
					out_cases_coin.erase(out_cases_coin.begin() + pos_first);
					out_cases_coin.emplace_back(out_cases_coin[pos_first]);
				}
				else {
					Vec2 case_cool = curr + ((init_dir - curr_dir) - Vec2(1, 1)) * (1. / 2);
					if (case_cool != Vec2(i,j) ) {
						case_cool = curr + ((init_dir - curr_dir)*(-1) - Vec2(1, 1)) * (1. / 2);
					}
					out_cases_coin.emplace_back(case_cool);
					out_cases_coin[pos_first] = out_cases_coin.back();
				}
			}
		}
	}
	void convert_coins_to_bord_limits(const path& bords_coin, const path& cases_coin, Fpath& limit_ext, Fpath& limit_int , float bord_width_ratio) {
		assert(bords_coin.size() == cases_coin.size());
		
		const Vec2<float> centre(421.f * zoom_factor, 461.f * zoom_factor);
		const Vec2<float> top_to_center(0, zoom_factor * tile_size.y() / 2.f);
		const Vec2<float> coin_haut = centre - top_to_center;

		limit_ext.reserve(bords_coin.size());
		limit_int.reserve(cases_coin.size());
		for (int i = 0; i < cases_coin.size(); i++) {
			limit_ext.push_back( coin_haut + map_to_cartesian(bords_coin[i]) );
			Vec2<float> c = coin_haut + top_to_center + map_to_cartesian(cases_coin[i]);
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
			if (i_gen != i-1 && path_side1[i_gen] == path_side1[i-1]) {
				i_gen = i;
				continue;
			}
			SDL_FPoint pos1 = to_FPoint(path_side1[i - 1]);
			SDL_FPoint pos2 = to_FPoint(path_side1[i]);

			SDL_FPoint pos1p = to_FPoint(path_side2[i - 1]);
			SDL_FPoint pos2p = to_FPoint(path_side2[i]);
			
			triangles.push_back(pos1);
			triangles.push_back(pos2);
			triangles.push_back(pos1p);
			
			triangles.push_back(pos2);
			triangles.push_back(pos1p);
			triangles.push_back(pos2p);
		}
		return triangles;
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
	void draw_contour(const std::set<point>& squares, float ratio) {
		path res1, res2;
		find_contour(squares, res1, res2);
		Fpath p1, p2;
		convert_coins_to_bord_limits(res1, res2, p1, p2, ratio);
		draw_triangles( generate_triangles_thick_paths(p1, p2) );
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


}