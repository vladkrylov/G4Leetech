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
class TString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();    
  virtual ~PrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*); //generate altctrons by default parameters

	//Good names of the functions :)
  void SetParticleName(G4String valS);
  void SetParticlePosition(G4double xval, G4double yval, G4double zval);
  void SetParticleDir(G4double xval, G4double yval, G4double zval);
  void SetParticleDirRMS(G4double xvalRMS, G4double yvalRMS, G4double zvalRMS);
  void SetParticleMom(G4double valMin, G4double valMax);
  
private:
  G4ParticleGun*           particleGun;	     //pointer a to G4  class
  DetectorConstruction*    Detector;         //pointer to the geometry
  
  PrimaryGeneratorMessenger* gunMessenger;   //messenger of this class
  TRandom3 *rndMy;
  G4double GenerateEkin();
  void GenerateParticleDir();
  G4double GetEkinFromMom(G4double pp, G4double mm);
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
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


