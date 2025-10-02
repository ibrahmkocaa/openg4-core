#include "DetectorConstruction.hh"
#include "Materials.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
  // --- Malzemeleri tek yerden yükle ---
  Mat::Init();

  // =======================
  // World volume
  // =======================
  G4double world_size = 100 * cm;
  auto solidWorld = new G4Box("World", world_size, world_size, world_size);
  auto logicWorld = new G4LogicalVolume(solidWorld, Mat::World(), "World");
  auto physWorld = new G4PVPlacement(nullptr, {}, logicWorld, "World", nullptr, false, 0, true);

  // =======================
  // Boyut parametreleri
  // =======================
  G4double fuel_or = 0.39218 * cm;    // Fuel yarıçapı
  G4double clad_or = 0.45720 * cm;    // Cladding dış yarıçapı
  G4double pin_length = 400 * cm;     // Yakıt çubuğu uzunluğu
  G4double pitch = 21.42 * cm / 17.0; // ≈ 1.26 cm
  int N = 17;

  // =======================
  // Assembly kutusu
  // =======================
  G4double assembly_half = (N * pitch) / 2.0;
  auto solidAssemblyBox = new G4Box("AssemblyBox", assembly_half, assembly_half, pin_length / 2.);
  auto logicAssemblyBox = new G4LogicalVolume(solidAssemblyBox, Mat::Water(), "AssemblyBox");
  new G4PVPlacement(nullptr, {}, logicAssemblyBox, "AssemblyBox", logicWorld, false, 0, true);

  // =======================
  // Fuel / Clad / Cell
  // =======================
  auto solidFuel = new G4Tubs("Fuel", 0, fuel_or, pin_length / 2, 0. * deg, 360. * deg);
  auto logicFuel = new G4LogicalVolume(solidFuel, Mat::Fuel(), "Fuel");

  auto solidClad = new G4Tubs("Clad", fuel_or, clad_or, pin_length / 2, 0. * deg, 360. * deg);
  auto logicClad = new G4LogicalVolume(solidClad, Mat::Clad(), "Clad");

  auto solidCell = new G4Tubs("Cell", 0, clad_or, pin_length / 2, 0. * deg, 360. * deg);
  auto logicCell = new G4LogicalVolume(solidCell, Mat::Water(), "Cell");

  new G4PVPlacement(nullptr, {}, logicFuel, "Fuel", logicCell, false, 0, true);
  new G4PVPlacement(nullptr, {}, logicClad, "Clad", logicCell, false, 0, true);

  // =======================
  // Kontrol Çubuğu (Guide yerine)
  // =======================
  auto solidCtrlRod = new G4Tubs("CtrlRod", 0, fuel_or, pin_length / 2, 0. * deg, 360. * deg);
  auto logicCtrlRod = new G4LogicalVolume(solidCtrlRod, Mat::B4C(), "CtrlRod");  // <-- B4C

  auto solidCtrlClad = new G4Tubs("CtrlClad", fuel_or, clad_or, pin_length / 2, 0. * deg, 360. * deg);
  auto logicCtrlClad = new G4LogicalVolume(solidCtrlClad, Mat::Clad(), "CtrlClad");  // <-- Clad (Zr veya SS)

  auto solidCtrlCell = new G4Tubs("CtrlCell", 0, clad_or, pin_length / 2, 0. * deg, 360. * deg);
  auto logicCtrlCell = new G4LogicalVolume(solidCtrlCell, Mat::Water(), "CtrlCell");

  new G4PVPlacement(nullptr, {}, logicCtrlRod, "CtrlRod", logicCtrlCell, false, 0, true);
  new G4PVPlacement(nullptr, {}, logicCtrlClad, "CtrlClad", logicCtrlCell, false, 0, true);

  // =======================
  // Instrument Tube (su dolu)
  // =======================
  auto solidInstr = new G4Tubs("Instr", 0, clad_or, pin_length / 2, 0. * deg, 360. * deg);
  auto logicInstr = new G4LogicalVolume(solidInstr, Mat::Water(), "Instr");

  // OpenMC ile aynı 24 guide + 1 instrument
  std::vector<std::pair<int, int>> guidePositions = {
      {5, 2}, {8, 2}, {11, 2}, {3, 3}, {13, 3}, {2, 5}, {5, 5}, {8, 5}, {11, 5}, {14, 5},
      {2, 8}, {5, 8}, {8, 8}, {11, 8}, {14, 8}, {2, 11}, {5, 11}, {8, 11}, {11, 11}, {14, 11},
      {3, 13}, {13, 13}, {5, 14}, {8, 14}, {11, 14}
  };
  std::pair<int, int> instPos = {8, 8};

  int copyNo = 0;
  double offset = (N - 1) * pitch / 2.0;
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      auto pos = G4ThreeVector(-offset + i * pitch, -offset + j * pitch, 0);

      if (i == instPos.first && j == instPos.second)
      {
        new G4PVPlacement(nullptr, pos, logicInstr, "InstrumentTube", logicAssemblyBox, false, copyNo++, true);
        continue;
      }
      bool isGuide = false;
      for (auto &gp : guidePositions)
      {
        if (gp.first == i && gp.second == j)
        {
          isGuide = true;
          break;
        }
      }
      if (isGuide)
      {
        new G4PVPlacement(nullptr, pos, logicCtrlCell, "ControlRodCell", logicAssemblyBox, false, copyNo++, true);
      }
      else
      {
        new G4PVPlacement(nullptr, pos, logicCell, "FuelPinCell", logicAssemblyBox, false, copyNo++, true);
      }
    }
  }

  // =======================
  // Görselleştirme
  // =======================
  auto visFuel = new G4VisAttributes(G4Colour(1, 0, 0));
  visFuel->SetForceSolid(true);
  logicFuel->SetVisAttributes(visFuel);

  auto visClad = new G4VisAttributes(G4Colour(0.8, 0.8, 0.8));
  visClad->SetForceSolid(true);
  logicClad->SetVisAttributes(visClad);

  auto visCell = new G4VisAttributes(G4Colour(0, 0, 1, 0.2));
  visCell->SetForceSolid(true);
  logicCell->SetVisAttributes(visCell);

  auto visCtrlRod = new G4VisAttributes(G4Colour(0, 0, 0));   // Siyah: B4C
  visCtrlRod->SetForceSolid(true);
  logicCtrlRod->SetVisAttributes(visCtrlRod);

  auto visCtrlClad = new G4VisAttributes(G4Colour(0.6, 0.6, 0.6)); // Gri: Clad
  visCtrlClad->SetForceSolid(true);
  logicCtrlClad->SetVisAttributes(visCtrlClad);

  auto visCtrlCell = new G4VisAttributes(G4Colour(0, 1, 0, 0.1));  // Şeffaf yeşil: su
  logicCtrlCell->SetVisAttributes(visCtrlCell);

  auto visInstr = new G4VisAttributes(G4Colour(0.5, 0.5, 1.0, 0.7));
  visInstr->SetForceSolid(true);
  logicInstr->SetVisAttributes(visInstr);

  logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
  return physWorld;
}
