#include "Map.h"

Map::Map(int side) : size(side) {}


void Map::setRandomTerains() {
	for (int i = 0; i < size * size; i++) {
		map[i].setTerrain((TerrainID)(1 + rand() % numberOfTerrains));
	}
}
int Map::prand(int probas[], int n) {
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

int Map::linearise(int i, int j) {
	return (int)(i + size * j);
}

void Map::setRandomMap() {
	for (size_t a = 0; a < size * size; a++) {
		int probas[numberOfTerrains] = { 25, 25, 25, 25 };
		map[a].setTerrain((TerrainID)(1 + prand(probas, numberOfTerrains)));
		std::vector<TerrainAlterationID> alts = possibleAlterations(map[a].getTerrain());
		map[a].setAlteration(alts[rand()%alts.size()]);
		std::vector<ResourceID> ress = possibleResources(map[a].getTerrain(), map[a].getAlteration());
		map[a].setResource(ress[rand() % ress.size()]);
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

size_t Map::getSize() const {
	return size;
}
