#include "../include/topoana.h"
#include <cstdlib>

unsigned int topoana::countPFSts(vector<int> & vPid,vector<int> & vMidx, list<int> pFSts)
{
  vector<int> vIdx;
  vIdx.clear();
  list<int>::iterator liit=pFSts.begin();
  for(unsigned int i=0;i<vPid.size();i++)
    {
      if(vPid[i]==(*liit)) vIdx.push_back(i);
    }

  unsigned int nCount=0;
  list<int> pFStsTmp;
  for(unsigned int i=0;i<vIdx.size();i++)
    {
      pFStsTmp.clear();
      for(unsigned int j=vIdx[i]+1;j<vPid.size();j++)
        {
          bool specifiedP=false;
          for(liit++;liit!=pFSts.end();liit++)
            {
              if(vPid[j]==(*liit))
                {
                  specifiedP=true;
                  break;
                }
            }
          liit=pFSts.begin();
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
          for(liit++;liit!=pFSts.end();liit++)
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
          liit=pFSts.begin();
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
                  // If the gFSR photons are ignored, then they don't show themselves up here. If they are not ignored, then their PDG codes are modified to be -22 or m_pidOfgFSRGam in the "sortPs" function.
                  if(m_ignoreGFSR==true) pFStsTmp.push_back(vPid[j]);
                  else if(abs(vPid[j])!=-22&&vPid[j]!=m_pidOfgFSRGam) pFStsTmp.push_back(vPid[j]);
                }
            } 
        }
     if(pFStsTmp.size()>0)
       {
         sortByPidAndPchrg(pFStsTmp);
         liit=pFSts.begin();
         pFStsTmp.push_front((*liit));
         if(pFStsTmp==pFSts) nCount++;
       }
    }
  return nCount;
}
