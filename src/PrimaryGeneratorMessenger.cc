#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* Gun)
: primGenAction(Gun)
{
	gunDir = new G4UIdirectory("/Leetech/gun/");
	gunDir->SetGuidance("PrimaryGenerator control");

	ParticleCmd = new G4UIcmdWithAString("/Leetech/gun/PerticleType",this);
	ParticleCmd->SetGuidance("Choose particle type: gamma, e-, e+, mu-, mu+, pi-, pi+, kaon-, kaon+, anti_proton, proton");
	ParticleCmd->SetParameterName("PrticleType",false);
	ParticleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	ParticlePositionCmd = new G4UIcmdWith3Vector("/Leetech/gun/ParticlePosition", this);
	ParticlePositionCmd -> SetGuidance("Choose initial particle position.");
	ParticlePositionCmd -> SetParameterName("ParticlePositionX",
					  "ParticlePositionY",
					  "ParticlePositionZ", false);
	ParticlePositionCmd -> AvailableForStates(G4State_Idle);

	ParticleDirectionCmd = new G4UIcmdWith3Vector("/Leetech/gun/ParticleDirection", this);
	ParticleDirectionCmd -> SetGuidance("Choose initial particle direction.");
	ParticleDirectionCmd -> SetParameterName("ParticleDirX",
					   "ParticleDirY",
					   "ParticleDirZ", false);
	ParticleDirectionCmd -> AvailableForStates(G4State_Idle);

	ParticleDirectionRMSCmd = new G4UIcmdWithADoubleAndUnit("/Leetech/gun/ParticleDirectionRMS", this);
	ParticleDirectionRMSCmd -> SetGuidance("Choose initial particle smearing of the direction.");
	ParticleDirectionRMSCmd -> SetParameterName("DirectionRMS", false);
	ParticleDirectionRMSCmd -> AvailableForStates(G4State_Idle);

	ParticleEnergyCmd = new G4UIcmdWithADoubleAndUnit("/Leetech/gun/ParticleEnergy", this);
	ParticleEnergyCmd -> SetGuidance("Choose initial particle kinetic energy.");
	ParticleEnergyCmd -> SetParameterName("Energy", false);
	ParticleEnergyCmd -> SetRange("Energy>=0.");
	ParticleEnergyCmd -> AvailableForStates(G4State_Idle);

	ROOTFileCmd = new G4UIcmdWithAString("/Leetech/gun/SourceROOTFile", this);
	ROOTFileCmd -> SetGuidance("ROOT file for PrimaryGeneratorAction.");
	ROOTFileCmd -> SetParameterName("filename", false);
	ROOTFileCmd -> AvailableForStates(G4State_Idle);

	TreeNameCmd = new G4UIcmdWithAString("/Leetech/gun/tree", this);
	TreeNameCmd -> SetGuidance("Name of tree in the ROOT file for PrimaryGeneratorAction.");
	TreeNameCmd -> SetParameterName("Tree name", false);
	TreeNameCmd -> AvailableForStates(G4State_Idle);
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

//	if( command == ParticleCmd ){
//		primGenAction->SetParticleName(newValue);
//	}
//	if( command == ParticlePositionCmd ){
//		primGenAction->SetParticlePosition(ParticlePositionCmd->GetNew3VectorValue(newValue).x(),
//					ParticlePositionCmd->GetNew3VectorValue(newValue).y(),
//					ParticlePositionCmd->GetNew3VectorValue(newValue).z());
//	}
//	if( command == ParticleDirectionCmd ){
//		primGenAction->SetParticleDir(ParticleDirectionCmd->GetNew3VectorValue(newValue).x(),
//			   ParticleDirectionCmd->GetNew3VectorValue(newValue).y(),
//			   ParticleDirectionCmd->GetNew3VectorValue(newValue).z());
//	}
	if( command == ParticleDirectionRMSCmd ){
		primGenAction->SetDirectionRMS(ParticleDirectionRMSCmd->GetNewDoubleValue(newValue));
	}
	if( command == ParticleEnergyCmd ){
		primGenAction->SetParticleEnergy(ParticleEnergyCmd->GetNewDoubleValue(newValue));
	}
}


