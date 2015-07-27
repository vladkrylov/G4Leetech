#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"

ActionInitialization::ActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::Build() const
{
	SetUserAction(new PrimaryGeneratorAction);
	SetUserAction(new RunAction);
	SetUserAction(new EventAction);
	SetUserAction(new SteppingAction);
	SetUserAction(new TrackingAction);
}
