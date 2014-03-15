#include <igc/pch.h>
#include <igc/igc.h>
#include <igc/missionIGC.h>
#include <iostream>
#include <thread>

std::ostream& operator<<( std::ostream& os, const Vector& v ) {
  os << v.x << ", " << v.y << ", " << v.z;
  return os;
}

std::ostream& operator<<( std::ostream& os, const Orientation& o ) {
  return os;
}

class DummyIgcSite : public IIgcSite
{
};

int main( int argc, char** argv )
{
  UTL::SetArtPath("../../../Artwork/");
  std::cout << "Creating mission\n";
  DummyIgcSite dummySite;
  CmissionIGC mission;
  MissionParams params;
  
  Time t = Clock::now();
  mission.Initialize( t, &dummySite );
  mission.SetMissionParams( &params );
  LoadIGCStaticCore( "cc_14", &mission, false, nullptr );
  mission.EnterGame();

  DataSideIGC dSide;
  memset(&dSide,0,sizeof(dSide));
  dSide.civilizationID = 18;
  dSide.sideID = 0;
  dSide.gasAttributes.Initialize();
  dSide.color = Color(1.0f,0.0f,0.0f);
  strcpy(dSide.name,"bar");
  IsideIGC* civ = (IsideIGC*)mission.CreateObject(t, OT_side, &dSide, sizeof(dSide));

  DataShipIGC dShip;
  memset(&dShip,0,sizeof(dShip));
  dShip.shipID = mission.GenerateNewShipID();
  dShip.sideID = 0;
  dShip.hullID = 210;
  dShip.pilotType = c_ptPlayer;
  dShip.abmOrders = 0;
  dShip.baseObjectID = NA;
  strcpy(dShip.name,"foo");

  IshipIGC* ship = (IshipIGC*)mission.CreateObject(t, OT_ship, &dShip, sizeof(dShip));
  std::cout << "Created ship\n";
  for( auto p : *ship->GetHullType()->GetPreferredPartTypes() )
  {
    std::cout << "Ship wants a " << p->GetName() << " " << p->GetEquipmentType() << "\n";
    Mount maxMounts = (p->GetEquipmentType() == ET_Weapon ) ? ship->GetHullType()->GetMaxWeapons() : 1;
    for( Mount i = 0; i < maxMounts; ++i )
    {
      std::cout << "Creating and adding part\n";
      ship->CreateAndAddPart(p,i, 0x7fff);
      std::cout << "Part added\n";
    }
  }
  
  for( int i = 0; i < 10; ++i )
  {
    t += Duration(0.1f);
    std::cout << "Tick!\n";
    std::cout << "Pos: " << ship->GetPosition() << " Forward: " << ship->GetOrientation().GetForward() << " Ammo: " << ship->GetAmmo() << "\n";
    for( auto p : *ship->GetParts() )
    {
      std::cout << "  Part: " << p->GetEquipmentType() << "\n";
    }
    mission.Update(t);
  }

  std::cout << "Shutting down\n";
  return 0;
}
