#include "RunActionMessenger.hh"

#include "RunAction.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ios.hh"
#include "globals.hh"


RunActionMessenger::RunActionMessenger(RunAction* run)
:Run(run)
{
 
  RootFileCmd = new G4UIcmdWithAString("/Micromegas/RootFile",this);
  RootFileCmd->SetGuidance("Output root file");
  RootFileCmd->SetParameterName("RootFile", false);
  RootFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

RunActionMessenger::~RunActionMessenger(){
  delete RootFileCmd;  
}

void RunActionMessenger::SetNewValue(G4UIcommand * command,G4String newValue){ 
  if(command == RootFileCmd)
    Run->SetRootFile(newValue);
}





