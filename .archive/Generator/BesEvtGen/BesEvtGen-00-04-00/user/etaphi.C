/******************************************
Decay cards

Decay vpho
1 gamma gamma* CONEXC 23;
Enddecay

Decay gamma*
1 eta phi PHSP;
Enddecay

Decay phi
1 K+ K- PHSP;
Enddecay

Decay eta
1 gamma gamma PHSP;
Enddecay


End
*******************************************/
EvtVector4R vgamst,veta,vphi,vkaon;
vgamst      = part->getDaug(1)->getP4();              //gamma* momentum  
vphi        = part->getDaug(1)->getDaug(1)->getP4();  //phi momentum 
veta        = part->getDaug(1)->getDaug(0)->getP4();  //eta momentum
vkaon       = part->getDaug(1)->getDaug(1)->getDaug(0)->getP4();  // K+ 

double mphi= vphi.mass();
double meta= veta.mass();
if(mphi<0.01 || meta<0.01) return sqrt(-1.0);//To tag the bad gamma*->eta phi with zero mass of eta and phi

EvtHelSys angles0(vgamst,vphi);
double theta0 = angles0.getHelAng(1);
double phi0   = angles0.getHelAng(2);

EvtHelSys angles1(vphi,vkaon);
double theta1 = angles1.getHelAng(1);
double phi1   = angles1.getHelAng(2);

//std::cout<<vphi<<veta<<std::endl;

amps=0;
double H[3]={-1,0,1};
EvtComplex camps=0;
/*
for(int M=-1;M<=1;M+=2){
  for(int lam=-1;lam<=1;lam++){
    for(int lamp=-1;lamp<=1;lamp++){
      camps += Djmn(1,M,lam,phi0,theta0,0)*conj(Djmn(1,M,lamp,phi0,theta0,0))*H[lam+1]*H[lamp+1]
      *Djmn(1,lam,0,phi1,theta1,0)*conj(Djmn(1,lamp,0,phi1,theta1,0)); 
    }
  }
 }
amps = real(camps);
*/
amps = (6 + 2*cos(2*(phi0 - phi1)) + 2*cos(2*theta0) - cos(2*(phi0 - phi1 + theta0)) - cos(2*(-phi0 + phi1 + theta0)))*
	sin(theta1)*sin(theta1);

//std::cout<<camps<<" "<<H[0]<<" "<<H[1]<<" "<<H[2]<<std::endl;
