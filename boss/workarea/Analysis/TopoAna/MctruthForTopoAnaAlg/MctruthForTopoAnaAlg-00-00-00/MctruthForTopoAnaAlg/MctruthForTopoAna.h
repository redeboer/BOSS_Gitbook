#ifndef Physics_Analysis_MctruthForTopoAna_H
#define Physics_Analysis_MctruthForTopoAna_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "GaudiKernel/Algorithm.h"
	#include "GaudiKernel/NTuple.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_packages
/// @{

	/// Helper class for the `topoana` package.
	/// It is essential to run load this algorithm along with your own algorithm if you want to run `topoana.exe` over the output of your initial event selection. This algorithm (`MctruthForTopoAna`) will store the relevant 'Monte Carlo truth' information.
	/// @remark January 21st, 2019: <b>This procedure has been implemented in the `TrackSelector` and does not have to be loaded anymore.</b>
	/// @author Zhou Xingyu 周兴玉
	class MctruthForTopoAna : public Algorithm
	{
	public:
		MctruthForTopoAna(const std::string& name, ISvcLocator* pSvcLocator);
		StatusCode initialize();
		StatusCode execute();
		StatusCode finalize();
	private:
		NTuple::Tuple *m_tuple;     ///< `NTuple` that will be the eventual `"MctruthForTopoAna"` tree.
		NTuple::Item<int>  m_ievt;  ///< @b Counter for number of events (not the ID!)
		NTuple::Item<int>  m_runid; ///< Run number ID.
		NTuple::Item<int>  m_evtid; ///< Rvent number ID.
		NTuple::Item<int>  m_nmcps; ///< Number of MC particles stored for this event. This one is necessary for loading following two items, because they are arrays.
		NTuple::Array<int> m_pid; ///< PDG code for the particle in this array.
		NTuple::Array<int> m_midx; ///< Track index of the mother particle.
	};

/// @}
#endif