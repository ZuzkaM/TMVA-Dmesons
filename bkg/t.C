#define t_cxx
#include "t.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TNtuple.h>
#include <TMath.h>
#include "TLegend.h"
#include "TColor.h"

void t::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L t.C
//      Root > t t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   TFile *f = new TFile("histo_bkg2.root","RECREATE");

   int ptB = 4; // # of pt bins
   int nvar = 12;

   TH1D *his[4][12];
   TCanvas* c_p[12];
   TLegend *leg[4][12];
   
   gStyle->SetOptStat(0);

   for(int i = 0; i < ptB; i++)
   {
      TString histname = "histogram";
      histname += i;
      his[i][0] = new TH1D(histname + "cen", "centrality;centrality class", 9, 0, 9); 
      his[i][1] = new TH1D(histname + "Kpt", "p_T of K; p_t [GeV]", 200, 0, 10); 
      his[i][2] = new TH1D(histname + "pi1pt", "p_T of pi 1; p_t [GeV]", 200, 0, 10); 
      his[i][3] = new TH1D(histname + "pi2pt", "p_T of pi 2; p_t [GeV]", 200, 0, 10); 
      his[i][4] = new TH1D(histname + "DCA_K", "DCA of K; DCA [#\mum]", 3000, 0, 3000); 
      his[i][5] = new TH1D(histname + "DCA_pi1", "DCA of pi 1; DCA [#\mum]", 3000, 0, 3000); 
      his[i][6] = new TH1D(histname + "DCA_pi2", "DCA of pi 2; DCA [#\mum]", 3000, 0, 3000); 
      his[i][7] = new TH1D(histname + "DCAd", "DCA of daughters; DCA [#\mum]", 8000, 0, 8000); 
      his[i][8] = new TH1D(histname + "cosTheta", "cos(#theta)", 2001, -1.1, 1.1); 
      his[i][9] = new TH1D(histname + "decayLength", "Decay Length; d [#\mum]", 30000, 0, 30000);
      his[i][10] = new TH1D(histname + "maxd", "Max distance of pair vertices; [#\mum]", 20000, 0, 20000);
      his[i][11] = new TH1D(histname + "D_mass", "D meson mass; [MeV]", 20000, 0, 10);
   }


   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
	   if(jentry%(nentries/100)==0) std::cout<<100*jentry/(nentries)<<"%"<<std::endl;
      float var[12];


      var[0] = centrality;
      var[1] = k_pt;
      var[2] = pi1_pt;
      var[3] = pi2_pt;
      var[4] = 10000*k_dca;
      var[5] = 10000*pi1_dca;
      var[6] = 10000*pi2_dca;
      var[7] = 10000*dcaMax;
      var[8] = TMath::Cos(D_theta);
      var[9] = 10000*D_decayL;
      var[10] = 10000*D_dV0Max;
      var[11] = D_mass;
      
      //if((flag > 1) && (primVz < 6))
              // ntp_DMeson->Fill(var);

      for(int i = 0; i < 12; i++) {
      if(D_pt <= 1)
         his[0][i]->Fill(var[i]);
      else if(D_pt <= 2)
         his[1][i]->Fill(var[i]);
      else if (D_pt <= 8) 
         his[2][i]->Fill(var[i]);
      else
         his[3][i]->Fill(var[i]);
}

   }
 
   
   TString canname = "SimCan";
   c_p[0] = new TCanvas(canname + "cen", "Simulations",1000,400);
   c_p[1] = new TCanvas(canname + "Kpt", "Simulations",1000,400);
   c_p[2] = new TCanvas(canname + "pi1pt", "Simulations",1000,400);
   c_p[3] = new TCanvas(canname + "pi2pt", "Simulations",1000,400);
   c_p[4] = new TCanvas(canname + "DCA_K", "Simulations",1000,400);
   c_p[5] = new TCanvas(canname + "DCA_pi1", "Simulations",1000,400);
   c_p[6] = new TCanvas(canname + "DCA_pi2", "Simulations",1000,400);
   c_p[7] = new TCanvas(canname + "DCAd", "Simulations",1000,400);
   c_p[8] = new TCanvas(canname + "cosTheta", "Simulations",1000,400);
   c_p[9] = new TCanvas(canname + "decayLength", "Simulations",1000,400);
   c_p[10] = new TCanvas(canname + "maxd", "Simulations",1000,400);
   c_p[11] = new TCanvas(canname + "Dmass", "Simulations",1000,400);

   for (int i = 0; i < 12; i++)
      c_p[i]->Divide(2,2);




for(int j = 0; j < 12; j++)  
{
 for(int i = 0; i < ptB; i++)
   {
      c_p[j]->cd(i+1);
      if(j > 0)
         gPad->SetLogy();
      his[i][j]->SetFillColor(kRed);
      his[i][j]->SetFillStyle(3005);
      his[i][j]->SetStats(0);
      his[i][j]->Draw();
      leg[i][j] = new TLegend(0.6, 0.7, 0.9, 0.9);
      leg[i][j]->AddEntry(his[0][j], "Bkg from data", "f");
      leg[i][j]->Draw();
      f->cd();
      his[i][j]->Write();
      
   }  
   f->cd();
   c_p[j]->Write(); 
}

   f->Close();
}
