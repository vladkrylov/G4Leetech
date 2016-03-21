#ifndef SteppingAction_h
#define SteppingAction_h 1

#include <vector>
#include "G4UserSteppingAction.hh"
#include "g4root.hh"

class EventAction;
class GhostDetector;

class SteppingAction: public G4UserSteppingAction
{
public:
	SteppingAction(EventAction*);
	virtual ~SteppingAction();

	void UserSteppingAction(const G4Step*);
private:
  EventAction* fEventAction;
	std::vector<GhostDetector*>* detectors;

	G4AnalysisManager* analysisManager;
	G4int BranchId;
	G4double Px, Py, Pz, P;
	G4double theta;
};

#endif /* SteppingAction_h */
