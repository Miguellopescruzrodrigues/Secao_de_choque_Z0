#ifndef __CINT__
  #include "RooGlobalFunc.h"
  #endif
  #include "RooRealVar.h"
  #include "RooDataSet.h"
  #include "RooGaussian.h"
  #include "RooChebychev.h"
  #include "RooAddPdf.h"
  #include "RooExtendPdf.h"
 #include "RooFitResult.h"
 #include "RooDataHist.h"
 #include "TCanvas.h"
 #include "TAxis.h"
 #include "TH1F.h"
 #include "TF1.h"
 #include "TRandom.h"
 #include "TFitResult.h"
 #include "TMatrixDSym.h"
 #include "RooPlot.h"
 #include "RooProduct.h"
 using namespace RooFit ;
 
 
 void exampleIntegral()
 {
 
   const float XMIN = -10; const float XMAX = 10;
   
   const float sideband_min = -2.0;
   const float sideband_max = 2.0;
   const float signal_min = 2.0;
   const float signal_max = XMAX;
 
 
   const float MEAN_INPUT = 0.0;
   const float SIGMA_INPUT = 1.0;
 
   // Declare observable x
   RooRealVar x("x","x",XMIN,XMAX) ;
 
   // Create Gaussian PDFs g(x,mean,sigma) 
   RooRealVar mean("mean","mean of gaussian",MEAN_INPUT, -1000, 1000) ;
   RooRealVar sigma("sigma","width of gaussians",SIGMA_INPUT, 0, 1000) ;
   RooGaussian sig("sig","gaussian",x,mean,sigma);
   
   TH1F * h = new TH1F("h", "ROOT fit", 100, XMIN, XMAX);
   for(int i = 0; i != 1000; ++i)
     h->Fill(gRandom->Gaus(MEAN_INPUT, SIGMA_INPUT));
 
   // Define signal range in which events counts are to be defined
   x.setRange("sideband", sideband_min, sideband_max);
   x.setRange("extrapolate", signal_min, signal_max);
   x.setRange("fullRange", XMIN, XMAX);
 
   // Associated nsig as expected number of events
   RooRealVar nsig("nsig","number of signal events",50,0.,10000) ;
   RooExtendPdf esig("esig","extended signal p.d.f",sig,nsig,"fullRange") ;
 
   RooDataHist h2("h2", "h2", x, Import(*h));

  TF1 *f1 = new TF1("f1", "gaus", sideband_min, sideband_max);
  TFitResultPtr r = h->Fit(f1, "LRS");
  h->Draw("e same");

  Double_t integ = f1->Integral(signal_min, signal_max, 0)/ h->GetBinWidth(0);
  Double_t dinteg = 
    f1->IntegralError(signal_min, signal_max, 0,r->GetCovarianceMatrix().GetMatrixArray())
    / h->GetBinWidth(0);

 // Perform unbinned extended ML fit to data
  RooFitResult * rf = esig.fitTo(h2,Extended(kTRUE), Range("sideband"), Save()) ;
 
  //RooArgList pars = rf->floatParsFinal();
 // LM: you need to use the parameters of the fitted function (the one from floatParsFinal is probably a copy)
  RooArgList pars(* esig.getParameters(RooArgSet(x) ) );

  //LM: make fitted function (need to add normalized term to pdf)
  RooArgSet prodSet(esig); //prodSet.add(nsig);
  RooProduct unNormPdf("fitted Function", "fitted Function", prodSet);
  TF1 * f2 = unNormPdf.asTF(RooArgList(x), pars);

  float nsig1 = ((RooRealVar*) pars.find("nsig"))->getVal();
  float dnsig1 = ((RooRealVar*) pars.find("nsig"))->getError();
  float mean1 = ((RooRealVar*) pars.find("mean"))->getVal();
  float dmean1 = ((RooRealVar*) pars.find("mean"))->getError();
  float sigma1 = ((RooRealVar*) pars.find("sigma"))->getVal();
  float dsigma1 = ((RooRealVar*) pars.find("sigma"))->getError();
  cout << " nsig = " << nsig1 << " +- " << dnsig1 << endl;
  cout << " mean = " << mean1 << " +- " << dmean1 << endl;
  cout << " sigma = " << sigma1 << " +- " << dsigma1 << endl;

  //Double_t par_tmp[3] = {nsig1, mean1, sigma1};
  //LM not needed can use stored values in f2

  Double_t integ2_full = f2->Integral(XMIN, XMAX);
  Double_t integ2 = nsig1*f2->Integral(signal_min, signal_max, 0)/integ2_full;
  // LM: for drawing need to clone 
  // new TCanvas(); f2->DrawClone();
  Double_t dinteg2 = nsig1*f2->IntegralError(signal_min, signal_max, 0, rf->covarianceMatrix().GetMatrixArray())/integ2_full;

  RooPlot * xframe = x.frame(Title("RooFit fit"));
  h2.plotOn(xframe);

  esig.plotOn(xframe);
  esig.paramOn(xframe,Layout(0.55));

  new TCanvas();
  xframe->Draw();

  cout << "\n\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
  int min_bin = h->FindBin(signal_min);
  int max_bin = h->FindBin(signal_max);
  cout << " Actual # of entries between [" << signal_min << ", " << signal_max 
       << "] = " << h->Integral(min_bin, max_bin) << endl;
  cout << " Extrapolation between [" << signal_min << ", " << signal_max 
       << "] with ROOT, integral = " << integ << " +- " << dinteg << endl;
  cout << " Extrapolation between [" << signal_min << ", " << signal_max 
       << "] with RooFit, integral = " << integ2 << " +- " << dinteg2
       << endl;
  cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

}