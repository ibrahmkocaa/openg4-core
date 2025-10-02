#ifndef Materials_h
#define Materials_h 1

class G4Material;

namespace Mat
{
  // UYGULAMADA 1 kez çağır
  void Init(); // tüm malzemeleri oluşturur (idempotent)

  // Getter'lar
  G4Material *World(); // G4_AIR
  G4Material *Water(); // G4_WATER
  G4Material *Fuel();  // UO2 (ρ=10.97 g/cm3)
  G4Material *Clad();  // Zr   (ρ=6.52 g/cm3)
  G4Material *B4C();   // Bor Karbür (ρ=2.52 g/cm3)
}

#endif
