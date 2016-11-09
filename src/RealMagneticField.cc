#include <algorithm>

#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "RealMagneticField.hh"
#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RealMagneticField::RealMagneticField()
: G4MagneticField(), fMessenger(0), By(1.0*tesla)
{
    // define commands for this class
    DefineCommands();

    CreateBZMap();
    detector = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
    z0 = detector->GetMagnetZCenter();


	G4cout << "**********************************************" << G4endl;
	G4cout << "z0 = " << z0 << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RealMagneticField::~RealMagneticField()
{ 
    delete fMessenger; 
}

void RealMagneticField::GetFieldValue(const G4double point[4], G4double *bField) const
{
    bField[0] = 0.;
    bField[1] = 0;
    bField[2] = 0.;

    G4double z = point[2] - z0;
    if ((z < zmin) || (z > zmax))
    	return;

    std::vector<G4double>::const_iterator lower = std::lower_bound(zmap.begin(), zmap.end(), z);
    G4double z1 = *lower;

    G4int ilower = lower - zmap.begin();
    G4double z2 = zmap[ilower+1];
    G4double B1 = Bzmap[ilower];
    G4double B2 = Bzmap[ilower+1];

    bField[1] = By*(B1 + (B2-B1) / (z2-z1) * (z-z1));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RealMagneticField::DefineCommands()
{
    // Define /B5/field command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this, 
                                        "/Leetech/det/",
                                        "Field control");

    // fieldValue command 
    G4GenericMessenger::Command& valueCmd
      = fMessenger->DeclareMethodWithUnit("setField","tesla",
                                  &RealMagneticField::SetField,
                                  "Set field strength.");
    valueCmd.SetParameterName("field", true);
    valueCmd.SetDefaultValue("1.");
}

void RealMagneticField::AddBZPoint(G4double z, G4double B)
{
	zmap.push_back(z);
	Bzmap.push_back(B);
}

void RealMagneticField::CreateBZMap()
{
	const int N = 101;
	G4double digitizedZ[N] = {-500.54, -490.33, -480.67, -471.0, -460.08, -450.41, -440.74, -429.65, -419.98, -410.31, -400.29, -390.26, -380.59, -369.85, -360.19, -350.88, -340.49, -330.83, -320.09, -310.78, -300.04, -290.37, -280.7, -269.96, -260.29, -250.98, -239.89, -230.22, -220.55, -210.88, -200.5, -190.83, -180.81, -170.07, -160.76, -151.09, -140.35, -130.68, -120.3, -109.56, -100.61, -90.58, -79.48, -69.82, -60.51, -50.84, -40.46, -30.43, -20.77, -10.38, -0.72, 8.95, 20.05, 30.08, 39.03, 50.13, 59.79, 69.1, 79.48, 89.87, 99.53, 109.92, 119.58, 129.25, 139.63, 149.66, 159.68, 170.43, 180.81, 189.76, 199.79, 209.81, 220.19, 229.5, 239.53, 249.55, 259.58, 269.6, 279.63, 289.65, 300.04, 310.06, 319.73, 329.75, 340.49, 349.45, 359.47, 369.14, 379.16, 389.19, 398.85, 409.6, 419.62, 428.93, 440.39, 449.7, 459.72, 470.1, 479.77, 489.44, 499.46};
	G4double digitizedB[N] = {0.023349, 0.024155, 0.025765, 0.02657, 0.028986, 0.029522, 0.031133, 0.03328, 0.034353, 0.035963, 0.039184, 0.040794, 0.044552, 0.047772, 0.050456, 0.053677, 0.057971, 0.064412, 0.069243, 0.074611, 0.081052, 0.088567, 0.098229, 0.10789, 0.12131, 0.135266, 0.151906, 0.173913, 0.199678, 0.232958, 0.277509, 0.337091, 0.420827, 0.531401, 0.675255, 0.820183, 0.917338, 0.961353, 0.979603, 0.985507, 0.989265, 0.991948, 0.993559, 0.994632, 0.996779, 0.997316, 0.999463, 0.999463, 1.0, 1.0, 0.999463, 0.998926, 0.999463, 0.999463, 1.0, 0.99839, 0.996243, 0.995169, 0.992485, 0.991948, 0.990338, 0.985507, 0.977992, 0.961353, 0.91358, 0.813741, 0.663983, 0.521739, 0.412775, 0.331723, 0.274289, 0.231884, 0.197531, 0.172303, 0.149758, 0.133119, 0.120236, 0.106817, 0.097155, 0.08803, 0.080515, 0.074074, 0.06817, 0.062802, 0.057434, 0.052603, 0.05153, 0.047772, 0.044552, 0.041331, 0.039184, 0.0365, 0.034353, 0.032206, 0.031133, 0.030059, 0.028986, 0.027912, 0.026838, 0.024691, 0.023618};

//	zmap.assign(digitizedZ, digitizedZ+N);
//	Bzmap.assign(digitizedB, digitizedB+N);

	zmap.clear();
	Bzmap.clear();
	for(int i=0; i<N; i++) {
		AddBZPoint(digitizedZ[i], digitizedB[i]);
	}

	zmin = *std::min_element(zmap.begin(), zmap.end());
	zmax = *std::max_element(zmap.begin(), zmap.end());

//	G4cout << "**********************************************" << G4endl;
//	G4cout << "zmin = " << zmin << "; zmax = " << zmax << G4endl;
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
