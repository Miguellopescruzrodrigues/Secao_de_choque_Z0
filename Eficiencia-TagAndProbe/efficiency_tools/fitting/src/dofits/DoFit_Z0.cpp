using namespace RooFit;
using namespace std;

//We start by declaring the nature of our dataset. (Is the data real or simulated?)
const char* output_folder_name = "Z0_Run_2011";

//Header of this function
double _mmin = 60;
double _mmax = 120;
double fit_bins = 0;

// Information for output at the end of run
const char* fit_functions = "BW + CrystalBall + Exponnecial";
string prefix_file_name = "";

double* doFit(string condition, string MuonId, const char* savePath = NULL) // RETURNS ARRAY WITH [yield_all, yield_pass, err_all, err_pass]
{
	string MuonId_str = "";
	if      (MuonId == "trackerMuon")    MuonId_str = "PassingProbeTrackingMuon";
	else if (MuonId == "standaloneMuon") MuonId_str = "PassingProbeStandAloneMuon";
	else if (MuonId == "globalMuon")     MuonId_str = "PassingProbeGlobalMuon";
	
	TFile *file0       = TFile::Open("DATA/TPSingleMu2011AOD.root");
	TTree *DataTree    = (TTree*)file0->Get(("tagandprobe"));
	
	RooCategory MuonId_var(MuonId_str.c_str(), MuonId_str.c_str());
	MuonId_var.defineType("All");
	MuonId_var.defineType("PASSING");
	RooRealVar  InvariantMass("InvariantMass", "InvariantMass", _mmin, _mmax);
	RooRealVar  quantityPt   ("ProbeMuon_Pt",  "ProbeMuon_Pt",  15.0, 110.);
	RooRealVar  quantityEta  ("ProbeMuon_Eta", "ProbeMuon_Eta", -2.4, 2.4);
	RooRealVar  quantityPhi  ("ProbeMuon_Phi", "ProbeMuon_Phi", -TMath::Pi(), TMath::Pi());

	if (fit_bins > 0) InvariantMass.setBins(fit_bins);
	fit_bins = InvariantMass.getBinning().numBins();
	RooFormulaVar* redeuce   = new RooFormulaVar("PPTM_cond", condition.c_str(), RooArgList(quantityPt, quantityEta, quantityPhi));
	RooDataSet *Data_ALL     = new RooDataSet("DATA", "DATA", DataTree, RooArgSet(InvariantMass, MuonId_var, quantityPt, quantityEta, quantityPhi),*redeuce);

	RooFormulaVar* cutvar    = new RooFormulaVar("PPTM_mounid", (MuonId_str + "==1").c_str(), RooArgList(MuonId_var));
	RooDataSet *Data_PASSING = new RooDataSet("DATA_PASS", "DATA_PASS", Data_ALL, RooArgSet(InvariantMass, MuonId_var, quantityPt, quantityEta, quantityPhi), *cutvar);
	
	RooDataHist* dh_ALL     = new RooDataHist(Data_ALL->GetName(),    Data_ALL->GetTitle(),     RooArgSet(InvariantMass), *Data_ALL);
	RooDataHist* dh_PASSING = new RooDataHist(Data_PASSING->GetName(),Data_PASSING->GetTitle(), RooArgSet(InvariantMass), *Data_PASSING);
	
	TCanvas* c_all  = new TCanvas;
	TCanvas* c_pass = new TCanvas;
	
	RooPlot *frame = InvariantMass.frame(RooFit::Title("Invariant Mass"));


//====================================AJUSTE COM AS FUNÇÕES=========================
	   
	// Variaveis gerais
	RooRealVar mean("mean","mean",91.1976, 80., 100.); //Z0 mass
	RooRealVar sigma("sigma","sigma", 4.12, 0.1, 100.0);

	// VOIGTIAN VARIABLES
	RooRealVar width("width","width",2.45);

	//CRYSTALBALL VARIABLES
	RooRealVar sigma_cb("sigma_cb","sigma_cb", 2.0, 1.0, 40.0);
	RooRealVar alpha("alpha", "alpha", 3.3, 0., 5.);
	RooRealVar n("n", "n",  5.1, 0., 10.);

	//BreitWigner VARIABLES
	RooRealVar largura("largura","largura", 2.4952);

	//BACKGROUND VARIABLES
    RooRealVar a0("a0", "a0", 0, -10, 100);
    RooRealVar a1("a1", "a1", 0, -10, 100);
    RooRealVar a("a", "a", 0.,-20.0, 20.0);

	//FIT FUNCTIONS

	RooCBShape crystalball("CBS", "CBS", InvariantMass, mean, sigma_cb, alpha, n); //-----------------------------------

	RooBreitWigner BW("BW","BW",InvariantMass,mean,sigma); //----------------------------------

	RooExponential background("BG", "BG", InvariantMass, a);

	double n_signal_initial_total = 80000;
	double n_back_initial = 10000;
	RooRealVar frac1("frac1","frac1",0.5);



	RooAddPdf* signal;
	signal      = new RooAddPdf("signal", "signal", RooArgList(crystalball, BW), RooArgList(frac1));

	RooRealVar n_signal_total("n_signal_total","n_signal_total",n_signal_initial_total,0.,Data_ALL->sumEntries());
	RooRealVar n_signal_total_pass("n_signal_total_pass","n_signal_total_pass",n_signal_initial_total,0.,Data_PASSING->sumEntries());
	
	RooRealVar n_back("n_back","n_back",n_back_initial,0.,Data_ALL->sumEntries());
	RooRealVar n_back_pass("n_back_pass","n_back_pass",n_back_initial,0.,Data_PASSING->sumEntries());
	RooAddPdf* model;
	RooAddPdf* model_pass;
	
	model      = new RooAddPdf("model","model", RooArgList(*signal, background),RooArgList(n_signal_total, n_back));
	model_pass = new RooAddPdf("model_pass", "model_pass", RooArgList(*signal, background),RooArgList(n_signal_total_pass, n_back_pass));


	// SIMULTANEOUS FIT
	RooCategory sample("sample","sample") ;
	sample.defineType("All") ;
	sample.defineType("PASSING") ;
	RooDataHist combData("combData","combined data",InvariantMass,Index(sample),Import("ALL",*dh_ALL),Import("PASSING",*dh_PASSING));
	
	RooSimultaneous simPdf("simPdf","simultaneous pdf",sample) ;
	
	simPdf.addPdf(*model,"ALL");
	simPdf.addPdf(*model_pass,"PASSING");
	
	RooFitResult* fitres = new RooFitResult;
	fitres = simPdf.fitTo(combData, RooFit::Save());



RooAbsReal *integral_bkg = background.createIntegral(InvariantMass);
Double_t integral_bkg_value = integral_bkg->getVal();
//Double_t integral_bkg_value_error = integral_bkg->getPropagatedError(*fit_result_data, x);


	//legenda
	TH1F *orange = new TH1F("h1","Ex",1,-10,10);
	TH1F *green = new TH1F("h2","Ex",1,-10,10);
	TH1F *red = new TH1F("h3","Ex",1,-10,10);
	TH1F *magenta = new TH1F("h4","Ex",1,-10,10);
	TH1F *blue = new TH1F("h5","Ex",1,-10,10);
	TH1F *Teal = new TH1F("h5","Ex",1,-10,10);
	orange->SetLineColor(kOrange);
	green->SetLineColor(kGreen);
	red->SetLineColor(kRed);
	magenta->SetLineColor(kMagenta);
	blue->SetLineColor(kBlue);
	Teal->SetLineColor(kTeal);
	Teal->SetLineStyle(2);
	orange->SetLineStyle(2);
	green->SetLineStyle(2);
	red->SetLineStyle(2);
	magenta->SetLineStyle(2);
	
	// OUTPUT ARRAY
	double* output = new double[4];
	
	RooRealVar* yield_ALL = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total");
	RooRealVar* yield_PASS = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total_pass");
	
	output[0] = yield_ALL->getVal();
	output[1] = yield_PASS->getVal();
	
	output[2] = yield_ALL->getError();
	output[3] = yield_PASS->getError();
	
	frame->SetTitle("Probe");
	frame->SetYTitle("Eventos");
	frame->SetXTitle("#mu^{+}#mu^{-} Massa invariante [GeV/c^{2}]");
	Data_ALL->plotOn(frame);


	model->plotOn(frame);
	double chis = frame->chiSquare();
	model->plotOn(frame,RooFit::Components("signal"),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
	model->plotOn(frame,RooFit::Components("BG"),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
	model->plotOn(frame,RooFit::Components("VT"),RooFit::LineStyle(kDashed),RooFit::LineColor(kOrange));
	model->plotOn(frame,RooFit::Components("VT2"),RooFit::LineStyle(kDashed),RooFit::LineColor(kTeal));
	model->plotOn(frame,RooFit::Components("CBS"),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta));
	model->plotOn(frame,RooFit::Components("BW"),RooFit::LineStyle(kDashed),RooFit::LineColor(kTeal));
	
	
	c_all->cd();
	frame->Draw("");
	//CREATING LEGEND
	TLegend *leg = new TLegend(0.3,0.6,0.1,0.9);
	leg->AddEntry(blue,"Ajuste Total","l");
	leg->AddEntry(green,"Sinal","l");
	leg->AddEntry(Teal,"BreitWigner","l");
	leg->AddEntry(magenta,"Crystalball","l");
	leg->AddEntry(red,"Fundo","l");
	leg->AddEntry("frame",(Form("#Chi^{2}: %2.5f", chis)),"" );
	leg->AddEntry("frame_pass",(Form("Entradas: %f", dh_ALL->sumEntries())),"" );
	leg->AddEntry("frame_pass",(Form("Fundo: %f", integral_bkg->getVal())),"" );
	leg->Draw();

	RooPlot *frame_pass = InvariantMass.frame(RooFit::Title("Invariant Mass"));
	
	c_pass->cd();
	
	frame_pass->SetTitle("Tag");
	frame_pass->SetYTitle("Eventos");
	frame_pass->SetXTitle("#mu^{+}#mu^{-} Massa invariante [GeV/c^{2}]");
	Data_PASSING->plotOn(frame_pass);
	
	model_pass->plotOn(frame_pass);
	double chis_pass = frame_pass->chiSquare();
	model_pass->plotOn(frame_pass,RooFit::Components("signal"),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
	model_pass->plotOn(frame_pass,RooFit::Components("BG"),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
	model_pass->plotOn(frame_pass,RooFit::Components("VT"),RooFit::LineStyle(kDashed),RooFit::LineColor(kOrange));
	model_pass->plotOn(frame_pass,RooFit::Components("VT2"),RooFit::LineStyle(kDashed),RooFit::LineColor(kTeal));
	model_pass->plotOn(frame_pass,RooFit::Components("CBS"),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta));
	model_pass->plotOn(frame_pass,RooFit::Components("BW"),RooFit::LineStyle(kDashed),RooFit::LineColor(kTeal));
	

	TLegend *leg2 = new TLegend(0.3,0.6,0.1,0.9);
	leg2->AddEntry(blue,"Ajuste Total","l");
	leg2->AddEntry(green,"Signal","l");
	leg2->AddEntry(Teal,"BreitWigner","l");
	leg2->AddEntry(magenta,"Crystalball","l");
	leg2->AddEntry(red,"Background","l");
	leg2->AddEntry("frame_pass",(Form("#Chi^{2}: %2.5f", chis_pass)),"" );
	leg2->AddEntry("frame_pass",(Form("Entradas: %f", dh_PASSING->sumEntries())),"" );
	leg2->AddEntry("frame_pass",(Form("Entradas: %f", integral_bkg->getVal())),"" );

	leg2->Draw();

	frame_pass->Draw();
		//CREATING LEGEND
	leg2->Draw();


	ofstream fout("chiSquare.txt", ios::app); 
	fout << "chiSquare " + condition + "_All = " + chis << endl;
	fout << "chiSquare " + condition + "_PASS = " + chis_pass << endl;

	if (savePath != NULL)

	{
		c_pass->SaveAs((string(savePath) + condition + "_PASS.png").c_str());
		c_all->SaveAs ((string(savePath) + condition + "_ALL.png").c_str());
		c_pass->SaveAs((string(savePath) + condition + "_PASS.root").c_str());
		c_all->SaveAs ((string(savePath) + condition + "_ALL.root").c_str());
	}




	// DELETING ALLOCATED MEMORY*/
	delete file0;
	//
	delete Data_ALL;
	delete Data_PASSING;
	//
	delete dh_ALL;
	delete dh_PASSING;
	//
	delete cutvar;
	delete redeuce;
	//
	delete signal;
	//
	delete c_all;
	delete c_pass;
	//
	delete model;
	delete model_pass;
	delete fitres;
	
	return output;
}

