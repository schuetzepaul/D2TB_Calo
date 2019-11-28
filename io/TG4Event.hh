#ifndef TG4Event_hh
#define TG4Event_hh 1

#include "TG4PhotonDetHit.hh"

#include <TObject.h>

class TG4Event : public TObject {
public:
    TG4Event(void) {}
    virtual ~TG4Event();

    /// The run number
    int RunId;

    /// The event number
    int EventId;

    ///The number of scintillation photon in the event
    int NScint;

    /// A map of sensitive detector names and vectors of photon detector hits.  The
    /// map is keyed using the sensitive volume name.
    TG4HitDetectors Detectors;

    ClassDef(TG4Event,1)
};
#endif
