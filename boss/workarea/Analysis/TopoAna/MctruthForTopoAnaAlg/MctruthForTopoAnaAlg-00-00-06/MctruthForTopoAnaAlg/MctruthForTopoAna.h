#ifndef Physics_Analysis_MctruthForTopoAna_H
#define Physics_Analysis_MctruthForTopoAna_H

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/NTuple.h"

class MctruthForTopoAna : public Algorithm 
{
 public:
  MctruthForTopoAna(const std::string& name, ISvcLocator* pSvcLocator);
  StatusCode initialize();
  StatusCode execute();
  StatusCode finalize();  
 private:
  NTuple::Tuple * m_tuple;
  NTuple::Item<int>  m_ievt;
  NTuple::Item<int>  m_runid;
  NTuple::Item<int>  m_evtid;
  NTuple::Item<int>  m_nmcps;
  NTuple::Array<int>  m_pid;
  NTuple::Array<int>  m_midx;
};

#endif
