#include "../include/topoana.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>

void topoana::writeRsltIntoTexFl()
{
  string NmOfOptTexFl=m_mainNmOfOptFls+".tex";
  ofstream fout(NmOfOptTexFl.c_str(),ios::out);
  if(!fout)
    {
      cerr<<"Error: Can't create the output tex file \""<<NmOfOptTexFl<<"\"!"<<endl;
      cerr<<"Infor: Please check it."<<endl;
      exit(-1);
    }

  fout<<"\\documentclass[landscape]{article}"<<endl;
  string pathOfGeometryStyFl=m_pkgPath+"share/";
  fout<<"\\usepackage{"<<pathOfGeometryStyFl<<"geometry}"<<endl;
  fout<<"\\usepackage[colorlinks,linkcolor=blue]{hyperref}"<<endl;
  //fout<<"\\usepackage{array}"<<endl;
  string pathOfMakeCellStyFl=m_pkgPath+"share/";
  fout<<"\\usepackage{"<<pathOfMakeCellStyFl<<"makecell}"<<endl;
  fout<<"\\usepackage{color}"<<endl;
  fout<<"\\setcellgapes[t]{2pt}"<<endl;
  fout<<"\\makegapedcells"<<endl;
  fout<<"\\begin{document}"<<endl;
  fout<<"\\title{Topology Analysis \\footnote{\\normalsize{This package is implemented with reference to a program called {\\sc Topo}, which is developed by Prof. Shuxian Du from Zhengzhou University in China and has been widely used by people in BESIII collaboration. Several years ago, when I was a PhD student working on BESIII experiment, I learned the idea of topology analysis and a lot of programming techniques from the {\\sc Topo} program. So, I really appreciate Prof. Du's original work very much. To meet my own needs and to practice developing analysis tools with C++, ROOT and LaTex, I wrote the package from scratch. At that time, the package functioned well but was relatively simple. At the end of last year (2017), my co-supervisor, Prof. Chengping Shen reminded me that it could be a useful tool for Belle II experiment as well. So, I revised and extended it, making it more well-rounded and suitable for Belle II experiment. Here, I would like to thank Prof. Du for his orignial work, Prof. Shen for his suggestion and encouragement, and Wencheng Yan, Sen Jia, Yubo Li, Suxian Li, Longke Li, Guanda Gong, Junhao Yin, Xiaoping Qin, Xiqing Hao, HongPeng Wang, JiaWei Zhang and Yeqi Chen for their efforts in helping me test the program.}} \\\\ \\vspace{0.1cm} \\Large{("<<m_verNum<<")}}"<<endl;
  fout<<"\\author{Xingyu Zhou \\footnote{\\normalsize{Email: zhouxy@buaa.edu.cn}} \\\\ \\vspace{0.1cm} Beihang University}"<<endl;
  fout<<"\\maketitle"<<endl;
  fout<<endl<<"\\clearpage"<<endl<<endl;
  fout<<"\\newgeometry{left=2.5cm,right=2.5cm,top=2.5cm,bottom=2.5cm}"<<endl<<endl;
  fout<<"\\listoftables"<<endl<<endl;
  fout<<"\\newgeometry{left=0.0cm,right=0.0cm,top=2.5cm,bottom=2.5cm}"<<endl;
  const unsigned int nLinesMin=36;
  const unsigned int nLinesMax=32;
  unsigned int nBrsInALine=6; // The value of this variable is changed to 5 for the case of the table titled signal event trees and their respective initial-final states.
  unsigned int nLines;
  unsigned long nCmltEvts;

  if(m_sttaEvtTrsAndEvtIFSts==false)
    {
      if(m_ccSwitch==true) nBrsInALine=5;
      nLines=0;
      vector< list<int> > evtTr;
      list<int> evtBr;
      list<int> evtIFSts;
      nCmltEvts=0;
      unsigned long nEvtTrsToBePrtd=m_vEvtTr.size()<m_nEvtTrsMax?m_vEvtTr.size():m_nEvtTrsMax;
      for(unsigned long i=0;i<nEvtTrsToBePrtd;i++)
        {
          if(nLines==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0)
                {
                  fout<<"\\caption{Event trees and their respective initial-final states.}"<<endl;
                  nLines++;
                }
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & \\thead{event tree \\\\ (event initial-final states)} & ";
              if(m_sprTopoTags==false) fout<<"iEvtTr & iEvtIFSts & ";
              fout<<"nEvts & ";
              if(m_ccSwitch==true) fout<<"nCcEvts & nTotEvts & ";
              fout<<"nCmltEvts \\\\"<<endl;
              nLines++;
              nLines++;
              fout<<"\\hline"<<endl;
            }
          evtTr.clear();
          evtTr=m_vEvtTr[i];
          unsigned int nVldEvtBrs=0;
          fout<<i+1<<" & \\makecell{ $ "<<endl;
          for(unsigned int j=0;j<evtTr.size();j++)
            {
              if(j==0&&m_spr1stBrsOfEvtTrs==true&&evtTr[0].size()==3) continue;
              evtBr.clear();
              evtBr=evtTr[j];
              // Since a lot of pi0s are produced and almost all of them decay to gammma pairs, to save paper and for convenience of readers, all branches of pi0 to gamma pairs are not outputted into the tex file, and hence not printed in the pdf file.
              list<int>::iterator liit=evtBr.begin();
              list<int>::iterator liit1=evtBr.begin();
              liit1++;
              list<int>::iterator liit2=evtBr.begin();
              liit2++;
              liit2++;
              if(((*liit)==111)&&(evtBr.size()==3)&&((*liit1)==22)&&((*liit2)==22)) continue;
              nVldEvtBrs++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              // The condition "j==0" is set for the initial state particle pair e+e-;the condition "(*liit)==1, 2, 3, 4, 5 or 6" is set for the intermediate state quark pair ddbar, uubar, ssbar, ccbar, bbbar or ttbar;
              if(j==0||(*liit)==1||(*liit)==2||(*liit)==3||(*liit)==4||(*liit)==5||(*liit)==6)
                {
                  liit++;
                  writePnmFromPid(fout,"TexPnm",(*liit));
                }
              fout<<"\\rightarrow ";
              for(liit++;liit!=evtBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              // The following bool variable is used to judge whether the current branch is the last one outputed or not.
              bool lastOneOrAllPi0sToGamPairsBehind=true;
              list<int> evtBrTmp;
              for(unsigned int k=j+1;k<evtTr.size();k++)
                {
                  evtBrTmp.clear();
                  evtBrTmp=evtTr[k];
                  list<int>::iterator liitTmp=evtBrTmp.begin();
                  list<int>::iterator liitTmp1=evtBrTmp.begin();
                  liitTmp1++;
                  list<int>::iterator liitTmp2=evtBrTmp.begin();
                  liitTmp2++;
                  liitTmp2++;
                  if(((*liitTmp)!=111)||(evtBrTmp.size()!=3)||((*liitTmp1)!=22)||((*liitTmp2)!=22))
                    {
                      lastOneOrAllPi0sToGamPairsBehind=false;
                      break;
                    }
                }
              if(!lastOneOrAllPi0sToGamPairsBehind)
                {
                  fout<<","<<endl;
                  if(nVldEvtBrs%nBrsInALine==0) fout<<"$ \\\\ $"<<endl;
                }
              else
                {
                  fout<<endl<<"$ \\\\ ($"<<endl;
                  break;
                }
            }

          for(unsigned int j=0;j<m_vEvtIFSts.size();j++)
            {
              if(m_vIEvtIFSts[j]==m_iEvtTrIEvtIFStsMap[m_vIEvtTr[i]])
                {
                  evtIFSts.clear();
                  evtIFSts=m_vEvtIFSts[j];
                  break;
                }
            }
          list<int>::iterator liit;
          if(m_spr1stBrsOfEvtTrs==true&&evtTr[0].size()==3)
            {
              liit=evtTr[0].begin();
              liit++;
              liit++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              liit=evtIFSts.begin();
              liit++;
            }
          else
            {
              liit=evtIFSts.begin();
              writePnmFromPid(fout,"TexPnm",(*liit));
              liit++;
              writePnmFromPid(fout,"TexPnm",(*liit));
            }
          fout<<"\\rightarrow ";
          for(liit++;liit!=evtIFSts.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<endl<<"$) } & ";
          if(m_sprTopoTags==false) fout<<m_vIEvtTr[i]<<" & "<<m_iEvtTrIEvtIFStsMap[m_vIEvtTr[i]]<<" & ";
          fout<<m_vNEvtTr[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcEvtTr[i]==0) fout<<"---";
              else fout<<m_vNCcEvtTr[i];
              fout<<" & "<<m_vNEvtTr[i]+m_vNCcEvtTr[i]<<" & ";
              nCmltEvts=nCmltEvts+m_vNEvtTr[i]+m_vNCcEvtTr[i];          
            }
          else
            {
              nCmltEvts=nCmltEvts+m_vNEvtTr[i];
            }
          fout<<nCmltEvts<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          nLines=nLines+ceil(nVldEvtBrs/((double) nBrsInALine))+1;
          if(nLines>=nLinesMin||i==nEvtTrsToBePrtd-1)
            {
              if(i==nEvtTrsToBePrtd-1&&nEvtTrsToBePrtd<m_vEvtTr.size())
                {
                  if(nLines>=nLinesMin)
                    {
                      fout<<"\\end{tabular}"<<endl;
                      fout<<"\\end{table}"<<endl;

                      fout<<endl<<"\\clearpage"<<endl<<endl;
                      fout<<"\\begin{table}[htbp!]"<<endl;
                      fout<<"\\small"<<endl;
                      fout<<"\\centering"<<endl;
                      fout<<"\\begin{tabular}{|c|c|c|c|c|c|";
                      if(m_ccSwitch==true) fout<<"c|c|";
                      fout<<"}"<<endl;
                      fout<<"\\hline"<<endl;
                      fout<<"index & \\thead{event tree \\\\ (event initial-final states)} & ";
                      if(m_sprTopoTags==false) fout<<"iEvtTr & iEvtIFSts & ";
                      fout<<"nEvts & ";
                      if(m_ccSwitch==true) fout<<"nCcEvts & nTotEvts & ";
                      fout<<"nCmltEvts \\\\"<<endl;
                      fout<<"\\hline"<<endl;
                    }
                  fout<<"rest"<<" & \\makecell{ $ "<<endl;
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
                          writePnmFromPid(fout,"TexPnm",pid);
                          fout<<"\\rightarrow \\rm{others}";
                          fout<<endl<<"$ \\\\ ($"<<endl;
                          writePnmFromPid(fout,"TexPnm",pid);
                        }
                      else
                        {
                          writePnmFromPid(fout,"TexPnm",-11);
                          writePnmFromPid(fout,"TexPnm",11);
                          fout<<"\\rightarrow \\rm{others}";
                          fout<<endl<<"$ \\\\ ($"<<endl;
                          writePnmFromPid(fout,"TexPnm",-11);
                          writePnmFromPid(fout,"TexPnm",11);
                        }
                    }
                  else
                    {
                      writePnmFromPid(fout,"TexPnm",-11);
                      writePnmFromPid(fout,"TexPnm",11);
                      fout<<"\\rightarrow \\rm{others}";
                      fout<<endl<<"$ \\\\ ($"<<endl;
                      writePnmFromPid(fout,"TexPnm",-11);
                      writePnmFromPid(fout,"TexPnm",11);
                    }
                  fout<<"\\rightarrow \\rm{corresponding\\ to\\ others}"<<endl;
                  fout<<"$) } & ";
                  if(m_sprTopoTags==false) fout<<"---"<<" & "<<"---"<<" & ";
                  unsigned long nCmltEvtsOfRest=0;
                  if(m_ccSwitch==true)
                    {
                      fout<<"---"<<" & "<<"---"<<" & ";
                      for(unsigned long i=nEvtTrsToBePrtd;i<m_vEvtTr.size();i++) nCmltEvtsOfRest=nCmltEvtsOfRest+m_vNEvtTr[i]+m_vNCcEvtTr[i];
                    }
                  else
                    {
                      for(unsigned long i=nEvtTrsToBePrtd;i<m_vEvtTr.size();i++) nCmltEvtsOfRest=nCmltEvtsOfRest+m_vNEvtTr[i];
                    }
                  nCmltEvts=nCmltEvts+nCmltEvtsOfRest;
                  fout<<nCmltEvtsOfRest<<" & "<<nCmltEvts<<" \\\\"<<endl;
                  fout<<"\\hline"<<endl;
                }
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
              nLines=0;
            }
        }
      if(m_ccSwitch==true) nBrsInALine=6;

      //list<int> evtIFSts; The list<int> variable evtIFSts has been previously declared.
      nCmltEvts=0;
      unsigned long nEvtIFStsToBePrtd=m_vEvtIFSts.size()<m_nEvtIFStsMax?m_vEvtIFSts.size():m_nEvtIFStsMax;
      for(unsigned long i=0;i<nEvtIFStsToBePrtd;i++)
        {
          if(i%nLinesMax==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0) fout<<"\\caption{Event initial-final states.}"<<endl;
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|"; 
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & event initial-final states & ";
              if(m_sprTopoTags==false) fout<<"iEvtIFSts & ";
              fout<<"nEvts & ";
              if(m_ccSwitch==true) fout<<"nCcEvts & nTotEvts & ";
              fout<<"nCmltEvts \\\\"<<endl;
              fout<<"\\hline"<<endl;
            }
          fout<<i+1<<" & $ "; 
          evtIFSts.clear();
          evtIFSts=m_vEvtIFSts[i];
          list<int>::iterator liit=evtIFSts.begin();
          writePnmFromPid(fout,"TexPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"\\rightarrow ";
          for(liit++;liit!=evtIFSts.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"$ & ";
          if(m_sprTopoTags==false) fout<<m_vIEvtIFSts[i]<<" & ";
          fout<<m_vNEvtIFSts[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcEvtIFSts[i]==0) fout<<"---";
              else fout<<m_vNCcEvtIFSts[i];
              fout<<" & "<<m_vNEvtIFSts[i]+m_vNCcEvtIFSts[i]<<" & ";
              nCmltEvts=nCmltEvts+m_vNEvtIFSts[i]+m_vNCcEvtIFSts[i];
            }
          else
            {
              nCmltEvts=nCmltEvts+m_vNEvtIFSts[i];
            }
          fout<<nCmltEvts<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          if(i%nLinesMax==nLinesMax-1||i==nEvtIFStsToBePrtd-1)
            {
              if(i==nEvtIFStsToBePrtd-1&&nEvtIFStsToBePrtd<m_vEvtIFSts.size())
                {
                  if(i%nLinesMax==nLinesMax-1)
                    {
                      fout<<"\\end{tabular}"<<endl;
                      fout<<"\\end{table}"<<endl;

                      fout<<endl<<"\\clearpage"<<endl<<endl;
                      fout<<"\\begin{table}[htbp!]"<<endl;
                      fout<<"\\small"<<endl;
                      fout<<"\\centering"<<endl;
                      fout<<"\\begin{tabular}{|c|c|c|c|c|";
                      if(m_ccSwitch==true) fout<<"c|c|";
                      fout<<"}"<<endl;
                      fout<<"\\hline"<<endl;
                      fout<<"index & event initial-final states & ";
                      if(m_sprTopoTags==false) fout<<"iEvtIFSts & ";
                      fout<<"nEvts & ";
                      if(m_ccSwitch==true) fout<<"nCcEvts & nTotEvts & ";
                      fout<<"nCmltEvts \\\\"<<endl;
                      fout<<"\\hline"<<endl;
                    }
                  fout<<"rest"<<" & $ ";
                  writePnmFromPid(fout,"TexPnm",-11);
                  writePnmFromPid(fout,"TexPnm",11);
                  fout<<"\\rightarrow \\rm{others}";
                  fout<<" $ & ";
                  if(m_sprTopoTags==false) fout<<"---"<<" & ";
                  unsigned long nCmltEvtsOfRest=0;
                  if(m_ccSwitch==true)
                    {
                      fout<<"---"<<" & "<<"---"<<" & ";
                      for(unsigned long i=nEvtIFStsToBePrtd;i<m_vEvtIFSts.size();i++) nCmltEvtsOfRest=nCmltEvtsOfRest+m_vNEvtIFSts[i]+m_vNCcEvtIFSts[i];
                    }
                  else
                    {
                      for(unsigned long i=nEvtIFStsToBePrtd;i<m_vEvtIFSts.size();i++) nCmltEvtsOfRest=nCmltEvtsOfRest+m_vNEvtIFSts[i];
                    }
                  nCmltEvts=nCmltEvts+nCmltEvtsOfRest;
                  fout<<nCmltEvtsOfRest<<" & "<<nCmltEvts<<" \\\\"<<endl;
                  fout<<"\\hline"<<endl;
                }
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
            }
        }
    }

  if(m_vVEvtBrP.size()>0)
    {
      for(unsigned int i=0;i<m_vVEvtBrP.size();i++)
        {
          list<int> evtBrP;
          unsigned long nCmltEtrs=0;
          unsigned long nEvtBrPToBePrtd=m_vVEvtBrP[i].size()<m_vNEvtBrsMax[i]?m_vVEvtBrP[i].size():m_vNEvtBrsMax[i];
          for(unsigned int j=0;j<nEvtBrPToBePrtd;j++)
            {
              if(j%nLinesMax==0)
                {
                  fout<<endl<<"\\clearpage"<<endl<<endl;
                  fout<<"\\begin{table}[htbp!]"<<endl;
                  if(j==0)
                    {
                      fout<<"\\caption{Event branches begun with $ ";
                      writePnmFromPid(fout,"TexPnm",m_vPttaPid[i]);
                      fout<<"$.}"<<endl;
                    }
                  fout<<"\\small"<<endl;
                  fout<<"\\centering"<<endl;
                  fout<<"\\begin{tabular}{|c|c|c|c|c|";
                  if(m_ccSwitch==true) fout<<"c|c|";
                  fout<<"}"<<endl;
                  fout<<"\\hline"<<endl;
                  fout<<"index & event branch begun with $ ";
                  writePnmFromPid(fout,"TexPnm",m_vPttaPid[i]);
                  fout<<"$ & ";
                  if(m_sprTopoTags==false) fout<<"iEvtBrP"<<i+1<<" & ";
                  fout<<"nEtrs & ";
                  if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & ";
                  fout<<"nCmltEtrs \\\\"<<endl;
                  fout<<"\\hline"<<endl;
                }
              fout<<j+1<<" & $ "; 
              evtBrP.clear();
              evtBrP=m_vVEvtBrP[i][j];
              list<int>::iterator liit=evtBrP.begin();
              writePnmFromPid(fout,"TexPnm",(*liit));
              fout<<"\\rightarrow ";
              for(liit++;liit!=evtBrP.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              fout<<"$ & ";
              if(m_sprTopoTags==false) fout<<m_vVIEvtBrP[i][j]<<" & ";
              fout<<m_vVNEvtBrP[i][j]<<" & ";
              if(m_ccSwitch==true)
                {
                  if(m_vPttaICcPid[i]==0&&m_vVIEvtBrCcP[i][j]==0) fout<<"---";
                  else fout<<m_vVNEvtBrCcP[i][j];
                  fout<<" & "<<m_vVNEvtBrP[i][j]+m_vVNEvtBrCcP[i][j]<<" & ";
                  nCmltEtrs=nCmltEtrs+m_vVNEvtBrP[i][j]+m_vVNEvtBrCcP[i][j];
                }
              else
                {
                  nCmltEtrs=nCmltEtrs+m_vVNEvtBrP[i][j];
                }
              fout<<nCmltEtrs<<" \\\\"<<endl;
              fout<<"\\hline"<<endl;
              if(j%nLinesMax==nLinesMax-1||j==nEvtBrPToBePrtd-1)
                {
                  if(j==nEvtBrPToBePrtd-1&&nEvtBrPToBePrtd<m_vVEvtBrP[i].size())
                    {
                      if(j%nLinesMax==nLinesMax-1)
                        {
                          fout<<"\\end{tabular}"<<endl;
                          fout<<"\\end{table}"<<endl;

                          fout<<endl<<"\\clearpage"<<endl<<endl;
                          fout<<"\\begin{table}[htbp!]"<<endl;
                          fout<<"\\small"<<endl;
                          fout<<"\\centering"<<endl;
                          fout<<"\\begin{tabular}{|c|c|c|c|c|";
                          if(m_ccSwitch==true) fout<<"c|c|";
                          fout<<"}"<<endl;
                          fout<<"\\hline"<<endl;
                          fout<<"index & event branch begun with $ ";
                          writePnmFromPid(fout,"TexPnm",m_vPttaPid[i]);
                          fout<<"$ & ";
                          if(m_sprTopoTags==false) fout<<"iEvtBrP"<<i+1<<" & ";
                          fout<<"nEtrs & ";
                          if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & ";
                          fout<<"nCmltEtrs \\\\"<<endl;
                          fout<<"\\hline"<<endl;
                        }
                      fout<<"rest"<<" & $ ";
                      evtBrP.clear();
                      evtBrP=m_vVEvtBrP[i][j];
                      list<int>::iterator liit=evtBrP.begin();
                      writePnmFromPid(fout,"TexPnm",(*liit));
                      fout<<"\\rightarrow \\rm{others}";
                      fout<<" $ & ";
                      if(m_sprTopoTags==false) fout<<"---"<<" & ";
                      unsigned long nCmltEtrsOfRest=0;
                      if(m_ccSwitch==true)
                        {
                          if(m_vPttaICcPid[i]==0)
                            {
                              fout<<"---"<<" & "<<"---"<<" & ";
                            }
                          else
                            {
                              unsigned long nCmltEtrsOfRestTemp1=0;
                              unsigned long nCmltEtrsOfRestTemp2=0;
                              for(unsigned int j=nEvtBrPToBePrtd;j<m_vVEvtBrP[i].size();j++) nCmltEtrsOfRestTemp1=nCmltEtrsOfRestTemp1+m_vVNEvtBrP[i][j];
                              for(unsigned int j=nEvtBrPToBePrtd;j<m_vVEvtBrP[i].size();j++) nCmltEtrsOfRestTemp2=nCmltEtrsOfRestTemp2+m_vVNEvtBrCcP[i][j];
                              fout<<nCmltEtrsOfRestTemp1<<" & "<<nCmltEtrsOfRestTemp2<<" & ";
                            }
                          for(unsigned int j=nEvtBrPToBePrtd;j<m_vVEvtBrP[i].size();j++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vVNEvtBrP[i][j]+m_vVNEvtBrCcP[i][j];
                        }
                      else
                        {
                          for(unsigned int j=nEvtBrPToBePrtd;j<m_vVEvtBrP[i].size();j++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vVNEvtBrP[i][j];
                        }
                      nCmltEtrs=nCmltEtrs+nCmltEtrsOfRest;
                      fout<<nCmltEtrsOfRest<<" & "<<nCmltEtrs<<" \\\\"<<endl;
                      fout<<"\\hline"<<endl;
                    }
                  fout<<"\\end{tabular}"<<endl;
                  fout<<"\\end{table}"<<endl;
                }
            }
        }
    }

  if(m_vVEvtBrIncEvtBr.size()>0)
    {
      for(unsigned int i=0;i<m_vVEvtBrIncEvtBr.size();i++)
        {
          list<int> evtBrIncEvtBr;
          unsigned long nCmltEtrs=0;
          unsigned long nEvtBrIncEvtBrToBePrtd=m_vVEvtBrIncEvtBr[i].size()<m_vNEvtBrsMax1[i]?m_vVEvtBrIncEvtBr[i].size():m_vNEvtBrsMax1[i];
          for(unsigned int j=0;j<nEvtBrIncEvtBrToBePrtd;j++)
            {
              if(j%nLinesMax==0)
                {
                  fout<<endl<<"\\clearpage"<<endl<<endl;
                  fout<<"\\begin{table}[htbp!]"<<endl;
                  if(j==0)
                    {
                      fout<<"\\caption{Exclusive event branches matched with $ ";
                      list<int>::iterator liit=m_vPttaIncEvtBr[i].begin();
                      writePnmFromPid(fout,"TexPnm",(*liit));
                      if((*liit)==-11)
                        {
                          liit++;
                          writePnmFromPid(fout,"TexPnm",(*liit));
                        }
                      fout<<"\\rightarrow ";
                      for(liit++;liit!=m_vPttaIncEvtBr[i].end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
                      fout<<"+ \\rm{anything} ";
                      fout<<"$.}"<<endl;
                    }
                  fout<<"\\small"<<endl;
                  fout<<"\\centering"<<endl;
                  fout<<"\\begin{tabular}{|c|c|c|c|c|";
                  if(m_ccSwitch==true) fout<<"c|c|";
                  fout<<"}"<<endl;
                  fout<<"\\hline"<<endl;
                  fout<<"index & exclusive event branch matched with $ ";
                  list<int>::iterator liit=m_vPttaIncEvtBr[i].begin();
                  writePnmFromPid(fout,"TexPnm",(*liit));
                  if((*liit)==-11)
                    {
                      liit++;
                      writePnmFromPid(fout,"TexPnm",(*liit));
                    }
                  fout<<"\\rightarrow ";
                  for(liit++;liit!=m_vPttaIncEvtBr[i].end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
                  fout<<"+ \\rm{anything} ";
                  fout<<"$ & ";
                  if(m_sprTopoTags==false) fout<<"iEvtBrIncEvtBr"<<i+1<<" & ";
                  fout<<"nEtrs & ";
                  if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & ";
                  fout<<"nCmltEtrs \\\\"<<endl;
                  fout<<"\\hline"<<endl;
                }
              fout<<j+1<<" & $ "; 
              evtBrIncEvtBr.clear();
              evtBrIncEvtBr=m_vVEvtBrIncEvtBr[i][j];
              list<int>::iterator liit=evtBrIncEvtBr.begin();
              writePnmFromPid(fout,"TexPnm",(*liit));
              if((*liit)==-11)
                {
                  liit++;
                  writePnmFromPid(fout,"TexPnm",(*liit));
                }
              fout<<"\\rightarrow ";
              for(liit++;liit!=evtBrIncEvtBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              fout<<"$ & ";
              if(m_sprTopoTags==false) fout<<m_vVIEvtBrIncEvtBr[i][j]<<" & ";
              fout<<m_vVNEvtBrIncEvtBr[i][j]<<" & ";
              if(m_ccSwitch==true)
                {
                  if(m_vPttaICcPid[i]==0&&m_vVIEvtBrCcIncEvtBr[i][j]==0) fout<<"---";
                  else fout<<m_vVNEvtBrCcIncEvtBr[i][j];
                  fout<<" & "<<m_vVNEvtBrIncEvtBr[i][j]+m_vVNEvtBrCcIncEvtBr[i][j]<<" & ";
                  nCmltEtrs=nCmltEtrs+m_vVNEvtBrIncEvtBr[i][j]+m_vVNEvtBrCcIncEvtBr[i][j];
                }
              else
                {
                  nCmltEtrs=nCmltEtrs+m_vVNEvtBrIncEvtBr[i][j];
                }
              fout<<nCmltEtrs<<" \\\\"<<endl;
              fout<<"\\hline"<<endl;
              if(j%nLinesMax==nLinesMax-1||j==nEvtBrIncEvtBrToBePrtd-1)
                {
                  if(j==nEvtBrIncEvtBrToBePrtd-1&&nEvtBrIncEvtBrToBePrtd<m_vVEvtBrIncEvtBr[i].size())
                    {
                      if(j%nLinesMax==nLinesMax-1)
                        {
                          fout<<"\\end{tabular}"<<endl;
                          fout<<"\\end{table}"<<endl;

                          fout<<endl<<"\\clearpage"<<endl<<endl;
                          fout<<"\\begin{table}[htbp!]"<<endl;
                          fout<<"\\small"<<endl;
                          fout<<"\\centering"<<endl;
                          fout<<"\\begin{tabular}{|c|c|c|c|c|";
                          if(m_ccSwitch==true) fout<<"c|c|";
                          fout<<"}"<<endl;
                          fout<<"\\hline"<<endl;
                          fout<<"index & exclusive event branch matched with $ ";
                          list<int>::iterator liit=m_vPttaIncEvtBr[i].begin();
                          writePnmFromPid(fout,"TexPnm",(*liit));
                          if((*liit)==-11)
                            {
                              liit++;
                              writePnmFromPid(fout,"TexPnm",(*liit));
                            }
                          fout<<"\\rightarrow ";
                          for(liit++;liit!=m_vPttaIncEvtBr[i].end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
                          fout<<"+ \\rm{anything} ";
                          fout<<"$ & ";
                          if(m_sprTopoTags==false) fout<<"iEvtBrIncEvtBr"<<i+1<<" & ";
                          fout<<"nEtrs & ";
                          if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & ";
                          fout<<"nCmltEtrs \\\\"<<endl;
                          fout<<"\\hline"<<endl;
                        }
                      fout<<"rest"<<" & $ ";
                      evtBrIncEvtBr.clear();
                      evtBrIncEvtBr=m_vVEvtBrIncEvtBr[i][j];
                      list<int>::iterator liit=evtBrIncEvtBr.begin();
                      writePnmFromPid(fout,"TexPnm",(*liit));
                      if((*liit)==-11)
                        {
                          liit++;
                          writePnmFromPid(fout,"TexPnm",(*liit));
                        }
                      fout<<"\\rightarrow \\rm{others}";
                      fout<<" $ & ";
                      if(m_sprTopoTags==false) fout<<"---"<<" & ";
                      unsigned long nCmltEtrsOfRest=0;
                      if(m_ccSwitch==true)
                        {
                          if(m_vPttaICcPid[i]==0)
                            {
                              fout<<"---"<<" & "<<"---"<<" & ";
                            }
                          else
                            {
                              unsigned long nCmltEtrsOfRestTemp1=0;
                              unsigned long nCmltEtrsOfRestTemp2=0;
                              for(unsigned int j=nEvtBrIncEvtBrToBePrtd;j<m_vVEvtBrIncEvtBr[i].size();j++) nCmltEtrsOfRestTemp1=nCmltEtrsOfRestTemp1+m_vVNEvtBrIncEvtBr[i][j];
                              for(unsigned int j=nEvtBrIncEvtBrToBePrtd;j<m_vVEvtBrIncEvtBr[i].size();j++) nCmltEtrsOfRestTemp2=nCmltEtrsOfRestTemp2+m_vVNEvtBrCcIncEvtBr[i][j];
                              fout<<nCmltEtrsOfRestTemp1<<" & "<<nCmltEtrsOfRestTemp2<<" & ";
                            }
                          for(unsigned int j=nEvtBrIncEvtBrToBePrtd;j<m_vVEvtBrIncEvtBr[i].size();j++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vVNEvtBrIncEvtBr[i][j]+m_vVNEvtBrCcIncEvtBr[i][j];
                        }
                      else
                        {
                          for(unsigned int j=nEvtBrIncEvtBrToBePrtd;j<m_vVEvtBrIncEvtBr[i].size();j++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vVNEvtBrIncEvtBr[i][j];
                        }
                      nCmltEtrs=nCmltEtrs+nCmltEtrsOfRest;
                      fout<<nCmltEtrsOfRest<<" & "<<nCmltEtrs<<" \\\\"<<endl;
                      fout<<"\\hline"<<endl;
                    }
                  fout<<"\\end{tabular}"<<endl;
                  fout<<"\\end{table}"<<endl;
                }
            }
        }
    }

  if(m_vSigPid.size()>0)
    {
      unsigned long nCmltPs=0;
      for(unsigned int i=0;i<m_vNSigP.size();i++)
        {
          if(i%nLinesMax==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0) fout<<"\\caption{Signal particles.}"<<endl;
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & signal particle & ";
              if(m_sprTopoTags==false) fout<<"iSigP & ";
              fout<<"nPs & ";
              if(m_ccSwitch==true) fout<<"nCcPs & nTotPs & ";
              fout<<"nCmltPs \\\\"<<endl;
              fout<<"\\hline"<<endl;
            }
          fout<<i+1<<" & $ ";
          writePnmFromPid(fout,"TexPnm",m_vSigPid[i]);
          fout<<"$ & ";
          if(m_sprTopoTags==false) fout<<m_vISigP[i]<<" & ";
          fout<<m_vNSigP[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigP[i]>0) fout<<m_vNCcSigP[i];
              else fout<<"---";
              fout<<" & "<<m_vNSigP[i]+m_vNCcSigP[i]<<" & ";
              nCmltPs=nCmltPs+m_vNSigP[i]+m_vNCcSigP[i];
            }
          else
            {
              nCmltPs=nCmltPs+m_vNSigP[i];
            }
          fout<<nCmltPs<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          if(i%nLinesMax==nLinesMax-1||i==(m_vNSigP.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
            }
        }
    }

  if(m_vSigIncEvtBr.size()>0)
    {
      list<int> sigIncEvtBr;
      unsigned long nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigIncEvtBr.size();i++)
        {
          if(i%nLinesMax==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0) fout<<"\\caption{Signal inclusive event branches.}"<<endl;
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & signal inclusive event branch & ";
              if(m_sprTopoTags==false) fout<<"iSigIncEvtBr & ";
              fout<<"nEtrs & ";
              if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & "<<endl;
              fout<<"nCmltEtrs \\\\"<<endl;
              fout<<"\\hline"<<endl;
            }
          fout<<i+1<<" & $ "; 
          sigIncEvtBr.clear();
          sigIncEvtBr=m_vSigIncEvtBr[i];
          list<int>::iterator liit=sigIncEvtBr.begin();
          writePnmFromPid(fout,"TexPnm",(*liit));
          if((*liit)==-11)
            {
              liit++;
              writePnmFromPid(fout,"TexPnm",(*liit));
            }          
          fout<<"\\rightarrow ";
          for(liit++;liit!=sigIncEvtBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<" + \\rm{anything}";
          fout<<"$ & ";
          if(m_sprTopoTags==false) fout<<m_vISigIncEvtBr[i]<<" & ";
          fout<<m_vNSigIncEvtBr[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigIncEvtBr[i]>0) fout<<m_vNCcSigIncEvtBr[i];
              else fout<<"---";
              fout<<" & "<<m_vNSigIncEvtBr[i]+m_vNCcSigIncEvtBr[i]<<" & ";
              nCmltEtrs=nCmltEtrs+m_vNSigIncEvtBr[i]+m_vNCcSigIncEvtBr[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNSigIncEvtBr[i];
            }
          fout<<nCmltEtrs<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          if(i%nLinesMax==nLinesMax-1||i==(m_vSigIncEvtBr.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
            }
        }
    }

  if(m_vSigSeqEvtBrs.size()>0)
    {
      if(m_ccSwitch==true) nBrsInALine=4;
      else nBrsInALine=5;
      nLines=0;
      vector< list<int> > sigSeqEvtBrs;
      vector<int> vSigSeqIdxOfHead;
      list<int> sigEvtBr;
      unsigned long nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigSeqEvtBrs.size();i++)
        {
          if(nLines==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0)
                {
                  fout<<"\\caption{Signal sequential event branches.}"<<endl;
                  nLines++;
                }
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & \\thead{signal sequential event branches} & ";
              if(m_sprTopoTags==false) fout<<"iSigSeqEvtBrs & ";
              fout<<"nEtrs & ";
              if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & "<<endl;
              fout<<"nCmltEtrs \\\\"<<endl;
              nLines++;
              fout<<"\\hline"<<endl;
            }
          sigSeqEvtBrs.clear();
          sigSeqEvtBrs=m_vSigSeqEvtBrs[i];
          vSigSeqIdxOfHead=m_vVSigSeqIdxOfHead[i];
          unsigned int nVldSigSeqEvtBrs=0;
          fout<<i+1<<" & \\makecell{ $ "<<endl;
          for(unsigned int j=0;j<sigSeqEvtBrs.size();j++)
            {
              sigEvtBr.clear();
              sigEvtBr=sigSeqEvtBrs[j];              
              // Since a lot of pi0s are produced and almost all of them decay to gammma pairs, to save paper and for convenience of readers, all branches of pi0 to gamma pairs are not outputted into the tex file, and hence not printed in the pdf file.
              list<int>::iterator liit=sigEvtBr.begin();
              list<int>::iterator liit1=sigEvtBr.begin();
              liit1++;
              list<int>::iterator liit2=sigEvtBr.begin();
              liit2++;
              liit2++;
              if(((*liit)==111)&&(sigEvtBr.size()==3)&&((*liit1)==22)&&((*liit2)==22)) continue;
              nVldSigSeqEvtBrs++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              if(j==0&&vSigSeqIdxOfHead[0]==-1)
              {
                liit++;
                writePnmFromPid(fout,"TexPnm",(*liit));
              }
              fout<<"\\rightarrow ";
              for(liit++;liit!=sigEvtBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              // The following bool variable is used to judge whether the current branch is the last one outputed or not.
              bool lastOneOrAllPi0sToGamPairsBehind=true;
              list<int> sigEvtBrTmp;
              for(unsigned int k=j+1;k<sigSeqEvtBrs.size();k++)
                {
                  sigEvtBrTmp.clear();
                  sigEvtBrTmp=sigSeqEvtBrs[k];
                  list<int>::iterator liitTmp=sigEvtBrTmp.begin();
                  list<int>::iterator liitTmp1=sigEvtBrTmp.begin();
                  liitTmp1++;
                  list<int>::iterator liitTmp2=sigEvtBrTmp.begin();
                  liitTmp2++;
                  liitTmp2++;
                  if(((*liitTmp)!=111)||(sigEvtBrTmp.size()!=3)||((*liitTmp1)!=22)||((*liitTmp2)!=22))
                    {
                      lastOneOrAllPi0sToGamPairsBehind=false;
                      break;
                    }
                }
              if(!lastOneOrAllPi0sToGamPairsBehind)
                {
                  fout<<","<<endl;
                  if(nVldSigSeqEvtBrs%nBrsInALine==0) fout<<"$ \\\\ $"<<endl;
                }
              else
                {
                  fout<<endl<<"$ }";
                  break;
                }
            }

          fout<<" & ";
          if(m_sprTopoTags==false) fout<<m_vISigSeqEvtBrs[i]<<" & ";
          fout<<m_vNSigSeqEvtBrs[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigSeqEvtBrs[i]>0) fout<<m_vNCcSigSeqEvtBrs[i];
              else fout<<"---";
              fout<<" & "<<m_vNSigSeqEvtBrs[i]+m_vNCcSigSeqEvtBrs[i]<<" & ";
              nCmltEtrs=nCmltEtrs+m_vNSigSeqEvtBrs[i]+m_vNCcSigSeqEvtBrs[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNSigSeqEvtBrs[i];
            }
          fout<<nCmltEtrs<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          nLines=nLines+ceil(nVldSigSeqEvtBrs/((double ) nBrsInALine));
          if(nLines>=nLinesMin||i==(m_vSigSeqEvtBrs.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
              nLines=0;
            }
        }
      if(m_ccSwitch==true) nBrsInALine=5;   
    }

  if(m_vSigIncSeqEvtBrs.size()>0)
    {
      if(m_ccSwitch==true) nBrsInALine=4;
      else nBrsInALine=5;
      nLines=0;
      vector< list<int> > sigIncSeqEvtBrs;
      vector<int> vSigIncSeqIdxOfHead;
      list<int> sigEvtBr;
      unsigned long nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigIncSeqEvtBrs.size();i++)
        {
          if(nLines==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0)
                {
                  fout<<"\\caption{Signal inclusive sequential event branches.}"<<endl;
                  nLines++;
                }
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & \\thead{signal inclusive sequential event branches} & ";
              if(m_sprTopoTags==false) fout<<"iSigIncSeqEvtBrs & ";
              fout<<"nEtrs & ";
              if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & "<<endl;
              fout<<"nCmltEtrs \\\\"<<endl;
              nLines++;
              fout<<"\\hline"<<endl;
            }
          sigIncSeqEvtBrs.clear();
          sigIncSeqEvtBrs=m_vSigIncSeqEvtBrs[i];
          vSigIncSeqIdxOfHead=m_vVSigIncSeqIdxOfHead[i];
          unsigned int nVldSigIncSeqEvtBrs=0;
          fout<<i+1<<" & \\makecell{ $ "<<endl;
          for(unsigned int j=0;j<sigIncSeqEvtBrs.size();j++)
            {
              sigEvtBr.clear();
              sigEvtBr=sigIncSeqEvtBrs[j];              
              // Since a lot of pi0s are produced and almost all of them decay to gammma pairs, to save paper and for convenience of readers, all branches of pi0 to gamma pairs are not outputted into the tex file, and hence not printed in the pdf file.
              list<int>::iterator liit=sigEvtBr.begin();
              list<int>::iterator liit1=sigEvtBr.begin();
              liit1++;
              list<int>::iterator liit2=sigEvtBr.begin();
              liit2++;
              liit2++;
              if(((*liit)==111)&&(sigEvtBr.size()==3)&&((*liit1)==22)&&((*liit2)==22)) continue;
              nVldSigIncSeqEvtBrs++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              if(j==0&&vSigIncSeqIdxOfHead[0]==-1)
              {
                liit++;
                writePnmFromPid(fout,"TexPnm",(*liit));
              }
              fout<<"\\rightarrow ";
              for(liit++;liit!=sigEvtBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              // The following bool variable is used to judge whether the current branch is the last one outputed or not.
              bool lastOneOrAllPi0sToGamPairsBehind=true;
              list<int> sigEvtBrTmp;
              for(unsigned int k=j+1;k<sigIncSeqEvtBrs.size();k++)
                {
                  sigEvtBrTmp.clear();
                  sigEvtBrTmp=sigIncSeqEvtBrs[k];
                  list<int>::iterator liitTmp=sigEvtBrTmp.begin();
                  list<int>::iterator liitTmp1=sigEvtBrTmp.begin();
                  liitTmp1++;
                  list<int>::iterator liitTmp2=sigEvtBrTmp.begin();
                  liitTmp2++;
                  liitTmp2++;
                  if(((*liitTmp)!=111)||(sigEvtBrTmp.size()!=3)||((*liitTmp1)!=22)||((*liitTmp2)!=22))
                    {
                      lastOneOrAllPi0sToGamPairsBehind=false;
                      break;
                    }
                }
              if(!lastOneOrAllPi0sToGamPairsBehind)
                {
                  fout<<","<<endl;
                  if(nVldSigIncSeqEvtBrs%nBrsInALine==0) fout<<"$ \\\\ $"<<endl;
                }
              else
                {
                  fout<<endl<<"$ }";
                  break;
                }
            }

          fout<<" & ";
          if(m_sprTopoTags==false) fout<<m_vISigIncSeqEvtBrs[i]<<" & ";
          fout<<m_vNSigIncSeqEvtBrs[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigIncSeqEvtBrs[i]>0) fout<<m_vNCcSigIncSeqEvtBrs[i];
              else fout<<"---";
              fout<<" & "<<m_vNSigIncSeqEvtBrs[i]+m_vNCcSigIncSeqEvtBrs[i]<<" & ";
              nCmltEtrs=nCmltEtrs+m_vNSigIncSeqEvtBrs[i]+m_vNCcSigIncSeqEvtBrs[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNSigIncSeqEvtBrs[i];
            }
          fout<<nCmltEtrs<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          nLines=nLines+ceil(nVldSigIncSeqEvtBrs/((double ) nBrsInALine));
          if(nLines>=nLinesMin||i==(m_vSigIncSeqEvtBrs.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
              nLines=0;
            }
        }
      if(m_ccSwitch==true) nBrsInALine=5;   
    }

  if(m_vSigPFSts.size()>0)
    {
      list<int> sigPFSts;
      unsigned long nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigPFSts.size();i++)
        {
          if(i%nLinesMax==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0) fout<<"\\caption{Signal particle final states.}"<<endl;
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & signal particle final states & ";
              if(m_sprTopoTags==false) fout<<"iSigPFSts & ";
              fout<<"nEtrs & ";
              if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & "<<endl;
              fout<<"nCmltEtrs \\\\"<<endl;
              fout<<"\\hline"<<endl;
            }
          fout<<i+1<<" & $ "; 
          sigPFSts.clear();
          sigPFSts=m_vSigPFSts[i];
          list<int>::iterator liit=sigPFSts.begin();
          writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"\\rightarrow ";
          for(liit++;liit!=sigPFSts.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"$ & ";
          if(m_sprTopoTags==false) fout<<m_vISigPFSts[i]<<" & ";
          fout<<m_vNSigPFSts[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigPFSts[i]>0) fout<<m_vNCcSigPFSts[i];
              else fout<<"---";
              fout<<" & "<<m_vNSigPFSts[i]+m_vNCcSigPFSts[i]<<" & ";
              nCmltEtrs=nCmltEtrs+m_vNSigPFSts[i]+m_vNCcSigPFSts[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNSigPFSts[i];
            }
          fout<<nCmltEtrs<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          if(i%nLinesMax==nLinesMax-1||i==(m_vSigPFSts.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
            }
        }
    }

  if(m_vSigEvtTr.size()>0)
    {
      if(m_ccSwitch==true) nBrsInALine=4;
      else nBrsInALine=5;
      nLines=0;
      vector< list<int> > sigEvtTr;
      list<int> sigEvtBr;
      list<int> sigEvtIFSts;
      nCmltEvts=0;
      for(unsigned int i=0;i<m_vSigEvtTr.size();i++)
        {
          if(nLines==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0)
                {
                  fout<<"\\caption{Signal event trees and their respective initial-final states.}"<<endl;
                  nLines++;
                }
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & \\thead{signal event tree \\\\ (signal event initial-final states)} & ";
              if(m_sprTopoTags==false) fout<<"iSigEvtTr & iSigEvtIFSts & iEvtTr & iEvtIFSts & ";
              fout<<"nEvts & ";
              if(m_ccSwitch==true) fout<<"nCcEvts & nTotEvts & ";
              fout<<"nCmltEvts \\\\"<<endl;
              nLines++;
              nLines++;
              fout<<"\\hline"<<endl;
            }
          sigEvtTr.clear();
          sigEvtTr=m_vSigEvtTr[i];
          unsigned int nVldSigEvtTr=0;
          fout<<i+1<<" & \\makecell{ $ "<<endl;
          for(unsigned int j=0;j<sigEvtTr.size();j++)
            {
              sigEvtBr.clear();
              sigEvtBr=sigEvtTr[j];              
              // Since a lot of pi0s are produced and almost all of them decay to gammma pairs, to save paper and for convenience of readers, all branches of pi0 to gamma pairs are not outputted into the tex file, and hence not printed in the pdf file.
              list<int>::iterator liit=sigEvtBr.begin();
              list<int>::iterator liit1=sigEvtBr.begin();
              liit1++;
              list<int>::iterator liit2=sigEvtBr.begin();
              liit2++;
              liit2++;
              if(((*liit)==111)&&(sigEvtBr.size()==3)&&((*liit1)==22)&&((*liit2)==22)) continue;
              nVldSigEvtTr++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              // The condition "j==0" is set for the initial state particle pair e+e-;the condition "(*liit)==1, 2, 3, 4, 5 or 6" is set for the intermediate state quark pair ddbar, uubar, ssbar, ccbar, bbbar or ttbar;
              if(j==0||(*liit)==1||(*liit)==2||(*liit)==3||(*liit)==4||(*liit)==5||(*liit)==6)
                {
                  liit++;
                  writePnmFromPid(fout,"TexPnm",(*liit));
                }
              fout<<"\\rightarrow ";
              for(liit++;liit!=sigEvtBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              // The following bool variable is used to judge whether the current branch is the last one outputed or not.
              bool lastOneOrAllPi0sToGamPairsBehind=true;
              list<int> sigEvtBrTmp;
              for(unsigned int k=j+1;k<sigEvtTr.size();k++)
                {
                  sigEvtBrTmp.clear();
                  sigEvtBrTmp=sigEvtTr[k];
                  list<int>::iterator liitTmp=sigEvtBrTmp.begin();
                  list<int>::iterator liitTmp1=sigEvtBrTmp.begin();
                  liitTmp1++;
                  list<int>::iterator liitTmp2=sigEvtBrTmp.begin();
                  liitTmp2++;
                  liitTmp2++;
                  if(((*liitTmp)!=111)||(sigEvtBrTmp.size()!=3)||((*liitTmp1)!=22)||((*liitTmp2)!=22))
                    {
                      lastOneOrAllPi0sToGamPairsBehind=false;
                      break;
                    }
                }
              if(!lastOneOrAllPi0sToGamPairsBehind)
                {
                  fout<<","<<endl;
                  if(nVldSigEvtTr%nBrsInALine==0) fout<<"$ \\\\ $"<<endl;
                }
              else
                {
                  fout<<endl<<"$ \\\\ ($"<<endl;
                  break;
                }
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
          list<int>::iterator liit=sigEvtIFSts.begin();
          writePnmFromPid(fout,"TexPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"\\rightarrow ";
          for(liit++;liit!=sigEvtIFSts.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<endl<<"$) } & ";
          if(m_sprTopoTags==false)
            {
              fout<<m_vISigEvtTr[i]<<" & ";
              if(m_iSigEvtTrICcSigEvtIFStsMap[m_vISigEvtTr[i]]>=0) fout<<m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]];
              else fout<<"$ "<<m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]]<<"_{cc} $";
              fout<<" & ";
              if(m_iSigEvtTrIEvtTrMap.find(m_vISigEvtTr[i])!=m_iSigEvtTrIEvtTrMap.end())
                {
                  if(m_iSigEvtTrICcEvtTrMap[m_vISigEvtTr[i]]>=0) fout<<m_iSigEvtTrIEvtTrMap[m_vISigEvtTr[i]];
                  else fout<<"$ "<<m_iSigEvtTrIEvtTrMap[m_vISigEvtTr[i]]<<"_{cc} $";
                }
              else fout<<"---";
              fout<<" & ";
              if(m_iSigEvtIFStsIEvtIFStsMap.find(m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]])!=m_iSigEvtIFStsIEvtIFStsMap.end())
                {
                  if(m_iSigEvtIFStsICcEvtIFStsMap[m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]]]>=0) fout<<m_iSigEvtIFStsIEvtIFStsMap[m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]]];
                  else fout<<"$ "<<m_iSigEvtIFStsIEvtIFStsMap[m_iSigEvtTrISigEvtIFStsMap[m_vISigEvtTr[i]]]<<"_{cc} $";
                }
              else fout<<"---";
              fout<<" & ";
            }
          fout<<m_vNSigEvtTr[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigEvtTr[i]==0) fout<<"---";
              else fout<<m_vNCcSigEvtTr[i];
              fout<<" & "<<m_vNSigEvtTr[i]+m_vNCcSigEvtTr[i]<<" & ";
              nCmltEvts=nCmltEvts+m_vNSigEvtTr[i]+m_vNCcSigEvtTr[i];          
            }
          else
            {
              nCmltEvts=nCmltEvts+m_vNSigEvtTr[i];
            }
          fout<<nCmltEvts<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          nLines=nLines+ceil(nVldSigEvtTr/((double ) nBrsInALine))+1;
          if(nLines>=nLinesMin||i==(m_vSigEvtTr.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
              nLines=0;
            }
        }

      //list<int> sigEvtIFSts; The list<int> variable sigEvtIFSts has been previously declared.
      nCmltEvts=0;
      for(unsigned int i=0;i<m_vSigEvtIFSts.size();i++)
        {
          if(i%nLinesMax==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0) fout<<"\\caption{Signal event initial-final states corresponding to signal event trees.}"<<endl;
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & signal event initial-final states & ";
              if(m_sprTopoTags==false) fout<<"iSigEvtIFSts & iEvtIFSts & ";
              fout<<"nEvts & ";
              if(m_ccSwitch==true) fout<<"nCcEvts & nTotEvts & ";
              fout<<"nCmltEvts \\\\"<<endl;
              fout<<"\\hline"<<endl;
            }
          fout<<i+1<<" & $ "; 
          sigEvtIFSts.clear();
          sigEvtIFSts=m_vSigEvtIFSts[i];
          list<int>::iterator liit=sigEvtIFSts.begin();
          writePnmFromPid(fout,"TexPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"\\rightarrow ";
          for(liit++;liit!=sigEvtIFSts.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"$ & ";
          if(m_sprTopoTags==false)
            {
              fout<<m_vISigEvtIFSts[i]<<" & ";
              if(m_iSigEvtIFStsIEvtIFStsMap.find(m_vISigEvtIFSts[i])!=m_iSigEvtIFStsIEvtIFStsMap.end())
                {
                  if(m_iSigEvtIFStsICcEvtIFStsMap[m_vISigEvtIFSts[i]]>=0) fout<<m_iSigEvtIFStsIEvtIFStsMap[m_vISigEvtIFSts[i]];
                  else fout<<"$ "<<m_iSigEvtIFStsIEvtIFStsMap[m_vISigEvtIFSts[i]]<<"_{cc} $";
                }
              else fout<<"---";
              fout<<" & ";
            }
          fout<<m_vNSigEvtIFSts[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigEvtIFSts[i]==0) fout<<"---";
              else fout<<m_vNCcSigEvtIFSts[i];
              fout<<" & "<<m_vNSigEvtIFSts[i]+m_vNCcSigEvtIFSts[i]<<" & ";
              nCmltEvts=nCmltEvts+m_vNSigEvtIFSts[i]+m_vNCcSigEvtIFSts[i];
            }
          else
            {
              nCmltEvts=nCmltEvts+m_vNSigEvtIFSts[i];
            }
          fout<<nCmltEvts<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          if(i%nLinesMax==nLinesMax-1||i==(m_vSigEvtIFSts.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
            }
        }
    }

  if(m_vSigEvtIFSts2.size()>0)
    {
      list<int> sigEvtIFSts2;
      nCmltEvts=0;
      for(unsigned int i=0;i<m_vSigEvtIFSts2.size();i++)
        {
          if(i%nLinesMax==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0) fout<<"\\caption{Signal event initial-final states.}"<<endl;
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & signal event initial-final states & ";
              if(m_sprTopoTags==false) fout<<"iSigEvtIFSts2 & iEvtIFSts & ";
              fout<<"nEvts & ";
              if(m_ccSwitch==true) fout<<"nCcEvts & nTotEvts & ";
              fout<<"nCmltEvts \\\\"<<endl;
              fout<<"\\hline"<<endl;
            }
          fout<<i+1<<" & $ "; 
          sigEvtIFSts2.clear();
          sigEvtIFSts2=m_vSigEvtIFSts2[i];
          list<int>::iterator liit=sigEvtIFSts2.begin();
          writePnmFromPid(fout,"TexPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"\\rightarrow ";
          for(liit++;liit!=sigEvtIFSts2.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"$ & ";
          if(m_sprTopoTags==false)
            {
              fout<<m_vISigEvtIFSts2[i]<<" & ";
              if(m_iSigEvtIFSts2IEvtIFStsMap.find(m_vISigEvtIFSts2[i])!=m_iSigEvtIFSts2IEvtIFStsMap.end())
                {
                  if(m_iSigEvtIFSts2ICcEvtIFStsMap[m_vISigEvtIFSts2[i]]>=0) fout<<m_iSigEvtIFSts2IEvtIFStsMap[m_vISigEvtIFSts2[i]];
                  else fout<<"$ "<<m_iSigEvtIFSts2IEvtIFStsMap[m_vISigEvtIFSts2[i]]<<"_{cc} $";
                }
              else fout<<"---";
              fout<<" & ";
            }
          fout<<m_vNSigEvtIFSts2[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigEvtIFSts2[i]==0) fout<<"---";
              else fout<<m_vNCcSigEvtIFSts2[i];
              fout<<" & "<<m_vNSigEvtIFSts2[i]+m_vNCcSigEvtIFSts2[i]<<" & ";
              nCmltEvts=nCmltEvts+m_vNSigEvtIFSts2[i]+m_vNCcSigEvtIFSts2[i];
            }
          else
            {
              nCmltEvts=nCmltEvts+m_vNSigEvtIFSts2[i];
            }
          fout<<nCmltEvts<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          if(i%nLinesMax==nLinesMax-1||i==(m_vSigEvtIFSts2.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
            }
        }
    }

  fout<<"\\end{document}"<<endl;
}
