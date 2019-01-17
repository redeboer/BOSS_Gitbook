#include "../include/topoana.h"
#include <cstdlib>

void topoana::getEvtTr(vector<int> vPid, vector<int> vMidx, vector< list<int> > & evtTr)
{
  evtTr.clear();
  list<int> evtBr;
  evtBr.clear();
  for(unsigned int i=0;i<vPid.size();i++)
    {
      if(((unsigned int) vMidx[i])==i) evtBr.push_back(vPid[i]);
    }
  evtBr.push_front(11);
  evtBr.push_front(-11);
  evtTr.push_back(evtBr);
  evtBr.clear();
  for(unsigned int i=0;i<vPid.size();i++)
    {
      for(unsigned int j=i+1;j<vPid.size();j++)
        {
          if(((unsigned int) vMidx[j])==i) evtBr.push_back(vPid[j]); 
        }
      if(evtBr.size()==0) continue; 
      if(abs(vPid[i])!=1&&abs(vPid[i])!=2&&abs(vPid[i])!=3&&abs(vPid[i])!=4&&abs(vPid[i])!=5&&abs(vPid[i])!=6)
        {
          evtBr.push_front(vPid[i]);
        }
      else
        {
          evtBr.push_front(-abs(vPid[i]));
          evtBr.push_front(abs(vPid[i]));    
        }
      evtTr.push_back(evtBr);
      evtBr.clear();
    }
}

void topoana::getEvtTr(vector<int> vPid, vector<int> vMidx, vector< list<int> > & evtTr, vector <int> & vIdxOfHead, vector<int> & vMidxOfHead)
{
  evtTr.clear();
  vIdxOfHead.clear();
  vMidxOfHead.clear();
  list<int> evtBr;
  evtBr.clear();
  for(unsigned int i=0;i<vPid.size();i++)
    {
      if(((unsigned int) vMidx[i])==i) evtBr.push_back(vPid[i]);
    }
  evtBr.push_front(11);
  evtBr.push_front(-11);
  evtTr.push_back(evtBr);
  vIdxOfHead.push_back(-1);
  vMidxOfHead.push_back(-1);
  evtBr.clear();
  for(unsigned int i=0;i<vPid.size();i++)
    {
      for(unsigned int j=i+1;j<vPid.size();j++)
        {
          if(((unsigned int) vMidx[j])==i) evtBr.push_back(vPid[j]); 
        }
      if(evtBr.size()==0) continue; 
      if(abs(vPid[i])!=1&&abs(vPid[i])!=2&&abs(vPid[i])!=3&&abs(vPid[i])!=4&&abs(vPid[i])!=5&&abs(vPid[i])!=6)
        {
          evtBr.push_front(vPid[i]);
        }
      else
        {
          evtBr.push_front(-abs(vPid[i]));
          evtBr.push_front(abs(vPid[i]));    
        }
      evtTr.push_back(evtBr);
      vIdxOfHead.push_back(i);
      vMidxOfHead.push_back(vMidx[i]);
      evtBr.clear();
    }
}
