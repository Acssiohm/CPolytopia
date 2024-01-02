#include "Renderer.h"
#include "SDL2/SDL.h"

#include <set>
	

int main(int argc, char ** argv) {
	srand((unsigned int)time(NULL));
	Player p;
	Renderer::Init();
	Map carte(20);
	carte.getTileAt(1, 1) ->setUnit(Unit(&p, UnitID::Warrior));
	carte.getTileAt(1, 2)->setUnit(Unit(&p, UnitID::Archer));
	carte.getTileAt(1, 3)->setUnit(Unit(&p, UnitID::Catapult));
	carte.getTileAt(1, 4)->setUnit(Unit(&p, UnitID::Cloak));
	carte.getTileAt(1, 5)->setUnit(Unit(&p, UnitID::Defender));
	carte.getTileAt(1, 6)->setUnit(Unit(&p, UnitID::Giant));
	carte.getTileAt(1, 7)->setUnit(Unit(&p, UnitID::Knight));
	carte.getTileAt(1, 8)->setUnit(Unit(&p, UnitID::MindBender));
	carte.getTileAt(1, 9)->setUnit(Unit(&p, UnitID::Rider));
	carte.getTileAt(1, 10)->setUnit(Unit(&p, UnitID::Swordsman));

	carte.setRandomMap();
	Renderer::renderMap(carte);
	std::set<std::tuple<int, int>> cases;
	cases.insert({ 0,0 });
	cases.insert({ 1,1 });
	cases.insert({ 1,2 });
	cases.insert({ 1,3 });
	cases.insert({ 2,1 });
	cases.insert({ 3,1 });
	cases.insert({ 3,3 });
	cases.insert({ 4,3 });
	cases.insert({ 2,3 });
	cases.insert({ 3,2 });
	SDL_SetRenderDrawColor(Renderer::renderer, 0, 255, 180, 200);
	Renderer::draw_contour(cases);
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
					Renderer::renderMap(carte);
					Renderer::draw_contour(cases);
					Renderer::update();

				}
				break;
			case SDL_MOUSEWHEEL:
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				if (event.wheel.y > 0) {
					Renderer::zoom(1.2f, Vec2(mouseX, mouseY));
				}
				else if (event.wheel.y < 0) {
					Renderer::zoom(1/1.2f, Vec2(mouseX, mouseY));

				}
				Renderer::clear();
				Renderer::renderMap(carte);
				Renderer::draw_contour(cases);
				Renderer::update();

				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_r){
					carte.setRandomMap();
				}else if (event.key.keysym.sym == SDLK_UP) {
					Renderer::tile_size = Renderer::tile_size - unit_y;
				}else if (event.key.keysym.sym == SDLK_DOWN) {
					Renderer::tile_size = Renderer::tile_size + unit_y;
				}else if (event.key.keysym.sym == SDLK_LEFT) {
					Renderer::tile_size = Renderer::tile_size + unit_x;
				}else if (event.key.keysym.sym == SDLK_RIGHT) {
					Renderer::tile_size = Renderer::tile_size - unit_x;
				}
				Renderer::clear();
				Renderer::renderMap(carte);
				Renderer::draw_contour(cases);
				Renderer::update();

				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					int win_width;
					int win_height;
					SDL_GetWindowSize(Renderer::window, &win_width, &win_height);
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