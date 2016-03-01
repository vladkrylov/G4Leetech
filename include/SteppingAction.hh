#ifndef SteppingAction_h
#define SteppingAction_h 1

#include <vector>
#include "G4UserSteppingAction.hh"

class SensitiveXZPlane;

class SteppingAction: public G4UserSteppingAction
{
public:
	SteppingAction();
	virtual ~SteppingAction();

	void UserSteppingAction(const G4Step*);

private:
	std::vector<SensitiveXZPlane*>* detectors;
};

#endif /* SteppingAction_h */
