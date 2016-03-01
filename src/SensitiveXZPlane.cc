/*
 * SensitivePlane.cc
 *
 *  Created on: Feb 25, 2016
 *      Author: vlad
 */

#include "G4Step.hh"
#include "G4Polyline.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

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

void SensitiveXZPlane::Visualize(G4double worldYhalfSize)
{
	G4Polyline planeCountour;

	// Set red line colour
	G4Colour         red(1.0, 0.0, 0.0);
	G4VisAttributes  att(red);
	planeCountour.SetVisAttributes(&att);

	// Set vertex positions
	planeCountour.push_back( G4Point3D(xc+halfLength, worldYhalfSize, zc) );
	planeCountour.push_back( G4Point3D(xc-halfLength, worldYhalfSize, zc) );
	planeCountour.push_back( G4Point3D(xc+halfLength, -worldYhalfSize, zc) );
	planeCountour.push_back( G4Point3D(xc-halfLength, -worldYhalfSize, zc) );
}
