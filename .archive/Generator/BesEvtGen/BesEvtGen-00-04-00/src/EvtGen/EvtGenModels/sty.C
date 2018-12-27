    //================ style sets don't touch it
    TStyle *dayaStyle= new TStyle("DAYA","Daya Bay style");
    Int_t icol=0;
    dayaStyle->SetFrameBorderMode(icol);
    dayaStyle->SetCanvasBorderMode(icol);
    dayaStyle->SetPadBorderMode(icol);
    dayaStyle->SetPadColor(icol);
    dayaStyle->SetCanvasColor(icol);
    dayaStyle->SetStatColor(icol);
    dayaStyle->SetTitleFillColor(icol);
    dayaStyle->SetPalette(1); // set a good color palette 
    dayaStyle->SetPaperSize(TStyle::kUSLetter);
    dayaStyle->SetPadTopMargin(.12);
    dayaStyle->SetPadLeftMargin(.15);
    dayaStyle->SetPadRightMargin(.08);
    dayaStyle->SetPadBottomMargin(.15);
    Int_t font=132; 
    Double_t tsize=0.05; //should be set between 0.03-0.05, is in units of "% of pad"
    dayaStyle->SetTextFont(font);
    dayaStyle->SetTextSize(tsize);

    dayaStyle->SetLabelFont(font,"xyz");
    dayaStyle->SetLabelSize(tsize,"xyz");
    dayaStyle->SetLabelOffset(0.01,"xyz");

    dayaStyle->SetTitleFont(42,"xyz");
    dayaStyle->SetTitleSize(0.08,"xyz");
    dayaStyle->SetTitleYSize(0.07);
    dayaStyle->SetTitleXOffset(0.8);
    dayaStyle->SetTitleYOffset(1.0); //offset the title of y axis a bit
    dayaStyle->SetTitleBorderSize(0);
    dayaStyle->SetNdivisions(505); 

    //use bold lines and markers
// dayaStyle->SetMarkerStyle(0);  //** comment by pingrg, this will give a good quality for scatterplot
    dayaStyle->SetMarkerSize(0.8);
    dayaStyle->SetLineWidth(0.5);
    dayaStyle->SetHistLineWidth(2.);
    dayaStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
    //do not display any of the standard histogram decorations

    dayaStyle->SetOptTitle(0);     //no title box
    //dayaStyle->SetOptStat(1111); //standard stat info
    dayaStyle->SetOptStat(0);    //no stat info
    //dayaStyle->SetOptStat(10);     //number of entries only

    //dayaStyle->SetOptFit(1111);  //standard fit info
    dayaStyle->SetOptFit(0);       //no fit info


    // put tick marks on top and RHS of plots
    //dayaStyle->SetPadTickX(1);
    //dayaStyle->SetPadTickY(1);
    // tick marks only on left and bottom
    dayaStyle->SetPadTickX(0);
    dayaStyle->SetPadTickY(0);

  // additional series styles (Thanks, Jianglai)
  //   1 = solid
  //   2 = long dash (30 10)
  //   3 = dotted (4 8)
  //   4 = dash-dot (15 12 4 12)
  //   5 = short dash ( 15 15 )
  //   6 = dash-dot-dot   gStyle->SetLineStyleString(1,"[]");
  dayaStyle->SetLineStyleString(2,"[30 10]");
  dayaStyle->SetLineStyleString(3,"[4 8]");
  dayaStyle->SetLineStyleString(4,"[15 12 4 12]");
  dayaStyle->SetLineStyleString(5,"[15 15]");
  dayaStyle->SetLineStyleString(6,"[15 12 4 12 4 12]");
  dayaStyle->SetOptDate(0);
  dayaStyle->SetDateY(.98);
  dayaStyle->SetStripDecimals(kFALSE);

  dayaStyle->SetEndErrorSize(0.0); //make the end of error bar longer 
				  
    gROOT->SetStyle("DAYA");
    gROOT->ForceStyle();  
/////////// end of style set    
