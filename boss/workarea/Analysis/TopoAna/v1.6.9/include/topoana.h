#ifndef TOPOANA_H
#define TOPOANA_H

#include <vector>
#include <string>
#include <list>
#include <map>
#include <climits>
#include <unordered_map>
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
    unsigned long m_nEtrsMax;
    string m_cut;
    bool m_cutMethod;
    bool m_ccSwitch;
    bool m_ignoreGISR;
    bool m_ignoreGFSR;
    unsigned int m_hHdDcyBrsMax;
    vector<int> m_vIdPid;
    vector<int> m_vIddPid;
    unsigned long m_nDcyTrsToBePrtdMax;
    unsigned long m_nDcyFStsToBePrtdMax;
    bool m_spr1stBrsOfDcyTrs;
    bool m_sprTopoTags;
    unsigned long m_nEtrsMaxInASngOptRootFl;
    unsigned long m_nDcyTrsToBeAnlzdMax;
    unsigned long m_nEtrsOfCtrSmpMax;
    bool m_sttaDcyTrsAndDcyFSts;
    vector<int> m_vPttaPid;
    vector<unsigned long> m_vNDcyBrsMax;
    vector< vector<int> > m_vVPttaPid1;
    vector<unsigned long> m_vNDcyBrsMax1;
    vector<int> m_vSigPid;
    vector< vector<int> > m_vVSigPid1;
    vector< vector<int> > m_vVSigPid2;
    vector< vector<int> > m_vVSigMidx2;
    vector< vector<int> > m_vVSigPid12;
    vector< vector<int> > m_vVSigMidx12;
    vector< vector<int> > m_vVSigPid3;
    vector< vector<int> > m_vVSigPid123;
    vector< vector<int> > m_vVSigMidx123;
    vector< vector<int> > m_vVSigPid4;
    vector< vector<int> > m_vVSigMidx4;
    vector< vector<int> > m_vVSigPid5;

    vector<int> m_vIdCcPid;
    vector<int> m_vIdICcPid;
    vector<int> m_vIddCcPid;
    vector<int> m_vIddICcPid;
    vector<int> m_vPttaCcPid;
    vector<int> m_vPttaICcPid;
    // Pay attention to that m_vVDcyBrCcP, m_vVIDcyBrCcP and m_vVNDcyBrCcP equate to m_vVCcDcyBrP, m_vVICcDcyBrP and m_vVNCcDcyBrP for self-charge-conjugate particles.
    vector< vector< list<int> > > m_vVDcyBrP, m_vVDcyBrCcP;
    vector< vector<int> > m_vVIDcyBrP, m_vVIDcyBrCcP;
    vector< vector<int> > m_vVNDcyBrP, m_vVNDcyBrCcP;
    vector< list<int> > m_vPttaIncDcyBr,m_vPttaCcIncDcyBr;
    vector<int> m_vPttaICcIncDcyBr;
    // Pay attention to that m_vVDcyBrCcIncDcyBr, m_vVIDcyBrCcIncDcyBr and m_vVNDcyBrCcIncDcyBr equate to m_vVCcDcyBrIncDcyBr, m_vVICcDcyBrIncDcyBr and m_vVNCcDcyBrIncDcyBr for self-charge-conjugate inclusive decays.
    vector< vector< list<int> > > m_vVDcyBrIncDcyBr, m_vVDcyBrCcIncDcyBr;
    vector< vector<int> > m_vVIDcyBrIncDcyBr, m_vVIDcyBrCcIncDcyBr;
    vector< vector<int> > m_vVNDcyBrIncDcyBr, m_vVNDcyBrCcIncDcyBr;
    vector<int> m_vCcSigPid;
    vector<int> m_vISigP, m_vICcSigP;              
    vector<int> m_vNSigP, m_vNCcSigP;
    vector< list<int> > m_vSigIncDcyBr, m_vCcSigIncDcyBr;
    vector<int> m_vISigIncDcyBr, m_vICcSigIncDcyBr;
    vector<int> m_vNSigIncDcyBr, m_vNCcSigIncDcyBr;
    vector< vector< list<int> > > m_vSigSeqDcyBrs, m_vCcSigSeqDcyBrs;
    vector< vector<int> > m_vVSigSeqDcyBrsIdxOfHead, m_vVCcSigSeqDcyBrsIdxOfHead;
    vector< vector<int> > m_vVSigSeqDcyBrsMidxOfHead, m_vVCcSigSeqDcyBrsMidxOfHead;
    vector<int> m_vISigSeqDcyBrs, m_vICcSigSeqDcyBrs;
    vector<int> m_vNSigSeqDcyBrs, m_vNCcSigSeqDcyBrs;
    vector< vector< list<int> > > m_vSigIncSeqDcyBrs, m_vCcSigIncSeqDcyBrs;
    vector< vector<int> > m_vVSigIncSeqDcyBrsIdxOfHead, m_vVCcSigIncSeqDcyBrsIdxOfHead;
    vector< vector<int> > m_vVSigIncSeqDcyBrsMidxOfHead, m_vVCcSigIncSeqDcyBrsMidxOfHead;
    vector< vector<int> > m_vVIIncSigIncSeqDcyBrs;
    vector<int> m_vISigIncSeqDcyBrs, m_vICcSigIncSeqDcyBrs;
    vector<int> m_vNSigIncSeqDcyBrs, m_vNCcSigIncSeqDcyBrs;
    vector< list<int> > m_vSigIRADcyBr, m_vCcSigIRADcyBr;
    vector<int> m_vISigIRADcyBr, m_vICcSigIRADcyBr;
    vector<int> m_vNSigIRADcyBr, m_vNCcSigIRADcyBr;
    vector< vector< list<int> > > m_vSigIncOrIRASeqDcyBrs, m_vCcSigIncOrIRASeqDcyBrs;
    vector< vector<int> > m_vVSigIncOrIRASeqDcyBrsIdxOfHead, m_vVCcSigIncOrIRASeqDcyBrsIdxOfHead;
    vector< vector<int> > m_vVSigIncOrIRASeqDcyBrsMidxOfHead, m_vVCcSigIncOrIRASeqDcyBrsMidxOfHead;
    vector< vector<int> > m_vVIIncSigIncOrIRASeqDcyBrs;
    vector< vector<int> > m_vVIIRASigIncOrIRASeqDcyBrs;
    vector<int> m_vISigIncOrIRASeqDcyBrs, m_vICcSigIncOrIRASeqDcyBrs;
    vector<int> m_vNSigIncOrIRASeqDcyBrs, m_vNCcSigIncOrIRASeqDcyBrs;
    vector< vector< list<int> > > m_vSigDcyTr, m_vCcSigDcyTr;
    vector<int> m_vISigDcyTr, m_vICcSigDcyTr;
    vector<int> m_vNSigDcyTr, m_vNCcSigDcyTr;
    vector< list<int> > m_vSigDcyFSt, m_vCcSigDcyFSt;
    vector<int> m_vISigDcyFSt, m_vICcSigDcyFSt;
    vector<int> m_vNSigDcyFSt, m_vNCcSigDcyFSt;
    map<int,int> m_iSigDcyTrISigDcyFStMap, m_iSigDcyTrICcSigDcyFStMap;
    vector< list<int> > m_vSigDcyFSt2, m_vCcSigDcyFSt2;
    vector<int> m_vISigDcyFSt2, m_vICcSigDcyFSt2;
    vector<int> m_vNSigDcyFSt2, m_vNCcSigDcyFSt2;
    vector< vector< list<int> > > m_vDcyTr, m_vCcDcyTr;
    vector<int> m_vIDcyTr, m_vICcDcyTr;
    vector<int> m_vNDcyTr, m_vNCcDcyTr;
    unordered_map<string,int> m_uomDcyTr, m_uomCcDcyTr;
    vector< list<int> > m_vDcyFSt, m_vCcDcyFSt;
    vector<int> m_vIDcyFSt, m_vICcDcyFSt;
    vector<int> m_vNDcyFSt, m_vNCcDcyFSt;
    unordered_map<string,int> m_uomDcyFSt, m_uomCcDcyFSt;
    map<int,int> m_iDcyTrIDcyFStMap, m_iDcyTrICcDcyFStMap;
    map<int,int> m_iSigDcyTrIDcyTrMap, m_iSigDcyTrICcDcyTrMap;
    map<int,int> m_iSigDcyFStIDcyFStMap, m_iSigDcyFStICcDcyFStMap;
    map<int,int> m_iSigDcyFSt2IDcyFStMap, m_iSigDcyFSt2ICcDcyFStMap;
    int m_pidOfGISRGam;
    int m_pidOfGFSRGam;
    int m_pidOfISt;
    int m_pidOfAnything;
    int m_pidOfIRAFlag;
  public:
    topoana()
    {
      m_pkgPath="/besfs/users/zhouxy/tools/topoana/v1.6.9/";
      m_verNum="v1.6.9";

      m_nmsOfIptRootFls.clear();
      m_nEtrsMax=ULONG_MAX; // ULONG_MAX=9223372036854775807 is the maximum unsigned long int number. 
      m_cutMethod=false;
      m_ccSwitch=false;
      m_ignoreGISR=true;
      m_ignoreGFSR=true;
      m_hHdDcyBrsMax=UINT_MAX;
      m_nDcyTrsToBePrtdMax=ULONG_MAX;
      m_nDcyFStsToBePrtdMax=ULONG_MAX;
      m_spr1stBrsOfDcyTrs=false;
      m_sprTopoTags=false;
      m_nEtrsMaxInASngOptRootFl=ULONG_MAX;
      m_nDcyTrsToBeAnlzdMax=ULONG_MAX;
      m_nEtrsOfCtrSmpMax=100000;
      m_sttaDcyTrsAndDcyFSts=false;

      m_vIdPid.clear();
      m_vIddPid.clear();
      m_vPttaPid.clear();
      m_vNDcyBrsMax.clear();
      m_vVPttaPid1.clear();
      m_vNDcyBrsMax1.clear();
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

      m_pidOfGISRGam=22222222;
      m_pidOfGFSRGam=-22222222;
      m_pidOfISt=99999999;
      m_pidOfAnything=-99999999;
      m_pidOfIRAFlag=100000000;
    }
    ~topoana(){}
    string &       trim(string & line);
    void           makeMapsOnPdata();
    int 	   getPidFromTxtPnm(string pnm);
    void           writeErrInfOnPid3PchrgMap(int pid);
    void           writePnmFromPid(ostream & out,string pnmType,int pid);
    void           sortByPidAndPchrg(vector<int> &via,vector<int> &vib,vector<int> &vic,vector<int> &vid);
    void           sortBySzPidAndPchrg(vector< vector<int> > &vVia,vector< vector<int> > &vVib,vector< vector<int> >&vVic,vector< vector<int> > &vVid);
    bool           cmprBySzPidAndPchrg(vector<int> & via, vector<int> & vib);
    void           recurHigherHierExchOrd(vector<int> & vNewIdx, vector<int> & vOldIdx, vector<int> & vPid, vector<int> & vMidx);
    void           recurHighHierExchOrd(vector<int> & vIdxYngi, vector<int> & vIdxYngj, vector<int> & vPid, vector<int> & vMidx);
    void           sortPs(vector<int> & vPid, vector<int> & vMidx);
    int		   getCcPid(int pid);
    void           getDcyTr(vector<int> vPid,vector<int> vMidx,vector< list<int> > & dcyTr);
    void           getDcyTr(vector<int> vPid,vector<int> vMidx,vector< list<int> > & dcyTr, vector<int> & vIdxOfHead, vector<int> & vMidxOfHead);
    void           getDcyTr(vector<int> vPid,vector<int> vMidx,vector< list<int> > & dcyTr, vector<int> & vIdxOfHead, vector<int> & vMidxOfHead, vector<int> & vIIncIncSeqDcyBrs);
    void           getDcyTr(vector<int> vPid,vector<int> vMidx,vector< list<int> > & dcyTr, vector<int> & vIdxOfHead, vector<int> & vMidxOfHead, vector<int> & vIIncIncOrIRASeqDcyBrs, vector<int> & vIIRAIncOrIRASeqDcyBrs);
    void           sortByPidAndPchrg(list<int> &lia);
    void           getDcyFSt(vector<int> & vPid,vector<int> & vMidx,list<int> & dcyFSt);
    unsigned int   countIRADcyBr(vector<int> & vPid, vector<int> & vMidx, list<int> IRADcyBr, bool areHeadsRequiredToBeMatched=false);
    void           readOpenCurly(ifstream & fin, string & line, string prompt);
    void           read1stLineOrCloseCurly(ifstream & fin, string & line, bool essential, string errinforprompt);
    void           readExtraLinesOrCloseCurly(ifstream & fin, string & line, string prompt);
    void           readCloseCurly(ifstream & fin, string & line, string prompt);
    void           readCard(string topoAnaCardFlNm);
    void	   checkInput();
    unsigned int   countIncLiaInVlib(list<int> & lia, vector< list<int> > & Vlib);
    bool	   isLiaMatchedWithLib(list<int> & lia, list<int> & lib);
    unsigned int   countSeqDcyBrsInDcyTr(vector< list<int> > & seqDcyBrs, vector<int> vIdxOfHead1, vector<int> vMidxOfHead1, vector< list<int> > & dcyTr, vector<int> vIdxOfHead2, vector<int> vMidxOfHead2);
    unsigned int   countIncSeqDcyBrsInDcyTr(vector<int> vIIncIncSeqDcyBrs, vector< list<int> > & incSeqDcyBrs, vector<int> vIdxOfHead1, vector<int> vMidxOfHead1, vector< list<int> > & dcyTr, vector<int> vIdxOfHead2, vector<int> vMidxOfHead2);
    void           getSubsetsOfVPidandVMidxFromAP(vector<int> & vPid,vector<int> & vMidx, unsigned int idx);
    bool	   isPaADescendantOfPb(vector<int> vMidx, unsigned int idxA, unsigned int idxB);
    unsigned int   countIncOrIRASeqDcyBrsInDcyTr(vector<int> vIIncIncOrIRASeqDcyBrs, vector<int> vIIRAIncOrIRASeqDcyBrs, vector< list<int> > & incOrIRASeqDcyBrs, vector<int> vIdxOfHead1, vector<int> vMidxOfHead1, vector< list<int> > & dcyTr, vector<int> vIdxOfHead2, vector<int> vMidxOfHead2, vector<int> vPid, vector<int> vMidx);
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
