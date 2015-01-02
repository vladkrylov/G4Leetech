#include "DetectorConstruction.hh"
#include "DetectorConstructionP2.hh"
#include "DetectorConstructionP3.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

//#include "TROOT.h"
#include "TMath.h"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UserLimits.hh"


#include "MagneticField.hh"
//magnetic field
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4UserLimits.hh"


//C, C++
#include "assert.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:magField(0),
 solidWorld(0),logicWorld(0),physiWorld(0),
 solidBeamPipe(0),logicBeamPipe(0),physiBeamPipe(0),
 solidBeamPipeV(0),logicBeamPipeV(0),physiBeamPipeV(0),
 solidCup(0),logicCup(0),physiCup(0),
 solidAperture(0),logicAperture(0),physiAperture(0),
 solidSenDet1(0),logicSenDet1(0),physiSenDet1(0),
 solidShield1(0),logicShield1(0), physiShield1(0),
 solidSenDet2(0),logicSenDet2(0),physiSenDet2(0),
 solidDipol_L(0),logicDipol_L(0),physiDipol_L(0)
{
  
  // materials
  DefineMaterials();

  //swich the magneric field
  SetMagField(300);
  SetCupLenght(0.4*cm);
  SetApertureInRadius(0.1*cm);
  SetApertureLenght(7.0*cm);
  SetGeomID(3);

  SetMaxStep(8*m);
  SetMaxLength(8*mm);
  SetMaxTime(8*s);
  SetMinEkin(8*MeV);
  SetMionRang(8*m);

  //LB 08.02.2012
  magField = new MagneticField();

  // create commands for interactive definition of the calorimeter
  geom4 = new DetectorConstructionP2(this, magField);
  geom7 = new DetectorConstructionP3(this, magField);
  detectorMessenger = new DetectorMessenger(this,geom4 ,geom7 );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ 
  delete geom4;
  delete geom7;
  delete stepLimit;
  delete detectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	switch(_geomID)
	{
	case 12:
		return geom7->ConstructGeom12();
		break;
	case 11:
		return geom7->ConstructGeom11();
		break;
	case 10:
		return geom7->ConstructGeom10();
		break;
	case 9:
		return geom7->ConstructGeom9();
		break;
	case 8:
		return geom7->ConstructGeom8();
		break;
	case 7:
		return geom7->ConstructGeom7();
		break;
	case 6:
		return geom4->ConstructGeom6();
		break;
	case 5:
		return geom4->ConstructGeom5();
		break;
	case 4:
		return geom4->ConstructGeom4();
		break;
	case 3:
		return ConstructGeom3();
		break;
	case 2:
		return ConstructGeom2();
		break;
	case 1:
		return ConstructGeom1();
		break;
	default:
		G4cout<<"Can not identity of the geometry!\n";
		assert(0);
		return 0;
		break;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials(){ 
  //This function illustrates the possible ways to define materials
  
  G4String symbol;             //a=mass of a mole;
  G4double a, z, density;      //z=mean number of protons;  
//  G4int iz, n;                 //iz=number of protons  in an isotope; 
                               // n=number of nucleons in an isotope;
  
  G4int ncomponents;//, natoms;
  //G4double abundance, fractionmass;
  G4double fractionmass;
  
  //
  // define Elements and simple materials
  //
  
  //G4Element* H  = new G4Element("Hydrogen",  symbol="H",  z= 1.00, a= 1.0100*g/mole);
  //G4Element* C  = new G4Element("Carbon",    symbol="C",  z= 6.00, a= 12.010*g/mole);
  G4Element* N  = new G4Element("Nitrogen",  symbol="N",  z= 7.00, a= 14.010*g/mole);
  G4Element* O  = new G4Element("Oxygen",    symbol="O",  z= 8.00, a= 16.000*g/mole);
  //G4Element* Si = new G4Element("Silicon",   symbol="Si", z= 14.0, a= 28.090*g/mole);
  G4Element* Cr = new G4Element("Chromium",  symbol="Cr", z= 24.0, a= 51.996*g/mole);
  G4Element* Mn = new G4Element("Manganese", symbol="Mn", z= 25.0, a= 54.938*g/mole);
  G4Element* Fe = new G4Element("Ferum",     symbol="Fe", z= 26.0, a= 55.845*g/mole);
  G4Element* Ni = new G4Element("Nickel",    symbol="Ni", z= 28.0, a= 58.693*g/mole);
  
  alMy          = new G4Material("Aluminium", z=13.0, a=26.980*g/mole, density=2.700*g/cm3);
  titanMy       = new G4Material("Titan",     z=22.0, a=47.867*g/mole, density=4.506*g/cm3);

  G4Lead = new G4Material("Lead",     z=82.0, a=207.2*g/mole, density=11.3*g/cm3);
  
  G4NistManager* nistMan = G4NistManager::Instance();
  U = nistMan->FindOrBuildMaterial("G4_U");

  //////////////////////////Stainless Steel///////////////////////////////
  steelMy = new G4Material("steelMy", density= 7.9*g/cm3, ncomponents=4);
  steelMy->AddElement(Fe, fractionmass=0.72);
  steelMy->AddElement(Cr, fractionmass=0.18);
  steelMy->AddElement(Ni, fractionmass=0.08);
  steelMy->AddElement(Mn, fractionmass=0.02);
  ////////////////////////////////////////////////////////////////////////
  
  /////////////////////////////////Air////////////////////////////////////
  airMy = new G4Material("Air", density= 1.290*mg/cm3, ncomponents=2);
  airMy->AddElement(N, fractionmass=0.7);
  airMy->AddElement(O, fractionmass=0.3);
  ///////////////////////////////////////////////////////////////////////

  /////////////////////////////beamVacuum////////////////////////////////
  beamVacuum = new G4Material("beamVacuum", density= 1.290*mg/(1.0e11)/cm3, ncomponents=2);
  beamVacuum->AddElement(N, fractionmass=0.7);
  beamVacuum->AddElement(O, fractionmass=0.3);
  ///////////////////////////////////////////////////////////////////////

  /////////////////////////////vacuumMy//////////////////////////////////
  //vacuumMy = new G4Material("vacuumMy", density= 1.290*mg/(1.0e3)/cm3, ncomponents=2);
  vacuumMy = new G4Material("vacuumMy", density= 1.290*mg/(1.0e5)/cm3, ncomponents=2);
  //vacuumMy = new G4Material("vacuumMy", density= 1.290*mg/(1.0e60)/cm3, ncomponents=2);
  vacuumMy->AddElement(N, fractionmass=0.7);
  vacuumMy->AddElement(O, fractionmass=0.3);
  ///////////////////////////////////////////////////////////////////////

  // print table
  //
  //G4cout <<*(G4Material::GetMaterialTable()) << G4endl;  
}

G4VPhysicalVolume* DetectorConstruction::ConstructGeom3(){
  
  G4double WorldSizeX         = 120.0*cm;
  G4double WorldSizeY         = WorldSizeX;
  G4double WorldSizeZ         = 170.0*cm;  

  G4double gapSize            = 1.0*mm;

  G4double beamPipeLenght     = 20.0*cm;
  G4double beamPipeOutRadius  = 3.00*cm;
  G4double beamPipeInRadius   = 2.70*cm;
  G4double beamPipeVOutRadius = beamPipeInRadius - 0.1*cm;

  G4double cupOutRadius       = beamPipeOutRadius;
  G4double cupLenght          = _cupLenght;

  G4double apertureOutRadius  = 3*cm;
  G4double apertureInRadius   = _apertureInRadius;
  G4double apertureLenght     = _apertureLenght;

G4cout<<"DET In RADIUS = "<<_Det1InRad<<std::endl;
G4cout<<"DET Out RADIUS = "<<_Det1OutRad<<std::endl;

  G4double senDet1OutRadius   = WorldSizeY/2 - 0.2*cm;
  G4double senDet1InRadius    = apertureOutRadius + 0.1*cm;
  G4double senDet1Lenght      = 0.1*cm;
  G4double shield1OutRadius   = apertureOutRadius;//senDet1OutRadius;
  G4double shield1InRadius    = _apertureInRadius;//senDet1InRadius;
  G4double shield1Lenght      = 7.0*cm;

  G4double senDet2_X          = WorldSizeX - 0.5*cm;
  G4double senDet2_Y          = WorldSizeY - 0.5*cm;
  G4double senDet2_Z          = senDet1Lenght;

  //Razmer geometricheskij v ploskosti zakruchivanija electrona:  ~53 cm x ~31 cm.
  //Razmer (oblast"  polya) v ploskosti zakruchivanija electrona: ~50 cm x ~25 cm.
  //Maximal"noe pole: 570 G, eto znachit, kak uchit Alessandro, chto mozhno podnimat"  pole do 450-470 G.
  //Ponyatno, chto pole neodnorodno po krajam, no dlya Geanta eto poka, navernoe, ne nuzhno, da my i ne znaem 
  G4double dipol_LR_X         = 5.00*cm;
  G4double dipol_LR_Y         = 42.0*cm;
  G4double dipol_LR_Z         = 53.0*cm;
  G4double dipol_TB_X         = 55.0*cm;//in realyty 62 = 31 + 31 but effective size of the field + gap between two magnets
  G4double dipol_TB_Y         = 5.00*cm;
  G4double dipol_TB_Z         = 53.0*cm;

  G4double fieldBox_X         = 25.0*cm; 
  G4double fieldBox_Y         = 30.0*cm;
  G4double fieldBox_Z         = 50.0*cm;

  G4double x_tot = dipol_TB_X + gapSize + gapSize + dipol_LR_X + dipol_LR_X;
  G4double y_tot = dipol_LR_Y;
  G4double z_tot = beamPipeLenght + gapSize + cupLenght + gapSize + apertureLenght + gapSize + dipol_LR_Z;

	//Checking validity of set value
  if(WorldSizeX<x_tot){
    G4cout<<"WorldSizeX < x_tot"<<G4endl
	  <<"WorldSizeX = "<<WorldSizeX<<G4endl
	  <<"x_tot      = "<<x_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeY<y_tot){
    G4cout<<"WorldSizeY < y_tot"<<G4endl
	  <<"WorldSizeY = "<<WorldSizeY<<G4endl
	  <<"y_tot      = "<<y_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeZ<z_tot){
    G4cout<<"WorldSizeZ < z_tot"<<G4endl
	  <<"WorldSizeZ = "<<WorldSizeZ<<G4endl
	  <<"z_tot      = "<<z_tot<<G4endl;
    assert(0);
  }
  if(dipol_TB_X<fieldBox_X*2){
    G4cout<<"dipol_TB_X   < fieldBox_X*2"<<G4endl
	  <<"dipol_TB_X   = "<<dipol_TB_X<<G4endl
	  <<"fieldBox_X*2 = "<<fieldBox_X*2<<G4endl;
    assert(0);
  }
  if(dipol_LR_Z<fieldBox_Z){
    G4cout<<"dipol_LR_Z < fieldBox_Z"<<G4endl
	  <<"dipol_LR_Z = "<<dipol_LR_Z<<G4endl
	  <<"fieldBox_Z = "<<fieldBox_Z<<G4endl;
    assert(0);
  }
  if((dipol_LR_Y - dipol_TB_Y*2)<fieldBox_Y){
    G4cout<<"(dipol_LR_Y - dipol_TB_Y*2) < fieldBox_Y"<<G4endl
	  <<" dipol_LR_Y - dipol_TB_Y*2  = "<<dipol_LR_Y - dipol_TB_Y*2<<G4endl
	  <<" fieldBox_Y                 = "<<fieldBox_Y<<G4endl;
    assert(0);
  }
  
// setting constans of geometry
  G4double beamPipe_xc = 0.0;
  G4double beamPipe_yc = 0.0;
  G4double beamPipe_zc = -cupLenght/2.0 - gapSize - beamPipeLenght/2.0;

  G4double cup_xc      = 0.0;
  G4double cup_yc      = 0.0;
  G4double cup_zc      = 0.0;

  G4double aperture_xc = 0.0;
  G4double aperture_yc = 0.0;
  G4double aperture_zc = cupLenght/2.0 + gapSize + apertureLenght/2.0;

  G4double dipol_L_xc  = beamPipeOutRadius + gapSize + dipol_LR_X/2.0;
  G4double dipol_L_yc  = 0.0;
  G4double dipol_L_zc  = cupLenght/2.0 + gapSize + apertureLenght + gapSize + dipol_LR_Z/2.0;

  G4double dipol_R_xc  = dipol_L_xc - dipol_LR_X/2.0 - gapSize - dipol_TB_X - gapSize - dipol_LR_X/2.0;
  G4double dipol_R_yc  = 0.0;
  G4double dipol_R_zc  = dipol_L_zc;

  G4double dipol_T_xc  = dipol_L_xc - gapSize - dipol_LR_X/2.0 - dipol_TB_X/2.0;
  G4double dipol_T_yc  = dipol_LR_Y/2.0 - dipol_TB_Y/2.0;
  G4double dipol_T_zc  = dipol_L_zc;

  G4double dipol_B_xc  = dipol_T_xc;
  G4double dipol_B_yc  = -dipol_LR_Y/2.0 + dipol_TB_Y/2.0;
  G4double dipol_B_zc  = dipol_L_zc;

  G4double fieldBox_L_xc = dipol_L_xc - dipol_LR_X/2.0 - gapSize - fieldBox_X/2.0;
  G4double fieldBox_L_yc = 0.0;
  G4double fieldBox_L_zc = cupLenght/2.0 + gapSize + apertureLenght + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;

  G4double fieldBox_R_xc = dipol_R_xc + dipol_LR_X/2.0 + gapSize + fieldBox_X/2.0;
  G4double fieldBox_R_yc = 0.0;
  G4double fieldBox_R_zc = cupLenght/2.0 + gapSize + apertureLenght + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;

  G4double senDet1_xc    = 0.0;
  G4double senDet1_yc    = 0.0;
  G4double senDet1_zc    = aperture_zc + apertureLenght/2.0 - senDet1Lenght/2.0;

  G4double shield1_xc    = 0.0;
  G4double shield1_yc    = 0.0;
  G4double shield1_zc    = senDet1_zc - gapSize*3 - shield1Lenght/2.0;

  G4double senDet2_xc    = 0.0;
  G4double senDet2_yc    = 0.0;
  G4double senDet2_zc    = dipol_T_zc + dipol_TB_Z/2 + gapSize + senDet2_Z/2.0;

  

  G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4cout<<"DetectorConstruction::ConstructGeom3 _MagFieldVal "<<_MagFieldVal<<G4endl;
  magField->setBfieldY(_MagFieldVal);
  magField->setFieldBox1(fieldBox_L_xc, fieldBox_L_yc, fieldBox_L_zc, 
			 fieldBox_X, fieldBox_Y, fieldBox_Z);
  magField->setFieldBox2(fieldBox_R_xc, fieldBox_R_yc, fieldBox_R_zc, 
			 fieldBox_X, fieldBox_Y, fieldBox_Z);
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  fieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
   
  //     
  // World
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"World material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);                     
  logicWorld = new G4LogicalVolume(solidWorld,	    //its solid
				   vacuumMy,        //its material
				   "World");	    //its name                                   
  physiWorld = new G4PVPlacement(0,		    //no rotation
  				 G4ThreeVector(),   //at (0,0,0)
				 logicWorld,	    //its logical volume				 
				 "World",	    //its name
				 0,		    //its mother  volume
				 false,		    //no boolean operation
				 0);		    //copy number

  //
  // Beampipe
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Beampipe material"<<G4endl
	//<<steelMy<<G4endl;
  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe,	//its solid
				      steelMy,         	//its material
				      "BeamPipe");	//its name
  physiBeamPipe = new G4PVPlacement(0,			//no rotation
				    G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),	//its position
				    logicBeamPipe,	//its logical volume				 
				    "BeamPipe",		//its name
				    logicWorld,		//its mother  volume
				    false,		//no boolean operation
				    0);			//copy number

  //
  // Beampipe Vacuum
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Beampipe Vacuum material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV,	//its solid
				       beamVacuum,      //its material
				       "BeamPipeV");	//its name
  physiBeamPipeV = new G4PVPlacement(0,			//no rotation
				     G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc), //its position
				     logicBeamPipeV,	//its logical volume				 
				     "BeamPipeV",	//its name
				     logicWorld,	//its mother  volume
				     false,		//no boolean operation
				     0);		//copy number

  //
  // Beampipe cup - absorber
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Cup"<<G4endl
	//<<alMy<<G4endl;
  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup,		//its solid
				 alMy,	//its material
				 "Cup");		//its name  
  physiCup = new G4PVPlacement(0,			//no rotation
			       G4ThreeVector(cup_xc,cup_yc,cup_zc),//its position
			       logicCup,		//its logical volume				 
			       "Cup",		        //its name
			       logicWorld,		//its mother  volume
			       false,			//no boolean operation
			       0);		        //copy number

  //
  //Aperture -colim
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"aperture"<<G4endl
	//<<alMy<<G4endl;
  solidAperture = new G4Tubs("Aperture", apertureInRadius, apertureOutRadius, apertureLenght/2.0, 0, 360.0*deg);
  logicAperture = new G4LogicalVolume(solidAperture,	    //its solid
				      alMy,	            //its material
				      "Aperture");          //its name  
  physiAperture = new G4PVPlacement(0,			    //no rotation
				    G4ThreeVector(aperture_xc,aperture_yc,aperture_zc),//its position
				    logicAperture,	    //its logical volume				 
				    "Cup",		    //its name
				    logicWorld,		    //its mother  volume
				    false,		    //no boolean operation
				    0);		            //copy number



  //     
  // Dipol left - blue
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol left  material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_L = new G4Box("Dipol_L",dipol_LR_X/2,dipol_LR_Y/2,dipol_LR_Z/2);
  logicDipol_L = new G4LogicalVolume(solidDipol_L,  //its solid
				     alMy,          //its material
				     "Dipol_L");    //its name
  physiDipol_L = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_L_xc, dipol_L_yc, dipol_L_zc),   //at (0,0,0)
				   logicDipol_L,	    //its logical volume
				   "Dipol_L",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol right
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol right material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_R = new G4Box("Dipol_R",dipol_LR_X/2,dipol_LR_Y/2,dipol_LR_Z/2);
  logicDipol_R = new G4LogicalVolume(solidDipol_R,  //its solid
				     alMy,          //its material
				     "Dipol_R");    //its name
  physiDipol_R = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_R_xc, dipol_R_yc, dipol_R_zc),   //at (0,0,0)
				   logicDipol_R,	    //its logical volume
				   "Dipol_R",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol top
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol top material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_T = new G4Box("Dipol_T",dipol_TB_X/2,dipol_TB_Y/2,dipol_TB_Z/2);
  logicDipol_T = new G4LogicalVolume(solidDipol_T,  //its solid
				     alMy,          //its material
				     "Dipol_T");    //its name
  physiDipol_T = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_T_xc, dipol_T_yc, dipol_T_zc),   //at (0,0,0)
				   logicDipol_T,    //its logical volume
				   "Dipol_T",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol bottom
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol bottom material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_B = new G4Box("Dipol_B",dipol_TB_X/2,dipol_TB_Y/2,dipol_TB_Z/2);
  logicDipol_B = new G4LogicalVolume(solidDipol_B,  //its solid
				     alMy,          //its material
				     "Dipol_B");    //its name
  physiDipol_B = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_B_xc, dipol_B_yc, dipol_B_zc),   //at (0,0,0)
				   logicDipol_B,    //its logical volume
				   "Dipol_B",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Field box left
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Field box left  material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidFieldBox_L = new G4Box("FieldBox_L",fieldBox_X/2,fieldBox_Y/2,fieldBox_Z/2);
  logicFieldBox_L = new G4LogicalVolume(solidFieldBox_L,    //its solid
					vacuumMy,           //its material
					"FieldBox_L");      //its name
  physiFieldBox_L = new G4PVPlacement(0,		    //no rotation
				      G4ThreeVector(fieldBox_L_xc, fieldBox_L_yc, fieldBox_L_zc),   //its position
				      logicFieldBox_L,	    //its logical volume
				      "FieldBox_L",	    //its name
				      logicWorld,	    //its mother  volume
				      false,     	    //no boolean operation
				      0);		    //copy number

  //     
  // Field box right
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Field box left  material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidFieldBox_R = new G4Box("FieldBox_R",fieldBox_X/2,fieldBox_Y/2,fieldBox_Z/2);
  logicFieldBox_R = new G4LogicalVolume(solidFieldBox_R,    //its solid
					vacuumMy,           //its material
					"FieldBox_R");      //its name
  physiFieldBox_R = new G4PVPlacement(0,		    //no rotation
				      G4ThreeVector(fieldBox_R_xc, fieldBox_R_yc, fieldBox_R_zc),   //its position
				      logicFieldBox_R,	    //its logical volume
				      "FieldBox_R",	    //its name
				      logicWorld,	    //its mother  volume
				      false,     	    //no boolean operation
				      0);		    //copy number

  
  //     
  // Sensative detector 1
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Sensative detector 1 material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidSenDet1 = new G4Tubs("SenDet1", senDet1InRadius, senDet1OutRadius, senDet1Lenght/2.0, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     beamVacuum,
				     "SenDet1");
  
  physiSenDet1 = new G4PVPlacement(0,		       	//no rotation
				   G4ThreeVector(senDet1_xc,senDet1_yc,senDet1_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume				 
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

   
  // Shield 1
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Shield 1 1 material"<<G4endl
	//<<alMy<<G4endl;

  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Shield size"<<G4endl
	//<<shield1InRadius<<"  "<<shield1OutRadius<<G4endl;

  solidShield1 = new G4Tubs("Shield1", shield1InRadius, shield1OutRadius, shield1Lenght/2.0, 0, 360.0*deg);
  logicShield1 = new G4LogicalVolume(solidShield1,
				     alMy,
				     "Shield1");
  
  physiShield1 = new G4PVPlacement(0,		       	//no rotation
				   G4ThreeVector(shield1_xc,shield1_yc,shield1_zc), //at (0,0,0)
				   logicShield1,	//its logical volume				 
				   "Shield1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number


  //     
  // Sensative detector 2
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Sensative detector 2 material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidSenDet2 = new G4Box("SenDet2", senDet2_X/2, senDet2_Y/2, senDet2_Z/2);
  logicSenDet2 = new G4LogicalVolume(solidSenDet2,  //its solid
				     beamVacuum,    //its material
				     "SenDet2");    //its name
  physiSenDet2 = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(senDet2_xc, senDet2_yc, senDet2_zc),   //its position
				   logicSenDet2,    //its logical volume
				   "SenDet2",	    //its name
				   logicWorld,	    //its mother  volume
				   false,     	    //no boolean operation
				   0);		    //copy number

  //                                        
  // Visualization attributes
  //

  G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  logicCup->SetVisAttributes(cupVisAtt);

  G4VisAttributes* alVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicBeamPipe->SetVisAttributes(alVisAtt);
  logicDipol_L->SetVisAttributes(alVisAtt);
  logicDipol_R->SetVisAttributes(alVisAtt);
  logicDipol_T->SetVisAttributes(alVisAtt);
  logicDipol_B->SetVisAttributes(alVisAtt);

  G4VisAttributes* appVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  logicAperture->SetVisAttributes(appVisAtt);

  G4VisAttributes* magVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  logicFieldBox_L->SetVisAttributes(magVisAtt);
  logicFieldBox_R->SetVisAttributes(magVisAtt);

  G4VisAttributes* beamVacuumVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  logicBeamPipeV->SetVisAttributes(beamVacuumVisAtt);

  //Slate Blue
  G4VisAttributes* senDetVisAtt = new G4VisAttributes(G4Colour(106.0/255.0, 90.0/255.0, 205.0/255.0));
  logicSenDet1->SetVisAttributes(senDetVisAtt);
  logicSenDet2->SetVisAttributes(senDetVisAtt);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  G4double maxStep   = _maxStep;
  G4double maxLength = _maxLength;
  G4double maxTime   = _maxTime; 
  G4double minEkin   = _minEkin;
  G4double mionRang  = _mionRang;  
/*
  G4double maxStep   = 0.1*mm;
  G4double maxLength = 5.0*m;
  G4double maxTime   = 20.0*ns; 
  G4double minEkin   = 1.0/100*MeV;
  G4double mionRang  = 0.01*mm;  
  */

  stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);
  logicWorld->SetUserLimits(stepLimit);
  logicFieldBox_L->SetUserLimits(stepLimit);
  logicFieldBox_R->SetUserLimits(stepLimit);
  logicCup->SetUserLimits(stepLimit);

  //
  //always return the physical World
  //
  return physiWorld;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume* DetectorConstruction::ConstructGeom2(){

	//magField = new MagneticField();

  G4double WorldSizeX         = 120.0*cm;
  G4double WorldSizeY         = WorldSizeX;
  G4double WorldSizeZ         = 170.0*cm;  

  G4double gapSize            = 1.0*mm;

  G4double beamPipeLenght     = 20.0*cm;
  G4double beamPipeOutRadius  = 3.00*cm;
  G4double beamPipeInRadius   = 2.70*cm;
  G4double beamPipeVOutRadius = beamPipeInRadius - 0.1*cm;

  G4double cupOutRadius       = beamPipeOutRadius;
  G4double cupLenght          = _cupLenght;

  G4double apertureOutRadius  = beamPipeOutRadius;
  G4double apertureInRadius   = _apertureInRadius;
  G4double apertureLenght     = _apertureLenght;

  G4double senDet1OutRadius   =_Det1OutRad;//WorldSizeY/2 - 0.2*cm; 
  G4double senDet1InRadius    = _Det1InRad;
  G4double senDet1Lenght      = 0.1*cm;

  G4double shield1OutRadius   = beamPipeOutRadius;
 G4double shield1InRadius    = senDet1OutRadius;//_Det1OutRad;
  G4double shield1Lenght      = 1.0*cm;

  //Razmer geometricheskij v ploskosti zakruchivanija electrona:  ~53 cm x ~31 cm.
  //Razmer (oblast"  polya) v ploskosti zakruchivanija electrona: ~50 cm x ~25 cm.
  //Maximal"noe pole: 570 G, eto znachit, kak uchit Alessandro, chto mozhno podnimat"  pole do 450-470 G.
  //Ponyatno, chto pole neodnorodno po krajam, no dlya Geanta eto poka, navernoe, ne nuzhno, da my i ne znaem 
  G4double dipol_LR_X         = 5.00*cm;
  G4double dipol_LR_Y         = 42.0*cm;
  G4double dipol_LR_Z         = 53.0*cm;
  G4double dipol_TB_X         = 55.0*cm;//in realyty 62 = 31 + 31 but effective size of the field + gap between two magnets
  G4double dipol_TB_Y         = 5.00*cm;
  G4double dipol_TB_Z         = 53.0*cm;

  G4double fieldBox_X         = 25.0*cm; 
  G4double fieldBox_Y         = 30.0*cm;
  G4double fieldBox_Z         = 50.0*cm;

  G4double x_tot = dipol_TB_X + gapSize + gapSize + dipol_LR_X + dipol_LR_X;
  G4double y_tot = dipol_LR_Y;
  G4double z_tot = beamPipeLenght + gapSize + cupLenght + gapSize + apertureLenght + gapSize + dipol_LR_Z;

	//Checking validity of set value
  if(WorldSizeX<x_tot){
    G4cout<<"WorldSizeX < x_tot"<<G4endl
	  <<"WorldSizeX = "<<WorldSizeX<<G4endl
	  <<"x_tot      = "<<x_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeY<y_tot){
    G4cout<<"WorldSizeY < y_tot"<<G4endl
	  <<"WorldSizeY = "<<WorldSizeY<<G4endl
	  <<"y_tot      = "<<y_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeZ<z_tot){
    G4cout<<"WorldSizeZ < z_tot"<<G4endl
	  <<"WorldSizeZ = "<<WorldSizeZ<<G4endl
	  <<"z_tot      = "<<z_tot<<G4endl;
    assert(0);
  }
  if(dipol_TB_X<fieldBox_X*2){
    G4cout<<"dipol_TB_X   < fieldBox_X*2"<<G4endl
	  <<"dipol_TB_X   = "<<dipol_TB_X<<G4endl
	  <<"fieldBox_X*2 = "<<fieldBox_X*2<<G4endl;
    assert(0);
  }
  if(dipol_LR_Z<fieldBox_Z){
    G4cout<<"dipol_LR_Z < fieldBox_Z"<<G4endl
	  <<"dipol_LR_Z = "<<dipol_LR_Z<<G4endl
	  <<"fieldBox_Z = "<<fieldBox_Z<<G4endl;
    assert(0);
  }
  if((dipol_LR_Y - dipol_TB_Y*2)<fieldBox_Y){
    G4cout<<"(dipol_LR_Y - dipol_TB_Y*2) < fieldBox_Y"<<G4endl
	  <<" dipol_LR_Y - dipol_TB_Y*2  = "<<dipol_LR_Y - dipol_TB_Y*2<<G4endl
	  <<" fieldBox_Y                 = "<<fieldBox_Y<<G4endl;
    assert(0);
  }
  
// setting constans of geometry
  G4double beamPipe_xc = 0.0;
  G4double beamPipe_yc = 0.0;
  G4double beamPipe_zc = -cupLenght/2.0 - gapSize - beamPipeLenght/2.0;

  G4double cup_xc      = 0.0;
  G4double cup_yc      = 0.0;
  G4double cup_zc      = 0.0;

  G4double aperture_xc = 0.0;
  G4double aperture_yc = 0.0;
  G4double aperture_zc = cupLenght/2.0 + gapSize + apertureLenght/2.0;

  G4double dipol_L_xc  = beamPipeOutRadius + gapSize + dipol_LR_X/2.0;
  G4double dipol_L_yc  = 0.0;
  G4double dipol_L_zc  = cupLenght/2.0 + gapSize + apertureLenght + gapSize + dipol_LR_Z/2.0;

  G4double dipol_R_xc  = dipol_L_xc - dipol_LR_X/2.0 - gapSize - dipol_TB_X - gapSize - dipol_LR_X/2.0;
  G4double dipol_R_yc  = 0.0;
  G4double dipol_R_zc  = dipol_L_zc;

  G4double dipol_T_xc  = dipol_L_xc - gapSize - dipol_LR_X/2.0 - dipol_TB_X/2.0;
  G4double dipol_T_yc  = dipol_LR_Y/2.0 - dipol_TB_Y/2.0;
  G4double dipol_T_zc  = dipol_L_zc;

  G4double dipol_B_xc  = dipol_T_xc;
  G4double dipol_B_yc  = -dipol_LR_Y/2.0 + dipol_TB_Y/2.0;
  G4double dipol_B_zc  = dipol_L_zc;

  G4double fieldBox_L_xc = dipol_L_xc - dipol_LR_X/2.0 - gapSize - fieldBox_X/2.0;
  G4double fieldBox_L_yc = 0.0;
  G4double fieldBox_L_zc = cupLenght/2.0 + gapSize + apertureLenght + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;

  G4double fieldBox_R_xc = dipol_R_xc + dipol_LR_X/2.0 + gapSize + fieldBox_X/2.0;
  G4double fieldBox_R_yc = 0.0;
  G4double fieldBox_R_zc = cupLenght/2.0 + gapSize + apertureLenght + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;

  G4double senDet1_xc    = -_Det1X;
  G4double senDet1_yc    = 0.0;
  G4double senDet1_zc    = gapSize - shield1Lenght + cupLenght/2 + apertureLenght - senDet1Lenght/2.0;

  G4double shield1_xc    = -_Det1X;
  G4double shield1_yc    = 0.0;
  G4double shield1_zc    = gapSize - shield1Lenght/2.0 + cupLenght/2 + apertureLenght;


  G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4cout<<"DetectorConstruction::ConstructGeom3 _MagFieldVal "<<_MagFieldVal<<G4endl;
  magField->setBfieldY(_MagFieldVal);
  magField->setFieldBox1(fieldBox_L_xc, fieldBox_L_yc, fieldBox_L_zc, 
			 fieldBox_X, fieldBox_Y, fieldBox_Z);
  magField->setFieldBox2(fieldBox_R_xc, fieldBox_R_yc, fieldBox_R_zc, 
			 fieldBox_X, fieldBox_Y, fieldBox_Z);
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  fieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
   
  //     
  // World
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"World material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);                     
  logicWorld = new G4LogicalVolume(solidWorld,	    //its solid
				   vacuumMy,        //its material
				   "World");	    //its name                                   
  physiWorld = new G4PVPlacement(0,		    //no rotation
  				 G4ThreeVector(),   //at (0,0,0)
				 logicWorld,	    //its logical volume				 
				 "World",	    //its name
				 0,		    //its mother  volume
				 false,		    //no boolean operation
				 0);		    //copy number

  //
  // Beampipe
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Beampipe material"<<G4endl
	//<<steelMy<<G4endl;
  solidBeamPipe = new G4Tubs("BeamPipe", beamPipeInRadius, beamPipeOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipe = new G4LogicalVolume(solidBeamPipe,	//its solid
				      steelMy,         	//its material
				      "BeamPipe");	//its name
  physiBeamPipe = new G4PVPlacement(0,			//no rotation
				    G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc),	//its position
				    logicBeamPipe,	//its logical volume				 
				    "BeamPipe",		//its name
				    logicWorld,		//its mother  volume
				    false,		//no boolean operation
				    0);			//copy number

  //
  // Beampipe Vacuum
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Beampipe Vacuum material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidBeamPipeV = new G4Tubs("BeamPipeV", 0.0, beamPipeVOutRadius, beamPipeLenght/2.0, 0, 360.0*deg);
  logicBeamPipeV = new G4LogicalVolume(solidBeamPipeV,	//its solid
				       beamVacuum,      //its material
				       "BeamPipeV");	//its name
  physiBeamPipeV = new G4PVPlacement(0,			//no rotation
				     G4ThreeVector(beamPipe_xc,beamPipe_yc,beamPipe_zc), //its position
				     logicBeamPipeV,	//its logical volume				 
				     "BeamPipeV",	//its name
				     logicWorld,	//its mother  volume
				     false,		//no boolean operation
				     0);		//copy number

  //
  // Beampipe cup - absorber
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Cup"<<G4endl
	//<<alMy<<G4endl;
  solidCup = new G4Tubs("Cup", 0.0*cm, cupOutRadius, cupLenght/2.0, 0, 360.0*deg);
  logicCup = new G4LogicalVolume(solidCup,		//its solid
				 alMy,	//its material
				 "Cup");		//its name  
  physiCup = new G4PVPlacement(0,			//no rotation
			       G4ThreeVector(cup_xc,cup_yc,cup_zc),//its position
			       logicCup,		//its logical volume				 
			       "Cup",		        //its name
			       logicWorld,		//its mother  volume
			       false,			//no boolean operation
			       0);		        //copy number

  //
  //Aperture -colim
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"aperture"<<G4endl
	//<<alMy<<G4endl;
  solidAperture = new G4Tubs("Aperture", apertureInRadius, apertureOutRadius, apertureLenght/2.0, 0, 360.0*deg);
  logicAperture = new G4LogicalVolume(solidAperture,	    //its solid
				      alMy,	            //its material
				      "Aperture");          //its name  
  physiAperture = new G4PVPlacement(0,			    //no rotation
				    G4ThreeVector(aperture_xc,aperture_yc,aperture_zc),//its position
				    logicAperture,	    //its logical volume				 
				    "Cup",		    //its name
				    logicWorld,		    //its mother  volume
				    false,		    //no boolean operation
				    0);		            //copy number



  //     
  // Dipol left - blue
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol left  material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_L = new G4Box("Dipol_L",dipol_LR_X/2,dipol_LR_Y/2,dipol_LR_Z/2);
  logicDipol_L = new G4LogicalVolume(solidDipol_L,  //its solid
				     alMy,          //its material
				     "Dipol_L");    //its name
  physiDipol_L = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_L_xc, dipol_L_yc, dipol_L_zc),   //at (0,0,0)
				   logicDipol_L,	    //its logical volume
				   "Dipol_L",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol right
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol right material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_R = new G4Box("Dipol_R",dipol_LR_X/2,dipol_LR_Y/2,dipol_LR_Z/2);
  logicDipol_R = new G4LogicalVolume(solidDipol_R,  //its solid
				     alMy,          //its material
				     "Dipol_R");    //its name
  physiDipol_R = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_R_xc, dipol_R_yc, dipol_R_zc),   //at (0,0,0)
				   logicDipol_R,	    //its logical volume
				   "Dipol_R",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol top
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol top material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_T = new G4Box("Dipol_T",dipol_TB_X/2,dipol_TB_Y/2,dipol_TB_Z/2);
  logicDipol_T = new G4LogicalVolume(solidDipol_T,  //its solid
				     alMy,          //its material
				     "Dipol_T");    //its name
  physiDipol_T = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_T_xc, dipol_T_yc, dipol_T_zc),   //at (0,0,0)
				   logicDipol_T,    //its logical volume
				   "Dipol_T",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Dipol bottom
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Dipol bottom material"<<G4endl
	//<<alMy<<G4endl;
  solidDipol_B = new G4Box("Dipol_B",dipol_TB_X/2,dipol_TB_Y/2,dipol_TB_Z/2);
  logicDipol_B = new G4LogicalVolume(solidDipol_B,  //its solid
				     alMy,          //its material
				     "Dipol_B");    //its name
  physiDipol_B = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(dipol_B_xc, dipol_B_yc, dipol_B_zc),   //at (0,0,0)
				   logicDipol_B,    //its logical volume
				   "Dipol_B",	    //its name
				   logicWorld,	    //its mother  volume
				   false,	    //no boolean operation
				   0);		    //copy number

  //     
  // Field box left
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Field box left  material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidFieldBox_L = new G4Box("FieldBox_L",fieldBox_X/2,fieldBox_Y/2,fieldBox_Z/2);
  logicFieldBox_L = new G4LogicalVolume(solidFieldBox_L,    //its solid
					vacuumMy,           //its material
					"FieldBox_L");      //its name
  physiFieldBox_L = new G4PVPlacement(0,		    //no rotation
				      G4ThreeVector(fieldBox_L_xc, fieldBox_L_yc, fieldBox_L_zc),   //its position
				      logicFieldBox_L,	    //its logical volume
				      "FieldBox_L",	    //its name
				      logicWorld,	    //its mother  volume
				      false,     	    //no boolean operation
				      0);		    //copy number

  //     
  // Field box right
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Field box left  material"<<G4endl
	//<<vacuumMy<<G4endl;
  solidFieldBox_R = new G4Box("FieldBox_R",fieldBox_X/2,fieldBox_Y/2,fieldBox_Z/2);
  logicFieldBox_R = new G4LogicalVolume(solidFieldBox_R,    //its solid
					vacuumMy,           //its material
					"FieldBox_R");      //its name
  physiFieldBox_R = new G4PVPlacement(0,		    //no rotation
				      G4ThreeVector(fieldBox_R_xc, fieldBox_R_yc, fieldBox_R_zc),   //its position
				      logicFieldBox_R,	    //its logical volume
				      "FieldBox_R",	    //its name
				      logicWorld,	    //its mother  volume
				      false,     	    //no boolean operation
				      0);		    //copy number

  
  //     
  // Sensative detector 1
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Sensative detector 1 material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidSenDet1 = new G4Tubs("SenDet1", senDet1InRadius, senDet1OutRadius, senDet1Lenght/2.0, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     beamVacuum,
				     "SenDet1");
  
  physiSenDet1 = new G4PVPlacement(0,		       	//no rotation
				   G4ThreeVector(senDet1_xc,senDet1_yc,senDet1_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume				 
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

  //     
  // Shield 1
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Shield 1 1 material"<<G4endl
	//<<alMy<<G4endl;
  solidShield1 = new G4Tubs("Shield1", shield1InRadius, shield1OutRadius, shield1Lenght/2.0, 0, 360.0*deg);
  logicShield1 = new G4LogicalVolume(solidShield1,
				     alMy,
				     "Shield1");
  
  physiShield1 = new G4PVPlacement(0,		       	//no rotation
				   G4ThreeVector(shield1_xc,shield1_yc,shield1_zc), //at (0,0,0)
				   logicShield1,	//its logical volume				 
				   "Shield1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number

/*
  //     
  // Sensative detector 2
  //
  //G4cout<<G4endl<<G4endl<<G4endl
	//<<"--------------------"<<G4endl
	//<<"Sensative detector 2 material"<<G4endl
	//<<beamVacuum<<G4endl;
  solidSenDet2 = new G4Box("SenDet2", senDet2_X/2, senDet2_Y/2, senDet2_Z/2);
  logicSenDet2 = new G4LogicalVolume(solidSenDet2,  //its solid
				     beamVacuum,    //its material
				     "SenDet2");    //its name
  physiSenDet2 = new G4PVPlacement(0,		    //no rotation
				   G4ThreeVector(senDet2_xc, senDet2_yc, senDet2_zc),   //its position
				   logicSenDet2,    //its logical volume
				   "SenDet2",	    //its name
				   logicWorld,	    //its mother  volume
				   false,     	    //no boolean operation
				   0);		    //copy number
*/
  //                                        
  // Visualization attributes
  //

  G4VisAttributes* cupVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  logicCup->SetVisAttributes(cupVisAtt);

  G4VisAttributes* alVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicBeamPipe->SetVisAttributes(alVisAtt);
  logicDipol_L->SetVisAttributes(alVisAtt);
  logicDipol_R->SetVisAttributes(alVisAtt);
  logicDipol_T->SetVisAttributes(alVisAtt);
  logicDipol_B->SetVisAttributes(alVisAtt);

  G4VisAttributes* appVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  logicAperture->SetVisAttributes(appVisAtt);

  G4VisAttributes* magVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  logicFieldBox_L->SetVisAttributes(magVisAtt);
  logicFieldBox_R->SetVisAttributes(magVisAtt);

  G4VisAttributes* beamVacuumVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  logicBeamPipeV->SetVisAttributes(beamVacuumVisAtt);

  //Slate Blue
  G4VisAttributes* senDetVisAtt = new G4VisAttributes(G4Colour(106.0/255.0, 90.0/255.0, 205.0/255.0));
  logicSenDet1->SetVisAttributes(senDetVisAtt);
 // logicSenDet2->SetVisAttributes(senDetVisAtt);

  G4double maxStep   = _maxStep;
  G4double maxLength = _maxLength;
  G4double maxTime   = _maxTime; 
  G4double minEkin   = _minEkin;
  G4double mionRang  = _mionRang;  

/*
  G4double maxStep   = 0.1*mm;
  G4double maxLength = 5.0*m;
  G4double maxTime   = 20.0*ns; 
  G4double minEkin   = 1.0/100*MeV;
  G4double mionRang  = 0.01*mm;  
*/
G4cout<<std::endl<<"maxStep = "<<std::setw(14)<<_maxStep<<std::endl;
G4cout<<"maxLength = "<<std::setw(14)<<_maxLength<<std::endl;
G4cout<<"maxTime = "<<std::setw(14)<<_maxTime<<std::endl;
G4cout<<"minEkin = "<<std::setw(14)<<_minEkin<<std::endl;
G4cout<<"mionRangHERE = "<<std::setw(14)<<_mionRang<<std::endl<<std::endl;

  stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);
  logicWorld->SetUserLimits(stepLimit);
  logicFieldBox_L->SetUserLimits(stepLimit);
  logicFieldBox_R->SetUserLimits(stepLimit);
  logicCup->SetUserLimits(stepLimit);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  //
  //always return the physical World
  //
  return physiWorld;
}

G4VPhysicalVolume* DetectorConstruction::ConstructGeom1(){

	//magField = new MagneticField();
  
  G4double WorldSizeX         = 120.0*cm;
  G4double WorldSizeY         = WorldSizeX;
  G4double WorldSizeZ         = 170.0*cm;  

  G4double gapSize            = 1.0*mm;

  G4double beamPipeLenght     = 20.0*cm;
  G4double beamPipeOutRadius  = 3.00*cm;
  G4double beamPipeInRadius   = 2.70*cm;
  G4double beamPipeVOutRadius = beamPipeInRadius - 0.1*cm;

  G4double cupOutRadius       = beamPipeOutRadius;
  //G4double cupLenght          = 0.4*cm;
  G4double cupLenght          = _cupLenght;

  G4double apertureLenght     = _apertureLenght;

  G4double senDet1OutRadius   =_Det1OutRad;//WorldSizeY/2 - 0.2*cm; 
  G4double senDet1InRadius    = _Det1InRad;
  G4double senDet1Lenght      = 10*cm;

  //Razmer geometricheskij v ploskosti zakruchivanija electrona:  ~53 cm x ~31 cm.
  //Razmer (oblast"  polya) v ploskosti zakruchivanija electrona: ~50 cm x ~25 cm.
  //Maximal"noe pole: 570 G, eto znachit, kak uchit Alessandro, chto mozhno podnimat"  pole do 450-470 G.
  //Ponyatno, chto pole neodnorodno po krajam, no dlya Geanta eto poka, navernoe, ne nuzhno, da my i ne znaem 
  G4double dipol_LR_X         = 5.00*cm;
  G4double dipol_LR_Y         = 42.0*cm;
  G4double dipol_LR_Z         = 53.0*cm;
  G4double dipol_TB_X         = 55.0*cm;//in realyty 62 = 31 + 31 but effective size of the field + gap between two magnets
  G4double dipol_TB_Y         = 5.00*cm;
  G4double dipol_TB_Z         = 53.0*cm;

  G4double fieldBox_X         = 25.0*cm; 
  G4double fieldBox_Y         = 30.0*cm;
  G4double fieldBox_Z         = 50.0*cm;

  G4double x_tot = dipol_TB_X + gapSize + gapSize + dipol_LR_X + dipol_LR_X;
  G4double y_tot = dipol_LR_Y;
  G4double z_tot = beamPipeLenght + gapSize + cupLenght + gapSize + apertureLenght + gapSize + dipol_LR_Z;

	//Checking validity of set value
  if(WorldSizeX<x_tot){
    G4cout<<"WorldSizeX < x_tot"<<G4endl
	  <<"WorldSizeX = "<<WorldSizeX<<G4endl
	  <<"x_tot      = "<<x_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeY<y_tot){
    G4cout<<"WorldSizeY < y_tot"<<G4endl
	  <<"WorldSizeY = "<<WorldSizeY<<G4endl
	  <<"y_tot      = "<<y_tot<<G4endl;
    assert(0);
  }
  if(WorldSizeZ<z_tot){
    G4cout<<"WorldSizeZ < z_tot"<<G4endl
	  <<"WorldSizeZ = "<<WorldSizeZ<<G4endl
	  <<"z_tot      = "<<z_tot<<G4endl;
    assert(0);
  }
  if(dipol_TB_X<fieldBox_X*2){
    G4cout<<"dipol_TB_X   < fieldBox_X*2"<<G4endl
	  <<"dipol_TB_X   = "<<dipol_TB_X<<G4endl
	  <<"fieldBox_X*2 = "<<fieldBox_X*2<<G4endl;
    assert(0);
  }
  if(dipol_LR_Z<fieldBox_Z){
    G4cout<<"dipol_LR_Z < fieldBox_Z"<<G4endl
	  <<"dipol_LR_Z = "<<dipol_LR_Z<<G4endl
	  <<"fieldBox_Z = "<<fieldBox_Z<<G4endl;
    assert(0);
  }
  if((dipol_LR_Y - dipol_TB_Y*2)<fieldBox_Y){
    G4cout<<"(dipol_LR_Y - dipol_TB_Y*2) < fieldBox_Y"<<G4endl
	  <<" dipol_LR_Y - dipol_TB_Y*2  = "<<dipol_LR_Y - dipol_TB_Y*2<<G4endl
	  <<" fieldBox_Y                 = "<<fieldBox_Y<<G4endl;
    assert(0);
  }
  
// setting constans of geometry
//  G4double beamPipe_xc = 0.0;
//  G4double beamPipe_yc = 0.0;
//  G4double beamPipe_zc = -cupLenght/2.0 - gapSize - beamPipeLenght/2.0;
//
//  G4double cup_xc      = 0.0;
//  G4double cup_yc      = 0.0;
//  G4double cup_zc      = 0.0;
//
//  //G4double aperture_xc = -_Det1X;//16.7*cm;
//  //G4double aperture_yc = 0.0;
//  G4double aperture_zc = - apertureLenght/2.0 + gapSize + cupLenght/2.0;
//
//  G4double dipol_L_xc  = beamPipeOutRadius + gapSize + dipol_LR_X/2.0;
//  G4double dipol_L_yc  = 0.0;
//  G4double dipol_L_zc  = cupLenght/2.0 + gapSize + dipol_LR_Z/2.0;
//
//  G4double dipol_R_xc  = dipol_L_xc - dipol_LR_X/2.0 - gapSize - dipol_TB_X - gapSize - dipol_LR_X/2.0;
//  G4double dipol_R_yc  = 0.0;
//  G4double dipol_R_zc  = dipol_L_zc;
//
//  G4double dipol_T_xc  = dipol_L_xc - gapSize - dipol_LR_X/2.0 - dipol_TB_X/2.0;
//  G4double dipol_T_yc  = dipol_LR_Y/2.0 - dipol_TB_Y/2.0;
//  G4double dipol_T_zc  = dipol_L_zc;
//
//  G4double dipol_B_xc  = dipol_T_xc;
//  G4double dipol_B_yc  = -dipol_LR_Y/2.0 + dipol_TB_Y/2.0;
//  G4double dipol_B_zc  = dipol_L_zc;
//
//  G4double fieldBox_L_xc = dipol_L_xc - dipol_LR_X/2.0 - gapSize - fieldBox_X/2.0;
//  G4double fieldBox_L_yc = 0.0;
//  G4double fieldBox_L_zc = cupLenght/2.0 + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;
//
//  G4double fieldBox_R_xc = dipol_R_xc + dipol_LR_X/2.0 + gapSize + fieldBox_X/2.0;
//  G4double fieldBox_R_yc = 0.0;
//  G4double fieldBox_R_zc = cupLenght/2.0 + gapSize + (dipol_TB_Z - fieldBox_Z)/2.0 + fieldBox_Z/2.0;
//
  G4double targetThickness = 2*mm;
  G4double targetRadius = 2*mm;

  G4double senDetThickness = 5*mm;
  G4double senDetRadius = 30*mm;

  G4double l1 = 0*mm;
  G4double l2 = 5*mm;

  G4double target_xc    = 0;
  G4double target_yc    = 0;
  G4double target_zc    = targetThickness/2 + l1;
  G4double senDet1_zc    = l1 + targetThickness + l2 + senDetThickness/2;


  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
   
  //     
  // World
  //
  solidWorld = new G4Box("World",WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);                     
  logicWorld = new G4LogicalVolume(solidWorld,	    //its solid
				   vacuumMy,        //its material
				   "World");	    //its name                                   
  physiWorld = new G4PVPlacement(0,		    //no rotation
  				 G4ThreeVector(),   //at (0,0,0)
				 logicWorld,	    //its logical volume				 
				 "World",	    //its name
				 0,		    //its mother  volume
				 false,		    //no boolean operation
				 0);		    //copy number


  G4Tubs* targetTube = new G4Tubs("Target", 0, targetRadius, targetThickness/2, 0, 360.0*deg);
  G4LogicalVolume* logicTarget = new G4LogicalVolume(targetTube, alMy, "Target");
  G4VPhysicalVolume* physiTarget = new G4PVPlacement(0,
		  	  	  	  	  	  	  	G4ThreeVector(target_xc,target_yc,target_zc),
		  	  	  	  	  	  	  	logicTarget,
		  	  	  	  	  	  	  	"Target",
		  	  	  	  	  	  	  	logicWorld,
		  	  	  	  	  	  	  	false,
		  	  	  	  	  	  	  	0);
  G4VisAttributes* senDetVisAtt = new G4VisAttributes(G4Colour::Green());
  logicTarget->SetVisAttributes(senDetVisAtt);

  //////////////////////// Detector ///////////////////////////
  solidSenDet1 = new G4Tubs("SenDet1", 0, senDetRadius, senDetThickness/2, 0, 360.0*deg);
  logicSenDet1 = new G4LogicalVolume(solidSenDet1,
				     beamVacuum,
				     "SenDet1");
  
  physiSenDet1 = new G4PVPlacement(0,		       	//no rotation
				   G4ThreeVector(target_xc,target_yc,senDet1_zc), //at (0,0,0)
				   logicSenDet1,	//its logical volume				 
				   "SenDet1",		//its name
				   logicWorld,	     	//its mother  volume
				   false,      		//no boolean operation
				   0);			//copy number
  //Slate Blue
  G4VisAttributes* senDetVisAtt1 = new G4VisAttributes(G4Colour(106.0/255.0, 90.0/255.0, 205.0/255.0));
  logicSenDet1->SetVisAttributes(senDetVisAtt1);

  G4double maxStep   = _maxStep;
  G4double maxLength = _maxLength;
  G4double maxTime   = _maxTime; 
  G4double minEkin   = _minEkin;
  G4double mionRang  = _mionRang;  

/*
  G4double maxStep   = 0.1*mm;
  G4double maxLength = 5.0*m;
  G4double maxTime   = 20.0*ns; 
  G4double minEkin   = 1.0/100*MeV;
  G4double mionRang  = 0.01*mm;  
*/
G4cout<<std::endl<<"maxStep = "<<std::setw(14)<<_maxStep<<std::endl;
G4cout<<"maxLength = "<<std::setw(14)<<_maxLength<<std::endl;
G4cout<<"maxTime = "<<std::setw(14)<<_maxTime<<std::endl;
G4cout<<"minEkin = "<<std::setw(14)<<_minEkin<<std::endl;
G4cout<<"mionRangHERE = "<<std::setw(14)<<_mionRang<<std::endl<<std::endl;

  stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);

  logicWorld->SetUserLimits(stepLimit);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  //
  //always return the physical World
  //
  return physiWorld;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry(){
  G4cout<<"DetectorConstruction::UpdateGeometry()"<<G4endl;
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

G4Material *DetectorConstruction::GetMaterial(G4int t)
{
  switch(t)
  {
	  //test = alMy;
	case 1:
		return alMy;
		break;
	case 2:
		return steelMy;
		break;
	case 3:
		return airMy;
		break;
	case 4:
		return titanMy;
		break;
	case 5:
		return vacuumMy;
		break;
	case 6:
		return beamVacuum;
		break;
	case 7:
		return G4Lead;
		break;
	case 8:
		return U;
			break;
  default:
	  return beamVacuum;
		break;
  }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
