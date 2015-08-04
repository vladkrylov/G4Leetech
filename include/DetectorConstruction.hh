//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B5DetectorConstruction.hh 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file B5DetectorConstruction.hh
/// \brief Definition of the B5DetectorConstruction class

#ifndef B5DetectorConstruction_h
#define B5DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"

#include <vector>

class B5MagneticField;

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger;
class MagneticField;
class DetectorMessenger;
class G4UniformMagField;

/// Detector construction

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    G4ThreeVector GetBeamPipeCenter();
    G4ThreeVector GetTargetFaceCenter();

private:
//    static G4ThreadLocal G4UniformMagField* fMagneticField;
//    static G4ThreadLocal G4FieldManager* fFieldMgr;

    DetectorMessenger* detectorMessenger;

	void DefineMaterials();
    G4Material* GetMaterial(G4int t);

    G4UniformMagField *fMagneticField;
  	G4FieldManager* fFieldMgr;
    G4VPhysicalVolume* physiBeamPipeV;
    G4VPhysicalVolume* phyTarget;
    G4LogicalVolume *logicInnerBox;

	// Define materials
	G4Material*        alMy;
	G4Material*        steelMy;
	G4Material*        airMy;
	G4Material*        titanMy;
	G4Material*        vacuumMy;
	G4Material*        beamVacuum;
	G4Material*        G4Lead;
	G4Material* 	   U;

    // members that are accessible/changeable from other classes
	G4double _collEntranceGapX;
	G4double _collEntranceGapY;
	G4double _collExit1GapX;
  	G4double _collExit1GapY;

  	G4double _MagFieldVal;
    G4double _targetThickness;			//length of the absorber
    G4double _apertureInRadius;		//radius of first calimator
    G4double _apertureLenght;		//length of first calimator

	G4double _maxStep;
	G4double _maxLength;
	G4double _maxTime;
  	G4double _minEkin;
  	G4double _mionRang;


  	G4double _Det1InRad;
  	G4double _Det1OutRad;
  	G4double _Det1X;
  	G4double _RotationDeg;
  	G4double _RotAddDist;

// messenger access functions
public:
    void SetMagField(G4double valMy);	//set value of B - induction of magnetic field in Gauss
    void SetCupLenght(G4double valMy);	//set length of absorber
    void SetApertureInRadius(G4double valMy);//radius of the first collimator
    void SetApertureLenght(G4double valMy); //length of the first collimator
    G4ThreeVector getBeamPipeCenter();

	void SetMaxStep(G4double valMy);
	void SetMaxLength(G4double valMy);
	void SetMaxTime(G4double valMy);
	void SetMinEkin(G4double valMy);
	void SetMionRang(G4double valMy);

	void SetDet1OutRad(G4double valMy);
	void SetDet1InRad(G4double valMy);
	void SetDet1X(G4double valMy);

	void SetRotationDeg(G4double valMy);
	void SetRotationAddDistCmd(G4double valMy);

	void SetCollimatorGapEntranceX(G4double valMy);
	void SetCollimatorGapEntranceY(G4double valMy);
	void SetCollimatorGapExit1X(G4double valMy);
	void SetCollimatorGapExit1Y(G4double valMy);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorConstruction::SetMagField(G4double valMy){
  _MagFieldVal = valMy;
}

inline void DetectorConstruction::SetCupLenght(G4double valMy){
	_targetThickness = valMy;
}

inline void DetectorConstruction::SetApertureInRadius(G4double valMy){
  _apertureInRadius = valMy;
}

inline void DetectorConstruction::SetApertureLenght(G4double valMy){
  _apertureLenght = valMy;
}

inline void DetectorConstruction::SetMaxStep(G4double valMy){
  _maxStep = valMy;
}

inline void DetectorConstruction::SetMaxLength(G4double valMy){
  _maxLength = valMy;
}

inline void DetectorConstruction::SetMaxTime(G4double valMy){
  _maxTime = valMy;
}

inline void DetectorConstruction::SetMinEkin(G4double valMy){
  _minEkin = valMy;
  G4cout<<"_minEkin = "<<_minEkin<<std::endl;
}

inline void DetectorConstruction::SetMionRang(G4double valMy){
  _mionRang = valMy;
}

inline void DetectorConstruction::SetDet1InRad(G4double valMy){
  _Det1InRad = valMy;
}

inline void DetectorConstruction::SetDet1OutRad(G4double valMy){
  _Det1OutRad = valMy;
}

inline void DetectorConstruction::SetDet1X(G4double valMy){
  _Det1X = valMy;
}

inline void DetectorConstruction::SetRotationDeg(G4double valMy){
  _RotationDeg = valMy;
}

inline void DetectorConstruction::SetRotationAddDistCmd(G4double valMy){
	_RotAddDist = valMy;
}

inline void DetectorConstruction::SetCollimatorGapEntranceX(G4double valMy){
	_collEntranceGapX = valMy;
}

inline void DetectorConstruction::SetCollimatorGapEntranceY(G4double valMy){
	_collEntranceGapY = valMy;
}

inline void DetectorConstruction::SetCollimatorGapExit1X(G4double valMy){
	_collExit1GapX = valMy;
}

inline void DetectorConstruction::SetCollimatorGapExit1Y(G4double valMy){
	_collExit1GapY = valMy;
}


#endif
