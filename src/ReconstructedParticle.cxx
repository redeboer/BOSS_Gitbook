// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "ReconstructedParticle.h"
	#include <iostream>



// * =========================================== * //
// * ------- CONSTRUCTORS AND DESTRUCTORS ------ * //
// * =========================================== * //


	/**
	 * @brief Construct particle based on its code in the PDG.
	 */
	ReconstructedParticle::ReconstructedParticle(int pdgCode) : Particle(pdgCode)
	{
		Initialize();
	}


	/**
	 * @brief Construct particle based on its code in the PDG.
	 */
	ReconstructedParticle::ReconstructedParticle(const char* particleName) : Particle(particleName)
	{
		Initialize();
	}


	/**
	 * @brief Construct particle based on its code in the PDG.
	 */
	ReconstructedParticle::ReconstructedParticle(int pdgCode, const char* daughters) :
		Particle(pdgCode), fDaughterLabels(daughters)
	{
		Initialize();
	}


	/**
	 * @brief Construct particle based on its code in the PDG. This constructor also sets the daughters.
	 */
	ReconstructedParticle::ReconstructedParticle(const char* particleName, const char* daughters) :
		Particle(particleName), fDaughterLabels(daughters)
	{
		Initialize();
	}


	/**
	 * @brief Encapsulation of what any constructor for this object needs to do.
	 */
	void ReconstructedParticle::Initialize()
	{
		DetermineReconstructionParameters();
	}



// * ======================= * //
// * ------- SETTERS ------- * //
// * ======================= * //


	/**
	 * @brief Set a LaTeX label for the daughters. This method is public as to allow you to modify it later. Construction cannot be automised, as it is up to you to decide which decay channel you want to analyse.
	 */
	void ReconstructedParticle::SetDaughterLabel(const char* daughters)
	{
		fDaughterLabels = daughters;
	}



// * ======================= * //
// * ------- GETTERS ------- * //
// * ======================= * //


	/**
	 * @brief Get (compute) the lower mass boundary. Useful for fitting parameters.
	 */
	const double ReconstructedParticle::GetLowerMass() const
	{
		if(fParticlePDG) return (1. - fMassOffset) * GetMass();
		else return 0.;
	}


	/**
	 * @brief Get (compute) the upper mass boundary. Useful for fitting parameters.
	 */
	const double ReconstructedParticle::GetUpperMass() const
	{
		if(fParticlePDG) return (1. + fMassOffset) * GetMass();
		else return 0.;
	}


	/**
	 * @brief Get offset mass percentage. Useful for fitting parameters.
	 */
	const double ReconstructedParticle::GetMassOffsetPercentage() const
	{
		return fMassOffset;
	}


	/**
	 * @brief Get estimate for the width of the Breit-Wigner function when fitting a pure BW only.
	 */
	const double ReconstructedParticle::GetBWPureWidth() const
	{
		return fBWPureWidth;
	}


	/**
	 * @brief Get estimate for the width of the Breit-Wigner function when the BW is convoluted with a Double gaussian.
	 */
	const double ReconstructedParticle::GetBWConvolutedWidth() const
	{
		return fBWConvolutedWidth;
	}


	/**
	 * @brief Get estimate for the width of the Breit-Wigner function when the BW is convoluted with a Double gaussian.
	 */
	const double ReconstructedParticle::GetSingleGaussianWidth() const
	{
		return fSingleGaussianWidth;
	}


	/**
	 * @brief Get the estimate for the width of the smaller Gaussian function.
	 */
	const double ReconstructedParticle::GetGaussianSmallWidth() const
	{
		return fDoubleGaussianWidths.first;
	}


	/**
	 * @brief Get the estimate for the width of the wider Gaussian function.
	 */
	const double ReconstructedParticle::GetGaussianWideWidth() const
	{
		return fDoubleGaussianWidths.second;
	}


	/**
	 * @brief Get the left boundary of the plotting range.
	 */
	const double ReconstructedParticle::FitFrom() const
	{
		return fFitRange.first;
	}


	/**
	 * @brief Get the right boundary of the plotting range.
	 */
	const double ReconstructedParticle::FitUntil() const
	{
		return fFitRange.second;
	}


	/**
	 * @brief Get the left boundary of the plotting range.
	 */
	const double ReconstructedParticle::PlotFrom() const
	{
		return fPlotRange.first;
	}


	/**
	 * @brief Get the right boundary of the plotting range.
	 */
	const double ReconstructedParticle::PlotUntil() const
	{
		return fPlotRange.second;
	}


	/**
	 * @brief Get plot range through a `return` statement.
	 */
	const std::pair<double, double> ReconstructedParticle::GetDoubleGaussianWidths() const
	{
		return fDoubleGaussianWidths;
	}


	/**
	 * @brief Get fit range through a `return` statement.
	 */
	const std::pair<double, double> ReconstructedParticle::GetFitRange() const
	{
		return fFitRange;
	}


	/**
	 * @brief Get plot range through a `return` statement.
	 */
	const std::pair<double, double> ReconstructedParticle::GetPlotRange() const
	{
		return fPlotRange;
	}


	/**
	 * @brief Get the plot range through reference.
	 */
	void ReconstructedParticle::GetDoubleGaussianWidths(double& from, double& to) const
	{
		from = fDoubleGaussianWidths.first;
		to   = fDoubleGaussianWidths.second;
	}


	/**
	 * @brief Get the plot range through reference.
	 */
	void ReconstructedParticle::GetFitRange(double& from, double& to) const
	{
		from = fFitRange.first;
		to   = fFitRange.second;
	}


	/**
	 * @brief Get the plot range through reference.
	 */
	void ReconstructedParticle::GetPlotRange(double& from, double& to) const
	{
		from = fPlotRange.first;
		to   = fPlotRange.second;
	}


	/**
	 * @brief Get the LaTeX label for the daughters.
	 */
	const char* ReconstructedParticle::GetDaughterLabel() const
	{
		return fDaughterLabels.Data();
	}



// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //
// ! Tweak your analysis parameters here ! //


	/**
	 * @brief Determine the wide and small sigma estimates for the double Gaussian fit. These are supposed to characterise the resolution of the deterctor.
	 */
	void ReconstructedParticle::DetermineReconstructionParameters()
	{
		if(fParticlePDG) {
			switch(fParticlePDG->PdgCode()) {
				case 111: // neutral pion
					fMassOffset           = .02;
					fBWPureWidth          = .013;
					fBWConvolutedWidth    = .00002;
					fSingleGaussianWidth  = .002;
					fDoubleGaussianWidths = {.00499, .0135};
					fFitRange             = {.10, .17};
					fPlotRange            = {.10, .17};
					break;
				case 113: // neutral rho
					fMassOffset           = .05;
					fBWPureWidth          = .8;
					fBWConvolutedWidth    = .001;
					fSingleGaussianWidth  = .002;
					fDoubleGaussianWidths = {.0469, .1312};
					fFitRange             = {.40, 1.1};
					fPlotRange            = {.30, 1.7};
					break;
				case 213:
				case -213: // rho meson
					fMassOffset           = .05;
					fBWPureWidth          = .8;
					fBWConvolutedWidth    = .0004;
					fSingleGaussianWidth  = .002;
					fDoubleGaussianWidths = {.0542, .209};
					fFitRange             = {.50, 1.42};
					fPlotRange            = {.30, 1.7};
					break;
				case 421: // D0 meson
					fMassOffset           = .05;
					fBWPureWidth          = .8;    //! not yet optimised
					fBWConvolutedWidth    = .0004; //! not yet optimised
					fSingleGaussianWidth  = .002;
					fDoubleGaussianWidths = {.0542, .209};
					fFitRange             = {1.84, 1.91};
					fPlotRange            = {1.83, 1.94};
					break;
				case 333: // phi meson
					fMassOffset           = .05;
					fBWPureWidth          = .8;    //! not yet optimised
					fBWConvolutedWidth    = .0004; //! not yet optimised
					fSingleGaussianWidth  = .002;
					fDoubleGaussianWidths = {.002, .006};
					fFitRange             = {.99, 1.053};
					fPlotRange            = {.99, 1.18};
					break;
				case 443: // J/psi meson
					fMassOffset           = .05;
					fBWPureWidth          = .8;
					fBWConvolutedWidth    = .0004;
					fSingleGaussianWidth  = .002;
					fDoubleGaussianWidths = {1e-8, 1e-8};
					fFitRange             = {3.096813, 3.096815};
					fPlotRange            = {3.096813, 3.096815};
					break;
				default:
					std::cout << "ERROR: No particle reconstruction defined for PDG code " << fParticlePDG->PdgCode() << " (" << fParticlePDG->GetName() << ")" << std::endl;
					break;
			}
		}
	}