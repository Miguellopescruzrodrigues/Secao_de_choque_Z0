#include <stdio.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <iostream>
using namespace std;
 
 void Massa_Invariante() {

	TLorentzVector A; 
	TLorentzVector B; 
	TLorentzVector C;
	double m;

   TFile *f = new TFile("output3.root");
   
   TTree *tree = (TTree*)f->Get("aod2nanoaod/Events");
   
   
   UInt_t           nMuon;
   Float_t         Muon_pt[5];   //[NMuon]
   Float_t         Muon_eta[5];   //[NMuon]
   Float_t         Muon_phi[5];   //[NMuon]
   Float_t         Muon_mass[5];   //[NMuon]
   Int_t           Muon_charge[5];   //[NMuon]
   //Float_t         Muon_Iso[5];   //[NMuon]
  

   // List of branches
   TBranch        *b_nMuon;   //!
   TBranch        *b_Muon_pt;   //!
   TBranch        *b_Muon_eta;   //!
   TBranch        *b_Muon_phi;   //!
   TBranch        *b_Muon_mass;   //!
   TBranch        *b_Muon_charge;   //!
   //TBranch        *b_Muon_Iso;   //!
   
   
   tree->SetBranchAddress("nMuon", &nMuon);
   tree->SetBranchAddress("Muon_pt", &Muon_pt);
   tree->SetBranchAddress("Muon_eta", &Muon_eta);
   tree->SetBranchAddress("Muon_phi", &Muon_phi);
   tree->SetBranchAddress("Muon_mass", &Muon_mass);
   tree->SetBranchAddress("Muon_charge", &Muon_charge);
   //tree->SetBranchAddress("Muon_Iso", &Muon_Iso);
   
   TH1F *h1 = new TH1F("h1", "Massa Invariante de 2Muons", 200, 0, 150);
   
   Int_t nentries= (Int_t)tree->GetEntries();
   for (Int_t i = 0; i<nentries; i++) {
   tree->GetEntry(i);
     if(nMuon==2 && /*Muon_Iso[0]< 0.05 && Muon_Iso[1]< 0.05 && */Muon_charge[0] != Muon_charge[1]){
       for (int m=0; m<nMuon;m++){
         if (m==0) A.SetPtEtaPhiM(Muon_pt[m], Muon_eta[m], Muon_phi[m], Muon_mass[m]);
         if (m==1) B.SetPtEtaPhiM(Muon_pt[m], Muon_eta[m], Muon_phi[m], Muon_mass[m]);
         C= A + B;
         m= C.M();
    //cout << (A.M())<<"  "<< (B.M()) <<endl;
    //cout << Muon_pt[0] <<"  "<< Muon_pt[1] << "  Eta "<< Muon_eta[0]<< "  "<< Muon_eta[1]<<endl;
    //cout << Muon_pt[0] <<"  "<< Muon_pt[1]<<" "<< Muon_pt[2]<<endl;
    //cout << (A.M())<<endl;
    //h1->Fill(m);
         h1->Fill((A + B).M());
      }
   } // if condicao  
   
 
 } //for
   
   h1->Draw(); 
   
}