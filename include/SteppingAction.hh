#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class DetectorConstruction;
class EventAction;
class RunAction;

class SteppingAction : public G4UserSteppingAction
{
public:
	SteppingAction();
	virtual ~SteppingAction();

	void UserSteppingAction(const G4Step*);

	RunAction* GetRunAction();
    
private:
	DetectorConstruction* detector;
	EventAction*          eventAction;
	RunAction*            runAction;
};

inline RunAction* SteppingAction::GetRunAction() {return runAction;}

#endif
