#include "CLHEP/Vector/LorentzVector.h"
#include "string.h"
#include "MdcRecEvent/RecMdcKalTrack.h"
const double mmass[5] = {0.000511, 0.105658, 0.139570,0.493677, 0.938272};
// mmass[5] = { e, mu, pion, Kion, proton}
class m_trkInfo{
  int i;
  double mom; 
  HepLorentzVector trk;
  RecMdcKalTrack *mdcTrk;
 public:
  m_trkInfo(int index, double momentum, HepLorentzVector trkp, RecMdcKalTrack *mdcKaltrk){
    i=index;
    mom=momentum;
    trk=trkp;
    mdcTrk=mdcKaltrk;
  }
  int getIndex(){return i;}
  double getMom(){return mom;}
  HepLorentzVector getTrk(){return trk;}
  HepLorentzVector getTrk(std::string m_particle_type);
};

// Define a function which can calculate the momentum difference between two Lorentz vectors.
inline double cal_via_mom(HepLorentzVector m_mom_a, HepLorentzVector m_mom_b){
  return( m_mom_b.vect().mag()*1000-m_mom_a.vect().mag());
} // where a factor 1000 arise from the differences between the unit of MC truth and reconstruction

inline HepLorentzVector m_trkInfo::getTrk(std::string m_part_type){
  if(m_part_type=="e"){
    mdcTrk->setPidType(RecMdcKalTrack::electron);
    trk.setPx(mdcTrk->px());
    trk.setPy(mdcTrk->py());
    trk.setPz(mdcTrk->pz());
    double p3(mdcTrk->p());
    trk.setE( sqrt(p3*p3 + mmass[0]*mmass[0]) );
    return trk;
  }
  if(m_part_type=="muon"){
    mdcTrk->setPidType(RecMdcKalTrack::muon);
    trk.setPx(mdcTrk->px());
    trk.setPy(mdcTrk->py());
    trk.setPz(mdcTrk->pz());
    double p3(mdcTrk->p());
    trk.setE( sqrt(p3*p3 + mmass[1]*mmass[1]) );
    return trk;
  }
  if(m_part_type=="pion"){
    mdcTrk->setPidType(RecMdcKalTrack::pion);
    trk.setPx(mdcTrk->px());
    trk.setPy(mdcTrk->py());
    trk.setPz(mdcTrk->pz());
    double p3(mdcTrk->p());
    trk.setE( sqrt(p3*p3 + mmass[2]*mmass[2]) );
    return trk;
  }
  if(m_part_type=="kion"){
    mdcTrk->setPidType(RecMdcKalTrack::kaon);
    trk.setPx(mdcTrk->px());
    trk.setPy(mdcTrk->py());
    trk.setPz(mdcTrk->pz());
    double p3(mdcTrk->p());
    trk.setE( sqrt(p3*p3 + mmass[3]*mmass[3]) );
    return trk;
  }
  if(m_part_type=="proton"){
    mdcTrk->setPidType(RecMdcKalTrack::proton);
    trk.setPx(mdcTrk->px());
    trk.setPy(mdcTrk->py());
    trk.setPz(mdcTrk->pz());
    double p3(mdcTrk->p());
    trk.setE( sqrt(p3*p3 + mmass[4]*mmass[4]) );
    return trk;
  }
  else return trk;
}
