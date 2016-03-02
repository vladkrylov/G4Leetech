#ifndef SRC_GHOSTDETECTOR_HH_
#define SRC_GHOSTDETECTOR_HH_

#include "globals.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"

class GhostDetector {
public:
	GhostDetector(G4String name,
				 G4Material* material,
				 G4double halfX,
				 G4double halfY,
				 G4double halfZ,
				 G4ThreeVector position,
				 G4LogicalVolume* mother);
	virtual ~GhostDetector();

private:



};

#endif /* SRC_GHOSTDETECTOR_HH_ */
