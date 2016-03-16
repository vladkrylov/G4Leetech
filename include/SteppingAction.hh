#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
class EventAction;

class SteppingAction: public G4UserSteppingAction
{
public:
	SteppingAction(EventAction*);
	virtual ~SteppingAction();

	void UserSteppingAction(const G4Step*);
private:
  EventAction* fEventAction;
};

#endif /* SteppingAction_h */
