#include <ctime>
#include <vector>

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"
#include "Randomize.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"

#include "RunAction.hh"
#include "RunActionMessenger.hh"
#include "DetectorConstruction.hh"
#include "SensitiveXZPlane.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
: G4UserRunAction()
, rootFileName("leetech")
{
	runMessenger = new RunActionMessenger(this);

    // automatic (time-based) random seeds for each run
    G4cout << "*******************" << G4endl;
    G4cout << "*** AUTOSEED ON ***" << G4endl;
    G4cout << "*******************" << G4endl;
    long seeds[2];
    time_t systime = time(NULL);
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());
    G4Random::setTheSeeds(seeds);
    G4Random::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
  delete runMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
	// Get the information about detectors from DetectorConstruction
	DetectorConstruction* geometry = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	const std::vector<SensitiveXZPlane*>* detectors = geometry->GetPlaneDetectorList();

    // Create analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetFileName(rootFileName);

    // Creating ntuples
    for(size_t i=0; i<detectors->size(); i++)
    {
		volatile int id = analysisManager->CreateNtuple(detectors->at(i)->GetName(), "Leetech simulation results");
		analysisManager->CreateNtupleIColumn("TypeID");
		analysisManager->CreateNtupleIColumn("TrackID");
		analysisManager->CreateNtupleIColumn("ParentID");
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
		analysisManager->CreateNtupleDColumn("PDGEncoding");
		analysisManager->FinishNtuple();
    }

	// Open an output file
	// The default file name is set in RunAction::RunAction(),
	// it can be overwritten in a macro
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



