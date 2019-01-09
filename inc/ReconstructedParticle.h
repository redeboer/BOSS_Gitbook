#ifndef Physics_Analysis_ReconstructedParticle_H
#define Physics_Analysis_ReconstructedParticle_H

/// @addtogroup BOSS_Afterburner_objects
/// @{

/**
 * @brief    Container for particle data. In essence an container of `ROOT`'s `TParticlePDG`.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     October 25th, 2018
 * @details  This object is an extention of `Particle.h`. It contains additional parameters that are relevant when reconstructing the particle, like fit range in the invariant mass plot. The idea is that you can use this object to generalise functions like fitting, where the mass range is particle-specific. It is then convenient to only have to feed the function this object.
 * @remark   @b DEVELOPMENTAL
 */


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
#include "../inc/Particle.h"
#include "TParticlePDG.h"
#include "TString.h"
#include <iostream>
#include <utility>

// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
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

#endif