#include "TrackingAction.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

TrackingAction::TrackingAction() {}
TrackingAction::~TrackingAction() {}

void TrackingAction::PreUserTrackingAction(const G4Track *track)
{
    // Şimdilik burada iş yapmıyoruz
}

void TrackingAction::PostUserTrackingAction(const G4Track *track)
{
    if (track->GetDefinition()->GetParticleName() == "neutron")
    {
        auto status = track->GetTrackStatus();

        // Kaç adım attığını yazdır
        G4int nSteps = track->GetCurrentStepNumber();
        G4cout << ">>> Neutron finished after "
               << nSteps << " steps." << G4endl;

        // Son kinetik enerjisini yazdır
        G4double eFinal = track->GetKineticEnergy();
        G4cout << ">>> Neutron final energy: "
               << eFinal / MeV << " MeV" << G4endl;

        // Time of flight (global time)
        G4double tof = track->GetGlobalTime();
        G4cout << ">>> Neutron time of flight: "
               << tof / ns << " ns" << G4endl;

        // Escape olmuş mu kontrol et (Transportation ile bitmişse)
        if (status == fStopAndKill || status == fKillTrackAndSecondaries)
        {
            const G4VProcess *proc = track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep();
            if (proc)
            {
                G4String pname = proc->GetProcessName();
                if (pname == "Transportation")
                {
                    G4ThreeVector pos = track->GetPosition();
                    G4cout << ">>> Escaped neutron at "
                           << pos << " [mm]" << G4endl;
                }
            }
        }
    }
}