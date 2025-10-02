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
  static G4Material *g_b4c = nullptr;   // <-- B4C eklendi

  void Init()
  {
    if (inited)
      return;
    inited = true;

    auto *nist = G4NistManager::Instance();

    // ==========================
    // World (hava) ve su
    // ==========================
    g_world = nist->FindOrBuildMaterial("G4_AIR");
    g_water = nist->FindOrBuildMaterial("G4_WATER");

    // ==========================
    // Fuel (UO2, OpenMC ile aynı)
    // ==========================
    auto U234 = new G4Isotope("U234", 92, 234, 234.04 * g / mole);
    auto U235 = new G4Isotope("U235", 92, 235, 235.04 * g / mole);
    auto U238 = new G4Isotope("U238", 92, 238, 238.05 * g / mole);
    auto O16  = new G4Isotope("O16",  8,  16,  16.00 * g / mole);

    auto Uenr = new G4Element("EnrichedU", "U", 3);
    Uenr->AddIsotope(U234, 0.0197);
    Uenr->AddIsotope(U235, 4.45);
    Uenr->AddIsotope(U238, 95.53);

    auto Oxy = new G4Element("Oxygen16", "O", 1);
    Oxy->AddIsotope(O16, 1.0);

    g_uo2 = new G4Material("Fuel", 10.29769 * g / cm3, 2);
    g_uo2->AddElement(Uenr, 1);
    g_uo2->AddElement(Oxy, 2);

    // ==========================
    // Cladding (Zirkonyum)
    // ==========================
    G4Element *Zr = nist->FindOrBuildElement("Zr");
    g_zirc = new G4Material("Zirconium", 6.52 * g / cm3, 1);
    g_zirc->AddElement(Zr, 1);

    // ==========================
    // Control Rod (B4C)
    // ==========================

    // --- Doğal Bor tanımı (yaklaşık %19.9 B-10, %80.1 B-11) ---
    auto B10 = new G4Isotope("B10", 5, 10, 10.0129 * g / mole);
    auto B11 = new G4Isotope("B11", 5, 11, 11.0093 * g / mole);

    auto Boron = new G4Element("Boron", "B", 2);
    Boron->AddIsotope(B10, 0.199);
    Boron->AddIsotope(B11, 0.801);

    // Karbon (doğal)
    G4Element* Carbon = nist->FindOrBuildElement("C");

    g_b4c = new G4Material("B4C", 2.52 * g / cm3, 2);
    g_b4c->AddElement(Boron, 4);
    g_b4c->AddElement(Carbon, 1);

    // Eğer %100 B-10 enriched istersen:
    // auto B10only = new G4Element("Boron10", "B10", 1);
    // B10only->AddIsotope(B10, 1.0);
    // g_b4c = new G4Material("B4C_enr", 2.52 * g / cm3, 2);
    // g_b4c->AddElement(B10only, 4);
    // g_b4c->AddElement(Carbon, 1);
  }

  // ==========================
  // Getter fonksiyonları
  // ==========================
  G4Material *World() { return g_world; }
  G4Material *Water() { return g_water; }
  G4Material *Fuel()  { return g_uo2; }
  G4Material *Clad()  { return g_zirc; }
  G4Material *B4C()   { return g_b4c; }

} // namespace Mat
