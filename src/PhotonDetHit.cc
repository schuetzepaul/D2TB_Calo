#include "PhotonDetHit.hh"

G4ThreadLocal G4Allocator<PhotonDetHit>* PhotonDetHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonDetHit::PhotonDetHit()
{
    fArrivalTime = 0.;
    fPosArrive   = G4ThreeVector(0., 0., 0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonDetHit::PhotonDetHit(G4ThreeVector pArrive, G4double pTime)
{
    fPosArrive   = pArrive;
    fArrivalTime = pTime;
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
    fPosArrive   = right.fPosArrive;
    fArrivalTime = right.fArrivalTime;

    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool PhotonDetHit::operator==(const PhotonDetHit& right) const
{
    return (fPosArrive   == right.fPosArrive  && fArrivalTime == right.fArrivalTime);
}
