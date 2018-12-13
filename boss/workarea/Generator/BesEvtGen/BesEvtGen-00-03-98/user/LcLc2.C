//---- read out the momentum for each particle, then using them to calculate the amplitude squared, for example:
/*********** using the following decay cards ****************
where spin for Lambda_c and anti-Lambda_c is assumed to be 3/2 for spin-test
Particle vpho 4.6 0

Decay vpho
1 Lambda_c+ anti-Lambda_c- PHSP;
Enddecay

Decay Lambda_c+
1 Lambda0 pi+  PHSP;
Enddecay

Decay anti-Lambda_c-
1 anti-Lambda0 pi-  PHSP;
Enddecay

Decay Lambda0
1 p+ pi- PHSP;
Enddecay

Decay anti-Lambda0
1 anti-p- pi+ PHSP;
Enddecay

End
*////////
//----------------------------------
  EvtVector4R vpho,lambdac,lambdacbar,lambda,lambdabar,proton,antiproton;
  vpho      = part->getP4();              //vpho momentum  
  lambdac   = part->getDaug(0)->getP4();  //lambda_c+ momentum 
  lambdacbar= part->getDaug(1)->getP4();  //anti-Lambda_c-  momentum
  lambda    = part->getDaug(0)->getDaug(0)->getP4();  //Lambda momentum
  lambdabar = part->getDaug(1)->getDaug(0)->getP4();  //anti-Lambda momentum

  EvtHelSys angles0(vpho,lambdac);
  double theta0 = angles0.getHelAng(1);
  double phi0   = angles0.getHelAng(2);
 
  EvtHelSys angles1(lambdac,lambda);
  double theta1 = angles1.getHelAng(1);
  double phi1   = angles1.getHelAng(2);

  EvtHelSys angles2(lambdacbar,lambdabar);
  double theta2 = angles2.getHelAng(1);
  double phi2   = angles2.getHelAng(2);
  
  amps = myamps_(&theta0,&theta1,&theta2);
