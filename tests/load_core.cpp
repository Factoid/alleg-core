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
public:
  Time ServerTimeFromClientTime( Time clTime )
  {
//    std::cout << "IGCSite::ServerTimeFromClientTime()\n";
    return IIgcSite::ServerTimeFromClientTime( clTime );
  }

  void Preload( const char* mn, const char* tn )
  {
//    std::cout << "IGCSite::Preload( " << (mn == nullptr ? "" : mn) << ", " << (tn == nullptr ? "" : tn) << ")\n" << std::flush;
    IIgcSite::Preload( mn, tn );
  }

  void KillShipEvent( Time now, IshipIGC* ship, ImodelIGC* launcher, float amount, const Vector& p1, const Vector& p2 )
  {
    ship->Kill(now,nullptr,amount,p1,p2);
  }
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

  describe( "CVH Tests", []() {
    MultiHullBasePtr hitTestA = HitTest::Load("ss93");
    AssertThat( hitTestA.get() == nullptr, IsFalse() );
    AssertThat( hitTestA->GetFrame("launch1") == nullptr, IsFalse() );
  });

  describe( "ImissionIGC", []() {
    DummyIgcSite dummySite;
    CmissionIGC mission;

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
      MissionParams params;
      params.strGameName = std::string("My Test Game");
      params.nTeams = 2;
      params.rgCivID = { 18, 18 };
      params.mmMapType = c_mmBrawl;
      mission.SetMissionParams( &params );
      AssertThat( mission.GetMissionParams()->strGameName, Equals("My Test Game") );
    });

    it( "can load a core", [&]() {
      LoadIGCStaticCore( "cc_14", &mission, false, nullptr );
      AssertThat( mission.GetStaticCore() == nullptr, IsFalse() );
      AssertThat( *mission.GetDroneTypes(), !IsEmpty() );
      AssertThat( *mission.GetStationTypes(), !IsEmpty() );
      AssertThat( *mission.GetExpendableTypes(), !IsEmpty() );
      AssertThat( *mission.GetPartTypes(), !IsEmpty() );
      AssertThat( *mission.GetProjectileTypes(), !IsEmpty() );
      AssertThat( *mission.GetDevelopments(), !IsEmpty() );
      AssertThat( *mission.GetCivilizations(), !IsEmpty() );
    });

    it( "can set start time", [&]() {
      AssertThat( Seconds(mission.GetMissionParams()->timeStart-t).count(), !EqualsWithDelta(0.0f,0.01f) );
      mission.SetStartTime( t );
      AssertThat( Seconds(mission.GetMissionParams()->timeStart-t).count(), EqualsWithDelta(0.0f,0.01f) );
    });

    it_skip( "can generate sides", [&]() {
      AssertThat( mission.GetStaticCore() != nullptr, IsTrue() );
      DataSideIGC dSide("Team 1");
      dSide.civilizationID = 18;
      dSide.sideID = 0;
      dSide.gasAttributes.Initialize();
      dSide.color = Color(1.0f,0.0f,0.0f);
      IsideIGC* civ1 = (IsideIGC*)mission.CreateObject(t, OT_side, &dSide, sizeof(dSide));
      AssertThat( *mission.GetSides(), Contains(civ1) );

      dSide = DataSideIGC("Team 2");
      dSide.civilizationID = 18;
      dSide.sideID = 1;
      dSide.gasAttributes.Initialize();
      dSide.color = Color(0.0f,0.0f,1.0f);
      IsideIGC* civ2 = (IsideIGC*)mission.CreateObject(t, OT_side, &dSide, sizeof(dSide));
      AssertThat( *mission.GetSides(), Contains(civ2) );
    });

    it( "can update sides", [&]() {
      const char sideNames[c_cSidesMax][c_cbSideName] = { "foo", "bar" };
      AssertThat( mission.GetMissionParams()->nTeams, Equals(2) );
      mission.UpdateSides( t, mission.GetMissionParams(), sideNames );
      AssertThat( mission.GetSide(0)->GetName(), Equals("foo") );
      AssertThat( mission.GetSide(1)->GetName(), Equals("bar") );
    });

    it( "can generate mission", [&]() {
      AssertThat( mission.GetStaticCore() != nullptr, IsTrue() );
      AssertThat( *mission.GetSides(), !IsEmpty() );
      mission.GenerateMission( t, mission.GetMissionParams(), nullptr, nullptr );
      AssertThat( *mission.GetClusters(), !IsEmpty() );
      AssertThat( *mission.GetWarps(), IsEmpty() );
      AssertThat( *mission.GetStations(), !IsEmpty() );
      AssertThat( *mission.GetAsteroids(), !IsEmpty() );
      AssertThat( *mission.GetTreasures(), !IsEmpty() );
//      AssertThat( *mission.GetShips(), !IsEmpty() );
      AssertThat( *mission.GetSides(), !IsEmpty() );
    });

    it( "can enter game", [&]() {
      if( mission.GetMissionParams()->IsProsperityGame() )
      {
        AssertThat( *mission.GetSide(0)->GetBuckets(), IsEmpty());
      }
      mission.EnterGame();
      if( mission.GetMissionParams()->IsProsperityGame() )
      {
        AssertThat( *mission.GetSide(0)->GetBuckets(), !IsEmpty());
      }
      for( auto tre : *mission.GetTreasures() )
      {
        AssertThat( tre->GetLastUpdate(), Equals(t) );
      }
      for( auto st : *mission.GetStations() )
      {
        AssertThat( st->GetLastUpdate(), Equals(t) );
      }
    });

    it( "can update time", [&]() {
      AssertThat( mission.GetMissionStage(), Equals(STAGE_NOTSTARTED) );
      mission.SetMissionStage(STAGE_STARTED);
      AssertThat( mission.GetMissionStage(), Equals(STAGE_STARTED) );
      Seconds interval(0.1f);
      while( (t - appStart) < Seconds(60.0f) )
      {
        t += interval;
        mission.Update(t);
      }
      AssertThat( (mission.GetLastUpdate() - t), Equals( Time::duration(0) ) );
      AssertThat( (mission.GetLastUpdate() - appStart ), Equals( Seconds(60.0f) ) );
    });

    it( "can launch a ship from a station", [&]() {
      IsideIGC* side0 = mission.GetSide(0);
      AssertThat( side0 == nullptr, IsFalse() );
      AssertThat( *side0->GetShips(), IsEmpty() );
      AssertThat( side0->GetStations()->size(), Equals<unsigned long>(1) );

      DataShipIGC shipData;
      shipData.shipID = mission.GenerateNewShipID();
      shipData.hullID = 210; // The Factoid scout
      shipData.sideID = side0->GetObjectID();
      shipData.nKills = 0;
      shipData.nDeaths = 0;
      shipData.pilotType = c_ptPlayer;
      shipData.nEjections = 0;
      strcpy(shipData.name,"Factoid");
      shipData.nParts = 0;
      shipData.baseObjectID = NA;
       
      IshipIGC* ship = (IshipIGC*)mission.CreateObject(mission.GetLastUpdate(),OT_ship,&shipData,sizeof(shipData));
      AssertThat( ship == nullptr, IsFalse() );

      IstationIGC* station = side0->GetStation(0);
      AssertThat( station == nullptr, IsFalse() );
      station->RepairAndRefuel(ship);
      station->Launch(ship);
      AssertThat( ship->GetAmmo(), Equals(ship->GetHullType()->GetMaxAmmo()) );
      AssertThat( ship->GetEnergy(), Equals(ship->GetHullType()->GetMaxEnergy()) );
      AssertThat( ship->GetFuel(), Equals(ship->GetHullType()->GetMaxFuel()) );
      Seconds interval(0.1f);
      Time startT = t;
      ControlData cd;
      cd.jsValues[c_axisThrottle] = 1.0f;
      ship->SetControls(cd);
      while( (t - startT) < Seconds(10.0f) )
      {
        t += interval;
        mission.Update(t);
      }
      AssertThat( ship->GetVelocity().LengthSquared(), IsGreaterThan(1.0f) );
    });

    it( "can fly into an asteroid", [&]() {
      IshipIGC* ship = mission.GetShip(0);
      AssertThat( ship == nullptr, IsFalse() );
      IclusterIGC* cluster = ship->GetCluster();
      AssertThat( cluster == nullptr, IsFalse() );
      IasteroidIGC* rock = cluster->GetAsteroid(0);
      AssertThat( rock == nullptr, IsFalse() );
      ship->SetPosition( rock->GetPosition() + (rock->GetRadius() + 100) * rock->GetOrientation().GetForward() );
      ship->SetOrientation( Orientation( rock->GetOrientation().GetForward() * -1, rock->GetOrientation().GetUp() ) );
      AssertThat( ship->GetOrientation().GetForward() * rock->GetOrientation().GetForward(), EqualsWithDelta(-1.0f,0.01f) );
      ship->SetVelocity( ship->GetOrientation().GetForward() * 160.0f );
      ControlData cd;
      cd.jsValues[c_axisThrottle] = 1.0f;
      ship->SetControls(cd);
      Time startT = t;
      Seconds interval(0.1f);
      while( (t - startT) < Seconds(20.0f) )
      {
        t += interval;
        mission.Update(t);
      }
      AssertThat( ship->GetHullType()->GetName(), Equals("Lifepod") );
    });

    it( "can kill the lifepod", [&]() {
      IshipIGC* ship = mission.GetShip(0);
      AssertThat( ship == nullptr, IsFalse() );
      AssertThat( ship->GetHullType()->GetName(), Equals("Lifepod") );
      ship->ReceiveDamage(c_dmgidCollision, 1000, mission.GetLastUpdate(), ship->GetPosition(), Vector::GetZero(), NULL);
    });

    it( "can terminate", [&]() {
      mission.Terminate();
      AssertThat( *mission.GetClusters(), IsEmpty() );
      AssertThat( *mission.GetWarps(), IsEmpty() );
      AssertThat( *mission.GetStations(), IsEmpty() );
      AssertThat( *mission.GetAsteroids(), IsEmpty() );
      AssertThat( *mission.GetTreasures(), IsEmpty() );
      AssertThat( *mission.GetShips(), IsEmpty() );
      AssertThat( *mission.GetSides(), IsEmpty() );
    });
  });
});

int main( int argc, char** argv )
{
  return bandit::run(argc,argv);
}
