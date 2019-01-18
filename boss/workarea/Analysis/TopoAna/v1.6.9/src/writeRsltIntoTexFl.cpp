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
  fout<<"\\title{Topology Analysis \\footnote{\\normalsize{This package is implemented with reference to a program called {\\sc Topo}, which is developed by Prof. Shuxian Du from Zhengzhou University in China and has been widely used by people in BESIII collaboration. Several years ago, when I was a PhD student working on BESIII experiment, I learned the idea of topology analysis and a lot of programming techniques from the {\\sc Topo} program. So, I really appreciate Prof. Du's original work very much. To meet my own needs and to practice developing analysis tools with C++, ROOT and LaTex, I wrote the package from scratch. At that time, the package functioned well but was relatively simple. At the end of last year (2017), my co-supervisor, Prof. Chengping Shen reminded me that it could be a useful tool for Belle II experiment as well. So, I revised and extended it, making it more well-rounded and suitable for Belle II experiment. Here, I would like to thank Prof. Du for his orignial work, Prof. Shen for his suggestion and encouragement, and Xian Xiong, Wencheng Yan, Sen Jia, Yubo Li, Suxian Li, Longke Li, Guanda Gong, Junhao Yin, Xiaoping Qin, Xiqing Hao, HongPeng Wang, JiaWei Zhang, Yeqi Chen and Runqiu Ma for their efforts in helping me test the program. I would also like to thank my good friend Xi Chen, a professional programmer, for his suggestion in core algrithm and efficiency.}} \\\\ \\vspace{0.1cm} \\Large{("<<m_verNum<<")}}"<<endl;
  fout<<"\\author{Xingyu Zhou \\footnote{\\normalsize{Email: zhouxy@buaa.edu.cn}} \\\\ \\vspace{0.1cm} Beihang University}"<<endl;
  fout<<"\\maketitle"<<endl;
  fout<<endl<<"\\clearpage"<<endl<<endl;
  fout<<"\\newgeometry{left=2.5cm,right=2.5cm,top=2.5cm,bottom=2.5cm}"<<endl<<endl;
  fout<<"\\listoftables"<<endl<<endl;
  fout<<"\\newgeometry{left=0.0cm,right=0.0cm,top=2.5cm,bottom=2.5cm}"<<endl;
  const unsigned int nLinesMin=36;
  const unsigned int nLinesMax=32;
  unsigned int nBrsInALine=6; // The value of this variable is changed to 5 for the case of the table titled signal decay trees and their respective final states.
  unsigned int nLines;
  unsigned long nCmltEtrs;

  if(m_sttaDcyTrsAndDcyFSts==false)
    {
      if(m_ccSwitch==true) nBrsInALine=5;
      nLines=0;
      vector< list<int> > dcyTr;
      list<int> dcyBr;
      list<int> dcyFSt;
      nCmltEtrs=0;
      unsigned long nDcyTrsToBePrtd=m_vDcyTr.size()<m_nDcyTrsToBePrtdMax?m_vDcyTr.size():m_nDcyTrsToBePrtdMax;
      for(unsigned long i=0;i<nDcyTrsToBePrtd;i++)
        {
          if(nLines==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0)
                {
                  fout<<"\\caption{Decay trees and their respective final states.}"<<endl;
                  nLines++;
                }
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & \\thead{decay tree \\\\ (decay final states)} & ";
              if(m_sprTopoTags==false) fout<<"iDcyTr & iDcyFSt & ";
              fout<<"nEtrs & ";
              if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & ";
              fout<<"nCmltEtrs \\\\"<<endl;
              nLines++;
              nLines++;
              fout<<"\\hline"<<endl;
            }
          dcyTr.clear();
          dcyTr=m_vDcyTr[i];
          unsigned int nVldDcyBrs=0;
          fout<<i+1<<" & \\makecell{ $ "<<endl;
          for(unsigned int j=0;j<dcyTr.size();j++)
            {
              if(j==0&&m_spr1stBrsOfDcyTrs==true&&dcyTr[0].size()==3) continue;
              dcyBr.clear();
              dcyBr=dcyTr[j];
              // Since a lot of pi0s are produced and almost all of them decay to gammma pairs, to save paper and for convenience of readers, all branches of pi0 to gamma pairs are not outputted into the tex file, and hence not printed in the pdf file.
              list<int>::iterator liit=dcyBr.begin();
              list<int>::iterator liit1=dcyBr.begin();
              liit1++;
              list<int>::iterator liit2=dcyBr.begin();
              liit2++;
              liit2++;
              if(((*liit)==111)&&(dcyBr.size()==3)&&((*liit1)==22)&&((*liit2)==22)) continue;
              nVldDcyBrs++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              // The condition "j==0" is set for the initial state particle pair e+e-;the condition "(*liit)==1, 2, 3, 4, 5 or 6" is set for the intermediate state quark pair ddbar, uubar, ssbar, ccbar, bbbar or ttbar;
              if(j==0||(*liit)==1||(*liit)==2||(*liit)==3||(*liit)==4||(*liit)==5||(*liit)==6)
                {
                  liit++;
                  writePnmFromPid(fout,"TexPnm",(*liit));
                }
              fout<<"\\rightarrow ";
              for(liit++;liit!=dcyBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              // The following bool variable is used to judge whether the current branch is the last one outputed or not.
              bool lastOneOrAllPi0sToGamPairsBehind=true;
              list<int> dcyBrTmp;
              for(unsigned int k=j+1;k<dcyTr.size();k++)
                {
                  dcyBrTmp.clear();
                  dcyBrTmp=dcyTr[k];
                  list<int>::iterator liitTmp=dcyBrTmp.begin();
                  list<int>::iterator liitTmp1=dcyBrTmp.begin();
                  liitTmp1++;
                  list<int>::iterator liitTmp2=dcyBrTmp.begin();
                  liitTmp2++;
                  liitTmp2++;
                  if(((*liitTmp)!=111)||(dcyBrTmp.size()!=3)||((*liitTmp1)!=22)||((*liitTmp2)!=22))
                    {
                      lastOneOrAllPi0sToGamPairsBehind=false;
                      break;
                    }
                }
              if(!lastOneOrAllPi0sToGamPairsBehind)
                {
                  fout<<","<<endl;
                  if(nVldDcyBrs%nBrsInALine==0) fout<<"$ \\\\ $"<<endl;
                }
              else
                {
                  fout<<endl<<"$ \\\\ ($"<<endl;
                  break;
                }
            }

          for(unsigned int j=0;j<m_vDcyFSt.size();j++)
            {
              if(m_vIDcyFSt[j]==m_iDcyTrIDcyFStMap[m_vIDcyTr[i]])
                {
                  dcyFSt.clear();
                  dcyFSt=m_vDcyFSt[j];
                  break;
                }
            }
          list<int>::iterator liit;
          if(m_spr1stBrsOfDcyTrs==true&&dcyTr[0].size()==3)
            {
              liit=dcyTr[0].begin();
              liit++;
              liit++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              liit=dcyFSt.begin();
              liit++;
            }
          else
            {
              liit=dcyFSt.begin();
              writePnmFromPid(fout,"TexPnm",(*liit));
              liit++;
              writePnmFromPid(fout,"TexPnm",(*liit));
            }
          fout<<"\\rightarrow ";
          for(liit++;liit!=dcyFSt.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<endl<<"$) } & ";
          if(m_sprTopoTags==false) fout<<m_vIDcyTr[i]<<" & "<<m_iDcyTrIDcyFStMap[m_vIDcyTr[i]]<<" & ";
          fout<<m_vNDcyTr[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcDcyTr[i]==0) fout<<"---";
              else fout<<m_vNCcDcyTr[i];
              fout<<" & "<<m_vNDcyTr[i]+m_vNCcDcyTr[i]<<" & ";
              nCmltEtrs=nCmltEtrs+m_vNDcyTr[i]+m_vNCcDcyTr[i];          
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNDcyTr[i];
            }
          fout<<nCmltEtrs<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          nLines=nLines+ceil(nVldDcyBrs/((double) nBrsInALine))+1;
          if(nLines>=nLinesMin||i==nDcyTrsToBePrtd-1)
            {
              if(i==nDcyTrsToBePrtd-1&&nDcyTrsToBePrtd<m_vDcyTr.size())
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
                      fout<<"index & \\thead{decay tree \\\\ (decay final states)} & ";
                      if(m_sprTopoTags==false) fout<<"iDcyTr & iDcyFSt & ";
                      fout<<"nEtrs & ";
                      if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & ";
                      fout<<"nCmltEtrs \\\\"<<endl;
                      fout<<"\\hline"<<endl;
                    }
                  fout<<"rest"<<" & \\makecell{ $ "<<endl;
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
                  unsigned long nCmltEtrsOfRest=0;
                  if(m_ccSwitch==true)
                    {
                      fout<<"---"<<" & "<<"---"<<" & ";
                      for(unsigned long i=nDcyTrsToBePrtd;i<m_vDcyTr.size();i++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vNDcyTr[i]+m_vNCcDcyTr[i];
                    }
                  else
                    {
                      for(unsigned long i=nDcyTrsToBePrtd;i<m_vDcyTr.size();i++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vNDcyTr[i];
                    }
                  nCmltEtrs=nCmltEtrs+nCmltEtrsOfRest;
                  fout<<nCmltEtrsOfRest<<" & "<<nCmltEtrs<<" \\\\"<<endl;
                  fout<<"\\hline"<<endl;
                }
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
              nLines=0;
            }
        }
      if(m_ccSwitch==true) nBrsInALine=6;

      //list<int> dcyFSt; The list<int> variable dcyFSt has been previously declared.
      nCmltEtrs=0;
      unsigned long nDcyFStsToBePrtd=m_vDcyFSt.size()<m_nDcyFStsToBePrtdMax?m_vDcyFSt.size():m_nDcyFStsToBePrtdMax;
      for(unsigned long i=0;i<nDcyFStsToBePrtd;i++)
        {
          if(i%nLinesMax==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0) fout<<"\\caption{Decay final states.}"<<endl;
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|"; 
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & decay final states & ";
              if(m_sprTopoTags==false) fout<<"iDcyFSt & ";
              fout<<"nEtrs & ";
              if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & ";
              fout<<"nCmltEtrs \\\\"<<endl;
              fout<<"\\hline"<<endl;
            }
          fout<<i+1<<" & $ "; 
          dcyFSt.clear();
          dcyFSt=m_vDcyFSt[i];
          list<int>::iterator liit=dcyFSt.begin();
          writePnmFromPid(fout,"TexPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"\\rightarrow ";
          for(liit++;liit!=dcyFSt.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"$ & ";
          if(m_sprTopoTags==false) fout<<m_vIDcyFSt[i]<<" & ";
          fout<<m_vNDcyFSt[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcDcyFSt[i]==0) fout<<"---";
              else fout<<m_vNCcDcyFSt[i];
              fout<<" & "<<m_vNDcyFSt[i]+m_vNCcDcyFSt[i]<<" & ";
              nCmltEtrs=nCmltEtrs+m_vNDcyFSt[i]+m_vNCcDcyFSt[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNDcyFSt[i];
            }
          fout<<nCmltEtrs<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          if(i%nLinesMax==nLinesMax-1||i==nDcyFStsToBePrtd-1)
            {
              if(i==nDcyFStsToBePrtd-1&&nDcyFStsToBePrtd<m_vDcyFSt.size())
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
                      fout<<"index & decay final states & ";
                      if(m_sprTopoTags==false) fout<<"iDcyFSt & ";
                      fout<<"nEtrs & ";
                      if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & ";
                      fout<<"nCmltEtrs \\\\"<<endl;
                      fout<<"\\hline"<<endl;
                    }
                  fout<<"rest"<<" & $ ";
                  writePnmFromPid(fout,"TexPnm",-11);
                  writePnmFromPid(fout,"TexPnm",11);
                  fout<<"\\rightarrow \\rm{others}";
                  fout<<" $ & ";
                  if(m_sprTopoTags==false) fout<<"---"<<" & ";
                  unsigned long nCmltEtrsOfRest=0;
                  if(m_ccSwitch==true)
                    {
                      fout<<"---"<<" & "<<"---"<<" & ";
                      for(unsigned long i=nDcyFStsToBePrtd;i<m_vDcyFSt.size();i++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vNDcyFSt[i]+m_vNCcDcyFSt[i];
                    }
                  else
                    {
                      for(unsigned long i=nDcyFStsToBePrtd;i<m_vDcyFSt.size();i++) nCmltEtrsOfRest=nCmltEtrsOfRest+m_vNDcyFSt[i];
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

  if(m_vVDcyBrP.size()>0)
    {
      for(unsigned int i=0;i<m_vVDcyBrP.size();i++)
        {
          list<int> dcyBrP;
          unsigned long nCmltCases=0;
          unsigned long nDcyBrPToBePrtd=m_vVDcyBrP[i].size()<m_vNDcyBrsMax[i]?m_vVDcyBrP[i].size():m_vNDcyBrsMax[i];
          for(unsigned int j=0;j<nDcyBrPToBePrtd;j++)
            {
              if(j%nLinesMax==0)
                {
                  fout<<endl<<"\\clearpage"<<endl<<endl;
                  fout<<"\\begin{table}[htbp!]"<<endl;
                  if(j==0)
                    {
                      fout<<"\\caption{Decay branches begun with $ ";
                      writePnmFromPid(fout,"TexPnm",m_vPttaPid[i]);
                      fout<<"$.}"<<endl;
                    }
                  fout<<"\\small"<<endl;
                  fout<<"\\centering"<<endl;
                  fout<<"\\begin{tabular}{|c|c|c|c|c|";
                  if(m_ccSwitch==true) fout<<"c|c|";
                  fout<<"}"<<endl;
                  fout<<"\\hline"<<endl;
                  fout<<"index & decay branch begun with $ ";
                  writePnmFromPid(fout,"TexPnm",m_vPttaPid[i]);
                  fout<<"$ & ";
                  if(m_sprTopoTags==false) fout<<"iDcyBrP"<<i+1<<" & ";
                  fout<<"nCases & ";
                  if(m_ccSwitch==true) fout<<"nCcCases & nTotCases & ";
                  fout<<"nCmltCases \\\\"<<endl;
                  fout<<"\\hline"<<endl;
                }
              fout<<j+1<<" & $ "; 
              dcyBrP.clear();
              dcyBrP=m_vVDcyBrP[i][j];
              list<int>::iterator liit=dcyBrP.begin();
              writePnmFromPid(fout,"TexPnm",(*liit));
              fout<<"\\rightarrow ";
              for(liit++;liit!=dcyBrP.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              fout<<"$ & ";
              if(m_sprTopoTags==false) fout<<m_vVIDcyBrP[i][j]<<" & ";
              fout<<m_vVNDcyBrP[i][j]<<" & ";
              if(m_ccSwitch==true)
                {
                  if(m_vPttaICcPid[i]==0&&m_vVIDcyBrCcP[i][j]==0) fout<<"---";
                  else fout<<m_vVNDcyBrCcP[i][j];
                  fout<<" & "<<m_vVNDcyBrP[i][j]+m_vVNDcyBrCcP[i][j]<<" & ";
                  nCmltCases=nCmltCases+m_vVNDcyBrP[i][j]+m_vVNDcyBrCcP[i][j];
                }
              else
                {
                  nCmltCases=nCmltCases+m_vVNDcyBrP[i][j];
                }
              fout<<nCmltCases<<" \\\\"<<endl;
              fout<<"\\hline"<<endl;
              if(j%nLinesMax==nLinesMax-1||j==nDcyBrPToBePrtd-1)
                {
                  if(j==nDcyBrPToBePrtd-1&&nDcyBrPToBePrtd<m_vVDcyBrP[i].size())
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
                          fout<<"index & decay branch begun with $ ";
                          writePnmFromPid(fout,"TexPnm",m_vPttaPid[i]);
                          fout<<"$ & ";
                          if(m_sprTopoTags==false) fout<<"iDcyBrP"<<i+1<<" & ";
                          fout<<"nCases & ";
                          if(m_ccSwitch==true) fout<<"nCcCases & nTotCases & ";
                          fout<<"nCmltCases \\\\"<<endl;
                          fout<<"\\hline"<<endl;
                        }
                      fout<<"rest"<<" & $ ";
                      dcyBrP.clear();
                      dcyBrP=m_vVDcyBrP[i][j];
                      list<int>::iterator liit=dcyBrP.begin();
                      writePnmFromPid(fout,"TexPnm",(*liit));
                      fout<<"\\rightarrow \\rm{others}";
                      fout<<" $ & ";
                      if(m_sprTopoTags==false) fout<<"---"<<" & ";
                      unsigned long nCmltCasesOfRest=0;
                      if(m_ccSwitch==true)
                        {
                          if(m_vPttaICcPid[i]==0)
                            {
                              fout<<"---"<<" & "<<"---"<<" & ";
                            }
                          else
                            {
                              unsigned long nCmltCasesOfRestTemp1=0;
                              unsigned long nCmltCasesOfRestTemp2=0;
                              for(unsigned int j=nDcyBrPToBePrtd;j<m_vVDcyBrP[i].size();j++) nCmltCasesOfRestTemp1=nCmltCasesOfRestTemp1+m_vVNDcyBrP[i][j];
                              for(unsigned int j=nDcyBrPToBePrtd;j<m_vVDcyBrP[i].size();j++) nCmltCasesOfRestTemp2=nCmltCasesOfRestTemp2+m_vVNDcyBrCcP[i][j];
                              fout<<nCmltCasesOfRestTemp1<<" & "<<nCmltCasesOfRestTemp2<<" & ";
                            }
                          for(unsigned int j=nDcyBrPToBePrtd;j<m_vVDcyBrP[i].size();j++) nCmltCasesOfRest=nCmltCasesOfRest+m_vVNDcyBrP[i][j]+m_vVNDcyBrCcP[i][j];
                        }
                      else
                        {
                          for(unsigned int j=nDcyBrPToBePrtd;j<m_vVDcyBrP[i].size();j++) nCmltCasesOfRest=nCmltCasesOfRest+m_vVNDcyBrP[i][j];
                        }
                      nCmltCases=nCmltCases+nCmltCasesOfRest;
                      fout<<nCmltCasesOfRest<<" & "<<nCmltCases<<" \\\\"<<endl;
                      fout<<"\\hline"<<endl;
                    }
                  fout<<"\\end{tabular}"<<endl;
                  fout<<"\\end{table}"<<endl;
                }
            }
        }
    }

  if(m_vVDcyBrIncDcyBr.size()>0)
    {
      for(unsigned int i=0;i<m_vVDcyBrIncDcyBr.size();i++)
        {
          list<int> dcyBrIncDcyBr;
          unsigned long nCmltCases=0;
          unsigned long nDcyBrIncDcyBrToBePrtd=m_vVDcyBrIncDcyBr[i].size()<m_vNDcyBrsMax1[i]?m_vVDcyBrIncDcyBr[i].size():m_vNDcyBrsMax1[i];
          for(unsigned int j=0;j<nDcyBrIncDcyBrToBePrtd;j++)
            {
              if(j%nLinesMax==0)
                {
                  fout<<endl<<"\\clearpage"<<endl<<endl;
                  fout<<"\\begin{table}[htbp!]"<<endl;
                  if(j==0)
                    {
                      fout<<"\\caption{Exclusive decay branches matched with $ ";
                      list<int>::iterator liit=m_vPttaIncDcyBr[i].begin();
                      writePnmFromPid(fout,"TexPnm",(*liit));
                      if((*liit)==-11)
                        {
                          liit++;
                          writePnmFromPid(fout,"TexPnm",(*liit));
                        }
                      fout<<"\\rightarrow ";
                      for(liit++;liit!=m_vPttaIncDcyBr[i].end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
                      fout<<"+ \\rm{anything} ";
                      fout<<"$.}"<<endl;
                    }
                  fout<<"\\small"<<endl;
                  fout<<"\\centering"<<endl;
                  fout<<"\\begin{tabular}{|c|c|c|c|c|";
                  if(m_ccSwitch==true) fout<<"c|c|";
                  fout<<"}"<<endl;
                  fout<<"\\hline"<<endl;
                  fout<<"index & exclusive decay branch matched with $ ";
                  list<int>::iterator liit=m_vPttaIncDcyBr[i].begin();
                  writePnmFromPid(fout,"TexPnm",(*liit));
                  if((*liit)==-11)
                    {
                      liit++;
                      writePnmFromPid(fout,"TexPnm",(*liit));
                    }
                  fout<<"\\rightarrow ";
                  for(liit++;liit!=m_vPttaIncDcyBr[i].end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
                  fout<<"+ \\rm{anything} ";
                  fout<<"$ & ";
                  if(m_sprTopoTags==false) fout<<"iDcyBrIncDcyBr"<<i+1<<" & ";
                  fout<<"nCases & ";
                  if(m_ccSwitch==true) fout<<"nCcCases & nTotCases & ";
                  fout<<"nCmltCases \\\\"<<endl;
                  fout<<"\\hline"<<endl;
                }
              fout<<j+1<<" & $ "; 
              dcyBrIncDcyBr.clear();
              dcyBrIncDcyBr=m_vVDcyBrIncDcyBr[i][j];
              list<int>::iterator liit=dcyBrIncDcyBr.begin();
              writePnmFromPid(fout,"TexPnm",(*liit));
              if((*liit)==-11)
                {
                  liit++;
                  writePnmFromPid(fout,"TexPnm",(*liit));
                }
              fout<<"\\rightarrow ";
              for(liit++;liit!=dcyBrIncDcyBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              fout<<"$ & ";
              if(m_sprTopoTags==false) fout<<m_vVIDcyBrIncDcyBr[i][j]<<" & ";
              fout<<m_vVNDcyBrIncDcyBr[i][j]<<" & ";
              if(m_ccSwitch==true)
                {
                  if(m_vPttaICcPid[i]==0&&m_vVIDcyBrCcIncDcyBr[i][j]==0) fout<<"---";
                  else fout<<m_vVNDcyBrCcIncDcyBr[i][j];
                  fout<<" & "<<m_vVNDcyBrIncDcyBr[i][j]+m_vVNDcyBrCcIncDcyBr[i][j]<<" & ";
                  nCmltCases=nCmltCases+m_vVNDcyBrIncDcyBr[i][j]+m_vVNDcyBrCcIncDcyBr[i][j];
                }
              else
                {
                  nCmltCases=nCmltCases+m_vVNDcyBrIncDcyBr[i][j];
                }
              fout<<nCmltCases<<" \\\\"<<endl;
              fout<<"\\hline"<<endl;
              if(j%nLinesMax==nLinesMax-1||j==nDcyBrIncDcyBrToBePrtd-1)
                {
                  if(j==nDcyBrIncDcyBrToBePrtd-1&&nDcyBrIncDcyBrToBePrtd<m_vVDcyBrIncDcyBr[i].size())
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
                          fout<<"index & exclusive decay branch matched with $ ";
                          list<int>::iterator liit=m_vPttaIncDcyBr[i].begin();
                          writePnmFromPid(fout,"TexPnm",(*liit));
                          if((*liit)==-11)
                            {
                              liit++;
                              writePnmFromPid(fout,"TexPnm",(*liit));
                            }
                          fout<<"\\rightarrow ";
                          for(liit++;liit!=m_vPttaIncDcyBr[i].end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
                          fout<<"+ \\rm{anything} ";
                          fout<<"$ & ";
                          if(m_sprTopoTags==false) fout<<"iDcyBrIncDcyBr"<<i+1<<" & ";
                          fout<<"nCases & ";
                          if(m_ccSwitch==true) fout<<"nCcCases & nTotCases & ";
                          fout<<"nCmltCases \\\\"<<endl;
                          fout<<"\\hline"<<endl;
                        }
                      fout<<"rest"<<" & $ ";
                      dcyBrIncDcyBr.clear();
                      dcyBrIncDcyBr=m_vVDcyBrIncDcyBr[i][j];
                      list<int>::iterator liit=dcyBrIncDcyBr.begin();
                      writePnmFromPid(fout,"TexPnm",(*liit));
                      if((*liit)==-11)
                        {
                          liit++;
                          writePnmFromPid(fout,"TexPnm",(*liit));
                        }
                      fout<<"\\rightarrow \\rm{others}";
                      fout<<" $ & ";
                      if(m_sprTopoTags==false) fout<<"---"<<" & ";
                      unsigned long nCmltCasesOfRest=0;
                      if(m_ccSwitch==true)
                        {
                          if(m_vPttaICcPid[i]==0)
                            {
                              fout<<"---"<<" & "<<"---"<<" & ";
                            }
                          else
                            {
                              unsigned long nCmltCasesOfRestTemp1=0;
                              unsigned long nCmltCasesOfRestTemp2=0;
                              for(unsigned int j=nDcyBrIncDcyBrToBePrtd;j<m_vVDcyBrIncDcyBr[i].size();j++) nCmltCasesOfRestTemp1=nCmltCasesOfRestTemp1+m_vVNDcyBrIncDcyBr[i][j];
                              for(unsigned int j=nDcyBrIncDcyBrToBePrtd;j<m_vVDcyBrIncDcyBr[i].size();j++) nCmltCasesOfRestTemp2=nCmltCasesOfRestTemp2+m_vVNDcyBrCcIncDcyBr[i][j];
                              fout<<nCmltCasesOfRestTemp1<<" & "<<nCmltCasesOfRestTemp2<<" & ";
                            }
                          for(unsigned int j=nDcyBrIncDcyBrToBePrtd;j<m_vVDcyBrIncDcyBr[i].size();j++) nCmltCasesOfRest=nCmltCasesOfRest+m_vVNDcyBrIncDcyBr[i][j]+m_vVNDcyBrCcIncDcyBr[i][j];
                        }
                      else
                        {
                          for(unsigned int j=nDcyBrIncDcyBrToBePrtd;j<m_vVDcyBrIncDcyBr[i].size();j++) nCmltCasesOfRest=nCmltCasesOfRest+m_vVNDcyBrIncDcyBr[i][j];
                        }
                      nCmltCases=nCmltCases+nCmltCasesOfRest;
                      fout<<nCmltCasesOfRest<<" & "<<nCmltCases<<" \\\\"<<endl;
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

  if(m_vSigIncDcyBr.size()>0)
    {
      list<int> sigIncDcyBr;
      unsigned long nCmltCases=0;
      for(unsigned int i=0;i<m_vSigIncDcyBr.size();i++)
        {
          if(i%nLinesMax==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0) fout<<"\\caption{Signal inclusive decay branches.}"<<endl;
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & signal inclusive decay branch & ";
              if(m_sprTopoTags==false) fout<<"iSigIncDcyBr & ";
              fout<<"nCases & ";
              if(m_ccSwitch==true) fout<<"nCcCases & nTotCases & "<<endl;
              fout<<"nCmltCases \\\\"<<endl;
              fout<<"\\hline"<<endl;
            }
          fout<<i+1<<" & $ "; 
          sigIncDcyBr.clear();
          sigIncDcyBr=m_vSigIncDcyBr[i];
          list<int>::iterator liit=sigIncDcyBr.begin();
          writePnmFromPid(fout,"TexPnm",(*liit));
          if((*liit)==-11)
            {
              liit++;
              writePnmFromPid(fout,"TexPnm",(*liit));
            }          
          fout<<"\\rightarrow ";
          for(liit++;liit!=sigIncDcyBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<" + \\rm{anything}";
          fout<<"$ & ";
          if(m_sprTopoTags==false) fout<<m_vISigIncDcyBr[i]<<" & ";
          fout<<m_vNSigIncDcyBr[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigIncDcyBr[i]>0) fout<<m_vNCcSigIncDcyBr[i];
              else fout<<"---";
              fout<<" & "<<m_vNSigIncDcyBr[i]+m_vNCcSigIncDcyBr[i]<<" & ";
              nCmltCases=nCmltCases+m_vNSigIncDcyBr[i]+m_vNCcSigIncDcyBr[i];
            }
          else
            {
              nCmltCases=nCmltCases+m_vNSigIncDcyBr[i];
            }
          fout<<nCmltCases<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          if(i%nLinesMax==nLinesMax-1||i==(m_vSigIncDcyBr.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
            }
        }
    }

  if(m_vSigSeqDcyBrs.size()>0)
    {
      if(m_ccSwitch==true) nBrsInALine=4;
      else nBrsInALine=5;
      nLines=0;
      vector< list<int> > sigSeqDcyBrs;
      vector<int> vSigSeqDcyBrsIdxOfHead;
      list<int> sigDcyBr;
      unsigned long nCmltCases=0;
      for(unsigned int i=0;i<m_vSigSeqDcyBrs.size();i++)
        {
          if(nLines==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0)
                {
                  fout<<"\\caption{Signal sequential decay branches.}"<<endl;
                  nLines++;
                }
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & \\thead{signal sequential decay branches} & ";
              if(m_sprTopoTags==false) fout<<"iSigSeqDcyBrs & ";
              fout<<"nCases & ";
              if(m_ccSwitch==true) fout<<"nCcCases & nTotCases & "<<endl;
              fout<<"nCmltCases \\\\"<<endl;
              nLines++;
              fout<<"\\hline"<<endl;
            }
          sigSeqDcyBrs.clear();
          sigSeqDcyBrs=m_vSigSeqDcyBrs[i];
          vSigSeqDcyBrsIdxOfHead=m_vVSigSeqDcyBrsIdxOfHead[i];
          unsigned int nVldSigSeqDcyBrs=0;
          fout<<i+1<<" & \\makecell{ $ "<<endl;
          for(unsigned int j=0;j<sigSeqDcyBrs.size();j++)
            {
              sigDcyBr.clear();
              sigDcyBr=sigSeqDcyBrs[j];              
              list<int>::iterator liit=sigDcyBr.begin();
              nVldSigSeqDcyBrs++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              if(j==0&&vSigSeqDcyBrsIdxOfHead[0]==-1)
              {
                liit++;
                writePnmFromPid(fout,"TexPnm",(*liit));
              }
              fout<<"\\rightarrow ";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              if(j<(sigSeqDcyBrs.size()-1))
                {
                  fout<<","<<endl;
                  if(nVldSigSeqDcyBrs%nBrsInALine==0) fout<<"$ \\\\ $"<<endl;
                }
              else
                {
                  fout<<endl<<"$ }";
                  break;
                }
            }

          fout<<" & ";
          if(m_sprTopoTags==false) fout<<m_vISigSeqDcyBrs[i]<<" & ";
          fout<<m_vNSigSeqDcyBrs[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigSeqDcyBrs[i]>0) fout<<m_vNCcSigSeqDcyBrs[i];
              else fout<<"---";
              fout<<" & "<<m_vNSigSeqDcyBrs[i]+m_vNCcSigSeqDcyBrs[i]<<" & ";
              nCmltCases=nCmltCases+m_vNSigSeqDcyBrs[i]+m_vNCcSigSeqDcyBrs[i];
            }
          else
            {
              nCmltCases=nCmltCases+m_vNSigSeqDcyBrs[i];
            }
          fout<<nCmltCases<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          nLines=nLines+ceil(nVldSigSeqDcyBrs/((double ) nBrsInALine));
          if(nLines>=nLinesMin||i==(m_vSigSeqDcyBrs.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
              nLines=0;
            }
        }
      if(m_ccSwitch==true) nBrsInALine=5;   
    }

  if(m_vSigIncSeqDcyBrs.size()>0)
    {
      if(m_ccSwitch==true) nBrsInALine=4;
      else nBrsInALine=5;
      nLines=0;
      vector< list<int> > sigIncSeqDcyBrs;
      vector<int> vSigIncSeqDcyBrsIdxOfHead;
      list<int> sigDcyBr;
      unsigned long nCmltCases=0;
      for(unsigned int i=0;i<m_vSigIncSeqDcyBrs.size();i++)
        {
          if(nLines==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0)
                {
                  fout<<"\\caption{Signal inclusive sequential decay branches.}"<<endl;
                  nLines++;
                }
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & \\thead{signal inclusive sequential decay branches} & ";
              if(m_sprTopoTags==false) fout<<"iSigIncSeqDcyBrs & ";
              fout<<"nCases & ";
              if(m_ccSwitch==true) fout<<"nCcCases & nTotCases & "<<endl;
              fout<<"nCmltCases \\\\"<<endl;
              nLines++;
              fout<<"\\hline"<<endl;
            }
          sigIncSeqDcyBrs.clear();
          sigIncSeqDcyBrs=m_vSigIncSeqDcyBrs[i];
          vSigIncSeqDcyBrsIdxOfHead=m_vVSigIncSeqDcyBrsIdxOfHead[i];
          unsigned int nVldSigIncSeqDcyBrs=0;
          fout<<i+1<<" & \\makecell{ $ "<<endl;
          for(unsigned int j=0;j<sigIncSeqDcyBrs.size();j++)
            {
              sigDcyBr.clear();
              sigDcyBr=sigIncSeqDcyBrs[j];              
              list<int>::iterator liit=sigDcyBr.begin();
              nVldSigIncSeqDcyBrs++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              if(j==0&&vSigIncSeqDcyBrsIdxOfHead[0]==-1)
              {
                liit++;
                writePnmFromPid(fout,"TexPnm",(*liit));
              }
              fout<<"\\rightarrow ";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              if(m_vVIIncSigIncSeqDcyBrs[i][j]==1) writePnmFromPid(fout,"TexPnm",m_pidOfAnything);
              if(j<(sigIncSeqDcyBrs.size()-1))
                {
                  fout<<","<<endl;
                  if(nVldSigIncSeqDcyBrs%nBrsInALine==0) fout<<"$ \\\\ $"<<endl;
                }
              else
                {
                  fout<<endl<<"$ }";
                  break;
                }
            }

          fout<<" & ";
          if(m_sprTopoTags==false) fout<<m_vISigIncSeqDcyBrs[i]<<" & ";
          fout<<m_vNSigIncSeqDcyBrs[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigIncSeqDcyBrs[i]>0) fout<<m_vNCcSigIncSeqDcyBrs[i];
              else fout<<"---";
              fout<<" & "<<m_vNSigIncSeqDcyBrs[i]+m_vNCcSigIncSeqDcyBrs[i]<<" & ";
              nCmltCases=nCmltCases+m_vNSigIncSeqDcyBrs[i]+m_vNCcSigIncSeqDcyBrs[i];
            }
          else
            {
              nCmltCases=nCmltCases+m_vNSigIncSeqDcyBrs[i];
            }
          fout<<nCmltCases<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          nLines=nLines+ceil(nVldSigIncSeqDcyBrs/((double ) nBrsInALine));
          if(nLines>=nLinesMin||i==(m_vSigIncSeqDcyBrs.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
              nLines=0;
            }
        }
      if(m_ccSwitch==true) nBrsInALine=5;   
    }

  if(m_vSigIRADcyBr.size()>0)
    {
      list<int> sigIRADcyBr;
      unsigned long nCmltCases=0;
      for(unsigned int i=0;i<m_vSigIRADcyBr.size();i++)
        {
          if(i%nLinesMax==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0) fout<<"\\caption{Signal intermediate-resonance-allowed decay branches.}"<<endl;
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & signal intermediate-resonance-allowed decay branch & ";
              if(m_sprTopoTags==false) fout<<"iSigIRADcyBr & ";
              fout<<"nCases & ";
              if(m_ccSwitch==true) fout<<"nCcCases & nTotCases & "<<endl;
              fout<<"nCmltCases \\\\"<<endl;
              fout<<"\\hline"<<endl;
            }
          fout<<i+1<<" & $ "; 
          sigIRADcyBr.clear();
          sigIRADcyBr=m_vSigIRADcyBr[i];
          list<int>::iterator liit=sigIRADcyBr.begin();
          writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"\\rightarrow ";
          for(liit++;liit!=sigIRADcyBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"$ & ";
          if(m_sprTopoTags==false) fout<<m_vISigIRADcyBr[i]<<" & ";
          fout<<m_vNSigIRADcyBr[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigIRADcyBr[i]>0) fout<<m_vNCcSigIRADcyBr[i];
              else fout<<"---";
              fout<<" & "<<m_vNSigIRADcyBr[i]+m_vNCcSigIRADcyBr[i]<<" & ";
              nCmltCases=nCmltCases+m_vNSigIRADcyBr[i]+m_vNCcSigIRADcyBr[i];
            }
          else
            {
              nCmltCases=nCmltCases+m_vNSigIRADcyBr[i];
            }
          fout<<nCmltCases<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          if(i%nLinesMax==nLinesMax-1||i==(m_vSigIRADcyBr.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
            }
        }
    }

  if(m_vSigIncOrIRASeqDcyBrs.size()>0)
    {
      if(m_ccSwitch==true) nBrsInALine=4;
      else nBrsInALine=5;
      nLines=0;
      vector< list<int> > sigIncOrIRASeqDcyBrs;
      vector<int> vSigIncOrIRASeqDcyBrsIdxOfHead;
      list<int> sigDcyBr;
      unsigned long nCmltCases=0;
      for(unsigned int i=0;i<m_vSigIncOrIRASeqDcyBrs.size();i++)
        {
          if(nLines==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0)
                {
                  fout<<"\\caption{Signal inclusive or intermediate-resonance-allowed sequential decay branches.}"<<endl;
                  nLines++;
                }
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & \\thead{signal inclusive or intermediate-resonance-allowed \\\\ sequential decay branches} & ";
              if(m_sprTopoTags==false) fout<<"iSigIncOrIRASeqDcyBrs & ";
              fout<<"nCases & ";
              if(m_ccSwitch==true) fout<<"nCcCases & nTotCases & "<<endl;
              fout<<"nCmltCases \\\\"<<endl;
              nLines++;
              fout<<"\\hline"<<endl;
            }
          sigIncOrIRASeqDcyBrs.clear();
          sigIncOrIRASeqDcyBrs=m_vSigIncOrIRASeqDcyBrs[i];
          vSigIncOrIRASeqDcyBrsIdxOfHead=m_vVSigIncOrIRASeqDcyBrsIdxOfHead[i];
          unsigned int nVldSigIncOrIRASeqDcyBrs=0;
          fout<<i+1<<" & \\makecell{ $ "<<endl;
          for(unsigned int j=0;j<sigIncOrIRASeqDcyBrs.size();j++)
            {
              sigDcyBr.clear();
              sigDcyBr=sigIncOrIRASeqDcyBrs[j];              
              list<int>::iterator liit=sigDcyBr.begin();
              nVldSigIncOrIRASeqDcyBrs++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              if(j==0&&vSigIncOrIRASeqDcyBrsIdxOfHead[0]==-1)
                {
                  liit++;
                  writePnmFromPid(fout,"TexPnm",(*liit));
                }
              if(m_vVIIRASigIncOrIRASeqDcyBrs[i][j]==1) fout<<"\\; ( \\rightarrow X ) ";
              fout<<"\\rightarrow ";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              if(m_vVIIncSigIncOrIRASeqDcyBrs[i][j]==1) writePnmFromPid(fout,"TexPnm",m_pidOfAnything);
              // The following bool variable is used to judge whether the current branch is the last one outputed or not.
              if(j<(sigIncOrIRASeqDcyBrs.size()-1))
                {
                  fout<<","<<endl;
                  if(nVldSigIncOrIRASeqDcyBrs%nBrsInALine==0) fout<<"$ \\\\ $"<<endl;
                }
              else
                {
                  fout<<endl<<"$ }";
                  break;
                }
            }

          fout<<" & ";
          if(m_sprTopoTags==false) fout<<m_vISigIncOrIRASeqDcyBrs[i]<<" & ";
          fout<<m_vNSigIncOrIRASeqDcyBrs[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigIncOrIRASeqDcyBrs[i]>0) fout<<m_vNCcSigIncOrIRASeqDcyBrs[i];
              else fout<<"---";
              fout<<" & "<<m_vNSigIncOrIRASeqDcyBrs[i]+m_vNCcSigIncOrIRASeqDcyBrs[i]<<" & ";
              nCmltCases=nCmltCases+m_vNSigIncOrIRASeqDcyBrs[i]+m_vNCcSigIncOrIRASeqDcyBrs[i];
            }
          else
            {
              nCmltCases=nCmltCases+m_vNSigIncOrIRASeqDcyBrs[i];
            }
          fout<<nCmltCases<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          nLines=nLines+ceil(nVldSigIncOrIRASeqDcyBrs/((double ) nBrsInALine));
          if(nLines>=nLinesMin||i==(m_vSigIncOrIRASeqDcyBrs.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
              nLines=0;
            }
        }
      if(m_ccSwitch==true) nBrsInALine=5;   
    }

  if(m_vSigDcyTr.size()>0)
    {
      if(m_ccSwitch==true) nBrsInALine=4;
      else nBrsInALine=5;
      nLines=0;
      vector< list<int> > sigDcyTr;
      list<int> sigDcyBr;
      list<int> sigDcyFSt;
      nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigDcyTr.size();i++)
        {
          if(nLines==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0)
                {
                  fout<<"\\caption{Signal decay trees and their respective final states.}"<<endl;
                  nLines++;
                }
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & \\thead{signal decay tree \\\\ (signal decay final states)} & ";
              if(m_sprTopoTags==false) fout<<"iSigDcyTr & iSigDcyFSt & iDcyTr & iDcyFSt & ";
              fout<<"nEtrs & ";
              if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & ";
              fout<<"nCmltEtrs \\\\"<<endl;
              nLines++;
              nLines++;
              fout<<"\\hline"<<endl;
            }
          sigDcyTr.clear();
          sigDcyTr=m_vSigDcyTr[i];
          unsigned int nVldSigDcyTr=0;
          fout<<i+1<<" & \\makecell{ $ "<<endl;
          for(unsigned int j=0;j<sigDcyTr.size();j++)
            {
              if(j==0&&m_spr1stBrsOfDcyTrs==true&&sigDcyTr[0].size()==3) continue;
              sigDcyBr.clear();
              sigDcyBr=sigDcyTr[j];              
              // Since a lot of pi0s are produced and almost all of them decay to gammma pairs, to save paper and for convenience of readers, all branches of pi0 to gamma pairs are not outputted into the tex file, and hence not printed in the pdf file.
              list<int>::iterator liit=sigDcyBr.begin();
              list<int>::iterator liit1=sigDcyBr.begin();
              liit1++;
              list<int>::iterator liit2=sigDcyBr.begin();
              liit2++;
              liit2++;
              if(((*liit)==111)&&(sigDcyBr.size()==3)&&((*liit1)==22)&&((*liit2)==22)) continue;
              nVldSigDcyTr++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              // The condition "j==0" is set for the initial state particle pair e+e-;the condition "(*liit)==1, 2, 3, 4, 5 or 6" is set for the intermediate state quark pair ddbar, uubar, ssbar, ccbar, bbbar or ttbar;
              if(j==0||(*liit)==1||(*liit)==2||(*liit)==3||(*liit)==4||(*liit)==5||(*liit)==6)
                {
                  liit++;
                  writePnmFromPid(fout,"TexPnm",(*liit));
                }
              fout<<"\\rightarrow ";
              for(liit++;liit!=sigDcyBr.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
              // The following bool variable is used to judge whether the current branch is the last one outputed or not.
              bool lastOneOrAllPi0sToGamPairsBehind=true;
              list<int> sigDcyBrTmp;
              for(unsigned int k=j+1;k<sigDcyTr.size();k++)
                {
                  sigDcyBrTmp.clear();
                  sigDcyBrTmp=sigDcyTr[k];
                  list<int>::iterator liitTmp=sigDcyBrTmp.begin();
                  list<int>::iterator liitTmp1=sigDcyBrTmp.begin();
                  liitTmp1++;
                  list<int>::iterator liitTmp2=sigDcyBrTmp.begin();
                  liitTmp2++;
                  liitTmp2++;
                  if(((*liitTmp)!=111)||(sigDcyBrTmp.size()!=3)||((*liitTmp1)!=22)||((*liitTmp2)!=22))
                    {
                      lastOneOrAllPi0sToGamPairsBehind=false;
                      break;
                    }
                }
              if(!lastOneOrAllPi0sToGamPairsBehind)
                {
                  fout<<","<<endl;
                  if(nVldSigDcyTr%nBrsInALine==0) fout<<"$ \\\\ $"<<endl;
                }
              else
                {
                  fout<<endl<<"$ \\\\ ($"<<endl;
                  break;
                }
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
          list<int>::iterator liit;
          if(m_spr1stBrsOfDcyTrs==true&&sigDcyTr[0].size()==3)
            {
              liit=sigDcyTr[0].begin();
              liit++;
              liit++;
              writePnmFromPid(fout,"TexPnm",(*liit));
              liit=sigDcyFSt.begin();
              liit++;
            }
          else
            {
              liit=sigDcyFSt.begin();
              writePnmFromPid(fout,"TexPnm",(*liit));
              liit++;
              writePnmFromPid(fout,"TexPnm",(*liit));
            }
          fout<<"\\rightarrow ";
          for(liit++;liit!=sigDcyFSt.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<endl<<"$) } & ";
          if(m_sprTopoTags==false)
            {
              fout<<m_vISigDcyTr[i]<<" & ";
              if(m_iSigDcyTrICcSigDcyFStMap[m_vISigDcyTr[i]]>=0) fout<<m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]];
              else fout<<"$ "<<m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]]<<"_{\\rm cc} $";
              fout<<" & ";
              if(m_iSigDcyTrIDcyTrMap.find(m_vISigDcyTr[i])!=m_iSigDcyTrIDcyTrMap.end())
                {
                  if(m_iSigDcyTrICcDcyTrMap[m_vISigDcyTr[i]]>=0) fout<<m_iSigDcyTrIDcyTrMap[m_vISigDcyTr[i]];
                  else fout<<"$ "<<m_iSigDcyTrIDcyTrMap[m_vISigDcyTr[i]]<<"_{\\rm cc} $";
                }
              else fout<<"---";
              fout<<" & ";
              if(m_iSigDcyFStIDcyFStMap.find(m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]])!=m_iSigDcyFStIDcyFStMap.end())
                {
                  if(m_iSigDcyFStICcDcyFStMap[m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]]]>=0) fout<<m_iSigDcyFStIDcyFStMap[m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]]];
                  else fout<<"$ "<<m_iSigDcyFStIDcyFStMap[m_iSigDcyTrISigDcyFStMap[m_vISigDcyTr[i]]]<<"_{\\rm cc} $";
                }
              else fout<<"---";
              fout<<" & ";
            }
          fout<<m_vNSigDcyTr[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigDcyTr[i]==0) fout<<"---";
              else fout<<m_vNCcSigDcyTr[i];
              fout<<" & "<<m_vNSigDcyTr[i]+m_vNCcSigDcyTr[i]<<" & ";
              nCmltEtrs=nCmltEtrs+m_vNSigDcyTr[i]+m_vNCcSigDcyTr[i];          
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNSigDcyTr[i];
            }
          fout<<nCmltEtrs<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          nLines=nLines+ceil(nVldSigDcyTr/((double ) nBrsInALine))+1;
          if(nLines>=nLinesMin||i==(m_vSigDcyTr.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
              nLines=0;
            }
        }

      //list<int> sigDcyFSt; The list<int> variable sigDcyFSt has been previously declared.
      nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigDcyFSt.size();i++)
        {
          if(i%nLinesMax==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0) fout<<"\\caption{Signal decay final states corresponding to signal decay trees.}"<<endl;
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & signal decay final states & ";
              if(m_sprTopoTags==false) fout<<"iSigDcyFSt & iDcyFSt & ";
              fout<<"nEtrs & ";
              if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & ";
              fout<<"nCmltEtrs \\\\"<<endl;
              fout<<"\\hline"<<endl;
            }
          fout<<i+1<<" & $ "; 
          sigDcyFSt.clear();
          sigDcyFSt=m_vSigDcyFSt[i];
          list<int>::iterator liit=sigDcyFSt.begin();
          writePnmFromPid(fout,"TexPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"\\rightarrow ";
          for(liit++;liit!=sigDcyFSt.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"$ & ";
          if(m_sprTopoTags==false)
            {
              fout<<m_vISigDcyFSt[i]<<" & ";
              if(m_iSigDcyFStIDcyFStMap.find(m_vISigDcyFSt[i])!=m_iSigDcyFStIDcyFStMap.end())
                {
                  if(m_iSigDcyFStICcDcyFStMap[m_vISigDcyFSt[i]]>=0) fout<<m_iSigDcyFStIDcyFStMap[m_vISigDcyFSt[i]];
                  else fout<<"$ "<<m_iSigDcyFStIDcyFStMap[m_vISigDcyFSt[i]]<<"_{\\rm cc} $";
                }
              else fout<<"---";
              fout<<" & ";
            }
          fout<<m_vNSigDcyFSt[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigDcyFSt[i]==0) fout<<"---";
              else fout<<m_vNCcSigDcyFSt[i];
              fout<<" & "<<m_vNSigDcyFSt[i]+m_vNCcSigDcyFSt[i]<<" & ";
              nCmltEtrs=nCmltEtrs+m_vNSigDcyFSt[i]+m_vNCcSigDcyFSt[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNSigDcyFSt[i];
            }
          fout<<nCmltEtrs<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          if(i%nLinesMax==nLinesMax-1||i==(m_vSigDcyFSt.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
            }
        }
    }

  if(m_vSigDcyFSt2.size()>0)
    {
      list<int> sigDcyFSt2;
      nCmltEtrs=0;
      for(unsigned int i=0;i<m_vSigDcyFSt2.size();i++)
        {
          if(i%nLinesMax==0)
            {
              fout<<endl<<"\\clearpage"<<endl<<endl;
              fout<<"\\begin{table}[htbp!]"<<endl;
              if(i==0) fout<<"\\caption{Signal decay final states.}"<<endl;
              fout<<"\\small"<<endl;
              fout<<"\\centering"<<endl;
              fout<<"\\begin{tabular}{|c|c|c|c|c|c|";
              if(m_ccSwitch==true) fout<<"c|c|";
              fout<<"}"<<endl;
              fout<<"\\hline"<<endl;
              fout<<"index & signal decay final states & ";
              if(m_sprTopoTags==false) fout<<"iSigDcyFSt2 & iDcyFSt & ";
              fout<<"nEtrs & ";
              if(m_ccSwitch==true) fout<<"nCcEtrs & nTotEtrs & ";
              fout<<"nCmltEtrs \\\\"<<endl;
              fout<<"\\hline"<<endl;
            }
          fout<<i+1<<" & $ "; 
          sigDcyFSt2.clear();
          sigDcyFSt2=m_vSigDcyFSt2[i];
          list<int>::iterator liit=sigDcyFSt2.begin();
          writePnmFromPid(fout,"TexPnm",(*liit));
          liit++;
          writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"\\rightarrow ";
          for(liit++;liit!=sigDcyFSt2.end();liit++) writePnmFromPid(fout,"TexPnm",(*liit));
          fout<<"$ & ";
          if(m_sprTopoTags==false)
            {
              fout<<m_vISigDcyFSt2[i]<<" & ";
              if(m_iSigDcyFSt2IDcyFStMap.find(m_vISigDcyFSt2[i])!=m_iSigDcyFSt2IDcyFStMap.end())
                {
                  if(m_iSigDcyFSt2ICcDcyFStMap[m_vISigDcyFSt2[i]]>=0) fout<<m_iSigDcyFSt2IDcyFStMap[m_vISigDcyFSt2[i]];
                  else fout<<"$ "<<m_iSigDcyFSt2IDcyFStMap[m_vISigDcyFSt2[i]]<<"_{\\rm cc} $";
                }
              else fout<<"---";
              fout<<" & ";
            }
          fout<<m_vNSigDcyFSt2[i]<<" & ";
          if(m_ccSwitch==true)
            {
              if(m_vICcSigDcyFSt2[i]==0) fout<<"---";
              else fout<<m_vNCcSigDcyFSt2[i];
              fout<<" & "<<m_vNSigDcyFSt2[i]+m_vNCcSigDcyFSt2[i]<<" & ";
              nCmltEtrs=nCmltEtrs+m_vNSigDcyFSt2[i]+m_vNCcSigDcyFSt2[i];
            }
          else
            {
              nCmltEtrs=nCmltEtrs+m_vNSigDcyFSt2[i];
            }
          fout<<nCmltEtrs<<" \\\\"<<endl;
          fout<<"\\hline"<<endl;
          if(i%nLinesMax==nLinesMax-1||i==(m_vSigDcyFSt2.size()-1))
            {
              fout<<"\\end{tabular}"<<endl;
              fout<<"\\end{table}"<<endl;
            }
        }
    }

  fout<<"\\end{document}"<<endl;
}
