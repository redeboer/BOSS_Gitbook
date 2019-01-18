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

  if(m_nEtrsMax!=ULONG_MAX)
    {
      cout<<"Maximum number of entries to be processed: "<<m_nEtrsMax<<endl<<endl;
    }
  else
    {
      cout<<"Maximum number of entries to be processed: ULONG_MAX (default)"<<endl<<endl;
    }

  if(m_cut.empty())
    {
      cout<<"No cut is used to select entries."<<endl<<endl;
    }
  else
    {
      cout<<"Cut to select entries: "<<m_cut<<endl<<endl;
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

  if(m_ignoreGISR==true)
    {
      cout<<"gISR photons are ignored (default)."<<endl<<endl;
    }
  else
    {
      cout<<"gISR photons are not ignored."<<endl<<endl;
    }

  if(m_ignoreGFSR==true)
    {
      cout<<"gFSR photons are ignored (default)."<<endl<<endl;
    }
  else
    {
      cout<<"gFSR photons are not ignored."<<endl<<endl;
    }

  if(m_hHdDcyBrsMax!=UINT_MAX)
    {
      cout<<"Maximum hierarchy of heading decay branches to be processed in each event: "<<m_hHdDcyBrsMax<<endl<<endl;
    }
  else
    {
      cout<<"Maximum hierarchy of heading decay branches to be processed in each event: UINT_MAX (default)"<<endl<<endl;
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

  if(m_nDcyTrsToBePrtdMax!=ULONG_MAX)
    {
      cout<<"Maximum number of decay trees to be printed: "<<m_nDcyTrsToBePrtdMax<<endl<<endl;
    }
  else
    {
      cout<<"Maximum number of decay trees to be printed: ULONG_MAX (default)"<<endl<<endl;
    }

  if(m_nDcyFStsToBePrtdMax!=ULONG_MAX)
    {
      cout<<"Maximum number of decay final states to be printed: "<<m_nDcyFStsToBePrtdMax<<endl<<endl;
    }
  else
    {
      cout<<"Maximum number of decay final states to be printed: ULONG_MAX (default)"<<endl<<endl;
    }

  if(m_spr1stBrsOfDcyTrs==true)
    {
      cout<<"The first branches of decay trees in the output txt/tex/pdf files are suppressed."<<endl<<endl;
    }
  else
    {
      cout<<"The first branches of decay trees in the output txt/tex/pdf files are not suppressed. (default)."<<endl<<endl;
    }

  if(m_sprTopoTags==true)
    {
      cout<<"The topology tags in the output tex/pdf files are suppressed."<<endl<<endl;
    }
  else
    {
      cout<<"The topology tags in the output tex/pdf files are not suppressed. (default)."<<endl<<endl;
    }

  if(m_nEtrsMaxInASngOptRootFl!=ULONG_MAX)
    { 
      cout<<"Maximum number of entries to be saved in a single output root file:"<<m_nEtrsMaxInASngOptRootFl<<endl<<endl;
    }
  else
    { 
      cout<<"Upper limit of the total bytes of the cloned TTree object of "<<m_trNm<<": 3 GB in memory (default)."<<endl<<endl;
    }

  if(m_nDcyTrsToBeAnlzdMax!=ULONG_MAX)
    {
      cout<<"Maximum number of decay trees to be analyzed: "<<m_nDcyTrsToBeAnlzdMax<<endl<<endl;
      if(m_nEtrsOfCtrSmpMax!=100000)
        {
          cout<<"Maximum number of entries in the control sample: "<<m_nEtrsOfCtrSmpMax<<endl<<endl;  
        }
      else
        {
          cout<<"Maximum number of entries in the control sample: 100000 (default)."<<endl<<endl;
        }
    }
  else
    {
      cout<<"Maximum number of decay trees to be analyzed: ULONG_MAX (default)"<<endl<<endl;
    }

  if(m_sttaDcyTrsAndDcyFSts==true)
    {
      cout<<"The topology analysis of decay trees and decay final states are skipped."<<endl<<endl;
    }
  else
    {
      cout<<"The topology analysis of decay trees and decay final states are not skipped. (default)."<<endl<<endl;
    }

  if(m_vPttaPid.size()==0)
    {
      cout<<"No particles are specified for performing the topology analysis of the decay branches begun with them."<<endl<<endl;
    }
  else
    {
      cout<<"Perform the topology analysis of the decay branches begun with the following particles:"<<endl<<endl;
      for(unsigned int i=0;i<m_vPttaPid.size();i++)
        {
          cout<<" ";
          writePnmFromPid(cout,"TxtPnm",m_vPttaPid[i]);
          if(m_pidTxtPnmMap[m_vPttaPid[i]].size()<=6) cout<<"\t\t\t";
          else if(m_pidTxtPnmMap[m_vPttaPid[i]].size()<=14) cout<<"\t\t";
          else if(m_pidTxtPnmMap[m_vPttaPid[i]].size()<=22) cout<<"\t";
          if(m_vNDcyBrsMax[i]!=ULONG_MAX)
            {
              cout<<"Maximum number of decay branches to be printed: "<<m_vNDcyBrsMax[i]<<endl<<endl;
            }
          else
            {
              cout<<"Maximum number of decay branches to be printed: ULONG_MAX (default)"<<endl<<endl;
            }
        }
      cout<<endl;

      m_vPttaCcPid.clear();
      m_vPttaICcPid.clear();
      m_vVDcyBrP.clear(); m_vVDcyBrCcP.clear();
      vector< list<int> > vDcyBrP, vDcyBrCcP;
      vDcyBrP.clear(); vDcyBrCcP.clear();
      m_vVIDcyBrP.clear(); m_vVIDcyBrCcP.clear();
      vector<int> vIDcyBrP, vIDcyBrCcP;
      vIDcyBrP.clear(); vIDcyBrCcP.clear();
      m_vVNDcyBrP.clear(); m_vVNDcyBrCcP.clear();
      vector<int> vNDcyBrP, vNDcyBrCcP;
      vNDcyBrP.clear(); vNDcyBrCcP.clear();
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

          m_vVDcyBrP.push_back(vDcyBrP);
          m_vVIDcyBrP.push_back(vIDcyBrP);
          m_vVNDcyBrP.push_back(vNDcyBrP);
          if(m_ccSwitch==true)
            {
              m_vPttaCcPid.push_back(getCcPid(m_vPttaPid[i]));
              if(m_vPttaCcPid[i]==m_vPttaPid[i]) m_vPttaICcPid.push_back(0);
              else m_vPttaICcPid.push_back(1);
              m_vVDcyBrCcP.push_back(vDcyBrCcP);
              m_vVIDcyBrCcP.push_back(vIDcyBrCcP);
              m_vVNDcyBrCcP.push_back(vNDcyBrCcP);              
            }     
        }
    }

  if(m_vVPttaPid1.size()==0)
    {
      cout<<"No inclusive decay branches are specified for performing the topology analysis of the exclusive decay branches matched with them."<<endl<<endl;
    }
  else
    {
      cout<<"Perform the topology analysis of the exclusive decay branches matched with the following inclusive decay branches:"<<endl<<endl;
      for(unsigned int i=0;i<m_vVPttaPid1.size();i++)
        {
          for(unsigned int j=0;j<m_vVPttaPid1[i].size();j++)
	    {
	      cout<<"  "<<j<<"\t";
              writePnmFromPid(cout,"TxtPnm",m_vVPttaPid1[i][j]);
              cout<<endl;
	    }
          if(m_vNDcyBrsMax1[i]!=ULONG_MAX)
            {
              cout<<"Maximum number of decay branches to be printed: "<<m_vNDcyBrsMax1[i]<<endl<<endl;
            }
          else
            {
              cout<<"Maximum number of decay branches to be printed: ULONG_MAX (default)"<<endl<<endl;
            }
        }
      cout<<endl;

      m_vPttaIncDcyBr.clear(); m_vPttaCcIncDcyBr.clear();
      list<int> pttaIncDcyBr, pttaCcIncDcyBr;
      m_vPttaICcIncDcyBr.clear();
      m_vVDcyBrIncDcyBr.clear(); m_vVDcyBrCcIncDcyBr.clear();
      vector< list<int> > vDcyBrIncDcyBr, vDcyBrCcIncDcyBr;
      vDcyBrIncDcyBr.clear(); vDcyBrCcIncDcyBr.clear();
      m_vVIDcyBrIncDcyBr.clear(); m_vVIDcyBrCcIncDcyBr.clear();
      vector<int> vIDcyBrIncDcyBr, vIDcyBrCcIncDcyBr;
      vIDcyBrIncDcyBr.clear(); vIDcyBrCcIncDcyBr.clear();
      m_vVNDcyBrIncDcyBr.clear(); m_vVNDcyBrCcIncDcyBr.clear();
      vector<int> vNDcyBrIncDcyBr, vNDcyBrCcIncDcyBr;
      vNDcyBrIncDcyBr.clear(); vNDcyBrCcIncDcyBr.clear();
      for(unsigned int i=0;i<m_vVPttaPid1.size();i++)
        {
          pttaIncDcyBr.clear();
          for(unsigned int j=1;j<m_vVPttaPid1[i].size();j++) pttaIncDcyBr.push_back(m_vVPttaPid1[i][j]);
          sortByPidAndPchrg(pttaIncDcyBr);
          if(m_vVPttaPid1[i][0]!=m_pidOfISt)
            {
              pttaIncDcyBr.push_front(m_vVPttaPid1[i][0]);
            }
          else
            {
              pttaIncDcyBr.push_front(11);
              pttaIncDcyBr.push_front(-11);
            }

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vPttaIncDcyBr.size();j++)
            {
              if(pttaIncDcyBr==m_vPttaIncDcyBr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" inclusive decay is same as the "<<j+1<<ordNumSufj<<" inclusive decay!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vPttaICcIncDcyBr[j]!=0&&pttaIncDcyBr==m_vPttaCcIncDcyBr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" inclusive decay is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" inclusive decay!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }

          m_vPttaIncDcyBr.push_back(pttaIncDcyBr);
          m_vVDcyBrIncDcyBr.push_back(vDcyBrIncDcyBr);
          m_vVIDcyBrIncDcyBr.push_back(vIDcyBrIncDcyBr);
          m_vVNDcyBrIncDcyBr.push_back(vNDcyBrIncDcyBr);
          if(m_ccSwitch==true)
            {
              pttaCcIncDcyBr.clear();
              list<int>::iterator liit=pttaIncDcyBr.begin();
              if((*liit)==-11) liit++;
              for(liit++;liit!=pttaIncDcyBr.end();liit++) pttaCcIncDcyBr.push_back(getCcPid((*liit)));
              sortByPidAndPchrg(pttaCcIncDcyBr);
              liit=pttaIncDcyBr.begin();
              if((*liit)!=-11)
                {
                  pttaCcIncDcyBr.push_front(getCcPid((*liit)));
                }
              else
                {
                  pttaCcIncDcyBr.push_front(11);
                  pttaCcIncDcyBr.push_front(-11);
                }
              m_vPttaCcIncDcyBr.push_back(pttaCcIncDcyBr);
              if(pttaCcIncDcyBr==pttaIncDcyBr) m_vPttaICcIncDcyBr.push_back(0);
              else m_vPttaICcIncDcyBr.push_back(1);
              m_vVDcyBrCcIncDcyBr.push_back(vDcyBrCcIncDcyBr);
              m_vVIDcyBrCcIncDcyBr.push_back(vIDcyBrCcIncDcyBr);
              m_vVNDcyBrCcIncDcyBr.push_back(vNDcyBrCcIncDcyBr);
            }
        }

      cout<<"i.e.:"<<endl<<endl;
      for(unsigned int i=0;i<m_vPttaIncDcyBr.size();i++)
        {
          pttaIncDcyBr.clear();
          pttaIncDcyBr=m_vPttaIncDcyBr[i];
          cout<<" ";
          list<int>::iterator liit=pttaIncDcyBr.begin();
          writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" -->";
          for(liit++;liit!=pttaIncDcyBr.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" + anything"<<endl;
          if(m_vNDcyBrsMax1[i]!=ULONG_MAX)
            {
              cout<<"Maximum number of decay branches to be printed: "<<m_vNDcyBrsMax1[i]<<endl<<endl;
            }
          else
            {
              cout<<"Maximum number of decay branches to be printed: ULONG_MAX (default)"<<endl<<endl;
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
      cout<<"No signal inclusive decay branches are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal inclusive decay branches:"<<endl<<endl;
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

      m_vSigIncDcyBr.clear(); m_vCcSigIncDcyBr.clear();
      list<int> sigIncDcyBr, ccSigIncDcyBr;
      m_vISigIncDcyBr.clear(); m_vICcSigIncDcyBr.clear();
      m_vNSigIncDcyBr.clear(); m_vNCcSigIncDcyBr.clear();
      for(unsigned int i=0;i<m_vVSigPid1.size();i++)
        {
          sigIncDcyBr.clear();
          for(unsigned int j=1;j<m_vVSigPid1[i].size();j++) sigIncDcyBr.push_back(m_vVSigPid1[i][j]);
          sortByPidAndPchrg(sigIncDcyBr);
          if(m_vVSigPid1[i][0]!=m_pidOfISt)
            {
              sigIncDcyBr.push_front(m_vVSigPid1[i][0]);
            }
          else
            {
              sigIncDcyBr.push_front(11);
              sigIncDcyBr.push_front(-11);
            }

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigIncDcyBr.size();j++)
            {
              if(sigIncDcyBr==m_vSigIncDcyBr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal inclusive decay branch is same as the "<<j+1<<ordNumSufj<<" signal inclusive decay branch!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vICcSigIncDcyBr[j]!=0&&sigIncDcyBr==m_vCcSigIncDcyBr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal inclusive decay branch is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal inclusive decay branch!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }

          m_vSigIncDcyBr.push_back(sigIncDcyBr);
          m_vISigIncDcyBr.push_back(m_vSigIncDcyBr.size()-1);
          m_vNSigIncDcyBr.push_back(0);
          if(m_ccSwitch==true)
            {
              ccSigIncDcyBr.clear();
              list<int>::iterator liit=sigIncDcyBr.begin();
              if((*liit)==-11) liit++;
              for(liit++;liit!=sigIncDcyBr.end();liit++) ccSigIncDcyBr.push_back(getCcPid((*liit)));
              sortByPidAndPchrg(ccSigIncDcyBr);
              liit=sigIncDcyBr.begin();
              if((*liit)!=-11)
                {
                  ccSigIncDcyBr.push_front(getCcPid((*liit)));
                }
              else
                {
                  ccSigIncDcyBr.push_front(11);
                  ccSigIncDcyBr.push_front(-11); 
                }
              m_vCcSigIncDcyBr.push_back(ccSigIncDcyBr);
              if(ccSigIncDcyBr==sigIncDcyBr) m_vICcSigIncDcyBr.push_back(0);
              else m_vICcSigIncDcyBr.push_back(1);
              m_vNCcSigIncDcyBr.push_back(0);
            }
        }

      cout<<"i.e.:"<<endl<<endl;
      for(unsigned int i=0;i<m_vSigIncDcyBr.size();i++)
        {
          sigIncDcyBr.clear();
          sigIncDcyBr=m_vSigIncDcyBr[i];
          cout<<" ";
          list<int>::iterator liit=sigIncDcyBr.begin();
          writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" -->";
          for(liit++;liit!=sigIncDcyBr.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" + anything";
          cout<<endl<<endl;
        }
    }

  if(m_vVSigPid2.size()==0)
    {
      cout<<"No signal sequential decay branches are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal sequential decay branches:"<<endl<<endl;
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

      m_vSigSeqDcyBrs.clear(); m_vCcSigSeqDcyBrs.clear();
      vector< list<int> > sigSeqDcyBrs, ccSigSeqDcyBrs;
      m_vVSigSeqDcyBrsIdxOfHead.clear(); m_vVCcSigSeqDcyBrsIdxOfHead.clear();
      vector<int> vSigSeqDcyBrsIdxOfHead, vCcSigSeqDcyBrsIdxOfHead; 
      m_vVSigSeqDcyBrsMidxOfHead.clear(); m_vVCcSigSeqDcyBrsMidxOfHead.clear();
      vector<int> vSigSeqDcyBrsMidxOfHead, vCcSigSeqDcyBrsMidxOfHead;
      m_vISigSeqDcyBrs.clear(); m_vICcSigSeqDcyBrs.clear();
      m_vNSigSeqDcyBrs.clear(); m_vNCcSigSeqDcyBrs.clear();
      vector<int> vCcSigPid2, vCcSigMidx2;
      for(unsigned int i=0;i<m_vVSigPid2.size();i++)
        {
          sortPs(m_vVSigPid2[i],m_vVSigMidx2[i]);
          sigSeqDcyBrs.clear();      
          vSigSeqDcyBrsIdxOfHead.clear();
          vSigSeqDcyBrsMidxOfHead.clear();
          getDcyTr(m_vVSigPid2[i],m_vVSigMidx2[i],sigSeqDcyBrs,vSigSeqDcyBrsIdxOfHead,vSigSeqDcyBrsMidxOfHead);
          // remove the branch from e+ and e- to the first particle if all the following particles are descendant s of the first particle.
          if(sigSeqDcyBrs[0].size()==3)
          { 
            sigSeqDcyBrs.erase(sigSeqDcyBrs.begin());
            vSigSeqDcyBrsIdxOfHead.erase(vSigSeqDcyBrsIdxOfHead.begin());
            vSigSeqDcyBrsMidxOfHead.erase(vSigSeqDcyBrsMidxOfHead.begin());
          }

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigSeqDcyBrs.size();j++)
            {
              if(sigSeqDcyBrs==m_vSigSeqDcyBrs[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal sequential decay branches is same as the "<<j+1<<ordNumSufj<<" signal sequential decay branches!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);    
                }
              else if(m_ccSwitch==true&&m_vICcSigSeqDcyBrs[j]!=0&&sigSeqDcyBrs==m_vCcSigSeqDcyBrs[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal sequential decay branches is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal sequential decay branches!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }                
            }

          m_vSigSeqDcyBrs.push_back(sigSeqDcyBrs);
          m_vVSigSeqDcyBrsIdxOfHead.push_back(vSigSeqDcyBrsIdxOfHead);
          m_vVSigSeqDcyBrsMidxOfHead.push_back(vSigSeqDcyBrsMidxOfHead);
          m_vISigSeqDcyBrs.push_back(m_vSigSeqDcyBrs.size()-1);
          m_vNSigSeqDcyBrs.push_back(0);
          if(m_ccSwitch==true)
            {
              vCcSigPid2.clear();
              vCcSigMidx2.clear();
              for(unsigned int j=0;j<m_vVSigPid2[i].size();j++) vCcSigPid2.push_back(getCcPid(m_vVSigPid2[i][j])); 
              vCcSigMidx2=m_vVSigMidx2[i];
              sortPs(vCcSigPid2,vCcSigMidx2);
              ccSigSeqDcyBrs.clear();
              vCcSigSeqDcyBrsIdxOfHead.clear();
              vCcSigSeqDcyBrsMidxOfHead.clear();
              getDcyTr(vCcSigPid2,vCcSigMidx2,ccSigSeqDcyBrs,vCcSigSeqDcyBrsIdxOfHead,vCcSigSeqDcyBrsMidxOfHead);
              // remove the branch from e+ and e- to the first particle if all the following particles are descendant s of the first particle.
              if(ccSigSeqDcyBrs[0].size()==3)
              {
                ccSigSeqDcyBrs.erase(ccSigSeqDcyBrs.begin());
                vCcSigSeqDcyBrsIdxOfHead.erase(vCcSigSeqDcyBrsIdxOfHead.begin());
                vCcSigSeqDcyBrsMidxOfHead.erase(vCcSigSeqDcyBrsMidxOfHead.begin());
              }
              m_vCcSigSeqDcyBrs.push_back(ccSigSeqDcyBrs);
              m_vVCcSigSeqDcyBrsIdxOfHead.push_back(vCcSigSeqDcyBrsIdxOfHead);
              m_vVCcSigSeqDcyBrsMidxOfHead.push_back(vCcSigSeqDcyBrsMidxOfHead);
              if(ccSigSeqDcyBrs==sigSeqDcyBrs) m_vICcSigSeqDcyBrs.push_back(0);
              else m_vICcSigSeqDcyBrs.push_back(1);
              m_vNCcSigSeqDcyBrs.push_back(0);
            }          
        }

      cout<<"i.e.:"<<endl<<endl;
      list<int> sigDcyBr;
      for(unsigned int i=0;i<m_vSigSeqDcyBrs.size();i++)
        { 
          sigSeqDcyBrs.clear();
          sigSeqDcyBrs=m_vSigSeqDcyBrs[i];
          vSigSeqDcyBrsIdxOfHead=m_vVSigSeqDcyBrsIdxOfHead[i];
          for(unsigned int j=0;j<sigSeqDcyBrs.size();j++)
            {
              sigDcyBr.clear();
              sigDcyBr=sigSeqDcyBrs[j];
              cout<<" ";
              list<int>::iterator liit=sigDcyBr.begin();
              writePnmFromPid(cout,"TxtPnm",(*liit));
              if(j==0&&vSigSeqDcyBrsIdxOfHead[0]==-1)
              {
                liit++;
                writePnmFromPid(cout,"TxtPnm",(*liit));
              }
              cout<<" -->";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
              cout<<endl;
            }
          cout<<endl;
        }
    }

  if(m_vVSigPid12.size()==0)
    {
      cout<<"No signal inclusive sequential decay branches are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal inclusive sequential decay branches:"<<endl<<endl;
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

      m_vSigIncSeqDcyBrs.clear(); m_vCcSigIncSeqDcyBrs.clear();
      vector< list<int> > sigIncSeqDcyBrs, ccSigIncSeqDcyBrs;
      m_vVSigIncSeqDcyBrsIdxOfHead.clear(); m_vVCcSigIncSeqDcyBrsIdxOfHead.clear();
      vector<int> vSigIncSeqDcyBrsIdxOfHead, vCcSigIncSeqDcyBrsIdxOfHead; 
      m_vVSigIncSeqDcyBrsMidxOfHead.clear(); m_vVCcSigIncSeqDcyBrsMidxOfHead.clear();
      vector<int> vSigIncSeqDcyBrsMidxOfHead, vCcSigIncSeqDcyBrsMidxOfHead;
      m_vVIIncSigIncSeqDcyBrs.clear();
      vector<int> vIIncSigIncSeqDcyBrs;
      m_vISigIncSeqDcyBrs.clear(); m_vICcSigIncSeqDcyBrs.clear();
      m_vNSigIncSeqDcyBrs.clear(); m_vNCcSigIncSeqDcyBrs.clear();
      vector<int> vCcSigPid12, vCcSigMidx12;
      for(unsigned int i=0;i<m_vVSigPid12.size();i++)
        {
          sortPs(m_vVSigPid12[i],m_vVSigMidx12[i]);
          sigIncSeqDcyBrs.clear();      
          vSigIncSeqDcyBrsIdxOfHead.clear();
          vSigIncSeqDcyBrsMidxOfHead.clear();
          vIIncSigIncSeqDcyBrs.clear(); 
          getDcyTr(m_vVSigPid12[i],m_vVSigMidx12[i],sigIncSeqDcyBrs,vSigIncSeqDcyBrsIdxOfHead,vSigIncSeqDcyBrsMidxOfHead,vIIncSigIncSeqDcyBrs);
          // remove the branch from e+ and e- to the first particle if all the following particles are descendant s of the first particle.
          if(sigIncSeqDcyBrs[0].size()==3)
          { 
            sigIncSeqDcyBrs.erase(sigIncSeqDcyBrs.begin());
            vSigIncSeqDcyBrsIdxOfHead.erase(vSigIncSeqDcyBrsIdxOfHead.begin());
            vSigIncSeqDcyBrsMidxOfHead.erase(vSigIncSeqDcyBrsMidxOfHead.begin());
            vIIncSigIncSeqDcyBrs.erase(vIIncSigIncSeqDcyBrs.begin());
          }

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigIncSeqDcyBrs.size();j++)
            {
              if(sigIncSeqDcyBrs==m_vSigIncSeqDcyBrs[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal inclusive sequential decay branches is same as the "<<j+1<<ordNumSufj<<" signal inclusive sequential decay branches!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);    
                }
              else if(m_ccSwitch==true&&m_vICcSigIncSeqDcyBrs[j]!=0&&sigIncSeqDcyBrs==m_vCcSigIncSeqDcyBrs[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal inclusive sequential decay branches is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal inclusive sequential decay branches!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }                
            }

          m_vSigIncSeqDcyBrs.push_back(sigIncSeqDcyBrs);
          m_vVSigIncSeqDcyBrsIdxOfHead.push_back(vSigIncSeqDcyBrsIdxOfHead);
          m_vVSigIncSeqDcyBrsMidxOfHead.push_back(vSigIncSeqDcyBrsMidxOfHead);
          m_vVIIncSigIncSeqDcyBrs.push_back(vIIncSigIncSeqDcyBrs);  
          m_vISigIncSeqDcyBrs.push_back(m_vSigIncSeqDcyBrs.size()-1);
          m_vNSigIncSeqDcyBrs.push_back(0);
          if(m_ccSwitch==true)
            {
              vCcSigPid12.clear();
              vCcSigMidx12.clear();
              for(unsigned int j=0;j<m_vVSigPid12[i].size();j++) vCcSigPid12.push_back(getCcPid(m_vVSigPid12[i][j])); 
              vCcSigMidx12=m_vVSigMidx12[i];
              sortPs(vCcSigPid12,vCcSigMidx12);
              ccSigIncSeqDcyBrs.clear();
              vCcSigIncSeqDcyBrsIdxOfHead.clear();
              vCcSigIncSeqDcyBrsMidxOfHead.clear();
              getDcyTr(vCcSigPid12,vCcSigMidx12,ccSigIncSeqDcyBrs,vCcSigIncSeqDcyBrsIdxOfHead,vCcSigIncSeqDcyBrsMidxOfHead);
              // remove the branch from e+ and e- to the first particle if all the following particles are descendant s of the first particle.
              if(ccSigIncSeqDcyBrs[0].size()==3)
              {
                ccSigIncSeqDcyBrs.erase(ccSigIncSeqDcyBrs.begin());
                vCcSigIncSeqDcyBrsIdxOfHead.erase(vCcSigIncSeqDcyBrsIdxOfHead.begin());
                vCcSigIncSeqDcyBrsMidxOfHead.erase(vCcSigIncSeqDcyBrsMidxOfHead.begin());
              }
              m_vCcSigIncSeqDcyBrs.push_back(ccSigIncSeqDcyBrs);
              m_vVCcSigIncSeqDcyBrsIdxOfHead.push_back(vCcSigIncSeqDcyBrsIdxOfHead);
              m_vVCcSigIncSeqDcyBrsMidxOfHead.push_back(vCcSigIncSeqDcyBrsMidxOfHead);
              if(ccSigIncSeqDcyBrs==sigIncSeqDcyBrs) m_vICcSigIncSeqDcyBrs.push_back(0);
              else m_vICcSigIncSeqDcyBrs.push_back(1);
              m_vNCcSigIncSeqDcyBrs.push_back(0);
            }          
        }

      cout<<"i.e.:"<<endl<<endl;
      list<int> sigDcyBr;
      for(unsigned int i=0;i<m_vSigIncSeqDcyBrs.size();i++)
        { 
          sigIncSeqDcyBrs.clear();
          sigIncSeqDcyBrs=m_vSigIncSeqDcyBrs[i];
          vSigIncSeqDcyBrsIdxOfHead=m_vVSigIncSeqDcyBrsIdxOfHead[i];
          for(unsigned int j=0;j<sigIncSeqDcyBrs.size();j++)
            {
              sigDcyBr.clear();
              sigDcyBr=sigIncSeqDcyBrs[j];
              cout<<" ";
              list<int>::iterator liit=sigDcyBr.begin();
              writePnmFromPid(cout,"TxtPnm",(*liit));
              if(j==0&&vSigIncSeqDcyBrsIdxOfHead[0]==-1)
                {
                  liit++;
                  writePnmFromPid(cout,"TxtPnm",(*liit));
                }
              cout<<" -->";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
              if(m_vVIIncSigIncSeqDcyBrs[i][j]==1)
                {
                  cout<<" +";
                  writePnmFromPid(cout,"TxtPnm",m_pidOfAnything);
                }
              cout<<endl;
            }
          cout<<endl;
        }
    }

  if(m_vVSigPid3.size()==0)
    {
      cout<<"No signal intermediate-resonance-allowed decay branches are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal intermediate-resonance-allowed decay branches:"<<endl<<endl;
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

      m_vSigIRADcyBr.clear(); m_vCcSigIRADcyBr.clear();
      list<int> sigIRADcyBr, ccSigIRADcyBr;
      m_vISigIRADcyBr.clear(); m_vICcSigIRADcyBr.clear();
      m_vNSigIRADcyBr.clear(); m_vNCcSigIRADcyBr.clear();
      for(unsigned int i=0;i<m_vVSigPid3.size();i++)
        {
          sigIRADcyBr.clear();
          for(unsigned int j=1;j<m_vVSigPid3[i].size();j++) sigIRADcyBr.push_back(m_vVSigPid3[i][j]);
          sortByPidAndPchrg(sigIRADcyBr);
          sigIRADcyBr.push_front(m_vVSigPid3[i][0]);

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigIRADcyBr.size();j++)
            {
              if(sigIRADcyBr==m_vSigIRADcyBr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal intermediate-resonance-allowed decay branch is same as the "<<j+1<<ordNumSufj<<" signal intermediate-resonance-allowed decay branch!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vICcSigIRADcyBr[j]!=0&&sigIRADcyBr==m_vCcSigIRADcyBr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal intermediate-resonance-allowed decay branch is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal intermediate-resonance-allowed decay branch!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }

          m_vSigIRADcyBr.push_back(sigIRADcyBr);
          m_vISigIRADcyBr.push_back(m_vSigIRADcyBr.size()-1);
          m_vNSigIRADcyBr.push_back(0);
          if(m_ccSwitch==true)
            {
              ccSigIRADcyBr.clear();
              list<int>::iterator liit=sigIRADcyBr.begin();
              for(liit++;liit!=sigIRADcyBr.end();liit++) ccSigIRADcyBr.push_back(getCcPid((*liit)));
              sortByPidAndPchrg(ccSigIRADcyBr);
              liit=sigIRADcyBr.begin();
              ccSigIRADcyBr.push_front(getCcPid((*liit)));
              m_vCcSigIRADcyBr.push_back(ccSigIRADcyBr);
              if(ccSigIRADcyBr==sigIRADcyBr) m_vICcSigIRADcyBr.push_back(0);
              else m_vICcSigIRADcyBr.push_back(1);
              m_vNCcSigIRADcyBr.push_back(0);
            }
        }

      cout<<"i.e.:"<<endl<<endl;
      for(unsigned int i=0;i<m_vSigIRADcyBr.size();i++)
        {
          sigIRADcyBr.clear();
          sigIRADcyBr=m_vSigIRADcyBr[i];
          cout<<" ";
          list<int>::iterator liit=sigIRADcyBr.begin();
          writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" -->";
          for(liit++;liit!=sigIRADcyBr.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<endl<<endl;
        }
    }

  if(m_vVSigPid123.size()==0)
    {
      cout<<"No signal inclusive or intermediate-resonance-allowed sequential decay branches are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal inclusive or intermediate-resonance-allowed sequential decay branches:"<<endl<<endl;
      for(unsigned int i=0;i<m_vVSigPid123.size();i++)
	{
          for(unsigned int j=0;j<m_vVSigPid123[i].size();j++)
	    {
	      cout<<"  "<<j<<"\t";
              writePnmFromPid(cout,"TxtPnm",m_vVSigPid123[i][j]);
              if(m_pidTxtPnmMap[m_vVSigPid123[i][j]].size()<7)  cout<<"\t\t\t";
              else if(m_pidTxtPnmMap[m_vVSigPid123[i][j]].size()<15) cout<<"\t\t";
              else cout<<"\t";
              cout<<setiosflags(ios::right)<<setw(3)<<m_vVSigMidx123[i][j]<<resetiosflags(ios::adjustfield)<<endl;
	    }
          cout<<endl;
	}

      m_vSigIncOrIRASeqDcyBrs.clear(); m_vCcSigIncOrIRASeqDcyBrs.clear();
      vector< list<int> > sigIncOrIRASeqDcyBrs, ccSigIncOrIRASeqDcyBrs;
      m_vVSigIncOrIRASeqDcyBrsIdxOfHead.clear(); m_vVCcSigIncOrIRASeqDcyBrsIdxOfHead.clear();
      vector<int> vSigIncOrIRASeqDcyBrsIdxOfHead, vCcSigIncOrIRASeqDcyBrsIdxOfHead; 
      m_vVSigIncOrIRASeqDcyBrsMidxOfHead.clear(); m_vVCcSigIncOrIRASeqDcyBrsMidxOfHead.clear();
      vector<int> vSigIncOrIRASeqDcyBrsMidxOfHead, vCcSigIncOrIRASeqDcyBrsMidxOfHead;
      m_vVIIncSigIncOrIRASeqDcyBrs.clear();
      vector<int> vIIncSigIncOrIRASeqDcyBrs;
      m_vVIIRASigIncOrIRASeqDcyBrs.clear();
      vector<int> vIIRASigIncOrIRASeqDcyBrs;      
      m_vISigIncOrIRASeqDcyBrs.clear(); m_vICcSigIncOrIRASeqDcyBrs.clear();
      m_vNSigIncOrIRASeqDcyBrs.clear(); m_vNCcSigIncOrIRASeqDcyBrs.clear();
      vector<int> vCcSigPid123, vCcSigMidx123;
      for(unsigned int i=0;i<m_vVSigPid123.size();i++)
        {
          sortPs(m_vVSigPid123[i],m_vVSigMidx123[i]);
          sigIncOrIRASeqDcyBrs.clear();
          vSigIncOrIRASeqDcyBrsIdxOfHead.clear();
          vSigIncOrIRASeqDcyBrsMidxOfHead.clear();
          vIIncSigIncOrIRASeqDcyBrs.clear();
          vIIRASigIncOrIRASeqDcyBrs.clear();
          getDcyTr(m_vVSigPid123[i],m_vVSigMidx123[i],sigIncOrIRASeqDcyBrs,vSigIncOrIRASeqDcyBrsIdxOfHead,vSigIncOrIRASeqDcyBrsMidxOfHead,vIIncSigIncOrIRASeqDcyBrs,vIIRASigIncOrIRASeqDcyBrs);
          // remove the branch from e+ and e- to the first particle if all the following particles are descendant s of the first particle.
          if(sigIncOrIRASeqDcyBrs[0].size()==3)
          { 
            sigIncOrIRASeqDcyBrs.erase(sigIncOrIRASeqDcyBrs.begin());
            vSigIncOrIRASeqDcyBrsIdxOfHead.erase(vSigIncOrIRASeqDcyBrsIdxOfHead.begin());
            vSigIncOrIRASeqDcyBrsMidxOfHead.erase(vSigIncOrIRASeqDcyBrsMidxOfHead.begin());
            vIIncSigIncOrIRASeqDcyBrs.erase(vIIncSigIncOrIRASeqDcyBrs.begin());
            vIIRASigIncOrIRASeqDcyBrs.erase(vIIRASigIncOrIRASeqDcyBrs.begin());
          }

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigIncOrIRASeqDcyBrs.size();j++)
            {
              if(sigIncOrIRASeqDcyBrs==m_vSigIncOrIRASeqDcyBrs[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal inclusive or intermediate-resonance-allowed sequential decay branches is same as the "<<j+1<<ordNumSufj<<" signal inclusive or intermediate-resonance-allowed sequential decay branches!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);    
                }
              else if(m_ccSwitch==true&&m_vICcSigIncOrIRASeqDcyBrs[j]!=0&&sigIncOrIRASeqDcyBrs==m_vCcSigIncOrIRASeqDcyBrs[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal inclusive or intermediate-resonance-allowed sequential decay branches is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal inclusive or intermediate-resonance-allowed sequential decay branches!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }                
            }

          m_vSigIncOrIRASeqDcyBrs.push_back(sigIncOrIRASeqDcyBrs);
          m_vVSigIncOrIRASeqDcyBrsIdxOfHead.push_back(vSigIncOrIRASeqDcyBrsIdxOfHead);
          m_vVSigIncOrIRASeqDcyBrsMidxOfHead.push_back(vSigIncOrIRASeqDcyBrsMidxOfHead);
          m_vVIIncSigIncOrIRASeqDcyBrs.push_back(vIIncSigIncOrIRASeqDcyBrs);
          m_vVIIRASigIncOrIRASeqDcyBrs.push_back(vIIRASigIncOrIRASeqDcyBrs);
          m_vISigIncOrIRASeqDcyBrs.push_back(m_vSigIncOrIRASeqDcyBrs.size()-1);
          m_vNSigIncOrIRASeqDcyBrs.push_back(0);
          if(m_ccSwitch==true)
            {
              vCcSigPid123.clear();
              vCcSigMidx123.clear();
              for(unsigned int j=0;j<m_vVSigPid123[i].size();j++) vCcSigPid123.push_back(getCcPid(m_vVSigPid123[i][j])); 
              vCcSigMidx123=m_vVSigMidx123[i];
              sortPs(vCcSigPid123,vCcSigMidx123);
              ccSigIncOrIRASeqDcyBrs.clear();
              vCcSigIncOrIRASeqDcyBrsIdxOfHead.clear();
              vCcSigIncOrIRASeqDcyBrsMidxOfHead.clear();
              getDcyTr(vCcSigPid123,vCcSigMidx123,ccSigIncOrIRASeqDcyBrs,vCcSigIncOrIRASeqDcyBrsIdxOfHead,vCcSigIncOrIRASeqDcyBrsMidxOfHead);
              // remove the branch from e+ and e- to the first particle if all the following particles are descendant s of the first particle.
              if(ccSigIncOrIRASeqDcyBrs[0].size()==3)
              {
                ccSigIncOrIRASeqDcyBrs.erase(ccSigIncOrIRASeqDcyBrs.begin());
                vCcSigIncOrIRASeqDcyBrsIdxOfHead.erase(vCcSigIncOrIRASeqDcyBrsIdxOfHead.begin());
                vCcSigIncOrIRASeqDcyBrsMidxOfHead.erase(vCcSigIncOrIRASeqDcyBrsMidxOfHead.begin());
              }
              m_vCcSigIncOrIRASeqDcyBrs.push_back(ccSigIncOrIRASeqDcyBrs);
              m_vVCcSigIncOrIRASeqDcyBrsIdxOfHead.push_back(vCcSigIncOrIRASeqDcyBrsIdxOfHead);
              m_vVCcSigIncOrIRASeqDcyBrsMidxOfHead.push_back(vCcSigIncOrIRASeqDcyBrsMidxOfHead);
              if(ccSigIncOrIRASeqDcyBrs==sigIncOrIRASeqDcyBrs) m_vICcSigIncOrIRASeqDcyBrs.push_back(0);
              else m_vICcSigIncOrIRASeqDcyBrs.push_back(1);
              m_vNCcSigIncOrIRASeqDcyBrs.push_back(0);
            }          
        }

      cout<<"i.e.:"<<endl<<endl;
      list<int> sigDcyBr;
      for(unsigned int i=0;i<m_vSigIncOrIRASeqDcyBrs.size();i++)
        { 
          sigIncOrIRASeqDcyBrs.clear();
          sigIncOrIRASeqDcyBrs=m_vSigIncOrIRASeqDcyBrs[i];
          vSigIncOrIRASeqDcyBrsIdxOfHead=m_vVSigIncOrIRASeqDcyBrsIdxOfHead[i];
          for(unsigned int j=0;j<sigIncOrIRASeqDcyBrs.size();j++)
            {
              sigDcyBr.clear();
              sigDcyBr=sigIncOrIRASeqDcyBrs[j];
              cout<<" ";
              list<int>::iterator liit=sigDcyBr.begin();
              writePnmFromPid(cout,"TxtPnm",(*liit));
              if(j==0&&vSigIncOrIRASeqDcyBrsIdxOfHead[0]==-1)
                {
                  liit++;
                  writePnmFromPid(cout,"TxtPnm",(*liit));
                }
              if(m_vVIIRASigIncOrIRASeqDcyBrs[i][j]==1) cout<<" (--> X)";
              cout<<" -->";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
              if(m_vVIIncSigIncOrIRASeqDcyBrs[i][j]==1)
                {
                  cout<<" +";
                  writePnmFromPid(cout,"TxtPnm",m_pidOfAnything);
                }
              if(m_vVIIncSigIncOrIRASeqDcyBrs[i][j]==1&&m_vVIIRASigIncOrIRASeqDcyBrs[i][j]==1)
                {
                  cout<<endl<<endl;
                  cerr<<"Error: The Branch is not only inclusive but also . Such Branches are not supported in the current version of the \"TopoAna\" program!"<<endl;
                  cerr<<"Infor: If you really need the function, Please contact me (zhouxy@buaa.edu.cn) and then I will try to implement the function for you."<<endl;
                  exit(-1);
                }
              cout<<endl;
            }
          cout<<endl;
        }
    }

  if(m_vVSigPid4.size()==0)
    {
      cout<<"No signal decay trees are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal decay trees:"<<endl<<endl;
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

      m_vSigDcyTr.clear(); m_vCcSigDcyTr.clear();
      vector< list<int> > sigDcyTr, ccSigDcyTr;
      m_vISigDcyTr.clear(); m_vICcSigDcyTr.clear();
      m_vNSigDcyTr.clear(); m_vNCcSigDcyTr.clear(); 
      m_vSigDcyFSt.clear(); m_vCcSigDcyFSt.clear();
      list<int> sigDcyFSt, ccSigDcyFSt;
      m_vISigDcyFSt.clear(); m_vICcSigDcyFSt.clear();
      m_vNSigDcyFSt.clear(); m_vNCcSigDcyFSt.clear();
      m_iSigDcyTrISigDcyFStMap.clear(); m_iSigDcyTrICcSigDcyFStMap.clear();
      vector<int> vCcSigPid;
      vector<int> vCcSigMidx;
      int iSigDcyFSt, iCcSigDcyFSt;
      for(unsigned int i=0;i<m_vVSigPid4.size();i++)
        {
          sortPs(m_vVSigPid4[i],m_vVSigMidx4[i]);
          sigDcyTr.clear();      
          getDcyTr(m_vVSigPid4[i],m_vVSigMidx4[i],sigDcyTr);
          sigDcyFSt.clear();
          getDcyFSt(m_vVSigPid4[i],m_vVSigMidx4[i],sigDcyFSt);
          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigDcyTr.size();j++)
            {
              if(sigDcyTr==m_vSigDcyTr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal decay tree is same as the "<<j+1<<ordNumSufj<<" signal decay tree!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);    
                }
              else if(m_ccSwitch==true&&m_vICcSigDcyTr[j]!=0&&sigDcyTr==m_vCcSigDcyTr[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal decay tree is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal decay tree!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
            }
          m_vSigDcyTr.push_back(sigDcyTr);
          m_vISigDcyTr.push_back(m_vSigDcyTr.size()-1);
          m_vNSigDcyTr.push_back(0);          
          if(m_ccSwitch==true)
            {
              vCcSigPid.clear();
              vCcSigMidx.clear();
              for(unsigned int j=0;j<m_vVSigPid4[i].size();j++) vCcSigPid.push_back(getCcPid(m_vVSigPid4[i][j]));
              vCcSigMidx=m_vVSigMidx4[i];
              sortPs(vCcSigPid,vCcSigMidx);
              ccSigDcyTr.clear();
              getDcyTr(vCcSigPid,vCcSigMidx,ccSigDcyTr);
              m_vCcSigDcyTr.push_back(ccSigDcyTr);
              if(ccSigDcyTr==sigDcyTr) m_vICcSigDcyTr.push_back(0);
              else m_vICcSigDcyTr.push_back(1);
              m_vNCcSigDcyTr.push_back(0);
            }

          iSigDcyFSt=-1; // If the variable is still equal to -1 after the following loop, then the signal decay final states is a new signal decay final states.
          for(unsigned int j=0;j<m_vSigDcyFSt.size();j++)
            {
              if(sigDcyFSt==m_vSigDcyFSt[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Infor: The final state of the "<<i+1<<ordNumSufi<<" signal decay tree is same as that of the "<<j+1<<ordNumSufj<<" signal decay tree."<<endl<<endl;
                  iSigDcyFSt=j;
                  if(m_ccSwitch==true) iCcSigDcyFSt=m_vICcSigDcyFSt[j];
                  break;
                }
              else if(m_ccSwitch==true&&m_vICcSigDcyFSt[j]!=0&&sigDcyFSt==m_vCcSigDcyFSt[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Infor: The final state of the "<<i+1<<ordNumSufi<<" signal decay tree is same as that of the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal decay tree."<<endl<<endl;
                  iSigDcyFSt=j;
                  iCcSigDcyFSt=-1;
                  break;
                }
            }
          if(iSigDcyFSt==-1)
            {
              iSigDcyFSt=m_vSigDcyFSt.size();
              m_vSigDcyFSt.push_back(sigDcyFSt);
              m_vISigDcyFSt.push_back(m_vSigDcyFSt.size()-1);
              m_vNSigDcyFSt.push_back(0);
              if(m_ccSwitch==true)
                {
                  ccSigDcyFSt.clear();
                  getDcyFSt(vCcSigPid,vCcSigMidx,ccSigDcyFSt);
                  m_vCcSigDcyFSt.push_back(ccSigDcyFSt);
                  if(ccSigDcyFSt==sigDcyFSt) iCcSigDcyFSt=0;
                  else iCcSigDcyFSt=1;
                  m_vICcSigDcyFSt.push_back(iCcSigDcyFSt);
                  m_vNCcSigDcyFSt.push_back(0);
                }
            }
          m_iSigDcyTrISigDcyFStMap[m_vSigDcyTr.size()-1]=iSigDcyFSt;
          if(m_ccSwitch==true) m_iSigDcyTrICcSigDcyFStMap[m_vSigDcyTr.size()-1]=iCcSigDcyFSt;   
        }

      cout<<"With the initial e+ e-:"<<endl<<endl;
      list<int> sigDcyBr;
      for(unsigned int i=0;i<m_vSigDcyTr.size();i++)
        { 
          sigDcyTr.clear();
          sigDcyTr=m_vSigDcyTr[i];
          for(unsigned int j=0;j<sigDcyTr.size();j++)
            {
              sigDcyBr.clear();
              sigDcyBr=sigDcyTr[j];
              cout<<" ";
              list<int>::iterator liit=sigDcyBr.begin();
              writePnmFromPid(cout,"TxtPnm",(*liit));
              // The condition "j==0" is set for the initial state particle pair e+e-;the condition "(*liit)==1, 2, 3, 4, 5 or 6" is set for the intermediate state quark pair ddbar, uubar, ssbar, ccbar, bbbar or ttbar;
              if(j==0||(*liit)==1||(*liit)==2||(*liit)==3||(*liit)==4||(*liit)==5||(*liit)==6)
                {
                  liit++;
                  writePnmFromPid(cout,"TxtPnm",(*liit));
                }
              cout<<" -->";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
              cout<<endl;
            }
          cout<<endl;
        }
    }

  if(m_vVSigPid5.size()==0)
    {
      cout<<"No signal decay final states are specified."<<endl<<endl;
    }
  else
    {
      cout<<"Signal decay final states:"<<endl<<endl;
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

      m_vSigDcyFSt2.clear(); m_vCcSigDcyFSt2.clear();
      list<int> sigDcyFSt2, ccSigDcyFSt2;
      m_vISigDcyFSt2.clear(); m_vICcSigDcyFSt2.clear();
      m_vNSigDcyFSt2.clear(); m_vNCcSigDcyFSt2.clear();
      for(unsigned int i=0;i<m_vVSigPid5.size();i++)
        {
          sigDcyFSt2.clear();
          for(unsigned int j=0;j<m_vVSigPid5[i].size();j++) sigDcyFSt2.push_back(m_vVSigPid5[i][j]);
          sortByPidAndPchrg(sigDcyFSt2);
          sigDcyFSt2.push_front(11);
          sigDcyFSt2.push_front(-11);

          string ordNumSufi="th";
          string ordNumSufj="th";
          for(unsigned int j=0;j<m_vSigDcyFSt2.size();j++)
            {
              if(sigDcyFSt2==m_vSigDcyFSt2[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal decay final state is same as the "<<j+1<<ordNumSufj<<" signal decay final state!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }
              else if(m_ccSwitch==true&&m_vICcSigDcyFSt2[j]!=0&&sigDcyFSt2==m_vCcSigDcyFSt2[j])
                {
                  if(i==0) ordNumSufi="st"; else if(i==1) ordNumSufi="nd"; else if(i==2) ordNumSufi="rd";
                  if(j==0) ordNumSufj="st"; else if(j==1) ordNumSufj="nd"; else if(j==2) ordNumSufj="rd";
                  cerr<<"Error: The "<<i+1<<ordNumSufi<<" signal decay final state is same as the charge conjugate counterpart of "<<j+1<<ordNumSufj<<" signal decay final state!"<<endl;
                  cerr<<"Infor: Please check the input card and remove one of them."<<endl;
                  exit(-1);
                }               
            }

          m_vSigDcyFSt2.push_back(sigDcyFSt2);
          m_vISigDcyFSt2.push_back(m_vSigDcyFSt2.size()-1);
          m_vNSigDcyFSt2.push_back(0);
          if(m_ccSwitch==true)
            {
              ccSigDcyFSt2.clear();
              list<int>::iterator liit=sigDcyFSt2.begin();
              liit++;
              for(liit++;liit!=sigDcyFSt2.end();liit++) ccSigDcyFSt2.push_back(getCcPid((*liit)));
              sortByPidAndPchrg(ccSigDcyFSt2);
              ccSigDcyFSt2.push_front(11);
              ccSigDcyFSt2.push_front(-11);
              m_vCcSigDcyFSt2.push_back(ccSigDcyFSt2);
              if(ccSigDcyFSt2==sigDcyFSt2) m_vICcSigDcyFSt2.push_back(0);
              else m_vICcSigDcyFSt2.push_back(1);
              m_vNCcSigDcyFSt2.push_back(0);
            }
        }

      cout<<"With the initial e+ e-:"<<endl<<endl;
      for(unsigned int i=0;i<m_vSigDcyFSt2.size();i++)
        {
          sigDcyFSt2.clear();
          sigDcyFSt2=m_vSigDcyFSt2[i];
          cout<<" ";
          list<int>::iterator liit=sigDcyFSt2.begin();
          writePnmFromPid(cout,"TxtPnm",(*liit));
          liit++;
          writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<" -->";
          for(liit++;liit!=sigDcyFSt2.end();liit++) writePnmFromPid(cout,"TxtPnm",(*liit));
          cout<<endl<<endl;
        }
    }

  bool haveTopoAnaTasks=false;
  if((m_sttaDcyTrsAndDcyFSts==false)||(m_vPttaPid.size()!=0)||(m_vVPttaPid1.size()!=0)||(m_vSigPid.size()!=0)||(m_vVSigPid1.size()!=0)||(m_vVSigPid2.size()!=0)||(m_vVSigPid12.size()!=0)||(m_vVSigPid3.size()!=0)||(m_vVSigPid123.size()!=0)||(m_vVSigPid4.size()!=0)||(m_vVSigPid5.size()!=0)) haveTopoAnaTasks=true;
  if(haveTopoAnaTasks==false)
    {
      cerr<<"Error: No topology analysis is set up to be performed!"<<endl;
      cerr<<"Infor: Please check the input card!"<<endl;
      exit(-1);        
    }

}
