#include "PhotonDetHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4Circle.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<PhotonDetHit>* PhotonDetHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonDetHit::PhotonDetHit()
{
    fArrivalTime = 0.;
    fPosArrive   = G4ThreeVector(0., 0., 0.);
    fPosArriveLocal = G4ThreeVector(0., 0., 0.);
    fPosExit     = G4ThreeVector(0., 0., 0.);
    fLogicalVolume = nullptr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonDetHit::PhotonDetHit(G4ThreeVector pExit, G4ThreeVector pArrive, G4ThreeVector pArriveLocal, G4double pTime, G4LogicalVolume* pLogV)
{
    fPosExit     = pExit;
    fPosArrive   = pArrive;
    fPosArriveLocal = pArriveLocal;
    fArrivalTime = pTime;
    fLogicalVolume = pLogV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonDetHit::~PhotonDetHit() { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonDetHit::PhotonDetHit(const PhotonDetHit &right)
: G4VHit()
{
    *this = right;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const PhotonDetHit& PhotonDetHit::operator=(const PhotonDetHit &right)
{
    fPosExit     = right.fPosExit;
    fPosArrive   = right.fPosArrive;
    fPosArriveLocal = right.fPosArriveLocal;
    fArrivalTime = right.fArrivalTime;
    fLogicalVolume = right.fLogicalVolume;

    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool PhotonDetHit::operator==(const PhotonDetHit& right) const
{
    return (fPosExit == right.fPosExit && fPosArrive == right.fPosArrive && fPosArriveLocal == right.fPosArriveLocal && fArrivalTime == right.fArrivalTime && fLogicalVolume == right.fLogicalVolume);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhotonDetHit::Draw()
{
    auto visManager = G4VVisManager::GetConcreteInstance();
    if ( ! visManager ) return;

    G4Point3D p3D = G4Point3D(fPosArrive);
    G4Circle chit(p3D);
    chit.SetScreenDiameter(3.0);
    chit.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    chit.SetVisAttributes(attribs);
    visManager->Draw(chit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* PhotonDetHit::GetAttDefs() const
{
    G4bool isNew;
    auto store = G4AttDefStore::GetInstance("PhotonDetHit",isNew);

    if (isNew) {
        (*store)["HitType"] = G4AttDef("HitType","Hit Type","Physics","","G4String");

        (*store)["PosExit"] = G4AttDef("PosExit","Exit Position","Physics","G4BestUnit","G4ThreeVector");

        (*store)["PosArrive"] = G4AttDef("PosArrive", "Arrive Position on SiPM (global)", "Physics", "G4BestUnit",
        "G4ThreeVector");

        (*store)["PosArriveLocal"] = G4AttDef("PosArriveLocal", "Arrive Position on SiPM (local)", "Physics", "G4BestUnit",
        "G4ThreeVector");

        (*store)["Time"] = G4AttDef("Time", "Time", "Physics","G4BestUnit",
        "G4double");

        (*store)["LVol"] = G4AttDef("LVol","Logical Volume","Physics","","G4String");
    }
    return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* PhotonDetHit::CreateAttValues() const
{
    auto values = new std::vector<G4AttValue>;

    values->push_back(G4AttValue("HitType","PhotonDetHit",""));

    values->push_back(G4AttValue("PosExit",G4BestUnit(fPosExit,"Length"),""));

    values->push_back(G4AttValue("PosArrive",G4BestUnit(fPosArrive,"Length"),""));

    values->push_back(G4AttValue("PosArriveLocal",G4BestUnit(fPosArriveLocal,"Length"),""));

    values->push_back(G4AttValue("Time",G4BestUnit(fArrivalTime,"Time"),""));

    if (fLogicalVolume)
    values->push_back(G4AttValue("LVol",fLogicalVolume->GetName(),""));
    else
    values->push_back(G4AttValue("LVol"," ",""));

    return values;
}
