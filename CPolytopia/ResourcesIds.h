#pragma once
#include <vector>

constexpr int numberOfTerrains = 4;

enum class TerrainID {
	None = 0,
	Field = 1,
	Mountain = 2,
	Water = 3,
	Ocean = 4
};

constexpr int numberOfAlterations = 1;

enum class TerrainAlterationID {
	None = 0,
	Forest = 1
};

constexpr int numberOfResources = 6;

enum class ResourceID {
	None = 0,
	Fruit,
	Crop,
	WildAnimal,
	Metal,
	Fish,
	Whale
};

constexpr int numberOfBuildings = 7;

enum class BuildingID {
	None = 0,
	Farm,
	Forge,
	LumberHut,
	Mine,
	Port,
	Sawmill,
	Windmill
};

constexpr int numberOfUnits = 10;

enum class UnitID {
	None = 0,
	Warrior,
	Archer,
	Catapult,
	Rider,
	Knight,
	Defender,
	Cloak,
	Swordsman,
	MindBender,
	Giant
};

constexpr int numberOfIds = numberOfTerrains + numberOfAlterations + numberOfResources + numberOfBuildings + numberOfUnits;

constexpr int idof(const TerrainID id) {
	return (int)id - 1;
}

constexpr int idof(const TerrainAlterationID id) {
	if (id == TerrainAlterationID::None) {
		return -1;
	}
	return (int)id -1 + numberOfTerrains;
}

constexpr int idof(const ResourceID id) {
	if (id == ResourceID::None) {
		return -1;
	}
	return (int)id -1 + numberOfTerrains + numberOfAlterations;
}

constexpr int idof(const BuildingID id) {
	if (id == BuildingID::None) {
		return -1;
	}
	return (int)id -1 + numberOfTerrains + numberOfAlterations + numberOfResources;
}

constexpr int idof(const UnitID id) {
	if (id == UnitID::None) {
		return -1;
	}
	return (int)id - 1 + numberOfTerrains + numberOfAlterations + numberOfResources + numberOfBuildings;
}

std::vector<TerrainAlterationID> possibleAlterations(TerrainID ter);
std::vector<ResourceID> possibleResources(TerrainID ter, TerrainAlterationID ter_al);