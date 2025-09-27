#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

SteppingAction::SteppingAction() {}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *step)
{
    const G4Track *track = step->GetTrack();
    const G4StepPoint *preStep = step->GetPreStepPoint();
    const G4StepPoint *postStep = step->GetPostStepPoint();
    const G4VProcess *process = postStep->GetProcessDefinedStep();

    // -------------------------------
    // 1. Enerji depozisyonu analizi
    // -------------------------------
    G4String particle = track->GetDefinition()->GetParticleName();
    const G4VPhysicalVolume *volume = preStep->GetPhysicalVolume();

    G4String volName = "OutOfWorld";
    if (volume)
    {
        volName = volume->GetName();
    }

    G4double edep = step->GetTotalEnergyDeposit();
    if (edep > 0)
    {
        G4cout << ">>> " << particle
               << " deposited " << edep / MeV << " MeV"
               << " in volume " << volName
               << G4endl;
    }

    // -------------------------------
    // 2. Neutron history
    // -------------------------------
    if (particle == "neutron" && process)
    {
        G4String procName = process->GetProcessName();
        G4ThreeVector pos = postStep->GetPosition();
        G4double ekin = track->GetKineticEnergy();

        G4cout << "    [History] Step "
               << track->GetCurrentStepNumber()
               << " : " << procName
               << " at " << pos
               << " with E = " << ekin / MeV << " MeV"
               << G4endl;
    }

    // -------------------------------
    // 3. Capture / Fission / Scatter analizleri
    // -------------------------------
    if (process && particle == "neutron")
    {
        G4String procName = process->GetProcessName();

        // --- Fission ---
        if (procName == "nFission" || procName == "nFissionHP")
        {

            G4ThreeVector pos = postStep->GetPosition();
            G4cout << ">>> " << procName
                   << " at position " << pos << " [mm]" << G4endl;

            // İkinci kuşak parçacıklar
            const auto *secondaries = step->GetSecondaryInCurrentStep();
            for (auto sec : *secondaries)
            {
                G4String pname = sec->GetDefinition()->GetParticleName();
                G4ThreeVector spos = sec->GetPosition();
                G4double sekE = sec->GetKineticEnergy();

                // Yazdırma
                if (sec->GetDefinition()->GetParticleType() == "nucleus")
                {
                    G4int Z = sec->GetDefinition()->GetAtomicNumber();
                    G4int A = sec->GetDefinition()->GetAtomicMass();

                    G4cout << "    ---> Fission fragment: " << pname
                           << " (Z=" << Z << ", A=" << A << ")"
                           << " with E=" << sekE / MeV << " MeV"
                           << " at " << spos << " [mm]" << G4endl;
                }
                else
                {
                    G4cout << "    ---> Secondary: " << pname
                           << " with E=" << sekE / MeV << " MeV"
                           << " at " << spos << " [mm]" << G4endl;
                }
            }
        }

        // --- Capture ---
        else if (procName == "nCapture" || procName == "nCaptureHP")
        {
            G4ThreeVector pos = postStep->GetPosition();
            G4cout << ">>> " << procName
                   << " at position " << pos << " [mm]" << G4endl;
        }
    }
}
