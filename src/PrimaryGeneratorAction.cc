#include <cmath>

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"
#include "G4ParticleGun.hh"

#include "TFile.h"
#include "TTree.h"

#include "PrimaryGeneratorMessenger.hh"
#include "G4GeneralParticleSource.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction* PrimaryGeneratorAction::fgInstance = 0;
const PrimaryGeneratorAction* PrimaryGeneratorAction::Instance()
{
// Static acces function via G4RunManager
  return fgInstance;
}
PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
, _particleName("e-")

{
	fgInstance = this;
	fParticleGun = new G4ParticleGun();
	fParticleGun->SetNumberOfParticles(1);
	fParticleGun->SetParticleDefinition(FindParticle(_particleName));
	gunMessenger = new PrimaryGeneratorMessenger(this);

	Detector = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();

//	pgun->SetParticlePosition(G4ThreeVector(0.,0.,1.));
//	pgun->SetParticlePosition(Detector->GetBeamPipeCenter());

	// reading parameters from .root file
//	f = new TFile("/home/vlad/10g4work/LeetechRuns/EntranceCollScans/EntranceCollScan_7detectors/opening=19mm_E=3500keV/leetech.root");
//	tree = (TTree*)f->Get("ExitChamber");

	eventsCounter = 0;

	fParticleGun = new G4ParticleGun();
	fParticleGun->SetNumberOfParticles(1);
	fParticleGun->SetParticleDefinition(FindParticle("e-"));
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

	fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -10*cm));
	fParticleGun->SetParticleEnergy(16.*MeV);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete fParticleGun;
	delete gunMessenger;
	fgInstance = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
	for(int i=eventsCounter%nEvents; i<nEvents; i++) {
		tree->GetEntry(i);
		// choose only electrons
		if (type == 11) {
			eventsCounter++;
			if (eventsCounter%1000 == 0)
				G4cout <<"Event " << eventsCounter << G4endl;

			fParticleGun->SetParticlePosition(G4ThreeVector(x0 + 0.5*mm*(G4UniformRand()-0.5), y0 + 0.5*mm*(G4UniformRand()-0.5), z0));
			fParticleGun->SetParticleEnergy(kinEnergy + 20*keV*(G4UniformRand()-0.5));
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(Px, Py, Pz));
			fParticleGun->GeneratePrimaryVertex(event);
			break;
		}
	}
}

G4ParticleDefinition* PrimaryGeneratorAction::FindParticle(G4String particleName)
{
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
	return particle;
}

void PrimaryGeneratorAction::SetRootFile(G4String newValue)
{
	rootFileName = newValue;
	if (!f) f = new TFile(rootFileName);
}

void PrimaryGeneratorAction::SetTreeName(G4String newValue)
{
	rootTreeName = newValue;
	tree = (TTree*)f->Get(rootTreeName);
	tree->SetBranchAddress("Energy", &kinEnergy);
	tree->SetBranchAddress("PX", &Px);
	tree->SetBranchAddress("PY", &Py);
	tree->SetBranchAddress("PZ", &Pz);
	tree->SetBranchAddress("PosX", &x0);
	tree->SetBranchAddress("PosY", &y0);
	tree->SetBranchAddress("PosZ", &z0);
	tree->SetBranchAddress("PDGEncoding", &type);

	nEvents = tree->GetEntries();
}


