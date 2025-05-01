#include "Unit.h"

Unit::Unit(Map& map, Player* owner , UnitID id )
	:m_owner(owner), m_type(id), m_map(map), m_health(0) {}

bool Unit::isDead() const
{
	return m_health <= 0;
}
UnitID Unit::getType() const {
	return m_type;
}
bool Unit::attack(Unit* target){
	if (target) {
		target->m_health -= 10; // Example damage value
		return target->isDead();
	}
	else {
		std::cerr << "Error: Target unit is null!" << std::endl;
		return false;
	}
}
