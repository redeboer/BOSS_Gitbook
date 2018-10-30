/**
 * @brief Example script for RooFit fitting.
 * @details Based on this tutorial: https://root.cern.ch/roofit-20-minutes.
 */

#include "RooAddPdf.h"
#include "RooArgList.h"
#include "RooArgusBG.h"
#include "RooDataSet.h"
#include "RooFit.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
using namespace RooFit;


void RooFitExample()
{
	
}

void temp()
{

	/*
		For example a Gaussian probability density function consists typically of four objects, three objects representing the observable, the mean and the sigma parameters, and one object representing a Gaussian probability density function.
	*/

	// * --- Observable --- * //
	RooRealVar mes("mes","m_{ES} (GeV)",5.20,5.30) ;

	// * --- Parameters --- * //
	RooRealVar sigmean("sigmean","B^{#pm} mass",5.28,5.20,5.30) ;
	RooRealVar sigwidth("sigwidth","B^{#pm} width",0.0027,0.001,1.) ;

	// * --- Build Gaussian PDF --- * //
	RooGaussian signal("signal","signal PDF",mes,sigmean,sigwidth) ;

	/*
		Similarly, model building operations such as addition, multiplication, integration are represented by separate operator objects and make the modeling language scale well to models of arbitrary complexity.
	*/

	/*
		Starting with the Gaussian signal p.d.f. defined above, the example below constructs a one-dimensional probability density function with a Gaussian signal component and a 'ARGUS' phase space background component.
	*/

	// * --- Build Argus background PDF --- * //
	RooRealVar argpar("argpar", "argus shape parameter", -20.0, -100., -1.);
	RooArgusBG background("background", "Argus PDF", mes, RooConst(5.291), argpar);

	// * --- Construct signal+background PDF --- * //
	RooRealVar nsig("nsig", "#signal events", 200, 0., 10000);
	RooRealVar nbkg("nbkg", "#background events", 800, 0., 10000);
	RooAddPdf model("model", "g+a", RooArgList(signal, background), RooArgList(nsig, nbkg));

	/*
		We can now use this p.d.f. to generate an unbinned toy dataset, fit the p.d.f to that dataset using an unbinned maximum likelihood fit and visalizes the data with the p.d.f overlaid.
	*/

	// * --- Generate a toyMC sample from composite PDF --- * //
	RooDataSet *data = model.generate(mes,2000);

	// * --- Perform extended ML fit of composite PDF to toy data --- * //
	model.fitTo(*data);

	// * --- Plot toy data and composite PDF overlaid --- * //
	RooPlot* mesframe = mes.frame();
	data->plotOn(mesframe);
	model.plotOn(mesframe);
	model.plotOn(mesframe,Components(argus),LineStyle(kDashed));

	/*
		In the example above, all indivual components of the RooFit p.d.f (the variables, component p.d.f.s and combined p.d.f.) are all created individually in the macro. It is also possible to organize them in a container class 'the workspace' that has an associated factory tool to create trees of RooFit objects of arbitrary complexity from a simple construction language. A much shorter macro that uses a workspace and is equivalent to the above macro is shown below:
	*/

	RooWorkspace w("w",kTRUE);
	w.factory("Gaussian::gauss(mes[5.20,5.30],mean[5.28,5.2,5.3],width[0.0027,0.001,1])");
	w.factory("ArgusBG::argus(mes,5.291,argpar[-20,-100,-1])");
	w.factory("SUM::sum(nsig[200,0,10000]*gauss,nbkg[800,0,10000]*argus)");

	// * --- Generate a toyMC sample from composite PDF --- * //
	RooDataSet *data = w::sum.generate(w::mes,2000);

	// * --- Perform extended ML fit of composite PDF to toy data --- * //
	w::sum.fitTo(*data);

	// * --- Plot toy data and composite PDF overlaid --- * //
	RooPlot* mesframe = w::mes.frame();
	data->plotOn(mesframe);
	w::sum.plotOn(mesframe);
	w::sum.plotOn(mesframe,Components(w::argus),LineStyle(kDashed));

	mesframe->Draw();
}