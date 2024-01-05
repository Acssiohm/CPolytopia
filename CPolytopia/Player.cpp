#include "Player.h"

Player::Player(Map& map)
	:discovered(map.getSize()* map.getSize(), false), m_map(map) {}

bool Player::isDiscovered(int i, int j) const {
	return discovered[m_map.linearise(i, j)];
}

void Player::discover(int i, int j) {
	discovered[m_map.linearise(i, j)] = true;
}