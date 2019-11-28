#ifndef TG4PhotonDetHit_hh
#define TG4PhotonDetHit_hh 1

#include <TVector3.h>
#include <TObject.h>

#include <map>
#include <vector>

class PersistencyManager;
class TG4PhotonDetHit;

typedef std::vector<TG4PhotonDetHit> TG4PhotonDetHitContainer;
typedef std::map<std::string,TG4PhotonDetHitContainer> TG4HitDetectors;

class TG4PhotonDetHit : public TObject {
    friend class PersistencyManager;
public:
    TG4PhotonDetHit()
    : fArrivalTime(0), fCrystalNo(0), fSiPMNo(0),
    fPosExit(0, 0, 0), fPosArrive(0, 0, 0), fPosArriveLocal(0, 0, 0) {}

    virtual ~TG4PhotonDetHit();

    /// Arrival time of the photon on the SiPM
    float GetArrivalTime() const {return fArrivalTime;}

    /// The number of the crystal volume
    int GetCrystalNumber() const {return fCrystalNo;}

    /// The number of the SiPM volume inside the crystal
    int GetSiPMNumber() const {return fSiPMNo;}

    /// Exit position of the photon in case it got out of the detector
    const TVector3& GetExitPosition() const {return fPosExit;}

    /// Arrival position of the photon in the global coordinates
    const TVector3& GetArrivePosition() const {return fPosArrive;}

    /// Arrival position of the photon in the local coordinates (in the SiPM coordinates)
    const TVector3& GetArriveLocalPosition() const {return fPosArriveLocal;}

private:

    Float_t fArrivalTime;
    Int_t fCrystalNo;
    Int_t fSiPMNo;
    TVector3 fPosExit;
    TVector3 fPosArrive;
    TVector3 fPosArriveLocal;

    ClassDef(TG4PhotonDetHit, 1);
};
#endif
