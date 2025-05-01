#include "Map.h"
#include <queue>
#include <set>

Map::Map(int side) : size(side) {}


void Map::setRandomTerains() {
	for (size_t i = 0; i < size * size; i++) {
		map[i].setTerrain((TerrainID)(1 + rand() % numberOfTerrains));
	}
}

int Map::prand(const int probas[], int n) {
	// aléatoire avec des probabolités
	int total = 0;
	for (int i = 0; i < n; i++) {
		total += probas[i];
	}
	int r = rand() % total;

	int s = 0;
	for (int i = 0; i < n; i++) {
		s += probas[i];
		if (r < s) {
			return i;
		}
	}
	return 0; // Never happens
}
Vec2<int> Map::delinearise(int a) {
	return Vec2<int>((int)(a%size), (int)(a/size));
}

int Map::linearise(int i, int j) const {
	return (int)(i + size * j);
}

void Map::setRandomMapNaive() {
	for (size_t a = 0; a < size * size; a++) {
		int probas[numberOfTerrains] = { 25, 25, 25, 25 };
		map[a].setTerrain((TerrainID)(1 + prand(probas, numberOfTerrains)));
		std::vector<TerrainAlterationID> alts = possibleAlterations(map[a].getTerrain());
		map[a].setAlteration(alts[rand()%alts.size()]);
		std::vector<ResourceID> ress = possibleResources(map[a].getTerrain(), map[a].getAlteration());
		map[a].setResource(ress[rand() % ress.size()]);
	}
}

void Map::setRandomMap() {
	const int capital_spacing = 6;
	const int uncerrtain_spacing = 1;
	const int margin = uncerrtain_spacing+1;
	std::queue<Vec2<int>> exploration;
	std::set<Vec2<int>> visited;
	for (int k = 0; k < size*size; k++) {
		map[k].setTerrain(TerrainID::None);
		map[k].setAlteration(TerrainAlterationID::None);
		map[k].setResource(ResourceID::None);
		map[k].setBuilding(BuildingID::None);
	}
	for (int i = margin; i < size-margin; i+=capital_spacing ){
		for (int j = margin; j < size-margin; j += capital_spacing) {
			int di = ( rand() % (2* uncerrtain_spacing+1)) - uncerrtain_spacing;
			int dj = ( rand() % (2* uncerrtain_spacing+1)) - uncerrtain_spacing;
			int i_ = i + di;
			int j_ = j + dj;
			map[linearise(i_, j_)].setBuilding(BuildingID::Capital);
			
			for (int k = -2; k <= 2; k++) {
				for (int l = -2; l <= 2; l++) {
					if (k * k + l * l < 4) {
						map[linearise(i_+k, j_+l)].setTerrain(TerrainID::Field);
						visited.insert(Vec2<int>(i_+k, j_+l));
					}
					else {
						exploration.push(Vec2<int>(i_ + k, j_ + l));
					}
				}
			}
		}
	}
	while (!exploration.empty()) {
		Vec2<int> pos = exploration.front();
		exploration.pop();
		if (visited.contains(pos) || !isInBounds(pos.x(), pos.y())) {
			continue;
		}
		int i = pos.x();
		int j = pos.y();
		int nb_field = 0;
		int nb_water = 0;
		for (int k = -1; k <= 1; k++) {
			for (int l = -1; l <= 1; l++) {
				if ((k == 0) == (l == 0)) {
					continue;
				}
				if (!visited.contains(Vec2<int>(i + k, j + l))) {
					continue;
				}
				Tile* tile = getTileAt(i + k, j + l);
				if (tile->getTerrain() == TerrainID::Field) {
					nb_field++;
				}
				else if (tile->getTerrain() == TerrainID::Water) {
					nb_water++;
				}
			}
		}
		int chosen = 0;
		int probas[2] = { 6 + nb_field * 3,  3 + nb_water*10 };
		chosen = prand(probas, 2);
		if (chosen == 0) {
			map[linearise(i, j)].setTerrain(TerrainID::Field);
		}
		else {
			map[linearise(i, j)].setTerrain(TerrainID::Water);
		}
		visited.insert(pos);
		for (int k = -1; k <= 1; k++) {
			for (int l = -1; l <= 1; l++) {
				exploration.push(Vec2<int>(i + k, j + l));
			}
		}
	}
	// transform non reachable water into ocean
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (map[linearise(i, j)].getTerrain() != TerrainID::Water) {
				continue;
			}
			int nb_field = 0;
			for (int k = -1; k <= 1; k++) {
				for (int l = -1; l <= 1; l++) {
					if ((k == 0) == (l == 0) || !isInBounds(i+k, j+l)) {
						continue;
					}
					if (getTileAt(i + k, j + l)->getTerrain() == TerrainID::Field) {
						nb_field++;
					}
				}
			}
			if (nb_field == 0) {
				map[linearise(i, j)].setTerrain(TerrainID::Ocean);
			}
		}
	}
	// randomly transform fields into mountain
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (map[linearise(i, j)].getTerrain() != TerrainID::Field 
				|| map[linearise(i, j)].getBuilding() == BuildingID::Capital) {
				continue;
			}
			int probas[3] = { 20, 5, 2 };
			int alteration = prand(probas, 3);
			if (alteration == 1) {
				map[linearise(i, j)].setAlteration(TerrainAlterationID::Forest);
				probas[0] = 1;
				probas[1] = 1;
				int animal = prand(probas, 2);
				if (animal == 1) {
					map[linearise(i, j)].setResource(ResourceID::WildAnimal);
				}
			}
			else if (alteration == 2) {
				map[linearise(i, j)].setTerrain(TerrainID::Mountain);
				probas[0] = 1;
				probas[1] = 1;
				int metal = prand(probas, 2);
				if (metal == 1) {
					map[linearise(i, j)].setResource(ResourceID::Metal);
				}
			} else {
				// 	None/Fruit/Crop/
				int res_probas[3] = { 6, 3, 1 };
				int resource = prand(res_probas, 3);
				map[linearise(i, j)].setResource(ResourceID(resource));
			}
		}
	}
}

Tile* Map::getTileAt(int i, int j) {
	if (i >= size || i < 0 || j < 0 || j >= size) {
		std::cerr << "Error , index out of range : ( "
			<< i << " , " << j << " )  size : " << size << std::endl;
		return nullptr;
	}
	return &map[linearise(i,j)];
}
const Tile* Map::getTileAt(int i, int j) const {
	if (i >= size || i < 0 || j < 0 || j >= size) {
		std::cerr << "Error , index out of range : ( "
			<< i << " , " << j << " )  size : " << size << std::endl;
		return nullptr;
	}
	return &map[linearise(i, j)];
}

size_t Map::getSize() const {
	return size;
}

void Map::moveUnit(int i, int j, int i_dest, int j_dest) {
	Tile* tile = getTileAt(i, j);
	Tile* tile_dest = getTileAt(i_dest, j_dest);
	if (tile_dest->getUnit() == nullptr ) {
		tile_dest->setUnit(tile->getUnit());
		tile->setUnit(nullptr);
	}
	else {
		std::cerr << "Error : destination tile already has a unit" << std::endl;
	}
}

bool Map::isInBounds(int i, int j) const {
	return (i >= 0 && i < size && j >= 0 && j < size);
}