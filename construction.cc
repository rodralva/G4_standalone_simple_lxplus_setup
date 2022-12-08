#include "construction.hh"
#include "G4GDMLParser.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    // We need to specify the material before the detector is defined:
    // Some are already predefined so we pick the ones we need
    G4NistManager *nist = G4NistManager::Instance();

    Air = nist->FindOrBuildMaterial("G4_AIR");
    LAr = nist->FindOrBuildMaterial("G4_lAr");
    Plastic = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    Metal = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    
    const G4int num = 2;  
    G4double energy[num] = {1.239841939*eV/0.128, 1.239841939*eV/0.9}; //momentum of optical photon; conversion wavelenght(um) to energy 
    G4double rindexAir[num] = {1.0, 1.0}; // Refraction index for propagation (for photons to propagate in Air)
    G4double rindexLAr[num] = {1.38, 1.38}; //not considering dispersion so assume rindex constant
    G4double reflectivity[num] = {0.94, 0.94};
    // G4double efficiency[num] = {0.8, 0.1};

    G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
    mptAir->AddProperty("RINDEX", energy, rindexAir, num);
    Air->SetMaterialPropertiesTable(mptAir);

    G4MaterialPropertiesTable *mptLAr = new G4MaterialPropertiesTable();
    mptLAr->AddProperty("RINDEX", energy, rindexLAr, num); //energy=mom in relativistic approximation
    LAr->SetMaterialPropertiesTable(mptLAr);

}

void MyDetectorConstruction::ConstructScintillator()
{
// >> COLORS:
//    white = G4Colour(1.0, 1.0, 1.0);
//    pink = G4Colour(0.94, 0.5, 0.5);
//    grey = G4Colour(0.46, 0.53, 0.6);
//    yellow = G4Colour(1.0, 1.0, 0.);
//    blue = G4Colour(0.25,0.41, 0.88 );
//    lightBlue = G4Colour(0.28, 0.82, 0.8);
//    green = G4Colour(0., 1., 0.);
//    brown = G4Colour(0.5, 0.5, 0.);
//    purple = G4Colour(0.85,0.44,0.84);
//    red = G4Colour(1.0, 0.0, 0.0);
//    orange = G4Colour(1.,0.5,0.); 
//    black =  G4Colour(0.,0.,0.); 

    // >> Set visAttributes for various components
    G4VisAttributes* logicSCVisAtt = new G4VisAttributes(G4Colour(0.25,0.88, 0.41, 0.5)); //green
    logicSCVisAtt->SetForceSolid(true);


    // >> SC Frame (small)
    G4Box *solidSC = new G4Box("solidSC", 37.5*mm, 49*mm, 2*mm); //200x75 mm a 5.5 cm de la fuente (172 medidas)
    logicSC1 = new G4LogicalVolume(solidSC, LAr, "logicSC1");
    logicSC2 = new G4LogicalVolume(solidSC, LAr, "logicSC2");
    physSC1 = new G4PVPlacement(0, G4ThreeVector(0*mm,  51*mm, -2000*mm), logicSC1, "physSC1", logicWorld, false, 1, true); 
    physSC2 = new G4PVPlacement(0, G4ThreeVector(0*mm, -51*mm, -2000*mm), logicSC2, "physSC2", logicWorld, false, 1, true); 
    logicSC1->SetVisAttributes(logicSCVisAtt);
    logicSC2->SetVisAttributes(logicSCVisAtt);

    // SBND from xml ~not working for now
    // G4GDMLParser parser;
    // parser.Read("gdml/sbnd_v02_00.gdml");
    // G4VPhysicalVolume *w=parser.GetWorldVolume();
    
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    DefineMaterials();

    solidWorld = new G4Box("solidWorld", 4000*mm, 4000*mm, 2000*mm); //(name, half of the length)
    logicWorld = new G4LogicalVolume(solidWorld, LAr, "logicWorld"); //(solid, material, name)
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //(rotation, center coordinates, mothervolume, name, ..., true=checks overlaps ALWAYS)
    logicWorld->SetVisAttributes(new G4VisAttributes(G4Colour(0.,0.,0.)));

    ConstructScintillator();
    
    return physWorld; // return highest mother volume
}


void MyDetectorConstruction::ConstructSDandField()
{

    // >> Dos ventanas carla separadas por centro insensible
    MySensitiveDetector *sensSC1 = new MySensitiveDetector("SensitiveSC1");
    MySensitiveDetector *sensSC2 = new MySensitiveDetector("SensitiveSC2");
    logicSC1->SetSensitiveDetector(sensSC1);
    logicSC2->SetSensitiveDetector(sensSC2);

}