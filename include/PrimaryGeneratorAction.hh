#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "TString.h"

class G4ParticleGun;
class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;
class TRandom3;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	PrimaryGeneratorAction();
	virtual ~PrimaryGeneratorAction();

	void GeneratePrimaries(G4Event*);

	void SetParticleName(G4String valS);
	void SetParticlePosition(G4double xval, G4double yval, G4double zval);
	void SetParticleDir(G4double xval, G4double yval, G4double zval);
	void SetParticleDirRMS(G4double xvalRMS, G4double yvalRMS, G4double zvalRMS);
	void SetParticleMom(G4double valMin, G4double valMax);
  
private:
	G4ParticleGun*           particleGun;
	DetectorConstruction*    Detector;

	PrimaryGeneratorMessenger* gunMessenger;
	TRandom3 *rndMy;

	TString _particleName;
	G4double _xInit;
	G4double _yInit;
	G4double _zInit;
	G4double _pMin;
	G4double _pMax;
	G4double _xDir;
	G4double _yDir;
	G4double _zDir;
	G4double _xDir0;
	G4double _yDir0;
	G4double _zDir0;
	G4double _xDirRMS;
	G4double _yDirRMS;
	G4double _zDirRMS;

	G4double GenerateEkin();
	void GenerateParticleDir();
	G4double GetEkinFromMom(G4double pp, G4double mm);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


