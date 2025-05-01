#pragma once

#include "Tile.h"

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <map>
#include "IDs.h"
#include "Vec.h"

class Tile;

class Map {
public:
	Map() :Map(0) {};
	Map(int side);
	void setRandomTerains();
	// Randomly choose a number between 0 and n-1 with the given probabilities
	int prand(const int probas[], int n);
	void setRandomMap();
	void setRandomMapNaive();
	Vec2<int> delinearise(int a);
	int linearise(int i, int j) const ;
	Tile* getTileAt(int i, int j);
	const Tile* getTileAt(int i, int j) const;
	size_t getSize() const;
	void moveUnit(int i, int j, int i_dest, int j_dest);
	bool isInBounds(int i, int j) const;

private:
	size_t size;
	std::vector<Tile> map{ size * size };
};
