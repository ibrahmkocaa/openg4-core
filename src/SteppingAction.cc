#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"   // <-- CSV yazımı için eklendi
#include <iostream>

SteppingAction::SteppingAction() {}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *step)
{
    const G4Track *track = step->GetTrack();
    const G4StepPoint *preStep = step->GetPreStepPoint();
    const G4StepPoint *postStep = step->GetPostStepPoint();
    const G4VProcess *process = postStep->GetProcessDefinedStep();

    // Event ID bilgisini al
    G4int eventID = -1;
    if (auto evt = G4RunManager::GetRunManager()->GetCurrentEvent())
        eventID = evt->GetEventID();

    // Analiz yöneticisi
    auto analysisManager = G4AnalysisManager::Instance();

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
        // CSV’ye kaydet
        analysisManager->FillNtupleIColumn(0, 0, eventID);
        analysisManager->FillNtupleSColumn(0, 1, particle);
        analysisManager->FillNtupleSColumn(0, 2, volName);
        analysisManager->FillNtupleDColumn(0, 3, edep / MeV);
        analysisManager->AddNtupleRow(0);
    }

    // -------------------------------
    // 2. Neutron history
    // -------------------------------
    if (particle == "neutron" && process)
    {
        G4String procName = process->GetProcessName();
        G4ThreeVector pos = postStep->GetPosition();
        G4double ekin = track->GetKineticEnergy();

        // CSV’ye kaydet
        analysisManager->FillNtupleIColumn(1, 0, eventID);
        analysisManager->FillNtupleIColumn(1, 1, track->GetCurrentStepNumber());
        analysisManager->FillNtupleSColumn(1, 2, procName);
        analysisManager->FillNtupleDColumn(1, 3, pos.x() / mm);
        analysisManager->FillNtupleDColumn(1, 4, pos.y() / mm);
        analysisManager->FillNtupleDColumn(1, 5, pos.z() / mm);
        analysisManager->FillNtupleDColumn(1, 6, ekin / MeV);
        analysisManager->AddNtupleRow(1);
    }

    // -------------------------------
    // 3. Capture / Fission analizleri
    // -------------------------------
    if (process && particle == "neutron")
    {
        G4String procName = process->GetProcessName();

        // --- Fission ---
        if (procName == "nFission" || procName == "nFissionHP")
        {
            G4ThreeVector pos = postStep->GetPosition();

            // İkinci kuşak parçacıklar
            const auto *secondaries = step->GetSecondaryInCurrentStep();
            for (auto sec : *secondaries)
            {
                G4String pname = sec->GetDefinition()->GetParticleName();
                G4ThreeVector spos = sec->GetPosition();
                G4double sekE = sec->GetKineticEnergy();

                G4String secType = "secondary";
                G4int Z = 0, A = 0;
                if (sec->GetDefinition()->GetParticleType() == "nucleus")
                {
                    secType = "nucleus";
                    Z = sec->GetDefinition()->GetAtomicNumber();
                    A = sec->GetDefinition()->GetAtomicMass();
                }

                // CSV’ye kaydet
                analysisManager->FillNtupleIColumn(2, 0, eventID);
                analysisManager->FillNtupleSColumn(2, 1, procName);
                analysisManager->FillNtupleDColumn(2, 2, pos.x() / mm);
                analysisManager->FillNtupleDColumn(2, 3, pos.y() / mm);
                analysisManager->FillNtupleDColumn(2, 4, pos.z() / mm);
                analysisManager->FillNtupleSColumn(2, 5, secType);
                analysisManager->FillNtupleSColumn(2, 6, pname);
                analysisManager->FillNtupleIColumn(2, 7, Z);
                analysisManager->FillNtupleIColumn(2, 8, A);
                analysisManager->FillNtupleDColumn(2, 9, sekE / MeV);
                analysisManager->AddNtupleRow(2);
            }
        }

        // --- Capture ---
        else if (procName == "nCapture" || procName == "nCaptureHP")
        {
            G4ThreeVector pos = postStep->GetPosition();

            // CSV’ye kaydet
            analysisManager->FillNtupleIColumn(3, 0, eventID);
            analysisManager->FillNtupleSColumn(3, 1, procName);
            analysisManager->FillNtupleDColumn(3, 2, pos.x() / mm);
            analysisManager->FillNtupleDColumn(3, 3, pos.y() / mm);
            analysisManager->FillNtupleDColumn(3, 4, pos.z() / mm);
            analysisManager->AddNtupleRow(3);
        }
    }
}
