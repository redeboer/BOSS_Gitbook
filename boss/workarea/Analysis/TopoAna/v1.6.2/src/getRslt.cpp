#include "../include/topoana.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include <ctime>
#include "TTreeFormula.h"
#include <sstream>

void topoana::getRslt()
{
  unsigned int Nstars=89;
  for(unsigned int i=0;i<Nstars;i++) cout<<"*";
  cout<<endl<<endl;

  TChain * chn=new TChain(m_trNm.c_str());
  for(unsigned int i=0;i<m_nmsOfIptRootFls.size();i++)
    {
      chn->Add(m_nmsOfIptRootFls[i].c_str());
    }

  // int Nps,Pid[200],Midx[200]; // The statement instead of the following two ought to be used along with the Ntuple Tool MCGenKinematics with the option [200].
  const unsigned int NpsMax=chn->GetMaximum(m_brNmOfNps.c_str());
  int Nps,Pid[NpsMax],Midx[NpsMax];
  chn->SetBranchAddress(m_brNmOfNps.c_str(),&Nps);
  chn->SetBranchAddress(m_brNmOfPid.c_str(),&Pid);
  chn->SetBranchAddress(m_brNmOfMidx.c_str(),&Midx);

  vector<int> vPid, vCcPid;
  vector<int> vMidx, vCcMidx;

  vector< list<int> > evtTr, ccEvtTr;
  vector<int> vIdxOfHead, vCcIdxOfHead;
  vector<int> vMidxOfHead, vCcMidxOfHead;
  list<int> evtIFSts,ccEvtIFSts;

  if(m_sttaEvtTrsAndEvtIFSts==false)
    {
      m_vEvtTr.clear(); m_vCcEvtTr.clear();
      m_vIEvtTr.clear(); m_vICcEvtTr.clear();
      m_vNEvtTr.clear(); m_vNCcEvtTr.clear();
      m_vEvtIFSts.clear(); m_vCcEvtIFSts.clear();
      m_vIEvtIFSts.clear(); m_vICcEvtIFSts.clear();
      m_vNEvtIFSts.clear(); m_vNCcEvtIFSts.clear();
      m_iEvtTrIEvtIFStsMap.clear(); m_iEvtTrICcEvtIFStsMap.clear();
    }
  if(m_vSigEvtTr.size()>0)
    {
      m_iSigEvtTrIEvtTrMap.clear();
      m_iSigEvtIFStsIEvtIFStsMap.clear();
    }
  if(m_vSigEvtIFSts2.size()>0)
    {
      m_iSigEvtIFSts2IEvtIFStsMap.clear();
    }

  unsigned long nEvts=chn->GetEntries();
  cout<<"There are "<<nEvts<<" events in total in the input root files."<<endl<<endl;
  unsigned long nEvtsToBePrcsd=nEvts<m_nEvtsMax?nEvts:m_nEvtsMax;
  unsigned long nEvtsThroughTheCut=0;
  clock_t starttime;
  TTreeFormula * trfml;
  unsigned int nInsts=1;
  if(!m_cut.empty())
    { 
      trfml=new TTreeFormula("trfml",m_cut.c_str(),chn);
      chn->SetNotify(trfml); // This statement is indispensible if more than one root file is added to the object of the TChain class.
    }

  string NmOfOptRootFl;
  TFile * fl;
  TTree * tr;

  int iEvtTr,iCcEvtTr,iEvtIFSts,iCcEvtIFSts;
  const unsigned int sPttaPid=m_vPttaPid.size()>0?m_vPttaPid.size():1;
  char nameP[sPttaPid][64]; int nP[sPttaPid],iCcP[sPttaPid],nCcP[sPttaPid],nTotP[sPttaPid];
  int iEvtBrP[sPttaPid][200],iCcEvtBrP[sPttaPid][200],iEvtBrCcP[sPttaPid][200];

  const unsigned int sVPttaPid1=m_vPttaIncEvtBr.size()>0?m_vPttaIncEvtBr.size():1;
  char nameIncEvtBr[sVPttaPid1][64]; int nIncEvtBr[sVPttaPid1],iCcIncEvtBr[sVPttaPid1],nCcIncEvtBr[sVPttaPid1],nTotIncEvtBr[sVPttaPid1];
  int iEvtBrIncEvtBr[sVPttaPid1][200],iCcEvtBrIncEvtBr[sVPttaPid1][200],iEvtBrCcIncEvtBr[sVPttaPid1][200];

  const unsigned int sSigP=m_vSigPid.size()>0?m_vSigPid.size():1;
  int nSigP[sSigP],nCcSigP[sSigP],nTotSigP[sSigP],iCcSigP[sSigP];
  const unsigned int sSigIncEvtBr=m_vSigIncEvtBr.size()>0?m_vSigIncEvtBr.size():1;
  int nSigIncEvtBr[sSigIncEvtBr],nCcSigIncEvtBr[sSigIncEvtBr],nTotSigIncEvtBr[sSigIncEvtBr],iCcSigIncEvtBr[sSigIncEvtBr];
  const unsigned int sSigSeqEvtBrs=m_vSigSeqEvtBrs.size()>0?m_vSigSeqEvtBrs.size():1;
  int nSigSeqEvtBrs[sSigSeqEvtBrs],nCcSigSeqEvtBrs[sSigSeqEvtBrs],nTotSigSeqEvtBrs[sSigSeqEvtBrs],iCcSigSeqEvtBrs[sSigSeqEvtBrs];
  const unsigned int sSigIncSeqEvtBrs=m_vSigIncSeqEvtBrs.size()>0?m_vSigIncSeqEvtBrs.size():1;
  int nSigIncSeqEvtBrs[sSigIncSeqEvtBrs],nCcSigIncSeqEvtBrs[sSigIncSeqEvtBrs],nTotSigIncSeqEvtBrs[sSigIncSeqEvtBrs],iCcSigIncSeqEvtBrs[sSigIncSeqEvtBrs];
  const unsigned int sSigPFSts=m_vSigPFSts.size()>0?m_vSigPFSts.size():1;
  int nSigPFSts[sSigPFSts],nCcSigPFSts[sSigPFSts],nTotSigPFSts[sSigPFSts],iCcSigPFSts[sSigPFSts];
  int iSigEvtTr,iCcSigEvtTr,iSigEvtIFSts,iCcSigEvtIFSts;
  int iSigEvtIFSts2,iCcSigEvtIFSts2;

  for(unsigned int i=0;i<nEvtsToBePrcsd;i++)
    {
      if(i%1000000==0)
        {
          if(i==0)
            {
              NmOfOptRootFl=m_mainNmOfOptFls+".root";
            }
          else
            {
              ostringstream oss;
              oss<<i/1000000;
              string r=oss.str(); 
              NmOfOptRootFl=m_mainNmOfOptFls+"_"+r+".root";
            }
          fl=new TFile(NmOfOptRootFl.c_str(),"recreate");
          if(!fl)
            {
              cerr<<"Error: Can't create the output root file \""<<NmOfOptRootFl<<"\"!"<<endl;
              cerr<<"Infor: Please check it."<<endl;
              exit(-1);
            }
          //cout<<"debug1"<<endl;
          //cout<<tr<<endl;
          tr=chn->CloneTree(0);
          //cout<<tr<<endl;
          //cout<<"debug2"<<endl;

          if(m_sttaEvtTrsAndEvtIFSts==false)
            {
              tr->Branch("iEvtTr",&iEvtTr,"iEvtTr/I");
              tr->Branch("iEvtIFSts",&iEvtIFSts,"iEvtIFSts/I");
            }
          if(m_ccSwitch==true)
            {
              tr->Branch("iCcEvtTr",&iCcEvtTr,"iCcEvtTr/I");
              tr->Branch("iCcEvtIFSts",&iCcEvtIFSts,"iCcEvtIFSts/I");
            }
          if(m_vPttaPid.size()>0)
            {
              char specifier1[30],specifier2[30];
              for(unsigned int i=0;i<m_vPttaPid.size();i++)
                {
                  sprintf(specifier1,"nameP%d",i+1);
                  sprintf(specifier2,"nameP%d/C",i+1);
                  tr->Branch(specifier1,&nameP[i],specifier2);
                  strcpy(nameP[i],m_pidTxtPnmMap[m_vPttaPid[i]].c_str());
                  if(m_ccSwitch==true)
                    {
                      sprintf(specifier1,"iCcP%d",i+1);
                      sprintf(specifier2,"iCcP%d/I",i+1);
                      tr->Branch(specifier1,&iCcP[i],specifier2);
                      iCcP[i]=m_vPttaICcPid[i];
                      if(iCcP[i]==0)
                        {
                          sprintf(specifier1,"nP%d",i+1);
                          sprintf(specifier2,"nP%d/I",i+1);
                          tr->Branch(specifier1,&nP[i],specifier2);
                          sprintf(specifier1,"iEvtBrP%d",i+1);
                          sprintf(specifier2,"iEvtBrP%d[nP%d]/I",i+1,i+1);
                          tr->Branch(specifier1,&iEvtBrP[i],specifier2);
                          sprintf(specifier1,"iCcEvtBrP%d",i+1);
                          sprintf(specifier2,"iCcEvtBrP%d[nP%d]/I",i+1,i+1);
                          tr->Branch(specifier1,&iCcEvtBrP[i],specifier2);                  
                        }
                      else
                        {
                          sprintf(specifier1,"nP%d",i+1);
                          sprintf(specifier2,"nP%d/I",i+1);
                          tr->Branch(specifier1,&nP[i],specifier2);
                          sprintf(specifier1,"iEvtBrP%d",i+1);
                          sprintf(specifier2,"iEvtBrP%d[nP%d]/I",i+1,i+1);
                          tr->Branch(specifier1,&iEvtBrP[i],specifier2);
                          sprintf(specifier1,"nCcP%d",i+1);
                          sprintf(specifier2,"nCcP%d/I",i+1);
                          tr->Branch(specifier1,&nCcP[i],specifier2);
                          sprintf(specifier1,"iEvtBrCcP%d",i+1);
                          sprintf(specifier2,"iEvtBrCcP%d[nCcP%d]/I",i+1,i+1);
                          tr->Branch(specifier1,&iEvtBrCcP[i],specifier2);              
                          sprintf(specifier1,"nTotP%d",i+1);
                          sprintf(specifier2,"nTotP%d/I",i+1);
                          tr->Branch(specifier1,&nTotP[i],specifier2);
                        }
                    }
                  else
                    {
                      sprintf(specifier1,"nP%d",i+1);
                      sprintf(specifier2,"nP%d/I",i+1);
                      tr->Branch(specifier1,&nP[i],specifier2);
                      sprintf(specifier1,"iEvtBrP%d",i+1);
                      sprintf(specifier2,"iEvtBrP%d[nP%d]/I",i+1,i+1);
                      tr->Branch(specifier1,&iEvtBrP[i],specifier2);
                    }
                }
            }
          if(m_vPttaIncEvtBr.size()>0)
            {
              char specifier1[100],specifier2[100];
              list<int>::iterator liit;
              for(unsigned int i=0;i<m_vPttaIncEvtBr.size();i++)
                {
                  sprintf(specifier1,"nameIncEvtBr%d",i+1);
                  sprintf(specifier2,"nameIncEvtBr%d/C",i+1);
                  tr->Branch(specifier1,&nameIncEvtBr[i],specifier2);
                  // The following statement is used to clear the character array nameIncEvtBr[i].
                  strcpy(nameIncEvtBr[i],"");
                  liit=m_vPttaIncEvtBr[i].begin();
                  strcat(nameIncEvtBr[i],m_pidTxtPnmMap[(*liit)].c_str());
                  strcat(nameIncEvtBr[i]," ");
                  if((*liit)==-11)
                    {
                      liit++;
                      strcat(nameIncEvtBr[i],m_pidTxtPnmMap[(*liit)].c_str());
                      strcat(nameIncEvtBr[i]," ");
                    }
                  strcat(nameIncEvtBr[i],"-->");
                  for(liit++;liit!=m_vPttaIncEvtBr[i].end();liit++)
                    {
                      strcat(nameIncEvtBr[i]," ");
                      strcat(nameIncEvtBr[i],m_pidTxtPnmMap[(*liit)].c_str());
                    }
                  strcat(nameIncEvtBr[i]," + anything");
                  if(m_ccSwitch==true)
                    {
                      sprintf(specifier1,"iCcIncEvtBr%d",i+1);
                      sprintf(specifier2,"iCcIncEvtBr%d/I",i+1);
                      tr->Branch(specifier1,&iCcIncEvtBr[i],specifier2);
                      iCcIncEvtBr[i]=m_vPttaICcPid[i];
                      if(iCcIncEvtBr[i]==0)
                        {
                          sprintf(specifier1,"nIncEvtBr%d",i+1);
                          sprintf(specifier2,"nIncEvtBr%d/I",i+1);
                          tr->Branch(specifier1,&nIncEvtBr[i],specifier2);
                          sprintf(specifier1,"iEvtBrIncEvtBr%d",i+1);
                          sprintf(specifier2,"iEvtBrIncEvtBr%d[nIncEvtBr%d]/I",i+1,i+1);
                          tr->Branch(specifier1,&iEvtBrIncEvtBr[i],specifier2);
                          sprintf(specifier1,"iCcEvtBrIncEvtBr%d",i+1);
                          sprintf(specifier2,"iCcEvtBrIncEvtBr%d[nIncEvtBr%d]/I",i+1,i+1);
                          tr->Branch(specifier1,&iCcEvtBrIncEvtBr[i],specifier2);                  
                        }
                      else
                        {
                          sprintf(specifier1,"nIncEvtBr%d",i+1);
                          sprintf(specifier2,"nIncEvtBr%d/I",i+1);
                          tr->Branch(specifier1,&nIncEvtBr[i],specifier2);
                          sprintf(specifier1,"iEvtBrIncEvtBr%d",i+1);
                          sprintf(specifier2,"iEvtBrIncEvtBr%d[nIncEvtBr%d]/I",i+1,i+1);
                          tr->Branch(specifier1,&iEvtBrIncEvtBr[i],specifier2);
                          sprintf(specifier1,"nCcIncEvtBr%d",i+1);
                          sprintf(specifier2,"nCcIncEvtBr%d/I",i+1);
                          tr->Branch(specifier1,&nCcIncEvtBr[i],specifier2);
                          sprintf(specifier1,"iEvtBrCcIncEvtBr%d",i+1);
                          sprintf(specifier2,"iEvtBrCcIncEvtBr%d[nCcIncEvtBr%d]/I",i+1,i+1);
                          tr->Branch(specifier1,&iEvtBrCcIncEvtBr[i],specifier2);              
                          sprintf(specifier1,"nTotIncEvtBr%d",i+1);
                          sprintf(specifier2,"nTotIncEvtBr%d/I",i+1);
                          tr->Branch(specifier1,&nTotIncEvtBr[i],specifier2);
                        }
                    }
                  else
                    {
                      sprintf(specifier1,"nIncEvtBr%d",i+1);
                      sprintf(specifier2,"nIncEvtBr%d/I",i+1);
                      tr->Branch(specifier1,&nIncEvtBr[i],specifier2);
                      sprintf(specifier1,"iEvtBrIncEvtBr%d",i+1);
                      sprintf(specifier2,"iEvtBrIncEvtBr%d[nIncEvtBr%d]/I",i+1,i+1);
                      tr->Branch(specifier1,&iEvtBrIncEvtBr[i],specifier2);
                    }
                }
            }
          if(m_vSigPid.size()>0)
            {
              char specifier[30];
              if(m_ccSwitch==true)
                {
                  sprintf(specifier,"iCcSigP[%d]/I", int(sSigP));
                  tr->Branch("iCcSigP",&iCcSigP,specifier);
                  for(unsigned int i=0;i<sSigP;i++) iCcSigP[i]=m_vICcSigP[i];
                  sprintf(specifier,"nSigP[%d]/I", int(sSigP));
                  tr->Branch("nSigP",&nSigP,specifier);
                  sprintf(specifier,"nCcSigP[%d]/I", int(sSigP));
                  tr->Branch("nCcSigP",&nCcSigP,specifier);
                  sprintf(specifier,"nTotSigP[%d]/I", int(sSigP));
                  tr->Branch("nTotSigP",&nTotSigP,specifier);
                }
              else
                {
                  sprintf(specifier,"nSigP[%d]/I", int(sSigP));
                  tr->Branch("nSigP",&nSigP,specifier);        
                }  
            }
          if(m_vSigIncEvtBr.size()>0)
            {
              char specifier[30];
              if(m_ccSwitch==true)
                {
                  sprintf(specifier,"iCcSigIncEvtBr[%d]/I", int(sSigIncEvtBr));
                  tr->Branch("iCcSigIncEvtBr",&iCcSigIncEvtBr,specifier);
                  for(unsigned int i=0;i<sSigIncEvtBr;i++) iCcSigIncEvtBr[i]=m_vICcSigIncEvtBr[i];
                  sprintf(specifier,"nSigIncEvtBr[%d]/I", int(sSigIncEvtBr));
                  tr->Branch("nSigIncEvtBr",&nSigIncEvtBr,specifier);
                  sprintf(specifier,"nCcSigIncEvtBr[%d]/I", int(sSigIncEvtBr));
                  tr->Branch("nCcSigIncEvtBr",&nCcSigIncEvtBr,specifier);
                  sprintf(specifier,"nTotSigIncEvtBr[%d]/I", int(sSigIncEvtBr));
                  tr->Branch("nTotSigIncEvtBr",&nTotSigIncEvtBr,specifier);
                }
              else
                {
                  sprintf(specifier,"nSigIncEvtBr[%d]/I", int(sSigIncEvtBr));
                  tr->Branch("nSigIncEvtBr",&nSigIncEvtBr,specifier);
                }
            }
          if(m_vSigSeqEvtBrs.size()>0)
            {
              char specifier[30];
              if(m_ccSwitch==true)
                {
                  sprintf(specifier,"iCcSigSeqEvtBrs[%d]/I", int(sSigSeqEvtBrs));
                  tr->Branch("iCcSigSeqEvtBrs",&iCcSigSeqEvtBrs,specifier);
                  for(unsigned int i=0;i<sSigSeqEvtBrs;i++) iCcSigSeqEvtBrs[i]=m_vICcSigSeqEvtBrs[i];
                  sprintf(specifier,"nSigSeqEvtBrs[%d]/I", int(sSigSeqEvtBrs));
                  tr->Branch("nSigSeqEvtBrs",&nSigSeqEvtBrs,specifier);
                  sprintf(specifier,"nCcSigSeqEvtBrs[%d]/I", int(sSigSeqEvtBrs));
                  tr->Branch("nCcSigSeqEvtBrs",&nCcSigSeqEvtBrs,specifier);
                  sprintf(specifier,"nTotSigSeqEvtBrs[%d]/I", int(sSigSeqEvtBrs));
                  tr->Branch("nTotSigSeqEvtBrs",&nTotSigSeqEvtBrs,specifier);
                }
              else
                {
                  sprintf(specifier,"nSigSeqEvtBrs[%d]/I", int(sSigSeqEvtBrs));
                  tr->Branch("nSigSeqEvtBrs",&nSigSeqEvtBrs,specifier);
                }
            }
          if(m_vSigIncSeqEvtBrs.size()>0)
            {
              char specifier[30];
              if(m_ccSwitch==true)
                {
                  sprintf(specifier,"iCcSigIncSeqEvtBrs[%d]/I", int(sSigIncSeqEvtBrs));
                  tr->Branch("iCcSigIncSeqEvtBrs",&iCcSigIncSeqEvtBrs,specifier);
                  for(unsigned int i=0;i<sSigIncSeqEvtBrs;i++) iCcSigIncSeqEvtBrs[i]=m_vICcSigIncSeqEvtBrs[i];
                  sprintf(specifier,"nSigIncSeqEvtBrs[%d]/I", int(sSigIncSeqEvtBrs));
                  tr->Branch("nSigIncSeqEvtBrs",&nSigIncSeqEvtBrs,specifier);
                  sprintf(specifier,"nCcSigIncSeqEvtBrs[%d]/I", int(sSigIncSeqEvtBrs));
                  tr->Branch("nCcSigIncSeqEvtBrs",&nCcSigIncSeqEvtBrs,specifier);
                  sprintf(specifier,"nTotSigIncSeqEvtBrs[%d]/I", int(sSigIncSeqEvtBrs));
                  tr->Branch("nTotSigIncSeqEvtBrs",&nTotSigIncSeqEvtBrs,specifier);
                }
              else
                {
                  sprintf(specifier,"nSigIncSeqEvtBrs[%d]/I", int(sSigIncSeqEvtBrs));
                  tr->Branch("nSigIncSeqEvtBrs",&nSigIncSeqEvtBrs,specifier);
                }
            }
          if(m_vSigPFSts.size()>0)
            {
              char specifier[30];
              if(m_ccSwitch==true)
                {
                  sprintf(specifier,"iCcSigPFSts[%d]/I", int(sSigPFSts));
                  tr->Branch("iCcSigPFSts",&iCcSigPFSts,specifier);
                  for(unsigned int i=0;i<sSigPFSts;i++) iCcSigPFSts[i]=m_vICcSigPFSts[i];
                  sprintf(specifier,"nSigPFSts[%d]/I", int(sSigPFSts));
                  tr->Branch("nSigPFSts",&nSigPFSts,specifier);
                  sprintf(specifier,"nCcSigPFSts[%d]/I", int(sSigPFSts));
                  tr->Branch("nCcSigPFSts",&nCcSigPFSts,specifier);
                  sprintf(specifier,"nTotSigPFSts[%d]/I", int(sSigPFSts));
                  tr->Branch("nTotSigPFSts",&nTotSigPFSts,specifier);
                }
              else
                {
                  sprintf(specifier,"nSigPFSts[%d]/I", int(sSigPFSts));
                  tr->Branch("nSigPFSts",&nSigPFSts,specifier);
                }
            }
          if(m_vSigEvtTr.size()>0)
            {
              tr->Branch("iSigEvtTr",&iSigEvtTr,"iSigEvtTr/I");
              tr->Branch("iSigEvtIFSts",&iSigEvtIFSts,"iSigEvtIFSts/I");
              if(m_ccSwitch==true)
                {
                  tr->Branch("iCcSigEvtTr",&iCcSigEvtTr,"iCcSigEvtTr/I");
                  tr->Branch("iCcSigEvtIFSts",&iCcSigEvtIFSts,"iCcSigEvtIFSts/I");
                }
            }
          if(m_vSigEvtIFSts2.size()>0)
            {
              tr->Branch("iSigEvtIFSts2",&iSigEvtIFSts2,"iSigEvtIFSts2/I");
              if(m_ccSwitch==true)
                {
                  tr->Branch("iCcSigEvtIFSts2",&iCcSigEvtIFSts2,"iCcSigEvtIFSts2/I");
                }
            }
      }

      if(i==0) starttime=clock();

      chn->GetEntry(i);

      if(!m_cut.empty())
        { 
          // The following four statements are used to handle the cases where array variables are used in the cut.
          nInsts=trfml->GetNdata(); // This statement is indispensable if multiple instances have to be evaluated by the object of the TTreeFormula class.
          bool passTheCut;
          if(m_cutMethod==true)
            {
              passTheCut=false; 
              for(unsigned int j=0;j<nInsts;j++) if(trfml->EvalInstance(j)) {passTheCut=true; break;}
            }
          else
            {
              passTheCut=true; 
              for(unsigned int j=0;j<nInsts;j++) if(!(trfml->EvalInstance(j))) {passTheCut=false; break;}
            }
          if(!passTheCut)
            {
              if((i+1>=5000)&&(((i+1)%5000==0)||((i+1)==nEvtsToBePrcsd)))
                {
                  if((i+1)==5000) cout<<"Number of events processed\tNumber of seconds elapsed"<<endl<<endl;
                  cout<<setiosflags(ios::right)<<setw(14)<<i+1<<"\t\t\t"<<setiosflags(ios::fixed)<<setprecision(2)<<setw(14)<<(clock()-starttime)/((double) CLOCKS_PER_SEC)<<resetiosflags(ios::adjustfield)<<endl;
                  if((i+1)==nEvtsToBePrcsd) 
                    {
                      cout<<endl<<"Note that only "<<nEvtsThroughTheCut<<" events passed the cut."<<endl<<endl;
                    }
                }
              if((i+1)==nEvtsToBePrcsd)
                {
                  for(unsigned int j=0;j<Nstars;j++) cout<<"*";
                  cout<<endl<<endl;
                }
              continue;
            }   
          else
            {
              nEvtsThroughTheCut++;
            }
        }
      vPid.clear();
      vMidx.clear();
      for(int j=0;j<Nps;j++)
        {
          vPid.push_back(Pid[j]);
          vMidx.push_back(Midx[j]);
          //cout<<j<<"\t"<<Pid[j]<<"\t"<<Midx[j]<<endl; 
        }
      //cout<<endl;

      /*for(unsigned int j=0;j<vPid.size();j++)
        {
          cout<<j<<"\t"<<vPid[j]<<"\t"<<vMidx[j]<<endl;
        }
      cout<<endl;*/

      sortPs(vPid,vMidx);
      evtTr.clear();
      vIdxOfHead.clear();
      vMidxOfHead.clear();
      getEvtTr(vPid,vMidx,evtTr,vIdxOfHead,vMidxOfHead);
      evtIFSts.clear();
      getEvtIFSts(vPid,vMidx,evtIFSts);


     if(m_sttaEvtTrsAndEvtIFSts==false)
       {
          iEvtTr=-1; // If the variable is still equal to -1 after the following loop, then the event tree is a new event tree.
          for(unsigned int j=0;j<m_vEvtTr.size();j++)
            {
              if(evtTr==m_vEvtTr[j])
                {
                  iEvtTr=j;
                  iEvtIFSts=m_iEvtTrIEvtIFStsMap[iEvtTr];
                  if(m_ccSwitch==true)
                    {
                      iCcEvtTr=m_vICcEvtTr[j];
                      iCcEvtIFSts=m_iEvtTrICcEvtIFStsMap[iEvtTr];
                    }
                  break;
                }
              else if(m_ccSwitch==true&&m_vICcEvtTr[j]!=0&&evtTr==m_vCcEvtTr[j])
                {
                  iEvtTr=j;
                  iEvtIFSts=m_iEvtTrIEvtIFStsMap[iEvtTr];
                  iCcEvtTr=-1;
                  iCcEvtIFSts=-1*m_iEvtTrICcEvtIFStsMap[iEvtTr];
                  break;
                }
            }
          if(iEvtTr==-1)
            {
              iEvtTr=m_vEvtTr.size();
              m_vEvtTr.push_back(evtTr);
              m_vIEvtTr.push_back(iEvtTr);
              m_vNEvtTr.push_back(1);
              if(m_ccSwitch==true)
                {
                  vCcPid.clear(); 
                  vCcMidx.clear();
                  for(unsigned int j=0;j<vPid.size();j++) vCcPid.push_back(getCcPid(vPid[j]));
                  vCcMidx=vMidx;
                  sortPs(vCcPid,vCcMidx);
                  ccEvtTr.clear();
                  vCcIdxOfHead.clear();
                  vCcMidxOfHead.clear();
                  getEvtTr(vCcPid,vCcMidx,ccEvtTr,vCcIdxOfHead,vCcMidxOfHead);
                  if(ccEvtTr==evtTr) iCcEvtTr=0;
                  else iCcEvtTr=1;
                  m_vCcEvtTr.push_back(ccEvtTr);
                  m_vICcEvtTr.push_back(iCcEvtTr);
                  m_vNCcEvtTr.push_back(0);
                }

              iEvtIFSts=-1; // If the variable is still equal to -1 after the following loop, then the event initial-final states is a new event initial-final states.
              for(unsigned int j=0;j<m_vEvtIFSts.size();j++)
                {
                  if(evtIFSts==m_vEvtIFSts[j])
                    {
                      iEvtIFSts=j;
                      if(m_ccSwitch==true) iCcEvtIFSts=m_vICcEvtIFSts[j];
                      break;
                    }
                  else if(m_ccSwitch==true&&m_vICcEvtIFSts[j]!=0&&evtIFSts==m_vCcEvtIFSts[j])
                    {
                      iEvtIFSts=j;
                      iCcEvtIFSts=-1;
                      break;
                    }              
                }
              if(iEvtIFSts==-1)
                {
                  iEvtIFSts=m_vEvtIFSts.size();
                  m_vEvtIFSts.push_back(evtIFSts);
                  m_vIEvtIFSts.push_back(iEvtIFSts);
                  m_vNEvtIFSts.push_back(1);
                  if(m_ccSwitch==true)
                    {
                      ccEvtIFSts.clear();
                      getEvtIFSts(vCcPid,vCcMidx,ccEvtIFSts);
                      if(ccEvtIFSts==evtIFSts) iCcEvtIFSts=0;
                      else iCcEvtIFSts=1;
                      m_vCcEvtIFSts.push_back(ccEvtIFSts);
                      m_vICcEvtIFSts.push_back(iCcEvtIFSts);
                      m_vNCcEvtIFSts.push_back(0);
                    } 
                }
              else
                {
                  if(m_ccSwitch==true)
                    {
                      if(iCcEvtIFSts==-1) m_vNCcEvtIFSts[iEvtIFSts]++;
                      else m_vNEvtIFSts[iEvtIFSts]++;
                    }
                  else m_vNEvtIFSts[iEvtIFSts]++;             
                }
              m_iEvtTrIEvtIFStsMap[iEvtTr]=iEvtIFSts;
              if(m_ccSwitch==true) m_iEvtTrICcEvtIFStsMap[iEvtTr]=iCcEvtIFSts;
            }
          else
            {
              if(m_ccSwitch==true)
                {
                  if(iCcEvtTr==-1) m_vNCcEvtTr[iEvtTr]++;
                  else m_vNEvtTr[iEvtTr]++;
                  if(iCcEvtIFSts==-1) m_vNCcEvtIFSts[iEvtIFSts]++;
                  else m_vNEvtIFSts[iEvtIFSts]++;
                }
              else
                {
                  m_vNEvtTr[iEvtTr]++;
                  m_vNEvtIFSts[iEvtIFSts]++;
                }
            }
        }

      if(m_vPttaPid.size()>0)
        {
          // Pay attention to that evtBrCcP equate to ccEvtBrP for self-charge-conjugate particles.
          list<int> evtBrP, evtBrCcP;
          evtBrP.clear(); evtBrCcP.clear();
          for(unsigned int j=0;j<m_vPttaPid.size();j++)
            {
              nP[j]=0;
              if(m_ccSwitch==true&&m_vPttaICcPid[j]!=0)
                {
                  nCcP[j]=0;
                  nTotP[j]=0;
                }
            }     
          for(unsigned int j=1;j<evtTr.size();j++)
            {
              list<int>::iterator liit=evtTr[j].begin();
              for(unsigned int k=0;k<m_vPttaPid.size();k++)
                {
                  if((*liit)==m_vPttaPid[k])
                    {
                      evtBrP=evtTr[j];
                      int _iEvtBrP=-1; // If the variable is still equal to -1 after the following loop, then the event branch of the particle is a new event branch of the particle.
                      int _iCcEvtBrP=-9999;
                      for(unsigned int l=0;l<m_vVEvtBrP[k].size();l++)
                        {
                          if(evtBrP==m_vVEvtBrP[k][l])
                            {
                              _iEvtBrP=l;
                              if(m_ccSwitch==true&&m_vPttaICcPid[k]==0) _iCcEvtBrP=m_vVIEvtBrCcP[k][l];
                              break;
                            }
                          else if(m_ccSwitch==true&&m_vPttaICcPid[k]==0&&m_vVIEvtBrCcP[k][l]!=0&&evtBrP==m_vVEvtBrCcP[k][l])
                            {
                              _iEvtBrP=l;
                              _iCcEvtBrP=-1;
                              break;
                            }
                        }
                      if(_iEvtBrP==-1)
                        {
                          _iEvtBrP=m_vVEvtBrP[k].size();
                          iEvtBrP[k][(unsigned int) (nP[k])]=_iEvtBrP;
                          m_vVEvtBrP[k].push_back(evtBrP);
                          m_vVIEvtBrP[k].push_back(_iEvtBrP);
                          m_vVNEvtBrP[k].push_back(1);
                          if(m_ccSwitch==true)
                            {
                              evtBrCcP.clear();
                              list<int>::iterator liit=evtBrP.begin();
                              for(liit++;liit!=evtBrP.end();liit++) evtBrCcP.push_back(getCcPid((*liit)));
                              sortByPidAndPchrg(evtBrCcP); 
                              liit=evtBrP.begin();
                              if(m_vPttaICcPid[k]!=0)
                                {
                                  evtBrCcP.push_front(getCcPid((*liit))); // Here, (*liit) is not equal to getCcPid((*liit)).
                                  m_vVEvtBrCcP[k].push_back(evtBrCcP);
                                  m_vVIEvtBrCcP[k].push_back(_iEvtBrP);
                                  m_vVNEvtBrCcP[k].push_back(0);
                                }    
                              else
                                {
                                  evtBrCcP.push_front((*liit)); // Here, (*liit) is equal to getCcPid((*liit)).
                                  if(evtBrCcP==evtBrP) _iCcEvtBrP=0;
                                  else _iCcEvtBrP=1;
                                  iCcEvtBrP[k][(unsigned int) (nP[k])]=_iCcEvtBrP;
                                  m_vVEvtBrCcP[k].push_back(evtBrCcP);
                                  m_vVIEvtBrCcP[k].push_back(_iCcEvtBrP);
                                  m_vVNEvtBrCcP[k].push_back(0);
                                }
                            }
                        }
                      else
                        {
                          iEvtBrP[k][(unsigned int) (nP[k])]=_iEvtBrP;
                          if(m_ccSwitch==true&&m_vPttaICcPid[k]==0)
                            {
                              iCcEvtBrP[k][(unsigned int) (nP[k])]=_iCcEvtBrP;
                              if(_iCcEvtBrP==-1) m_vVNEvtBrCcP[k][_iEvtBrP]++;
                              else m_vVNEvtBrP[k][_iEvtBrP]++;
                            }
                          else
                            {
                              m_vVNEvtBrP[k][_iEvtBrP]++;
                            }
                        }
                      nP[k]++;
                      break;
                    }
                  else if(m_ccSwitch==true&&(*liit)==m_vPttaCcPid[k])
                    {
                      evtBrCcP=evtTr[j];
                      int _iEvtBrP=-1; // If the variable is still equal to -1 after the following loop, then the event branch of the particle is a new event branch of the particle.
                      for(unsigned int l=0;l<m_vVEvtBrCcP[k].size();l++)
                        {
                          if(evtBrCcP==m_vVEvtBrCcP[k][l])
                            {
                              _iEvtBrP=l;
                              break;
                            }
                        }
                      if(_iEvtBrP==-1)
                        {
                          _iEvtBrP=m_vVEvtBrP[k].size();
                          iEvtBrCcP[k][(unsigned int) (nCcP[k])]=_iEvtBrP;
                          m_vVEvtBrCcP[k].push_back(evtBrCcP);
                          m_vVIEvtBrCcP[k].push_back(_iEvtBrP);
                          m_vVNEvtBrCcP[k].push_back(1);

                          list<int> evtBrP;
                          evtBrP.clear();
                          list<int>::iterator liit=evtBrCcP.begin();
                          for(liit++;liit!=evtBrCcP.end();liit++) evtBrP.push_back(getCcPid((*liit)));
                          sortByPidAndPchrg(evtBrP);
                          liit=evtBrCcP.begin();
                          evtBrP.push_front(getCcPid((*liit))); // Here, (*liit) is not equal to getCcPid((*liit)).
                          m_vVEvtBrP[k].push_back(evtBrP);
                          m_vVIEvtBrP[k].push_back(_iEvtBrP);
                          m_vVNEvtBrP[k].push_back(0);
                        }
                      else
                        {
                          iEvtBrCcP[k][(unsigned int) (nCcP[k])]=_iEvtBrP;
                          m_vVNEvtBrCcP[k][_iEvtBrP]++;
                        }
                      nCcP[k]++;
                      break;                                    
                    }
                }
            }
          if(m_ccSwitch==true) for(unsigned int j=0;j<m_vPttaPid.size();j++) if(m_vPttaICcPid[j]!=0) nTotP[j]=nP[j]+nCcP[j];
        }

      if(m_vPttaIncEvtBr.size()>0)
        {
          // Pay attention to that evtBrCcIncEvtBr equate to ccEvtBrIncEvtBr for self-charge-conjugate inclusive decays.
          list<int> evtBrIncEvtBr, evtBrCcIncEvtBr;
          evtBrIncEvtBr.clear(); evtBrCcIncEvtBr.clear();
          for(unsigned int j=0;j<m_vPttaIncEvtBr.size();j++)
            {
              nIncEvtBr[j]=0;
              if(m_ccSwitch==true&&m_vPttaICcIncEvtBr[j]!=0)
                {
                  nCcIncEvtBr[j]=0;
                  nTotIncEvtBr[j]=0;
                }
            }     
          for(unsigned int j=0;j<evtTr.size();j++)
            {
              for(unsigned int k=0;k<m_vPttaIncEvtBr.size();k++)
                {
                  if(isLiaMatchedWithLib(m_vPttaIncEvtBr[k],evtTr[j]))
                    {
                      evtBrIncEvtBr=evtTr[j];
                      int _iEvtBrIncEvtBr=-1; // If the variable is still equal to -1 after the following loop, then the event branch of the particle is a new event branch of the particle.
                      int _iCcEvtBrIncEvtBr=-9999;
                      for(unsigned int l=0;l<m_vVEvtBrIncEvtBr[k].size();l++)
                        {
                          if(evtBrIncEvtBr==m_vVEvtBrIncEvtBr[k][l])
                            {
                              _iEvtBrIncEvtBr=l;
                              if(m_ccSwitch==true&&m_vPttaICcIncEvtBr[k]==0) _iCcEvtBrIncEvtBr=m_vVIEvtBrCcIncEvtBr[k][l];
                              break;
                            }
                          else if(m_ccSwitch==true&&m_vPttaICcIncEvtBr[k]==0&&m_vVIEvtBrCcIncEvtBr[k][l]!=0&&evtBrIncEvtBr==m_vVEvtBrCcIncEvtBr[k][l])
                            {
                              _iEvtBrIncEvtBr=l;
                              _iCcEvtBrIncEvtBr=-1;
                              break;
                            }
                        }
                      if(_iEvtBrIncEvtBr==-1)
                        {
                          _iEvtBrIncEvtBr=m_vVEvtBrIncEvtBr[k].size();
                          iEvtBrIncEvtBr[k][(unsigned int) (nIncEvtBr[k])]=_iEvtBrIncEvtBr;
                          m_vVEvtBrIncEvtBr[k].push_back(evtBrIncEvtBr);
                          m_vVIEvtBrIncEvtBr[k].push_back(_iEvtBrIncEvtBr);
                          m_vVNEvtBrIncEvtBr[k].push_back(1);
                          if(m_ccSwitch==true)
                            {
                              evtBrCcIncEvtBr.clear();
                              list<int>::iterator liit=evtBrIncEvtBr.begin();
                              if((*liit)==-11) liit++;
                              for(liit++;liit!=evtBrIncEvtBr.end();liit++) evtBrCcIncEvtBr.push_back(getCcPid((*liit)));
                              sortByPidAndPchrg(evtBrCcIncEvtBr); 
                              liit=evtBrIncEvtBr.begin();
                              if(m_vPttaICcIncEvtBr[k]!=0)
                                {
                                  if((*liit)!=-11)
                                    {
                                      evtBrCcIncEvtBr.push_front(getCcPid((*liit))); // Here, (*liit) is not equal to getCcPid((*liit)).
                                    }
                                  else
                                    {
                                      evtBrCcIncEvtBr.push_front(11);
                                      evtBrCcIncEvtBr.push_front(-11);
                                    }
                                  m_vVEvtBrCcIncEvtBr[k].push_back(evtBrCcIncEvtBr);
                                  m_vVIEvtBrCcIncEvtBr[k].push_back(_iEvtBrIncEvtBr);
                                  m_vVNEvtBrCcIncEvtBr[k].push_back(0);
                                }    
                              else
                                {
                                  if((*liit)!=-11)
                                    {
                                      evtBrCcIncEvtBr.push_front((*liit)); // Here, (*liit) is equal to getCcPid((*liit)).
                                    }
                                  else
                                    {
                                      evtBrCcIncEvtBr.push_front(11);
                                      evtBrCcIncEvtBr.push_front(-11);
                                    }
                                  if(evtBrCcIncEvtBr==evtBrIncEvtBr) _iCcEvtBrIncEvtBr=0;
                                  else _iCcEvtBrIncEvtBr=1;
                                  iCcEvtBrIncEvtBr[k][(unsigned int) (nIncEvtBr[k])]=_iCcEvtBrIncEvtBr;
                                  m_vVEvtBrCcIncEvtBr[k].push_back(evtBrCcIncEvtBr);
                                  m_vVIEvtBrCcIncEvtBr[k].push_back(_iCcEvtBrIncEvtBr);
                                  m_vVNEvtBrCcIncEvtBr[k].push_back(0);
                                }
                            }
                        }
                      else
                        {
                          iEvtBrIncEvtBr[k][(unsigned int) (nIncEvtBr[k])]=_iEvtBrIncEvtBr;
                          if(m_ccSwitch==true&&m_vPttaICcIncEvtBr[k]==0)
                            {
                              iCcEvtBrIncEvtBr[k][(unsigned int) (nIncEvtBr[k])]=_iCcEvtBrIncEvtBr;
                              if(_iCcEvtBrIncEvtBr==-1) m_vVNEvtBrCcIncEvtBr[k][_iEvtBrIncEvtBr]++;
                              else m_vVNEvtBrIncEvtBr[k][_iEvtBrIncEvtBr]++;
                            }
                          else
                            {
                              m_vVNEvtBrIncEvtBr[k][_iEvtBrIncEvtBr]++;
                            }
                        }
                      nIncEvtBr[k]++;
                      break;
                    }
                  else if(m_ccSwitch==true&&isLiaMatchedWithLib(m_vPttaCcIncEvtBr[k],evtTr[j]))
                    {
                      evtBrCcIncEvtBr=evtTr[j];
                      int _iEvtBrIncEvtBr=-1; // If the variable is still equal to -1 after the following loop, then the event branch of the particle is a new event branch of the particle.
                      for(unsigned int l=0;l<m_vVEvtBrCcIncEvtBr[k].size();l++)
                        {
                          if(evtBrCcIncEvtBr==m_vVEvtBrCcIncEvtBr[k][l])
                            {
                              _iEvtBrIncEvtBr=l;
                              break;
                            }
                        }
                      if(_iEvtBrIncEvtBr==-1)
                        {
                          _iEvtBrIncEvtBr=m_vVEvtBrIncEvtBr[k].size();
                          iEvtBrCcIncEvtBr[k][(unsigned int) (nCcIncEvtBr[k])]=_iEvtBrIncEvtBr;
                          m_vVEvtBrCcIncEvtBr[k].push_back(evtBrCcIncEvtBr);
                          m_vVIEvtBrCcIncEvtBr[k].push_back(_iEvtBrIncEvtBr);
                          m_vVNEvtBrCcIncEvtBr[k].push_back(1);

                          list<int> evtBrIncEvtBr;
                          evtBrIncEvtBr.clear();
                          list<int>::iterator liit=evtBrCcIncEvtBr.begin();
                          if((*liit)==-11) liit++;
                          for(liit++;liit!=evtBrCcIncEvtBr.end();liit++) evtBrIncEvtBr.push_back(getCcPid((*liit)));
                          sortByPidAndPchrg(evtBrIncEvtBr);
                          liit=evtBrCcIncEvtBr.begin();
                          if((*liit)!=-11)
                            {
                              evtBrIncEvtBr.push_front(getCcPid((*liit))); // Here, (*liit) is not equal to getCcPid((*liit)).
                            }
                          else
                            {
                              evtBrIncEvtBr.push_front(11);
                              evtBrIncEvtBr.push_front(-11);
                            }
                          m_vVEvtBrIncEvtBr[k].push_back(evtBrIncEvtBr);
                          m_vVIEvtBrIncEvtBr[k].push_back(_iEvtBrIncEvtBr);
                          m_vVNEvtBrIncEvtBr[k].push_back(0);
                        }
                      else
                        {
                          iEvtBrCcIncEvtBr[k][(unsigned int) (nCcIncEvtBr[k])]=_iEvtBrIncEvtBr;
                          m_vVNEvtBrCcIncEvtBr[k][_iEvtBrIncEvtBr]++;
                        }
                      nCcIncEvtBr[k]++;
                      break;                                    
                    }
                }
            }
          if(m_ccSwitch==true) for(unsigned int j=0;j<m_vPttaIncEvtBr.size();j++) if(m_vPttaICcIncEvtBr[j]!=0) nTotIncEvtBr[j]=nIncEvtBr[j]+nCcIncEvtBr[j];
        }

      if(m_vSigPid.size()>0)
        {
          for(unsigned int j=0;j<m_vSigPid.size();j++)
            {
              nSigP[j]=0;
              nCcSigP[j]=0;
            }
          for(unsigned int j=0;j<vPid.size();j++)
            {
              for(unsigned int k=0;k<m_vSigPid.size();k++)
                {
                  if(vPid[j]==m_vSigPid[k])
                    {
                      m_vNSigP[k]++;
                      nSigP[k]++;
                      break;
                    }
                  else if(m_ccSwitch==true&&m_vICcSigP[k]!=0&&vPid[j]==m_vCcSigPid[k])
                    {
                      m_vNCcSigP[k]++;
                      nCcSigP[k]++;
                      break;
                    }
                }
            }
          if(m_ccSwitch==true) for(unsigned int j=0;j<m_vSigPid.size();j++) nTotSigP[j]=nSigP[j]+nCcSigP[j];
        }

      if(m_vSigIncEvtBr.size()>0)
        {
          for(unsigned int j=0;j<m_vSigIncEvtBr.size();j++)
            {
              nSigIncEvtBr[j]=countIncLiaInVlib(m_vSigIncEvtBr[j],evtTr);
              m_vNSigIncEvtBr[j]=m_vNSigIncEvtBr[j]+nSigIncEvtBr[j];
              if(m_ccSwitch==true)
                {
                  nCcSigIncEvtBr[j]=countIncLiaInVlib(m_vCcSigIncEvtBr[j],evtTr);
                  m_vNCcSigIncEvtBr[j]=m_vNCcSigIncEvtBr[j]+nCcSigIncEvtBr[j];
                  nTotSigIncEvtBr[j]=nSigIncEvtBr[j]+nCcSigIncEvtBr[j];
                }
            }
        }

      if(m_vSigSeqEvtBrs.size()>0)
        {
          for(unsigned int j=0;j<m_vSigSeqEvtBrs.size();j++)
            {
              nSigSeqEvtBrs[j]=countSeqEvtBrsInEvtTr(m_vSigSeqEvtBrs[j],m_vVSigSeqIdxOfHead[j],m_vVSigSeqMidxOfHead[j],evtTr,vIdxOfHead,vMidxOfHead);
              m_vNSigSeqEvtBrs[j]=m_vNSigSeqEvtBrs[j]+nSigSeqEvtBrs[j];
              if(m_ccSwitch==true)
                {
                  nCcSigSeqEvtBrs[j]=countSeqEvtBrsInEvtTr(m_vCcSigSeqEvtBrs[j],m_vVCcSigSeqIdxOfHead[j],m_vVCcSigSeqMidxOfHead[j],evtTr,vIdxOfHead,vMidxOfHead);
                  m_vNCcSigSeqEvtBrs[j]=m_vNCcSigSeqEvtBrs[j]+nCcSigSeqEvtBrs[j];
                  nTotSigSeqEvtBrs[j]=nSigSeqEvtBrs[j]+nCcSigSeqEvtBrs[j];
                }
            }
        }

      if(m_vSigIncSeqEvtBrs.size()>0)
        {
          for(unsigned int j=0;j<m_vSigIncSeqEvtBrs.size();j++)
            {
              nSigIncSeqEvtBrs[j]=countIncSeqEvtBrsInEvtTr(m_vVIIncSigIncSeqEvtBrs[j],m_vSigIncSeqEvtBrs[j],m_vVSigIncSeqIdxOfHead[j],m_vVSigIncSeqMidxOfHead[j],evtTr,vIdxOfHead,vMidxOfHead);
              m_vNSigIncSeqEvtBrs[j]=m_vNSigIncSeqEvtBrs[j]+nSigIncSeqEvtBrs[j];
              if(m_ccSwitch==true)
                {
                  nCcSigIncSeqEvtBrs[j]=countIncSeqEvtBrsInEvtTr(m_vVIIncSigIncSeqEvtBrs[j],m_vCcSigIncSeqEvtBrs[j],m_vVCcSigIncSeqIdxOfHead[j],m_vVCcSigIncSeqMidxOfHead[j],evtTr,vIdxOfHead,vMidxOfHead);
                  m_vNCcSigIncSeqEvtBrs[j]=m_vNCcSigIncSeqEvtBrs[j]+nCcSigIncSeqEvtBrs[j];
                  nTotSigIncSeqEvtBrs[j]=nSigIncSeqEvtBrs[j]+nCcSigIncSeqEvtBrs[j];
                }
            }
        }

      if(m_vSigPFSts.size()>0)
        {
          for(unsigned int j=0;j<m_vSigPFSts.size();j++)
            {
              nSigPFSts[j]=countPFSts(vPid,vMidx,m_vSigPFSts[j]);
              m_vNSigPFSts[j]=m_vNSigPFSts[j]+nSigPFSts[j];
              if(m_ccSwitch==true)
                {
                  nCcSigPFSts[j]=countPFSts(vPid,vMidx,m_vCcSigPFSts[j]);
                  m_vNCcSigPFSts[j]=m_vNCcSigPFSts[j]+nCcSigPFSts[j];
                  nTotSigPFSts[j]=nSigPFSts[j]+nCcSigPFSts[j];
                }
            }
        }

      if(m_vSigEvtTr.size()>0)
        {
          iSigEvtTr=-1;
          iSigEvtIFSts=-1;
          if(m_ccSwitch==true)
            {
              iCcSigEvtTr=-9999;
              iCcSigEvtIFSts=-9999;
            }
          for(unsigned int j=0;j<m_vSigEvtTr.size();j++)
            {
              
              if(evtTr==m_vSigEvtTr[j])
                {
                  iSigEvtTr=j;
                  iSigEvtIFSts=m_iSigEvtTrISigEvtIFStsMap[iSigEvtTr]; 
                  m_vNSigEvtTr[iSigEvtTr]++;
                  m_vNSigEvtIFSts[iSigEvtIFSts]++;
                  if(m_ccSwitch==true)
                    {
                      if(m_vNSigEvtTr[iSigEvtTr]+m_vNCcSigEvtTr[iSigEvtTr]==1) m_iSigEvtTrIEvtTrMap[iSigEvtTr]=iEvtTr;
                      if(m_vNSigEvtIFSts[iSigEvtIFSts]+m_vNCcSigEvtIFSts[iSigEvtIFSts]==1) m_iSigEvtIFStsIEvtIFStsMap[iSigEvtIFSts]=iEvtIFSts;
                      iCcSigEvtTr=m_vICcSigEvtTr[j];
                      iCcSigEvtIFSts=m_iSigEvtTrICcSigEvtIFStsMap[iSigEvtTr];
                      if(m_vNSigEvtTr[iSigEvtTr]+m_vNCcSigEvtTr[iSigEvtTr]==1) m_iSigEvtTrICcEvtTrMap[iSigEvtTr]=iCcEvtTr;
                      if(m_vNSigEvtIFSts[iSigEvtIFSts]+m_vNCcSigEvtIFSts[iSigEvtIFSts]==1) m_iSigEvtIFStsICcEvtIFStsMap[iSigEvtIFSts]=iCcEvtIFSts;
                    }
                  else
                    {
                      if(m_vNSigEvtTr[iSigEvtTr]==1) m_iSigEvtTrIEvtTrMap[iSigEvtTr]=iEvtTr;
                      if(m_vNSigEvtIFSts[iSigEvtIFSts]==1) m_iSigEvtIFStsIEvtIFStsMap[iSigEvtIFSts]=iEvtIFSts;
                    }
                  break;
                }
              else if(m_ccSwitch==true&&m_vICcSigEvtTr[j]!=0&&evtTr==m_vCcSigEvtTr[j])
                {
                  iSigEvtTr=j;
                  iSigEvtIFSts=m_iSigEvtTrISigEvtIFStsMap[iSigEvtTr];
                  m_vNCcSigEvtTr[iSigEvtTr]++;
                  m_vNCcSigEvtIFSts[iSigEvtIFSts]++;
                  if(m_vNSigEvtTr[iSigEvtTr]+m_vNCcSigEvtTr[iSigEvtTr]==1) m_iSigEvtTrIEvtTrMap[iSigEvtTr]=iEvtTr;
                  if(m_vNSigEvtIFSts[iSigEvtIFSts]+m_vNCcSigEvtIFSts[iSigEvtIFSts]==1) m_iSigEvtIFStsIEvtIFStsMap[iSigEvtIFSts]=iEvtIFSts;
                  iCcSigEvtTr=-1;
                  iCcSigEvtIFSts=-1*m_iSigEvtTrICcSigEvtIFStsMap[iSigEvtTr];
                  if(m_vNSigEvtTr[iSigEvtTr]+m_vNCcSigEvtTr[iSigEvtTr]==1) m_iSigEvtTrICcEvtTrMap[iSigEvtTr]=-1*iCcEvtTr;
                  if(m_vNSigEvtIFSts[iSigEvtIFSts]+m_vNCcSigEvtIFSts[iSigEvtIFSts]==1) m_iSigEvtIFStsICcEvtIFStsMap[iSigEvtIFSts]=-1*iCcEvtIFSts;
                  break;
                }
            }
        }

      if(m_vSigEvtIFSts2.size()>0)
        {
          iSigEvtIFSts2=-1;
          iCcSigEvtIFSts2=-9999;
          for(unsigned int j=0;j<m_vSigEvtIFSts2.size();j++)
            {
              if(evtIFSts==m_vSigEvtIFSts2[j])
                {
                  iSigEvtIFSts2=j;
                  m_vNSigEvtIFSts2[iSigEvtIFSts2]++;
                  if(m_ccSwitch==true)
                    {
                      if(m_vNSigEvtIFSts2[iSigEvtIFSts2]+m_vNCcSigEvtIFSts2[iSigEvtIFSts2]==1) m_iSigEvtIFSts2IEvtIFStsMap[iSigEvtIFSts2]=iEvtIFSts;
                      iCcSigEvtIFSts2=m_vICcSigEvtIFSts2[iSigEvtIFSts2];
                      if(m_vNSigEvtIFSts2[j]+m_vNCcSigEvtIFSts2[j]==1) m_iSigEvtIFSts2ICcEvtIFStsMap[iSigEvtIFSts2]=iCcEvtIFSts;
                    }
                  else
                    {
                      if(m_vNSigEvtIFSts2[iSigEvtIFSts2]==1) m_iSigEvtIFSts2IEvtIFStsMap[iSigEvtIFSts2]=iEvtIFSts;
                    }
                  break;
                }
              else if(m_ccSwitch==true&&m_vICcSigEvtIFSts2[j]!=0&&evtIFSts==m_vCcSigEvtIFSts2[j])
                {
                  iSigEvtIFSts2=j;
                  m_vNCcSigEvtIFSts2[iSigEvtIFSts2]++;
                  if(m_vNSigEvtIFSts2[iSigEvtIFSts2]+m_vNCcSigEvtIFSts2[iSigEvtIFSts2]==1) m_iSigEvtIFSts2IEvtIFStsMap[iSigEvtIFSts2]=iEvtIFSts;
                  iCcSigEvtIFSts2=-1*m_vICcSigEvtIFSts2[iSigEvtIFSts2];
                  if(m_vNSigEvtIFSts2[j]+m_vNCcSigEvtIFSts2[j]==1) m_iSigEvtIFSts2ICcEvtIFStsMap[iSigEvtIFSts2]=-1*iCcEvtIFSts;
                  break;                 
                }
            }
        }

      tr->Fill();
      if((i+1>=5000)&&(((i+1)%5000==0)||((i+1)==nEvtsToBePrcsd)))
        {
          if((i+1)==5000) cout<<"Number of events processed\tNumber of seconds elapsed"<<endl<<endl;
          cout<<setiosflags(ios::right)<<setw(14)<<i+1<<"\t\t\t"<<setiosflags(ios::fixed)<<setprecision(2)<<setw(14)<<(clock()-starttime)/((double) CLOCKS_PER_SEC)<<resetiosflags(ios::adjustfield)<<endl;
          if((i+1)==nEvtsToBePrcsd) cout<<endl;
        }
      if(!m_cut.empty()&&(i+1)==nEvtsToBePrcsd) cout<<"Note that only "<<nEvtsThroughTheCut<<" events passed the cut."<<endl<<endl; 
      if((i+1)==nEvtsToBePrcsd)
        {
          for(unsigned int j=0;j<Nstars;j++) cout<<"*";
          cout<<endl<<endl;
        }
      if(((i+1)%1000000==0)||((i+1)==nEvtsToBePrcsd))
        {
          fl->Write();
          delete tr;
          fl->Close();
          delete fl;
        }
    }
  delete chn; // This statement is indispensable, or a tough problem will arise before the "return 0;" statement in the main function.

  if(m_sttaEvtTrsAndEvtIFSts==false)
    {
      if(m_ccSwitch==true)
        {
          sortBySumOf1stAnd2ndFromLrgToSml(m_vNEvtTr,m_vNCcEvtTr,m_vEvtTr,m_vCcEvtTr,m_vIEvtTr,m_vICcEvtTr);
          sortBySumOf1stAnd2ndFromLrgToSml(m_vNEvtIFSts,m_vNCcEvtIFSts,m_vEvtIFSts,m_vCcEvtIFSts,m_vIEvtIFSts,m_vICcEvtIFSts);
        }
      else
        {
          sortBy1stFromLrgToSml(m_vNEvtTr,m_vEvtTr,m_vIEvtTr);
          sortBy1stFromLrgToSml(m_vNEvtIFSts,m_vEvtIFSts,m_vIEvtIFSts);
        }
    }
  if(m_vPttaPid.size()>0)
    {
      for(unsigned int i=0;i<m_vPttaPid.size();i++)
        {
          if(m_ccSwitch==true)
            {
              sortBySumOf1stAnd2ndFromLrgToSml(m_vVNEvtBrP[i],m_vVNEvtBrCcP[i],m_vVEvtBrP[i],m_vVEvtBrCcP[i],m_vVIEvtBrP[i],m_vVIEvtBrCcP[i]);
            }
          else
            {
              sortBy1stFromLrgToSml(m_vVNEvtBrP[i],m_vVEvtBrP[i],m_vVIEvtBrP[i]);
            }
        }      
    }

  if(m_vPttaIncEvtBr.size()>0)
    {
      for(unsigned int i=0;i<m_vPttaIncEvtBr.size();i++)
        {
          if(m_ccSwitch==true)
            {
              sortBySumOf1stAnd2ndFromLrgToSml(m_vVNEvtBrIncEvtBr[i],m_vVNEvtBrCcIncEvtBr[i],m_vVEvtBrIncEvtBr[i],m_vVEvtBrCcIncEvtBr[i],m_vVIEvtBrIncEvtBr[i],m_vVIEvtBrCcIncEvtBr[i]);
            }
          else
            {
              sortBy1stFromLrgToSml(m_vVNEvtBrIncEvtBr[i],m_vVEvtBrIncEvtBr[i],m_vVIEvtBrIncEvtBr[i]);
            }
        }
    }



  if(m_vSigPid.size()>0)
    {  
      if(m_ccSwitch==true)
        {
          sortBySumOf1stAnd2ndFromLrgToSml(m_vNSigP,m_vNCcSigP,m_vSigPid,m_vCcSigPid,m_vISigP,m_vICcSigP);
        }
      else
        {
          sortBy1stFromLrgToSml(m_vNSigP,m_vSigPid,m_vISigP);
        }
    }

  if(m_vSigIncEvtBr.size()>0)
    { 
      if(m_ccSwitch==true)
        {
          sortBySumOf1stAnd2ndFromLrgToSml(m_vNSigIncEvtBr,m_vNCcSigIncEvtBr,m_vSigIncEvtBr,m_vCcSigIncEvtBr,m_vISigIncEvtBr,m_vICcSigIncEvtBr);
        }
      else
        {
          sortBy1stFromLrgToSml(m_vNSigIncEvtBr,m_vSigIncEvtBr,m_vISigIncEvtBr);
        }
    }

  if(m_vSigSeqEvtBrs.size()>0)
    {
      if(m_ccSwitch==true)
        {
          sortBySumOf1stAnd2ndFromLrgToSml(m_vNSigSeqEvtBrs,m_vNCcSigSeqEvtBrs,m_vSigSeqEvtBrs,m_vCcSigSeqEvtBrs,m_vISigSeqEvtBrs,m_vICcSigSeqEvtBrs,m_vVSigSeqIdxOfHead,m_vVCcSigSeqIdxOfHead,m_vVSigSeqMidxOfHead,m_vVCcSigSeqMidxOfHead);
        }
      else
        {
          sortBy1stFromLrgToSml(m_vNSigSeqEvtBrs,m_vSigSeqEvtBrs,m_vISigSeqEvtBrs,m_vVSigSeqIdxOfHead,m_vVSigSeqMidxOfHead);
        }
    }

  if(m_vSigIncSeqEvtBrs.size()>0)
    {
      if(m_ccSwitch==true)
        {
          sortBySumOf1stAnd2ndFromLrgToSml(m_vNSigIncSeqEvtBrs,m_vNCcSigIncSeqEvtBrs,m_vSigIncSeqEvtBrs,m_vCcSigIncSeqEvtBrs,m_vISigIncSeqEvtBrs,m_vICcSigIncSeqEvtBrs,m_vVSigIncSeqIdxOfHead,m_vVCcSigIncSeqIdxOfHead,m_vVSigIncSeqMidxOfHead,m_vVCcSigIncSeqMidxOfHead);
        }
      else
        {
          sortBy1stFromLrgToSml(m_vNSigIncSeqEvtBrs,m_vSigIncSeqEvtBrs,m_vISigIncSeqEvtBrs,m_vVSigIncSeqIdxOfHead,m_vVSigIncSeqMidxOfHead);
        }
    }

  if(m_vSigPFSts.size()>0)
    {
      if(m_ccSwitch==true)
        {
          sortBySumOf1stAnd2ndFromLrgToSml(m_vNSigPFSts,m_vNCcSigPFSts,m_vSigPFSts,m_vCcSigPFSts,m_vISigPFSts,m_vICcSigPFSts);
        }
      else
        {
          sortBy1stFromLrgToSml(m_vNSigPFSts,m_vSigPFSts,m_vISigPFSts);
        }
    }

  if(m_vSigEvtTr.size()>0)
    {
      if(m_ccSwitch==true)
        {
          sortBySumOf1stAnd2ndFromLrgToSml(m_vNSigEvtTr,m_vNCcSigEvtTr,m_vSigEvtTr,m_vCcSigEvtTr,m_vISigEvtTr,m_vICcSigEvtTr);
          sortBySumOf1stAnd2ndFromLrgToSml(m_vNSigEvtIFSts,m_vNCcSigEvtIFSts,m_vSigEvtIFSts,m_vCcSigEvtIFSts,m_vISigEvtIFSts,m_vICcSigEvtIFSts);
        }
      else
        {
          sortBy1stFromLrgToSml(m_vNSigEvtTr,m_vSigEvtTr,m_vISigEvtTr);
          sortBy1stFromLrgToSml(m_vNSigEvtIFSts,m_vSigEvtIFSts,m_vISigEvtIFSts);
        }
    }

  if(m_vSigEvtIFSts2.size()>0)
    {
      if(m_ccSwitch==true)
        {
          sortBySumOf1stAnd2ndFromLrgToSml(m_vNSigEvtIFSts2,m_vNCcSigEvtIFSts2,m_vSigEvtIFSts2,m_vCcSigEvtIFSts2,m_vISigEvtIFSts2,m_vICcSigEvtIFSts2);
        }
      else
        {      
          sortBy1stFromLrgToSml(m_vNSigEvtIFSts2,m_vSigEvtIFSts2,m_vISigEvtIFSts2);
        }
    }
}
