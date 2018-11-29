#ifndef Physics_Analysis_FitObject_H
#define Physics_Analysis_FitObject_H

/**
 * @brief    This is a base class for fit specific objects that you use for fitting invariant mass spectra.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 26th, 2018
 * @details  This base class provides its derivate classes with the members and methods necessary to load the spectrum and the `ReconstructedParticle` object of the particle that you want to reconstruct. Basic `RooFit` objects such as `RooDataHist` that are needed when fitting whatever the function you are fitting are initialised through this class as well.
 * @remark   @b DEVELOPMENTAL
 */


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "../inc/FrameworkSettings.h"
	#include "../inc/ReconstructedParticle.h"
	#include "RooDataHist.h"
	#include "RooRealVar.h"
	#include "RooAddPdf.h"
	#include "TH1D.h"
	#include <iostream>
	#include <utility>


// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class FitObject
{
public:
	// * Constructor and destructors *
	FitObject(TH1D& hist, const ReconstructedParticle& particle);
	~FitObject();

	// * Setters *
	void PerformFit();

	// * Getters *
	bool IsLoaded();

protected:
	// * Data members *
	ReconstructedParticle fParticle; //!< The particle that you are trying to reconstruct. <b>All fitting parameters should be described in this object!</b>
	RooDataHist fRooDataHist; //!< `RooFit` distribution as constructed from the 'imported' histogram.
	RooRealVar fRooRealVar; //!< Variable used by `RooFit` to perform fit on.
	RooArgList fSigArgs;
	RooArgList fBckArgs;
	std::unique_ptr<RooAddPdf> fFullShape;
	void AddPolynomialBackground(UChar_t nPol = 2);

	TH1D* fHistogram; //!< Pointer to histogram that you want to analyse (perform a fit on). <b>The object does not own the histogram!</b>
	UChar_t fNPolynomial;

	// * Private methods *
	void Initialize();
	void SetRooRealVar();
	void SetInvMassDistr();
	virtual void SetSignalArguments();

};


// * =========================================== * //
// * ------- CONSTRUCTORS AND DESTRUCTORS ------ * //
// * =========================================== * //
/**
 * @brief Construct particle based on its code in the PDG.
 */
FitObject::FitObject(TH1D& hist, const ReconstructedParticle& particle) :
	fParticle(particle), fHistogram(nullptr)
{
	Initialize();
}



// * ======================= * //
// * ------- SETTERS ------- * //
// * ======================= * //

/**
 * @brief perform fit if `RooFit` objects have been loaded.
 */
void FitObject::PerformFit()
{
	if(fFullShape) {
		fFullShape->fitTo(
			fRooDataHist,
			RooFit::Range(fParticle.FitFrom(), fParticle.FitUntil()));
	}
}



// * ======================= * //
// * ------- GETTERS ------- * //
// * ======================= * //

bool FitObject::IsLoaded()
{
	return fParticle.IsLoaded() && fHistogram;
}


// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //
/**
 * @brief Create and set `RooRealVar` data member specifically for resonstructing a certain particle (`ReconstructedParticle`). This data member will be used to as a variable in the `RooFit` procedure
 */
void FitObject::SetRooRealVar() {
	if(fParticle.IsLoaded()) {
		fRooRealVar = RooRealVar(
			Form("#it{M}_{%s}", fParticle.GetDaughterLabel()),
			Form("#it{M}_{%s} (GeV/#it{c}^{2})", fParticle.GetDaughterLabel()),
			fParticle.PlotFrom(),
			fParticle.PlotUntil()
		);
	}
}

/**
 * @brief Create a `RooDataHist` specifically for resonstructing a certain particle (`ReconstructedParticle`).
 */
void FitObject::SetInvMassDistr() {
	if(fParticle.IsLoaded() && fHistogram) {
		RooDataHist distr(
			Form("%scandidate_RooDataHist", fParticle.GetName()),
			fHistogram->GetTitle(), fRooRealVar, RooFit::Import(*fHistogram));
	}
}

/**
 * @brief Auxiliary function that allows you to share functionality among constructors.
 */
void FitObject::Initialize()
{
	if(fHistogram) {
		SetRooRealVar();
		SetInvMassDistr();
		fSigArgs.takeOwnership();
		fBckArgs.takeOwnership();
	}
}


#endif