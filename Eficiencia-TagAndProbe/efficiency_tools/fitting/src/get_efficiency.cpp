TEfficiency* get_efficiency(TH1D* all, TH1D* pass, string quantity, string MuonId, string prefix_name = "", bool shouldWrite = false)
{
	//Copy histograms to change axis titles later
	TH1D* pass_copy = (TH1D*)pass->Clone();
	TH1D* all_copy  = (TH1D*)all ->Clone();

	pass_copy->GetYaxis()->SetTitle("Eficiencia");
	all_copy ->GetYaxis()->SetTitle("Eficiencia");
	
	TEfficiency* pEff = new TEfficiency();
	pEff->SetPassedHistogram(*pass_copy, "f");
	pEff->SetTotalHistogram (*all_copy,  "f");

	delete all_copy;
	delete pass_copy;

	//Set plot config
	if (prefix_name != "")
	{
		pEff->SetName(string(MuonId + "_" + quantity + "_" + prefix_name + "_Efficiency").c_str());
		pEff->SetTitle(string("Eficiencia do " + MuonId + " " + quantity + " (" + prefix_name + ")").c_str());
	}
	else
	{
		pEff->SetName(string(MuonId + "_" + quantity + "_Efficiency").c_str());
		pEff->SetTitle(string("Eficiencia do " + MuonId + " " + quantity).c_str());
	}

	pEff->SetLineColor(kBlack);
	pEff->SetMarkerStyle(21);
	pEff->SetMarkerSize(0.5);
	pEff->SetMarkerColor(kBlack);
	
	if (shouldWrite)
		pEff->Write();
	
	TCanvas* oi = new TCanvas();
	oi->cd();
	pEff->Draw();
	
	gPad->Update();
	auto graph = pEff->GetPaintedGraph();
	graph->SetMinimum(0.6);
	graph->SetMaximum(1.0);
	gPad->Update();
	
	return pEff;
}
