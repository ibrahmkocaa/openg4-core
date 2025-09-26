#include "TrackingAction.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>


TrackingAction::TrackingAction() {}
TrackingAction::~TrackingAction() {}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
    // Burada sadece başlangıç bilgisi alınabilir, gerekirse
}

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
    if (track->GetDefinition()->GetParticleName() == "neutron")
    {
        auto status = track->GetTrackStatus();

        if (status == fStopAndKill || status == fKillTrackAndSecondaries)
        {
            const G4VProcess* proc = track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep();

            if (proc)
            {
                G4String pname = proc->GetProcessName();
                if (pname == "Transportation")
                {
                    G4ThreeVector pos = track->GetPosition();
                    G4cout << ">>> Escaped neutron at " << pos << " [mm]" << G4endl;
                }
            }
        }
    }
}
