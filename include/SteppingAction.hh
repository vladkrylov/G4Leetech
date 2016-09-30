#ifndef SteppingAction_h
#define SteppingAction_h 1

#include <vector>
#include "G4UserSteppingAction.hh"
#include "g4root.hh"

class EventAction;
class GhostDetector;
class G4VPhysicalVolume;

class SteppingAction: public G4UserSteppingAction
{
public:
	SteppingAction(EventAction*);
	virtual ~SteppingAction();

	void UserSteppingAction(const G4Step*);
	void ResetEDep();
	void FillDiamondHitsTuple();
	void FillDiamondEventsTuple();
private:
  EventAction* fEventAction;
	std::vector<GhostDetector*>* detectors;

	G4AnalysisManager* analysisManager;
	G4int BranchId;
	G4double Px, Py, Pz, P;
	G4double theta;

	// diamond relative variables
	G4VPhysicalVolume* diamond;

	G4int det_NPartPerEvent;
	G4double det_PDG;
	G4double det_eDep;
	G4double det_eBefore;
	G4double det_time;
	G4double det_posX;
	G4double det_posY;
	G4double det_posZ;
	G4double det_theta;
	G4double det_maxStepLength;
};

#endif /* SteppingAction_h */
