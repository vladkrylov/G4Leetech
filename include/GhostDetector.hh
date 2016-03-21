#ifndef SRC_GHOSTDETECTOR_HH_
#define SRC_GHOSTDETECTOR_HH_

#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "globals.hh"

class G4Step;
class G4String;
class G4VPhysicalVolume;

class GhostDetector {
public:
	GhostDetector(G4String my_name, double my_xc, double my_zc, double my_length, G4LogicalVolume* motherLogic);
	virtual ~GhostDetector();

	bool Crossed(const G4Step* step);
	const G4String& GetName();

private:
	double xc;
	double zc;
	double halfLength;
	G4VPhysicalVolume *phys;
};

#endif /* SRC_GHOSTDETECTOR_HH_ */
