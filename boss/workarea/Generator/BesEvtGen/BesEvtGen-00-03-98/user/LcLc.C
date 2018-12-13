//---- read out the momentum for each particle, then using them to calculate the amplitude squared, for example:
/*********** using the following decay cards ****************
Particle vpho 4.6 0
Decay vpho
1 Lambda_c+ anti-Lambda_c- PHSP;
Enddecay
 
Decay Lambda_c+
1 Lambda0 pi+ PHSP;
Enddecay
 
Decay anti-Lambda_c-
1 anti-Lambda0 pi- PHSP;
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
  proton    = part->getDaug(0)->getDaug(0)->getDaug(0)->getP4();  //proton  
  antiproton= part->getDaug(1)->getDaug(0)->getDaug(0)->getP4();  // anti-proton

  EvtHelSys angles0(vpho,lambdac);
  double theta0 = angles0.getHelAng(1);
  double phi0   = angles0.getHelAng(2);
 
  EvtHelSys angles1(lambdac,lambda);
  double theta1 = angles1.getHelAng(1);
  double phi1   = angles1.getHelAng(2);

  EvtHelSys angles2(lambdacbar,lambdabar);
  double theta2 = angles2.getHelAng(1);
  double phi2   = angles2.getHelAng(2);
  
  EvtHelSys angles3(lambda,proton);
  double theta3 = angles3.getHelAng(1);
  double phi3   = angles3.getHelAng(2);

  EvtHelSys angles4(lambdabar,antiproton);
  double theta4 = angles4.getHelAng(1);
  double phi4   = angles4.getHelAng(2);
  
  double a0=-0.34;
  double alc = -0.91; // for Lambda_c+
  double alcb=  0.91; // for anti-Lambda_c-
  double al  =  0.642; //Lambda0
  double alb = -0.642; //anti-Lambda0

   amps=cos(theta1)*cos(theta2)*cos(theta3)*cos(theta4)*pow(sin(theta0),2)
    *al*alb-cos(theta1)*cos(theta2)*cos(theta3)*cos(theta4)*a0*
    al*alb-cos(theta1)*cos(theta2)*cos(theta3)*cos(theta4)*al*alb+cos(theta1)*cos(theta2)*cos(theta3)*pow(sin(theta0),2)*al*alcb-cos(theta1)*cos(theta2)*cos(theta3)*a0*al*alcb-cos(theta1)*cos(theta2)*cos(theta3)*al*alcb+cos(theta1)*cos(theta2)*cos(theta4)*pow(sin(theta0),2)*alb*alc-cos(theta1)*cos(theta2)*cos(theta4)*a0*alb*alc-cos(theta1)*cos(theta2)*cos(theta4)*alb*alc+cos(theta1)*cos(theta2)*pow(sin(theta0),2)*alc*alcb-cos(theta1)*cos(theta2)*a0*alc*alcb-cos(theta1)*cos(theta2)*alc*alcb-cos(theta3)*cos(theta4)*pow(sin(theta0),2)*a0*al*alb*alc*alcb+cos(theta3)*cos(theta4)*a0*al*alb*alc*alcb+cos(theta3)*cos(theta4)*al*alb*alc*alcb-cos(theta3)*pow(sin(theta0),2)*a0*al*alc+cos(theta3)*a0*al*alc+cos(theta3)*al*alc-cos(theta4)*pow(sin(theta0),2)*a0*alb*alcb+cos(theta4)*a0*alb*alcb+cos(theta4)*alb*alcb-pow(sin(theta0),2)*a0+a0+1.0;
  
