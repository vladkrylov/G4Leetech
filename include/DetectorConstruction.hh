#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"


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
//class DetectorConstructionP2;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction //define constructor legacy from User
{
public:
	DetectorConstruction();
	~DetectorConstruction();

	G4VPhysicalVolume* Construct(); //construct geometry (if changet)
	virtual void ConstructSDandField();

	void SetMagField(G4double valB);	//set value of B - induction of mag field in Gauss
	void SetCupLenght(G4double valMy);	//set length of absorber
	void SetApertureInRadius(G4double valMy);//radius of the first calimator
	void SetApertureLenght(G4double valMy); //length of the first calimator
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


	void OffMagField();
	void UpdateGeometry(); //define world volume
	void SetGeomID(G4int gID);//off/on full geomethy

	G4Material *GetMaterial(int t);
    
private:
	// Define materials
	G4Material*        alMy;
	G4Material*        steelMy;
	G4Material*        airMy;
	G4Material*        titanMy;
	G4Material*        vacuumMy;
	G4Material*        beamVacuum;
	G4Material*        G4Lead;
	G4Material* 	   U;
	//G4Material        *test;

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

	G4Box*             solidSenDet2;
	G4LogicalVolume*   logicSenDet2;
	G4VPhysicalVolume* physiSenDet2;

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


	DetectorMessenger* detectorMessenger;  //pointer to the Messenger

	void DefineMaterials();		//define materials)
	void ConstructMagnet(double refX,double refY, double refZ, double gapSize, double thicknessOfChamber);
	//void ComputeCalorParameters();

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

	G4double collEntranceGapX;
	G4double collEntranceGapY;
	G4double collExit1GapX;
  	G4double collExit1GapY;

	G4VPhysicalVolume* ConstructGeom12();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorConstruction::SetGeomID(G4int gID){
  _geomID = gID;
}

//                                         gauss
inline void DetectorConstruction::SetMagField(G4double valB){
  _MagFieldVal = valB;
}

inline void DetectorConstruction::SetCupLenght(G4double valMy){
  _cupLenght = valMy;
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
	collEntranceGapX = valMy;
}

inline void DetectorConstruction::SetCollimatorGapEntranceY(G4double valMy){
	collEntranceGapY = valMy;
}

inline void DetectorConstruction::SetCollimatorGapExit1X(G4double valMy){
	collExit1GapX = valMy;
}

inline void DetectorConstruction::SetCollimatorGapExit1Y(G4double valMy){
	collExit1GapY = valMy;
}

#endif // DetectorConstruction_h

