#include "Unit.h"

Unit::Unit(Player* owner , UnitID id )
	:m_owner(owner), m_type(id) {}