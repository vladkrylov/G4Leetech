#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"


class G4GeneralParticleSource;
class G4GenericMessenger;
class G4Event;
class G4ParticleDefinition;
class DetectorConstruction;

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
    
	static const PrimaryGeneratorAction* Instance();
	       const G4GeneralParticleSource* GetParticleGun()
	           const { return pgun; }
private:
	       static PrimaryGeneratorAction* fgInstance;
	                  G4GeneralParticleSource* pgun;
    DetectorConstruction* Detector;

	G4String _particleName;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
