#include "../include/topoana.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "TChain.h"
#include "TChainElement.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

void topoana::checkInput()
{
  cout<<"The parameters set in the card file are as follows:"<<endl<<endl;

  if(m_nmsOfIptRootFls.size()==0)
    {
      cerr<<"Error: The names of input root files do not exist!"<<endl;
      exit(-1);
    }
  else
    {
      cout<<"Names of input root files:"<<endl<<endl;
      for(unsigned int i=0;i<m_nmsOfIptRootFls.size();i++)
        {
          cout<<"  "<<m_nmsOfIptRootFls[i]<<endl;
        }
      cout<<endl;

      bool hasWildcards=false;
      TString * nmOfIptRootFl;
      for(unsigned int i=0;i<m_nmsOfIptRootFls.size();i++)
        {
          nmOfIptRootFl=new TString(m_nmsOfIptRootFls[i]);
          if(nmOfIptRootFl->MaybeWildcard())
            {
              hasWildcards=true;
              delete nmOfIptRootFl;
              break;
            }
          else
            {
              delete nmOfIptRootFl;
            }
        }
      if(hasWildcards==true)
        {
          cout<<"With the wildcards parsed:"<<endl<<endl;
          TChain * chn=new TChain(m_trNm.c_str());
          for(unsigned int i=0;i<m_nmsOfIptRootFls.size();i++)
            {
              chn->Add(m_nmsOfIptRootFls[i].c_str());
            }
          TObjArray * objArray=chn->GetListOfFiles();
          TIter next(objArray);
          TChainElement * chnElmt=0;
          unsigned int nCmltFls=0;
          while((chnElmt=(TChainElement *) next()))
            {
              cout<<"  "<<chnElmt->GetTitle()<<endl;
              nCmltFls++;
            }
          if(nCmltFls>0)
            {
              cout<<endl<<"In total, "<<nCmltFls<<" files match the names listed above."<<endl<<endl;
            }
          else
            {
              cerr<<"Error: No valid input root files match the names listed above!"<<endl;
              exit(-1);
            }
        }
    }

  if(m_trNm.empty())
    {
      cerr<<"Error: The tree name is empty!"<<endl;
      exit(-1);
    }
  else
    {
      cout<<"Tree name: "<<m_trNm<<endl<<endl;
    }

  if(m_brNmOfNps.empty())
    {
      cerr<<"Error: The branch name of the number of particles is empty!"<<endl;
      exit(-1);
    }
  else
    {
      cout<<"Branch name of the number of particles: "<<m_brNmOfNps<<endl<<endl;
    }

  if(m_brNmOfPid.empty())
    {
      cerr<<"Error: The branch name of the array of particle identifications is empty!"<<endl;
      exit(-1);
    }
  else
    {
      cout<<"Branch name of the array of particle identifications: "<<m_brNmOfPid<<endl<<endl;
    }

  if(m_brNmOfMidx.empty())
    {
      cerr<<"Error: The branch name of the array of the mother indeces of particles is empty!"<<endl;
      exit(-1);
    }
  else
    {
      cout<<"Branch name of the array of the mother indeces of particles: "<<m_brNmOfMidx<<endl<<endl;
    }

  TChain * chn=new TChain(m_trNm.c_str());
  for(unsigned int i=0;i<m_nmsOfIptRootFls.size();i++)
    {
      chn->Add(m_nmsOfIptRootFls[i].c_str());
    }

  bool allIptsAreOK=true;
  TObjArray * objArray=chn->GetListOfFiles();
  TIter next(objArray);
  TChainElement * chnElmt=0;
  while((chnElmt=(TChainElement *) next()))
    {
      TFile * fl=new TFile(chnElmt->GetTitle());
      if(fl->IsZombie())
        {
          cerr<<"Error: The input root file \""<<chnElmt->GetTitle()<<"\" is zombie!"<<endl<<endl;
          allIptsAreOK=false;
        }
      else
        {
          TTree * tr=(TTree *) fl->Get(m_trNm.c_str());
          if(!tr)
            {
              cerr<<"Error: The input root file \""<<chnElmt->GetTitle()<<"\" does not contain a tree named \""<<m_trNm<<"\"!"<<endl<<endl;
              allIptsAreOK=false;
            }
          else
            {
              TBranch * br0=tr->FindBranch(m_brNmOfNps.c_str());
              if(!br0)
                {
                  cerr<<"Error: The tree \""<<m_trNm<<"\" in the input root file \""<<chnElmt->GetTitle()<<"\" does not contain a branch named \""<<m_brNmOfNps<<"\"!"<<endl<<endl;
                  allIptsAreOK=false;
                }
              TBranch * br1=tr->FindBranch(m_brNmOfPid.c_str());
              if(!br1)
                {
                  cerr<<"Error: The tree \""<<m_trNm<<"\" in the input root file \""<<chnElmt->GetTitle()<<"\" does not contain a branch named \""<<m_brNmOfPid<<"\"!"<<endl<<endl;
                  allIptsAreOK=false;
                }
              TBranch * br2=tr->FindBranch(m_brNmOfMidx.c_str());
              if(!br2)
                {
                  cerr<<"Error: The tree \""<<m_trNm<<"\" in the input root file \""<<chnElmt->GetTitle()<<"\" does not contain a branch named \""<<m_brNmOfMidx<<"\"!"<<endl<<endl;
                  allIptsAreOK=false;
                }
            }
          delete fl;
        }
    }
  if(!allIptsAreOK) exit(-1);
  delete chn;

  if(m_mainNmOfOptFls.empty())
    {
      cerr<<"Error: The main name of output files is empty!"<<endl;
      exit(-1);
    }
  else
    {
      cout<<"Main name of output files: "<<m_mainNmOfOptFls<<endl<<endl;
    }

  if(m_nEvtsMax!=ULONG_MAX)
    {
      cout<<"Maximum number of events to be processed: "<<m_nEvtsMax<<endl<<endl;
    }
  else
    {
      cout<<"Maximum number of events to be processed: ULONG_MAX (default)"<<endl<<endl;
    }

  if(m_cut.empty())
    {
      cout<<"No cut is used to select events."<<endl<<endl;
    }
  else
    {
      cout<<"Cut used to select events: "<<m_cut<<endl<<endl;
      if(m_cutMethod==true)
        {
          cout<<"The method to apply cut to array variables: True as long as one instance is true."<<endl<<endl;
        }
      else
        {
          cout<<"The method to apply cut to array variables: False as long as one instance is false (default)."<<endl<<endl;
        }
    }

  if(m_ccSwitch==true)
    {
      cout<<"Charge conjugate objects are processed together."<<endl<<endl;
    }
  else
    {
      cout<<"Charge conjugate objects are not processed together (default)."<<endl<<endl;
    }

  if(m_ignoreISR==true)
    {
      cout<<"ISR photons are ignored (default)."<<endl<<endl;
    }
  else
    {
      cout<<"ISR photons are not ignored."<<endl<<endl;
    }

  if(m_ignoreGFSR==true)
    {
      cout<<"gFSR photons are ignored (default)."<<endl<<endl;
    }
  else
    {
      cout<<"gFSR photons are not ignored."<<endl<<endl;
    }

  if(m_hHdEvtBrsMax!=UINT_MAX)
    {
      cout<<"Maximum hierarchy of heading event branches to be processed in each event: "<<m_hHdEvtBrsMax<<endl<<endl;
    }
  else
    {
      cout<<"Maximum hierarchy of heading event branches to be processed in each event: UINT_MAX (default)"<<endl<<endl;
    }

  if(m_vIdPid.size()==0)
    { 
      cout<<"No particles are specified to ignore their decay."<<endl<<endl;
    }
  else
    {
      cout<<"Ignore the decay of the following particles:"<<endl<<endl;
      for(unsigned int i=0;i<m_vIdPid.size();i++)
        {
          cout<<" ";
          writePnmFromPid(cout,"TxtPnm",m_vIdPid[i]);
          cout<<endl;
        }
      cout<<endl;

      m_vIdCcPid.clear();
      m_vIdICcPid.clear();
      for(unsigned int i=0;i<m_vIdPid.size();i++)
        {
          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<i;j++)
            {
              if(m_vIdPid[i]==m_vIdPid[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" particle is same as the "<<j+1<<ordNumSufj<<" particle!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vIdICcPid[j]!=0&&m_vIdPid[i]==m_vIdCcPid[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" particle is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" particle!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }
          if(m_ccSwitch==true)
            {
              m_vIdCcPid.push_back(getCcPid(m_vIdPid[i]));
              if(m_vIdCcPid[i]==m_vIdPid[i]) m_vIdICcPid.push_back(0);
              else m_vIdICcPid.push_back(1);
            }          
        }

      if(m_ccSwitch==true)
        {
          for(unsigned int i=0;i<m_vIdCcPid.size();i++)
            {
              if(m_vIdICcPid[i]!=0) m_vIdPid.push_back(m_vIdCcPid[i]);
            }
        }
    }

  if(m_vIddPid.size()==0)
    { 
      cout<<"No particles are specified to ignore the decay of their daughters."<<endl<<endl;
    }
  else
    {
      cout<<"Ignore the decay of the daughters of the following particles:"<<endl<<endl;
      for(unsigned int i=0;i<m_vIddPid.size();i++)
        {
          cout<<" ";
          writePnmFromPid(cout,"TxtPnm",m_vIddPid[i]);
          cout<<endl;
        }
      cout<<endl;

      m_vIddCcPid.clear();
      m_vIddICcPid.clear();
      for(unsigned int i=0;i<m_vIddPid.size();i++)
        {
          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<i;j++)
            {
              if(m_vIddPid[i]==m_vIddPid[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" particle is same as the "<<j+1<<ordNumSufj<<" particle!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vIddICcPid[j]!=0&&m_vIddPid[i]==m_vIddCcPid[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" particle is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" particle!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }
          if(m_ccSwitch==true)
            {
              m_vIddCcPid.push_back(getCcPid(m_vIddPid[i]));
              if(m_vIddCcPid[i]==m_vIddPid[i]) m_vIddICcPid.push_back(0);
              else m_vIddICcPid.push_back(1);
            }          
        }

      if(m_ccSwitch==true)
        {
          for(unsigned int i=0;i<m_vIddCcPid.size();i++)
            {
              if(m_vIddICcPid[i]!=0) m_vIddPid.push_back(m_vIddCcPid[i]);
            }
        }
    }

  if(m_nEvtTrsMax!=ULONG_MAX)
    {
      cout<<"Maximum number of event trees to be printed: "<<m_nEvtTrsMax<<endl<<endl;
    }
  else
    {
      cout<<"Maximum number of event trees to be printed: ULONG_MAX (default)"<<endl<<endl;
    }

  if(m_nEvtIFStsMax!=ULONG_MAX)
    {
      cout<<"Maximum number of event initial-final states to be printed: "<<m_nEvtIFStsMax<<endl<<endl;
    }
  else
    {
      cout<<"Maximum number of event initial-final states to be printed: ULONG_MAX (default)"<<endl<<endl;
    }

  if(m_spr1stBrsOfEvtTrs==true)
    {
      cout<<"The first branches of event trees in the output txt/tex/pdf files are suppressed."<<endl<<endl;
    }
  else
    {
      cout<<"The first branches of event trees in the output txt/tex/pdf files are not suppressed. (default)."<<endl<<endl;
    }

  if(m_sprTopoTags==true)
    {
      cout<<"The topology tags in the output tex/pdf files are suppressed."<<endl<<endl;
    }
  else
    {
      cout<<"The topology tags in the output tex/pdf files are not suppressed. (default)."<<endl<<endl;
    }

  if(m_sttaEvtTrsAndEvtIFSts==true)
    {
      cout<<"The topology analysis of event trees and event initial-final states are skipped."<<endl<<endl;
    }
  else
    {
      cout<<"The topology analysis of event trees and event initial-final states are not skipped. (default)."<<endl<<endl;
    }

  if(m_vPttaPid.size()==0)
    {
      cout<<"No particles are specified for performing the topology analysis of the event branches begun with them."<<endl<<endl;
    }
  else
    {
      cout<<"Perform the topology analysis of the event branches begun with the following particles:"<<endl<<endl;
      for(unsigned int i=0;i<m_vPttaPid.size();i++)
        {
          cout<<" ";
          writePnmFromPid(cout,"TxtPnm",m_vPttaPid[i]);
          if(m_pidTxtPnmMap[m_vPttaPid[i]].size()<=6) cout<<"\t\t\t";
          else if(m_pidTxtPnmMap[m_vPttaPid[i]].size()<=14) cout<<"\t\t";
          else if(m_pidTxtPnmMap[m_vPttaPid[i]].size()<=22) cout<<"\t";
          if(m_vNEvtBrsMax[i]!=ULONG_MAX)
            {
              cout<<"Maximum number of event branches to be printed: "<<m_vNEvtBrsMax[i]<<endl<<endl;
            }
          else
            {
              cout<<"Maximum number of event branches to be printed: ULONG_MAX (default)"<<endl<<endl;
            }
        }
      cout<<endl;

      m_vPttaCcPid.clear();
      m_vPttaICcPid.clear();
      m_vVEvtBrP.clear(); m_vVEvtBrCcP.clear();
      vector< list<int> > vEvtBrP, vEvtBrCcP;
      vEvtBrP.clear(); vEvtBrCcP.clear();
      m_vVIEvtBrP.clear(); m_vVIEvtBrCcP.clear();
      vector<int> vIEvtBrP, vIEvtBrCcP;
      vIEvtBrP.clear(); vIEvtBrCcP.clear();
      m_vVNEvtBrP.clear(); m_vVNEvtBrCcP.clear();
      vector<int> vNEvtBrP, vNEvtBrCcP;
      vNEvtBrP.clear(); vNEvtBrCcP.clear();
      for(unsigned int i=0;i<m_vPttaPid.size();i++)
        {
          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<i;j++)
            {
              if(m_vPttaPid[i]==m_vPttaPid[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" particle is same as the "<<j+1<<ordNumSufj<<" particle!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vPttaICcPid[j]!=0&&m_vPttaPid[i]==m_vPttaCcPid[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" particle is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" particle!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }

          m_vVEvtBrP.push_back(vEvtBrP);
          m_vVIEvtBrP.push_back(vIEvtBrP);
          m_vVNEvtBrP.push_back(vNEvtBrP);
          if(m_ccSwitch==true)
            {
              m_vPttaCcPid.push_back(getCcPid(m_vPttaPid[i]));
              if(m_vPttaCcPid[i]==m_vPttaPid[i]) m_vPttaICcPid.push_back(0);
              else m_vPttaICcPid.push_back(1);
              m_vVEvtBrCcP.push_back(vEvtBrCcP);
              m_vVIEvtBrCcP.push_back(vIEvtBrCcP);
              m_vVNEvtBrCcP.push_back(vNEvtBrCcP);              
            }     
        }
    }

  if(m_vVPttaPid1.size()==0)
    {
      cout<<"No inclusive event branches are specified for performing the topology analysis of the exclusive event branches matched with them."<<endl<<endl;
    }
  else
    {
      cout<<"Perform the topology analysis of the exclusive event branches matched with the following inclusive event branches:"<<endl<<endl;
      for(unsigned int i=0;i<m_vVPttaPid1.size();i++)
        {
          for(unsigned int j=0;j<m_vVPttaPid1[i].size();j++)
	    {
	      cout<<"  "<<j<<"\t";
              writePnmFromPid(cout,"TxtPnm",m_vVPttaPid1[i][j]);
              cout<<endl;
	    }
          if(m_vNEvtBrsMax1[i]!=ULONG_MAX)
            {
              cout<<"Maximum number of event branches to be printed: "<<m_vNEvtBrsMax1[i]<<endl<<endl;
            }
          else
            {
              cout<<"Maximum number of event branches to be printed: ULONG_MAX (default)"<<endl<<endl;
            }
        }
      cout<<endl;

      m_vPttaIncEvtBr.clear(); m_vPttaCcIncEvtBr.clear();
      list<int> pttaIncEvtBr, pttaCcIncEvtBr;
      m_vPttaICcIncEvtBr.clear();
      m_vVEvtBrIncEvtBr.clear(); m_vVEvtBrCcIncEvtBr.clear();
      vector< list<int> > vEvtBrIncEvtBr, vEvtBrCcIncEvtBr;
      vEvtBrIncEvtBr.clear(); vEvtBrCcIncEvtBr.clear();
      m_vVIEvtBrIncEvtBr.clear(); m_vVIEvtBrCcIncEvtBr.clear();
      vector<int> vIEvtBrIncEvtBr, vIEvtBrCcIncEvtBr;
      vIEvtBrIncEvtBr.clear(); vIEvtBrCcIncEvtBr.clear();
      m_vVNEvtBrIncEvtBr.clear(); m_vVNEvtBrCcIncEvtBr.clear();
      vector<int> vNEvtBrIncEvtBr, vNEvtBrCcIncEvtBr;
      vNEvtBrIncEvtBr.clear(); vNEvtBrCcIncEvtBr.clear();
      for(unsigned int i=0;i<m_vVPttaPid1.size();i++)
        {
          pttaIncEvtBr.clear();
          for(unsigned int j=1;j<m_vVPttaPid1[i].size();j++) pttaIncEvtBr.push_back(m_vVPttaPid1[i][j]);
          sortByPidAndPchrg(pttaIncEvtBr);
          if(m_vVPttaPid1[i][0]!=m_pidOfISt)
            {
              pttaIncEvtBr.push_front(m_vVPttaPid1[i][0]);
            }
          else
            {
              pttaIncEvtBr.push_front(11);
              pttaIncEvtBr.push_front(-11);
            }

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vPttaIncEvtBr.size();j++)
            {
              if(pttaIncEvtBr==m_vPttaIncEvtBr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" inclusive decay is same as the "<<j+1<<ordNumSufj<<" inclusive decay!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vPttaICcIncEvtBr[j]!=0&&pttaIncEvtBr==m_vPttaCcIncEvtBr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" inclusive decay is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" inclusive decay!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }

          m_vPttaIncEvtBr.push_back(pttaIncEvtBr);
          m_vVEvtBrIncEvtBr.push_back(vEvtBrIncEvtBr);
          m_vVIEvtBrIncEvtBr.push_back(vIEvtBrIncEvtBr);
          m_vVNEvtBrIncEvtBr.push_back(vNEvtBrIncEvtBr);
          if(m_ccSwitch==true)
            {
              pttaCcIncEvtBr.clear();
              list<int>::iterator liit=pttaIncEvtBr.begin();
              if((*liit)==-11) liit++;
              for(liit++;liit!=pttaIncEvtBr.end();liit++) pttaCcIncEvtBr.push_back(getCcPid((*liit)));
              sortByPidAndPchrg(pttaCcIncEvtBr);
              liit=pttaIncEvtBr.begin();
              if((*liit)!=-11)
                {
                  pttaCcIncEvtBr.push_front(getCcPid((*liit)));
                }
              else
                {
                  pttaCcIncEvtBr.push_front(11);
                  pttaCcIncEvtBr.push_front(-11);
                }
              m_vPttaCcIncEvtBr.push_back(pttaCcIncEvtBr);
              if(pttaCcIncEvtBr==pttaIncEvtBr) m_vPttaICcIncEvtBr.push_back(0);
              else m_vPttaICcIncEvtBr.push_back(1);
              m_vVEvtBrCcIncEvtBr.push_back(vEvtBrCcIncEvtBr);
              m_vVIEvtBrCcIncEvtBr.push_back(vIEvtBrCcIncEvtBr);
              m_vVNEvtBrCcIncEvtBr.push_back(vNEvtBrCcIncEvtBr);
            }
        }

      cout<<"i.e.:"<<endl<<endl;
      for(unsigned int i=0;i<m_vPttaIncEvtBr.size();i++)
        {
          pttaIncEvtBr.clear();
          pttaIncEvtBr=m_vPttaIncEvtBr[i];
          cout<<" ";
          list<int>::iterator liit=pttaIncEvtBr.begin();
          writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" -->";
          for(liit++;liit!=pttaIncEvtBr.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" + anything"<<endl;
          if(m_vNEvtBrsMax1[i]!=ULONG_MAX)
            {
              cout<<"Maximum number of event branches to be printed: "<<m_vNEvtBrsMax1[i]<<endl<<endl;
            }
          else
            {
              cout<<"Maximum number of event branches to be printed: ULONG_MAX (default)"<<endl<<endl;
            }
        }
    }

  if(m_vSigPid.size()==0)
    {
      cout<<"No signal particles are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal particles:"<<endl<<endl;
      for(unsigned int i=0;i<m_vSigPid.size();i++)
        {
          cout<<" ";
          writePnmFromPid(cout,"TxtPnm",m_vSigPid[i]);
          cout<<endl;
        }
      cout<<endl;

      m_vISigP.clear();
      m_vNSigP.clear();
      if(m_ccSwitch==true)
        {
          m_vCcSigPid.clear();
          m_vICcSigP.clear();
          m_vNCcSigP.clear();
        }
      for(unsigned int i=0;i<m_vSigPid.size();i++)
        { 
          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<i;j++)
            {
              if(m_vSigPid[i]==m_vSigPid[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal particle is same as the "<<j+1<<ordNumSufj<<" signal particle!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vICcSigP[j]!=0&&m_vSigPid[i]==m_vCcSigPid[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal particle is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal particle!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }

          m_vISigP.push_back(i);
          m_vNSigP.push_back(0);
          if(m_ccSwitch==true)
            {
              m_vCcSigPid.push_back(getCcPid(m_vSigPid[i]));
              if(m_vCcSigPid[i]==m_vSigPid[i]) m_vICcSigP.push_back(0);
              else m_vICcSigP.push_back(1);
              m_vNCcSigP.push_back(0);
            }
        }
    }

  if(m_vVSigPid1.size()==0)
    {
      cout<<"No signal inclusive event branches are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal inclusive event branches:"<<endl<<endl;
      for(unsigned int i=0;i<m_vVSigPid1.size();i++)
	{
          for(unsigned int j=0;j<m_vVSigPid1[i].size();j++)
	    {
	      cout<<"  "<<j<<"\t";
              writePnmFromPid(cout,"TxtPnm",m_vVSigPid1[i][j]);
              cout<<endl;
	    }
          cout<<endl;
	}

      m_vSigIncEvtBr.clear(); m_vCcSigIncEvtBr.clear();
      list<int> sigIncEvtBr, ccSigIncEvtBr;
      m_vISigIncEvtBr.clear(); m_vICcSigIncEvtBr.clear();
      m_vNSigIncEvtBr.clear(); m_vNCcSigIncEvtBr.clear();
      for(unsigned int i=0;i<m_vVSigPid1.size();i++)
        {
          sigIncEvtBr.clear();
          for(unsigned int j=1;j<m_vVSigPid1[i].size();j++) sigIncEvtBr.push_back(m_vVSigPid1[i][j]);
          sortByPidAndPchrg(sigIncEvtBr);
          if(m_vVSigPid1[i][0]!=m_pidOfISt)
            {
              sigIncEvtBr.push_front(m_vVSigPid1[i][0]);
            }
          else
            {
              sigIncEvtBr.push_front(11);
              sigIncEvtBr.push_front(-11);
            }

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigIncEvtBr.size();j++)
            {
              if(sigIncEvtBr==m_vSigIncEvtBr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal inclusive event branch is same as the "<<j+1<<ordNumSufj<<" signal inclusive event branch!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vICcSigIncEvtBr[j]!=0&&sigIncEvtBr==m_vCcSigIncEvtBr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal inclusive event branch is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal inclusive event branch!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }

          m_vSigIncEvtBr.push_back(sigIncEvtBr);
          m_vISigIncEvtBr.push_back(m_vSigIncEvtBr.size()-1);
          m_vNSigIncEvtBr.push_back(0);
          if(m_ccSwitch==true)
            {
              ccSigIncEvtBr.clear();
              list<int>::iterator liit=sigIncEvtBr.begin();
              if((*liit)==-11) liit++;
              for(liit++;liit!=sigIncEvtBr.end();liit++) ccSigIncEvtBr.push_back(getCcPid((*liit)));
              sortByPidAndPchrg(ccSigIncEvtBr);
              liit=sigIncEvtBr.begin();
              if((*liit)!=-11)
                {
                  ccSigIncEvtBr.push_front(getCcPid((*liit)));
                }
              else
                {
                  ccSigIncEvtBr.push_front(11);
                  ccSigIncEvtBr.push_front(-11); 
                }
              m_vCcSigIncEvtBr.push_back(ccSigIncEvtBr);
              if(ccSigIncEvtBr==sigIncEvtBr) m_vICcSigIncEvtBr.push_back(0);
              else m_vICcSigIncEvtBr.push_back(1);
              m_vNCcSigIncEvtBr.push_back(0);
            }
        }

      cout<<"i.e.:"<<endl<<endl;
      for(unsigned int i=0;i<m_vSigIncEvtBr.size();i++)
        {
          sigIncEvtBr.clear();
          sigIncEvtBr=m_vSigIncEvtBr[i];
          cout<<" ";
          list<int>::iterator liit=sigIncEvtBr.begin();
          writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" -->";
          for(liit++;liit!=sigIncEvtBr.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" + anything";
          cout<<endl<<endl;
        }
    }

  if(m_vVSigPid2.size()==0)
    {
      cout<<"No signal sequential event branches are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal sequential event branches:"<<endl<<endl;
      for(unsigned int i=0;i<m_vVSigPid2.size();i++)
	{
          for(unsigned int j=0;j<m_vVSigPid2[i].size();j++)
	    {
	      cout<<"  "<<j<<"\t";
              writePnmFromPid(cout,"TxtPnm",m_vVSigPid2[i][j]);
              if(m_pidTxtPnmMap[m_vVSigPid2[i][j]].size()<7)  cout<<"\t\t\t";
              else if(m_pidTxtPnmMap[m_vVSigPid2[i][j]].size()<15) cout<<"\t\t";
              else cout<<"\t";
              cout<<setiosflags(ios::right)<<setw(3)<<m_vVSigMidx2[i][j]<<resetiosflags(ios::adjustfield)<<endl;
	    }
          cout<<endl;
	}

      m_vSigSeqEvtBrs.clear(); m_vCcSigSeqEvtBrs.clear();
      vector< list<int> > sigSeqEvtBrs, ccSigSeqEvtBrs;
      m_vVSigSeqIdxOfHead.clear(); m_vVCcSigSeqIdxOfHead.clear();
      vector<int> vSigSeqIdxOfHead, vCcSigSeqIdxOfHead; 
      m_vVSigSeqMidxOfHead.clear(); m_vVCcSigSeqMidxOfHead.clear();
      vector<int> vSigSeqMidxOfHead, vCcSigSeqMidxOfHead;
      m_vISigSeqEvtBrs.clear(); m_vICcSigSeqEvtBrs.clear();
      m_vNSigSeqEvtBrs.clear(); m_vNCcSigSeqEvtBrs.clear();
      vector<int> vCcSigPid2, vCcSigMidx2;
      for(unsigned int i=0;i<m_vVSigPid2.size();i++)
        {
          sortPs(m_vVSigPid2[i],m_vVSigMidx2[i]);
          sigSeqEvtBrs.clear();      
          vSigSeqIdxOfHead.clear();
          vSigSeqMidxOfHead.clear();
          getEvtTr(m_vVSigPid2[i],m_vVSigMidx2[i],sigSeqEvtBrs,vSigSeqIdxOfHead,vSigSeqMidxOfHead);
          // remove the branch from e+ and e- to the first particle if all the following particles are descendant s of the first particle.
          if(sigSeqEvtBrs[0].size()==3)
          { 
            sigSeqEvtBrs.erase(sigSeqEvtBrs.begin());
            vSigSeqIdxOfHead.erase(vSigSeqIdxOfHead.begin());
            vSigSeqMidxOfHead.erase(vSigSeqMidxOfHead.begin());
          }

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigSeqEvtBrs.size();j++)
            {
              if(sigSeqEvtBrs==m_vSigSeqEvtBrs[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal sequential event branches is same as the "<<j+1<<ordNumSufj<<" signal sequential event branches!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);    
                }
              else if(m_ccSwitch==true&&m_vICcSigSeqEvtBrs[j]!=0&&sigSeqEvtBrs==m_vCcSigSeqEvtBrs[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal sequential event branches is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal sequential event branches!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }                
            }

          m_vSigSeqEvtBrs.push_back(sigSeqEvtBrs);
          m_vVSigSeqIdxOfHead.push_back(vSigSeqIdxOfHead);
          m_vVSigSeqMidxOfHead.push_back(vSigSeqMidxOfHead);
          m_vISigSeqEvtBrs.push_back(m_vSigSeqEvtBrs.size()-1);
          m_vNSigSeqEvtBrs.push_back(0);
          if(m_ccSwitch==true)
            {
              vCcSigPid2.clear();
              vCcSigMidx2.clear();
              for(unsigned int j=0;j<m_vVSigPid2[i].size();j++) vCcSigPid2.push_back(getCcPid(m_vVSigPid2[i][j])); 
              vCcSigMidx2=m_vVSigMidx2[i];
              sortPs(vCcSigPid2,vCcSigMidx2);
              ccSigSeqEvtBrs.clear();
              vCcSigSeqIdxOfHead.clear();
              vCcSigSeqMidxOfHead.clear();
              getEvtTr(vCcSigPid2,vCcSigMidx2,ccSigSeqEvtBrs,vCcSigSeqIdxOfHead,vCcSigSeqMidxOfHead);
              // remove the branch from e+ and e- to the first particle if all the following particles are descendant s of the first particle.
              if(ccSigSeqEvtBrs[0].size()==3)
              {
                ccSigSeqEvtBrs.erase(ccSigSeqEvtBrs.begin());
                vCcSigSeqIdxOfHead.erase(vCcSigSeqIdxOfHead.begin());
                vCcSigSeqMidxOfHead.erase(vCcSigSeqMidxOfHead.begin());
              }
              m_vCcSigSeqEvtBrs.push_back(ccSigSeqEvtBrs);
              m_vVCcSigSeqIdxOfHead.push_back(vCcSigSeqIdxOfHead);
              m_vVCcSigSeqMidxOfHead.push_back(vCcSigSeqMidxOfHead);
              if(ccSigSeqEvtBrs==sigSeqEvtBrs) m_vICcSigSeqEvtBrs.push_back(0);
              else m_vICcSigSeqEvtBrs.push_back(1);
              m_vNCcSigSeqEvtBrs.push_back(0);
            }          
        }

      cout<<"i.e.:"<<endl<<endl;
      list<int> sigEvtBr;
      for(unsigned int i=0;i<m_vSigSeqEvtBrs.size();i++)
        { 
          sigSeqEvtBrs.clear();
          sigSeqEvtBrs=m_vSigSeqEvtBrs[i];
          vSigSeqIdxOfHead=m_vVSigSeqIdxOfHead[i];
          for(unsigned int j=0;j<sigSeqEvtBrs.size();j++)
            {
              sigEvtBr.clear();
              sigEvtBr=sigSeqEvtBrs[j];
              cout<<" ";
              list<int>::iterator liit=sigEvtBr.begin();
              writePnmFromPid(cout,"TxtPnm",(*liit));
              if(j==0&&vSigSeqIdxOfHead[0]==-1)
              {
                liit++;
                writePnmFromPid(cout,"TxtPnm",(*liit));
              }
              cout<<" -->";
              for(liit++;liit!=sigEvtBr.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
              cout<<endl;
            }
          cout<<endl;
        }
    }

  if(m_vVSigPid12.size()==0)
    {
      cout<<"No signal inclusive sequential event branches are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal inclusive sequential event branches:"<<endl<<endl;
      for(unsigned int i=0;i<m_vVSigPid12.size();i++)
	{
          for(unsigned int j=0;j<m_vVSigPid12[i].size();j++)
	    {
	      cout<<"  "<<j<<"\t";
              writePnmFromPid(cout,"TxtPnm",m_vVSigPid12[i][j]);
              if(m_pidTxtPnmMap[m_vVSigPid12[i][j]].size()<7)  cout<<"\t\t\t";
              else if(m_pidTxtPnmMap[m_vVSigPid12[i][j]].size()<15) cout<<"\t\t";
              else cout<<"\t";
              cout<<setiosflags(ios::right)<<setw(3)<<m_vVSigMidx12[i][j]<<resetiosflags(ios::adjustfield)<<endl;
	    }
          cout<<endl;
	}

      m_vSigIncSeqEvtBrs.clear(); m_vCcSigIncSeqEvtBrs.clear();
      vector< list<int> > sigIncSeqEvtBrs, ccSigIncSeqEvtBrs;
      m_vVSigIncSeqIdxOfHead.clear(); m_vVCcSigIncSeqIdxOfHead.clear();
      vector<int> vSigIncSeqIdxOfHead, vCcSigIncSeqIdxOfHead; 
      m_vVSigIncSeqMidxOfHead.clear(); m_vVCcSigIncSeqMidxOfHead.clear();
      vector<int> vSigIncSeqMidxOfHead, vCcSigIncSeqMidxOfHead;
      m_vISigIncSeqEvtBrs.clear(); m_vICcSigIncSeqEvtBrs.clear();
      m_vNSigIncSeqEvtBrs.clear(); m_vNCcSigIncSeqEvtBrs.clear();
      vector<int> vCcSigPid12, vCcSigMidx12;
      for(unsigned int i=0;i<m_vVSigPid12.size();i++)
        {
          sortPs(m_vVSigPid12[i],m_vVSigMidx12[i]);
          sigIncSeqEvtBrs.clear();      
          vSigIncSeqIdxOfHead.clear();
          vSigIncSeqMidxOfHead.clear();
          getEvtTr(m_vVSigPid12[i],m_vVSigMidx12[i],sigIncSeqEvtBrs,vSigIncSeqIdxOfHead,vSigIncSeqMidxOfHead);
          // remove the branch from e+ and e- to the first particle if all the following particles are descendant s of the first particle.
          if(sigIncSeqEvtBrs[0].size()==3)
          { 
            sigIncSeqEvtBrs.erase(sigIncSeqEvtBrs.begin());
            vSigIncSeqIdxOfHead.erase(vSigIncSeqIdxOfHead.begin());
            vSigIncSeqMidxOfHead.erase(vSigIncSeqMidxOfHead.begin());
          }

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigIncSeqEvtBrs.size();j++)
            {
              if(sigIncSeqEvtBrs==m_vSigIncSeqEvtBrs[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal sequential event branches is same as the "<<j+1<<ordNumSufj<<" signal sequential event branches!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);    
                }
              else if(m_ccSwitch==true&&m_vICcSigIncSeqEvtBrs[j]!=0&&sigIncSeqEvtBrs==m_vCcSigIncSeqEvtBrs[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal sequential event branches is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal sequential event branches!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }                
            }

          m_vSigIncSeqEvtBrs.push_back(sigIncSeqEvtBrs);
          m_vVSigIncSeqIdxOfHead.push_back(vSigIncSeqIdxOfHead);
          m_vVSigIncSeqMidxOfHead.push_back(vSigIncSeqMidxOfHead);
          m_vISigIncSeqEvtBrs.push_back(m_vSigIncSeqEvtBrs.size()-1);
          m_vNSigIncSeqEvtBrs.push_back(0);
          if(m_ccSwitch==true)
            {
              vCcSigPid12.clear();
              vCcSigMidx12.clear();
              for(unsigned int j=0;j<m_vVSigPid12[i].size();j++) vCcSigPid12.push_back(getCcPid(m_vVSigPid12[i][j])); 
              vCcSigMidx12=m_vVSigMidx12[i];
              sortPs(vCcSigPid12,vCcSigMidx12);
              ccSigIncSeqEvtBrs.clear();
              vCcSigIncSeqIdxOfHead.clear();
              vCcSigIncSeqMidxOfHead.clear();
              getEvtTr(vCcSigPid12,vCcSigMidx12,ccSigIncSeqEvtBrs,vCcSigIncSeqIdxOfHead,vCcSigIncSeqMidxOfHead);
              // remove the branch from e+ and e- to the first particle if all the following particles are descendant s of the first particle.
              if(ccSigIncSeqEvtBrs[0].size()==3)
              {
                ccSigIncSeqEvtBrs.erase(ccSigIncSeqEvtBrs.begin());
                vCcSigIncSeqIdxOfHead.erase(vCcSigIncSeqIdxOfHead.begin());
                vCcSigIncSeqMidxOfHead.erase(vCcSigIncSeqMidxOfHead.begin());
              }
              m_vCcSigIncSeqEvtBrs.push_back(ccSigIncSeqEvtBrs);
              m_vVCcSigIncSeqIdxOfHead.push_back(vCcSigIncSeqIdxOfHead);
              m_vVCcSigIncSeqMidxOfHead.push_back(vCcSigIncSeqMidxOfHead);
              if(ccSigIncSeqEvtBrs==sigIncSeqEvtBrs) m_vICcSigIncSeqEvtBrs.push_back(0);
              else m_vICcSigIncSeqEvtBrs.push_back(1);
              m_vNCcSigIncSeqEvtBrs.push_back(0);
            }          
        }

      m_vVIIncSigIncSeqEvtBrs.clear();
      vector<int> vIIncSigIncSeqEvtBrs;
      int iIncSigIncSeqEvtBrs;
      cout<<"i.e.:"<<endl<<endl;
      list<int> sigEvtBr;
      for(unsigned int i=0;i<m_vSigIncSeqEvtBrs.size();i++)
        { 
          vIIncSigIncSeqEvtBrs.clear();
          sigIncSeqEvtBrs.clear();
          sigIncSeqEvtBrs=m_vSigIncSeqEvtBrs[i];
          vSigIncSeqIdxOfHead=m_vVSigIncSeqIdxOfHead[i];
          for(unsigned int j=0;j<sigIncSeqEvtBrs.size();j++)
            {
              iIncSigIncSeqEvtBrs=0;
              sigEvtBr.clear();
              sigEvtBr=sigIncSeqEvtBrs[j];
              cout<<" ";
              list<int>::iterator liit=sigEvtBr.begin();
              writePnmFromPid(cout,"TxtPnm",(*liit));
              if(j==0&&vSigIncSeqIdxOfHead[0]==-1)
                {
                  liit++;
                  writePnmFromPid(cout,"TxtPnm",(*liit));
                }
              cout<<" -->";
              for(liit++;liit!=sigEvtBr.end();liit++)
                {
                  if((*liit)==m_pidOfAnything) iIncSigIncSeqEvtBrs=1;
                  writePnmFromPid(cout,"TxtPnm",(*liit));
                }
              vIIncSigIncSeqEvtBrs.push_back(iIncSigIncSeqEvtBrs);
              cout<<endl;
            }
          m_vVIIncSigIncSeqEvtBrs.push_back(vIIncSigIncSeqEvtBrs);  
          cout<<endl;
        }
    }

  if(m_vVSigPid3.size()==0)
    {
      cout<<"No signal particle final states are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal particle final states:"<<endl<<endl;
      for(unsigned int i=0;i<m_vVSigPid3.size();i++)
	{
          for(unsigned int j=0;j<m_vVSigPid3[i].size();j++)
	    {
	      cout<<"  "<<j<<"\t";
              writePnmFromPid(cout,"TxtPnm",m_vVSigPid3[i][j]);
              cout<<endl;
	    }
          cout<<endl;
	}

      m_vSigPFSts.clear(); m_vCcSigPFSts.clear();
      list<int> sigPFSts, ccSigPFSts;
      m_vISigPFSts.clear(); m_vICcSigPFSts.clear();
      m_vNSigPFSts.clear(); m_vNCcSigPFSts.clear();
      for(unsigned int i=0;i<m_vVSigPid3.size();i++)
        {
          sigPFSts.clear();
          for(unsigned int j=1;j<m_vVSigPid3[i].size();j++) sigPFSts.push_back(m_vVSigPid3[i][j]);
          sortByPidAndPchrg(sigPFSts);
          sigPFSts.push_front(m_vVSigPid3[i][0]);

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigPFSts.size();j++)
            {
              if(sigPFSts==m_vSigPFSts[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal particle final state is same as the "<<j+1<<ordNumSufj<<" signal particle final state!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vICcSigPFSts[j]!=0&&sigPFSts==m_vCcSigPFSts[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal particle final state is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal particle final state!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }

          m_vSigPFSts.push_back(sigPFSts);
          m_vISigPFSts.push_back(m_vSigPFSts.size()-1);
          m_vNSigPFSts.push_back(0);
          if(m_ccSwitch==true)
            {
              ccSigPFSts.clear();
              list<int>::iterator liit=sigPFSts.begin();
              for(liit++;liit!=sigPFSts.end();liit++) ccSigPFSts.push_back(getCcPid((*liit)));
              sortByPidAndPchrg(ccSigPFSts);
              liit=sigPFSts.begin();
              ccSigPFSts.push_front(getCcPid((*liit)));
              m_vCcSigPFSts.push_back(ccSigPFSts);
              if(ccSigPFSts==sigPFSts) m_vICcSigPFSts.push_back(0);
              else m_vICcSigPFSts.push_back(1);
              m_vNCcSigPFSts.push_back(0);
            }
        }

      cout<<"i.e.:"<<endl<<endl;
      for(unsigned int i=0;i<m_vSigPFSts.size();i++)
        {
          sigPFSts.clear();
          sigPFSts=m_vSigPFSts[i];
          cout<<" ";
          list<int>::iterator liit=sigPFSts.begin();
          writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" -->";
          for(liit++;liit!=sigPFSts.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<endl<<endl;
        }
    }

  if(m_vVSigPid4.size()==0)
    {
      cout<<"No signal event trees are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal event trees:"<<endl<<endl;
      for(unsigned int i=0;i<m_vVSigPid4.size();i++)
	{
          for(unsigned int j=0;j<m_vVSigPid4[i].size();j++)
	    {
	      cout<<"  "<<j<<"\t";
              writePnmFromPid(cout,"TxtPnm",m_vVSigPid4[i][j]);
              if(m_pidTxtPnmMap[m_vVSigPid4[i][j]].size()<7)  cout<<"\t\t\t";
              else if(m_pidTxtPnmMap[m_vVSigPid4[i][j]].size()<15) cout<<"\t\t";
              else cout<<"\t";
              cout<<setiosflags(ios::right)<<setw(3)<<m_vVSigMidx4[i][j]<<resetiosflags(ios::adjustfield)<<endl;
	    }
          cout<<endl;
	}

      m_vSigEvtTr.clear(); m_vCcSigEvtTr.clear();
      vector< list<int> > sigEvtTr, ccSigEvtTr;
      m_vISigEvtTr.clear(); m_vICcSigEvtTr.clear();
      m_vNSigEvtTr.clear(); m_vNCcSigEvtTr.clear(); 
      m_vSigEvtIFSts.clear(); m_vCcSigEvtIFSts.clear();
      list<int> sigEvtIFSts, ccSigEvtIFSts;
      m_vISigEvtIFSts.clear(); m_vICcSigEvtIFSts.clear();
      m_vNSigEvtIFSts.clear(); m_vNCcSigEvtIFSts.clear();
      m_iSigEvtTrISigEvtIFStsMap.clear(); m_iSigEvtTrICcSigEvtIFStsMap.clear();
      vector<int> vCcSigPid;
      vector<int> vCcSigMidx;
      int iSigEvtIFSts, iCcSigEvtIFSts;
      for(unsigned int i=0;i<m_vVSigPid4.size();i++)
        {
          sortPs(m_vVSigPid4[i],m_vVSigMidx4[i]);
          sigEvtTr.clear();      
          getEvtTr(m_vVSigPid4[i],m_vVSigMidx4[i],sigEvtTr);
          sigEvtIFSts.clear();
          getEvtIFSts(m_vVSigPid4[i],m_vVSigMidx4[i],sigEvtIFSts);
          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigEvtTr.size();j++)
            {
              if(sigEvtTr==m_vSigEvtTr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal event tree is same as the "<<j+1<<ordNumSufj<<" signal event tree!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);    
                }
              else if(m_ccSwitch==true&&m_vICcSigEvtTr[j]!=0&&sigEvtTr==m_vCcSigEvtTr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal event tree is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal event tree!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }
          m_vSigEvtTr.push_back(sigEvtTr);
          m_vISigEvtTr.push_back(m_vSigEvtTr.size()-1);
          m_vNSigEvtTr.push_back(0);          
          if(m_ccSwitch==true)
            {
              vCcSigPid.clear();
              vCcSigMidx.clear();
              for(unsigned int j=0;j<m_vVSigPid4[i].size();j++) vCcSigPid.push_back(getCcPid(m_vVSigPid4[i][j]));
              vCcSigMidx=m_vVSigMidx4[i];
              sortPs(vCcSigPid,vCcSigMidx);
              ccSigEvtTr.clear();
              getEvtTr(vCcSigPid,vCcSigMidx,ccSigEvtTr);
              m_vCcSigEvtTr.push_back(ccSigEvtTr);
              if(ccSigEvtTr==sigEvtTr) m_vICcSigEvtTr.push_back(0);
              else m_vICcSigEvtTr.push_back(1);
              m_vNCcSigEvtTr.push_back(0);
            }

          iSigEvtIFSts=-1; // If the variable is still equal to -1 after the following loop, then the signal event initial-final states is a new signal event initial-final states.
          for(unsigned int j=0;j<m_vSigEvtIFSts.size();j++)
            {
              if(sigEvtIFSts==m_vSigEvtIFSts[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Infor: The final state of the "<<i+1<<ordNumSufi<<" signal event tree is same as that of the "<<j+1<<ordNumSufj<<" signal event tree."<<endl<<endl;
                  iSigEvtIFSts=j;
                  if(m_ccSwitch==true) iCcSigEvtIFSts=m_vICcSigEvtIFSts[j];
                  break;
                }
              else if(m_ccSwitch==true&&m_vICcSigEvtIFSts[j]!=0&&sigEvtIFSts==m_vCcSigEvtIFSts[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Infor: The final state of the "<<i+1<<ordNumSufi<<" signal event tree is same as that of the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal event tree."<<endl<<endl;
                  iSigEvtIFSts=j;
                  iCcSigEvtIFSts=-1;
                  break;
                }
            }
          if(iSigEvtIFSts==-1)
            {
              iSigEvtIFSts=m_vSigEvtIFSts.size();
              m_vSigEvtIFSts.push_back(sigEvtIFSts);
              m_vISigEvtIFSts.push_back(m_vSigEvtIFSts.size()-1);
              m_vNSigEvtIFSts.push_back(0);
              if(m_ccSwitch==true)
                {
                  ccSigEvtIFSts.clear();
                  getEvtIFSts(vCcSigPid,vCcSigMidx,ccSigEvtIFSts);
                  m_vCcSigEvtIFSts.push_back(ccSigEvtIFSts);
                  if(ccSigEvtIFSts==sigEvtIFSts) iCcSigEvtIFSts=0;
                  else iCcSigEvtIFSts=1;
                  m_vICcSigEvtIFSts.push_back(iCcSigEvtIFSts);
                  m_vNCcSigEvtIFSts.push_back(0);
                }
            }
          m_iSigEvtTrISigEvtIFStsMap[m_vSigEvtTr.size()-1]=iSigEvtIFSts;
          if(m_ccSwitch==true) m_iSigEvtTrICcSigEvtIFStsMap[m_vSigEvtTr.size()-1]=iCcSigEvtIFSts;   
        }

      cout<<"With the initial e+ e-:"<<endl<<endl;
      list<int> sigEvtBr;
      for(unsigned int i=0;i<m_vSigEvtTr.size();i++)
        { 
          sigEvtTr.clear();
          sigEvtTr=m_vSigEvtTr[i];
          for(unsigned int j=0;j<sigEvtTr.size();j++)
            {
              sigEvtBr.clear();
              sigEvtBr=sigEvtTr[j];
              cout<<" ";
              list<int>::iterator liit=sigEvtBr.begin();
              writePnmFromPid(cout,"TxtPnm",(*liit));
              // The condition "j==0" is set for the initial state particle pair e+e-;the condition "(*liit)==1, 2, 3, 4, 5 or 6" is set for the intermediate state quark pair ddbar, uubar, ssbar, ccbar, bbbar or ttbar;
              if(j==0||(*liit)==1||(*liit)==2||(*liit)==3||(*liit)==4||(*liit)==5||(*liit)==6)
                {
                  liit++;
                  writePnmFromPid(cout,"TxtPnm",(*liit));
                }
              cout<<" -->";
              for(liit++;liit!=sigEvtBr.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
              cout<<endl;
            }
          cout<<endl;
        }
    }

  if(m_vVSigPid5.size()==0)
    {
      cout<<"No signal event final states are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal event final states:"<<endl<<endl;
      for(unsigned int i=0;i<m_vVSigPid5.size();i++)
	{
          for(unsigned int j=0;j<m_vVSigPid5[i].size();j++)
	    {
	      cout<<"  "<<j<<"\t";
              writePnmFromPid(cout,"TxtPnm",m_vVSigPid5[i][j]);
              cout<<endl;
	    }
          cout<<endl;
	}

      m_vSigEvtIFSts2.clear(); m_vCcSigEvtIFSts2.clear();
      list<int> sigEvtIFSts2, ccSigEvtIFSts2;
      m_vISigEvtIFSts2.clear(); m_vICcSigEvtIFSts2.clear();
      m_vNSigEvtIFSts2.clear(); m_vNCcSigEvtIFSts2.clear();
      for(unsigned int i=0;i<m_vVSigPid5.size();i++)
        {
          sigEvtIFSts2.clear();
          for(unsigned int j=0;j<m_vVSigPid5[i].size();j++) sigEvtIFSts2.push_back(m_vVSigPid5[i][j]);
          sortByPidAndPchrg(sigEvtIFSts2);
          sigEvtIFSts2.push_front(11);
          sigEvtIFSts2.push_front(-11);

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigEvtIFSts2.size();j++)
            {
              if(sigEvtIFSts2==m_vSigEvtIFSts2[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal event final state is same as the "<<j+1<<ordNumSufj<<" signal event final state!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vICcSigEvtIFSts2[j]!=0&&sigEvtIFSts2==m_vCcSigEvtIFSts2[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal event final state is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal event final state!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }               
            }

          m_vSigEvtIFSts2.push_back(sigEvtIFSts2);
          m_vISigEvtIFSts2.push_back(m_vSigEvtIFSts2.size()-1);
          m_vNSigEvtIFSts2.push_back(0);
          if(m_ccSwitch==true)
            {
              ccSigEvtIFSts2.clear();
              list<int>::iterator liit=sigEvtIFSts2.begin();
              liit++;
              for(liit++;liit!=sigEvtIFSts2.end();liit++) ccSigEvtIFSts2.push_back(getCcPid((*liit)));
              sortByPidAndPchrg(ccSigEvtIFSts2);
              ccSigEvtIFSts2.push_front(11);
              ccSigEvtIFSts2.push_front(-11);
              m_vCcSigEvtIFSts2.push_back(ccSigEvtIFSts2);
              if(ccSigEvtIFSts2==sigEvtIFSts2) m_vICcSigEvtIFSts2.push_back(0);
              else m_vICcSigEvtIFSts2.push_back(1);
              m_vNCcSigEvtIFSts2.push_back(0);
            }
        }

      cout<<"With the initial e+ e-:"<<endl<<endl;
      for(unsigned int i=0;i<m_vSigEvtIFSts2.size();i++)
        {
          sigEvtIFSts2.clear();
          sigEvtIFSts2=m_vSigEvtIFSts2[i];
          cout<<" ";
          list<int>::iterator liit=sigEvtIFSts2.begin();
          writePnmFromPid(cout,"TxtPnm",(*liit));
          liit++;
          writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" -->";
          for(liit++;liit!=sigEvtIFSts2.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<endl<<endl;
        }
    }
}
