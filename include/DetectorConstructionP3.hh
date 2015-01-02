#ifndef DetectorConstructionP3_h
#define DetectorConstructionP3_h 1

#include "DetectorConstruction.hh"

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Tubs;
class G4UnionSolid;
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

class DetectorConstructionP3// : public G4VUserDetectorConstruction 
{
public:
  DetectorConstructionP3(DetectorConstruction*, MagneticField*);
  ~DetectorConstructionP3();
  
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

 G4VPhysicalVolume* ConstructGeom7();
 G4VPhysicalVolume* ConstructGeom8();
 G4VPhysicalVolume* ConstructGeom9();
 G4VPhysicalVolume* ConstructGeom10();
 G4VPhysicalVolume* ConstructGeom11();
 G4VPhysicalVolume* ConstructGeom12();

private:
 void ConstructMagnet(double refX,double refY, double refZ, double gapSize);
 void ConstructMagnet(double refX,double refY, double refZ, double gapSize, double thicknessOfChamber);

 DetectorConstruction *MainDet;

 G4Box*             solidMagnetTopMain;
 G4LogicalVolume*   logicMagnetTopMain;
 G4VPhysicalVolume* physiMagnetTopMain;

 G4Box*             solidMagnetTopRight;
 G4LogicalVolume*   logicMagnetTopRight;
 G4VPhysicalVolume* physiMagnetTopRight;

 G4Box*             solidMagnetTopMid;
 G4LogicalVolume*   logicMagnetTopMid;
 G4VPhysicalVolume* physiMagnetTopMid;

 G4Box*             solidMagnetTopLeft;
 G4LogicalVolume*   logicMagnetTopLeft;
 G4VPhysicalVolume* physiMagnetTopLeft;

 G4Box*             solidMagnetBotMain;
 G4LogicalVolume*   logicMagnetBotMain;
 G4VPhysicalVolume* physiMagnetBotMain;

 G4Box*             solidMagnetBotRight;
 G4LogicalVolume*   logicMagnetBotRight;
 G4VPhysicalVolume* physiMagnetBotRight;

 G4Box*             solidMagnetBotMid;
 G4LogicalVolume*   logicMagnetBotMid;
 G4VPhysicalVolume* physiMagnetBotMid;

 G4Box*             solidMagnetBotLeft;
 G4LogicalVolume*   logicMagnetBotLeft;
 G4VPhysicalVolume* physiMagnetBotLeft;

	//G4Material *testMat;
  
  G4UserLimits* stepLimit;        // pointer to user step limits
  
  MagneticField *magField; 		//create mag fienlg

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
  
  G4Tubs*            solidSenDet1;
  G4LogicalVolume*   logicSenDet1;
  G4VPhysicalVolume* physiSenDet1;

  G4Tubs*            solidCup;
  G4LogicalVolume*   logicCup;
  G4VPhysicalVolume* physiCup;
  
  G4Box*             solidChamber1;
  G4UnionSolid*      solidChamber2;
  G4UnionSolid*      solidChamber;
  G4LogicalVolume*   logicChamber;
  G4VPhysicalVolume* physiChamber;
  G4Box*             solidAdditionalBox;
  
  G4Box*             solidInnerBox;
  G4Box*             solidAdditionalInnerBox;
  G4Box*             solidNeck;
  G4UnionSolid*      solidInnerBoxUNeck;
  G4UnionSolid*      solidInnerBoxUNeck2;
  G4UnionSolid*      solidInnerBoxUNeck2AddBox;
  G4UnionSolid*      solidInnerBoxUNeck2AddBox2;
  G4LogicalVolume*   logicInnerBox;
  G4VPhysicalVolume* physiInnerBox;
  

  G4Box*             solidFieldBox;
  G4LogicalVolume*   logicFieldBox;
  G4VPhysicalVolume* physiFieldBox;
  
  G4Box*             solidChamberVac;
  G4LogicalVolume*   logicChamberVac;
  G4VPhysicalVolume* physiChamberVac;

  G4Box* 	     solidShieldBarrier;
  G4LogicalVolume*   logicShieldBarrier;
  G4VPhysicalVolume* physiShieldBarrier;

  G4Box* 	     solidShieldBarrier1;
  G4LogicalVolume*   logicShieldBarrier1;
  G4VPhysicalVolume* physiShieldBarrier1;

  G4Box* 	     solidShieldBarrier2;
  G4LogicalVolume*   logicShieldBarrier2;
  G4VPhysicalVolume* physiShieldBarrier2;
  
