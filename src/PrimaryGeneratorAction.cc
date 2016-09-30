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

//	fParticleGun->SetParticlePosition(beamPipeCenter);
//	fParticleGun->SetParticleMomentumDirection(targetCenter - beamPipeCenter);

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
	G4double x, y, z, P, m;
//	int N = 2;
	int N = 9626; // corresponding to 1 PHIL bunch of 32 pC charge
	m = fParticleGun->GetParticleDefinition()->GetPDGMass();

	/**
	 * Test for EDep calculations
	 * make a point unidirectional source in front of diamond
	 */
	G4ThreeVector diamondCenter = Detector->GetDetectorPhys()->GetObjectTranslation();
//	Px = 0;
//	Py = 0;
//	Pz = -3*MeV;
//
//	x = diamondCenter.getX();
//	y = diamondCenter.getY();
//	z = diamondCenter.getZ() + 5*mm;

	for(int i=0; i<N; i++) {
		Px = GeneratePX();
		Py = GeneratePY();
		Pz = GeneratePZ();

		x = GeneratePosX();
		y = GeneratePosY();
		z = GeneratePosZ();

		P = std::sqrt(Px*Px + Py*Py + Pz*Pz);
		fParticleGun->SetParticleEnergy(std::sqrt(P*P + m*m) - m);
		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(Px, Py, Pz));
		fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));

		fParticleGun->GeneratePrimaryVertex(event);
	}
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

G4double PrimaryGeneratorAction::GeneratePX()
{
	return rndEngine->Gaus(-2.79e-02, 2.52e-02);
}

G4double PrimaryGeneratorAction::GeneratePY()
{
	return rndEngine->Gaus(0., 2.91904e-02);
}

G4double PrimaryGeneratorAction::GeneratePZ()
{
	return rndEngine->Uniform(-2.88, -2.67);
}

G4double PrimaryGeneratorAction::GeneratePosX()
{
	return rndEngine->Uniform(188., 218.);
}

G4double PrimaryGeneratorAction::GeneratePosY()
{
	return rndEngine->Gaus(0., 9.56);
}

G4double PrimaryGeneratorAction::GeneratePosZ()
{
	return 7.021;
}
