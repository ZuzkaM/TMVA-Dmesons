#define t_cxx
#include "t.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLegend.h"
#include <iostream>
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

   TFile *f = new TFile("unicorn.root","RECREATE");

   int ptB = 4; // # of pt bins
   int nvar = 11;
   //float bins[5] = {0,1,2,8,20};

   TH1F *his[4][11]; //nechce mi to zobrať his[ptB][nvar]
   TCanvas* c_p[11];
   TLegend *leg[4][11];
	
   gStyle->SetOptStat(0);

   for(int i = 0; i < ptB; i++)
   {
      TString histname = "histogram";
      histname += i;
      his[i][0] = new TH1F(histname + "cen", "centrality;centrality class", 20, 0, 9); //centrality class
      his[i][1] = new TH1F(histname + "Kpt", "p_T of K; p_t [GeV]", 200, 0, 10); //GeV
      his[i][2] = new TH1F(histname + "pi1pt", "p_T of pi 1; p_t [GeV]", 200, 0, 10); //GeV
      his[i][3] = new TH1F(histname + "pi2pt", "p_T of pi 2; p_t [GeV]", 200, 0, 10); //GeV
      his[i][4] = new TH1F(histname + "DCA_K", "DCA of K; DCA [#\mum]", 3000, 0, 3000); //mikrometer
      his[i][5] = new TH1F(histname + "DCA_pi1", "DCA of pi 1; DCA [#\mum]", 3000, 0, 3000); //mikrometer
      his[i][6] = new TH1F(histname + "DCA_pi2", "DCA of pi 2; DCA [#\mum]", 3000, 0, 3000); //mikrometer
      his[i][7] = new TH1F(histname + "DCAd", "DCA of daughters; DCA [#\mum]", 8000, 0, 8000); //mikrometer
      his[i][8] = new TH1F(histname + "cosTheta", "cos(#theta)", 2001, -1.1, 1.1); 
      his[i][9] = new TH1F(histname + "decayLength", "Decay Length; d [#\mum]", 30000, 0, 30000); //mikrometre
      his[i][10] = new TH1F(histname + "maxd", "Max distance of pair vertices; [#\mum]", 20000, 0, 20000); //mikrometre
   }

   
   //pt D cka

   cout << "unicorn";

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   float var[11];
      var[0] = cent;
      var[1] = kRPt;
      var[2] = p1RPt;
      var[3] = p2RPt;
      var[4] = kRDca;
      var[5] = p1RDca;
      var[6] = p2RDca;
      var[7] = dcaDaughters;
      var[8] = cosTheta;
      var[9] = decayLength;
      var[10] = mdV0Max;
   

     
     
    for(int i = 0; i < 11; i++) {
      if(pt <= 1)
         his[0][i]->Fill(var[i]);
      else if(pt <= 2)
         his[1][i]->Fill(var[i]);
      else if (pt <= 8) 
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


   for (int i = 0; i < 11; i++)
      c_p[i]->Divide(2,2);




for(int j = 0; j < 11; j++)  
{
 for(int i = 0; i < ptB; i++)
   {
      c_p[j]->cd(i+1);
      if(j > 0)
         gPad->SetLogy();
      his[i][j]->SetFillColor(kBlue+1);
      his[i][j]->SetFillStyle(3004);
      //his[i][j]->SetStats(0);
      his[i][j]->Draw();
      leg[i][j] = new TLegend(0.6, 0.7, 0.9, 0.9);
      leg[i][j]->AddEntry(his[0][j], "Simulation", "f");
      leg[i][j]->Draw();
      f->cd();
      his[i][j]->Write();
      
   }  
   f->cd();
   c_p[j]->Write(); 
}

   f->Close();
}
