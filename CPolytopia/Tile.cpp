#include "Tile.h"
#include <iostream>

Tile::Tile(
	TerrainID terrain,
	TerrainAlterationID alteration,
	ResourceID resource,
	BuildingID building,
	Unit * unit) :
	m_terrain(terrain), m_alteration(alteration), m_resource(resource), m_building(building), m_unit(unit) {}

TerrainID Tile::getTerrain() const {
	return m_terrain;
}
void Tile::setTerrain(TerrainID t) {
	m_terrain = t;
}

TerrainAlterationID Tile::getAlteration() const {
	return m_alteration;
}
bool Tile::setAlteration(TerrainAlterationID a) {
	// TODO: verify if the alteration can be on the current terrain
	m_alteration = a;
	return true;
}

ResourceID Tile::getResource() const {
	return m_resource;
}
bool Tile::setResource(ResourceID res) {
	// TODO: verify if the resource can be on the current terrain
	m_resource = res;
	return true;
}

BuildingID Tile::getBuilding() const {
	return m_building;
}
bool Tile::setBuilding(BuildingID build) {
	// TODO: verify if the building can be on the current terrain
	m_building = build;
	return true;
}


Unit * Tile::getUnit() {
	return m_unit;
}
const Unit* Tile::getUnit() const {
	return m_unit;
}
bool Tile::setUnit(Unit * unit) {
	if (m_unit && unit) {
		std::cerr << "Error: Tile already has a unit!" << std::endl;
		std::cerr << "Unit type: " << (int)m_unit->getType() << std::endl;
	}
	m_unit = unit;
	return true;
}
UnitID Tile::getUnitType() const {
	if (m_unit) {
		return m_unit->getType();
	}
	return UnitID::None;
}

void Tile::check() {
	// TODO: vérifier si des changement sont à apporter (disparition forêt => disparition annimal)
	//       lancer un check des cases voisines si besoin
}