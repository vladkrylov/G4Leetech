#include "RunAction.hh"

#include "G4Run.hh"
#include "RunActionMessenger.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "G4MPImanager.hh"
#include "TString.h"

#include "Randomize.hh"
#include <ctime>

//root
#include "TTree.h"
#include "TFile.h"

RunAction::RunAction()
{
  runMessenger = new RunActionMessenger(this);
  _RootFile = "micromegas.root";
  
  systime = time(NULL);
}


RunAction::~RunAction()
{
  delete runMessenger;
}


void RunAction::BeginOfRunAction(const G4Run* aRun){ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  G4int rank= G4MPImanager::GetManager()-> GetRank();
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  //G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");
bool autoSeed = true;
    if (autoSeed) {
     // automatic (time-based) random seeds for each run
     G4cout << "*******************" << G4endl;
     G4cout << "*** AUTOSEED ON ***" << G4endl;
     G4cout << "*******************" << G4endl;
     long seeds[2];
     //time_t systime = time(NULL);
     seeds[0] = (long) systime*(rank+1);
     seeds[1] = (long) (systime*G4UniformRand()/(rank+1));
     CLHEP::HepRandom::setTheSeeds(seeds);
     CLHEP::HepRandom::showEngineStatus();
  } else {
     CLHEP::HepRandom::showEngineStatus();
  }
  
  
  char str[64];
  sprintf(str, "%03d", rank);
  TString fname(str);
  TString resName = _RootFile + fname + ".root";
  //LB 09.02.2011
  //hfile = new TFile(_RootFile.Data(), "RECREATE", "Simulation", 1);
  hfile = new TFile(resName.Data(), "RECREATE", "Simulation", 1);
  if (hfile->IsZombie()) exit(-1);
  tree = new TTree("T", "Data Tree");
  hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);
  // Create new event
  TTree::SetBranchStyle(0);
  //Event
  tree->Branch("IsInDetector",  &HitInfo.IsInDetector, "IsInDetector/I");
  tree->Branch("EventID",  &HitInfo.EventID, "Event/I");
  tree->Branch("TypeID",   &HitInfo.TypeID,  "TypeID/I");
  tree->Branch("TrackID",  &HitInfo.TrackID, "TrackID/I");
  tree->Branch("ParentID", &HitInfo.ParentID,"ParentID/I");
  tree->Branch("Energy",   &HitInfo.Energy,  "Energy/D");
  tree->Branch("Time",     &HitInfo.Time,    "Time/D");
  tree->Branch("PosX",     &HitInfo.PosX,    "PosX/D");
  tree->Branch("PosY",     &HitInfo.PosY,    "PosY/D");
  tree->Branch("PosZ",     &HitInfo.PosZ,    "PosZ/D");
  tree->Branch("PosXY",    &HitInfo.PosXY,   "PosXY/D");
  tree->Branch("PX",       &HitInfo.PX,      "PX/D");
  tree->Branch("PY",       &HitInfo.PY,      "PY/D");
  tree->Branch("PZ",       &HitInfo.PZ,      "PZ/D");
  tree->Branch("Ptot",	   &HitInfo.P ,      "P/D");
  tree->Branch("Theta",    &HitInfo.Theta,   "Theta/D");

}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4cout << "Eand of " << aRun->GetRunID() << "run Acrion." << G4endl;
  
  hfile = tree->GetCurrentFile();
  hfile->Write();
  tree->Print();

  delete tree;
  delete hfile;
}

void RunAction::SetRootFile(G4String newValue){
  _RootFile = newValue;
}

