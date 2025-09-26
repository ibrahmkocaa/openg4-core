#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    // Tek parçacık için ParticleGun oluşturuyoruz
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // Nötron seçimi
    G4ParticleDefinition* particle =
        G4ParticleTable::GetParticleTable()->FindParticle("neutron");
    fParticleGun->SetParticleDefinition(particle);

    // Konum (örnek: assembly’nin merkezinin biraz altında)
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -10.*cm));

    // Yön (pozitif z yönünde yukarıya doğru, assembly içine)
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

    // Enerji (örnek: 2 MeV hızlı nötron)
    fParticleGun->SetParticleEnergy(2.0*MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
