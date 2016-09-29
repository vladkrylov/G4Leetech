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
#include "TRandom3.h"

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
	gunMessenger = new PrimaryGeneratorMessenger(this);

	Detector = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
	G4double LeetechRotationAngle = Detector->GetLeetechRotation();
	M = new G4RotationMatrix(G4ThreeVector(cos(LeetechRotationAngle), 0, -sin(LeetechRotationAngle)),
						     G4ThreeVector(0, 1, 0),
						     G4ThreeVector(sin(LeetechRotationAngle), 0, -cos(LeetechRotationAngle)));

	G4ThreeVector beamPipeCenter = Detector->GetBeamPipeCenter();
	G4ThreeVector targetCenter = Detector->GetTargetFaceCenter();

	eventsCounter = 0;

	fParticleGun = new G4ParticleGun();
	fParticleGun->SetNumberOfParticles(1);
	fParticleGun->SetParticleDefinition(FindParticle("e-"));

	fParticleGun->SetParticlePosition(beamPipeCenter);
	fParticleGun->SetParticleMomentumDirection(targetCenter - beamPipeCenter);

	//	randomization with seed
	//  from ROOT documentation:
	//  If seed is 0, the seed is automatically computed via a TUUID object.
	//  In this case the seed is guaranteed to be unique in space and time.
	rndEngine = new TRandom3(0);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete fParticleGun;
	delete gunMessenger;
	fgInstance = 0;
	delete M;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
	fParticleGun->SetParticleMomentumDirection(GenerateParticleDir());
	fParticleGun->SetParticleEnergy(kinEnergy);
	fParticleGun->GeneratePrimaryVertex(event);
}

G4ParticleDefinition* PrimaryGeneratorAction::FindParticle(G4String particleName)
{
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
	return particle;
}

void PrimaryGeneratorAction::SetParticleEnergy(G4double newValue)
{
	kinEnergy = newValue;
}

void PrimaryGeneratorAction::SetDirectionRMS(G4double newValue)
{
	dirRMS = newValue;
}

G4ThreeVector PrimaryGeneratorAction::GenerateParticleDir()
{
	double dispacement = 0.0;
	G4ThreeVector d(dispacement, dispacement, 1);
	//generation direction of the particle distributed by Gauss
	if (dirRMS != 0.0) {
		d += G4ThreeVector(rndEngine->Gaus(0, dirRMS),
						   rndEngine->Gaus(0, dirRMS),
						   0);
	}
	d *= *M;
	return d;
}

