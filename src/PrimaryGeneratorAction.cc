#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"

#include "TRandom3.h"
#include "TMath.h"
#include "TString.h"

#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	G4int n_particle = 1;
	particleGun  = new G4ParticleGun(n_particle);		//create particleGun witch will run 1 particle per once
	Detector = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();  //define detector???
	//create a messenger for this class
	gunMessenger = new PrimaryGeneratorMessenger(this);

	//initial parameters
	_particleName = "e-";
	_xInit = 0.0;
	_yInit = 0.0;
	_zInit = -50.0*cm;
	_pMin  = 0.5*MeV;
	_pMax  = 9.5*MeV;
	_xDir  = 0.0;
	_yDir  = 0.0;
	_zDir  = 1.0;
	_xDir0 = 0.0;
	_yDir0 = 0.0;
	_zDir0 = 1.0;
	_xDirRMS = 0.0;
	_yDirRMS = 0.0;
	_zDirRMS = 0.0;

	// default particle kinematic
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	particleTable->DumpTable("gamma");
	particleTable->DumpTable("e-");
	particleTable->DumpTable("e+");
	particleTable->DumpTable("mu-");
	particleTable->DumpTable("mu+");
	particleTable->DumpTable("pi-");
	particleTable->DumpTable("pi+");
	particleTable->DumpTable("kaon-");
	particleTable->DumpTable("kaon+");
	particleTable->DumpTable("anti_proton");
	particleTable->DumpTable("proton");

	//creating particles with parameterf
	G4ParticleDefinition* particle = particleTable->FindParticle("e-");
	particleGun->SetParticleDefinition(particle);
	particleGun->SetParticleMomentumDirection(G4ThreeVector(_xDir,_yDir,_zDir));
	particleGun->SetParticleEnergy(5.0*MeV);
	//  particleGun->SetParticlePosition(G4ThreeVector( _xInit, _yInit, _zInit));


	//	randomization with seed
	//  from ROOT documentation:
	//  If seed is 0, the seed is automatically computed via a TUUID object.
	//  In this case the seed is guaranteed to be unique in space and time.
	rndMy = new TRandom3(0);
	G4cout<<"PrimaryGeneratorAction::PrimaryGeneratorAction"<<G4endl;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction(){
  delete particleGun;
  delete gunMessenger;
  delete rndMy;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	//set electrons
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	//TString particleName = "gamma";
	//TString particleName = "e-";
	//TString particleName = "e+";
	//TString particleName = "mu-";
	//TString particleName = "mu+";
	//TString particleName = "pi-";
	//TString particleName = "pi+";
	//TString particleName = "kaon-";
	//TString particleName = "kaon+";
	//TString particleName = "anti_proton";
	//TString particleName = "proton";
	//G4ParticleDefinition* particle = particleTable->FindParticle(particleName.Data());
	G4ParticleDefinition* particle = particleTable->FindParticle(_particleName.Data());
	particleGun->SetParticleDefinition(particle);

	G4double beamSigma = 3*mm;
	//  particleGun->SetParticlePosition(G4ThreeVector( _xInit, _yInit, _zInit));
	particleGun->SetParticlePosition(Detector->getBeamPipeCenter());
	//								   + G4ThreeVector(rndMy->Gaus(0, beamSigma), 0, 0));
	GenerateParticleDir();
	if(_xDir==0 && _yDir==0 && _zDir==0)
		particleGun->SetParticleMomentumDirection(G4RandomDirection());
	else
		particleGun->SetParticleMomentumDirection(G4ThreeVector( _xDir, _yDir, _zDir));

	particleGun->SetParticleEnergy(GenerateEkin());
	particleGun->GeneratePrimaryVertex(anEvent);
}

void PrimaryGeneratorAction::GenerateParticleDir()
{
	if(_xDirRMS != 0.0)
		_xDir = rndMy->Gaus(_xDir0,_xDirRMS);
	else
		_xDir = _xDir0;

	if(_yDirRMS != 0.0)
		_yDir = rndMy->Gaus(_yDir0,_yDirRMS);
	else
		_yDir = _yDir0;

	if(_zDirRMS != 0.0)
		_zDir = rndMy->Gaus(_zDir0,_zDirRMS);
	else
		_zDir = _zDir0;
}

G4double PrimaryGeneratorAction::GenerateEkin()
{
	//generation Energy of the particle distributed by uniform
	G4double Ekin;
	G4double pp;
	//G4double pMin = 0.5*MeV;
	//G4double pMax = 9.0*MeV;
	//G4double pMin = 4.99999*MeV;
	//G4double pMax = 5.00000*MeV;
	if (_pMin < _pMax) {
		pp = rndMy->Uniform(_pMin,_pMax);
	} else if (_pMin == _pMax) {
		pp = _pMin;
	} else {
		G4cout  <<" ---> ERROR: _pMin > _pMax "<<G4endl
				<<"             _pMin = "<<_pMin<<G4endl
				<<"             _pMax = "<<_pMax<<G4endl;
	}
	Ekin = GetEkinFromMom(pp, particleGun->GetParticleDefinition()->GetPDGMass());
	return Ekin;
}

G4double PrimaryGeneratorAction::GetEkinFromMom(G4double pp, G4double mm)
{
	//Good name of the function :)
	return TMath::Sqrt(pp*pp + mm*mm) - mm;
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

void PrimaryGeneratorAction::SetParticleMom(G4double valMin, G4double valMax)
{
	_pMin  = valMin;
	_pMax  = valMax;
	G4cout  <<" PrimaryGeneratorAction::SetParticleMom"<<G4endl
			<<"_pMin = "<<_pMin<<G4endl
			<<"_pMax = "<<_pMax<<G4endl;
}
