#ifndef Physics_Analysis_MctruthForTopoAna_H
#define Physics_Analysis_MctruthForTopoAna_H

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/NTuple.h"

/**
 * @brief Helper class for the `topoana` package.
 * @details It is essential to run load this algorithm along with your own algorithm if you want to run `topoana.exe` over the output of your initial event selection. This algorithm (`MctruthForTopoAna`) will store the relevant 'MC truth' information.
 * @remark January 21st, 2019: *This procedure has been implemented in the `TrackSelector` and does not have to be loaded anymore.
 * @author Zhou Xingyu 周兴玉
 */
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
	NTuple::Array<int> m_pid;
	NTuple::Array<int> m_midx;
};

#endif
