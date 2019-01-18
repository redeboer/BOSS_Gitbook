#include "../include/topoana.h"
#include <cstdlib>

void topoana::getDcyTr(vector<int> vPid, vector<int> vMidx, vector< list<int> > & dcyTr)
{
  dcyTr.clear();
  list<int> dcyBr;
  dcyBr.clear();
  for(unsigned int i=0;i<vPid.size();i++)
    {
      if(((unsigned int) vMidx[i])==i) dcyBr.push_back(vPid[i]);
    }
  dcyBr.push_front(11);
  dcyBr.push_front(-11);
  dcyTr.push_back(dcyBr);
  dcyBr.clear();
  for(unsigned int i=0;i<vPid.size();i++)
    {
      for(unsigned int j=i+1;j<vPid.size();j++)
        {
          if(((unsigned int) vMidx[j])==i) dcyBr.push_back(vPid[j]); 
        }
      if(dcyBr.size()==0) continue; 
      if(abs(vPid[i])!=1&&abs(vPid[i])!=2&&abs(vPid[i])!=3&&abs(vPid[i])!=4&&abs(vPid[i])!=5&&abs(vPid[i])!=6)
        {
          dcyBr.push_front(vPid[i]);
        }
      else
        {
          dcyBr.push_front(-abs(vPid[i]));
          dcyBr.push_front(abs(vPid[i]));    
        }
      dcyTr.push_back(dcyBr);
      dcyBr.clear();
    }
}

void topoana::getDcyTr(vector<int> vPid, vector<int> vMidx, vector< list<int> > & dcyTr, vector <int> & vIdxOfHead, vector<int> & vMidxOfHead)
{
  dcyTr.clear();
  vIdxOfHead.clear();
  vMidxOfHead.clear();
  list<int> dcyBr;
  dcyBr.clear();
  for(unsigned int i=0;i<vPid.size();i++)
    {
      if((((unsigned int) vMidx[i])==i)&&(vPid[i]!=m_pidOfAnything)&&(vPid[i]!=m_pidOfIRAFlag)) dcyBr.push_back(vPid[i]);
    }
  dcyBr.push_front(11);
  dcyBr.push_front(-11);
  dcyTr.push_back(dcyBr);
  vIdxOfHead.push_back(-1);
  vMidxOfHead.push_back(-1);
  dcyBr.clear();
  for(unsigned int i=0;i<vPid.size();i++)
    {
      for(unsigned int j=i+1;j<vPid.size();j++)
        {
          if((((unsigned int) vMidx[j])==i)&&(vPid[j]!=m_pidOfAnything)&&(vPid[j]!=m_pidOfIRAFlag)) dcyBr.push_back(vPid[j]); 
        }
      if(dcyBr.size()==0) continue; 
      if(abs(vPid[i])!=1&&abs(vPid[i])!=2&&abs(vPid[i])!=3&&abs(vPid[i])!=4&&abs(vPid[i])!=5&&abs(vPid[i])!=6)
        {
          dcyBr.push_front(vPid[i]);
        }
      else
        {
          dcyBr.push_front(-abs(vPid[i]));
          dcyBr.push_front(abs(vPid[i]));    
        }
      dcyTr.push_back(dcyBr);
      vIdxOfHead.push_back(i);
      vMidxOfHead.push_back(vMidx[i]);
      dcyBr.clear();
    }
}

