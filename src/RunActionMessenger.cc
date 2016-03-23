#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ios.hh"
#include "globals.hh"

#include "RunActionMessenger.hh"
#include "RunAction.hh"

RunActionMessenger::RunActionMessenger(RunAction* run)
:Run(run)
{

  RootFileCmd = new G4UIcmdWithAString("/Leetech/OutRootFile",this);
  RootFileCmd->SetGuidance("Output root file");
  RootFileCmd->SetParameterName("RootFile", false);
  RootFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

RunActionMessenger::~RunActionMessenger(){
  delete RootFileCmd;
}

void RunActionMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  if(command == RootFileCmd)
    Run->SetOutRootFile(newValue);
}


