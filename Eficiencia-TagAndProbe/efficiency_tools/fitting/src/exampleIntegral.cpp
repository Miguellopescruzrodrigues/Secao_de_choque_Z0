//////////////////////////////////////////////////////////////////////////
//
// 'BASIC FUNCTIONALITY' RooFit tutorial macro #101
// 
// Fitting, plotting, toy data generation on one-dimensional p.d.f
//
// pdf = gauss(x,m,s) 
//
//
// 07/2008 - Wouter Verkerke 
// 
/////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooChi2Var.h"
#include "RooDataSet.h"
#include "RooCBShape.h"
#include "RooExponential.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
using namespace RooFit ;


void crystalballfit()
{
  // S e t u p   m o d e l 
  // ---------------------

  // Declare variables x,mean,sigma with associated name, title, initial value and allowed range
  RooRealVar m("m", "Dependent", 2.2,4.6);
  RooRealVar m0("m0", "M0", 3,0.1,10);
  RooRealVar sigma("sigma", "Sigma", 4,0.1,10);
  RooRealVar alpha("alpha", "Alpha", 5,0.1,10);
  RooRealVar n("n", "Order", 6,0.1,10);

  // Build gaussian p.d.f in terms of x,mean and sigma
  RooCBShape cbshape("cbshape","Crystal Ball",m,m0,sigma,alpha,n) ;  

  
//  RooRealVar x("x","x",0,10) ;
  RooRealVar c("c","C",1,-10,0.1) ;

  RooExponential bkg("exp","Background",m,c);

  // Construct plot frame in 'x'
//  RooPlot* xframe = m.frame(Title("Crystal Ball")) ;


  // P l o t   m o d e l   a n d   c h a n g e   p a r a m e t e r   v a l u e s
  // ---------------------------------------------------------------------------

  // Plot gauss in frame (i.e. in x) 
//  cbshape.plotOn(xframe) ;

  // Change the value of sigma to 3
  sigma.setVal(0.084) ;
  alpha.setVal(0.959) ;
  n.setVal(10) ;
  m0.setVal(3.123) ;
  c.setVal(-1.45) ;
/*
  // Plot gauss in frame (i.e. in x) and draw frame on canvas
  cbshape.plotOn(xframe,LineColor(kRed)) ;
  

  // G e n e r a t e   e v e n t s 
  // -----------------------------

  // Generate a dataset of 1000 events in x from gauss
  RooDataSet* data = cbshape.generate(m,200) ;  
  
  // Make a second plot frame in x and draw both the 
  // data and the p.d.f in the frame
  RooPlot* xframe2 = m.frame(Title("CBShape p.d.f. with data")) ;
  data->plotOn(xframe2) ;
  cbshape.plotOn(xframe2) ;
  

  // F i t   m o d e l   t o   d a t a
  // -----------------------------

  // Fit pdf to data
  cbshape.fitTo(*data) ;

  // Print values of mean and sigma (that now reflect fitted values and errors)
  m0.Print() ;
  sigma.Print() ;

  // Draw all frames on a canvas
  TCanvas* c = new TCanvas("cbshape","cbshape",800,400) ;
  c->Divide(2) ;
  c->cd(1) ; gPad->SetLeftMargin(0.15) ; xframe->GetYaxis()->SetTitleOffset(1.6) ; xframe->Draw() ;
  c->cd(2) ; gPad->SetLeftMargin(0.15) ; xframe2->GetYaxis()->SetTitleOffset(1.6) ; xframe2->Draw() ;
*/

  // A d d   s i g n a l   c o m p o n e n t s 
  // ------------------------------------------

  // Sum the signal components into a composite signal p.d.f.
  RooRealVar sig1frac("sig1frac","fraction of component 1 in signal",0.8,0.,1.) ;
  RooAddPdf sig("sig","Signal",RooArgList(cbshape),sig1frac) ;


  // A d d  s i g n a l   a n d   b a c k g r o u n d
  // ------------------------------------------------

  // Sum the composite signal and background 
  RooRealVar bkgfrac("bkgfrac","fraction of background",0.5,0.,1.) ;
  RooAddPdf  model("model","g1+g2+a",RooArgList(bkg,sig),bkgfrac) ;


  // S a m p l e ,   f i t   a n d   p l o t   m o d e l
  // ---------------------------------------------------

/*   TFile *ForT = new TFile("ForwardTreeCheckMass.root");
   background = (TH1F*)f->Get("background"); //pointer used in ftotal
   TH1F *result = (TH1F*)f->Get("result");

Read histogram
TFile f = new TFile (â€œmyZpeak.rootâ€, â€œREADâ€);
TH1* h = (TH1*)f â†’ Get(â€œhist_nameâ€);
Assign histogram dataset to x object
RooDataHist data("data", "dataset", x, h);


*/
  // Generate a data sample of 1000 events in x from model
//  RooDataSet *data = model.generate(m,750) ;

TFile *f = new TFile ("/home/sebas/macrostian/UPCTreeSRN/ForwardMuonCheckMass.root", "READ");
TH1 *h = (TH1*)f->Get("Dimuon_Mass_checkmass");
RooDataHist *data = new RooDataHist("data", "dataset", m, h);


  // Fit model to data
//  model.fitTo(*data) ;
  model.chi2FitTo(*data) ;
  // Plot data and PDF overlaid
  RooPlot* xframe = m.frame(Title("Crystal Ball Fit")) ;

//  RooPlot* xframe = m.frame(Title("Crystal Ball Fit")) ;
  data->plotOn(xframe) ;
  model.plotOn(xframe) ;

  // Overlay the background component of model with a dashed line
  model.plotOn(xframe,Components(bkg),LineStyle(kDashed)) ;

  // Overlay the background+sig2 components of model with a dotted line
  model.plotOn(xframe,Components(RooArgSet(bkg,cbshape)),LineStyle(kDotted)) ;
//  Double_t chi2 = model.GetShisquare();
  // Print structure of composite p.d.f.
  model.Print("v") ;
//  Double_t chi2 = xframe->chiSquare("model","data",1) ; 
/*

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // M E T H O D   2 - O n e   R o o A d d P d f   w i t h   r e c u r s i v e   f r a c t i o n s  //
  ////////////////////////////////////////////////////////////////////////////////////////////////////

  // Construct sum of models on one go using recursive fraction interpretations
  //
  //   model2 = bkg + (sig1 + sig2)
  //
  RooAddPdf  model2("model","g1+g2+a",RooArgList(bkg,cbshape),RooArgList(bkgfrac,sig1frac),kTRUE) ;    

  // NB: Each coefficient is interpreted as the fraction of the 
  // left-hand component of the i-th recursive sum, i.e.
  //
  //   sum4 = A + ( B + ( C + D)  with fraction fA, fB and fC expands to
  //
  //   sum4 = fA*A + (1-fA)*(fB*B + (1-fB)*(fC*C + (1-fC)*D))


  // P l o t   r e c u r s i v e   a d d i t i o n   m o d e l
  // ---------------------------------------------------------
  model2.plotOn(xframe,LineColor(kRed),LineStyle(kDashed)) ;
  model2.plotOn(xframe,Components(RooArgSet(bkg,cbshape)),LineColor(kRed),LineStyle(kDashed)) ;
  model2.Print("t") ;

*/

//////////Chi2/////////////

  RooDataHist* dh = data->Clone() ;

  // Construct a chi^2 of the data and the model.
  // When a p.d.f. is used in a chi^2 fit, the probability density scaled
  // by the number of events in the dataset to obtain the fit function
  // If model is an extended p.d.f, the expected number events is used
  // instead of the observed number of events.
  model.chi2FitTo(*dh) ;

  // NB: It is also possible to fit a RooAbsReal function to a RooDataHist
  // using chi2FitTo(). 

  // Note that entries with zero bins are _not_ allowed 
  // for a proper chi^2 calculation and will give error
  // messages  
//  RooDataSet* dsmall = (RooDataSet*) data->reduce(EventRange(1,100)) ;
//  RooDataHist* dhsmall = data->Clone() ;


  // Construct Chi2
  RooChi2Var chi2("chi2","chi2",model,*dh) ;

  // Start Minuit session on Chi2
  RooMinuit m2(chi2) ;
  m2.migrad() ;
  m2.hesse() ;
  RooFitResult* r2 = m2.save() ;

  // Print results 
  cout << "result of chi2 fit" << endl ;
  r2->Print("v") ;


/*
TStopwatch chiWatch;
chiWatch.Start();
RooChi2Var chi("chi","chisquare",cbshape,*dh);
RooMinuit chiMinuit(chi);
chiMinuit.optimizeConst(1);
chiMinuit.migrad();
cout << "Chi2 timing: ";
chiWatch.Stop();

cout << "Chi2 time: ";
chiWatch.Print();
*/


  RooChi2Var chi2_lowstat("chi2_lowstat","chi2",model,*dh) ;
  cout << chi2_lowstat.getVal() << endl ;
/*
TH1 * h1 = data->createHistogram(m);
TF1 * f1 = model->asTF(RooArgList(m) , parameters );
h1->Fit(f1);
*/
///////////////////////////

  cout << "chi2 = " << chi2 << endl ;



  new TCanvas("Crystal_Ball_Fit","Crystal_Ball_Fit",600,600) ;
  gPad->SetLeftMargin(0.15) ; xframe->GetYaxis()->SetTitleOffset(1.4) ; xframe->Draw() ;


  
 
}