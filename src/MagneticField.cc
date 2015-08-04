#include "G4SystemOfUnits.hh"

#include "MagneticField.hh"
#include "TMath.h"

MagneticField::MagneticField()
{
    G4cout << "MagneticField::MagneticField()" << G4endl;
    _BfieldY = 300.0*gauss;
    
	_xc1 = 0;
	_yc1 = 0;
	_zc1 = 0;

	_dx1 = 0;
	_dy1 = 0;
	_dz1 = 0;

	_xc2 = 0;
	_yc2 = 0;
	_zc2 = 0;

	_dx2 = 0;
	_dy2 = 0;
	_dz2 = 0;
}

MagneticField::~MagneticField()
{}

void MagneticField::GetFieldValue(const double Point[4],double *Bfield) const
{
    SetField(Point, Bfield[0],Bfield[1],Bfield[2]); //set mag field for some point
}

bool MagneticField::SetField( const double *Point,  G4double & Bx, G4double & By, G4double & Bz) const
{
	G4double xx1 = Point[0] - _xc1;
	G4double yy1 = Point[1] - _yc1;
	G4double zz1 = Point[2] - _zc1;

	G4double xx2 = Point[0] - _xc2;
	G4double yy2 = Point[1] - _yc2;
	G4double zz2 = Point[2] - _zc2;
  
	if((xx1>=-_dx1/2.0 && xx1<=_dx1/2.0) || (xx2>=-_dx2/2.0 && xx2<=_dx2/2.0)) {
		if((yy1>=-_dy1/2.0 && yy1<=_dy1/2.0) || (yy2>=-_dy2/2.0 && yy2<=_dy2/2.0)) {
			if((zz1>=-_dz1/2.0 && zz1<=_dz1/2.0) || (zz2>=-_dz2/2.0 && zz2<=_dz2/2.0)) {
				Bx = 0.0;
				By = _BfieldY;
				Bz = 0.0;
				//G4cout<<"By = "<<By<<G4endl
				//<<"1400*gauss "<<1400*gauss<<G4endl;
				return true;
			}
		}
	}
	Bx = 0.0;
	By = 0.0;
	Bz = 0.0;
	return false;
}

// gauss
void MagneticField::setBfieldY(G4double byVal){
	G4cout  << " Set B-field Y-component" << G4endl
			<<" _BfieldY "<<byVal<<" gauss"<<G4endl;
	_BfieldY = byVal;
}

void MagneticField::setFieldBox1( G4double valxc, G4double valyc, G4double valzc,
				  G4double valdx, G4double valdy, G4double valdz)
{
	_xc1 = valxc;
	_yc1 = valyc;
	_zc1 = valzc;
	_dx1 = valdx;
	_dy1 = valdy;
	_dz1 = valdz;

	G4cout  <<" Set Field Box 1 "<<G4endl
			<<"_xc1 = "<<_xc1<<G4endl
			<<"_yc1 = "<<_yc1<<G4endl
			<<"_zc1 = "<<_zc1<<G4endl
			<<"_dx1 = "<<_dx1<<G4endl
			<<"_dy1 = "<<_dy1<<G4endl
			<<"_dz1 = "<<_dz1<<G4endl;
}

void MagneticField::setFieldBox2( G4double valxc, G4double valyc, G4double valzc,
				  G4double valdx, G4double valdy, G4double valdz)
{
	_xc2 = valxc;
	_yc2 = valyc;
	_zc2 = valzc;
	_dx2 = valdx;
	_dy2 = valdy;
	_dz2 = valdz;
  
	G4cout  <<" Set Field Box 2 "<<G4endl
			<<"_xc2 = "<<_xc2<<G4endl
			<<"_yc2 = "<<_yc2<<G4endl
			<<"_zc2 = "<<_zc2<<G4endl
			<<"_dx2 = "<<_dx2<<G4endl
			<<"_dy2 = "<<_dy2<<G4endl
			<<"_dz2 = "<<_dz2<<G4endl;
}
