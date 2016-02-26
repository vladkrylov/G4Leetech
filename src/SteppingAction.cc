#include <cmath>

#include "g4root.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"

#include "DetectorConstruction.hh"
#include "SteppingAction.hh"
#include "SensitiveXZPlane.hh"

SteppingAction::SteppingAction()
{
	DetectorConstruction* geometry = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	p1 = geometry->p1;
}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
	G4int BranchId;
	G4double Px, Py, Pz, P;
	G4double theta;

    // Get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();


//	// get volume of the current step
//	G4VPhysicalVolume* volume = aStep->GetPostStepPoint()->GetPhysicalVolume();
//	if (!volume) return;
////
//	if(volume->GetName() == "SenDet1") {
    if (p1->Crossed(aStep)) {
		Px = aStep->GetPostStepPoint()->GetMomentum().getX();
		Py = aStep->GetPostStepPoint()->GetMomentum().getY();
		Pz = aStep->GetPostStepPoint()->GetMomentum().getZ();
		P = sqrt(Px*Px + Py*Py + Pz*Pz);

		theta = acos(Pz/P) * 180 / M_PI;

		// save the parameters of the step
		analysisManager->FillNtupleIColumn(BranchId=0, 1);
		analysisManager->FillNtupleIColumn(BranchId=1, aStep->GetTrack()->GetTrackID());
		analysisManager->FillNtupleIColumn(BranchId=2, aStep->GetTrack()->GetParentID());
		analysisManager->FillNtupleDColumn(BranchId=3, aStep->GetTrack()->GetKineticEnergy());
		analysisManager->FillNtupleDColumn(BranchId=4, aStep->GetPreStepPoint()->GetGlobalTime());
		analysisManager->FillNtupleDColumn(BranchId=5, aStep->GetPreStepPoint()->GetPosition().getX());
		analysisManager->FillNtupleDColumn(BranchId=6, aStep->GetPreStepPoint()->GetPosition().getY());
		analysisManager->FillNtupleDColumn(BranchId=7, aStep->GetPreStepPoint()->GetPosition().getZ());
		analysisManager->FillNtupleDColumn(BranchId=8, Px);
		analysisManager->FillNtupleDColumn(BranchId=9, Py);
		analysisManager->FillNtupleDColumn(BranchId=10, Pz);
		analysisManager->FillNtupleDColumn(BranchId=11, P);
		analysisManager->FillNtupleDColumn(BranchId=12, theta);
		analysisManager->FillNtupleDColumn(BranchId=13, aStep->GetStepLength());
		analysisManager->AddNtupleRow();

		aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	}
}

