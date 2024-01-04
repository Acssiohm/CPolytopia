#pragma once
#include <vector>

class Player{
public :
	Player(size_t map_size) 
		:discovered(map_size*map_size, false){}
	std::vector<bool> discovered;

};

