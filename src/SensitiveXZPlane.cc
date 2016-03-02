/*
 * SensitivePlane.cc
 *
 *  Created on: Feb 25, 2016
 *      Author: vlad
 */

#include "globals.hh"
#include "G4Step.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "SensitiveXZPlane.hh"

SensitiveXZPlane::SensitiveXZPlane(G4String my_name, double my_xc, double my_zc, double my_halfLength)
{
	name = my_name;
	xc = my_xc;
	zc = my_zc;
	halfLength = my_halfLength;
}

SensitiveXZPlane::~SensitiveXZPlane()
{}

/**
 * @brief: Checks whether the step crosses this plane
 *
 * this implementation is valid only for phi == 0
 * i.e. plane is parallel to X axis and perpendicular to Z axis
 *
 * Also it is assumed that the step is small compared to
 * plane size (length) so it simplifies check of crossing
 */
bool SensitiveXZPlane::Crossed(const G4Step* step)
{
	step->GetStepLength();

	return ( (step->GetPreStepPoint()->GetPosition().getZ() - zc)*(step->GetPostStepPoint()->GetPosition().getZ() - zc) < 0 ) &&
		( fabs(step->GetPreStepPoint()->GetPosition().getX() - xc) < halfLength );

}

/**
 * WARNING: call this method just to visualize the geometry!
 * Simulation results are not correct when plane is visualized.
 */
void SensitiveXZPlane::Visualize(G4LogicalVolume* world)
{
	G4NistManager* man = G4NistManager::Instance();
	G4Material* vacuum  = man->FindOrBuildMaterial("G4_Galactic");

	G4Box* solid = new G4Box(name, halfLength, halfLength, 1e-8);
	G4LogicalVolume* logic = new G4LogicalVolume(solid, vacuum, name);
	G4VPhysicalVolume* phys = new G4PVPlacement(0,	//rotation
											    G4ThreeVector(xc, 0., zc),
												logic,	//its logical volume
											    name,		//its name
												world,	     	//its mother  volume
											    false,      		//no boolean operation
												0);			//copy number

	logic->SetVisAttributes( new G4VisAttributes(G4Colour(153./255,76./255,0.)));
}
