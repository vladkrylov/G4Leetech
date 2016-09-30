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
		det_PDG = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
		det_eDep = 0;
		det_eBefore = aStep->GetTrack()->GetKineticEnergy();
		det_time = aStep->GetPostStepPoint()->GetGlobalTime();
		det_posX = aStep->GetPostStepPoint()->GetPosition().getX();
		det_posY = aStep->GetPostStepPoint()->GetPosition().getY();
		det_posZ = aStep->GetPostStepPoint()->GetPosition().getZ();
		det_theta = aStep->GetPostStepPoint()->GetMomentumDirection().theta() * 180 / M_PI;
		det_maxStepLength = 0;
	} else {
		// particle is inside diamond
		det_eDep += aStep->GetTotalEnergyDeposit();
		if (aStep->GetStepLength() > det_maxStepLength)
			det_maxStepLength = aStep->GetStepLength();
	}
	} else {
		if (aStep->GetPreStepPoint()->GetPhysicalVolume() != diamond) {
			// particle leaves the diamond

		}
	}

}

void SteppingAction::ResetEDep()
{
	det_eDep = 0.;
}

void SteppingAction::FillDiamondTuple()
{
	int diamondTupleID = detectors->size();  // diamond tuple is the next after all ghost detectors
	analysisManager->FillNtupleIColumn(diamondTupleID, BranchId=0, det_PDG);
	analysisManager->FillNtupleDColumn(diamondTupleID, BranchId=1, det_eBefore);
	analysisManager->FillNtupleDColumn(diamondTupleID, BranchId=2, det_eDep);
	analysisManager->FillNtupleDColumn(diamondTupleID, BranchId=3, det_time);
	analysisManager->FillNtupleDColumn(diamondTupleID, BranchId=4, det_posX);
	analysisManager->FillNtupleDColumn(diamondTupleID, BranchId=5, det_posY);
	analysisManager->FillNtupleDColumn(diamondTupleID, BranchId=6, det_posZ);
	analysisManager->FillNtupleDColumn(diamondTupleID, BranchId=7, det_theta);
	analysisManager->FillNtupleDColumn(diamondTupleID, BranchId=8, det_maxStepLength);
	analysisManager->AddNtupleRow(diamondTupleID);
}
