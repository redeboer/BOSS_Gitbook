#include "DTagAlg/utility.h"

HepLorentzVector utility::getp4(RecMdcKalTrack* mdcKalTrack, int pid){
  
  HepVector zhelix;
  double mass=0;

  if(pid==0){
    zhelix=mdcKalTrack->getZHelixE();
    mass=0.000511;
  }
  else if(pid==1){
    zhelix=mdcKalTrack->getZHelixMu();
    mass= 0.105658;
  }
  else if(pid==2){
    zhelix=mdcKalTrack->getZHelix();
    mass=0.139570;
  }
  else if(pid==3){
    zhelix=mdcKalTrack->getZHelixK();
    mass= 0.493677;
  }
  else{
    zhelix=mdcKalTrack->getZHelixP();
    mass= 0.938272;
  }

  double dr(0),phi0(0),kappa(0),dz(0),tanl(0);
  dr=zhelix[0];
  phi0=zhelix[1];
  kappa=zhelix[2];
  dz=zhelix[3];
  tanl=zhelix[4];

  int charge=0;
  if (kappa > 0.0000000001)
    charge = 1;
  else if (kappa < -0.0000000001)
    charge = -1;

  double pxy=0;
  if(kappa!=0) pxy = 1.0/fabs(kappa);

  double px = pxy * (-sin(phi0));
  double py = pxy * cos(phi0);
  double pz = pxy * tanl;

  double e  = sqrt( pxy*pxy + pz*pz + mass*mass );

  return HepLorentzVector(px, py, pz, e);


}


HepLorentzVector utility::vfit(string channel, vector<int> kaonid, vector<int> pionid, HepPoint3D vx, EvtRecTrackIterator charged_begin){
  //use charged tracks only, except pions from Ks

  HepLorentzVector pchange(0,0,0,0);

  int nvalid= kaonid.size()+pionid.size();
  if(nvalid<=1)
    return pchange;

  HepSymMatrix Evx(3, 0);
  double bx = 1E+6; Evx[0][0] = bx*bx;
  double by = 1E+6; Evx[1][1] = by*by;
  double bz = 1E+6; Evx[2][2] = bz*bz;
  VertexParameter vxpar; vxpar.setVx(vx); vxpar.setEvx(Evx);
  double xmass[5] = {0.000511, 0.105658, 0.139570,0.493677, 0.938272};


  VertexFit* vtxfit = VertexFit::instance();
  vtxfit->init();


  HepLorentzVector pold(0,0,0,0);

  for(int i=0; i<kaonid.size();++i){
    EvtRecTrackIterator itTrk=charged_begin + kaonid[i];
    RecMdcKalTrack *mdcKalTrk = (*itTrk)->mdcKalTrack();
    pold+=getp4(mdcKalTrk, 3);
    WTrackParameter wtrk(xmass[3],mdcKalTrk->getZHelixK(),mdcKalTrk->getZErrorK() );
    vtxfit->AddTrack(i, wtrk);
  }

  for(int i=0; i<pionid.size();++i){
    EvtRecTrackIterator itTrk=charged_begin + pionid[i];
    RecMdcKalTrack *mdcKalTrk = (*itTrk)->mdcKalTrack();
    pold+=getp4(mdcKalTrk, 2);
    WTrackParameter wtrk(xmass[2],mdcKalTrk->getZHelix(),mdcKalTrk->getZError() );
    vtxfit->AddTrack(kaonid.size()+i, wtrk);
  }

  vector<int> trkIdxCol;
  trkIdxCol.clear();
  for (int i = 0; i < nvalid; i++)
    trkIdxCol.push_back(i);
  vtxfit->AddVertex(0, vxpar, trkIdxCol);
  if(!vtxfit->Fit(0))
    return pchange;

  vtxfit->Swim(0);

  HepLorentzVector pnew(0,0,0,0);

  for(int i=0; i<nvalid;++i){
    WTrackParameter wtrk = vtxfit->wtrk(i);
    HepVector trk_val = HepVector(7,0);
    trk_val = wtrk.w();
    HepLorentzVector P_trk(trk_val[0],trk_val[1],trk_val[2],trk_val[3]);
    pnew+=P_trk;
  }

  return (pnew-pold);

}

