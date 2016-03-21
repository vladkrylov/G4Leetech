#include<G4Step.hh>
#include<fstream>
#include<iostream>
#include"SensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
//#include "Randomize.hh"
using namespace std;

SensitiveDetector::SensitiveDetector(G4String name): G4VSensitiveDetector(name)
{
	myfile.open ("data_out.txt");
	    myfile  <<std::setw(12)<<"%X "<<std::setw(12)<< "Y "<<std::setw(12)
	     <<"Z "<<std::setw(12)
	    <<"Theta "<<std::setw(12)
	    <<"Phi " <<std::setw(12)
	    <<"En_kin"<<std::setw(12)
	    <<"Charge "<<std::setw(12)<<" "<<std::endl;

}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *hist)
{
	 G4StepPoint* point1 = step->GetPreStepPoint();
	 myfile <<std::setw(12)<<point1->GetPosition().getX()<<std::setw(12)<<point1->GetPosition().getY()<<std::setw(12)<<point1->GetPosition().getZ()<<std::setw(12)
	              <<point1->GetPosition().getTheta()<<std::setw(12)<<point1->GetPosition().getPhi()<<std::setw(12)
	         <<point1->GetKineticEnergy()<<std::setw(12)
	          <<point1->GetCharge()<<std::setw(12)

			 <<std::endl;

  step->GetTrack()->SetTrackStatus(fStopAndKill);
  return true;
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
}



SensitiveDetector::~SensitiveDetector()
{  myfile.close();}
