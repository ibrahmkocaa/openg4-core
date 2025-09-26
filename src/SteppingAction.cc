#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

SteppingAction::SteppingAction() {}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    const G4Track* track = step->GetTrack();
    const G4StepPoint* postStep = step->GetPostStepPoint();
    const G4VProcess* process = postStep->GetProcessDefinedStep();

    // Sadece nötronlar için history yazdır
    if (track->GetDefinition()->GetParticleName() == "neutron")
    {
        if (process)
        {
            G4String procName = process->GetProcessName();
            G4ThreeVector pos = postStep->GetPosition();
            G4double ekin = track->GetKineticEnergy();

            G4cout << "    [History] Step " 
                   << track->GetCurrentStepNumber()
                   << " : " << procName
                   << " at " << pos
                   << " with E = " << ekin/MeV << " MeV"
                   << G4endl;
        }
    }

    // Mevcut fission/capture analiz kodların burada kalabilir
    if (process)
    {
        G4String procName = process->GetProcessName();

        if (procName == "nFission" || procName == "nFissionHP")
        {
            G4ThreeVector pos = postStep->GetPosition();
            G4cout << ">>> " << procName 
                   << " at position " << pos << " [mm]" << G4endl;

            // Secondaries yazdırma kodun burada kalacak
            const auto* secondaries = step->GetSecondaryInCurrentStep();
            for (auto sec : *secondaries)
            {
                G4String pname = sec->GetDefinition()->GetParticleName();
                G4ThreeVector spos = sec->GetPosition();
                G4double ekin = sec->GetKineticEnergy();

                if (sec->GetDefinition()->GetParticleType() == "nucleus")
                {
                    G4int Z = sec->GetDefinition()->GetAtomicNumber();
                    G4int A = sec->GetDefinition()->GetAtomicMass();

                    G4cout << "    ---> Fission fragment: " << pname
                           << " (Z=" << Z << ", A=" << A << ")"
                           << " with E=" << ekin/MeV << " MeV"
                           << " at " << spos << " [mm]" << G4endl;
                }
                else
                {
                    G4cout << "    ---> Secondary: " << pname
                           << " with E=" << ekin/MeV << " MeV"
                           << " at " << spos << " [mm]" << G4endl;
                }
            }
        }
        else if (procName == "nCapture" || procName == "nCaptureHP")
        {
            G4ThreeVector pos = postStep->GetPosition();
            G4cout << ">>> " << procName 
                   << " at position " << pos << " [mm]" << G4endl;
        }
    }
}
