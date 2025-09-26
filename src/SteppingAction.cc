#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

SteppingAction::SteppingAction() {}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    const G4StepPoint* postStep = step->GetPostStepPoint();
    const G4VProcess* process = postStep->GetProcessDefinedStep();

    if (process)
    {
        G4String procName = process->GetProcessName();

        // --- Fission olayını yakala ---
        if (procName == "nFission" || procName == "nFissionHP")
        {
            G4ThreeVector pos = postStep->GetPosition();
            G4cout << ">>> " << procName 
                   << " at position " << pos << " [mm]" << G4endl;

            // İkinci kuşak parçacıkları al
            const auto* secondaries = step->GetSecondaryInCurrentStep();
            for (auto sec : *secondaries)
            {
                G4String pname = sec->GetDefinition()->GetParticleName();
                G4ThreeVector spos = sec->GetPosition();
                G4double ekin = sec->GetKineticEnergy();

                // Eğer parçacık bir iyon/izotop ise (ör. Sr94, Xe140, Ba144, Kr90 ...)
                if (sec->GetDefinition()->GetParticleType() == "nucleus")
                {
                    G4int Z = sec->GetDefinition()->GetAtomicNumber();
                    G4int A = sec->GetDefinition()->GetAtomicMass();

                    G4cout << "    ---> Fission fragment: " << pname 
                           << " (Z=" << Z << ", A=" << A << ")"
                           << " with E=" << ekin/MeV << " MeV"
                           << " at " << spos << " [mm]" << G4endl;
                }

                // Ayrıca nötron, gamma, beta gibi hafif parçacıkları da görmek istersen:
                else if (pname == "neutron" || pname == "gamma" || pname == "e-" || pname == "beta-")
                {
                    G4cout << "    ---> Secondary: " << pname 
                           << " with E=" << ekin/MeV << " MeV"
                           << " at " << spos << " [mm]" << G4endl;
                }
            }
        }
    }
}
