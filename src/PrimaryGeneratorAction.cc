#include <cmath>

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"

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
	pgun = new G4GeneralParticleSource();
		  fgInstance = this;
		  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		   G4ParticleDefinition* particle = particleTable->FindParticle("e-");
		   pgun->SetParticleDefinition(particle);

	Detector = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();

	G4ThreeVector beamPipeCenter = Detector->GetBeamPipeCenter();
	G4ThreeVector targetCenter = Detector->GettTargetFaceCenter();
	G4ThreeVector targetDirection = targetCenter - beamPipeCenter;

	pgun->SetParticlePosition(G4ThreeVector(0.,0.,1.));
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete pgun;
		fgInstance = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
	//fParticleGun->SetParticleDefinition(FindParticle(_particleName));
	//fParticleGun->SetParticleMomentumDirection(GenerateParticleDir());
	//fParticleGun->SetParticleEnergy(GenerateEkin());
	pgun->GeneratePrimaryVertex(event);
}

