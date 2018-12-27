/******************************************
Decay cards

Particle vpho 2.23 0

Decay vpho
1 gamma gamma* ConExc 52;
Enddecay

Decay gamma*
1 omega pi0 PHSP;
Enddecay

Decay omega
1 pi+ pi- pi0 PHSP;
Enddecay

Decay pi0
1 gamma gamma PHSP;
Enddecay


End
*******************************************/
EvtVector4R vpart,vgamst,vpi0,vomega,vpion,vpion2,vpion3,Zaxis;
vgamst      = part->getDaug(1)->getP4();              //gamma* momentum  
vomega      = part->getDaug(1)->getDaug(0)->getP4();  //omega momentum 
vpi0        = part->getDaug(1)->getDaug(1)->getP4();  //pi0 momentum
vpion       = part->getDaug(1)->getDaug(0)->getDaug(0)->getP4();  // pi+ 
vpion2      = part->getDaug(1)->getDaug(0)->getDaug(1)->getP4();  // pi-
vpion3      = part->getDaug(1)->getDaug(0)->getDaug(2)->getP4();  // pi0
Zaxis       = vpion2.cross(vpion3);
vpart       = part->getP4();
double mw= vomega.mass();
double mpi0= vpi0.mass();
double mpart=vpart.mass();
double mvgst=vgamst.mass();
if((mpart-mvgst)<0.001 ){
  vgamst.set(0,mpart);
  vgamst.set(1,0);
  vgamst.set(2,0);
  vgamst.set(3,0);
  }

EvtHelSys angles0(vgamst,vomega);
double theta = angles0.getHelAng(1);
double phi   = angles0.getHelAng(2);

EvtEulerAngles EA(vpion,Zaxis);
double alpha=EA.getAlpha();
double beta =EA.getBeta();
double gamma=EA.getGamma();


amps=0;
EvtComplex camps=0;

amps =((6 - 2*cos(2*(alpha + phi)) + cos(2*(alpha + phi - theta)) + 
	2*cos(2*theta) + cos(2*(alpha + phi + theta)))*sin(beta)*sin(beta))/8; 

//std::cout<<"amps = "<<amps<<std::endl;
