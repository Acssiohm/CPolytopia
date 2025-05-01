#pragma once
#include <vector>
#include "Map.h"

class Player{
public :
	Player(Map& map);
	bool isDiscovered(int i, int j) const;
	void discover(int i, int j);
	void attackFrom(int i_attcker, int j_attacker, int i_defender, int j_defender);
	void moveUnit(int i, int j, int i_dest, int j_dest);
private:
	std::vector<bool> discovered;
	Map& m_map;
};

