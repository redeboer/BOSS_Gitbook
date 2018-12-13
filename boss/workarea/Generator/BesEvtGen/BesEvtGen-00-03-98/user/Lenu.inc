double  EvtDecay::ampsLenu(std::vector<double> Vexp,std::vector<double> vpars){
//Lambda_c->Lambda e nu 
  //vpars =<f1,f2>
  //Vexp =<thetaW,thetaL,qw>
  //ref: PRL 75, p624, Eq (1)
  double theAmps=0; 
  double thetaw=Vexp[0]; //theta_W
  double thetaL=Vexp[1]; //theta_Lambda
  double qw    =Vexp[2]; // q: the mass of the virtual W
  double aL    =0.642;
  double cosw=cos(thetaw);
  double cosL=cos(thetaL);
  double sinw=sin(thetaw);
  double mLam=1.115683;
  double mLamc=2.28646;
  double mLamc2 = mLamc*mLamc;
  double mLam2 = mLam*mLam;
  double m12p2  = (mLam+qw)*(mLam+qw);
  double m12m2  = (mLam-qw)*(mLam-qw);
  double PL  = sqrt((mLamc2-m12p2)*(mLamc2-m12m2))/2/mLamc;
  theAmps = qw*qw*PL/2/mLamc2;
  double H1=HV(0.5,1.0,Vexp,vpars)+HA(0.5,1.0,Vexp,vpars);
  double H2=HV(0.5,1.0,Vexp,vpars)-HA(0.5,1.0,Vexp,vpars);
  double H3=HV(0.5,0.0,Vexp,vpars)+HA(0.5,0.0,Vexp,vpars);
  double H4=HV(0.5,0.0,Vexp,vpars)-HA(0.5,0.0,Vexp,vpars);
  double term0=3./8*(1-cosw)*(1-cosw)*(1+aL*cosL)*H1*H1;
  double term1=3./8.*(1+cosw)*(1+cosw)*(1-aL*cosL)*H2*H2;
  double term2=3./4.*sinw*sinw*(1+aL*cosL)*H3*H3;
  double term3=3./4.*sinw*sinw*(1-aL*cosL)*H4*H4;
  double terms=term0+term1+term2+term3;
  theAmps *= terms;
  return theAmps;
}

double  EvtDecay::ampsLbenu(std::vector<double> Vexp,std::vector<double> vpars){
//Lambda_c->Lambda e nu 
  double theAmps=0; 
  double thetaw=Vexp[0]; //theta_W
  double thetaL=Vexp[1]; //theta_Lambda
  double qw    =Vexp[2]; // q: the mass of the virtual W
  double aL    =-0.642;
  double cosw=cos(thetaw);
  double cosL=cos(thetaL);
  double sinw=sin(thetaw);
  double mLam=1.115683;
  double mLamc=2.28646;
  double mLamc2 = mLamc*mLamc;
  double mLam2 = mLam*mLam;
  double m12p2  = (mLam+qw)*(mLam+qw);
  double m12m2  = (mLam-qw)*(mLam-qw);
  double PL  = sqrt((mLamc2-m12p2)*(mLamc2-m12m2))/2/mLamc;
  theAmps = qw*qw*PL/2/mLamc2;
  double H1=HV(0.5,1.0,Vexp,vpars)+HA(0.5,1.0,Vexp,vpars);
  double H2=HV(0.5,1.0,Vexp,vpars)-HA(0.5,1.0,Vexp,vpars);
  double H3=HV(0.5,0.0,Vexp,vpars)+HA(0.5,0.0,Vexp,vpars);
  double H4=HV(0.5,0.0,Vexp,vpars)-HA(0.5,0.0,Vexp,vpars);
  double term0=3./8*(1-cosw)*(1-cosw)*(1+aL*cosL)*H1*H1;
  double term1=3./8.*(1+cosw)*(1+cosw)*(1-aL*cosL)*H2*H2;
  double term2=3./4.*sinw*sinw*(1+aL*cosL)*H3*H3;
  double term3=3./4.*sinw*sinw*(1-aL*cosL)*H4*H4;
  double terms=term0+term1+term2+term3;
  theAmps *= terms;
  return theAmps;
}		    

double EvtDecay::HV(double i,double j,std::vector<double> Vexp, std::vector<double> vpars){
    //vpars =<f1,f2>
    //Vexp =<thetaW,thetaL,qw>
    //ref: PRL 75, p624, Eq (1)
    double myhv=0;
    double thetaw=Vexp[0]; //theta_W
    double thetaL=Vexp[1]; //theta_Lambda
    double qw    =Vexp[2]; // q: the mass of the virtual W
    double mLam=1.115683;
    double mLamc=2.28646;
    double Qp=(mLamc+mLam)*(mLamc+mLam)-qw*qw; 
    double Qm=(mLamc-mLam)*(mLamc-mLam)-qw*qw;
    double FV1 = vpars[0]+mLam/mLamc*vpars[1];
    double FV2 = 1./mLamc*vpars[1];
    double FA1 = -FV1;
    double FA2 = -FV2;
    if(Qp<0 || Qm<0){ std::cout<<"Qp,Qm"<<Qp<<" "<<Qm<<std::endl;abort();}
    if(i==0.5 && j==0 ){
      myhv = sqrt(Qm)/qw*((mLam+mLamc)*FV1 - qw*qw*FV2 );
    }else if(i==0.5 && j==1){
      myhv = sqrt(2*Qm)*(-FV1+(mLamc+mLam)*FV2 );
    }else{std::cout<<"AngFitFCN::HV::Bad helicity"<<std::endl;abort();}
    //std::cout<<"HV: "<<myhv<<std::endl;
    return myhv;
  }

double EvtDecay::HA(double i,double j,std::vector<double> Vexp, std::vector<double> vpars){
    //vpars =<f1,f2>
    //Vexp =<thetaW,thetaL,qw>
    //ref: PRL 75, p624, Eq (1)
    double myhv=0;
    double thetaw=Vexp[0]; //theta_W
    double thetaL=Vexp[1]; //theta_Lambda
    double qw    =Vexp[2]; // q: the mass of the virtual W
    double mLam=1.115683;
    double mLamc=2.28646;
    double Qp=(mLamc+mLam)*(mLamc+mLam)-qw*qw; 
    double Qm=(mLamc-mLam)*(mLamc-mLam)-qw*qw;
    double FV1 = vpars[0]+mLam/mLamc*vpars[1];
    double FV2 = 1./mLamc*vpars[1];
    double FA1 = -FV1;
    double FA2 = -FV2;
    if(i==0.5 && j==0 ){
      myhv = sqrt(Qp)/qw*((mLamc-mLam)*FA1 + qw*qw*FA2 );
    }else if(i==0.5 && j==1){
      myhv = sqrt(2*Qp)*(-FA1+(mLamc-mLam)*FA2 );
    }else{std::cout<<"AngFitFCN::HA::Bad helicity"<<std::endl;abort();}
    //std::cout<<"HA: "<<myhv<<std::endl;
    return myhv;
  }
