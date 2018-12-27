//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2006      Ping Rong-Gang, Pang Cai-Ying@IHEP
//
// Module: EvtEvtPloInt.cc
//
// Description: Routine to deal with polynomial interpolation 
//
// Modification history:
//
//    Ping R.-G.    December, 2010   Module created
//
//------------------------------------------------------------------------

#include "EvtGenBase/EvtPolInt.hh"

void EvtPolInt::polynomial(){
  double y,dy;
  int i,m,ns=0;
  double den,dif,dift,ho,hp,w;

  int n=vx.size();
  vector <double> c(n),d(n);
  dif = fabs(xx-vx[0]);
  for (i=0;i<n;i++){
    if ((dift = fabs(xx-vx[i]))<dif){
      ns=i;
      dif = dift;
    }
	c[i] = vy[i];
	d[i] = vy[i];
	}
    y = vy[ns--];
    for (m=1;m<n;m++){
      for (i=0;i<n-m;i++){
	ho = vx[i] -xx;
	hp = vx[i+m] -xx;
	w  = c[i+1] - d[i];
	if(( den = ho-hp) == 0.0) std::cout<<"Error in routine polint"<<std::endl;
	den = w/den;
	d[i] = hp*den;
	c[i] = ho*den;
      }
      y += (dy=(2*(ns+1)< (n-m) ? c[ns+1]: d[ns--]));
    }
  value = y;
  error = dy;
  // std::cout<<"value= "<<value<<std::endl;
  if(value <0) value = 0;
  return;
}

void EvtPolInt::ratint()
{
        double y,dy;
	const double TINY=1.0e-25;
	int m,i,ns=0;
	double w,t,hh,h,dd;

	int n=vx.size();
	vector <double> c(n),d(n);
	hh=fabs(xx-vx[0]);
	for (i=0;i<n;i++) {
		h=fabs(xx-vx[i]);
		if (h == 0.0) {
			y=vy[i];
			dy=0.0;
			return;
		} else if (h < hh) {
			ns=i;
			hh=h;
		}
		c[i]=vy[i];
		d[i]=vy[i]+TINY;
	}
	y=vy[ns--];
	for (m=1;m<n;m++) {
		for (i=0;i<n-m;i++) {
			w=c[i+1]-d[i];
			h=vx[i+m]-xx;
			t=(vx[i]-xx)*d[i]/h;
			dd=t-c[i+1];
			if (dd == 0.0) std::cout<<"Error in routine ratint"<<std::endl;
			dd=w/dd;
			d[i]=c[i+1]*dd;
			c[i]=t*dd;
		}
		y += (dy=(2*(ns+1) < (n-m) ? c[ns+1] : d[ns--]));
	}
  value = y;
  error = dy;
  if(value <0) value = 0;
  return;
}

vector <double> EvtPolInt::spline()
{
  vector <double> y2;
  y2.clear();
  for(int i=0;i<size;i++){y2.push_back(0.0);}
  double yp1=0;
  double ypn=0;
	int i,k;
	double p,qn,sig,un;

	int n=size;
	vector<double> u; //(n-1);
	if (yp1 > 0.99e30){
	u[0]=0.0;
	y2.push_back(0.0);}
	else {
        y2.push_back(-0.5);	
	u[0]=(3.0/(vx[1]-vx[0]))*((vy[1]-vy[0])/(vx[1]-vx[0])-yp1);
	}
	for (i=1;i<n-1;i++) {
		sig=(vx[i]-vx[i-1])/(vx[i+1]-vx[i-1]);
		p=sig*y2[i-1]+2.0;
		double yy2=(sig-1.0)/p;
		y2.push_back(yy2);
		u[i]=(vy[i+1]-vy[i])/(vx[i+1]-vx[i]) - (vy[i]-vy[i-1])/(vx[i]-vx[i-1]);
		u[i]=(6.0*u[i]/(vx[i+1]-vx[i-1])-sig*u[i-1])/p;
	}
	if (ypn > 0.99e30)
		qn=un=0.0;
	else {
		qn=0.5;
		un=(3.0/(vx[n-1]-vx[n-2]))*(ypn-(vy[n-1]-vy[n-2])/(vx[n-1]-vx[n-2]));
	}
	y2[n-1]=(un-qn*u[n-2])/(qn*y2[n-2]+1.0);
	for (k=n-2;k>=0;k--)
	y2[k]=y2[k]*y2[k+1]+u[k];
	return y2;  
    }

void EvtPolInt::splint(){
  vector <double> y2a = spline();
  double y;
	int k;
	double h,b,a;

	int n=vx.size();
	int klo=0;
	int khi=n-1;
	while (khi-klo > 1) {
		k=(khi+klo) >> 1;
		if (vx[k] > xx) khi=k;
		else klo=k;
	}
	h=vx[khi]-vx[klo];
	if (h == 0.0) std::cout<<"Bad xa input to routine splint"<<std::endl;
	a=(vx[khi]-xx)/h;
	b=(xx-vx[klo])/h;
	y=a*vy[klo]+b*vy[khi]+((a*a*a-a)*y2a[klo]
		+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
  value = y;
  return;
}

double EvtPolInt::getvalue(){
  polynomial();
  //  ratint();
  //  splint();
return value;
}

double EvtPolInt::geterror(){
  polynomial();
  //ratint();
return error;
}