  G4Box* 	     solidInnerTopShield;
  G4LogicalVolume*   logicInnerTopShield;
  G4VPhysicalVolume* physiInnerTopShield;

  G4Box* 	     solidInnerBotShield1;
  G4LogicalVolume*   logicInnerBotShield1;
  G4VPhysicalVolume* physiInnerBotShield1;

  G4Box* 	     solidInnerBotShield2;
  G4LogicalVolume*   logicInnerBotShield2;
  G4VPhysicalVolume* physiInnerBotShield2;
  
  G4Box* 	     solidInnerShield3;
  G4LogicalVolume*   logicInnerShield3;
  G4VPhysicalVolume* physiInnerShield3;

  G4Box* 	     solidInnerShield4;
  G4LogicalVolume*   logicInnerShield4;
  G4VPhysicalVolume* physiInnerShield4;

  G4Box* 	     solidInnerShield5;
  G4LogicalVolume*   logicInnerShield5;
  G4VPhysicalVolume* physiInnerShield5;

  G4Box*             solidColim11;
  G4LogicalVolume*   logicColim11;
  G4VPhysicalVolume* physiColim11;
  
  G4Box*             solidColim12;
  G4LogicalVolume*   logicColim12;
  G4VPhysicalVolume* physiColim12;
  
  G4Box*             solidColim21;
  G4LogicalVolume*   logicColim21;
  G4VPhysicalVolume* physiColim21;
  
  G4Box*             solidColim22;
  G4LogicalVolume*   logicColim22;
  G4VPhysicalVolume* physiColim22;
  
  G4Box*             solidColim31;
  G4LogicalVolume*   logicColim31;
  G4VPhysicalVolume* physiColim31;
  
  G4Box*             solidColim32;
  G4LogicalVolume*   logicColim32;
  G4VPhysicalVolume* physiColim32;
  
  G4Box*             solidColim41;
  G4LogicalVolume*   logicColim41;
  G4VPhysicalVolume* physiColim41;
  
  G4Box*             solidColim42;
  G4LogicalVolume*   logicColim42;
  G4VPhysicalVolume* physiColim42;

  G4Box*             solidColim51;
  G4LogicalVolume*   logicColim51;
  G4VPhysicalVolume* physiColim51;

  G4Box*             solidColim52;
  G4LogicalVolume*   logicColim52;
  G4VPhysicalVolume* physiColim52;

  G4Box*             solidColim61;
  G4LogicalVolume*   logicColim61;
  G4VPhysicalVolume* physiColim61;

  G4Box*             solidColim62;
  G4LogicalVolume*   logicColim62;
  G4VPhysicalVolume* physiColim62;

private:
  
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
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorConstructionP3::SetGeomID(G4int gID){
  _geomID = gID;
}

//                                         gauss
inline void DetectorConstructionP3::SetMagField(G4double valB){
  _MagFieldVal = valB;
}

inline void DetectorConstructionP3::SetCupLenght(G4double valMy){
  _cupLenght = valMy;
}

inline void DetectorConstructionP3::SetApertureInRadius(G4double valMy){
  _apertureInRadius = valMy;
}

inline void DetectorConstructionP3::SetApertureLenght(G4double valMy){
  _apertureLenght = valMy;
}

inline void DetectorConstructionP3::SetMaxStep(G4double valMy){
  _maxStep = valMy;
}

inline void DetectorConstructionP3::SetMaxLength(G4double valMy){
  _maxLength = valMy;
}

inline void DetectorConstructionP3::SetMaxTime(G4double valMy){
  _maxTime = valMy;
}

inline void DetectorConstructionP3::SetMinEkin(G4double valMy){
  _minEkin = valMy;
  G4cout<<"_minEkin = "<<_minEkin<<std::endl;
}

inline void DetectorConstructionP3::SetMionRang(G4double valMy){
  _mionRang = valMy;
}

inline void DetectorConstructionP3::SetDet1InRad(G4double valMy){
  _Det1InRad = valMy;
}

inline void DetectorConstructionP3::SetDet1OutRad(G4double valMy){
  _Det1OutRad = valMy;
}

inline void DetectorConstructionP3::SetDet1X(G4double valMy){
  _Det1X = valMy;
}

inline void DetectorConstructionP3::SetRotationDeg(G4double valMy){
  _RotationDeg = valMy;
}
inline void DetectorConstructionP3::SetRotationAddDistCmd(G4double valMy){
	_RotAddDist = valMy;
}
#endif

