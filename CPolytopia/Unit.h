#pragma once
#include "ResourcesIds.h"
#include "Player.h"

class Unit{
public:
	Unit(Player * owner = nullptr, UnitID id = UnitID::None):m_owner(owner), m_type(id) {}
	Player* m_owner;
	UnitID m_type;
};

