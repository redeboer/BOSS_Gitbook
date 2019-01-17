#include "../include/topoana.h"
#include <iostream>

unsigned int topoana::countIncSeqEvtBrsInEvtTr(vector<int> vIIncIncSeqEvtBrs, vector< list<int> > & incSeqEvtBrs, vector<int> vIdxOfHead1, vector<int> vMidxOfHead1, vector< list<int> > & evtTr, vector<int> vIdxOfHead2, vector<int> vMidxOfHead2)
{
  unsigned int nCount=0;
  if(incSeqEvtBrs.size()==0)
    {
      cerr<<"Infor: The size of the vector for the inclusive sequential event branches is zero!"<<endl<<endl;
      return nCount;
    }
  if(evtTr.size()==0)
    {
      cerr<<"Infor: The size of the vector for the event tree is zero!"<<endl<<endl;
      return nCount;
    }
  if(incSeqEvtBrs.size()<=evtTr.size())
    {
      /*for(unsigned int i=0;i<incSeqEvtBrs.size();i++)
        {
          list<int>::iterator liit,liitTmp=incSeqEvtBrs[i].end();liitTmp--;
          for(liit=incSeqEvtBrs[i].begin();liit!=incSeqEvtBrs[i].end();liit++)
            {
              if(liit!=liitTmp) cout<<(*liit)<<"\t";
              else cout<<(*liit)<<endl;
            }
        }
      cout<<endl;
      for(unsigned int i=0;i<evtTr.size();i++)
        {
          list<int>::iterator liit,liitTmp=evtTr[i].end();liitTmp--;
          for(liit=evtTr[i].begin();liit!=evtTr[i].end();liit++)
            {
              if(liit!=liitTmp) cout<<(*liit)<<"\t";
              else cout<<(*liit)<<endl;
            }
        }
      cout<<endl;*/
      vector< vector<int> > vVIdx2ToIdx1;
      vVIdx2ToIdx1.clear();
      vector<int> vIdx2ToIdx1;
      if(vIIncIncSeqEvtBrs[0]==0)
        {
          for(unsigned int i=0;i<evtTr.size();i++)
            {
              if(evtTr[i]==incSeqEvtBrs[0])
                {
                  vIdx2ToIdx1.clear();
                  vIdx2ToIdx1.push_back(i);
                  vVIdx2ToIdx1.push_back(vIdx2ToIdx1);
                }
            }
        }
      else
        {
          for(unsigned int i=0;i<evtTr.size();i++)
            {
              if(isLiaMatchedWithLib(incSeqEvtBrs[0],evtTr[i]))
                {
                  vIdx2ToIdx1.clear();
                  vIdx2ToIdx1.push_back(i);
                  vVIdx2ToIdx1.push_back(vIdx2ToIdx1);
                }
            }
        } 

      vector<int> vMidx1;
      vMidx1.push_back(0);
      for(unsigned int i=1;i<incSeqEvtBrs.size();i++)
        {
          if(vIdxOfHead1[i]==vMidxOfHead1[i])
            {
              vMidx1.push_back(0);
            }
          else
            {
              for(unsigned int j=0;j<i;j++)
                {
                  if(vIdxOfHead1[j]==vMidxOfHead1[i])
                    {
                      vMidx1.push_back(j);
                      break;
                    }
                }
            }
        }

      for(unsigned int i=0;i<vVIdx2ToIdx1.size();i++)
        {
          for(unsigned int j=1;j<incSeqEvtBrs.size();j++)
            {
              for(unsigned int k=vVIdx2ToIdx1[i][j-1]+1;k<evtTr.size();k++)
                {
                  if(vIIncIncSeqEvtBrs[j]==0)
                    {                  
                      if(evtTr[k]==incSeqEvtBrs[j]&&(vMidxOfHead2[k]==vIdxOfHead2[k]||vMidxOfHead2[k]==vIdxOfHead2[(unsigned int) vVIdx2ToIdx1[i][(unsigned int) vMidx1[j]]]))
                        {
                          vVIdx2ToIdx1[i].push_back(k);
                          break;
                        }
                    }
                  else
                    {
                      if(isLiaMatchedWithLib(incSeqEvtBrs[j],evtTr[k])&&(vMidxOfHead2[k]==vIdxOfHead2[k]||vMidxOfHead2[k]==vIdxOfHead2[(unsigned int) vVIdx2ToIdx1[i][(unsigned int) vMidx1[j]]]))
                        {
                          vVIdx2ToIdx1[i].push_back(k);
                          break;
                        }
                    }
                }
            }
          if(vVIdx2ToIdx1[i].size()==incSeqEvtBrs.size()) nCount++;
        }
    }

  return nCount;
}
