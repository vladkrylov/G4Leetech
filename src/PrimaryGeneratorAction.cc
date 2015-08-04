#include <cmath>

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"


#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
, fParticleGun(0)
, _particleName("e-")
, _xInit(0.0)
, _yInit(0.0)
, _zInit(-50.0*cm)
, _energy (5.*MeV)
, _xDir (0.0)
, _yDir (0.0)
, _zDir (1.0)
, _xDir0(0.0)
, _yDir0(0.0)
, _zDir0(1.0)
, _xDirRMS(0.0)
, _yDirRMS(0.0)
, _zDirRMS(0.0)
{
	gunMessenger = new PrimaryGeneratorMessenger(this);

	fParticleGun = new G4ParticleGun();
	fParticleGun->SetNumberOfParticles(1);
	fParticleGun->SetParticleDefinition(FindParticle(_particleName));
	fParticleGun->SetParticleEnergy(_energy);

	Detector = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();

	G4ThreeVector beamPipeCenter = Detector->GetBeamPipeCenter();
	G4ThreeVector targetCenter = Detector->GetTargetFaceCenter();
	G4ThreeVector targetDirection = targetCenter - beamPipeCenter;
	_xDir0 = _xDir = targetDirection.x();
	_yDir0 = _yDir = targetDirection.y();
	_zDir0 = _zDir = targetDirection.z();

	fParticleGun->SetParticlePosition(Detector->GetBeamPipeCenter());
	fParticleGun->SetParticleMomentumDirection(targetDirection);
}

G4ParticleDefinition* PrimaryGeneratorAction::FindParticle(G4String particleName)
{
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
	return particle;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
    delete gunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
	fParticleGun->SetParticleDefinition(FindParticle(_particleName));
	fParticleGun->SetParticleMomentumDirection(GenerateParticleDir());
	fParticleGun->SetParticleEnergy(GenerateEkin());
    fParticleGun->GeneratePrimaryVertex(event);
}


G4ThreeVector PrimaryGeneratorAction::GenerateParticleDir()
{
	if(_xDir == 0. && _yDir == 0. && _zDir == 0.) return G4RandomDirection();

	if(_xDirRMS != 0.0) _xDir = G4RandGauss::shoot(_xDir0,_xDirRMS);
	else _xDir = _xDir0;

	if(_yDirRMS != 0.0) _yDir = G4RandGauss::shoot(_yDir0,_yDirRMS);
	else _yDir = _yDir0;

	if(_zDirRMS != 0.0) _zDir = G4RandGauss::shoot(_zDir0,_zDirRMS);
	else _zDir = _zDir0;

	return G4ThreeVector(_xDir, _yDir, _zDir);
}

G4double PrimaryGeneratorAction::GenerateEkin()
{
	return _energy;
}

void PrimaryGeneratorAction::SetParticleName(G4String valS)
{
	_particleName = valS;
	G4cout  <<" PrimaryGeneratorAction::SetParticleName"<<G4endl
			<<"_particleName = "<<_particleName<<G4endl;
}

void PrimaryGeneratorAction::SetParticlePosition(G4double xval, G4double yval, G4double zval){
	_xInit = xval;
	_yInit = yval;
	_zInit = zval;
	G4cout  <<" PrimaryGeneratorAction::SetParticlePosition"<<G4endl
			<<"_xInit = "<<_xInit<<G4endl
			<<"_yInit = "<<_yInit<<G4endl
			<<"_zInit = "<<_zInit<<G4endl;
}

void PrimaryGeneratorAction::SetParticleDir(G4double xval, G4double yval, G4double zval)
{
	_xDir0 = xval;
	_yDir0 = yval;
	_zDir0 = zval;
	G4cout  <<" PrimaryGeneratorAction::SetParticleDir"<<G4endl
			<<"_xDir0 = "<<_xDir0<<G4endl
			<<"_yDir0 = "<<_yDir0<<G4endl
			<<"_zDir0 = "<<_zDir0<<G4endl;
}

void PrimaryGeneratorAction::SetParticleDirRMS(G4double xvalRMS, G4double yvalRMS, G4double zvalRMS)
{
	_xDirRMS = xvalRMS;
	_yDirRMS = yvalRMS;
	_zDirRMS = zvalRMS;
	G4cout  <<" PrimaryGeneratorAction::SetParticleDirRMS"<<G4endl
			<<"_xDirRMS = "<<_xDirRMS<<G4endl
			<<"_yDirRMS = "<<_yDirRMS<<G4endl
			<<"_zDirRMS = "<<_zDirRMS<<G4endl;
}

void PrimaryGeneratorAction::SetParticleEnergy(G4double val)
{
	_energy = val;
	G4cout  <<" PrimaryGeneratorAction::SetParticleEnergy"<<G4endl
			<<"_energy = "<< _energy <<G4endl;
}
