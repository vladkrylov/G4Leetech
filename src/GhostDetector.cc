#include "G4LogicalVolume.hh"
#include "GhostDetector.hh"
#include "G4PVPlacement.hh"

GhostDetector::GhostDetector(G4String name,
							 G4Material* material,
							 G4double halfX,
							 G4double halfY,
							 G4double halfZ,
							 G4ThreeVector position,
							 G4LogicalVolume* mother)
{
	G4Box* solid = new G4Box(name, halfX, halfY, halfZ);
	G4LogicalVolume* logic = new G4LogicalVolume(solid, material, name);
	G4VPhysicalVolume* phys = new G4PVPlacement(0,	//rotation
											    position,
												logic,	//its logical volume
											    name,		//its name
												mother,	     	//its mother  volume
											    false,      		//no boolean operation
												0);			//copy number
}

GhostDetector::~GhostDetector() {
	// TODO Auto-generated destructor stub
}

