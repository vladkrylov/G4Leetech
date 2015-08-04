#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* Gun)
: Action(Gun)
{
	gunDir = new G4UIdirectory("/Micromegas/gun/");
	gunDir->SetGuidance("PrimaryGenerator control");

	ParticleCmd = new G4UIcmdWithAString("/Micromegas/gun/PerticleType",this);
	ParticleCmd->SetGuidance("Choose particle type: gamma, e-, e+, mu-, mu+, pi-, pi+, kaon-, kaon+, anti_proton, proton");
	ParticleCmd->SetParameterName("PrticleType",false);
	ParticleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

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

	ParticleEnergyCmd = new G4UIcmdWithADoubleAndUnit("/Micromegas/gun/ParticleEnergy", this);
	ParticleEnergyCmd -> SetGuidance("Choose initial particle kinetic energy.");
	ParticleEnergyCmd -> SetParameterName("Energy", false);
	ParticleEnergyCmd -> SetRange("Energy>=0.");
	ParticleEnergyCmd -> AvailableForStates(G4State_Idle);
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
	delete ParticleCmd;
	delete ParticlePositionCmd;
	delete ParticleDirectionCmd;
	delete ParticleDirectionRMSCmd;
	delete ParticleEnergyCmd;
	delete gunDir;
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

	if( command == ParticleCmd ){
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
	if( command == ParticleEnergyCmd ){
		Action->SetParticleEnergy(ParticleEnergyCmd->GetNewDoubleValue(newValue));
	}
}


