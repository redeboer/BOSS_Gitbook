#ifndef BOSS_Afterburner_ReconstructedParticle_H
#define BOSS_Afterburner_ReconstructedParticle_H

// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "Particle.h"
	#include "TString.h"
	#include <utility>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_Afterburner_objects
/// @{


	/**
	 * @brief    Container for particle data. In essence an container of `ROOT`'s `TParticlePDG`.
	 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date     October 25th, 2018
	 * @details  This object is an extention of `Particle.h`. It contains additional parameters that are relevant when reconstructing the particle, like fit range in the invariant mass plot. The idea is that you can use this object to generalise functions like fitting, where the mass range is particle-specific. It is then convenient to only have to feed the function this object.
	 * @remark   @b DEVELOPMENTAL
	 */
	class ReconstructedParticle : public Particle
	{
	public:
		// * Constructor and destructors *
		ReconstructedParticle(int pdgCode);
		ReconstructedParticle(const char* pdgCode);
		ReconstructedParticle(int pdgCode, const char* daughters);
		ReconstructedParticle(const char* pdgCode, const char* daughters);

		// * Setters *
		void SetDaughterLabel(const char* daughters);

		// * Getters *
		const double GetLowerMass() const;
		const double GetUpperMass() const;
		const double GetMassOffsetPercentage() const;
		const double FitFrom() const;
		const double FitUntil() const;
		const double PlotFrom() const;
		const double PlotUntil() const;
		const double GetBWPureWidth() const;
		const double GetBWConvolutedWidth() const;
		const double GetSingleGaussianWidth() const;
		const double GetGaussianSmallWidth() const;
		const double GetGaussianWideWidth() const;
		const std::pair<double, double> GetDoubleGaussianWidths() const;
		const std::pair<double, double> GetDoubleGaussianRatio() const;
		const std::pair<double, double> GetFitRange() const;
		const std::pair<double, double> GetPlotRange() const;
		void GetDoubleGaussianWidths(double& from, double& to) const;
		void GetFitRange(double& from, double& to) const;
		void GetPlotRange(double& from, double& to) const;
		const char* GetDaughterLabel() const;

	protected:
		// * Data members *
		TString fDaughterLabels; //!< LaTeX formatted string for decay particles.
		double fMassOffset; //!< Percentage (divided by 100) that the mean (namely, the mass) may vary.
		double fBWPureWidth; //!< Estimate for the width of the Breit-Wigner function when fitting BW only.
		double fBWConvolutedWidth; //!< Estimate for the width of the Breit-Wigner function when convoluted with a double Gaussian.
		double fSingleGaussianWidth; //!< Estimate for the width of one Gaussian function.
		std::pair<double, double> fDoubleGaussianWidths; //!< Pair of two sigma values. You can use that as estimates of the widths for the double gaussian that you plan to fit. These sigmas are supposed to characterise the resolution of the detector. For consistency in naming, the first one should be smaller than the second.
		std::pair<double, double> fFitRange; //!< Invariant mass range over which you fit a function (double Gaussian, Crystal ball, Breit-Wigner, etc.).
		std::pair<double, double> fPlotRange; //!< Invariant mass range that you plot.

		// * Private methods *
		void Initialize();
		void DetermineReconstructionParameters();

	};


/// @}
#endif