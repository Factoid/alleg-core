#include <bandit/bandit.h>
#include <igc/pch.h>
#include <igc/igc.h>
#include <igc/missionIGC.h>
#include <iostream>
#include <thread>

using namespace bandit;

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

Time appStart;
float elapsed( Time now ) { return (now - appStart).count(); }

go_bandit( []() {
  UTL::SetArtPath("../../../Artwork/");

  describe( "Time", []() {
    Time t;
    Time v;
    t = Clock::now();
    v = t;
    it( "is equal", [&]() {
      AssertThat( (t - v).count(), Equals(0) );
    });

    it( "can increment", [&]() {
      t += ms(500);
      AssertThat( std::chrono::duration_cast<ms>(t - v).count(), EqualsWithDelta(500,1) );
      AssertThat( Seconds( t - v ).count(), EqualsWithDelta(0.5f,0.01f) );
      t = v + Seconds(1.3f);
      AssertThat( Seconds( t - v ).count(), EqualsWithDelta(1.3f,0.01f) );
    });

  });

  describe( "ImissionIGC", []() {
    DummyIgcSite dummySite;
    CmissionIGC mission;
    MissionParams params;
    params.strGameName = std::string("My Test Game").c_str();

    Time t = appStart = Clock::now();

    it( "can initialize", [&]() {
      AssertThat( mission.GetIgcSite() == nullptr, IsTrue() );
      AssertThat( mission.GetLastUpdate(), Equals(Time()) );
      mission.Initialize( t, &dummySite );
      AssertThat( mission.GetIgcSite() == &dummySite, IsTrue() );
      AssertThat( mission.GetLastUpdate(), Equals(appStart) );
    });

    it( "can set mission params", [&]() {
      AssertThat( mission.GetMissionParams()->strGameName, Equals("Uninitialized Game Name") );
      mission.SetMissionParams( &params );
      AssertThat( mission.GetMissionParams()->strGameName, Equals("My Test Game") );
    });

    it( "can load a core", [&]() {
      LoadIGCStaticCore( "cc_14", &mission, false, nullptr );
    });

    it( "can set start time", [&]() {
      AssertThat( Seconds(mission.GetMissionParams()->timeStart-t).count(), !EqualsWithDelta(0.0f,0.01f) );
      mission.SetStartTime( t );
      AssertThat( Seconds(mission.GetMissionParams()->timeStart-t).count(), EqualsWithDelta(0.0f,0.01f) );
    });

    it( "can generate sides", [&]() {
      DataSideIGC dSide("Team 1");
      dSide.civilizationID = 18;
      dSide.sideID = 0;
      dSide.gasAttributes.Initialize();
      dSide.color = Color(1.0f,0.0f,0.0f);
      IsideIGC* civ1 = (IsideIGC*)mission.CreateObject(t, OT_side, &dSide, sizeof(dSide));

      dSide = DataSideIGC("Team 2");
      dSide.civilizationID = 18;
      dSide.sideID = 1;
      dSide.gasAttributes.Initialize();
      dSide.color = Color(0.0f,0.0f,1.0f);
      IsideIGC* civ2 = (IsideIGC*)mission.CreateObject(t, OT_side, &dSide, sizeof(dSide));
    });

    it( "can generate mission", [&]() {
      mission.GenerateMission( t, &params, nullptr, nullptr ); 
    });

    it( "can terminate", [&]() {
      mission.Terminate();
      AssertThat( *mission.GetTreasures(), IsEmpty() );
      AssertThat( *mission.GetDroneTypes(), IsEmpty() );
      AssertThat( *mission.GetStationTypes(), IsEmpty() );
      AssertThat( *mission.GetExpendableTypes(), IsEmpty() );
      AssertThat( *mission.GetPartTypes(), IsEmpty() );
      AssertThat( *mission.GetProjectileTypes(), IsEmpty() );
      AssertThat( *mission.GetDevelopments(), IsEmpty() );
      AssertThat( *mission.GetCivilizations(), IsEmpty() );
    });
  });
});

int main( int argc, char** argv )
{
  return bandit::run(argc,argv);
}
/*
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
  std::cout << "Created side\n";

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
  ship->Update(t);
  std::cout << "Created ship\n";

  IshieldIGC* shield = nullptr;
  std::vector<IweaponIGC*> weapons;
  for( auto p : *ship->GetHullType()->GetPreferredPartTypes() )
  {
    std::cout << "Ship wants a " << p->GetName() << " " << p->GetEquipmentType() << "\n";
    Mount maxMounts = (p->GetEquipmentType() == ET_Weapon ) ? ship->GetHullType()->GetMaxWeapons() : 1;
    for( Mount i = 0; i < maxMounts; ++i )
    {
      std::cout << "Creating and adding part\n";
      IpartIGC* cp = ship->CreateAndAddPart(p,i, 0x7fff);
      switch( p->GetEquipmentType() )
      {
        case ET_Weapon:
          weapons.push_back(dynamic_cast<IweaponIGC*>(cp));
          break;
        case ET_Shield:
          shield = dynamic_cast<IshieldIGC*>(cp);
          break;
      }
      std::cout << "Part added\n";
    }
  }
  
  std::cout << "Starting mission\n";
  mission.SetMissionStage(STAGE_STARTED);

  std::cout << "Advancing Time\n";
  for( int i = 0; i < 10; ++i )
  {
    t += Duration(0.1f);
    std::cout << "Tick! " << elapsed(t) << "\n";
    std::cout << "Pos: " << ship->GetPosition() << " Forward: " << ship->GetOrientation().GetForward() << " Ammo: " << ship->GetAmmo() << "\n";
    std::cout << "Shield : " << shield->GetFraction()*shield->GetMaxStrength() << " / " << shield->GetMaxStrength() << "\n";
    mission.Update(t);
  }

  std::cout << "Shutting down\n";
  return 0;
}
*/
