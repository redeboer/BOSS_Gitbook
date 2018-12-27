/*********************************************************************
This is amplitude for ee->Lambda_c(3/2)anti-Lambda_c(3/2)->Lambda anti-lambda
 + pipi -> ppbar 2(pi+pi-)
-----------------------------------------------------------------------
using these decay cards
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

**********************************************************************/
      function myamps(theta0,theta1,theta2)
      implicit none
      real*8 theta0,theta1,theta2,myamps
      real*8 ald,ladb,aldc,aldcb,ans4,ans3,ans2,ans1,ans,pi
      real*8 a11,a13,a1m1,a33,ans5
cc
c     theta0, for ee->Lambda_c anti-Lambdac
c     aldc,theta1,phi1 for Lambda_c->p pi
c     adl, teta2,phi2  for anti-Lambda_c ->pbar pi
cc
      pi=3.1415926
      a11=0.85712
      a13=1
      a1m1=0.353
      a33=0.938
      aldc = -0.91
      ald  = 0.91

      ans4=-3.0*((((2.0*cos(theta2/2.0)**2-sin(theta2/2.0)**2)**2*(
     . ald+1.0)*sin(theta2/2.0)**2-(cos(theta2/2.0)**2-2.0*sin(theta2
     . /2.0)**2)**2*(ald-1.0)*cos(theta2/2.0)**2)*((aldc+1.0)*cos(
     . theta1/2.0)**2-(aldc-1.0)*sin(theta1/2.0)**2)-((2.0*cos(theta2
     . /2.0)**2-sin(theta2/2.0)**2)**2*(ald-1.0)*sin(theta2/2.0)**2-(
     . cos(theta2/2.0)**2-2.0*sin(theta2/2.0)**2)**2*(ald+1.0)*cos(
     . theta2/2.0)**2)*((aldc+1.0)*sin(theta1/2.0)**2-(aldc-1.0)*cos(
     . theta1/2.0)**2))*(cos(theta0/2.0)**4+sin(theta0/2.0)**4)*a13**
     . 2+24.0*(((ald*aldc+1.0)*cos(theta2/2.0)**2-(ald*aldc-1.0)*sin(
     . theta2/2.0)**2)*sin(theta1/2.0)**2+((ald*aldc+1.0)*sin(theta2/
     . 2.0)**2-(ald*aldc-1.0)*cos(theta2/2.0)**2)*cos(theta1/2.0)**2)
     . *cos(theta0/2.0)**2*cos(theta2/2.0)**2*sin(theta0/2.0)**2*sin(
     . theta2/2.0)**2*a33**2)*cos(theta1/2.0)**2*sin(theta1/2.0)**2
      ans3=-((((2.0*cos(theta2/2.0)**2-sin(theta2/2.0)**2)**2*(ald-
     . 1.0)*sin(theta2/2.0)**2-(cos(theta2/2.0)**2-2.0*sin(theta2/2.0
     . )**2)**2*(ald+1.0)*cos(theta2/2.0)**2)*a1m1**2-3.0*((ald+1.0)*
     . sin(theta2/2.0)**2-(ald-1.0)*cos(theta2/2.0)**2)*cos(theta2/
     . 2.0)**2*sin(theta2/2.0)**2*a13**2)*(cos(theta0/2.0)**4+sin(
     . theta0/2.0)**4)-4.0*((2.0*cos(theta2/2.0)**2-sin(theta2/2.0)**
     . 2)**2*(ald+1.0)*sin(theta2/2.0)**2-(cos(theta2/2.0)**2-2.0*sin
     . (theta2/2.0)**2)**2*(ald-1.0)*cos(theta2/2.0)**2)*cos(theta0/
     . 2.0)**2*sin(theta0/2.0)**2*a11**2)*((2.0*cos(theta1/2.0)**2-
     . sin(theta1/2.0)**2)**2*(aldc-1.0)*sin(theta1/2.0)**2-(cos(
     . theta1/2.0)**2-2.0*sin(theta1/2.0)**2)**2*(aldc+1.0)*cos(
     . theta1/2.0)**2)+ans4
      ans2=(4.0*((2.0*cos(theta2/2.0)**2-sin(theta2/2.0)**2)**2*(ald-
     . 1.0)*sin(theta2/2.0)**2-(cos(theta2/2.0)**2-2.0*sin(theta2/2.0
     . )**2)**2*(ald+1.0)*cos(theta2/2.0)**2)*cos(theta0/2.0)**2*sin(
     . theta0/2.0)**2*a11**2-3.0*((ald+1.0)*cos(theta2/2.0)**2-(ald-
     . 1.0)*sin(theta2/2.0)**2)*(cos(theta0/2.0)**4+sin(theta0/2.0)**
     . 4)*cos(theta2/2.0)**2*sin(theta2/2.0)**2*a13**2-((2.0*cos(
     . theta2/2.0)**2-sin(theta2/2.0)**2)**2*(ald+1.0)*sin(theta2/2.0
     . )**2-(cos(theta2/2.0)**2-2.0*sin(theta2/2.0)**2)**2*(ald-1.0)*
     . cos(theta2/2.0)**2)*(cos(theta0/2.0)**4+sin(theta0/2.0)**4)*
     . a1m1**2)*((2.0*cos(theta1/2.0)**2-sin(theta1/2.0)**2)**2*(aldc
     . +1.0)*sin(theta1/2.0)**2-(cos(theta1/2.0)**2-2.0*sin(theta1/
     . 2.0)**2)**2*(aldc-1.0)*cos(theta1/2.0)**2)+ans3
      ans5=pi**3
      ans1=2.0*ans2*ans5
      myamps=abs(-ans1)
      end

