//Change if you need
#include "src/dofits/DoFit_Z0.cpp"
#include "src/create_folder.cpp"
#include "src/get_efficiency.cpp"
#include "src/make_TH1D.cpp"



#include <iomanip>
#include <sstream>
#include <vector>
#include <TStyle.h>
#include <TAxis.h>
#include <TLatex.h>
#include <TPaveText.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TNtupleD.h>
#include <TH1D.h>
#include <TLorentzVector.h>
#include <TLegend.h>
#include <TSystem.h>
#include <RooWorkspace.h>
#include <RooRealVar.h>
#include <RooConstVar.h>
#include <RooDataSet.h>
#include <RooGaussian.h>
#include <RooChebychev.h>
#include <RooBernstein.h>
#include <RooExponential.h>
#include <RooAddPdf.h>
#include <RooPlot.h>
#include <RooBifurGauss.h>
#include "TMath.h"
#include <RooGenericPdf.h>
#include "TRatioPlot.h"
#include <RooBifurGauss.h>
#include <RooProduct.h>
#include <RooHist.h>
#include "RooStats/SPlot.h"
#include "RooProdPdf.h"
#include "RooAddition.h"
#include "RooProduct.h"
#include "RooAbsPdf.h"
#include "RooFit.h"
#include "RooFitResult.h"
#include "RooMCStudy.h"
#include <iostream>
#include <TF1.h>
#include <RooPolynomial.h>
#include <fstream>
#include <TGraph.h>
#include "TMultiGraph.h"
#include <RooExponential.h>
#include "RooGaussModel.h"
#include "RooDecay.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"




//Which Muon Id do you want to study?
string MuonId   = "trackerMuon";
//string MuonId   = "standaloneMuon";
//string MuonId   = "globalMuon";

//Which quantity do you want to use?
string quantity = "Pt";     double bins[] =  {10., 15., 20., 25., 30., 35., 40., 45., 50.0, 65.0};
//string quantity = "Eta";    double bins[] =  {-2.0, -1.5, -1.0, -0.5, 0., 0.5, 1.0, 1.5, 2.0};
//string quantity = "Phi";    double bins[] = {-3.0, -2.5, -2.0, -1.0, -0.5, -0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};

void efficiency()
{
	//Path where is going to save results png for every bin 
	const char* path_bins_fit_folder = "results/bins_fit/";
	create_folder(path_bins_fit_folder, true);

	// Loop for every bin and fit it
	int nbins = sizeof(bins)/sizeof(*bins) - 1;
	double** yields_n_errs = new double*[nbins];
	for (int i = 0; i < nbins; i++)
	{

		//Creates conditions
		string conditions = string(    "ProbeMuon_" + quantity + ">=" + to_string(bins[i]  ));
		conditions +=       string(" && ProbeMuon_" + quantity + "< " + to_string(bins[i+1]));
		yields_n_errs[i] = doFit(conditions, MuonId, path_bins_fit_folder);

	}

	//Path where is going to save efficiency 
	string directoryToSave = string("results/efficiencies/") + output_folder_name + string("/");
	create_folder(directoryToSave.c_str());

	//Create file
	string file_path = directoryToSave + prefix_file_name + quantity + "_" + MuonId + ".root";
	TFile* generatedFile = new TFile(file_path.c_str(),"recreate");
	
	//Create histograms
	generatedFile->mkdir("histograms/");
	generatedFile->   cd("histograms/");
	TH1D *yield_all  = make_TH1D("ALL" , yields_n_errs, 0, bins, nbins, quantity);
	TH1D *yield_pass = make_TH1D("PASS", yields_n_errs, 1, bins, nbins, quantity);
	
	//Create efficiencies
	generatedFile->   cd("/");
	get_efficiency(yield_all, yield_pass, quantity, MuonId, "", true);

	//Write file
	generatedFile->Write();

	cout << "\n[Settings]\n";
	cout << output_folder_name << "\n"<< quantity << " " << MuonId << "\n";
	cout << "Fitting:     " << fit_functions << "\n";
	cout << "Fit between: " << _mmin << " and " << _mmax << " GeV\n";
	cout << "Bins:        " << fit_bins << "\n";

	cout << "\n------------------------\n";
	cout << "Output: " << file_path;
	cout << "\n------------------------\n";
}