#include <cmath>

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"

#include "EventAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "GhostDetector.hh"

SteppingAction::SteppingAction(EventAction* EvAct):G4UserSteppingAction(),fEventAction(EvAct)
{
	DetectorConstruction* geometry = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	detectors = geometry->GetPlaneDetectorList();
	diamond = geometry->GetDetectorPhys();

	ResetEDep();

    // Get analysis manager
    analysisManager = G4AnalysisManager::Instance();
}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
	for(size_t i=0; i < detectors->size(); i++) {
		if (detectors->at(i)->Crossed(aStep)
		&& (aStep->GetTrack()->GetDefinition()->GetPDGEncoding() == 11) // save only electrons to avoid Gb output files
		) {

			Px = aStep->GetPostStepPoint()->GetMomentum().getX();
			Py = aStep->GetPostStepPoint()->GetMomentum().getY();
			Pz = aStep->GetPostStepPoint()->GetMomentum().getZ();
			P = sqrt(Px*Px + Py*Py + Pz*Pz);

			theta = acos(Pz/P) * 180 / M_PI;

			// save the parameters of the step
			analysisManager->FillNtupleIColumn(i, BranchId=0, 1);
			analysisManager->FillNtupleIColumn(i, BranchId=1, aStep->GetTrack()->GetTrackID());
			analysisManager->FillNtupleIColumn(i, BranchId=2, aStep->GetTrack()->GetParentID());
			analysisManager->FillNtupleIColumn(i, BranchId=3, aStep->GetTrack()->GetDefinition()->GetPDGEncoding());
			analysisManager->FillNtupleDColumn(i, BranchId=4, aStep->GetTrack()->GetKineticEnergy());
			analysisManager->FillNtupleDColumn(i, BranchId=5, aStep->GetPostStepPoint()->GetGlobalTime());
			analysisManager->FillNtupleDColumn(i, BranchId=6, aStep->GetPostStepPoint()->GetPosition().getX());
			analysisManager->FillNtupleDColumn(i, BranchId=7, aStep->GetPostStepPoint()->GetPosition().getY());
			analysisManager->FillNtupleDColumn(i, BranchId=8, aStep->GetPostStepPoint()->GetPosition().getZ());
			analysisManager->FillNtupleDColumn(i, BranchId=9, Px);
			analysisManager->FillNtupleDColumn(i, BranchId=10, Py);
			analysisManager->FillNtupleDColumn(i, BranchId=11, Pz);
			analysisManager->FillNtupleDColumn(i, BranchId=12, P);
			analysisManager->FillNtupleDColumn(i, BranchId=13, theta);
			analysisManager->FillNtupleDColumn(i, BranchId=14, aStep->GetStepLength());
			analysisManager->AddNtupleRow(i);

//			aStep->GetTrack()->SetTrackStatus(fStopAndKill);
			break;
		}
	}

	if (aStep->GetPostStepPoint()->GetPhysicalVolume() == diamond) {
	if (aStep->GetPreStepPoint()->GetPhysicalVolume() != diamond) {
		// particle enters the diamond
		det_NPartPerEvent++;

		det_PDG = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
		det_eBefore = aStep->GetTrack()->GetKineticEnergy();
		det_time = aStep->GetPostStepPoint()->GetGlobalTime();
		det_posX = aStep->GetPostStepPoint()->GetPosition().getX();
		det_posY = aStep->GetPostStepPoint()->GetPosition().getY();
		det_posZ = aStep->GetPostStepPoint()->GetPosition().getZ();
		det_theta = aStep->GetPostStepPoint()->GetMomentumDirection().theta() * 180 / M_PI;
		FillDiamondHitsTuple();
	}}

	if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume() == diamond) {
		det_eDep += aStep->GetTotalEnergyDeposit();
	}
}

void SteppingAction::ResetEDep()
{
	det_NPartPerEvent = 0;
	det_eDep = 0.;
}

void SteppingAction::FillDiamondHitsTuple()
{
	int diamondHitsTupleID = detectors->size();  // diamond tuple is the next after all ghost detectors
	analysisManager->FillNtupleIColumn(diamondHitsTupleID, BranchId=0, det_PDG);
	analysisManager->FillNtupleDColumn(diamondHitsTupleID, BranchId=1, det_eBefore);
	analysisManager->FillNtupleDColumn(diamondHitsTupleID, BranchId=2, det_time);
	analysisManager->FillNtupleDColumn(diamondHitsTupleID, BranchId=3, det_posX);
	analysisManager->FillNtupleDColumn(diamondHitsTupleID, BranchId=4, det_posY);
	analysisManager->FillNtupleDColumn(diamondHitsTupleID, BranchId=5, det_posZ);
	analysisManager->FillNtupleDColumn(diamondHitsTupleID, BranchId=6, det_theta);
	analysisManager->AddNtupleRow(diamondHitsTupleID);
}

void SteppingAction::FillDiamondEventsTuple()
{
	int diamondEventsTupleID = detectors->size()+1;
	analysisManager->FillNtupleIColumn(diamondEventsTupleID, BranchId=0, det_NPartPerEvent);
	analysisManager->FillNtupleDColumn(diamondEventsTupleID, BranchId=1, det_eDep);
	analysisManager->AddNtupleRow(diamondEventsTupleID);
}

