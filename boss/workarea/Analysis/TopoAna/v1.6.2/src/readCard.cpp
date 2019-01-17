#include "../include/topoana.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>

void topoana::readCard(string topoAnaCardFlNm)
{
  ifstream fin(topoAnaCardFlNm.c_str(),ios::in);
  if(!fin)
    {
      cerr<<"Error: Can't open the card file \""<<topoAnaCardFlNm<<"\"!"<<endl;
      if(topoAnaCardFlNm=="topoana.card")
        {
          cerr<<"Infor: The card file name is the default one."<<endl;
          cerr<<"Infor: Do you forget to append the name of your own card file to the end of the executable?"<<endl;
          cerr<<"Infor: If yes, please specify it along with the executable by the command line \"(pathOfTheExecutable/)topoana.exe nameOfYourOwnCardFile\""<<endl;
        }
      else
        {
          cerr<<"Infor: Please check it."<<endl;
        }
      exit(-1);
    }  

  cout<<endl<<"Reading the card file: "<<topoAnaCardFlNm<<" ..."<<endl<<endl;

  string line="";
  while(!fin.eof())      
    {
      while(!fin.eof())
        {
          getline(fin,line);
          trim(line);
          if(!line.empty()&&line[0]!='#') break;
        }
      if(line.empty()||line[0]=='#')
        {
          break;
        }
      else if(line=="% Names of input root files")
        {
          readOpenCurly(fin,line,"% Names of input root files");
          read1stLineOrCloseCurly(fin,line,true,"No root file names are found in the item with the prompt \"% Names of input root files");
          m_nmsOfIptRootFls.push_back(line);
          while(1)
            {
              readExtraLinesOrCloseCurly(fin,line,"% Names of input root files");
              if(line=="}")
                {
                  break;
                }
              else 
                {
                  m_nmsOfIptRootFls.push_back(line);
                }
            }
	}
      else if(line=="% Tree name")
        {
          readOpenCurly(fin,line,"% Tree name");
          read1stLineOrCloseCurly(fin,line,true,"No tree name is found in the item with the prompt \"% Tree name");
          m_trNm=line;
          readCloseCurly(fin,line,"% Tree name");
        }
      else if(line=="% Branch name of the number of particles")
        {
          readOpenCurly(fin,line,"% Branch name of the number of particles");
          read1stLineOrCloseCurly(fin,line,true,"No branch name of the number of particles is found in the item with the prompt \"% Branch name of the number of particles");
          m_brNmOfNps=line;
          readCloseCurly(fin,line,"% Branch name of the number of particles");
        }
      else if(line=="% Branch name of the array of particle identifications")
        {
          readOpenCurly(fin,line,"% Branch name of the array of particle identifications");
          read1stLineOrCloseCurly(fin,line,true,"No branch name of the array of particle identifications is found in the item with the prompt \"% Branch name of the array of particle identifications");
          m_brNmOfPid=line;
          readCloseCurly(fin,line,"% Branch name of the array of particle identifications");     
        }
      else if(line=="% Branch name of the array of the mother indeces of particles")
        {
          readOpenCurly(fin,line,"% Branch name of the array of the mother indeces of particles");
          read1stLineOrCloseCurly(fin,line,true,"No branch name of the array of the mother indeces of particles is found in the item with the prompt \"% Branch name of the array of the mother indeces of particles");
          m_brNmOfMidx=line;
          readCloseCurly(fin,line,"% Branch name of the array of the mother indeces of particles");  
        }
      else if(line=="% Main name of output files")
        {
          readOpenCurly(fin,line,"% Main name of output files");
          read1stLineOrCloseCurly(fin,line,true,"No main file name is found in the item with the prompt \"% Main name of output files");
          m_mainNmOfOptFls=line;
          readCloseCurly(fin,line,"% Main name of output files");
        }
      else if(line=="% Maximum number of events to be processed")
        {
          readOpenCurly(fin,line,"% Maximum number of events to be processed");
          read1stLineOrCloseCurly(fin,line,false,"% Maximum number of events to be processed");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              iss.str(line);
              iss>>m_nEvtsMax;
              readCloseCurly(fin,line,"% Maximum number of events to be processed");
            }
        }
      else if(line=="% Cut to select events")
        {
          readOpenCurly(fin,line,"% Cut to select events");
          read1stLineOrCloseCurly(fin,line,false,"% Cut to select events");
          if(line!="}")
            {
              m_cut=line;
              readCloseCurly(fin,line,"% Cut to select events");
            }
        }
      else if(line=="% Method to apply cut to array variables (Two options: T and F. Default: F)")
        {
          readOpenCurly(fin,line,"% Method to apply cut to array variables (Two options: T and F. Default: F)");
          read1stLineOrCloseCurly(fin,line,false,"% Method to apply cut to array variables (Two options: T and F. Default: F)");
          if(line!="}")
            {
              if(line=="T")
                {
                  m_cutMethod=true;
                }
              else if(line!="F")
                {
                  cerr<<"Error: The input \""<<line<<"\" for the item \"Method to apply cut to array variables\" is invalid!"<<endl;
                  cerr<<"Infor: It should be \"T\" or \"F\"."<<endl;
                  cerr<<"Infor: Please check it."<<endl;
                  exit(-1);
                } 
              readCloseCurly(fin,line,"% Method to apply cut to array variables (Two options: T and F. Default: F)");
            }
        }
      else if(line=="% Process charge conjugate objects together (Two options: Y and N. Default: N)")
        {
          readOpenCurly(fin,line,"% Process charge conjugate objects together (Two options: Y and N. Default: N)");
          read1stLineOrCloseCurly(fin,line,false,"% Process charge conjugate objects together (Two options: Y and N. Default: N)");
          if(line!="}")
            {
              if(line=="Y")
                {
                  m_ccSwitch=true;
                }
              else if(line!="N")
                {
                  cerr<<"Error: The input \""<<line<<"\" for the item \"Process charge conjugate objects together\" is invalid!"<<endl;
                  cerr<<"Infor: It should be \"Y\" or \"N\"."<<endl;
                  cerr<<"Infor: Please check it."<<endl;
                  exit(-1);
                } 
              readCloseCurly(fin,line,"% Process charge conjugate objects together (Two options: Y and N. Default: N)");
            }
        }
      else if(line=="% Ignore ISR photons (Two options: Y and N. Default: Y)")
        {
          readOpenCurly(fin,line,"% Ignore ISR photons (Two options: Y and N. Default: Y)");
          read1stLineOrCloseCurly(fin,line,false,"% Ignore ISR photons (Two options: Y and N. Default: Y)");
          if(line!="}")
            {
              if(line=="N")
                {
                  m_ignoreISR=false;
                }
              else if(line!="Y")
                {
                  cerr<<"Error: The input \""<<line<<"\" for the item \"Ignore ISR photons\" is invalid!"<<endl;
                  cerr<<"Infor: It should be \"Y\" or \"N\"."<<endl;
                  cerr<<"Infor: Please check it."<<endl;
                  exit(-1);
                }
              readCloseCurly(fin,line,"% Ignore ISR photons (Two options: Y and N. Default: Y)");
            }
        }
      else if(line=="% Ignore gFSR photons (Two options: Y and N. Default: Y)")
        {
          readOpenCurly(fin,line,"% Ignore gFSR photons (Two options: Y and N. Default: Y)");
          read1stLineOrCloseCurly(fin,line,false,"% Ignore gFSR photons (Two options: Y and N. Default: Y)");
          if(line!="}")
            {
              if(line=="N")
                {
                  m_ignoreGFSR=false;
                }
              else if(line!="Y")
                {
                  cerr<<"Error: The input \""<<line<<"\" for the item \"Ignore gFSR photons\" is invalid!"<<endl;
                  cerr<<"Infor: It should be \"Y\" or \"N\"."<<endl;
                  cerr<<"Infor: Please check it."<<endl;
                  exit(-1);
                } 
              readCloseCurly(fin,line,"% Ignore gFSR photons (Two options: Y and N. Default: Y)");
            }
        }
      else if(line=="% Maximum hierarchy of heading event branches to be processed in each event")
        {
          readOpenCurly(fin,line,"% Maximum hierarchy of heading event branches to be processed in each event");
          read1stLineOrCloseCurly(fin,line,false,"% Maximum hierarchy of heading event branches to be processed in each event");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              iss.str(line);
              iss>>m_hHdEvtBrsMax;
              readCloseCurly(fin,line,"% Maximum hierarchy of heading event branches to be processed in each event");
            }
        }
      else if(line=="% Ignore the decay of the following particles")
        {
          readOpenCurly(fin,line,"% Ignore the decay of the following particles");
          read1stLineOrCloseCurly(fin,line,false,"% Ignore the decay of the following particles");
          if(line!="}")
            {
              int idPid=getPidFromTxtPnm(line);
              m_vIdPid.push_back(idPid);
              while(1)
                {
                  readExtraLinesOrCloseCurly(fin,line,"% Ignore the decay of the following particles");
                  if(line=="}")
                    {
                      break;
                    }
                  else
                    {
                      idPid=getPidFromTxtPnm(line);
                      m_vIdPid.push_back(idPid);
                    }
                }
            }
        }
      else if(line=="% Ignore the decay of the daughters of the following particles")
        {
          readOpenCurly(fin,line,"% Ignore the decay of the daughters of the following particles");
          read1stLineOrCloseCurly(fin,line,false,"% Ignore the decay of the daughters of the following particles");
          if(line!="}")
            {
              int iddPid=getPidFromTxtPnm(line);
              m_vIddPid.push_back(iddPid);
              while(1)
                {
                  readExtraLinesOrCloseCurly(fin,line,"% Ignore the decay of the daughters of the following particles");
                  if(line=="}")
                    {
                      break;
                    }
                  else
                    {
                      iddPid=getPidFromTxtPnm(line);
                      m_vIddPid.push_back(iddPid);
                    }
                }
            }
        }
      else if(line=="% Maximum number of event trees to be printed")
        {
          readOpenCurly(fin,line,"% Maximum number of event trees to be printed");
          read1stLineOrCloseCurly(fin,line,false,"% Maximum number of event trees to be printed");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              iss.str(line);
              iss>>m_nEvtTrsMax;
              readCloseCurly(fin,line,"% Maximum number of event trees to be printed");
            }
        }
      else if(line=="% Maximum number of event initial-final states to be printed")
        {
          readOpenCurly(fin,line,"% Maximum number of event initial-final states to be printed");
          read1stLineOrCloseCurly(fin,line,false,"% Maximum number of event initial-final states to be printed");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              iss.str(line);
              iss>>m_nEvtIFStsMax;
              readCloseCurly(fin,line,"% Maximum number of event initial-final states to be printed");
            }
        }
      else if(line=="% Suppress the first branches of event trees in the output txt/tex/pdf files (Two options: Y and N. Default: N)")
        {
          readOpenCurly(fin,line,"% Suppress the first branches of event trees in the output txt/tex/pdf files (Two options: Y and N. Default: N)");
          read1stLineOrCloseCurly(fin,line,false,"% Suppress the first branches of event trees in the output txt/tex/pdf files (Two options: Y and N. Default: N)");
          if(line!="}")
            {
              if(line=="Y")
                {
                  m_spr1stBrsOfEvtTrs=true;
                }
              else if(line!="N")
                {
                  cerr<<"Error: The input \""<<line<<"\" for the item \"Suppress the first branches of event trees in the output txt/tex/pdf files\" is invalid!"<<endl;
                  cerr<<"Infor: It should be \"Y\" or \"N\"."<<endl;
                  cerr<<"Infor: Please check it."<<endl;
                  exit(-1);
                }
              readCloseCurly(fin,line,"% Suppress the first branches of event trees in the output txt/tex/pdf files (Two options: Y and N. Default: N)");
            }
        }
      else if(line=="% Suppress the topology tags in the output tex/pdf files (Two options: Y and N. Default: N)")
        {
          readOpenCurly(fin,line,"% Suppress the topology tags in the output tex/pdf files (Two options: Y and N. Default: N)");
          read1stLineOrCloseCurly(fin,line,false,"% Suppress the topology tags in the output tex/pdf files (Two options: Y and N. Default: N)");
          if(line!="}")
            {
              if(line=="Y")
                {
                  m_sprTopoTags=true;
                }
              else if(line!="N")
                {
                  cerr<<"Error: The input \""<<line<<"\" for the item \"Suppress the topology tags in the output tex/pdf files\" is invalid!"<<endl;
                  cerr<<"Infor: It should be \"Y\" or \"N\"."<<endl;
                  cerr<<"Infor: Please check it."<<endl;
                  exit(-1);
                }
              readCloseCurly(fin,line,"% Suppress the topology tags in the output tex/pdf files (Two options: Y and N. Default: N)");
            }
        }
      else if(line=="% Skip the topology analysis of event trees and event initial-final states (Two options: Y and N. Default: N)")
        {
          readOpenCurly(fin,line,"% Skip the topology analysis of event trees and event initial-final states (Two options: Y and N. Default: N)");
          read1stLineOrCloseCurly(fin,line,false,"% Skip the topology analysis of event trees and event initial-final states (Two options: Y and N. Default: N)");
          if(line!="}")
            {
              if(line=="Y")
                {
                  m_sttaEvtTrsAndEvtIFSts=true;
                }
              else if(line!="N")
                {
                  cerr<<"Error: The input \""<<line<<"\" for the item \"Skip the topology analysis of event trees and event initial-final states\" is invalid!"<<endl;
                  cerr<<"Infor: It should be \"Y\" or \"N\"."<<endl;
                  cerr<<"Infor: Please check it."<<endl;
                  exit(-1);
                }
              readCloseCurly(fin,line,"% Skip the topology analysis of event trees and event initial-final states (Two options: Y and N. Default: N)");
            }
        }
      else if(line=="% Perform the topology analysis of the event branches begun with the following particles")
        {
          readOpenCurly(fin,line,"% Perform the topology analysis of the event branches begun with the following particles");
          read1stLineOrCloseCurly(fin,line,false,"% Perform the topology analysis of the event branches begun with the following particles");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              iss.str(line);
              string pttaTxtPnm;
              iss>>pttaTxtPnm;
              int pttaPid=getPidFromTxtPnm(pttaTxtPnm);
              m_vPttaPid.push_back(pttaPid);
              unsigned long nEvtBrsMax;
              if(!iss.eof()) iss>>nEvtBrsMax;
              else nEvtBrsMax=ULONG_MAX;
              m_vNEvtBrsMax.push_back(nEvtBrsMax);
              while(1)
                {
                  readExtraLinesOrCloseCurly(fin,line,"% Perform the topology analysis of the event branches begun with the following particles");
                  if(line=="}")
                    {
                      break;
                    }
                  else
                    {
                      iss.clear();
                      iss.str(line);
                      iss>>pttaTxtPnm;
                      pttaPid=getPidFromTxtPnm(pttaTxtPnm);
                      m_vPttaPid.push_back(pttaPid);
                      if(!iss.eof()) iss>>nEvtBrsMax;
                      else nEvtBrsMax=ULONG_MAX;
                      m_vNEvtBrsMax.push_back(nEvtBrsMax);
                    }
                }
            }
        }
      else if(line=="% Perform the topology analysis of the exclusive event branches matched with the following inclusive event branches")
        {
          readOpenCurly(fin,line,"% Perform the topology analysis of the exclusive event branches matched with the following inclusive event branches");
          read1stLineOrCloseCurly(fin,line,false,"% Perform the topology analysis of the exclusive event branches matched with the following inclusive event branches");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              int pttaIdx1;
              vector<int> vPttaPid1;
              vPttaPid1.clear();
              string pttaTxtPnm1;
              int pttaPid1;
              iss.str(line);
              iss>>pttaIdx1>>pttaTxtPnm1;
              pttaPid1=getPidFromTxtPnm(pttaTxtPnm1);
              vPttaPid1.push_back(pttaPid1);
              unsigned long nEvtBrsMax1;
              if(!iss.eof()) iss>>nEvtBrsMax1;
              else nEvtBrsMax1=ULONG_MAX;
              m_vNEvtBrsMax1.push_back(nEvtBrsMax1);
              iss.clear();
              while(1)
                {
                  readExtraLinesOrCloseCurly(fin,line,"% Perform the topology analysis of the exclusive event branches matched with the following inclusive event branches");
                  if(line=="}")
                    {
                      m_vVPttaPid1.push_back(vPttaPid1);
                      vPttaPid1.clear();
                      break;
                    }
                  else
                    {
                      iss.str(line);
                      iss>>pttaIdx1>>pttaTxtPnm1;
                      if(pttaIdx1==0)
                        {
                          m_vVPttaPid1.push_back(vPttaPid1);
                          vPttaPid1.clear();
                          if(!iss.eof()) iss>>nEvtBrsMax1;
                          else nEvtBrsMax1=ULONG_MAX;
                          m_vNEvtBrsMax1.push_back(nEvtBrsMax1);
                        }
                      pttaPid1=getPidFromTxtPnm(pttaTxtPnm1);
                      vPttaPid1.push_back(pttaPid1);
                      iss.clear();
                    }
                }
            }
	}
      else if(line=="% Signal particles")
        {
          readOpenCurly(fin,line,"% Signal particles");
          read1stLineOrCloseCurly(fin,line,false,"% Signal particles");
          if(line!="}")
            {
              int sigPid=getPidFromTxtPnm(line);
              m_vSigPid.push_back(sigPid);
              while(1)
                {
                  readExtraLinesOrCloseCurly(fin,line,"% Signal particles");
                  if(line=="}")
                    {
                      break;
                    }
                  else 
                    {
                      sigPid=getPidFromTxtPnm(line);
                      m_vSigPid.push_back(sigPid);
                    }
                }
            }
	}      
      else if(line=="% Signal inclusive event branches")
        {
          readOpenCurly(fin,line,"% Signal inclusive event branches");
          read1stLineOrCloseCurly(fin,line,false,"% Signal inclusive event branches");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              int sigIdx1;
              vector<int> vSigPid1;
              vSigPid1.clear();
              string sigTxtPnm1;
              int sigPid1;
              iss.str(line);
              iss>>sigIdx1>>sigTxtPnm1;
              sigPid1=getPidFromTxtPnm(sigTxtPnm1);
              vSigPid1.push_back(sigPid1);
              iss.clear();
              while(1)
                {
                  readExtraLinesOrCloseCurly(fin,line,"% Signal inclusive event branches");
                  if(line=="}")
                    {
                      m_vVSigPid1.push_back(vSigPid1);
                      vSigPid1.clear();
                      break;
                    }
                  else
                    {
                      iss.str(line);
                      iss>>sigIdx1>>sigTxtPnm1;
                      if(sigIdx1==0)
                        {
                          m_vVSigPid1.push_back(vSigPid1);
                          vSigPid1.clear();
                        }
                      sigPid1=getPidFromTxtPnm(sigTxtPnm1);
                      vSigPid1.push_back(sigPid1);
                      iss.clear();
                    }
                }
            }
	}
      else if(line=="% Signal sequential event branches")
        {
          readOpenCurly(fin,line,"% Signal sequential event branches");
          read1stLineOrCloseCurly(fin,line,false,"% Signal sequential event branches");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              int sigIdx2;
              vector<int> vSigPid2;
              vSigPid2.clear();
              string sigTxtPnm2;
              int sigPid2;
              vector<int> vSigMidx2;
              vSigMidx2.clear();
              int sigMidx2;
              iss.str(line);
              iss>>sigIdx2>>sigTxtPnm2>>sigMidx2;
              sigPid2=getPidFromTxtPnm(sigTxtPnm2);
              vSigPid2.push_back(sigPid2);
              vSigMidx2.push_back(sigMidx2);
              iss.clear();
              while(1)
                {
                  readExtraLinesOrCloseCurly(fin,line,"% Signal sequential event branches");
                  if(line=="}")
                    {
                      m_vVSigPid2.push_back(vSigPid2);
                      vSigPid2.clear();
                      m_vVSigMidx2.push_back(vSigMidx2);
                      vSigMidx2.clear();
                      break;
                    }
                  else
                    {
                      iss.str(line);
                      iss>>sigIdx2>>sigTxtPnm2>>sigMidx2;
                      if(sigIdx2==0)
                        {
                          m_vVSigPid2.push_back(vSigPid2);
                          vSigPid2.clear();
                          m_vVSigMidx2.push_back(vSigMidx2);
                          vSigMidx2.clear();
                        }
                      sigPid2=getPidFromTxtPnm(sigTxtPnm2);
                      vSigPid2.push_back(sigPid2);
                      vSigMidx2.push_back(sigMidx2);
                      iss.clear();
                    }
                }
            }
	}
      else if(line=="% Signal inclusive sequential event branches")
        {
          readOpenCurly(fin,line,"% Signal inclusive sequential event branches");
          read1stLineOrCloseCurly(fin,line,false,"% Signal inclusive sequential event branches");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              int sigIdx12;
              vector<int> vSigPid12;
              vSigPid12.clear();
              // The star symbol "*" is used as a short version of the word "anything" in order to simplify the user's input.
              string sigTxtPnm12,star="*";
              int sigPid12;
              vector<int> vSigMidx12;
              vSigMidx12.clear();
              int sigMidx12;
              
              iss.str(line);
              iss>>sigIdx12>>sigTxtPnm12>>sigMidx12;
              // Pay attention to that if sigTxtPnm12==star, then 1 is returned. Please do not get this wrong.
              if(sigTxtPnm12==star) sigTxtPnm12="anything";
              sigPid12=getPidFromTxtPnm(sigTxtPnm12);
              vSigPid12.push_back(sigPid12);
              vSigMidx12.push_back(sigMidx12);
              iss.clear();
              while(1)
                {
                  readExtraLinesOrCloseCurly(fin,line,"% Signal inclusive sequential event branches");
                  if(line=="}")
                    {
                      m_vVSigPid12.push_back(vSigPid12);
                      vSigPid12.clear();
                      m_vVSigMidx12.push_back(vSigMidx12);
                      vSigMidx12.clear();
                      break;
                    }
                  else
                    {
                      iss.str(line);
                      iss>>sigIdx12>>sigTxtPnm12>>sigMidx12;
                      if(sigIdx12==0)
                        {
                          m_vVSigPid12.push_back(vSigPid12);
                          vSigPid12.clear();
                          m_vVSigMidx12.push_back(vSigMidx12);
                          vSigMidx12.clear();
                        }
                      // Pay attention to that if sigTxtPnm12==star, then 1 is returned. Please do not get this wrong.
                      if(sigTxtPnm12==star) sigTxtPnm12="anything";
                      sigPid12=getPidFromTxtPnm(sigTxtPnm12);
                      vSigPid12.push_back(sigPid12);
                      vSigMidx12.push_back(sigMidx12);
                      iss.clear();
                    }
                }
            }
	}
      else if(line=="% Signal particle final states")
        {
          readOpenCurly(fin,line,"% Signal particle final states");          
          read1stLineOrCloseCurly(fin,line,false,"% Signal particle final states");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              int sigIdx3;
              vector<int> vSigPid3;
              vSigPid3.clear();
              string sigTxtPnm3;
              int sigPid3;
              iss.str(line);
              iss>>sigIdx3>>sigTxtPnm3;
              sigPid3=getPidFromTxtPnm(sigTxtPnm3);
              vSigPid3.push_back(sigPid3);
              iss.clear();
              while(1)
                {
                  readExtraLinesOrCloseCurly(fin,line,"% Signal particle final states");
                  if(line=="}")
                    {
                      m_vVSigPid3.push_back(vSigPid3);
                      vSigPid3.clear();
                      break;
                    }
                  else
                    {
                      iss.str(line);
                      iss>>sigIdx3>>sigTxtPnm3;
                      if(sigIdx3==0)
                        {
                          m_vVSigPid3.push_back(vSigPid3);
                          vSigPid3.clear();
                        }
                      sigPid3=getPidFromTxtPnm(sigTxtPnm3);
                      vSigPid3.push_back(sigPid3);
                      iss.clear();
                    }
                }
            }
	}
      else if(line=="% Signal event trees")
        {
          readOpenCurly(fin,line,"% Signal event trees");
          read1stLineOrCloseCurly(fin,line,false,"% Signal event trees");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              int sigIdx4;
              vector<int> vSigPid4;
              vSigPid4.clear();
              string sigTxtPnm4;
              int sigPid4;
              vector<int> vSigMidx4;
              vSigMidx4.clear();
              int sigMidx4;
              iss.str(line);
              iss>>sigIdx4>>sigTxtPnm4>>sigMidx4;
              sigPid4=getPidFromTxtPnm(sigTxtPnm4);
              vSigPid4.push_back(sigPid4);
              vSigMidx4.push_back(sigMidx4);
              iss.clear();
              while(1)
                {
                  readExtraLinesOrCloseCurly(fin,line,"% Signal event trees");
                  if(line=="}")
                    {
                      m_vVSigPid4.push_back(vSigPid4);
                      vSigPid4.clear();
                      m_vVSigMidx4.push_back(vSigMidx4);
                      vSigMidx4.clear();
                      break;
                    }
                  else
                    {
                      iss.str(line);
                      iss>>sigIdx4>>sigTxtPnm4>>sigMidx4;
                      if(sigIdx4==0)
                        {
                          m_vVSigPid4.push_back(vSigPid4);
                          vSigPid4.clear();
                          m_vVSigMidx4.push_back(vSigMidx4);
                          vSigMidx4.clear();
                        }
                      sigPid4=getPidFromTxtPnm(sigTxtPnm4);
                      vSigPid4.push_back(sigPid4);
                      vSigMidx4.push_back(sigMidx4);
                      iss.clear();
                    }
                }
            }
	}
      else if(line=="% Signal event final states")
        {
          readOpenCurly(fin,line,"% Signal event final states");
          read1stLineOrCloseCurly(fin,line,false,"% Signal event final states");
          if(line!="}")
            {
              istringstream iss;
              iss.clear();
              int sigIdx5;
              vector<int> vSigPid5;
              vSigPid5.clear();
              string sigTxtPnm5;
              int sigPid5;
              iss.str(line);
              iss>>sigIdx5>>sigTxtPnm5;
              sigPid5=getPidFromTxtPnm(sigTxtPnm5);
              vSigPid5.push_back(sigPid5);
              iss.clear();
              while(1)
                {
                  readExtraLinesOrCloseCurly(fin,line,"% Signal event final states");
                  if(line=="}")
                    {
                      m_vVSigPid5.push_back(vSigPid5);
                      vSigPid5.clear();
                      break;
                    }
                  else
                    {
                      iss.str(line);
                      iss>>sigIdx5>>sigTxtPnm5;
                      if(sigIdx5==0)
                        {
                          m_vVSigPid5.push_back(vSigPid5);
                          vSigPid5.clear();
                        }
                      sigPid5=getPidFromTxtPnm(sigTxtPnm5);
                      vSigPid5.push_back(sigPid5);
                      iss.clear();
                    }
                }
            }
	}
      else
        {
          cerr<<"Error: The input line "<<line<<"\" is invalid!"<<endl;
          exit(-1);
        }
    }
}
