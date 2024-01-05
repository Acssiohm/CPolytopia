#pragma once
#include "IDs.h"

class Player;

class Unit{
public:
	Unit(Player* owner = nullptr, UnitID id = UnitID::None);

	Player* m_owner;
	UnitID m_type; 
};

