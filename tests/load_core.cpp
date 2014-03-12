#include <igc/pch.h>
#include <igc/igc.h>
#include <igc/missionIGC.h>
#include <iostream>

class DummyIgcSite : public IIgcSite
{
};

int main( int argc, char** argv )
{
  UTL::SetArtPath("../../../Artwork/");
  std::cout << "Creating mission\n";
  DummyIgcSite dummySite;
  CmissionIGC mission;
  mission.Initialize( Clock::now(), &dummySite );

  LoadIGCStaticCore( "cc_14", &mission, false, nullptr );
  std::cout << "Part Types\n";
  for( auto p : *mission.GetPartTypes() )
  {
    std::cout << "  " << p->GetName() << " " << p->GetModelName() << " $" << p->GetPrice() << "\n";
  }
  std::cout << "Drone Types\n";
  for( auto p : *mission.GetDroneTypes() )
  {
    std::cout << "  " << p->GetName() << " " << p->GetModelName() << " $" << p->GetPrice() << "\n";
  }
  std::cout << "Developments\n";
  for( auto p : *mission.GetDevelopments() )
  {
    std::cout << "  " << p->GetName() << " " << p->GetModelName() << " $" << p->GetPrice() << "\n";
  }
  std::cout << "Station Types\n";
  for( auto p : *mission.GetStationTypes() )
  {
    std::cout << "  " << p->GetName() << " " << p->GetModelName() << " $" << p->GetPrice() << "\n";
  }
  std::cout << "Hull Types\n";
  for( auto p : *mission.GetHullTypes() )
  {
    std::cout << "  " << p->GetName() << " " << p->GetModelName() << " $" << p->GetPrice() << "\n";
  }
  std::cout << "Expendable Types\n";
  for( auto p : *mission.GetExpendableTypes() )
  {
    std::cout << "  " << p->GetName() << " " << p->GetModelName() << "\n";
  }
  std::cout << "Treasures\n";
  for( auto p : *mission.GetTreasureSets() )
  {
    std::cout << "  " << p->GetName() << "\n";
  }
  std::cout << "Projectile Types\n";
  for( auto p : *mission.GetProjectileTypes() )
  {
    std::cout << p->GetModelTexture() << " " << p->GetModelName() << " " << p->GetPower() << " " << p->GetSpeed() << "\n";
  }
  std::cout << "Ammo Pack\n";
  {
    auto p = mission.GetAmmoPack();
    std::cout << "  " << p->GetName() << " " << p->GetModelName() << " $" << p->GetPrice() << "\n";
  }
  std::cout << "Fuel Pack\n";
  {
    auto p = mission.GetFuelPack();
    std::cout << "  " << p->GetName() << " " << p->GetModelName() << " $" << p->GetPrice() << "\n";
  }

  std::cout << "Shutting down\n";
  return 0;
}
