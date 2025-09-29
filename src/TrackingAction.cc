#include "TrackingAction.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"   // <-- CSV için eklendi
#include <iostream>

TrackingAction::TrackingAction() {}
TrackingAction::~TrackingAction() {}

void TrackingAction::PreUserTrackingAction(const G4Track *track)
{
    // Şimdilik burada iş yapmıyoruz
}

void TrackingAction::PostUserTrackingAction(const G4Track *track)
{
    // Event ID bilgisini al
    G4int eventID = -1;
    if (auto evt = G4RunManager::GetRunManager()->GetCurrentEvent())
        eventID = evt->GetEventID();

    if (track->GetDefinition()->GetParticleName() == "neutron")
    {
        auto analysisManager = G4AnalysisManager::Instance();
        auto status = track->GetTrackStatus();

        // Kaç adım attı
        G4int nSteps = track->GetCurrentStepNumber();

        // Son kinetik enerji
        G4double eFinal = track->GetKineticEnergy();

        // Time of flight (global time)
        G4double tof = track->GetGlobalTime();

        // Escape kontrolü
        G4String escaped = "false";
        G4double x = 0, y = 0, z = 0;

        if (status == fStopAndKill || status == fKillTrackAndSecondaries)
        {
            const G4VProcess *proc = track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep();
            if (proc && proc->GetProcessName() == "Transportation")
            {
                G4ThreeVector pos = track->GetPosition();
                escaped = "true";
                x = pos.x() / mm;
                y = pos.y() / mm;
                z = pos.z() / mm;
            }
        }

        // CSV’ye kaydet
        analysisManager->FillNtupleIColumn(4, 0, eventID);
        analysisManager->FillNtupleIColumn(4, 1, nSteps);
        analysisManager->FillNtupleDColumn(4, 2, eFinal / MeV);
        analysisManager->FillNtupleDColumn(4, 3, tof / ns);
        analysisManager->FillNtupleSColumn(4, 4, escaped);
        analysisManager->FillNtupleDColumn(4, 5, x);
        analysisManager->FillNtupleDColumn(4, 6, y);
        analysisManager->FillNtupleDColumn(4, 7, z);
        analysisManager->AddNtupleRow(4);
    }
}
