#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Threading.hh" // <-- thread ID için
#include <iostream>
#include <fstream>
#include <sstream>

#include "G4AnalysisManager.hh"

SteppingAction::SteppingAction() {}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *step)
{
    // ------------------------------------------------
    // 1) Thread-local dosya handle (her thread kendi dosyasına yazar)
    // ------------------------------------------------
    static thread_local std::ofstream stepLog;
    if (!stepLog.is_open())
    {
        std::ostringstream fname;
        fname << "output/steps_thread" << G4Threading::G4GetThreadId() << ".log";
        stepLog.open(fname.str(), std::ios::out);
    }

    // ------------------------------------------------
    // 2) Geant4 bilgilerini al
    // ------------------------------------------------
    const G4Track *track = step->GetTrack();
    const G4StepPoint *preStep = step->GetPreStepPoint();
    const G4StepPoint *postStep = step->GetPostStepPoint();
    const G4VProcess *process = postStep->GetProcessDefinedStep();

    G4int eventID = -1;
    if (auto evt = G4RunManager::GetRunManager()->GetCurrentEvent())
        eventID = evt->GetEventID();

    // Analiz yöneticisi
    auto analysisManager = G4AnalysisManager::Instance();

    G4String particle = track->GetDefinition()->GetParticleName();
    const G4VPhysicalVolume *volume = preStep->GetPhysicalVolume();

    G4String volName = "OutOfWorld";
    if (volume)
        volName = volume->GetName();

    // ------------------------------------------------
    // 3) Enerji depozisyonu
    // ------------------------------------------------
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep > 0)
    {
        stepLog << "[Event " << eventID << "] "
                << particle
                << " deposited " << edep / MeV << " MeV"
                << " in volume " << volName
                << std::endl;
        // CSV’ye kaydet
        analysisManager->FillNtupleIColumn(0, 0, eventID);
        analysisManager->FillNtupleSColumn(0, 1, particle);
        analysisManager->FillNtupleSColumn(0, 2, volName);
        analysisManager->FillNtupleDColumn(0, 3, edep / MeV);
        analysisManager->AddNtupleRow(0);
    }

    // ------------------------------------------------
    // 4) Neutron history
    // ------------------------------------------------
    if (particle == "neutron" && process)
    {
        G4String procName = process->GetProcessName();
        G4ThreeVector pos = postStep->GetPosition();
        G4double ekin = track->GetKineticEnergy();

        stepLog << "[Event " << eventID << "] "
                << "[History] Step "
                << track->GetCurrentStepNumber()
                << " : " << procName
                << " at " << pos
                << " with E = " << ekin / MeV << " MeV"
                << std::endl;
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

    // ------------------------------------------------
    // 5) Fission / Capture analizleri
    // ------------------------------------------------
    if (process && particle == "neutron")
    {
        G4String procName = process->GetProcessName();

        // --- Fission ---
        if (procName == "nFission" || procName == "nFissionHP")
        {
            G4ThreeVector pos = postStep->GetPosition();
            stepLog << "[Event " << eventID << "] "
                    << procName
                    << " at position " << pos << " [mm]" << std::endl;

            // İkinci kuşak parçacıklar
            const auto *secondaries = step->GetSecondaryInCurrentStep();
            for (auto sec : *secondaries)
            {
                G4String pname = sec->GetDefinition()->GetParticleName();
                G4ThreeVector spos = sec->GetPosition();
                G4double sekE = sec->GetKineticEnergy();

                if (sec->GetDefinition()->GetParticleType() == "nucleus")
                {
                    G4String secType = "secondary";
                    G4int Z = sec->GetDefinition()->GetAtomicNumber();
                    G4int A = sec->GetDefinition()->GetAtomicMass();

                    stepLog << "    [Event " << eventID << "] "
                            << "---> Fission fragment: " << pname
                            << " (Z=" << Z << ", A=" << A << ")"
                            << " with E=" << sekE / MeV << " MeV"
                            << " at " << spos << " [mm]" << std::endl;
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
                else
                {
                    stepLog << "    [Event " << eventID << "] "
                            << "---> Secondary: " << pname
                            << " with E=" << sekE / MeV << " MeV"
                            << " at " << spos << " [mm]" << std::endl;
                }
            }
        }

        // --- Capture ---
        else if (procName == "nCapture" || procName == "nCaptureHP")
        {
            G4ThreeVector pos = postStep->GetPosition();
            stepLog << "[Event " << eventID << "] "
                    << procName
                    << " at position " << pos << " [mm]" << std::endl;

            // CSV’ye kaydet
            analysisManager->FillNtupleIColumn(3, 0, eventID);
            analysisManager->FillNtupleSColumn(3, 1, procName);
            analysisManager->FillNtupleDColumn(3, 2, pos.x() / mm);
            analysisManager->FillNtupleDColumn(3, 3, pos.y() / mm);
            analysisManager->FillNtupleDColumn(3, 4, pos.z() / mm);
            analysisManager->AddNtupleRow(3);
        }
    }

    // ------------------------------------------------
    // 6) Escaped neutron kontrolü
    // ------------------------------------------------
    if (particle == "neutron")
    {
        auto analysisManager = G4AnalysisManager::Instance();
        auto status = track->GetTrackStatus();
        G4String escaped = "false";
        G4double x = 0, y = 0, z = 0;
        if (status == fStopAndKill || status == fKillTrackAndSecondaries)
        {
            if (process && process->GetProcessName() == "Transportation")
            {
                G4ThreeVector pos = track->GetPosition();
                stepLog << "[Event " << eventID << "] "
                        << "Escaped neutron at "
                        << pos << " [mm]" << std::endl;

                escaped = "true";
                x = pos.x() / mm;
                y = pos.y() / mm;
                z = pos.z() / mm;

                // CSV’ye kaydet
                analysisManager->FillNtupleIColumn(4, 0, eventID);
                analysisManager->FillNtupleSColumn(4, 4, escaped);
                analysisManager->FillNtupleDColumn(4, 5, x);
                analysisManager->FillNtupleDColumn(4, 6, y);
                analysisManager->FillNtupleDColumn(4, 7, z);
                analysisManager->AddNtupleRow(4);
            }
        }
    }
}
