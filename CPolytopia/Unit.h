#pragma once
#include "IDs.h"
#include <iostream>
#include "Map.h"

class Player;
class Map;

class Unit{
public:
	Unit(Map& map, Player* owner = nullptr, UnitID id = UnitID::None);
	bool isDead() const;
	bool attack(Unit* target);
	UnitID getType() const;

	Player* m_owner;
	UnitID m_type;
	int m_health;
	Map& m_map;
};

