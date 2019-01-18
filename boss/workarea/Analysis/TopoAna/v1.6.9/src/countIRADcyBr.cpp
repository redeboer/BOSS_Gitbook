#include "../include/topoana.h"
#include <cstdlib>
#include <iostream>

// The fourth argument is appended for the special usage in the "countIncOrIRASeqDcyBrsInDcyTr" method. 
unsigned int topoana::countIRADcyBr(vector<int> & vPid, vector<int> & vMidx, list<int> IRADcyBr, bool areHeadsRequiredToBeMatched)
{
  vector<int> vIdx;
  vIdx.clear();
  list<int>::iterator liit=IRADcyBr.begin();
  if(areHeadsRequiredToBeMatched==true)
    {
      if(vPid.size()>0&&vPid[0]==(*liit)) vIdx.push_back(0);
    }
  else
    {
      for(unsigned int i=0;i<vPid.size();i++)
        {
          if(vPid[i]==(*liit)) vIdx.push_back(i);
        }
    }
  unsigned int nCount=0;
  list<int> IRADcyBrTmp;
  for(unsigned int i=0;i<vIdx.size();i++)
    {
      IRADcyBrTmp.clear();
      for(unsigned int j=vIdx[i]+1;j<vPid.size();j++)
        {
          bool specifiedP=false;
          for(liit++;liit!=IRADcyBr.end();liit++)
            {
              if(vPid[j]==(*liit))
                {
                  specifiedP=true;
                  break;
                }
            }
          liit=IRADcyBr.begin();
          bool fsp=true;
          if(specifiedP==false)
            {
              for(unsigned int k=j+1;k<vPid.size();k++)
                {
                  if(((unsigned int) vMidx[k])==j)
                    {
                      fsp=false;
                      break;
                    }
                }
            }
          bool descendantOfOneSpecifiedP=false;
          for(liit++;liit!=IRADcyBr.end();liit++)
            {
              int k=j;
              while(1)
                {                     
                  if(vPid[vMidx[k]]==(*liit))
                    {
                      descendantOfOneSpecifiedP=true;
                      break;
                    }
                  else if(vMidx[k]==k||vMidx[k]==-1)
                    {
                      break;
                    }
                  else
                    {
                      k=vMidx[k];
                    }
                }
              if(descendantOfOneSpecifiedP==true) break;
            }
          liit=IRADcyBr.begin();
          if((specifiedP==true||fsp==true)&&(descendantOfOneSpecifiedP==false))
            {
              bool fromTheP=false;
              int k=j;
              while(1)
                {
                  if(vMidx[k]==vIdx[i])
                    {
                      fromTheP=true;
                      break;
                    }
                  else if(vMidx[k]==k||vMidx[k]==-1)
                    {
                      break;
                    }
                  else
                    {
                      k=vMidx[k];
                    }
                }
              // The following condition "abs(vPid[j])==22" is exerted specially for the comparison without final state photons.
              if(fromTheP==true)
                {
                  // If the gFSR photons are ignored, then they don't show themselves up here. If they are not ignored, then their PDG codes are modified to be -22 or m_pidOfGFSRGam in the "sortPs" function.
                  if(m_ignoreGFSR==true) IRADcyBrTmp.push_back(vPid[j]);
                  else if(abs(vPid[j])!=-22&&vPid[j]!=m_pidOfGFSRGam) IRADcyBrTmp.push_back(vPid[j]);
                }
            } 
        }
     if(IRADcyBrTmp.size()>0)
       {
         sortByPidAndPchrg(IRADcyBrTmp);
         liit=IRADcyBr.begin();
         IRADcyBrTmp.push_front((*liit));
         if(IRADcyBrTmp==IRADcyBr) nCount++;
       }
    }
  return nCount;
}
