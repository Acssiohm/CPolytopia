#pragma once

#include "Tile.h"

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <map>
#include "ResourcesIds.h"
#include "Vec.h"

class Map {
public:
	Map(int side);
	void setRandomTerains();
	int prand(int probas[], int n);
	void setRandomMap();
	Vec2<int> delinearise(int a);
	int linearise(int i, int j) const ;
	Tile* getTileAt(int i, int j);
	const Tile* getTileAt(int i, int j) const;
	size_t getSize() const;
private:
	size_t size;
	std::vector<Tile> map{ size * size };
};
