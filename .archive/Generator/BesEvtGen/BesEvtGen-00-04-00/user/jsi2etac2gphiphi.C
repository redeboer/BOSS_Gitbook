/*********** using the following decay cards ****************
Decay J/psi
1 gamma eta_c  PHSP;
Enddecay
 
Decay eta_c
1 phi phi PHSP;
Enddecay
 
Decay phi
1 K+ K- PHSP;
Enddecay
 
End
*////////
//----------------------------------
    EvtVector4R pjsi,petac,pphi1,pphi2,pkp1,pkp2;
    pjsi = part->getP4();
    petac= part->getDaug(1)->getP4();  //etac momentum 
    pphi1= part->getDaug(1)->getDaug(0)->getP4();  //phi1 momentum
    pphi2= part->getDaug(1)->getDaug(1)->getP4();  //phi2 momentum

    pkp1 = part->getDaug(1)->getDaug(0)->getDaug(0)->getP4();  //K+ from phi1 
    pkp2 = part->getDaug(1)->getDaug(1)->getDaug(0)->getP4();  //K+ from phi2 

    EvtHelSys angles0(pjsi,petac);
    double theta0 = angles0.getHelAng(1);
    double phi0   = angles0.getHelAng(2);
 
    EvtHelSys angles2(pphi1,pkp1);
    double theta2 = angles2.getHelAng(1);
    double phi2   = angles2.getHelAng(2);

    EvtHelSys angles3(pphi2,pkp2);
    double theta3 = angles3.getHelAng(1);
    double phi3   = angles3.getHelAng(2);

    double amps=(3+cos(2*theta0)) * sin(theta2)*sin(theta2) * sin(theta3)*sin(theta3) * sin(phi2+phi3)*sin(phi2+phi3);

