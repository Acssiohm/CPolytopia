#include "Player.h"

Player::Player(Map& map)
	:discovered(map.getSize()* map.getSize(), false), m_map(map) {}

bool Player::isDiscovered(int i, int j) const {
	return discovered[m_map.linearise(i, j)];
}

void Player::discover(int i, int j) {
	discovered[m_map.linearise(i, j)] = true;
}

void Player::attackFrom(int i_attcker, int j_attacker, int i_defender, int j_defender) {
	// Implement attack logic here
	// For example, you can check the type of units and apply damage accordingly
	// This is a placeholder implementation
	// You can replace it with your actual attack logic
	Unit * attacker = m_map.getTileAt(i_attcker, j_attacker)->getUnit();
	Unit * defender = m_map.getTileAt(i_defender, j_defender)->getUnit();
	if (attacker && defender) {
		// Example: attacker attacks defender
		attacker -> attack(defender);
		if (defender->isDead()) {
			m_map.getTileAt(i_defender, j_defender)->setUnit(nullptr);
		}
	}
	else {
		std::cerr << "Error: One of the units is null!" << std::endl;
	}

}

void Player::moveUnit(int i, int j, int i_dest, int j_dest) {
	Unit * moving_unit = m_map.getTileAt(i, j)->getUnit();
	Tile* tile_dest = m_map.getTileAt(i_dest, j_dest);
	if (moving_unit && moving_unit->m_owner == this ) {
		tile_dest->setUnit(moving_unit);
		m_map.getTileAt(i, j)->setUnit(nullptr);
	}
	else {
		std::cerr << "Error: One of the tiles is null!" << std::endl;
	}
}