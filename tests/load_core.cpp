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
  std::cout << "Shutting down\n";
  return 0;
}
