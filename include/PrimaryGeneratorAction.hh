#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class G4ParticleGun;
class G4GenericMessenger;
class G4Event;
class G4ParticleDefinition;
class DetectorConstruction;
class PrimaryGeneratorMessenger;

class TFile;
class TTree;

/// Primary generator
///
/// A single particle is generated.
/// User can select 
/// - the initial momentum and angle
/// - the momentum and angle spreads
/// - random selection of a particle type from proton, kaon+, pi+, muon+, e+ 


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();
    
    virtual void GeneratePrimaries(G4Event*);
    
	void SetParticleName(G4String valS);
	void SetParticlePosition(G4double xval, G4double yval, G4double zval);
	void SetParticleDir(G4double xval, G4double yval, G4double zval);
	void SetParticleDirRMS(G4double xvalRMS, G4double yvalRMS, G4double zvalRMS);
	void SetParticleEnergy(G4double val);

private:
    G4ParticleGun* fParticleGun;
    DetectorConstruction* Detector;
    PrimaryGeneratorMessenger* gunMessenger;

    G4ParticleDefinition* FindParticle(G4String particleName);

	G4String _particleName;
	G4double _xInit;
	G4double _yInit;
	G4double _zInit;
	G4double _energy;
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
	G4ThreeVector GenerateParticleDir();

	// for root file source
	G4int nEvents, eventsCounter;
	G4double kinEnergy;
	G4double Px, Py, Pz;
	G4double x0, y0, z0;
	G4double type;

	TFile* f;
	TTree* tree;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
