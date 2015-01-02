#ifndef DetectorConstructionP2_h
#define DetectorConstructionP2_h 1

#include "DetectorConstruction.hh"

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class DetectorMessenger;

class MagneticField;
class G4UserLimits;
class DetectorConstruction;
class G4SubtractionSolid;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstructionP2// : public G4VUserDetectorConstruction 
{
public:
  DetectorConstructionP2(DetectorConstruction*, MagneticField*);
 // DetectorConstructionP2();
  ~DetectorConstructionP2();
  
public:
  
  void SetMagField(G4double valB);	//set value of B - induction of mag field in Gauss
  void SetCupLenght(G4double valMy);	//set length of absorber
  void SetApertureInRadius(G4double valMy);//radius of the first calimator
  void SetApertureLenght(G4double valMy); //length of the first calimator

void SetMaxStep(G4double valMy);
void SetMaxLength(G4double valMy);
void SetMaxTime(G4double valMy);
void SetMinEkin(G4double valMy);
void SetMionRang(G4double valMy);

void SetDet1OutRad(G4double valMy);
void SetDet1InRad(G4double valMy);
void SetDet1X(G4double valMy);
void SetGeomID(G4int gID);//off/on full geomethy
 
void SetRotationDeg(G4double valMy);
void SetRotationAddDistCmd(G4double valMy);
  //void UpdateGeometry(); //define world volume 

 G4VPhysicalVolume* ConstructGeom4();
 G4VPhysicalVolume* ConstructGeom5();
 G4VPhysicalVolume* ConstructGeom6();

private:
	DetectorConstruction *MainDet;
	//G4Material *testMat;
  
  G4UserLimits* stepLimit;        // pointer to user step limits
  
  MagneticField *magField; 		//create mag fienlg
/*
// Define materials    
  G4Material*        alMy;
  G4Material*        steelMy;
  G4Material*        airMy;
  G4Material*        titanMy;
  G4Material*        vacuumMy;
  G4Material*        beamVacuum;
*/
	//world :)
  G4Box*             solidWorld;
  G4LogicalVolume*   logicWorld;
  G4VPhysicalVolume* physiWorld;

  G4Tubs*            solidBeamPipe;
  G4LogicalVolume*   logicBeamPipe;
  G4VPhysicalVolume* physiBeamPipe;
  
  G4Tubs*            solidBeamPipeV;
  G4LogicalVolume*   logicBeamPipeV;
  G4VPhysicalVolume* physiBeamPipeV;

  G4Tubs*            solidCup;
  G4LogicalVolume*   logicCup;
  G4VPhysicalVolume* physiCup;

  G4Tubs*            solidAperture;
  G4LogicalVolume*   logicAperture;
  G4VPhysicalVolume* physiAperture;

  G4Tubs*            solidSenDet1;
  G4LogicalVolume*   logicSenDet1;
  G4VPhysicalVolume* physiSenDet1;

  G4Tubs*            solidShield1;
  G4LogicalVolume*   logicShield1;
  G4VPhysicalVolume* physiShield1;

  G4Tubs*             solidShieldMain;
  G4Tubs*             solidDirka;
  G4SubtractionSolid* solidShieldRes;
  G4LogicalVolume*   logicShieldMain;
  G4VPhysicalVolume* physiShieldMain;
  
//  G4Box*             solidSenDet2;
//  G4LogicalVolume*   logicSenDet2;
//  G4VPhysicalVolume* physiSenDet2;

  G4Box*             solidDipol_L;//left
  G4LogicalVolume*   logicDipol_L;
  G4VPhysicalVolume* physiDipol_L;

  G4Box*             solidDipol_R;//right
  G4LogicalVolume*   logicDipol_R;
  G4VPhysicalVolume* physiDipol_R;

  G4Box*             solidDipol_T;//top
  G4LogicalVolume*   logicDipol_T;
  G4VPhysicalVolume* physiDipol_T;

  G4Box*             solidDipol_B;//botom
  G4LogicalVolume*   logicDipol_B;
  G4VPhysicalVolume* physiDipol_B;

  G4Box*             solidFieldBox_L;//left
  G4LogicalVolume*   logicFieldBox_L;
  G4VPhysicalVolume* physiFieldBox_L;

  G4Box*             solidFieldBox_R;//right
  G4LogicalVolume*   logicFieldBox_R;
  G4VPhysicalVolume* physiFieldBox_R;

  
  //DetectorMessenger* detectorMessenger;  //pointer to the Messenger

  
private:

  //void ComputeCalorParameters();
	//void DefineMaterials();
  
  G4int _geomID;			//number of geometry
  G4double _MagFieldVal;		//amount of B
  G4double _cupLenght;			//length of the absorber
  G4double _apertureInRadius;		//radius of first calimator
  G4double _apertureLenght;		//length of first calimator

  	G4double       _maxStep;
	G4double       _maxLength;
	G4double       _maxTime;
	G4double       _minEkin;
	G4double       _mionRang;

	G4double _Det1InRad;
	G4double _Det1OutRad;
	G4double _Det1X;
	G4double _RotationDeg;
	G4double _RotAddDist;

  // old geometry
  //_geomID = 999;
  //LB 05.05.2012 geometry with two sencative vlumes
  //_geomID = 3;
 

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorConstructionP2::SetGeomID(G4int gID){
  _geomID = gID;
}

//                                         gauss
inline void DetectorConstructionP2::SetMagField(G4double valB){
  _MagFieldVal = valB;
}

inline void DetectorConstructionP2::SetCupLenght(G4double valMy){
  _cupLenght = valMy;
}

inline void DetectorConstructionP2::SetApertureInRadius(G4double valMy){
  _apertureInRadius = valMy;
}

inline void DetectorConstructionP2::SetApertureLenght(G4double valMy){
  _apertureLenght = valMy;
}

inline void DetectorConstructionP2::SetMaxStep(G4double valMy){
  _maxStep = valMy;
}

inline void DetectorConstructionP2::SetMaxLength(G4double valMy){
  _maxLength = valMy;
}

inline void DetectorConstructionP2::SetMaxTime(G4double valMy){
  _maxTime = valMy;
}

inline void DetectorConstructionP2::SetMinEkin(G4double valMy){
  _minEkin = valMy;
  G4cout<<"_minEkin = "<<_minEkin<<std::endl;
}

inline void DetectorConstructionP2::SetMionRang(G4double valMy){
  _mionRang = valMy;
}

inline void DetectorConstructionP2::SetDet1InRad(G4double valMy){
  _Det1InRad = valMy;
}

inline void DetectorConstructionP2::SetDet1OutRad(G4double valMy){
  _Det1OutRad = valMy;
}

inline void DetectorConstructionP2::SetDet1X(G4double valMy){
  _Det1X = valMy;
}

inline void DetectorConstructionP2::SetRotationDeg(G4double valMy){
  _RotationDeg = valMy;
}
inline void DetectorConstructionP2::SetRotationAddDistCmd(G4double valMy){
	_RotAddDist = valMy;
}
#endif

