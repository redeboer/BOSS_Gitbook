#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;
int main()
{
  int pid,_3pchrg;
  string txtPnm,texPnm;
  vector<int> vPid,v3Pchrg;
  vector<string> vTxtPnm,vTexPnm;
  ifstream fin1("pid_3pchrg_txtpnm.dat_sorted",ios::in);
  while(fin1>>pid>>_3pchrg>>txtPnm)
  {
    vPid.push_back(pid);
    v3Pchrg.push_back(_3pchrg);
    vTxtPnm.push_back(txtPnm);
    //cout<<setiosflags(ios::right)<<setw(16)<<pid<<setw(8)<<_3pchrg<<resetiosflags(ios::right)<<"\t"<<setiosflags(ios::left)<<setw(24)<<txtPnm<<endl;
  }

  int pid_b2,_3pchrg_b2;
  string txtPnm_b2,texPnm_b2;
  vector<int> vPid_b2,v3Pchrg_b2;
  vector<string> vTxtPnm_b2,vTexPnm_b2;
  ifstream fin2("pid_3pchrg_txtpnm_texpnm.dat_b2",ios::in);
  while(fin2>>pid_b2>>_3pchrg_b2>>txtPnm_b2>>texPnm_b2)
  {
    vPid_b2.push_back(pid_b2);
    v3Pchrg_b2.push_back(_3pchrg_b2);
    vTxtPnm_b2.push_back(txtPnm_b2);
    vTexPnm_b2.push_back(texPnm_b2);
    //cout<<setiosflags(ios::right)<<setw(16)<<pid_b2<<setw(8)<<_3pchrg_b2<<resetiosflags(ios::right)<<"\t"<<setiosflags(ios::left)<<setw(24)<<txtPnm_b2<<setw(32)<<texPnm_b2<<endl;
  }

  bool lsFound;
  for(unsigned int i=0;i<vPid.size();i++)
  {
    lsFound=false;
    for(unsigned int j=0;j<vPid_b2.size();j++)
    {
      if(vPid_b2[j]==vPid[i]&&v3Pchrg_b2[j]==v3Pchrg[i]&&vTxtPnm_b2[j]==vTxtPnm[i])
      {
        vTexPnm.push_back(vTexPnm_b2[j]);
        lsFound=true;
        break;
      }
      else if(vPid_b2[j]==vPid[i]&&v3Pchrg_b2[j]==v3Pchrg[i])
      {
        cout<<setiosflags(ios::right)<<setw(16)<<vPid[i]<<setw(8)<<v3Pchrg[i]<<resetiosflags(ios::right)<<"\t"<<setiosflags(ios::left)<<setw(24)<<vTxtPnm[i]<<endl;
        cout<<setiosflags(ios::right)<<setw(16)<<vPid_b2[j]<<setw(8)<<v3Pchrg_b2[j]<<resetiosflags(ios::right)<<"\t"<<setiosflags(ios::left)<<setw(24)<<vTxtPnm_b2[j]<<setw(32)<<vTexPnm_b2[j]<<endl;
        cout<<endl;
      }
    }
    if(lsFound==false) vTexPnm.push_back("??");
  }

  ofstream fout("pid_3pchrg_txtpnm_texpnm.dat_auto",ios::out);
  for(unsigned int i=0;i<vPid.size();i++)
  {
    fout<<setiosflags(ios::right)<<setw(16)<<vPid[i]<<setw(8)<<v3Pchrg[i]<<resetiosflags(ios::right)<<"\t"<<setiosflags(ios::left)<<setw(24)<<vTxtPnm[i]<<setw(32)<<vTexPnm[i]<<endl;
  }
  return 0;
}
