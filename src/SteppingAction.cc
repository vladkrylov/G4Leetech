#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"

#include "TTree.h"

#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

SteppingAction::SteppingAction(){
  detector = (DetectorConstruction*)
    G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  eventAction = (EventAction*)
    G4RunManager::GetRunManager()->GetUserEventAction();	       
  runAction = (RunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
}

SteppingAction::~SteppingAction()
{ }

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
	// get volume of the current step
	G4VPhysicalVolume* volume
		= aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
	//G4cout<<volume->GetName()<<G4endl;
  
	runAction->HitInfo.IsInDetector = 0;
	runAction->HitInfo.TypeID   = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();

	//fill the parameters of the step
	if(volume->GetName()=="SenDet1") { //|| volume->GetName()=="SenDet2"){
		//G4cout<<"### Run "<<G4RunManager::GetRunManager()->GetRunID()<<G4endl
		//	  <<aStep->GetTrack()->GetKineticEnergy()<<G4endl;
		//	G4cout<<aStep->GetTrack()->GetDefinition()->GetPDGEncoding()<<std::endl;
		if(aStep->GetTrack()->GetDefinition()->GetPDGEncoding()==11) {
			runAction->HitInfo.IsInDetector = 1;
		  	runAction->HitInfo.EventID  = eventAction->_evtNb;
			runAction->HitInfo.TypeID   = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
			if(aStep->GetTrack()->GetDefinition()->GetPDGEncoding() == 22)
				G4cout<<"gamma was 100%\n";
			runAction->HitInfo.TrackID  = aStep->GetTrack()->GetTrackID();
			runAction->HitInfo.ParentID = aStep->GetTrack()->GetParentID();
			runAction->HitInfo.Energy   = aStep->GetTrack()->GetKineticEnergy();
			runAction->HitInfo.Time     = aStep->GetPostStepPoint()->GetGlobalTime();
			runAction->HitInfo.PosX     = aStep->GetPostStepPoint()->GetPosition().getX();
			runAction->HitInfo.PosY     = aStep->GetPostStepPoint()->GetPosition().getY();
			runAction->HitInfo.PosZ     = aStep->GetPostStepPoint()->GetPosition().getZ();
			runAction->HitInfo.PX       = aStep->GetPostStepPoint()->GetMomentum().getX();
			runAction->HitInfo.PY       = aStep->GetPostStepPoint()->GetMomentum().getY();
			runAction->HitInfo.PZ       = aStep->GetPostStepPoint()->GetMomentum().getZ();
	
			double Pe=sqrt(runAction->HitInfo.PZ*runAction->HitInfo.PZ+
						   runAction->HitInfo.PY*runAction->HitInfo.PY+
						   runAction->HitInfo.PX*runAction->HitInfo.PX);
			runAction->HitInfo.P = Pe;
	
			double Tet = acos(runAction->HitInfo.PZ/Pe)*180/3.14;
			runAction->HitInfo.Theta = Tet;

			double PosXYnow = sqrt(runAction->HitInfo.PosX*runAction->HitInfo.PosX+runAction->HitInfo.PosY*runAction->HitInfo.PosY);
			runAction->HitInfo.PosXY = PosXYnow;

			runAction->tree->Fill();
		}
		aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	}
}

