/******************************************
Decay cards

Decay vpho
1 Lambda_c+ anti-Lambda_c- PHSP;
Enddecay

Decay Lambda_c+
1 Lambda0 e+ nu_e PHSP;
Enddecay

Decay Lambda0
1 p+ pi- PHSP;
Enddecay


End
*******************************************/
EvtVector4R vpho,lambdac,lambdacbar,lambda,lambdabar,proton,antiproton,positron,enu,wp4,vpim;
vpho      = part->getP4();              //vpho momentum  
lambdac   = part->getDaug(0)->getP4();  //lambda_c+ momentum 
lambda    = part->getDaug(0)->getDaug(0)->getP4();  //Lambda
positron  = part->getDaug(0)->getDaug(1)->getP4();  //e+ 
enu       = part->getDaug(0)->getDaug(2)->getP4();  //nu_e
proton    = part->getDaug(0)->getDaug(0)->getDaug(0)->getP4();  //proton
vpim      = part->getDaug(0)->getDaug(0)->getDaug(1)->getP4();  //pronton
wp4       = enu + positron;


EvtVector4R bwp4,bps,bnu;
bwp4=-1*wp4; bwp4.set(0,wp4.get(0));
bnu=boostTo(enu,     bwp4);
bps=boostTo(positron,bwp4);
EvtHelSys angles1(wp4,bps);
//double thetaw1= angles1.getHelAng(1);
//std::cout<<bnu<<bps<<lambdac<<std::endl;

double cosw=bps.dot(lambda)/bps.d3mag()/lambda.d3mag(); //thetaw is defined as the angle between the positron and Lambda
double thetaw=acos(cosw);

EvtHelSys angles0(lambda,proton);
double thetaL = angles0.getHelAng(1);
double phiL   = angles0.getHelAng(2);
double mw = wp4.mass();

//debuging
//double cosL=lambda.dot(proton)/lambda.d3mag()/proton.d3mag();
//std::cout<<acos(cosL)<<" "<<thetaL<<std::endl; 
//std::cout<<bps<<bnu<<std::endl;
//std::cout<<proton<<vpim<<std::endl;
//std::cout<<thetaw<<" "<<thetaL<<" "<<mw<<std::endl;

EvtId LamId=part->getDaug(0)->getId();
std::vector<double> vpar,vexp;
vexp.push_back(thetaw);
vexp.push_back(thetaL);
vexp.push_back(mw);

double fratio=-0.25;
double f1=1;
double f2=f1*fratio;
vpar.push_back(f1);
vpar.push_back(f2);


//debugging
//std::cout<<vexp[0]<<" "<<vexp[1]<<" "<<vexp[2]<<std::endl;
//std::cout<<vexp[0]<<" "<<vexp[1]<<" "<<vexp[2]<<std::endl;
//std::cout<<"The simulation is :"<<EvtPDL::name(LamId)<<std::endl;

double aL=0.642;
if(LamId==EvtPDL::getId("Lambda_c+")){
  amps = ampsLenu(vexp,vpar); 
 }else if(LamId==EvtPDL::getId("anti-Lambda_c-")){
  aL = -aL;
  amps = ampsLbenu(vexp,vpar);
 }else{
  std::cout<<"Your particle is not Lambda_c+ or anti-Lambda_c-"<<std::endl;
 }

//std::cout<<amps<<" "<<vpar[0]<<" "<<vpar[1]<<std::endl;
