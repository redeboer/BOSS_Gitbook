#include "../include/topoana.h"
#include <cstdlib>

void  topoana::getEvtIFSts(vector<int> & vPid,vector<int> & vMidx,list<int> & evtIFSts)
{
  evtIFSts.clear();
  bool fsp;
  for(unsigned int i=0;i<vPid.size();i++)
    {
      fsp=true;
      for(unsigned int j=i+1;j<vPid.size();j++)
        {
          if(((unsigned int) vMidx[j])==i)
            {
              fsp=false;
              break;
            }
        }
      if(fsp&&abs(vPid[i])!=1&&abs(vPid[i])!=2&&abs(vPid[i])!=3&&abs(vPid[i])!=4&&abs(vPid[i])!=5&&abs(vPid[i])!=6) evtIFSts.push_back(vPid[i]);
    }
  sortByPidAndPchrg(evtIFSts);
  evtIFSts.push_front(11);
  evtIFSts.push_front(-11);
}
