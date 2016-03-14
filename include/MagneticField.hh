#ifndef MagneticField_H
#define MagneticField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"

class MagneticField : public G4MagneticField
{
public:
	MagneticField();
	virtual ~MagneticField();

	virtual void GetFieldValue( const  double Point[4], double *Bfield ) const;

	bool SetField( const double *Point, G4double & Bx, G4double & By, G4double & Bz) const;

	void setBfieldY(G4double byVal); // set B at Y axis

	void setFieldBox1( G4double valxc, G4double valyc, G4double valzc,		//setting size of volume woth field
			 G4double valdx, G4double valdy, G4double valdz);

private:
	G4double _BfieldY;

	G4double _xc1;
	G4double _yc1;
	G4double _zc1;

	G4double _dx1;
	G4double _dy1;
	G4double _dz1;
};


#endif

