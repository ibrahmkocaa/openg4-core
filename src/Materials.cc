#include "Materials.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4SystemOfUnits.hh"

namespace Mat
{

  static bool inited = false;
  static G4Material *g_world = nullptr;
  static G4Material *g_water = nullptr;
  static G4Material *g_uo2 = nullptr;
  static G4Material *g_zirc = nullptr;

  void Init()
  {
    if (inited)
      return;
    inited = true;

    auto *nist = G4NistManager::Instance();

    // World (hava) ve su: NIST’ten
    g_world = nist->FindOrBuildMaterial("G4_AIR");
    g_water = nist->FindOrBuildMaterial("G4_WATER");

    // ==========================
    // Fuel (OpenMC ile aynı)
    // ==========================

    // İzotop tanımları
    auto U234 = new G4Isotope("U234", 92, 234, 234.04 * g / mole);
    auto U235 = new G4Isotope("U235", 92, 235, 235.04 * g / mole);
    auto U238 = new G4Isotope("U238", 92, 238, 238.05 * g / mole);
    auto O16 = new G4Isotope("O16", 8, 16, 16.00 * g / mole);

    // U elementini izotoplarla kur (normalize edilmiş fraksiyonlarla)
    // OpenMC atom yoğunluklarından:
    // U234=4.4843e-6, U235=5.5815e-4, U238=2.2408e-2
    G4double sumU = 4.4843e-6 + 5.5815e-4 + 2.2408e-2;
    G4double fU234 = 4.4843e-6 / sumU;
    G4double fU235 = 5.5815e-4 / sumU;
    G4double fU238 = 2.2408e-2 / sumU;

    auto Uenr = new G4Element("EnrichedU", "U", 3);
    Uenr->AddIsotope(U234, fU234);
    Uenr->AddIsotope(U235, fU235);
    Uenr->AddIsotope(U238, fU238);

    // Oksijen elementini sadece O16 ile tanımla
    auto Oxy = new G4Element("Oxygen16", "O", 1);
    Oxy->AddIsotope(O16, 1.0);

    // Yakıt malzemesi (OpenMC yoğunluğu ile)
    g_uo2 = new G4Material("Fuel", 10.29769 * g / cm3, 2);
    g_uo2->AddElement(Uenr, 1);
    g_uo2->AddElement(Oxy, 2);

    // ==========================
    // Cladding (Zirkonyum)
    // ==========================
    G4Element *Zr = nist->FindOrBuildElement("Zr");
    g_zirc = new G4Material("Zirconium", 6.52 * g / cm3, 1);
    g_zirc->AddElement(Zr, 1);
  }

  G4Material *World() { return g_world; }
  G4Material *Water() { return g_water; }
  G4Material *Fuel() { return g_uo2; }
  G4Material *Clad() { return g_zirc; }

} // namespace Mat
