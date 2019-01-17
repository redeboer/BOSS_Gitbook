#ifndef TOPOANA_H
#define TOPOANA_H

#include <vector>
#include <string>
#include <list>
#include <map>
#include <climits>
using namespace std;

class topoana
{
  private:
    string m_pkgPath;
    string m_verNum;

    map<int,int> m_pid3PchrgMap;
    map<int,string> m_pidTxtPnmMap;
    map<int,string> m_pidTexPnmMap;
    map<int,int> m_pidIccPMap;

    vector<string> m_nmsOfIptRootFls;
    string m_trNm;
    string m_brNmOfNps;
    string m_brNmOfPid;
    string m_brNmOfMidx;
    string m_mainNmOfOptFls;
    unsigned long m_nEvtsMax;
    string m_cut;
    bool m_cutMethod;
    bool m_ccSwitch;
    bool m_ignoreISR;
    bool m_ignoreGFSR;
    unsigned int m_hHdEvtBrsMax;
    vector<int> m_vIdPid;
    vector<int> m_vIddPid;
    unsigned long m_nEvtTrsMax;
    unsigned long m_nEvtIFStsMax;
    bool m_spr1stBrsOfEvtTrs;
    bool m_sprTopoTags;
    bool m_sttaEvtTrsAndEvtIFSts;
    vector<int> m_vPttaPid;
    vector<unsigned long> m_vNEvtBrsMax;
    vector< vector<int> > m_vVPttaPid1;
    vector<unsigned long> m_vNEvtBrsMax1;
    vector<int> m_vSigPid;
    vector< vector<int> > m_vVSigPid1;
    vector< vector<int> > m_vVSigPid2;
    vector< vector<int> > m_vVSigMidx2;
    vector< vector<int> > m_vVSigPid12;
    vector< vector<int> > m_vVSigMidx12;
    vector< vector<int> > m_vVSigPid3;
    vector< vector<int> > m_vVSigPid4;
    vector< vector<int> > m_vVSigMidx4;
    vector< vector<int> > m_vVSigPid5;

