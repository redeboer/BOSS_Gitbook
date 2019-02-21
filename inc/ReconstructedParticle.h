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


	/// Extended data container of a reconstructed particle. In essence an container of `ROOT`'s `TParticlePDG`, but with extended functionality for particle reconstruction. It contains additional parameters that are relevant when reconstructing the particle, like fit range in the invariant mass plot. The idea is that you can use this object to generalise functions like fitting, where the mass range is particle-specific. It is then convenient to only have to feed the function this object.
	/// @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date     October 25th, 2018
	class ReconstructedParticle : public Particle
	{
	public:
		/// @name Constructors
			///@{
			ReconstructedParticle(int pdgCode);
			ReconstructedParticle(const char* pdgCode);
			ReconstructedParticle(int pdgCode, const char* daughters);
			ReconstructedParticle(const char* pdgCode, const char* daughters);
			///@}

		/// @name Setters
			///@{
			void SetDaughterLabel(const char* daughters);
			///@}

		/// @name Getters
			///@{
			const char* GetDaughterLabel() const;
			const double FitFrom() const;
			const double FitUntil() const;
			const double GetBWConvolutedWidth() const;
			const double GetBWPureWidth() const;
			const double GetGaussianSmallWidth() const;
			const double GetGaussianWideWidth() const;
			const double GetLowerMass() const;
			const double GetMassOffsetPercentage() const;
			const double GetSingleGaussianWidth() const;
			const double GetUpperMass() const;
			const double PlotFrom() const;
			const double PlotUntil() const;
			const std::pair<double, double> GetDoubleGaussianRatio() const;
			const std::pair<double, double> GetDoubleGaussianWidths() const;
			const std::pair<double, double> GetFitRange() const;
			const std::pair<double, double> GetPlotRange() const;
			void GetDoubleGaussianWidths(double& from, double& to) const;
			void GetFitRange(double& from, double& to) const;
			void GetPlotRange(double& from, double& to) const;
			///@}

	protected:
		/// @name Data members
			///@{
			TString fDaughterLabels; ///< LaTeX formatted string for decay particles.
			double fBWConvolutedWidth; ///< Estimate for the width of the Breit-Wigner function when convoluted with a double Gaussian.
			double fBWPureWidth; ///< Estimate for the width of the Breit-Wigner function when fitting BW only.
			double fMassOffset; ///< Percentage (divided by 100) that the mean (namely, the mass) may vary.
			double fSingleGaussianWidth; ///< Estimate for the width of one Gaussian function.
			std::pair<double, double> fDoubleGaussianWidths; ///< Pair of two sigma values. You can use that as estimates of the widths for the double gaussian that you plan to fit. These sigmas are supposed to characterise the resolution of the detector. For consistency in naming, the first one should be smaller than the second.
			std::pair<double, double> fFitRange; ///< Invariant mass range over which you fit a function (double Gaussian, Crystal ball, Breit-Wigner, etc.).
			std::pair<double, double> fPlotRange; ///< Invariant mass range that you plot.
			///@}

		/// @name Helper methods
			///@{
			void DetermineReconstructionParameters();
			void Initialize();
			///@}

	};


/// @}
#endif