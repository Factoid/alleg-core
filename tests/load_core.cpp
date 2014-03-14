#include <igc/pch.h>
#include <igc/igc.h>
#include <igc/missionIGC.h>
#include <iostream>
#include <thread>

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
  
  mission.Initialize( Clock::now(), &dummySite );
  mission.SetMissionParams( &params );
  LoadIGCStaticCore( "cc_14", &mission, false, nullptr );
  mission.EnterGame();

  DataShipIGC sd;
  memset(&sd,0,sizeof(sd));
  sd.shipID = mission.GenerateNewShipID();
  sd.sideID = NA;
  sd.hullID = 210;
  sd.pilotType = c_ptPlayer;
  sd.abmOrders = 0;
  sd.baseObjectID = NA;
  strcpy(sd.name,"foo");

  Time t = Clock::now();
  IshipIGC* ship = (IshipIGC*)mission.CreateObject(t, OT_ship, &sd, sizeof(sd));
  std::cout << "Created ship\n";
  
  for( int i = 0; i < 10; ++i )
  {
    t += Duration(0.1f);
    std::cout << "Tick!\n";
    mission.Update(t);
  }

  std::cout << "Shutting down\n";
  return 0;
}
