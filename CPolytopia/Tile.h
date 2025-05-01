#pragma once  
#include "IDs.h"  
#include "Unit.h" 

class Unit;
// Stores the information about what a tile contains  
class Tile {  
public:  
   Tile(  
       TerrainID terrain = TerrainID::Field,  
       TerrainAlterationID alteration = TerrainAlterationID::None,  
       ResourceID resource = ResourceID::None,  
       BuildingID building = BuildingID::None,  
       Unit* unit = nullptr);

   TerrainID getTerrain() const;  
   void setTerrain(TerrainID t);  

   TerrainAlterationID getAlteration() const;  
   bool setAlteration(TerrainAlterationID a);  

   ResourceID getResource() const;  
   bool setResource(ResourceID res);  

   BuildingID getBuilding() const;  
   bool setBuilding(BuildingID build);  

   Unit* getUnit();  
   const Unit* getUnit() const;  
   bool setUnit(Unit* unit);  
   UnitID getUnitType() const;  

   void check();  
private:  
   TerrainID m_terrain;  
   TerrainAlterationID m_alteration;  
   ResourceID m_resource;  
   BuildingID m_building;  
   Unit* m_unit;  
};
