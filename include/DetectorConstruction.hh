#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

/// DetectorConstruction sınıfı
/// Burada geometri tanımlamaları yapılır
class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

  // Geometriyi kuran fonksiyon (zorunlu)
  virtual G4VPhysicalVolume *Construct();
};

#endif
