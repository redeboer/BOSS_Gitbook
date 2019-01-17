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

  if(m_sttaEvtTrsAndEvtIFSts==false)
    {
      fout<<"Event trees and their respective initial-final states:"<<endl<<endl;
      vector< list<int> > evtTr;
      list<int> evtBr;
      list<int> evtIFSts;
      unsigned long nCmltEvts=0;
      unsigned long nEvtTrsToBePrtd=m_vEvtTr.size()<m_nEvtTrsMax?m_vEvtTr.size():m_nEvtTrsMax; 
      for(unsigned long i=0;i<nEvtTrsToBePrtd;i++)
        { 
          evtTr.clear();
          evtTr=m_vEvtTr[i];
          fout<<"index:  "<<i+1<<"\tiEvtTr:  "<<m_vIEvtTr[i]<<"\tiEvtIFSts:  "<<m_iEvtTrIEvtIFStsMap[m_vIEvtTr[i]]<<"\tnEvts:  "<<m_vNEvtTr[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcEvts:  ";
              if(m_vICcEvtTr[i]==0) fout<<"---";
              else fout<<m_vNCcEvtTr[i];
              fout<<"\tnTotEvts:  "<<m_vNEvtTr[i]+m_vNCcEvtTr[i];
              nCmltEvts=nCmltEvts+m_vNEvtTr[i]+m_vNCcEvtTr[i];
            }
          else
            {
              nCmltEvts=nCmltEvts+m_vNEvtTr[i];
            }
          fout<<"\tnCmltEvts:  "<<nCmltEvts<<endl;

          for(unsigned int j=0;j<evtTr.size();j++)
            {
              if(j==0&&m_spr1stBrsOfEvtTrs==true&&evtTr[0].size()==3) continue; 
              evtBr.clear();
              evtBr=evtTr[j];
              fout<<" ";
              list<int>::iterator liit=evtBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              // The condition "j==0" is set for the initial state particle pair e+e-;the condition "(*liit)==1, 2, 3, 4, 5 or 6" is set for the intermediate state quark pair ddbar, uubar, ssbar, ccbar, bbbar or ttbar;
              if(j==0||(*liit)==1||(*liit)==2||(*liit)==3||(*liit)==4||(*liit)==5||(*liit)==6)
                {
                  liit++;
                  writePnmFromPid(fout,"TxtPnm",(*liit));
                }
              fout<<" -->";
              for(liit++;liit!=evtBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<endl;
            }

          for(unsigned int j=0;j<m_vEvtIFSts.size();j++)
            {
              if(m_vIEvtIFSts[j]==m_iEvtTrIEvtIFStsMap[m_vIEvtTr[i]])
                {
                  evtIFSts.clear();
                  if(m_iEvtTrICcEvtIFStsMap[m_vIEvtTr[i]]!=-1) evtIFSts=m_vEvtIFSts[j];
                  else evtIFSts=m_vCcEvtIFSts[j];
                  break;
                }
            }
          fout<<"(";
          list<int>::iterator liit;
          if(m_spr1stBrsOfEvtTrs==true&&evtTr[0].size()==3)
            {
              liit=evtTr[0].begin();
              liit++;
              liit++;
              writePnmFromPid(fout,"TxtPnm",(*liit));
              liit=evtIFSts.begin();
              liit++;
            }
          else
            {
              liit=evtIFSts.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              liit++;
              writePnmFromPid(fout,"TxtPnm",(*liit));
            }
          fout<<" -->";
          for(liit++;liit!=evtIFSts.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" )"<<endl; 

          fout<<endl;
        }
      if(nEvtTrsToBePrtd<m_vEvtTr.size())
        {
          fout<<"index:  "<<"rest"<<"\tiEvtTr:  "<<"---"<<"\tiEvtIFSts:  "<<"---"<<"\tnEvts:  ";
          unsigned long nCmltEvtsOfRest=0;
          if(m_ccSwitch==true)
            {
              fout<<"---"<<"\tnCcEvts:  "<<"---"<<"\tnTotEvts:  ";
              for(unsigned long i=nEvtTrsToBePrtd;i<m_vEvtTr.size();i++) nCmltEvtsOfRest=nCmltEvtsOfRest+m_vNEvtTr[i]+m_vNCcEvtTr[i];
            }
          else
            {
              for(unsigned long i=nEvtTrsToBePrtd;i<m_vEvtTr.size();i++) nCmltEvtsOfRest=nCmltEvtsOfRest+m_vNEvtTr[i];
            }
          nCmltEvts=nCmltEvts+nCmltEvtsOfRest;
          fout<<nCmltEvtsOfRest<<"\tnCmltEvts:  "<<nCmltEvts<<endl;    

          fout<<" ";
          if(m_spr1stBrsOfEvtTrs==true)
            {
              bool isEpEmToAPInAllOtherEvtTrs=true;
              list<int>::iterator liit=m_vEvtTr[nEvtTrsToBePrtd][0].begin();
              liit++;
              liit++;
              int pid=(*liit);
              for(unsigned long i=nEvtTrsToBePrtd;i<m_vEvtTr.size();i++)
                { 
                  if(isEpEmToAPInAllOtherEvtTrs==true)
                    { 
                      if(m_vEvtTr[i][0].size()!=3)
                        { 
                          isEpEmToAPInAllOtherEvtTrs=false;
                        }
                      else
                        { 
                          liit=m_vEvtTr[i][0].begin();
                          liit++;
                          liit++;
                          if((*liit)!=pid) isEpEmToAPInAllOtherEvtTrs=false;
                        }
                    }
                }
              if(isEpEmToAPInAllOtherEvtTrs==true)
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

      fout<<"Event initial-final states:"<<endl<<endl;
      //list<int> evtIFSts; The list<int> variable evtIFSts has been previously declared.
      nCmltEvts=0;
      unsigned long nEvtIFStsToBePrtd=m_vEvtIFSts.size()<m_nEvtIFStsMax?m_vEvtIFSts.size():m_nEvtIFStsMax;
      for(unsigned long i=0;i<nEvtIFStsToBePrtd;i++)
        {
          evtIFSts.clear();
          evtIFSts=m_vEvtIFSts[i];
          fout<<"index:  "<<i+1<<"\tiEvtIFSts:  "<<m_vIEvtIFSts[i]<<"\tnEvts:  "<<m_vNEvtIFSts[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcEvts:  ";
              if(m_vICcEvtIFSts[i]==0) fout<<"---";
              else fout<<m_vNCcEvtIFSts[i];
              fout<<"\tnTotEvts:  "<<m_vNEvtIFSts[i]+m_vNCcEvtIFSts[i]; 
              nCmltEvts=nCmltEvts+m_vNEvtIFSts[i]+m_vNCcEvtIFSts[i];
            }
          else
            {
              nCmltEvts=nCmltEvts+m_vNEvtIFSts[i];
            }
          fout<<"\tnCmltEvts:  "<<nCmltEvts<<endl;
          fout<<" ";
          list<int>::iterator liit=evtIFSts.begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" -->";
          for(liit++;liit!=evtIFSts.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<endl<<endl;
        }
      if(nEvtIFStsToBePrtd<m_vEvtIFSts.size())
        {
          fout<<"index:  "<<"rest"<<"\tiEvtIFSts:  "<<"---"<<"\tnEvts:  ";
          unsigned long nCmltEvtsOfRest=0;
          if(m_ccSwitch==true)
            {
              fout<<"---"<<"\tnCcEvts:  "<<"---"<<"\tnTotEvts:  ";  
              for(unsigned long i=nEvtIFStsToBePrtd;i<m_vEvtIFSts.size();i++) nCmltEvtsOfRest=nCmltEvtsOfRest+m_vNEvtIFSts[i]+m_vNCcEvtIFSts[i];
            }
          else
            {
              for(unsigned long i=nEvtIFStsToBePrtd;i<m_vEvtIFSts.size();i++) nCmltEvtsOfRest=nCmltEvtsOfRest+m_vNEvtIFSts[i];
            }
          nCmltEvts=nCmltEvts+nCmltEvtsOfRest;
          fout<<nCmltEvtsOfRest<<"\tnCmltEvts:  "<<nCmltEvts<<endl;

          fout<<" ";
          writePnmFromPid(fout,"TxtPnm",-11);
          writePnmFromPid(fout,"TxtPnm",11);
          fout<<" --> others"<<endl;
          
          fout<<endl;
        }
    }

  if(m_vVEvtBrP.size()>0)
    {
      for(unsigned int i=0;i<m_vVEvtBrP.size();i++)
        {
          fout<<endl;

          fout<<"Event branches begun with";
          writePnmFromPid(fout,"TxtPnm",m_vPttaPid[i]);
          fout<<":"<<endl<<endl;

          list<int> evtBrP;
          unsigned long nCmltEtrs=0;
          unsigned long nEvtBrPToBePrtd=m_vVEvtBrP[i].size()<m_vNEvtBrsMax[i]?m_vVEvtBrP[i].size():m_vNEvtBrsMax[i];
          for(unsigned int j=0;j<nEvtBrPToBePrtd;j++)
            {
              evtBrP.clear();
              evtBrP=m_vVEvtBrP[i][j];
              fout<<"index:  "<<j+1<<"\tiEvtBrP"<<i+1<<":  "<<m_vVIEvtBrP[i][j]<<"\tnEtrs:  "<<m_vVNEvtBrP[i][j];
              if(m_ccSwitch==true)
                {
                  fout<<"\tnCcEtrs:  ";
                  if(m_vPttaICcPid[i]==0&&m_vVIEvtBrCcP[i][j]==0) fout<<"---";
                  else fout<<m_vVNEvtBrCcP[i][j];
                  fout<<"\tnTotEtrs:  "<<m_vVNEvtBrP[i][j]+m_vVNEvtBrCcP[i][j]; 
                  nCmltEtrs=nCmltEtrs+m_vVNEvtBrP[i][j]+m_vVNEvtBrCcP[i][j];
                }
              else
                {
                  nCmltEtrs=nCmltEtrs+m_vVNEvtBrP[i][j];
                }
              fout<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
              fout<<" ";
              list<int>::iterator liit=evtBrP.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<" -->";
              for(liit++;liit!=evtBrP.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<endl<<endl;
            }
          if(nEvtBrPToBePrtd<m_vVEvtBrP[i].size())
            {
              fout<<"index:  "<<"rest"<<"\tiEvtBrP"<<i+1<<":  "<<"---"<<"\tnEtrs:  ";
              unsigned long nCmltEtrsOfRest=0;
              if(m_ccSwitch==true)
                {
                  if(m_vPttaICcPid[i]==0)
                    {
                      fout<<"---"<<"\tnCcEtrs:  "<<"---"<<"\tnTotEtrs:  ";
                    }
                  else
                    {
                      unsigned long nCmltEtrsOfRestTemp1=0;
                      unsigned long nCmltEtrsOfRestTemp2=0;
                      for(unsigned int j=nEvtBrPToBePrtd;j<m_vVEvtBrP[i].size();j++) nCmltEtrsOfRestTemp1=nCmltEtrsOfRestTemp1+m_vVNEvtBrP[i][j];
                      for(unsigned int j=nEvtBrPToBePrtd;j<m_vVEvtBrP[i].size();j++) nCmltEtrsOfRestTemp2=nCmltEtrsOfRestTemp2+m_vVNEvtBrCcP[i][j];
                      fout<<nCmltEtrsOfRestTemp1<<"\tnCcEtrs:  "<<nCmltEtrsOfRestTemp2<<"\tnTotEtrs:  ";
                    }
                  for(unsigned int j=nEvtBrPToBePrtd;j<m_vVEvtBrP[i].size();j++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vVNEvtBrP[i][j]+m_vVNEvtBrCcP[i][j];
                }
              else
                {
                  for(unsigned int j=nEvtBrPToBePrtd;j<m_vVEvtBrP[i].size();j++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vVNEvtBrP[i][j];
                }
              nCmltEtrs=nCmltEtrs+nCmltEtrsOfRest;
              fout<<nCmltEtrsOfRest<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
              fout<<" ";
              list<int>::iterator liit=evtBrP.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<" --> others";
              fout<<endl<<endl;
            }
        }
    }

  if(m_vVEvtBrIncEvtBr.size()>0)
    {
      for(unsigned int i=0;i<m_vVEvtBrIncEvtBr.size();i++)
        {
          fout<<endl;

          fout<<"Exclusive event branches matched with";
          list<int>::iterator liit=m_vPttaIncEvtBr[i].begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          if((*liit)==-11)
            {
              liit++;
              writePnmFromPid(fout,"TxtPnm",(*liit));
            }
          fout<<" -->";
          for(liit++;liit!=m_vPttaIncEvtBr[i].end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" + anything:"<<endl<<endl;

          list<int> evtBrIncEvtBr;
          unsigned long nCmltEtrs=0;
          unsigned long nEvtBrIncEvtBrToBePrtd=m_vVEvtBrIncEvtBr[i].size()<m_vNEvtBrsMax1[i]?m_vVEvtBrIncEvtBr[i].size():m_vNEvtBrsMax1[i];
          for(unsigned int j=0;j<nEvtBrIncEvtBrToBePrtd;j++)
            {
              evtBrIncEvtBr.clear();
              evtBrIncEvtBr=m_vVEvtBrIncEvtBr[i][j];
              fout<<"index:  "<<j+1<<"\tiEvtBrIncEvtBr"<<i+1<<":  "<<m_vVIEvtBrIncEvtBr[i][j]<<"\tnEtrs:  "<<m_vVNEvtBrIncEvtBr[i][j];
              if(m_ccSwitch==true)
                {
                  fout<<"\tnCcEtrs:  ";
                  if(m_vPttaICcPid[i]==0&&m_vVIEvtBrCcIncEvtBr[i][j]==0) fout<<"---";
                  else fout<<m_vVNEvtBrCcIncEvtBr[i][j];
                  fout<<"\tnTotEtrs:  "<<m_vVNEvtBrIncEvtBr[i][j]+m_vVNEvtBrCcIncEvtBr[i][j]; 
                  nCmltEtrs=nCmltEtrs+m_vVNEvtBrIncEvtBr[i][j]+m_vVNEvtBrCcIncEvtBr[i][j];
                }
              else
                {
                  nCmltEtrs=nCmltEtrs+m_vVNEvtBrIncEvtBr[i][j];
                }
              fout<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
              fout<<" ";
              list<int>::iterator liit=evtBrIncEvtBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              if((*liit)==-11)
                {
                  liit++;
                  writePnmFromPid(fout,"TxtPnm",(*liit));
                }
              fout<<" -->";
              for(liit++;liit!=evtBrIncEvtBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<endl<<endl;
            }
          if(nEvtBrIncEvtBrToBePrtd<m_vVEvtBrIncEvtBr[i].size())
            {
              fout<<"index:  "<<"rest"<<"\tiEvtBrIncEvtBr"<<i+1<<":  "<<"---"<<"\tnEtrs:  ";
              unsigned long nCmltEtrsOfRest=0;
              if(m_ccSwitch==true)
                {
                  if(m_vPttaICcPid[i]==0)
                    {
                      fout<<"---"<<"\tnCcEtrs:  "<<"---"<<"\tnTotEtrs:  ";
                    }
                  else
                    {
                      unsigned long nCmltEtrsOfRestTemp1=0;
                      unsigned long nCmltEtrsOfRestTemp2=0;
                      for(unsigned int j=nEvtBrIncEvtBrToBePrtd;j<m_vVEvtBrIncEvtBr[i].size();j++) nCmltEtrsOfRestTemp1=nCmltEtrsOfRestTemp1+m_vVNEvtBrIncEvtBr[i][j];
                      for(unsigned int j=nEvtBrIncEvtBrToBePrtd;j<m_vVEvtBrIncEvtBr[i].size();j++) nCmltEtrsOfRestTemp2=nCmltEtrsOfRestTemp2+m_vVNEvtBrCcIncEvtBr[i][j];
                      fout<<nCmltEtrsOfRestTemp1<<"\tnCcEtrs:  "<<nCmltEtrsOfRestTemp2<<"\tnTotEtrs:  ";
                    }
                  for(unsigned int j=nEvtBrIncEvtBrToBePrtd;j<m_vVEvtBrIncEvtBr[i].size();j++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vVNEvtBrIncEvtBr[i][j]+m_vVNEvtBrCcIncEvtBr[i][j];
                }
              else
                {
                  for(unsigned int j=nEvtBrIncEvtBrToBePrtd;j<m_vVEvtBrIncEvtBr[i].size();j++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vVNEvtBrIncEvtBr[i][j];
                }
              nCmltEtrs=nCmltEtrs+nCmltEtrsOfRest;
              fout<<nCmltEtrsOfRest<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
              fout<<" ";
              list<int>::iterator liit=evtBrIncEvtBr.begin();
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

  if(m_vSigIncEvtBr.size()>0)
    {
      fout<<endl;

      fout<<"Signal inclusive event branches:"<<endl<<endl;

      list<int> sigIncEvtBr;
      unsigned long nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigIncEvtBr.size();i++)
        {
          sigIncEvtBr.clear();
          sigIncEvtBr=m_vSigIncEvtBr[i];
          fout<<"index:  "<<i+1<<"\tiSigIncEvtBr:  "<<m_vISigIncEvtBr[i]<<"\tnEtrs:  "<<m_vNSigIncEvtBr[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcEtrs:  ";
              if(m_vICcSigIncEvtBr[i]>0) fout<<m_vNCcSigIncEvtBr[i];
              else fout<<"---";
              fout<<"\tnTotEtrs:  "<<m_vNSigIncEvtBr[i]+m_vNCcSigIncEvtBr[i];
              nCmltEtrs=nCmltEtrs+m_vNSigIncEvtBr[i]+m_vNCcSigIncEvtBr[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNSigIncEvtBr[i];
            }
          fout<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
          fout<<" ";
          list<int>::iterator liit=sigIncEvtBr.begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          if((*liit)==-11)
            {
              liit++;
              writePnmFromPid(fout,"TxtPnm",(*liit));
            }
          fout<<" -->";
          for(liit++;liit!=sigIncEvtBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" + anything";
          fout<<endl<<endl;
        }
    }

  if(m_vSigSeqEvtBrs.size()>0)
    {
      fout<<endl;

      fout<<"Signal sequential event branches:"<<endl<<endl;
      vector< list<int> > sigSeqEvtBrs;
      vector<int> vSigSeqIdxOfHead;
      list<int> sigEvtBr;
      unsigned long nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigSeqEvtBrs.size();i++)
        { 
          sigSeqEvtBrs.clear();
          sigSeqEvtBrs=m_vSigSeqEvtBrs[i];
          vSigSeqIdxOfHead=m_vVSigSeqIdxOfHead[i];
          fout<<"index:  "<<i+1<<"\tiSigSeqEvtBrs:  "<<m_vISigSeqEvtBrs[i]<<"\tnEtrs:  "<<m_vNSigSeqEvtBrs[i];
          if(m_ccSwitch==true)
            { 
              fout<<"\tnCcEtrs:  ";
              if(m_vICcSigSeqEvtBrs[i]>0) fout<<m_vNCcSigSeqEvtBrs[i];
              else fout<<"---";
              fout<<"\tnTotEtrs:  "<<m_vNSigSeqEvtBrs[i]+m_vNCcSigSeqEvtBrs[i];
              nCmltEtrs=nCmltEtrs+m_vNSigSeqEvtBrs[i]+m_vNCcSigSeqEvtBrs[i];
            }
          else
            { 
              nCmltEtrs=nCmltEtrs+m_vNSigSeqEvtBrs[i];
            }
          fout<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
          for(unsigned int j=0;j<sigSeqEvtBrs.size();j++)
            {
              sigEvtBr.clear();
              sigEvtBr=sigSeqEvtBrs[j];
              fout<<" ";
              list<int>::iterator liit=sigEvtBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              if(j==0&&vSigSeqIdxOfHead[0]==-1)
              {
                liit++;
                writePnmFromPid(fout,"TxtPnm",(*liit));
              }
              fout<<" -->";
              for(liit++;liit!=sigEvtBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<endl;
            }
          fout<<endl;
        }
    }

  if(m_vSigIncSeqEvtBrs.size()>0)
    {
      fout<<endl;

      fout<<"Signal inclusive sequential event branches:"<<endl<<endl;
      vector< list<int> > sigIncSeqEvtBrs;
      vector<int> vSigIncSeqIdxOfHead;
      list<int> sigEvtBr;
      unsigned long nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigIncSeqEvtBrs.size();i++)
        { 
          sigIncSeqEvtBrs.clear();
          sigIncSeqEvtBrs=m_vSigIncSeqEvtBrs[i];
          vSigIncSeqIdxOfHead=m_vVSigIncSeqIdxOfHead[i];
          fout<<"index:  "<<i+1<<"\tiSigIncSeqEvtBrs:  "<<m_vISigIncSeqEvtBrs[i]<<"\tnEtrs:  "<<m_vNSigIncSeqEvtBrs[i];
          if(m_ccSwitch==true)
            { 
              fout<<"\tnCcEtrs:  ";
              if(m_vICcSigIncSeqEvtBrs[i]>0) fout<<m_vNCcSigIncSeqEvtBrs[i];
              else fout<<"---";
              fout<<"\tnTotEtrs:  "<<m_vNSigIncSeqEvtBrs[i]+m_vNCcSigIncSeqEvtBrs[i];
              nCmltEtrs=nCmltEtrs+m_vNSigIncSeqEvtBrs[i]+m_vNCcSigIncSeqEvtBrs[i];
            }
          else
            { 
              nCmltEtrs=nCmltEtrs+m_vNSigIncSeqEvtBrs[i];
            }
          fout<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
          for(unsigned int j=0;j<sigIncSeqEvtBrs.size();j++)
            {
              sigEvtBr.clear();
              sigEvtBr=sigIncSeqEvtBrs[j];
              fout<<" ";
              list<int>::iterator liit=sigEvtBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              if(j==0&&vSigIncSeqIdxOfHead[0]==-1)
              {
                liit++;
                writePnmFromPid(fout,"TxtPnm",(*liit));
              }
              fout<<" -->";
              for(liit++;liit!=sigEvtBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<endl;
            }
          fout<<endl;
        }
    }

  if(m_vSigPFSts.size()>0)
    {
      fout<<endl;

      fout<<"Signal particle final states:"<<endl<<endl;

      list<int> sigPFSts;
      unsigned long nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigPFSts.size();i++)
        {
          sigPFSts.clear();
          sigPFSts=m_vSigPFSts[i];
          fout<<"index:  "<<i+1<<"\tiSigPFSts:  "<<m_vISigPFSts[i]<<"\tnEtrs:  "<<m_vNSigPFSts[i];
          if(m_ccSwitch==true)
            { 
              fout<<"\tnCcEtrs:  ";
              if(m_vICcSigPFSts[i]>0) fout<<m_vNCcSigPFSts[i];
              else fout<<"---";
              fout<<"\tnTotEtrs:  "<<m_vNSigPFSts[i]+m_vNCcSigPFSts[i];
              nCmltEtrs=nCmltEtrs+m_vNSigPFSts[i]+m_vNCcSigPFSts[i];
            }
          else
            { 
              nCmltEtrs=nCmltEtrs+m_vNSigPFSts[i];
            }
          fout<<"\tnCmltEtrs:  "<<nCmltEtrs<<endl;
          fout<<" ";
          list<int>::iterator liit=sigPFSts.begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" -->";
          for(liit++;liit!=sigPFSts.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<endl<<endl;
        }
    }

  if(m_vSigEvtTr.size()>0)
    {
      fout<<endl;

      fout<<"Signal event trees and their respective initial-final states:"<<endl<<endl;
      vector< list<int> > sigEvtTr;
      list<int> sigEvtBr;
      list<int> sigEvtIFSts;
      unsigned long nCmltEvts=0;
      for(unsigned int i=0;i<m_vSigEvtTr.size();i++)
        { 
          sigEvtTr.clear();
          sigEvtTr=m_vSigEvtTr[i];
          fout<<"index:  "<<i+1<<"\tiSigEvtTr:  "<<m_vISigEvtTr[i]<<"\tiSigEvtIFSts:  ";
          if(m_iSigEvtTrICcSigEvtIFStsMap[m_vISigEvtTr[i]]>=0) fout<<m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]];
          else fout<<m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]]<<"_cc";
          fout<<"\tiEvtTr:  ";
          if(m_iSigEvtTrIEvtTrMap.find(m_vISigEvtTr[i])!=m_iSigEvtTrIEvtTrMap.end())
            {
              if(m_iSigEvtTrICcEvtTrMap[m_vISigEvtTr[i]]>=0) fout<<m_iSigEvtTrIEvtTrMap[m_vISigEvtTr[i]];
              else fout<<m_iSigEvtTrIEvtTrMap[m_vISigEvtTr[i]]<<"_cc";
            }
          else fout<<"---";
          fout<<"\tiEvtIFSts:  ";
          if(m_iSigEvtIFStsIEvtIFStsMap.find(m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]])!=m_iSigEvtIFStsIEvtIFStsMap.end()) 
            {
              if(m_iSigEvtIFStsICcEvtIFStsMap[m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]]]>=0) fout<<m_iSigEvtIFStsIEvtIFStsMap[m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]]];
              else fout<<m_iSigEvtIFStsIEvtIFStsMap[m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]]]<<"_cc";
            }
          else fout<<"---";
          fout<<"\tnEvts:  "<<m_vNSigEvtTr[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcEvts:  ";
              if(m_vICcSigEvtTr[i]==0) fout<<"---";
              else fout<<m_vNCcSigEvtTr[i];
              fout<<"\tnTotEvts:  "<<m_vNSigEvtTr[i]+m_vNCcSigEvtTr[i];
              nCmltEvts=nCmltEvts+m_vNSigEvtTr[i]+m_vNCcSigEvtTr[i];
            }
          else
            {
              nCmltEvts=nCmltEvts+m_vNSigEvtTr[i];
            }
          fout<<"\tnCmltEvts:  "<<nCmltEvts<<endl;
          for(unsigned int j=0;j<sigEvtTr.size();j++)
            {
              sigEvtBr.clear();
              sigEvtBr=sigEvtTr[j];
              fout<<" ";
              list<int>::iterator liit=sigEvtBr.begin();
              writePnmFromPid(fout,"TxtPnm",(*liit));
              // The condition "j==0" is set for the initial state particle pair e+e-;the condition "(*liit)==1, 2, 3, 4, 5 or 6" is set for the intermediate state quark pair ddbar, uubar, ssbar, ccbar, bbbar or ttbar;
              if(j==0||(*liit)==1||(*liit)==2||(*liit)==3||(*liit)==4||(*liit)==5||(*liit)==6)
                {
                  liit++;
                  writePnmFromPid(fout,"TxtPnm",(*liit));
                }
              fout<<" -->";
              for(liit++;liit!=sigEvtBr.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
              fout<<endl;
            }

          for(unsigned int j=0;j<m_vSigEvtIFSts.size();j++)
            {
              if(m_vISigEvtIFSts[j]==m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]])
                {
                  sigEvtIFSts.clear();
                  sigEvtIFSts=m_vSigEvtIFSts[j];
                  break;
                }
            }
          fout<<"(";
          list<int>::iterator liit=sigEvtIFSts.begin();
          writePnmFromPid(fout,"TxtPnm",(*liit)); 
          liit++;
          writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" -->";
          for(liit++;liit!=sigEvtIFSts.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" )"<<endl;

          fout<<endl;
        }

      fout<<endl;

      fout<<"Signal event initial-final states corresponding to signal event trees:"<<endl<<endl;
      //list<int> sigEvtIFSts; The list<int> variable sigEvtIFSts has been previously declared.
      nCmltEvts=0;
      for(unsigned int i=0;i<m_vSigEvtIFSts.size();i++)
        {
          sigEvtIFSts.clear();
          sigEvtIFSts=m_vSigEvtIFSts[i];
          fout<<"index:  "<<i+1<<"\tiSigEvtIFSts:  "<<m_vISigEvtIFSts[i]<<"\tiEvtIFSts:  ";
          if(m_iSigEvtIFStsIEvtIFStsMap.find(m_vISigEvtIFSts[i])!=m_iSigEvtIFStsIEvtIFStsMap.end())
            {
              if(m_iSigEvtIFStsICcEvtIFStsMap[m_vISigEvtIFSts[i]]>=0) fout<<m_iSigEvtIFStsIEvtIFStsMap[m_vISigEvtIFSts[i]];
              else fout<<m_iSigEvtIFStsIEvtIFStsMap[m_vISigEvtIFSts[i]]<<"_cc";
            }
          else fout<<"---";
          fout<<"\tnEvts:  "<<m_vNSigEvtIFSts[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcEvts:  ";
              if(m_vICcSigEvtIFSts[i]==0) fout<<"---";
              else fout<<m_vNCcSigEvtIFSts[i];
              fout<<"\tnTotEvts:  "<<m_vNSigEvtIFSts[i]+m_vNCcSigEvtIFSts[i]; 
              nCmltEvts=nCmltEvts+m_vNSigEvtIFSts[i]+m_vNCcSigEvtIFSts[i];
            }
          else
            {
              nCmltEvts=nCmltEvts+m_vNSigEvtIFSts[i];
            }
          fout<<"\tnCmltEvts:  "<<nCmltEvts<<endl;
          fout<<" ";
          list<int>::iterator liit=sigEvtIFSts.begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" -->";
          for(liit++;liit!=sigEvtIFSts.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<endl<<endl;
        }
    }

  if(m_vSigEvtIFSts2.size()>0)
    {
      fout<<endl;

      fout<<"Signal event initial-final states:"<<endl<<endl;

      list<int> sigEvtIFSts2;
      unsigned long nCmltEvts=0;
      for(unsigned int i=0;i<m_vSigEvtIFSts2.size();i++)
        {
          sigEvtIFSts2.clear();
          sigEvtIFSts2=m_vSigEvtIFSts2[i];
          fout<<"index:  "<<i+1<<"\tiSigEvtIFSts2:  "<<m_vISigEvtIFSts2[i]<<"\tiEvtIFSts2:  ";
          if(m_iSigEvtIFSts2IEvtIFStsMap.find(m_vISigEvtIFSts2[i])!=m_iSigEvtIFSts2IEvtIFStsMap.end())      
            {
              if(m_iSigEvtIFSts2ICcEvtIFStsMap[m_vISigEvtIFSts2[i]]>=0) fout<<m_iSigEvtIFSts2IEvtIFStsMap[m_vISigEvtIFSts2[i]];
              else fout<<m_iSigEvtIFSts2IEvtIFStsMap[m_vISigEvtIFSts2[i]]<<"_cc";
            }
          else fout<<"---";
          fout<<"\tnEvts:  "<<m_vNSigEvtIFSts2[i];
          if(m_ccSwitch==true)
            {
              fout<<"\tnCcEvts:  ";
              if(m_vICcSigEvtIFSts2[i]==0) fout<<"---";
              else fout<<m_vNCcSigEvtIFSts2[i];
              fout<<"\tnTotEvts:  "<<m_vNSigEvtIFSts2[i]+m_vNCcSigEvtIFSts2[i];
              nCmltEvts=nCmltEvts+m_vNSigEvtIFSts2[i]+m_vNCcSigEvtIFSts2[i];
            }
          else
            {
              nCmltEvts=nCmltEvts+m_vNSigEvtIFSts2[i];
            }
          fout<<"\tnCmltEvts:  "<<nCmltEvts<<endl;
          fout<<" ";
          list<int>::iterator liit=sigEvtIFSts2.begin();
          writePnmFromPid(fout,"TxtPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<" -->";
          for(liit++;liit!=sigEvtIFSts2.end();liit++) writePnmFromPid(fout,"TxtPnm",(*liit));
          fout<<endl<<endl;
        }
    }
}
