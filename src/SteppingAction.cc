#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"

//root
#include "TTree.h"


SteppingAction::SteppingAction(){
  detector = (DetectorConstruction*)
    G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  eventaction = (EventAction*)
    G4RunManager::GetRunManager()->GetUserEventAction();	       
  runaction = (RunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep){
  
  // get volume of the current step
  G4VPhysicalVolume* volume 
    = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  //G4cout<<volume->GetName()<<G4endl;
  
  runaction->HitInfo.IsInDetector = 0;
  runaction->HitInfo.TypeID   = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
//fill the para,eters of the step
  if(volume->GetName()=="SenDet1"){ //|| volume->GetName()=="SenDet2"){
    //G4cout<<"### Run "<<G4RunManager::GetRunManager()->GetRunID()<<G4endl
    //	  <<aStep->GetTrack()->GetKineticEnergy()<<G4endl;    
	//	G4cout<<aStep->GetTrack()->GetDefinition()->GetPDGEncoding()<<std::endl;
	  if(aStep->GetTrack()->GetDefinition()->GetPDGEncoding()==11)
	  {
		  	runaction->HitInfo.IsInDetector = 1;
		  	runaction->HitInfo.EventID  = eventaction->_evtNb;
			runaction->HitInfo.TypeID   = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
                        if(aStep->GetTrack()->GetDefinition()->GetPDGEncoding()==22)G4cout<<"gamma was 100%\n";
			runaction->HitInfo.TrackID  = aStep->GetTrack()->GetTrackID();
			runaction->HitInfo.ParentID = aStep->GetTrack()->GetParentID();
			runaction->HitInfo.Energy   = aStep->GetTrack()->GetKineticEnergy();
			runaction->HitInfo.Time     = aStep->GetPostStepPoint()->GetGlobalTime();
			runaction->HitInfo.PosX     = aStep->GetPostStepPoint()->GetPosition().getX();
			runaction->HitInfo.PosY     = aStep->GetPostStepPoint()->GetPosition().getY();
			runaction->HitInfo.PosZ     = aStep->GetPostStepPoint()->GetPosition().getZ();
			runaction->HitInfo.PX       = aStep->GetPostStepPoint()->GetMomentum().getX();
			runaction->HitInfo.PY       = aStep->GetPostStepPoint()->GetMomentum().getY();
			runaction->HitInfo.PZ       = aStep->GetPostStepPoint()->GetMomentum().getZ();
	
		double Pe=sqrt(runaction->HitInfo.PZ*runaction->HitInfo.PZ+
						 runaction->HitInfo.PY*runaction->HitInfo.PY+
						 runaction->HitInfo.PX*runaction->HitInfo.PX);
		 runaction->HitInfo.P = Pe;
	
	double Tet = acos(runaction->HitInfo.PZ/Pe)*180/3.14;
		runaction->HitInfo.Theta = Tet;

		double PosXYnow = sqrt(runaction->HitInfo.PosX*runaction->HitInfo.PosX+runaction->HitInfo.PosY*runaction->HitInfo.PosY);
		runaction->HitInfo.PosXY = PosXYnow;

			runaction->tree->Fill();
		}
			aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	  }
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