vector<double> utility::SecondaryVFit(EvtRecVeeVertex* ks, IVertexDbSvc* vtxsvc){

  // by default: chi2 = -999, length = -999, error = 999
  double vfitchi2 = -999;
  double vfitlength = -999;
  double vfiterror = 999;

  vector<double> results;
  results.push_back(vfitchi2);
  results.push_back(vfitlength);
  results.push_back(vfiterror);

  // --------------------------------------------------
  // Do a secondary vertex fit and check the flight significance
  // --------------------------------------------------

  EvtRecTrack* veeTrack1 = ks->pairDaughters().first;
  RecMdcKalTrack* veeKalTrack1 = veeTrack1->mdcKalTrack();
  WTrackParameter veeInitialWTrack1 = WTrackParameter(0.13957018, veeKalTrack1->getZHelix(), veeKalTrack1->getZError());

  EvtRecTrack* veeTrack2 = ks->pairDaughters().second;
  RecMdcKalTrack* veeKalTrack2 = veeTrack2->mdcKalTrack();
  WTrackParameter veeInitialWTrack2 = WTrackParameter(0.13957018, veeKalTrack2->getZHelix(), veeKalTrack2->getZError());

  VertexParameter wideVertex;
  HepPoint3D vWideVertex(0., 0., 0.);
  HepSymMatrix evWideVertex(3, 0);

  evWideVertex[0][0] = 1.0e12;
  evWideVertex[1][1] = 1.0e12;
  evWideVertex[2][2] = 1.0e12;

  wideVertex.setVx(vWideVertex);
  wideVertex.setEvx(evWideVertex);

  // First, perform a vertex fit
  VertexFit* vtxfit = VertexFit::instance();
  vtxfit->init();

  // add the daughters
  vtxfit->AddTrack(0,veeInitialWTrack1);
  vtxfit->AddTrack(1,veeInitialWTrack2);
  vtxfit->AddVertex(0,wideVertex,0,1);

  // do the fit
  vtxfit->Fit(0);
  vtxfit->Swim(0);
  vtxfit->BuildVirtualParticle(0);

  // Now perform the secondary vertex fit
  SecondVertexFit* svtxfit = SecondVertexFit::instance();
  svtxfit->init();

  // add the primary vertex
  VertexParameter beamSpot;
  HepPoint3D vBeamSpot(0., 0., 0.);
  HepSymMatrix evBeamSpot(3, 0);

  if(vtxsvc->isVertexValid()){
    double* dbv = vtxsvc->PrimaryVertex();
    double*  vv = vtxsvc->SigmaPrimaryVertex();
    for (unsigned int ivx = 0; ivx < 3; ivx++){
      vBeamSpot[ivx] = dbv[ivx];
      evBeamSpot[ivx][ivx] = vv[ivx] * vv[ivx];
    }
  }
  else{
    cout << "KSSELECTOR ERROR:  Could not find a vertex from VertexDbSvc" << endl;
    return results;
  }

  beamSpot.setVx(vBeamSpot);
  beamSpot.setEvx(evBeamSpot);

  VertexParameter primaryVertex(beamSpot);
  svtxfit->setPrimaryVertex(primaryVertex);

  // add the secondary vertex
  svtxfit->setVpar(vtxfit->vpar(0));

  // add the Ks or lambda
  svtxfit->AddTrack(0,vtxfit->wVirtualTrack(0));

  // do the second vertex fit
  // if the fit fails, the default values will not be changed
  if( !svtxfit->Fit() ) return results;

  // save the new ks parameters
  vfitlength = svtxfit->decayLength();
  vfiterror = svtxfit->decayLengthError();
  vfitchi2 = svtxfit->chisq();

  results.clear();
  results.push_back(vfitchi2);
  results.push_back(vfitlength);
  results.push_back(vfiterror);

  return results;
}
