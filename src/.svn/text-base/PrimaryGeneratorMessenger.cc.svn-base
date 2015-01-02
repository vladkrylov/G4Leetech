#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
                                          PrimaryGeneratorAction* Gun)
:Action(Gun)
{
  gunDir = new G4UIdirectory("/Micromegas/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");
   
  PerticleCmd = new G4UIcmdWithAString("/Micromegas/gun/PerticleType",this);
  PerticleCmd->SetGuidance("Choose particle type: gamma, e-, e+, mu-, mu+, pi-, pi+, kaon-, kaon+, anti_proton, proton");
  PerticleCmd->SetParameterName("PrticleType",false);
  PerticleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  ParticlePositionCmd = new G4UIcmdWith3Vector("/Micromegas/gun/ParticlePosition", this);
  ParticlePositionCmd -> SetGuidance("Choose initial particle position.");
  ParticlePositionCmd -> SetParameterName("ParticlePositionX", 
					  "ParticlePositionY", 
					  "ParticlePositionZ", false);
  ParticlePositionCmd -> AvailableForStates(G4State_Idle);

  ParticleDirectionCmd = new G4UIcmdWith3Vector("/Micromegas/gun/ParticleDirection", this);
  ParticleDirectionCmd -> SetGuidance("Choose initial particle direction.");
  ParticleDirectionCmd -> SetParameterName("ParticleDirX", 
					   "ParticleDirY", 
					   "ParticleDirZ", false);
  ParticleDirectionCmd -> AvailableForStates(G4State_Idle);
  
  ParticleDirectionRMSCmd = new G4UIcmdWith3Vector("/Micromegas/gun/ParticleDirectionRMS", this);
  ParticleDirectionRMSCmd -> SetGuidance("Choose initial particle smearing of the direction.");
  ParticleDirectionRMSCmd -> SetParameterName("ParticleDirRMSX", 
					      "ParticleDirRMSY", 
					      "ParticleDirRMSZ", false);
  ParticleDirectionRMSCmd -> AvailableForStates(G4State_Idle);

  ParticleMomentumCmd = new G4UIcmdWith3Vector("/Micromegas/gun/ParticleMomentum", this);
  ParticleMomentumCmd -> SetGuidance("Choose initial particle momentum (between min and max specified value).");
  ParticleMomentumCmd -> SetParameterName("ParticleMomMin", 
					  "ParticleMomMax", 
					  "ParticleMomNAN", false);
  ParticleMomentumCmd -> AvailableForStates(G4State_Idle);
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete PerticleCmd;
  delete ParticlePositionCmd;
  delete ParticleDirectionCmd;
  delete ParticleDirectionRMSCmd;
  delete ParticleMomentumCmd;
  delete gunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue){ 

  if( command == PerticleCmd ){ 
    Action->SetParticleName(newValue);
  }
  if( command == ParticlePositionCmd ){
    Action->SetParticlePosition(ParticlePositionCmd->GetNew3VectorValue(newValue).x(),
    				ParticlePositionCmd->GetNew3VectorValue(newValue).y(),
    				ParticlePositionCmd->GetNew3VectorValue(newValue).z());
  }
  if( command == ParticleDirectionCmd ){
    Action->SetParticleDir(ParticleDirectionCmd->GetNew3VectorValue(newValue).x(),
			   ParticleDirectionCmd->GetNew3VectorValue(newValue).y(),
			   ParticleDirectionCmd->GetNew3VectorValue(newValue).z());
  }
  if( command == ParticleDirectionRMSCmd ){
    Action->SetParticleDirRMS(ParticleDirectionRMSCmd->GetNew3VectorValue(newValue).x(),
			      ParticleDirectionRMSCmd->GetNew3VectorValue(newValue).y(),
			      ParticleDirectionRMSCmd->GetNew3VectorValue(newValue).z());
  }
  if( command == ParticleMomentumCmd ){
    Action->SetParticleMom(ParticleMomentumCmd->GetNew3VectorValue(newValue).x(),
			   ParticleMomentumCmd->GetNew3VectorValue(newValue).y());
  }

}


