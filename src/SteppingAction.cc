#include <cmath>

#include "g4root.hh"
#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"

#include "SteppingAction.hh"

SteppingAction::SteppingAction()
{

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

	// get volume of the current step
	G4VPhysicalVolume* volume = aStep->GetPostStepPoint()->GetPhysicalVolume();
	if (!volume) return;

	if(volume->GetName() == "SenDet1") {
//		if(aStep->GetTrack()->GetDefinition()->GetPDGEncoding() == 11) {
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
			analysisManager->FillNtupleDColumn(BranchId=4, aStep->GetPostStepPoint()->GetGlobalTime());
			analysisManager->FillNtupleDColumn(BranchId=5, aStep->GetPostStepPoint()->GetPosition().getX());
			analysisManager->FillNtupleDColumn(BranchId=6, aStep->GetPostStepPoint()->GetPosition().getY());
			analysisManager->FillNtupleDColumn(BranchId=7, aStep->GetPostStepPoint()->GetPosition().getZ());
			analysisManager->FillNtupleDColumn(BranchId=8, Px);
			analysisManager->FillNtupleDColumn(BranchId=9, Py);
			analysisManager->FillNtupleDColumn(BranchId=10, Pz);
			analysisManager->FillNtupleDColumn(BranchId=11, P);
			analysisManager->FillNtupleDColumn(BranchId=12, theta);
			analysisManager->AddNtupleRow();
//		}
		aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	}
}

