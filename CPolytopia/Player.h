#pragma once
#include <vector>
#include "Map.h"

class Player{
public :
	Player(Map& map);
	bool isDiscovered(int i, int j) const;
	void discover(int i, int j);
private:
	std::vector<bool> discovered;
	Map& m_map;
};

