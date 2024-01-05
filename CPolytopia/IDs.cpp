#include "IDs.h"

std::vector<TerrainAlterationID> possibleAlterations(TerrainID ter) {
	if (ter == TerrainID::Field) {
		return { TerrainAlterationID::None, TerrainAlterationID::Forest };
	}
	else {
		return { TerrainAlterationID::None };
	}
}

std::vector<ResourceID> possibleResources(TerrainID ter, TerrainAlterationID ter_al) {
	switch (ter) {
	case TerrainID::None:
		return { ResourceID::None };
	case TerrainID::Field:
		if (ter_al == TerrainAlterationID::Forest) {
			return { ResourceID::None, ResourceID::WildAnimal };
		}
		else {
			return { ResourceID::None, ResourceID::Fruit, ResourceID::Crop };
		}
	case TerrainID::Mountain:
		return { ResourceID::None, ResourceID::Metal };
	case TerrainID::Water:
		return { ResourceID::None, ResourceID::Fish };
	case TerrainID::Ocean:
		return { ResourceID::None, ResourceID::Fish, ResourceID::Whale };
	default:
		return {};
	}
}