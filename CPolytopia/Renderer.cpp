#include "Renderer.h"
#include <set>
#include <unordered_map>
#include <cassert>
namespace Renderer {
	constexpr int DEFAULT_WIN_WIDTH = 900;
	constexpr int DEFAULT_WIN_HEIGHT = 600;
	constexpr int DEFAULT_FONT_SIZE = 20;

	SDL_Window* window;
	SDL_Renderer* renderer;
	ImageStore* images;
	const static std::string font_file = "Resources/LargeFont.ttf";
	static TTF_Font* font;

	Vec2<double> shift;
	// parameters
	Vec2<int> tile_size(2*415, 2*250);
	Vec2<int> img_sizes(841, 1000);

	float zoom_factor = 0.2f;
	Vec2<int> position(DEFAULT_WIN_WIDTH / 2 - (int)(zoom_factor*tile_size.x() / 2), 15);

	void loadImages (){
		images = new ImageStore( renderer ,"./images_2/");
		
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
		//images->add_image("Forge_level_1.png"		, BuildingID::Forge);
		//images->add_image("Lumber_hut.png"		, BuildingID::LumberHut, 0.5);
		images->add_image("Mine.png"		, BuildingID::Mine);
		images->add_image("Port.png"		, BuildingID::Port);
		//images->add_image("Sawmill.png"		, BuildingID::Sawmill, 0.5);
		//images->add_image("Windmill_1.png"	, BuildingID::Windmill);

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
	}

	void loadFont(int font_size) {
		font = TTF_OpenFont(font_file.c_str(), font_size);
	}
	void unloadImages() {
		delete images;
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
	bool cannot_be_visible(int i , int j) {
		Vec2<int> coord = map_to_cartesian(i, j);
		int x = coord.x();
		int y = coord.y();
		return x > DEFAULT_WIN_WIDTH || x + img_sizes.x() * zoom_factor < 0 || y > DEFAULT_WIN_HEIGHT || img_sizes.y() * zoom_factor + y < 0;
	}
	void renderImg(ImageStore::Image * img, int i, int j) {
		img->draw(
			map_to_cartesian(i, j),
			zoom_factor
		);
	}
	void renderMap(Map& carte) {
		for (int i = 0; i < carte.getSize(); i++) {
			for (int j = 0; j < carte.getSize(); j++){
				//if (cannot_be_visible(i, j)) {continue;}
				Tile* tile = carte.getTileAt(i, j);
				render(tile ->getTerrain(), i, j);
				render(tile->getAlteration(), i, j);
				render(tile->getResource(), i, j);
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

	typedef std::tuple<int, int> point;
	typedef std::tuple<int, int> direction;
	typedef std::tuple< point, point> segment;
	typedef std::tuple< Vec2<int>, Vec2<int>> segment2;
	Vec2<int> rotation(Vec2<int> v, bool horaire) {
		int r = (horaire ? 1 : -1);
		return { -r* v.y() ,  r * v.x()};
	}
	void find_contour(const std::set<point>& squares, std::vector<Vec2<int>>& res2, std::vector<Vec2<int>>& res){
		const int directions_x[4] = {1,-1, 0, 0};
		const int directions_y[4] = {0, 0, 1, -1};
		std::set<segment2> segments_vu; 
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
					std::tie(c1, c2) = { c2, c1 }; // echange pour que la case soit � droite du vecteur c1 -> c2
				}
				bool square_is_left = false;
				assert(res.size() == res2.size());
				size_t pos_first = res.size();
				res2.emplace_back(0,0); // on garde la place pour ne pas avoir � ins�rer
				res.push_back(c1);
				res.push_back(c2);
				Vec2<int> curr = res.back();
				Vec2<int> init_dir = c2 - c1;
				Vec2 curr_dir = init_dir;
				int side = (square_is_left ? -1 : 1);
				while( curr != c1) {
					// Rotation horaire ssi side = 1 ssi la case est � droite , car on veut commencer par la direction plus proche de la case
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
						res.pop_back();
					}
					else {
						assert(d == 0 || d == 2);
						int convexite = (d == 0 ? 1 : -1 );
						Vec2 case_cool = curr + ( (new_dir - curr_dir)*convexite - Vec2(1,1))*(1./2);
						res2.push_back(case_cool);
					}
					res.emplace_back(curr + new_dir);
					segments_vu.insert({curr, res.back()});
					curr = res.back();
					curr_dir = new_dir;
				}
				if (curr_dir == init_dir) {
					res.erase(res.begin() + pos_first);
					res.back() = res[pos_first];
					res2.erase(res2.begin() + pos_first);
					res2.emplace_back(res2[pos_first]);
				}
				else {
					Vec2 case_cool = curr + ((init_dir - curr_dir) - Vec2(1, 1)) * (1. / 2);
					if (case_cool != Vec2(i,j) ) {
						case_cool = curr + ((init_dir - curr_dir)*(-1) - Vec2(1, 1)) * (1. / 2);
					}
					res2.emplace_back(case_cool);
					res2[pos_first] = res2.back();
				}
			}
		}
	}
	SDL_FPoint to_FPoint(Vec2<float> v) {
		return { v.x(), v.y() };
	}
	std::vector<SDL_FPoint> contour_triangule(const std::set<point>& squares, float ratio) {
		std::vector<Vec2<int>> res2;
		std::vector<Vec2<int>> res;
		find_contour(squares, res2, res);
		Vec2<float> centre(421.f*zoom_factor, 461.f*zoom_factor);
		Vec2<float> top_to_center(0, zoom_factor * tile_size.y() / 2.f);
		Vec2<float> coin_haut = centre - top_to_center  ;
		std::vector<SDL_FPoint> triangles; 
		size_t i_gen= 0;
		for (size_t i = 1; i < res.size(); i++){
			if (i_gen != i-1 && res[i_gen] == res[i-1]) {
				i_gen = i;
				continue;
			}
			Vec2<float> pos1 = map_to_cartesian(res[i-1]);
			Vec2<float> pos2 = map_to_cartesian(res[i]);
			
			Vec2<float> c1 = top_to_center + map_to_cartesian(res2[i - 1]); 
			Vec2<float> c2 = top_to_center + map_to_cartesian(res2[i]);
			
			Vec2<float> pos1p( c1*ratio +  pos1* (1 - ratio));
			Vec2<float> pos2p( c2*ratio +  pos2* (1 - ratio));
			
			triangles.push_back(to_FPoint( pos1 + coin_haut));
			triangles.push_back(to_FPoint( pos2 + coin_haut));
			triangles.push_back(to_FPoint(pos1p + coin_haut));
			
			triangles.push_back(to_FPoint( pos2 + coin_haut));
			triangles.push_back(to_FPoint(pos1p + coin_haut));
			triangles.push_back(to_FPoint(pos2p + coin_haut));
		}
		return triangles;
	}
	void draw_contour(const std::set<std::tuple<int, int> >& squares, float ratio) {
		auto triangles = contour_triangule(squares, ratio);
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