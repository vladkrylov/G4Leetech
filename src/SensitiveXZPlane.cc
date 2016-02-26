/*
 * SensitivePlane.cc
 *
 *  Created on: Feb 25, 2016
 *      Author: vlad
 */

#include "SensitiveXZPlane.hh"
#include "G4Step.hh"

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

