#include "Tile.h"

Tile::Tile(
	TerrainID terrain,
	TerrainAlterationID alteration,
	ResourceID resource,
	BuildingID building,
	Unit unit) :
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


Unit Tile::getUnit() const {
	return m_unit;
}
bool Tile::setUnit(Unit unit) {
	m_unit = unit;
	return true;
}

void Tile::check() {
	// TODO: v�rifier si des changement sont � apporter (disparition for�t => disparition annimal)
	//       lancer un check des cases voisines si besoin
}