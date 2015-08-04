#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWith3Vector;
class G4UIcmdWithADoubleAndUnit;

class PrimaryGeneratorMessenger: public G4UImessenger
{
public:
	PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
	virtual ~PrimaryGeneratorMessenger();

	void SetNewValue(G4UIcommand*, G4String);
    
private:
	PrimaryGeneratorAction* Action;
	G4UIdirectory*          gunDir;
	G4UIcmdWithAString*     ParticleCmd;
	G4UIcmdWith3Vector*     ParticlePositionCmd;
	G4UIcmdWith3Vector*     ParticleDirectionCmd;
	G4UIcmdWith3Vector*     ParticleDirectionRMSCmd;
	G4UIcmdWithADoubleAndUnit*     ParticleEnergyCmd;

	//SetParticleName(G4String valS);
	//SetParticlePosition(G4double xval, G4double yval, G4double zval);
	//SetParticleDir(G4double xval, G4double yval, G4double zval);
	//SetParticleDirRMS(G4double xvalRMS, G4double yvalRMS, G4double zvalRMS);
	//SetParticleMom(G4double valMin, G4double valMax);
	//G4ThreeVector GetNew3VectorValue
};

#endif

