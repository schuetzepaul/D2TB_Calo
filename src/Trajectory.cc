#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4AttDefStore.hh"

#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"

#include "Trajectory.hh"
#include "TrajectoryPoint.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"

#include "G4Polyline.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4Polymarker.hh"

//#define G4ATTDEBUG
#ifdef G4ATTDEBUG
#include "G4AttCheck.hh"
#endif

G4ThreadLocal G4Allocator<Trajectory>* TrajectoryAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Trajectory::Trajectory()
: fpPointsContainer(0),
fTrackID(0),
fParentID(0),
fPDGCharge(0.0),
fPDGEncoding(0),
fParticleName(""),
fInitialMomentum(G4ThreeVector())
{
    fParticleDefinition = nullptr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Trajectory::Trajectory(const G4Track* aTrack)
{
    fParticleDefinition = aTrack->GetDefinition();
    fParticleName = fParticleDefinition->GetParticleName();
    fPDGCharge = fParticleDefinition->GetPDGCharge();
    fPDGEncoding = fParticleDefinition->GetPDGEncoding();
    fTrackID = aTrack->GetTrackID();
    fParentID = aTrack->GetParentID();
    fInitialMomentum = aTrack->GetMomentum();
    fpPointsContainer = new TrajectoryPointContainer();
    // Following is for the first trajectory point
    fpPointsContainer->push_back(new TrajectoryPoint(aTrack));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Trajectory::Trajectory(Trajectory & right) : G4VTrajectory()
{
    fParticleDefinition=right.fParticleDefinition;
    fParticleName = right.fParticleName;
    fPDGCharge = right.fPDGCharge;
    fPDGEncoding = right.fPDGEncoding;
    fTrackID = right.fTrackID;
    fParentID = right.fParentID;
    fInitialMomentum = right.fInitialMomentum;
    fpPointsContainer = new TrajectoryPointContainer();

    for(size_t i=0;i<right.fpPointsContainer->size();++i) {
        TrajectoryPoint* rightPoint
        = (TrajectoryPoint*)((*(right.fpPointsContainer))[i]);
        fpPointsContainer->push_back(new TrajectoryPoint(*rightPoint));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Trajectory::~Trajectory()
{
    for(size_t i=0;i<fpPointsContainer->size();++i){
        delete  (*fpPointsContainer)[i];
    }
    fpPointsContainer->clear();

    delete fpPointsContainer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Trajectory::ShowTrajectory(std::ostream& os) const
{
    // Invoke the default implementation in G4VTrajectory...
    G4VTrajectory::ShowTrajectory(os);
    // ... or override with your own code here.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Trajectory::AppendStep(const G4Step* aStep)
{
    fpPointsContainer->push_back(new TrajectoryPoint(aStep));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ParticleDefinition* Trajectory::GetParticleDefinition()
{
    return (G4ParticleTable::GetParticleTable()->FindParticle(fParticleName));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Trajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
    if(!secondTrajectory) return;

    Trajectory* second = (Trajectory*)secondTrajectory;
    G4int ent = second->GetPointEntries();
    // initial point of the second trajectory should not be merged
    for(G4int i=1; i<ent; ++i) {
        fpPointsContainer->push_back((*(second->fpPointsContainer))[i]);
    }
    delete (*second->fpPointsContainer)[0];
    second->fpPointsContainer->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* Trajectory::GetAttDefs() const
{
    G4bool isNew;
    std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("Trajectory",isNew);

    if (isNew) {

        G4String ID("ID");
        (*store)[ID] = G4AttDef(ID,"Track ID","Bookkeeping","","G4int");

        G4String PID("PID");
        (*store)[PID] = G4AttDef(PID,"Parent ID","Bookkeeping","","G4int");

        G4String PN("PN");
        (*store)[PN] = G4AttDef(PN,"Particle Name","Physics","","G4String");

        G4String Ch("Ch");
        (*store)[Ch] = G4AttDef(Ch,"Charge","Physics","e+","G4double");

        G4String PDG("PDG");
        (*store)[PDG] = G4AttDef(PDG,"PDG Encoding","Physics","","G4int");

        G4String IMom("IMom");
        (*store)[IMom] = G4AttDef(IMom,
        "Momentum of track at start of trajectory",
        "Physics","G4BestUnit","G4ThreeVector");

        G4String IMag("IMag");
        (*store)[IMag] = G4AttDef(IMag,
        "Magnitude of momentum of track at start of trajectory",
        "Physics","G4BestUnit","G4double");

        G4String NTP("NTP");
        (*store)[NTP] = G4AttDef(NTP,"No. of points","Bookkeeping","","G4int");

    }
    return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* Trajectory::CreateAttValues() const
{
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

    values->push_back
    (G4AttValue("ID",G4UIcommand::ConvertToString(fTrackID),""));

    values->push_back
    (G4AttValue("PID",G4UIcommand::ConvertToString(fParentID),""));

    values->push_back(G4AttValue("PN",fParticleName,""));

    values->push_back
    (G4AttValue("Ch",G4UIcommand::ConvertToString(fPDGCharge),""));

    values->push_back
    (G4AttValue("PDG",G4UIcommand::ConvertToString(fPDGEncoding),""));

    values->push_back
    (G4AttValue("IMom",G4BestUnit(fInitialMomentum,"Energy"),""));

    values->push_back
    (G4AttValue("IMag",G4BestUnit(fInitialMomentum.mag(),"Energy"),""));

    values->push_back
    (G4AttValue("NTP",G4UIcommand::ConvertToString(GetPointEntries()),""));

    #ifdef G4ATTDEBUG
    G4cout << G4AttCheck(values,GetAttDefs());
    #endif
    return values;
}
