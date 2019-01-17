#include "../include/topoana.h"
#include <iostream>

unsigned int topoana::countSeqEvtBrsInEvtTr(vector< list<int> > & seqEvtBrs, vector<int> vIdxOfHead1, vector<int> vMidxOfHead1, vector< list<int> > & evtTr, vector<int> vIdxOfHead2, vector<int> vMidxOfHead2)
{
  unsigned int nCount=0;
  if(seqEvtBrs.size()==0)
    {
      cerr<<"Infor: The size of the vector for the sequential event branches is zero!"<<endl<<endl;
      return nCount;
    }
  if(evtTr.size()==0)
    {
      cerr<<"Infor: The size of the vector for the event tree is zero!"<<endl<<endl;
      return nCount;
    }
  if(seqEvtBrs.size()<=evtTr.size())
    {
      /*for(unsigned int i=0;i<seqEvtBrs.size();i++)
        {
          list<int>::iterator liit,liitTmp=seqEvtBrs[i].end();liitTmp--;
          for(liit=seqEvtBrs[i].begin();liit!=seqEvtBrs[i].end();liit++)
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
      for(unsigned int i=0;i<evtTr.size();i++)
        {
          if(evtTr[i]==seqEvtBrs[0])
            {
              vIdx2ToIdx1.clear();
              vIdx2ToIdx1.push_back(i);
              vVIdx2ToIdx1.push_back(vIdx2ToIdx1);
            }
        }

      vector<int> vMidx1;
      vMidx1.push_back(0);
      for(unsigned int i=1;i<seqEvtBrs.size();i++)
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
          for(unsigned int j=1;j<seqEvtBrs.size();j++)
            {
              for(unsigned int k=vVIdx2ToIdx1[i][j-1]+1;k<evtTr.size();k++)
                {
                  if(evtTr[k]==seqEvtBrs[j]&&(vMidxOfHead2[k]==vIdxOfHead2[k]||vMidxOfHead2[k]==vIdxOfHead2[(unsigned int) vVIdx2ToIdx1[i][(unsigned int) vMidx1[j]]]))
                    {
                      vVIdx2ToIdx1[i].push_back(k);
                      break;
                    }
                }
            }
          if(vVIdx2ToIdx1[i].size()==seqEvtBrs.size()) nCount++;
        }
    }

  return nCount;
}
