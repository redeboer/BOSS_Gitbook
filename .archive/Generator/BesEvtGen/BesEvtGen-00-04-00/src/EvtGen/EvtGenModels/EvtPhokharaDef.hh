#ifndef PHOKHARA__H
#define PHOKHARA__H

#include "cfortran/cfortran.h"
#include <string.h>

// common blocks
typedef struct { double r, i; } COMPLEX16;

// --- couplings, masses, momenta ---
extern "C" struct
{
  double pi, gev2nbarn, gev2pbarn, alpha, me, mmu, mpi, mpi0, Sp, mp;
  double ebeam, ver_s, rmass[10], momenta[4][15];
  COMPLEX16 cvac_s;
  double mnt, mKp, mKn;
  COMPLEX16 cvac_s_pi, cvac_s_phi;
  double mlamb, meta;
} ctes_;


// --- cuts ---
extern "C" struct
{
  double Emin, gmin, phot1cut, phot2cut, pi1cut, pi2cut;
  double piphcut, accecut, q2min, w, q2_max_c, q2_min_c;
} cuts_;

// --- Kaons masses and gamms
extern "C" struct
{
  double m_phi0_Kp, g_phi0_Kp, m_phi1_Kp, g_phi1_Kp;
  double m_om0_Kp, g_om0_Kp, m_om1_Kp, g_om1_Kp, m_om2_Kp, g_om2_Kp;
  double m_rho0_Kp, g_rho0_Kp, m_rho1_Kp, g_rho1_Kp, m_rho2_Kp, g_rho2_Kp;
} mass_gamm_Kp_;

// --- Pions pi+pi- masses and gamms
extern "C"  struct
{
  double m_rho0_pion, g_rho0_pion, m_rho1_pion;
  double g_rho1_pion, m_rho2_pion, g_rho2_pion, m_rho3_pion, g_rho3_pion;
  double m_om0_pion, g_om0_pion;
} mass_gamm_pion_;

// --- Lambdas
extern "C" struct
{
  double alpha_lamb, ratio_lamb;
} lambda_par_;

// --- histograms ---
typedef struct{ char val[20]; } titledef;

extern "C"  struct
{
  DOUBLE_PRECISION xlow[20], xup[20];
  int bins[20];
  titledef title[20];
} histo_;

// --- Maxima ---
extern "C" struct
{
  DOUBLE_PRECISION Mmax[3], gross[3], klein[3], tr[3], count[3];
  int iprint;
} maxima_;

// --- flags --
extern "C" struct
{
  int nlo, ph0, pion, fsr, fsrnlo, ivac, FF_pion, f0_model, FF_kaon, narr_res, FF_pp;
} flags_;


PROTOCCALLSFSUB2(RLXDINIT,rlxdinit,INT,INT)
#define RLXDINIT(LUXURY,SEED) CCALLSFSUB2(RLXDINIT,rlxdinit,INT,INT,LUXURY,SEED)

PROTOCCALLSFSUB1(RLXDRESETF,rlxdresetf,INTV)
#define RLXDRESETF(SEED) CCALLSFSUB1(RLXDRESETF,rlxdresetf,INTV, SEED)

PROTOCCALLSFSUB0(INPUT,input)
#define INPUT() CCALLSFSUB0(INPUT,input)

//PROTOCCALLSFSUB3(INPUT,input,PLONG,PINT,PSTRING);
//#define INPUT(NGES,NM,OUTFILE) CCALLSFSUB3(INPUT,input,PLONG,PINT,PSTRING,NGES,NM,OUTFILE)

//PROTOCCALLSFSUB0(INITHISTO,inithisto);
//#define INITHISTO() CCALLSFSUB0(INITHISTO,inithisto)

//PROTOCCALLSFSUB0(ENDHISTO,endhisto);
//#define ENDHISTO() CCALLSFSUB0(ENDHISTO,endhisto)

//PROTOCCALLSFSUB0(WRITEEVENT,writeevent);
//#define WRITEEVENT() CCALLSFSUB0(WRITEEVENT,writeevent)

PROTOCCALLSFSUB2(RANLXDF,ranlxdf,DOUBLEV,INT)
#define RANLXDF(AR, VAL) CCALLSFSUB2(RANLXDF,ranlxdf,DOUBLEV, INT, AR, VAL)

PROTOCCALLSFSUB7(GEN_1PH,gen_1ph,INT, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE)
#define GEN_1PH(I,QQMIN,QQMAX,COS1MIN,COS1MAX,COS3MIN,COS3MAX) CCALLSFSUB7(GEN_1PH,gen_1ph,INT, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE,I,QQMIN,QQMAX,COS1MIN,COS1MAX,COS3MIN,COS3MAX)

PROTOCCALLSFSUB8(GEN_2PH,gen_2ph,INT, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE)
#define GEN_2PH(I,QQMIN,COS1MIN,COS1MAX,COS2MIN,COS2MAX,COS3MIN,COS3MAX) CCALLSFSUB8(GEN_2PH,gen_2ph,INT, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE,I,QQMIN,COS1MIN,COS1MAX,COS2MIN,COS2MAX,COS3MIN,COS3MAX)

PROTOCCALLSFSUB5(GEN_0PH,gen_0ph,INT, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE)
#define GEN_0PH(I,QQMIN,SP,COS3MIN,COS3MAX) CCALLSFSUB5(GEN_0PH,gen_0ph,INT, PDOUBLE, PDOUBLE, PDOUBLE, PDOUBLE,I,QQMIN,SP,COS3MIN,COS3MAX)

#endif
