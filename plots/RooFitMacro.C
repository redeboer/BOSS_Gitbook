{
	using namespace RooFit;
	gROOT->ProcessLine(".x ~/rootlogon.C");

	TFile f("myZPeakCRAB.root");
	Z_mass=(TH1F*)Zmass->Clone();
	double hmin = Z_mass->GetXaxis()->GetXmin();
	double hmax = Z_mass->GetXaxis()->GetXmax();

	// Declare observable x
	RooRealVar x("x","x",hmin,hmax) ;
	RooDataHist dh("dh","dh",x,Import(*Z_mass)) ;

	RooPlot* frame = x.frame(Title("Z mass")) ;
	dh.plotOn(frame,MarkerColor(2),MarkerSize(0.9),MarkerStyle(21));  //this will show histogram data points on canvas
	dh.statOn(frame);  //this will display hist stat on canvas

	RooRealVar mean("mean"  , "mean" , 95.0, 70.0, 120.0);
	RooRealVar width("width", "width", 5.0 , 0.0 , 120.0);
	RooRealVar sigma("sigma", "sigma", 5.0 , 0.0 , 120.0);
	// RooGaussian gauss("gauss","gauss",x,mean,sigma);
	RooBreitWigner gauss("gauss","gauss",x,mean,sigma);
	// RooVoigtian gauss("gauss","gauss",x,mean,width,sigma);

	RooFitResult* filters = gauss.fitTo(dh,"qr");
	gauss.plotOn(frame,LineColor(4));//this will show fit overlay on canvas
	gauss.paramOn(frame); //this will display the fit parameters on canvas
	//filters->Print("v");

	// Draw all frames on a canvas
	TCanvas* c = new TCanvas("ZmassHisto","ZmassHisto",800,400) ;
	c->cd() ; gPad->SetLeftMargin(0.15);

	frame->GetXaxis()->SetTitle("Z mass (in GeV/c^{2})");  frame->GetXaxis()->SetTitleOffset(1.2);
	float binsize = Z_mass->GetBinWidth(1); char Bsize[50];
	//sprintf(Bsize,"Events per %2.2f",binsize);
	// frame->GetYaxis()->SetTitle(Bsize);
	//frame->GetYaxis()->SetTitleOffset(1.2);
	frame->Draw() ;
	c->Print("myZmaa.eps");

}