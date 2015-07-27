#include "G4UIcmdWithAnInteger.hh"

#include "EventActionMessenger.hh"
#include "EventAction.hh"

EventActionMessenger::EventActionMessenger(EventAction* EvAct)
: eventAction(EvAct)
{
	PrintCmd = new G4UIcmdWithAnInteger("/Micromegas/printModulo",this);
	PrintCmd->SetGuidance("Print events modulo n");
	PrintCmd->SetParameterName("EventNb",false);
	PrintCmd->SetRange("EventNb>0");
}

EventActionMessenger::~EventActionMessenger()
{
	delete PrintCmd;
}

void EventActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
	if(command == PrintCmd)	{
		eventAction->SetPrintModulo(PrintCmd->GetNewIntValue(newValue));
	}
}
