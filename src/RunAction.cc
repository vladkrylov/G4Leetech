#include <ctime>

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"
#include "Randomize.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"

#include "EventAction.hh"
#include "RunAction.hh"
#include "GhostDetector.hh"
#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
: G4UserRunAction()
, rootFileName("leetech")
{
    // automatic (time-based) random seeds for each run
    G4cout << "*******************" << G4endl;
    G4cout << "*** AUTOSEED ON ***" << G4endl;
    G4cout << "*******************" << G4endl;
    long seeds[2];
    time_t systime = time(NULL);
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());
//    seeds[0] = (long) 1;
//    seeds[1] = (long) 2;
    G4Random::setTheSeeds(seeds);
    G4Random::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
	// Get the information about detectors from DetectorConstruction
	DetectorConstruction* geometry = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	const std::vector<GhostDetector*>* detectors = geometry->GetPlaneDetectorList();

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetFileName(rootFileName);

    // Creating ntuples
    for(size_t i=0; i<detectors->size(); i++)
    {
    	analysisManager->CreateNtuple(detectors->at(i)->GetName(), "Leetech simulation results");

		analysisManager->CreateNtupleIColumn("TypeID");
		analysisManager->CreateNtupleIColumn("TrackID");
		analysisManager->CreateNtupleIColumn("ParentID");
		analysisManager->CreateNtupleIColumn("PDGEncoding");
		analysisManager->CreateNtupleDColumn("Energy");
		analysisManager->CreateNtupleDColumn("Time");
		analysisManager->CreateNtupleDColumn("PosX");
		analysisManager->CreateNtupleDColumn("PosY");
		analysisManager->CreateNtupleDColumn("PosZ");
		analysisManager->CreateNtupleDColumn("PX");
		analysisManager->CreateNtupleDColumn("PY");
		analysisManager->CreateNtupleDColumn("PZ");
		analysisManager->CreateNtupleDColumn("P");
		analysisManager->CreateNtupleDColumn("Theta");
		analysisManager->CreateNtupleDColumn("StepLength");
		analysisManager->FinishNtuple();
    }

	analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
	// save histograms & ntuple
	//
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->Write();
	analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
