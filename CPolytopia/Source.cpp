#include "Renderer.h"
#include "SDL2/SDL.h"

#include <set>
#include "Player.h"	

int main(int argc, char ** argv) {
	srand((unsigned int)time(NULL));
	Renderer::Init();
	Map carte(20);
	Player p1(carte);
	Player p2(carte);
	Player * p = &p1; 
	int turn = 1;
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 20; j++) {
			p->discover(i, j);
		}
	}
	for (int i = 10; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			p2.discover(i, j);
		}
	}
	carte.getTileAt(1, 1)->setUnit(new Unit(carte, p, UnitID::Warrior));
	carte.getTileAt(1, 2)->setUnit(new Unit(carte, p, UnitID::Archer));
	carte.getTileAt(1, 3)->setUnit(new Unit(carte, p, UnitID::Catapult));
	carte.getTileAt(1, 4)->setUnit(new Unit(carte, p, UnitID::Cloak));
	carte.getTileAt(1, 5)->setUnit(new Unit(carte, p, UnitID::Defender));
	carte.getTileAt(1, 6)->setUnit(new Unit(carte, p, UnitID::Giant));
	carte.getTileAt(1, 7)->setUnit(new Unit(carte, p, UnitID::Knight));
	carte.getTileAt(1, 8)->setUnit(new Unit(carte, p, UnitID::MindBender));
	carte.getTileAt(1, 9)->setUnit(new Unit(carte, p, UnitID::Rider));
	carte.getTileAt(1, 10)->setUnit(new Unit(carte, p, UnitID::Swordsman));

	carte.setRandomMap();
	std::set<std::tuple<int, int>> cases;
	cases.insert({ 0, 0 });
	cases.insert({ 1, 1 });
	cases.insert({ 1, 2 });
	cases.insert({ 1, 3 });
	cases.insert({ 2, 1 });
	cases.insert({ 3, 1 });
	cases.insert({ 3, 3 });
	cases.insert({ 4, 3 });
	cases.insert({ 2, 3 });
	cases.insert({ 3, 2 });
	cases = {};

	bool is_selected = false;
	std::tuple<int, int> selected_case = { 0, 0 };

	SDL_SetRenderDrawColor(Renderer::renderer, 230, 120 , 0, 200);
	bool is_map_visible = true;
	Renderer::renderAll(carte, *p, cases, is_map_visible);
	Renderer::update();
	bool on_hold = false;
	SDL_bool run = SDL_TRUE;
	while (run) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				run = SDL_FALSE;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					on_hold = true;
					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);
					Vec2 clicked = Renderer::cartesian_to_map(mouseX, mouseY);
					if (!carte.isInBounds(clicked.x(), clicked.y())) {
						
					} else if (cases.contains(clicked)) {
						cases.erase(clicked); 
					} else {
						if (cases.size() > 0) {
							auto cref = cases.begin();
							auto c = *cref;
							cases.erase(cases.begin(), cases.end());
							if (carte.getTileAt(std::get<0>(c), std::get<1>(c))->getUnit() != nullptr) {
								carte.moveUnit(std::get<0>(c), std::get<1>(c), std::get<0>((Renderer::point)clicked), std::get<1>((Renderer::point)clicked));
							}
							else {
								cases.insert(clicked);
							}
							
						} else {
							cases = { clicked };
						}
						
					}
					Renderer::clear();
					Renderer::renderAll(carte, *p , cases, is_map_visible);
					Renderer::update(); 
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					on_hold = false;
				}
				break;
			case SDL_MOUSEMOTION:
				if (on_hold) {
					Renderer::move(event.motion.xrel, event.motion.yrel);
					Renderer::clear();
					Renderer::renderAll(carte, *p, cases, is_map_visible);
					Renderer::update();
				}
				break;
			case SDL_MOUSEWHEEL:
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				if (event.wheel.y > 0) {
					Renderer::zoom(1.2f, Vec2<int>(mouseX, mouseY));
				}
				else if (event.wheel.y < 0) {
					Renderer::zoom(1/1.2f, Vec2<int>(mouseX, mouseY));

				}
				Renderer::clear();
				Renderer::renderAll(carte, *p, cases, is_map_visible);
				Renderer::update();

				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_r){
					carte.setRandomMap();
				}else if (event.key.keysym.sym == SDLK_UP) {
					Renderer::cloud_shift = Renderer::cloud_shift - unit_y;
					std::cout << Renderer::cloud_shift << std::endl;
				}else if (event.key.keysym.sym == SDLK_DOWN) {
					Renderer::cloud_shift = Renderer::cloud_shift + unit_y;
					std::cout << Renderer::cloud_shift << std::endl;
				}else if (event.key.keysym.sym == SDLK_LEFT) {
					Renderer::cloud_shift = Renderer::cloud_shift - unit_x;
					std::cout << Renderer::cloud_shift << std::endl;
				}else if (event.key.keysym.sym == SDLK_RIGHT) {
					Renderer::cloud_shift = Renderer::cloud_shift + unit_x;
					std::cout << Renderer::cloud_shift << std::endl;
				}else if (event.key.keysym.sym == SDLK_m){
					is_map_visible = !is_map_visible;
				}else if (event.key.keysym.sym == SDLK_t) {
					if (turn == 1) {
						turn = 2;
						p = &p2;
					}
					else {
						turn = 1;
						p = &p1;
					}
				}
				Renderer::clear();
				Renderer::renderAll(carte, *p, cases, is_map_visible);
				Renderer::update();

				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					int win_width;
					int win_height;
					SDL_GetWindowSize(Renderer::window, &win_width, &win_height);
					SDL_SetWindowSize(Renderer::window, win_width, win_height);
				}
				break;
			default:
				break;

			}
		}
		SDL_Delay(10);
	}

	Renderer::Quit();
	return 0;
}