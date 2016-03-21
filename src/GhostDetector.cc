#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "GhostDetector.hh"

GhostDetector::GhostDetector(G4String my_name,
		double my_xc,
		double my_zc,
		double my_halfLength,
		G4LogicalVolume* motherLogic)
{
	G4NistManager* man = G4NistManager::Instance();
	G4Material* vacuum  = man->FindOrBuildMaterial("G4_Galactic");

	xc = my_xc;
	zc = my_zc;
	halfLength = my_halfLength;

	G4Box* plate = new G4Box(my_name, my_halfLength, my_halfLength, 1*um);
	G4LogicalVolume *logic = new G4LogicalVolume(plate, vacuum, my_name);
	phys = new G4PVPlacement(0, G4ThreeVector(my_xc, 0, my_zc), logic, my_name, motherLogic, false, 0);

	logic->SetVisAttributes( new G4VisAttributes(G4Colour(153./255,76./255,0.)));
}

GhostDetector::~GhostDetector()
{}

/**
 * @brief: Checks whether the step crosses this detector
 *
 */
bool GhostDetector::Crossed(const G4Step* step)
{
//	step->GetStepLength();
//
//	return ( (step->GetPreStepPoint()->GetPosition().getZ() - zc)*(step->GetPostStepPoint()->GetPosition().getZ() - zc) < 0 ) &&
//		( fabs(step->GetPreStepPoint()->GetPosition().getX() - xc) < halfLength );
	return step->GetPostStepPoint()->GetPhysicalVolume() == phys;
}

const G4String& GhostDetector::GetName() {
	return phys->GetName();
}

