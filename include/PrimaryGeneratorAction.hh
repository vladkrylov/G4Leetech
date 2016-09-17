#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"


class G4GeneralParticleSource;
class G4ParticleGun;
class G4GenericMessenger;
class G4Event;
class G4ParticleDefinition;
class DetectorConstruction;
class PrimaryGeneratorMessenger;
class TFile;
class TTree;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();
    static const PrimaryGeneratorAction* Instance();
    
    virtual void GeneratePrimaries(G4Event*);
    
	// Messenger access functions
    void SetParticleEnergy(G4double newValue);

private:
	static PrimaryGeneratorAction* fgInstance;
	PrimaryGeneratorMessenger* gunMessenger;

	G4ParticleGun* fParticleGun;
    DetectorConstruction* Detector;
	G4String _particleName;

	G4ParticleDefinition* FindParticle(G4String particleName);

	// for root file source
	G4int nEvents, eventsCounter;
	G4double kinEnergy;
	G4double Px, Py, Pz;
	G4double x0, y0, z0;
	G4int type;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