void topoana::getDcyTr(vector<int> vPid, vector<int> vMidx, vector< list<int> > & dcyTr, vector <int> & vIdxOfHead, vector<int> & vMidxOfHead, vector<int> & vIIncIncSeqDcyBrs)
{
  dcyTr.clear();
  vIdxOfHead.clear();
  vMidxOfHead.clear();
  vIIncIncSeqDcyBrs.clear();
  list<int> dcyBr;
  dcyBr.clear();
  int iIncIncSeqDcyBrs=0;
  for(unsigned int i=0;i<vPid.size();i++)
    {
      if(((unsigned int) vMidx[i])==i)
        {
          if(vPid[i]==m_pidOfAnything) iIncIncSeqDcyBrs=1;
          else dcyBr.push_back(vPid[i]);
        }
    }
  dcyBr.push_front(11);
  dcyBr.push_front(-11);
  dcyTr.push_back(dcyBr);
  vIdxOfHead.push_back(-1);
  vMidxOfHead.push_back(-1);
  vIIncIncSeqDcyBrs.push_back(iIncIncSeqDcyBrs);
  dcyBr.clear();
  iIncIncSeqDcyBrs=0;
  for(unsigned int i=0;i<vPid.size();i++)
    {
      for(unsigned int j=i+1;j<vPid.size();j++)
        {
          if(((unsigned int) vMidx[j])==i)
            {
              if(vPid[j]==m_pidOfAnything) iIncIncSeqDcyBrs=1;
              else dcyBr.push_back(vPid[j]);
            }
        }
      if(dcyBr.size()==0) continue; 
      if(abs(vPid[i])!=1&&abs(vPid[i])!=2&&abs(vPid[i])!=3&&abs(vPid[i])!=4&&abs(vPid[i])!=5&&abs(vPid[i])!=6)
        {
          dcyBr.push_front(vPid[i]);
        }
      else
        {
          dcyBr.push_front(-abs(vPid[i]));
          dcyBr.push_front(abs(vPid[i]));    
        }
      dcyTr.push_back(dcyBr);
      vIdxOfHead.push_back(i);
      vMidxOfHead.push_back(vMidx[i]);
      vIIncIncSeqDcyBrs.push_back(iIncIncSeqDcyBrs);
      dcyBr.clear();
      iIncIncSeqDcyBrs=0;
    }
}

void topoana::getDcyTr(vector<int> vPid, vector<int> vMidx, vector< list<int> > & dcyTr, vector <int> & vIdxOfHead, vector<int> & vMidxOfHead, vector<int> & vIIncIncOrIRASeqDcyBrs, vector<int> & vIIRAIncOrIRASeqDcyBrs)
{
  dcyTr.clear();
  vIdxOfHead.clear();
  vMidxOfHead.clear();
  vIIncIncOrIRASeqDcyBrs.clear();
  vIIRAIncOrIRASeqDcyBrs.clear();
  list<int> dcyBr;
  dcyBr.clear();
  int iIncIncOrIRASeqDcyBrs=0;
  int iIRAIncOrIRASeqDcyBrs=0;
  for(unsigned int i=0;i<vPid.size();i++)
    {
      if(((unsigned int) vMidx[i])==i)
        {
          if(vPid[i]==m_pidOfAnything) iIncIncOrIRASeqDcyBrs=1;
          else if(vPid[i]==m_pidOfIRAFlag) iIRAIncOrIRASeqDcyBrs=1;
          else dcyBr.push_back(vPid[i]);
        }
    }
  dcyBr.push_front(11);
  dcyBr.push_front(-11);
  dcyTr.push_back(dcyBr);
  vIdxOfHead.push_back(-1);
  vMidxOfHead.push_back(-1);
  vIIncIncOrIRASeqDcyBrs.push_back(iIncIncOrIRASeqDcyBrs);
  vIIRAIncOrIRASeqDcyBrs.push_back(iIRAIncOrIRASeqDcyBrs);
  dcyBr.clear();
  iIncIncOrIRASeqDcyBrs=0;
  iIRAIncOrIRASeqDcyBrs=0;
  for(unsigned int i=0;i<vPid.size();i++)
    {
      for(unsigned int j=i+1;j<vPid.size();j++)
        {
          if(((unsigned int) vMidx[j])==i)
            {
              if(vPid[j]==m_pidOfAnything) iIncIncOrIRASeqDcyBrs=1;
              else if(vPid[j]==m_pidOfIRAFlag) iIRAIncOrIRASeqDcyBrs=1;
              else dcyBr.push_back(vPid[j]);
            }
        }
      if(dcyBr.size()==0) continue; 
      if(abs(vPid[i])!=1&&abs(vPid[i])!=2&&abs(vPid[i])!=3&&abs(vPid[i])!=4&&abs(vPid[i])!=5&&abs(vPid[i])!=6)
        {
          dcyBr.push_front(vPid[i]);
        }
      else
        {
          dcyBr.push_front(-abs(vPid[i]));
          dcyBr.push_front(abs(vPid[i]));    
        }
      dcyTr.push_back(dcyBr);
      vIdxOfHead.push_back(i);
      vMidxOfHead.push_back(vMidx[i]);
      vIIncIncOrIRASeqDcyBrs.push_back(iIncIncOrIRASeqDcyBrs);
      vIIRAIncOrIRASeqDcyBrs.push_back(iIRAIncOrIRASeqDcyBrs);
      dcyBr.clear();
      iIncIncOrIRASeqDcyBrs=0;
      iIRAIncOrIRASeqDcyBrs=0;
    }
}
