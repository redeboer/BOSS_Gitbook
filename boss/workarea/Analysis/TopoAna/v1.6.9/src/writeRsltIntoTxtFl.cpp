#include "../include/topoana.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

void topoana::writeRsltIntoTxtFl()
{
  string NmOfOptTxtFl=m_mainNmOfOptFls+".txt";
  ofstream fout(NmOfOptTxtFl.c_str(),ios::out);
  if(!fout)
    {
      cerr<<"Error: Can't create the output txt file \""<<NmOfOptTxtFl<<"\"!"<<endl;
      cerr<<"Infor: Please check it."<<endl;
      exit(-1);
    }

  if(m_sttaDcyTrsAndDcyFSts==false)
    {
      fout<<"Decay trees and their respective final states:"<<endl<<endl;
      vector< list<int> > dcyTr;
      list<int> dcyBr;
      list<int> dcyFSt;
      unsigned long nCmltEtrs=0;
      unsigned long nDcyTrsToBePrtd=m_vDcyTr.size()<m_nDcyTrsToBePrtdMax?m_vDcyTr.size():m_nDcyTrsToBePrtdMax; 
      for(unsigned long i=0;i<nDcyTrsToBePrtd;i++)
        { 
          dcyTr.clear();
          dcyTr=m_vDcyTr[i];
          fout<<"index:  "<<i+1<<"\tiDcyTr:  "<<m_vIDcyTr[i]<<"\tiDcyFSt:  "<<m_iDcyTrIDcyFStMap[m_vIDcyTr[i]]<<"\tnEtrs:  "<<m_vNDcyTr[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcEtrs:  ";
              if(m_vICcDcyTr[i]==0) fout<<"---";
              else fout<<m_vNCcDcyTr[i];
              fout<<"\tnTotEtrs:  "<<m_vNDcyTr[i]+m_vNCcDcyTr[i];
              nCmltEtrs=nCmltEtrs+m_vNDcyTr[i]+m_vNCcDcyTr[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNDcyTr[i];
            }
          fout<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;

          for(unsigned int j=0;j<dcyTr.size();j++)
            {
              if(j==0&&m_spr1stBrsOfDcyTrs==true&&dcyTr[0].size()==3) continue; 
              dcyBr.clear();
              dcyBr=dcyTr[j];
              fout<<" ";
              list<int>::iterator liit=dcyBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              // The condition "j==0" is set for the initial state particle pair e+e-;the condition "(*liit)==1, 2, 3, 4, 5 or 6" is set for the intermediate state quark pair ddbar, uubar, ssbar, ccbar, bbbar or ttbar;
              if(j==0||(*liit)==1||(*liit)==2||(*liit)==3||(*liit)==4||(*liit)==5||(*liit)==6)
                {
                  liit++;
                  writePnmFromPid(fout,"TxtPnm",(*liit));
                }
              fout<<" -->";
              for(liit++;liit!=dcyBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<endl;
            }

          for(unsigned int j=0;j<m_vDcyFSt.size();j++)
            {
              if(m_vIDcyFSt[j]==m_iDcyTrIDcyFStMap[m_vIDcyTr[i]])
                {
                  dcyFSt.clear();
                  if(m_iDcyTrICcDcyFStMap[m_vIDcyTr[i]]!=-1) dcyFSt=m_vDcyFSt[j];
                  else dcyFSt=m_vCcDcyFSt[j];
                  break;
                }
            }
          fout<<"(";
          list<int>::iterator liit;
          if(m_spr1stBrsOfDcyTrs==true&&dcyTr[0].size()==3)
            {
              liit=dcyTr[0].begin();
              liit++;
              liit++;
              writePnmFromPid(fout,"TxtPnm",(*liit));
              liit=dcyFSt.begin();
              liit++;
            }
          else
            {
              liit=dcyFSt.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              liit++;
              writePnmFromPid(fout,"TxtPnm",(*liit));
            }
          fout<<" -->";
          for(liit++;liit!=dcyFSt.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" )"<<endl; 

          fout<<endl;
        }
      if(nDcyTrsToBePrtd<m_vDcyTr.size())
        {
          fout<<"index:  "<<"rest"<<"\tiDcyTr:  "<<"---"<<"\tiDcyFSt:  "<<"---"<<"\tnEtrs:  ";
          unsigned long nCmltEtrsOfRest=0;
          if(m_ccSwitch==true)
            {
              fout<<"---"<<"\tnCcEtrs:  "<<"---"<<"\tnTotEtrs:  ";
              for(unsigned long i=nDcyTrsToBePrtd;i<m_vDcyTr.size();i++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vNDcyTr[i]+m_vNCcDcyTr[i];
            }
          else
            {
              for(unsigned long i=nDcyTrsToBePrtd;i<m_vDcyTr.size();i++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vNDcyTr[i];
            }
          nCmltEtrs=nCmltEtrs+nCmltEtrsOfRest;
          fout<<nCmltEtrsOfRest<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;    

          fout<<" ";
          if(m_spr1stBrsOfDcyTrs==true)
            {
              bool isEpEmToAPInAllOtherDcyTrs=true;
              list<int>::iterator liit=m_vDcyTr[nDcyTrsToBePrtd][0].begin();
              liit++;
              liit++;
              int pid=(*liit);
              for(unsigned long i=nDcyTrsToBePrtd;i<m_vDcyTr.size();i++)
                { 
                  if(isEpEmToAPInAllOtherDcyTrs==true)
                    { 
                      if(m_vDcyTr[i][0].size()!=3)
                        { 
                          isEpEmToAPInAllOtherDcyTrs=false;
                        }
                      else
                        { 
                          liit=m_vDcyTr[i][0].begin();
                          liit++;
                          liit++;
                          if((*liit)!=pid) isEpEmToAPInAllOtherDcyTrs=false;
                        }
                    }
                }
              if(isEpEmToAPInAllOtherDcyTrs==true)
                {
                  writePnmFromPid(fout,"TxtPnm",pid);
                  fout<<" --> others"<<endl;

                  fout<<"(";
                  writePnmFromPid(fout,"TxtPnm",pid);           
                }
              else
                {
                  writePnmFromPid(fout,"TxtPnm",-11);
                  writePnmFromPid(fout,"TxtPnm",11);
                  fout<<" --> others"<<endl;

                  fout<<"(";
                  writePnmFromPid(fout,"TxtPnm",-11);
                  writePnmFromPid(fout,"TxtPnm",11);
                }
            }
          else
            {
              writePnmFromPid(fout,"TxtPnm",-11);
              writePnmFromPid(fout,"TxtPnm",11);
              fout<<" --> others"<<endl;

              fout<<"(";
              writePnmFromPid(fout,"TxtPnm",-11);
              writePnmFromPid(fout,"TxtPnm",11); 
           }
          fout<<" --> corresponding to others )"<<endl;

          fout<<endl;    
        }

      fout<<endl;

      fout<<"Decay final states:"<<endl<<endl;
      //list<int> dcyFSt; The list<int> variable dcyFSt has been previously declared.
      nCmltEtrs=0;
      unsigned long nDcyFStsToBePrtd=m_vDcyFSt.size()<m_nDcyFStsToBePrtdMax?m_vDcyFSt.size():m_nDcyFStsToBePrtdMax;
      for(unsigned long i=0;i<nDcyFStsToBePrtd;i++)
        {
          dcyFSt.clear();
          dcyFSt=m_vDcyFSt[i];
          fout<<"index:  "<<i+1<<"\tiDcyFSt:  "<<m_vIDcyFSt[i]<<"\tnEtrs:  "<<m_vNDcyFSt[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcEtrs:  ";
              if(m_vICcDcyFSt[i]==0) fout<<"---";
              else fout<<m_vNCcDcyFSt[i];
              fout<<"\tnTotEtrs:  "<<m_vNDcyFSt[i]+m_vNCcDcyFSt[i]; 
              nCmltEtrs=nCmltEtrs+m_vNDcyFSt[i]+m_vNCcDcyFSt[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNDcyFSt[i];
            }
          fout<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
          fout<<" ";
          list<int>::iterator liit=dcyFSt.begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" -->";
          for(liit++;liit!=dcyFSt.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<endl<<endl;
        }
      if(nDcyFStsToBePrtd<m_vDcyFSt.size())
        {
          fout<<"index:  "<<"rest"<<"\tiDcyFSt:  "<<"---"<<"\tnEtrs:  ";
          unsigned long nCmltEtrsOfRest=0;
          if(m_ccSwitch==true)
            {
              fout<<"---"<<"\tnCcEtrs:  "<<"---"<<"\tnTotEtrs:  ";  
              for(unsigned long i=nDcyFStsToBePrtd;i<m_vDcyFSt.size();i++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vNDcyFSt[i]+m_vNCcDcyFSt[i];
            }
          else
            {
              for(unsigned long i=nDcyFStsToBePrtd;i<m_vDcyFSt.size();i++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vNDcyFSt[i];
            }
          nCmltEtrs=nCmltEtrs+nCmltEtrsOfRest;
          fout<<nCmltEtrsOfRest<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;

          fout<<" ";
          writePnmFromPid(fout,"TxtPnm",-11);
          writePnmFromPid(fout,"TxtPnm",11);
          fout<<" --> others"<<endl;
          
          fout<<endl;
        }
    }

  if(m_vVDcyBrP.size()>0)
    {
      for(unsigned int i=0;i<m_vVDcyBrP.size();i++)
        {
          fout<<endl;

          fout<<"Decay branches begun with";
          writePnmFromPid(fout,"TxtPnm",m_vPttaPid[i]);
          fout<<":"<<endl<<endl;

          list<int> dcyBrP;
          unsigned long nCmltCases=0;
          unsigned long nDcyBrPToBePrtd=m_vVDcyBrP[i].size()<m_vNDcyBrsMax[i]?m_vVDcyBrP[i].size():m_vNDcyBrsMax[i];
          for(unsigned int j=0;j<nDcyBrPToBePrtd;j++)
            {
              dcyBrP.clear();
              dcyBrP=m_vVDcyBrP[i][j];
              fout<<"index:  "<<j+1<<"\tiDcyBrP"<<i+1<<":  "<<m_vVIDcyBrP[i][j]<<"\tnCases:  "<<m_vVNDcyBrP[i][j];
              if(m_ccSwitch==true)
                {
                  fout<<"\tnCcCases:  ";
                  if(m_vPttaICcPid[i]==0&&m_vVIDcyBrCcP[i][j]==0) fout<<"---";
                  else fout<<m_vVNDcyBrCcP[i][j];
                  fout<<"\tnTotCases:  "<<m_vVNDcyBrP[i][j]+m_vVNDcyBrCcP[i][j]; 
                  nCmltCases=nCmltCases+m_vVNDcyBrP[i][j]+m_vVNDcyBrCcP[i][j];
                }
              else
                {
                  nCmltCases=nCmltCases+m_vVNDcyBrP[i][j];
                }
              fout<<"\tnCmltCases:  "<<nCmltCases<<endl;
              fout<<" ";
              list<int>::iterator liit=dcyBrP.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<" -->";
              for(liit++;liit!=dcyBrP.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<endl<<endl;
            }
          if(nDcyBrPToBePrtd<m_vVDcyBrP[i].size())
            {
              fout<<"index:  "<<"rest"<<"\tiDcyBrP"<<i+1<<":  "<<"---"<<"\tnCases:  ";
              unsigned long nCmltCasesOfRest=0;
              if(m_ccSwitch==true)
                {
                  if(m_vPttaICcPid[i]==0)
                    {
                      fout<<"---"<<"\tnCcCases:  "<<"---"<<"\tnTotCases:  ";
                    }
                  else
                    {
                      unsigned long nCmltCasesOfRestTemp1=0;
                      unsigned long nCmltCasesOfRestTemp2=0;
                      for(unsigned int j=nDcyBrPToBePrtd;j<m_vVDcyBrP[i].size();j++) nCmltCasesOfRestTemp1=nCmltCasesOfRestTemp1+m_vVNDcyBrP[i][j];
                      for(unsigned int j=nDcyBrPToBePrtd;j<m_vVDcyBrP[i].size();j++) nCmltCasesOfRestTemp2=nCmltCasesOfRestTemp2+m_vVNDcyBrCcP[i][j];
                      fout<<nCmltCasesOfRestTemp1<<"\tnCcCases:  "<<nCmltCasesOfRestTemp2<<"\tnTotCases:  ";
                    }
                  for(unsigned int j=nDcyBrPToBePrtd;j<m_vVDcyBrP[i].size();j++) nCmltCasesOfRest=nCmltCasesOfRest+m_vVNDcyBrP[i][j]+m_vVNDcyBrCcP[i][j];
                }
              else
                {
                  for(unsigned int j=nDcyBrPToBePrtd;j<m_vVDcyBrP[i].size();j++) nCmltCasesOfRest=nCmltCasesOfRest+m_vVNDcyBrP[i][j];
                }
              nCmltCases=nCmltCases+nCmltCasesOfRest;
              fout<<nCmltCasesOfRest<<"\tnCmltCases:  "<<nCmltCases<<endl;
              fout<<" ";
              list<int>::iterator liit=dcyBrP.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<" --> others";
              fout<<endl<<endl;
            }
        }
    }

  if(m_vVDcyBrIncDcyBr.size()>0)
    {
      for(unsigned int i=0;i<m_vVDcyBrIncDcyBr.size();i++)
        {
          fout<<endl;

          fout<<"Exclusive decay branches matched with";
          list<int>::iterator liit=m_vPttaIncDcyBr[i].begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          if((*liit)==-11)
            {
              liit++;
              writePnmFromPid(fout,"TxtPnm",(*liit));
            }
          fout<<" -->";
          for(liit++;liit!=m_vPttaIncDcyBr[i].end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" + anything:"<<endl<<endl;

          list<int> dcyBrIncDcyBr;
          unsigned long nCmltCases=0;
          unsigned long nDcyBrIncDcyBrToBePrtd=m_vVDcyBrIncDcyBr[i].size()<m_vNDcyBrsMax1[i]?m_vVDcyBrIncDcyBr[i].size():m_vNDcyBrsMax1[i];
          for(unsigned int j=0;j<nDcyBrIncDcyBrToBePrtd;j++)
            {
              dcyBrIncDcyBr.clear();
              dcyBrIncDcyBr=m_vVDcyBrIncDcyBr[i][j];
              fout<<"index:  "<<j+1<<"\tiDcyBrIncDcyBr"<<i+1<<":  "<<m_vVIDcyBrIncDcyBr[i][j]<<"\tnCases:  "<<m_vVNDcyBrIncDcyBr[i][j];
              if(m_ccSwitch==true)
                {
                  fout<<"\tnCcCases:  ";
                  if(m_vPttaICcPid[i]==0&&m_vVIDcyBrCcIncDcyBr[i][j]==0) fout<<"---";
                  else fout<<m_vVNDcyBrCcIncDcyBr[i][j];
                  fout<<"\tnTotCases:  "<<m_vVNDcyBrIncDcyBr[i][j]+m_vVNDcyBrCcIncDcyBr[i][j]; 
                  nCmltCases=nCmltCases+m_vVNDcyBrIncDcyBr[i][j]+m_vVNDcyBrCcIncDcyBr[i][j];
                }
              else
                {
                  nCmltCases=nCmltCases+m_vVNDcyBrIncDcyBr[i][j];
                }
              fout<<"\tnCmltCases:  "<<nCmltCases<<endl;
              fout<<" ";
              list<int>::iterator liit=dcyBrIncDcyBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              if((*liit)==-11)
                {
                  liit++;
                  writePnmFromPid(fout,"TxtPnm",(*liit));
                }
              fout<<" -->";
              for(liit++;liit!=dcyBrIncDcyBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<endl<<endl;
            }
          if(nDcyBrIncDcyBrToBePrtd<m_vVDcyBrIncDcyBr[i].size())
            {
              fout<<"index:  "<<"rest"<<"\tiDcyBrIncDcyBr"<<i+1<<":  "<<"---"<<"\tnCases:  ";
              unsigned long nCmltCasesOfRest=0;
              if(m_ccSwitch==true)
                {
                  if(m_vPttaICcPid[i]==0)
                    {
                      fout<<"---"<<"\tnCcCases:  "<<"---"<<"\tnTotCases:  ";
                    }
                  else
                    {
                      unsigned long nCmltCasesOfRestTemp1=0;
                      unsigned long nCmltCasesOfRestTemp2=0;
                      for(unsigned int j=nDcyBrIncDcyBrToBePrtd;j<m_vVDcyBrIncDcyBr[i].size();j++) nCmltCasesOfRestTemp1=nCmltCasesOfRestTemp1+m_vVNDcyBrIncDcyBr[i][j];
                      for(unsigned int j=nDcyBrIncDcyBrToBePrtd;j<m_vVDcyBrIncDcyBr[i].size();j++) nCmltCasesOfRestTemp2=nCmltCasesOfRestTemp2+m_vVNDcyBrCcIncDcyBr[i][j];
                      fout<<nCmltCasesOfRestTemp1<<"\tnCcCases:  "<<nCmltCasesOfRestTemp2<<"\tnTotCases:  ";
                    }
                  for(unsigned int j=nDcyBrIncDcyBrToBePrtd;j<m_vVDcyBrIncDcyBr[i].size();j++) nCmltCasesOfRest=nCmltCasesOfRest+m_vVNDcyBrIncDcyBr[i][j]+m_vVNDcyBrCcIncDcyBr[i][j];
                }
              else
                {
                  for(unsigned int j=nDcyBrIncDcyBrToBePrtd;j<m_vVDcyBrIncDcyBr[i].size();j++) nCmltCasesOfRest=nCmltCasesOfRest+m_vVNDcyBrIncDcyBr[i][j];
                }
              nCmltCases=nCmltCases+nCmltCasesOfRest;
              fout<<nCmltCasesOfRest<<"\tnCmltCases:  "<<nCmltCases<<endl;
              fout<<" ";
              list<int>::iterator liit=dcyBrIncDcyBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              if((*liit)==-11)
                {
                  liit++;
                  writePnmFromPid(fout,"TxtPnm",(*liit));
                }
              fout<<" --> others";
              fout<<endl<<endl;
            }
        }
    }

  if(m_vSigPid.size()>0)
    {
      fout<<endl;

      fout<<"Signal particles:"<<endl<<endl;
      unsigned long nCmltPs=0;
      for(unsigned int i=0;i<m_vNSigP.size();i++)
        {
          fout<<"index:  "<<i+1<<"\tSigP: ";
          writePnmFromPid(fout,"TxtPnm",m_vSigPid[i]);
          fout<<"\tiSigP:  "<<m_vISigP[i]<<"\tnPs:   "<<m_vNSigP[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcPs:   ";
              if(m_vICcSigP[i]>0) fout<<m_vNCcSigP[i];
              else fout<<"---";
              fout<<"\tnTotPs:   "<<m_vNSigP[i]+m_vNCcSigP[i];
              nCmltPs=nCmltPs+m_vNSigP[i]+m_vNCcSigP[i];
            }
          else
            {
              nCmltPs=nCmltPs+m_vNSigP[i];
            }
          fout<<"\tnCmltPs:  "<<nCmltPs<<endl<<endl;
        }
    }

  if(m_vSigIncDcyBr.size()>0)
    {
      fout<<endl;

      fout<<"Signal inclusive decay branches:"<<endl<<endl;

      list<int> sigIncDcyBr;
      unsigned long nCmltCases=0;
      for(unsigned int i=0;i<m_vSigIncDcyBr.size();i++)
        {
          sigIncDcyBr.clear();
          sigIncDcyBr=m_vSigIncDcyBr[i];
          fout<<"index:  "<<i+1<<"\tiSigIncDcyBr:  "<<m_vISigIncDcyBr[i]<<"\tnCases:  "<<m_vNSigIncDcyBr[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcCases:  ";
              if(m_vICcSigIncDcyBr[i]>0) fout<<m_vNCcSigIncDcyBr[i];
              else fout<<"---";
              fout<<"\tnTotCases:  "<<m_vNSigIncDcyBr[i]+m_vNCcSigIncDcyBr[i];
              nCmltCases=nCmltCases+m_vNSigIncDcyBr[i]+m_vNCcSigIncDcyBr[i];
            }
          else
            {
              nCmltCases=nCmltCases+m_vNSigIncDcyBr[i];
            }
          fout<<"\tnCmltCases:  "<<nCmltCases<<endl;
          fout<<" ";
          list<int>::iterator liit=sigIncDcyBr.begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          if((*liit)==-11)
            {
              liit++;
              writePnmFromPid(fout,"TxtPnm",(*liit));
            }
          fout<<" -->";
          for(liit++;liit!=sigIncDcyBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" + anything";
          fout<<endl<<endl;
        }
    }

  if(m_vSigSeqDcyBrs.size()>0)
    {
      fout<<endl;

      fout<<"Signal sequential decay branches:"<<endl<<endl;
      vector< list<int> > sigSeqDcyBrs;
      vector<int> vSigSeqDcyBrsIdxOfHead;
      list<int> sigDcyBr;
      unsigned long nCmltCases=0;
      for(unsigned int i=0;i<m_vSigSeqDcyBrs.size();i++)
        { 
          sigSeqDcyBrs.clear();
          sigSeqDcyBrs=m_vSigSeqDcyBrs[i];
          vSigSeqDcyBrsIdxOfHead=m_vVSigSeqDcyBrsIdxOfHead[i];
          fout<<"index:  "<<i+1<<"\tiSigSeqDcyBrs:  "<<m_vISigSeqDcyBrs[i]<<"\tnCases:  "<<m_vNSigSeqDcyBrs[i];
          if(m_ccSwitch==true)
            { 
              fout<<"\tnCcCases:  ";
              if(m_vICcSigSeqDcyBrs[i]>0) fout<<m_vNCcSigSeqDcyBrs[i];
              else fout<<"---";
              fout<<"\tnTotCases:  "<<m_vNSigSeqDcyBrs[i]+m_vNCcSigSeqDcyBrs[i];
              nCmltCases=nCmltCases+m_vNSigSeqDcyBrs[i]+m_vNCcSigSeqDcyBrs[i];
            }
          else
            { 
              nCmltCases=nCmltCases+m_vNSigSeqDcyBrs[i];
            }
          fout<<"\tnCmltCases:  "<<nCmltCases<<endl;
          for(unsigned int j=0;j<sigSeqDcyBrs.size();j++)
            {
              sigDcyBr.clear();
              sigDcyBr=sigSeqDcyBrs[j];
              fout<<" ";
              list<int>::iterator liit=sigDcyBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              if(j==0&&vSigSeqDcyBrsIdxOfHead[0]==-1)
              {
                liit++;
                writePnmFromPid(fout,"TxtPnm",(*liit));
              }
              fout<<" -->";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<endl;
            }
          fout<<endl;
        }
    }

  if(m_vSigIncSeqDcyBrs.size()>0)
    {
      fout<<endl;

      fout<<"Signal inclusive sequential decay branches:"<<endl<<endl;
      vector< list<int> > sigIncSeqDcyBrs;
      vector<int> vSigIncSeqDcyBrsIdxOfHead;
      list<int> sigDcyBr;
      unsigned long nCmltCases=0;
      for(unsigned int i=0;i<m_vSigIncSeqDcyBrs.size();i++)
        { 
          sigIncSeqDcyBrs.clear();
          sigIncSeqDcyBrs=m_vSigIncSeqDcyBrs[i];
          vSigIncSeqDcyBrsIdxOfHead=m_vVSigIncSeqDcyBrsIdxOfHead[i];
          fout<<"index:  "<<i+1<<"\tiSigIncSeqDcyBrs:  "<<m_vISigIncSeqDcyBrs[i]<<"\tnCases:  "<<m_vNSigIncSeqDcyBrs[i];
          if(m_ccSwitch==true)
            { 
              fout<<"\tnCcCases:  ";
              if(m_vICcSigIncSeqDcyBrs[i]>0) fout<<m_vNCcSigIncSeqDcyBrs[i];
              else fout<<"---";
              fout<<"\tnTotCases:  "<<m_vNSigIncSeqDcyBrs[i]+m_vNCcSigIncSeqDcyBrs[i];
              nCmltCases=nCmltCases+m_vNSigIncSeqDcyBrs[i]+m_vNCcSigIncSeqDcyBrs[i];
            }
          else
            { 
              nCmltCases=nCmltCases+m_vNSigIncSeqDcyBrs[i];
            }
          fout<<"\tnCmltCases:  "<<nCmltCases<<endl;
          for(unsigned int j=0;j<sigIncSeqDcyBrs.size();j++)
            {
              sigDcyBr.clear();
              sigDcyBr=sigIncSeqDcyBrs[j];
              fout<<" ";
              list<int>::iterator liit=sigDcyBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              if(j==0&&vSigIncSeqDcyBrsIdxOfHead[0]==-1)
              {
                liit++;
                writePnmFromPid(fout,"TxtPnm",(*liit));
              }
              fout<<" -->";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              if(m_vVIIncSigIncSeqDcyBrs[i][j]==1) writePnmFromPid(fout,"TxtPnm",m_pidOfAnything);
              fout<<endl;
            }
          fout<<endl;
        }
    }

  if(m_vSigIRADcyBr.size()>0)
    {
      fout<<endl;

      fout<<"Signal intermediate-resonance-allowed decay branches:"<<endl<<endl;

      list<int> sigIRADcyBr;
      unsigned long nCmltCases=0;
      for(unsigned int i=0;i<m_vSigIRADcyBr.size();i++)
        {
          sigIRADcyBr.clear();
          sigIRADcyBr=m_vSigIRADcyBr[i];
          fout<<"index:  "<<i+1<<"\tiSigIRADcyBr:  "<<m_vISigIRADcyBr[i]<<"\tnCases:  "<<m_vNSigIRADcyBr[i];
          if(m_ccSwitch==true)
            { 
              fout<<"\tnCcCases:  ";
              if(m_vICcSigIRADcyBr[i]>0) fout<<m_vNCcSigIRADcyBr[i];
              else fout<<"---";
              fout<<"\tnTotCases:  "<<m_vNSigIRADcyBr[i]+m_vNCcSigIRADcyBr[i];
              nCmltCases=nCmltCases+m_vNSigIRADcyBr[i]+m_vNCcSigIRADcyBr[i];
            }
          else
            { 
              nCmltCases=nCmltCases+m_vNSigIRADcyBr[i];
            }
          fout<<"\tnCmltCases:  "<<nCmltCases<<endl;
          fout<<" ";
          list<int>::iterator liit=sigIRADcyBr.begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" -->";
          for(liit++;liit!=sigIRADcyBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<endl<<endl;
        }
    }

  if(m_vSigIncOrIRASeqDcyBrs.size()>0)
    {
      fout<<endl;

      fout<<"Signal inclusive or intermediate-resonance-allowed sequential decay branches:"<<endl<<endl;
      vector< list<int> > sigIncOrIRASeqDcyBrs;
      vector<int> vSigIncOrIRASeqDcyBrsIdxOfHead;
      list<int> sigDcyBr;
      unsigned long nCmltCases=0;
      for(unsigned int i=0;i<m_vSigIncOrIRASeqDcyBrs.size();i++)
        { 
          sigIncOrIRASeqDcyBrs.clear();
          sigIncOrIRASeqDcyBrs=m_vSigIncOrIRASeqDcyBrs[i];
          vSigIncOrIRASeqDcyBrsIdxOfHead=m_vVSigIncOrIRASeqDcyBrsIdxOfHead[i];
          fout<<"index:  "<<i+1<<"\tiSigIncOrIRASeqDcyBrs:  "<<m_vISigIncOrIRASeqDcyBrs[i]<<"\tnCases:  "<<m_vNSigIncOrIRASeqDcyBrs[i];
          if(m_ccSwitch==true)
            { 
              fout<<"\tnCcCases:  ";
              if(m_vICcSigIncOrIRASeqDcyBrs[i]>0) fout<<m_vNCcSigIncOrIRASeqDcyBrs[i];
              else fout<<"---";
              fout<<"\tnTotCases:  "<<m_vNSigIncOrIRASeqDcyBrs[i]+m_vNCcSigIncOrIRASeqDcyBrs[i];
              nCmltCases=nCmltCases+m_vNSigIncOrIRASeqDcyBrs[i]+m_vNCcSigIncOrIRASeqDcyBrs[i];
            }
          else
            { 
              nCmltCases=nCmltCases+m_vNSigIncOrIRASeqDcyBrs[i];
            }
          fout<<"\tnCmltCases:  "<<nCmltCases<<endl;
          for(unsigned int j=0;j<sigIncOrIRASeqDcyBrs.size();j++)
            {
              sigDcyBr.clear();
              sigDcyBr=sigIncOrIRASeqDcyBrs[j];
              fout<<" ";
              list<int>::iterator liit=sigDcyBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              if(j==0&&vSigIncOrIRASeqDcyBrsIdxOfHead[0]==-1)
              {
                liit++;
                writePnmFromPid(fout,"TxtPnm",(*liit));
              }
              if(m_vVIIRASigIncOrIRASeqDcyBrs[i][j]==1) fout<<" (--> X)";
              fout<<" -->";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              if(m_vVIIncSigIncOrIRASeqDcyBrs[i][j]==1)
                {
                  writePnmFromPid(fout,"TxtPnm",m_pidOfAnything);
                }
              fout<<endl;
            }
          fout<<endl;
        }
    }

  if(m_vSigDcyTr.size()>0)
    {
      fout<<endl;

      fout<<"Signal decay trees and their respective final states:"<<endl<<endl;
      vector< list<int> > sigDcyTr;
      list<int> sigDcyBr;
      list<int> sigDcyFSt;
      unsigned long nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigDcyTr.size();i++)
        { 
          sigDcyTr.clear();
          sigDcyTr=m_vSigDcyTr[i];
          fout<<"index:  "<<i+1<<"\tiSigDcyTr:  "<<m_vISigDcyTr[i]<<"\tiSigDcyFSt:  ";
          if(m_iSigDcyTrICcSigDcyFStMap[m_vISigDcyTr[i]]>=0) fout<<m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]];
          else fout<<m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]]<<"_cc";
          fout<<"\tiDcyTr:  ";
          if(m_iSigDcyTrIDcyTrMap.find(m_vISigDcyTr[i])!=m_iSigDcyTrIDcyTrMap.end())
            {
              if(m_iSigDcyTrICcDcyTrMap[m_vISigDcyTr[i]]>=0) fout<<m_iSigDcyTrIDcyTrMap[m_vISigDcyTr[i]];
              else fout<<m_iSigDcyTrIDcyTrMap[m_vISigDcyTr[i]]<<"_cc";
            }
          else fout<<"---";
          fout<<"\tiDcyFSt:  ";
          if(m_iSigDcyFStIDcyFStMap.find(m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]])!=m_iSigDcyFStIDcyFStMap.end()) 
            {
              if(m_iSigDcyFStICcDcyFStMap[m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]]]>=0) fout<<m_iSigDcyFStIDcyFStMap[m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]]];
              else fout<<m_iSigDcyFStIDcyFStMap[m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]]]<<"_cc";
            }
          else fout<<"---";
          fout<<"\tnEtrs:  "<<m_vNSigDcyTr[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcEtrs:  ";
              if(m_vICcSigDcyTr[i]==0) fout<<"---";
              else fout<<m_vNCcSigDcyTr[i];
              fout<<"\tnTotEtrs:  "<<m_vNSigDcyTr[i]+m_vNCcSigDcyTr[i];
              nCmltEtrs=nCmltEtrs+m_vNSigDcyTr[i]+m_vNCcSigDcyTr[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNSigDcyTr[i];
            }
          fout<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
          for(unsigned int j=0;j<sigDcyTr.size();j++)
            {
              if(j==0&&m_spr1stBrsOfDcyTrs==true&&sigDcyTr[0].size()==3) continue;
              sigDcyBr.clear();
              sigDcyBr=sigDcyTr[j];
              fout<<" ";
              list<int>::iterator liit=sigDcyBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              // The condition "j==0" is set for the initial state particle pair e+e-;the condition "(*liit)==1, 2, 3, 4, 5 or 6" is set for the intermediate state quark pair ddbar, uubar, ssbar, ccbar, bbbar or ttbar;
              if(j==0||(*liit)==1||(*liit)==2||(*liit)==3||(*liit)==4||(*liit)==5||(*liit)==6)
                {
                  liit++;
                  writePnmFromPid(fout,"TxtPnm",(*liit));
                }
              fout<<" -->";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<endl;
            }

          for(unsigned int j=0;j<m_vSigDcyFSt.size();j++)
            {
              if(m_vISigDcyFSt[j]==m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]])
                {
                  sigDcyFSt.clear();
                  sigDcyFSt=m_vSigDcyFSt[j];
                  break;
                }
            }
          fout<<"(";
          list<int>::iterator liit;
          if(m_spr1stBrsOfDcyTrs==true&&sigDcyTr[0].size()==3)
            {
              liit=sigDcyTr[0].begin();
              liit++;
              liit++;
              writePnmFromPid(fout,"TxtPnm",(*liit));
              liit=sigDcyFSt.begin();
              liit++;
            }
          else
            {
              liit=sigDcyFSt.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              liit++;
              writePnmFromPid(fout,"TxtPnm",(*liit));
            }          
          fout<<" -->";
          for(liit++;liit!=sigDcyFSt.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" )"<<endl;

          fout<<endl;
        }

      fout<<endl;

      fout<<"Signal decay final states corresponding to signal decay trees:"<<endl<<endl;
      //list<int> sigDcyFSt; The list<int> variable sigDcyFSt has been previously declared.
      nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigDcyFSt.size();i++)
        {
          sigDcyFSt.clear();
          sigDcyFSt=m_vSigDcyFSt[i];
          fout<<"index:  "<<i+1<<"\tiSigDcyFSt:  "<<m_vISigDcyFSt[i]<<"\tiDcyFSt:  ";
          if(m_iSigDcyFStIDcyFStMap.find(m_vISigDcyFSt[i])!=m_iSigDcyFStIDcyFStMap.end())
            {
              if(m_iSigDcyFStICcDcyFStMap[m_vISigDcyFSt[i]]>=0) fout<<m_iSigDcyFStIDcyFStMap[m_vISigDcyFSt[i]];
              else fout<<m_iSigDcyFStIDcyFStMap[m_vISigDcyFSt[i]]<<"_cc";
            }
          else fout<<"---";
          fout<<"\tnEtrs:  "<<m_vNSigDcyFSt[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcEtrs:  ";
              if(m_vICcSigDcyFSt[i]==0) fout<<"---";
              else fout<<m_vNCcSigDcyFSt[i];
              fout<<"\tnTotEtrs:  "<<m_vNSigDcyFSt[i]+m_vNCcSigDcyFSt[i]; 
              nCmltEtrs=nCmltEtrs+m_vNSigDcyFSt[i]+m_vNCcSigDcyFSt[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNSigDcyFSt[i];
            }
          fout<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
          fout<<" ";
          list<int>::iterator liit=sigDcyFSt.begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" -->";
          for(liit++;liit!=sigDcyFSt.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<endl<<endl;
        }
    }

  if(m_vSigDcyFSt2.size()>0)
    {
      fout<<endl;

      fout<<"Signal decay final states:"<<endl<<endl;

      list<int> sigDcyFSt2;
      unsigned long nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigDcyFSt2.size();i++)
        {
          sigDcyFSt2.clear();
          sigDcyFSt2=m_vSigDcyFSt2[i];
          fout<<"index:  "<<i+1<<"\tiSigDcyFSt2:  "<<m_vISigDcyFSt2[i]<<"\tiDcyFSt2:  ";
          if(m_iSigDcyFSt2IDcyFStMap.find(m_vISigDcyFSt2[i])!=m_iSigDcyFSt2IDcyFStMap.end())      
            {
              if(m_iSigDcyFSt2ICcDcyFStMap[m_vISigDcyFSt2[i]]>=0) fout<<m_iSigDcyFSt2IDcyFStMap[m_vISigDcyFSt2[i]];
              else fout<<m_iSigDcyFSt2IDcyFStMap[m_vISigDcyFSt2[i]]<<"_cc";
            }
          else fout<<"---";
          fout<<"\tnEtrs:  "<<m_vNSigDcyFSt2[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcEtrs:  ";
              if(m_vICcSigDcyFSt2[i]==0) fout<<"---";
              else fout<<m_vNCcSigDcyFSt2[i];
              fout<<"\tnTotEtrs:  "<<m_vNSigDcyFSt2[i]+m_vNCcSigDcyFSt2[i];
              nCmltEtrs=nCmltEtrs+m_vNSigDcyFSt2[i]+m_vNCcSigDcyFSt2[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNSigDcyFSt2[i];
            }
          fout<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
          fout<<" ";
          list<int>::iterator liit=sigDcyFSt2.begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" -->";
          for(liit++;liit!=sigDcyFSt2.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<endl<<endl;
        }
    }
}
