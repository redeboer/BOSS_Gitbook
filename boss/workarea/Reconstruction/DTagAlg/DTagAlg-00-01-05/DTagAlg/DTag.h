#ifndef Reconstruction_DTag_H
#define Reconstruction_DTag_H 

#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/NTuple.h"

class DTag : public Algorithm {

public:
  DTag(const std::string& name, ISvcLocator* pSvcLocator);
  StatusCode initialize();
  StatusCode execute();
  StatusCode finalize();  

private:
  StatusCode registerParent(MsgStream& log);
  StatusCode clearEvtRecDTagCol(MsgStream& log);
  void registerEvtRecDTagCol(MsgStream& log);
 
private:
  bool m_recD0;
  Algorithm* m_NeutralDReconstruction;
  bool m_recDp;
  Algorithm* m_ChargedDReconstruction;
  bool m_recDs;
  Algorithm* m_DsReconstruction;

  bool m_rawdstonly;
};
#endif