    vector<int> m_vIdCcPid;
    vector<int> m_vIdICcPid;
    vector<int> m_vIddCcPid;
    vector<int> m_vIddICcPid;
    vector<int> m_vPttaCcPid;
    vector<int> m_vPttaICcPid;
    // Pay attention to that m_vVEvtBrCcP, m_vVIEvtBrCcP and m_vVNEvtBrCcP equate to m_vVCcEvtBrP, m_vVICcEvtBrP and m_vVNCcEvtBrP for self-charge-conjugate particles.
    vector< vector< list<int> > > m_vVEvtBrP, m_vVEvtBrCcP;
    vector< vector<int> > m_vVIEvtBrP, m_vVIEvtBrCcP;
    vector< vector<int> > m_vVNEvtBrP, m_vVNEvtBrCcP;
    vector< list<int> > m_vPttaIncEvtBr,m_vPttaCcIncEvtBr;
    vector<int> m_vPttaICcIncEvtBr;
    // Pay attention to that m_vVEvtBrCcIncEvtBr, m_vVIEvtBrCcIncEvtBr and m_vVNEvtBrCcIncEvtBr equate to m_vVCcEvtBrIncEvtBr, m_vVICcEvtBrIncEvtBr and m_vVNCcEvtBrIncEvtBr for self-charge-conjugate inclusive decays.
    vector< vector< list<int> > > m_vVEvtBrIncEvtBr, m_vVEvtBrCcIncEvtBr;
    vector< vector<int> > m_vVIEvtBrIncEvtBr, m_vVIEvtBrCcIncEvtBr;
    vector< vector<int> > m_vVNEvtBrIncEvtBr, m_vVNEvtBrCcIncEvtBr;
    vector<int> m_vCcSigPid;
    vector<int> m_vISigP, m_vICcSigP;              
    vector<int> m_vNSigP, m_vNCcSigP;
    vector< list<int> > m_vSigIncEvtBr, m_vCcSigIncEvtBr;
    vector<int> m_vISigIncEvtBr, m_vICcSigIncEvtBr;
    vector<int> m_vNSigIncEvtBr, m_vNCcSigIncEvtBr;
    vector< vector< list<int> > > m_vSigSeqEvtBrs, m_vCcSigSeqEvtBrs;
    vector<int> m_vISigSeqEvtBrs, m_vICcSigSeqEvtBrs;
    vector<int> m_vNSigSeqEvtBrs, m_vNCcSigSeqEvtBrs;
    vector< vector<int> > m_vVSigSeqIdxOfHead, m_vVCcSigSeqIdxOfHead;
    vector< vector<int> > m_vVSigSeqMidxOfHead, m_vVCcSigSeqMidxOfHead;
    vector< vector< list<int> > > m_vSigIncSeqEvtBrs, m_vCcSigIncSeqEvtBrs;
    vector<int> m_vISigIncSeqEvtBrs, m_vICcSigIncSeqEvtBrs;
    vector<int> m_vNSigIncSeqEvtBrs, m_vNCcSigIncSeqEvtBrs;
    vector< vector<int> > m_vVSigIncSeqIdxOfHead, m_vVCcSigIncSeqIdxOfHead;
    vector< vector<int> > m_vVSigIncSeqMidxOfHead, m_vVCcSigIncSeqMidxOfHead;
    vector< vector<int> > m_vVIIncSigIncSeqEvtBrs;
    vector< list<int> > m_vSigPFSts, m_vCcSigPFSts;
    vector<int> m_vISigPFSts, m_vICcSigPFSts;
    vector<int> m_vNSigPFSts, m_vNCcSigPFSts;
    vector< vector< list<int> > > m_vSigEvtTr, m_vCcSigEvtTr;
    vector<int> m_vISigEvtTr, m_vICcSigEvtTr;
    vector<int> m_vNSigEvtTr, m_vNCcSigEvtTr;
    vector< list<int> > m_vSigEvtIFSts, m_vCcSigEvtIFSts;
    vector<int> m_vISigEvtIFSts, m_vICcSigEvtIFSts;
    vector<int> m_vNSigEvtIFSts, m_vNCcSigEvtIFSts;
    map<int,int> m_iSigEvtTrISigEvtIFStsMap, m_iSigEvtTrICcSigEvtIFStsMap;
    vector< list<int> > m_vSigEvtIFSts2, m_vCcSigEvtIFSts2;
    vector<int> m_vISigEvtIFSts2, m_vICcSigEvtIFSts2;
    vector<int> m_vNSigEvtIFSts2, m_vNCcSigEvtIFSts2;
    vector< vector< list<int> > > m_vEvtTr, m_vCcEvtTr;
    vector<int> m_vIEvtTr, m_vICcEvtTr;
    vector<int> m_vNEvtTr, m_vNCcEvtTr;
    vector< list<int> > m_vEvtIFSts, m_vCcEvtIFSts;
    vector<int> m_vIEvtIFSts, m_vICcEvtIFSts;
    vector<int> m_vNEvtIFSts, m_vNCcEvtIFSts;
    map<int,int> m_iEvtTrIEvtIFStsMap, m_iEvtTrICcEvtIFStsMap;
    map<int,int> m_iSigEvtTrIEvtTrMap, m_iSigEvtTrICcEvtTrMap;
    map<int,int> m_iSigEvtIFStsIEvtIFStsMap, m_iSigEvtIFStsICcEvtIFStsMap;
    map<int,int> m_iSigEvtIFSts2IEvtIFStsMap, m_iSigEvtIFSts2ICcEvtIFStsMap;
    int m_pidOfISRGam;
    int m_pidOfgFSRGam;
    int m_pidOfISt;
    int m_pidOfAnything;
  public:
    topoana()
    {
      m_pkgPath="/besfs/users/zhouxy/tools/topoana/v1.6.2/";
      m_verNum="v1.6.2";

      m_nmsOfIptRootFls.clear();
      m_nEvtsMax=ULONG_MAX; // ULONG_MAX=9223372036854775807 is the maximum unsigned long int number. 
      m_cutMethod=false;
      m_ccSwitch=false;
      m_ignoreISR=true;
      m_ignoreGFSR=true;
      m_hHdEvtBrsMax=UINT_MAX;
      m_nEvtTrsMax=ULONG_MAX;
      m_nEvtIFStsMax=ULONG_MAX;
      m_spr1stBrsOfEvtTrs=false;
      m_sprTopoTags=false;
      m_sttaEvtTrsAndEvtIFSts=false;

      m_vIdPid.clear();
      m_vIddPid.clear();
      m_vPttaPid.clear();
      m_vNEvtBrsMax.clear();
      m_vVPttaPid1.clear();
      m_vNEvtBrsMax1.clear();
      m_vSigPid.clear();
      m_vVSigPid1.clear();
      m_vVSigPid2.clear();
      m_vVSigMidx2.clear();
      m_vVSigPid12.clear();
      m_vVSigMidx12.clear();
      m_vVSigPid3.clear();
      m_vVSigPid4.clear();
      m_vVSigMidx4.clear();
      m_vVSigPid5.clear();

      m_pidOfISRGam=22222222;
      m_pidOfgFSRGam=-22222222;
      m_pidOfISt=99999999;
      m_pidOfAnything=-99999999;
    }
    ~topoana(){}
    string &       trim(string & line);
    void           makeMapsOnPdata();
    int 	   getPidFromTxtPnm(string pnm);
    void           writeErrInfOnPid3PchrgMap(int pid);
    void           writePnmFromPid(ostream & out,string pnmType,int pid);
    void           sortByPidAndPchrg(vector<int> &via,vector<int> &vib,vector<int> &vic,vector<int> &vid);
    void           sortBySzPidAndPchrg(vector< vector<int> > &vVia,vector< vector<int> > &vVib,vector< vector<int> >&vVic,vector< vector<int> > &vVid);
    void           sortPs(vector<int> & vPid, vector<int> & vMidx);
    int		   getCcPid(int pid);
    void           getEvtTr(vector<int> vPid,vector<int> vMidx,vector< list<int> > & evtTr);
    void           getEvtTr(vector<int> vPid,vector<int> vMidx,vector< list<int> > & evtTr, vector<int> & vIdxOfHead, vector<int> & vMidxOfHead);
    void           sortByPidAndPchrg(list<int> &lia);
    void           getEvtIFSts(vector<int> & vPid,vector<int> & vMidx,list<int> & evtIFSts);
    unsigned int   countPFSts(vector<int> & vPid, vector<int> & vMidx, list<int> pFSts);
    void           readOpenCurly(ifstream & fin, string & line, string prompt);
    void           read1stLineOrCloseCurly(ifstream & fin, string & line, bool essential, string errinforprompt);
    void           readExtraLinesOrCloseCurly(ifstream & fin, string & line, string prompt);
    void           readCloseCurly(ifstream & fin, string & line, string prompt);
    void           readCard(string topoAnaCardFlNm);
    void	   checkInput();
    unsigned int   countIncLiaInVlib(list<int> & lia, vector< list<int> > & Vlib);
    bool	   isLiaMatchedWithLib(list<int> & lia, list<int> & lib);
    unsigned int   countSeqEvtBrsInEvtTr(vector< list<int> > & seqEvtBrs, vector<int> vIdxOfHead1, vector<int> vMidxOfHead1, vector< list<int> > & evtTr, vector<int> vIdxOfHead2, vector<int> vMidxOfHead2);
    unsigned int   countIncSeqEvtBrsInEvtTr(vector<int> vIIncIncSeqEvtBrs, vector< list<int> > & incSeqEvtBrs, vector<int> vIdxOfHead1, vector<int> vMidxOfHead1, vector< list<int> > & evtTr, vector<int> vIdxOfHead2, vector<int> vMidxOfHead2);
    void           sortBySumOf1stAnd2ndFromLrgToSml(vector<int> &via1,vector<int> &via2,vector< vector< list<int> > > &vVLib1,vector< vector< list<int> > > &vVLib2,vector<int> &vic1,vector<int> &vic2);
    void           sortBySumOf1stAnd2ndFromLrgToSml(vector<int> &via1,vector<int> &via2,vector< vector< list<int> > > &vVLib1,vector< vector< list<int> > > &vVLib2,vector<int> &vic1,vector<int> &vic2,vector< vector<int> > &vid1,vector< vector<int> > &vid2,vector< vector<int> > &vie1,vector< vector<int> > &vie2);
    void           sortBySumOf1stAnd2ndFromLrgToSml(vector<int> &via1,vector<int> &via2,vector< list<int> > &vLib1,vector< list<int> > &vLib2,vector<int> &vic1,vector<int> &vic2);
    void           sortBySumOf1stAnd2ndFromLrgToSml(vector<int> &via1,vector<int> &via2,vector<int> &vib1,vector<int> &vib2,vector<int> &vic1,vector<int> &vic2);
    void           sortBy1stFromLrgToSml(vector<int> &via,vector< vector< list<int> > > &vVLib,vector<int> &vic);
    void           sortBy1stFromLrgToSml(vector<int> &via,vector< vector< list<int> > > &vVLib,vector<int> &vic,vector< vector<int> > &vVid,vector< vector<int> > &vVie);
    void           sortBy1stFromLrgToSml(vector<int> &via,vector< list<int> > &vLib,vector<int> &vic);
    void           sortBy1stFromLrgToSml(vector<int> &via,vector<int> &vib,vector<int> &vic);
    void           getRslt();
    void           writeRsltIntoTxtFl();
    void           writeRsltIntoTexFl();
    void           getPdfFlFromTexFl();
    void           writeInfOnRslt();
};

#endif
