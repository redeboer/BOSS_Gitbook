/**
 * @brief    A (temporary?) script that is used to quickly visualize the contents of a RhoPi `ROOT` file.
 * @details  This script is used for debugging purposes, but also illustrates the use of classes in this repository.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 20th, 2018
 */


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include <iostream>
	#include "TCanvas.h"
	#include "TString.h"
	#include "../inc/RhopiRootFile.h"


// * ============================= * //
// * ------- MAIN FUNCTION ------- * //
// * ============================= * //
/**
 * @brief Main function that is used when you compile with `ROOT`.
 */
void QuickPlotRhopi()
{
	// * Load RhoPi ROOT analysis file * //
	RhopiRootFile rhopi("../data/root/ana_rhopi_mc.root");
	if(rhopi.IsZombie()) return;

	// * Invariant mass distributions * //
		rhopi.PlotDistribution1D("fit4c", "mpi0", 500, .1, .17,
			"#it{M}_{#gamma#gamma} distribution;#it{M}_{#gamma#gamma} (GeV/#it{c});counts",
			"ep", "../plots/pdf/QuickPlotRhopi.C/fit4c_mpi0.pdf");
		rhopi.PlotDistribution1D("fit5c", "mrh0", 200, .5, 1.2,
			"#it{M}_{#pi^{+}#pi^{-}} distribution;#it{M}_{#pi^{+}#pi^{-}} (GeV/#it{c});counts",
			"ep", "../plots/pdf/QuickPlotRhopi.C/fit4c_mrh0.pdf");
		rhopi.PlotDistribution1D("fit5c", "mrhp", 200, .5, 1.2,
			"#it{M}_{#pi^{+}#pi^{0}} distribution;#it{M}_{#pi^{+}#pi^{0}} (GeV/#it{c});counts",
			"ep", "../plots/pdf/QuickPlotRhopi.C/fit4c_mrhp.pdf");
		rhopi.PlotDistribution1D("fit5c", "mrhm", 200, .5, 1.2,
			"#it{M}_{#pi^{-}#pi^{0}} distribution;#it{M}_{#pi^{-}#pi^{0}} (GeV/#it{c});counts",
			"ep", "../plots/pdf/QuickPlotRhopi.C/fit4c_mrhm.pdf");
	// * Dalitz plots * //
		rhopi.PlotDistribution2D(
			"fit5c", "mrhm", "mrhp",  200, .2, 4., 150, .2, 3.,
			"Dalitz plot #rho^{-} vs #rho^{+} candidates;#it{M}_{#pi^{0}#pi^{-}} (GeV/#it{c});#it{M}_{#pi^{+}#pi^{0}} (GeV/#it{c})", "colz", kTRUE);
		rhopi.PlotDistribution2D(
			"fit5c", "mrh0", "mrhp",  200, .2, 4., 150, .2, 3.,
			"Dalitz plot #rho^{0} vs #rho^{+} candidates;#it{M}_{#pi^{+}#pi^{-}} (GeV/#it{c});#it{M}_{#pi^{+}#pi^{0}} (GeV/#it{c})", "colz", kTRUE);
		rhopi.PlotDistribution2D(
			"fit5c", "mrh0", "mrhm",  200, .2, 4., 150, .2, 3.,
			"Dalitz plot #rho^{0} vs #rho^{-} candidates;#it{M}_{#pi^{+}#pi^{-}} (GeV/#it{c});#it{M}_{#pi^{-}#pi^{0}} (GeV/#it{c})", "colz", kTRUE);
	// * ToF plots * //
		rhopi.PlotDistribution2D(
			"tof1", "ptrk", "tpi", 200, .1, 1.7, 120, -2., 7.,
			"TOF (#pi hypothesis) versus momentum;p (GeV/#it{c^{2}});TOF (50ns)", "colz", kTRUE);
		rhopi.PlotDistribution2D(
			"tof1", "ptrk", "tp",  200, .1, 1.7, 120, -15., 1.,
			"TOF (proton hypothesis) versus momentum;p (GeV/#it{c^{2}});TOF (50ns)", "colz", kTRUE);
}

/**
 * @brief Main function that allows you to compile outside `ROOT`.
 */
int main()
{
	QuickPlotRhopi();
	return 0;
}