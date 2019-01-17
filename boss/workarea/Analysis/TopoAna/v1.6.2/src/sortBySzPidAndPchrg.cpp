#include "../include/topoana.h"
#include <iostream>
#include <cstdlib>

void topoana::sortBySzPidAndPchrg(vector< vector<int> > &vVia,vector< vector<int> > &vVib,vector< vector<int> >&vVic,vector< vector<int> > &vVid)
{
  if(vVia.size()!=vVib.size()||vVib.size()!=vVic.size()||vVic.size()!=vVid.size())
    { 
      cerr<<"Error: The four vectors have different sizes!"<<endl;
      cerr<<"Infor: The size of the first vector is "<<vVia.size()<<"."<<endl;
      cerr<<"Infor: The size of the second vector is "<<vVib.size()<<"."<<endl;
      cerr<<"Infor: The size of the third vector is "<<vVic.size()<<"."<<endl;
      cerr<<"Infor: The size of the fourth vector is "<<vVid.size()<<"."<<endl;
      cerr<<"Infor: Please check them."<<endl;
      exit(-1);
    }
  if(vVia.size()==0)
    { 
      cerr<<"Infor: The sizes of the four vectors are zero!"<<endl<<endl;
      return;
    }

  /*cout<<"Before sort:"<<endl;
  for(unsigned int i=0;i<vVia.size();i++)
    for(unsigned int j=0;j<vVia[i].size();j++)
      {
        if(j<(vVia[i].size()-1)) cout<<vVia[i][j]<<" ";
        else cout<<vVia[i][j]<<endl; 
      }*/

  bool exchOrd;
  vector<int> viaTmp,vibTmp,vicTmp,vidTmp;
  int ibTmp;
  for(unsigned int i=0;i<(vVia.size()-1);i++)
    for(unsigned int j=i+1;j<vVia.size();j++)
      {
        exchOrd=false;
        if(vVia[i].size()>vVia[j].size()) exchOrd=true;
        else if(vVia[i].size()==vVia[j].size())
          {
            for(unsigned int k=0;k<vVia[i].size();k++)
              {
                if(abs(vVia[i][k])>abs(vVia[j][k]))
                  {
                    if(abs(vVia[j][k])!=22)
                      {                    
                        exchOrd=true;
                        break;
                      }
                    else
                      {
                        break;
                      }
                  }
                else if(abs(vVia[i][k])==abs(vVia[j][k]))
                  {
                    if(m_pid3PchrgMap.find(vVia[i][k])!=m_pid3PchrgMap.end()&&m_pid3PchrgMap.find(vVia[j][k])!=m_pid3PchrgMap.end())
                      {              
                        if(m_pid3PchrgMap[vVia[i][k]]<m_pid3PchrgMap[vVia[j][k]])
                          {
                            exchOrd=true;        
                            break;
                          }
                        else if(m_pid3PchrgMap[vVia[i][k]]>m_pid3PchrgMap[vVia[j][k]])
                          {
                            break;
                          }
                        else
                          {
                            if(vVia[i][k]<vVia[j][k])
                              {
                                exchOrd=true;
                                break;
                              }
                            else if(vVia[i][k]>vVia[j][k])
                              {
                                break;
                              }
                          }                        
                      }
                    else
                      {
                        if(m_pid3PchrgMap.find(vVia[i][k])==m_pid3PchrgMap.end()) writeErrInfOnPid3PchrgMap(vVia[i][k]);
                        if(m_pid3PchrgMap.find(vVia[j][k])==m_pid3PchrgMap.end()) writeErrInfOnPid3PchrgMap(vVia[j][k]);
                      }                    
                  }
                else // Please don't forget this part of the if statement.
                  { 
                    if(abs(vVia[i][k])==22)
                      {
                        exchOrd=true;
                        break;
                      }
                    else
                      {
                        break;
                      }
                  }
              }
          }
        if(exchOrd==true)
          {
            viaTmp=vVia[i];
            vVia[i]=vVia[j];
            vVia[j]=viaTmp;

            ibTmp=vVib[i][0];
            for(unsigned int k=0;k<vVib[i].size();k++) vVib[i][k]=vVib[j][0];
            for(unsigned int k=0;k<vVib[j].size();k++) vVib[j][k]=ibTmp;

            vibTmp=vVib[i];
            vVib[i]=vVib[j];
            vVib[j]=vibTmp;

            vicTmp=vVic[i];
            vVic[i]=vVic[j];
            vVic[j]=vicTmp;

            vidTmp=vVid[i];
            vVid[i]=vVid[j];
            vVid[j]=vidTmp;             
          }        
      }
  
  /*cout<<"After sort:"<<endl;
  for(unsigned int i=0;i<vVia.size();i++)
    for(unsigned int j=0;j<vVia[i].size();j++)
      { 
        if(j<(vVia[i].size()-1)) cout<<vVia[i][j]<<" ";
        else cout<<vVia[i][j]<<endl;
      }*/
}
