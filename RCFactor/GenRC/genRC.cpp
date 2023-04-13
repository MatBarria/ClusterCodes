// This codes use HAPRAD and the histograms generated by PhiHist.cpp to calculate the RC factors
// Return the Rc facotors in Phi histograms, one for each 4Dim bin of Q2, Nu, Zh, Pt2
// Can be compiled using Make, but you need to compile HAPRAD before
// For the target name use (C,Fe,Pb) for the solids targets and (DC,DFe,DPb) for the liquid target

#include <fstream>
#include <iostream>
#include <string>
#include "TString.h"
#include "TRadCor.h"
#include "TFile.h"
#include "TROOT.h"
#include "TNtuple.h"
#include "TMath.h"
#include "TH1.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "haprad_constants.h"
#include "Binning_Rc.h"


int main(int argc, char* argv[]) {

    std::cout << "Pre Start" << std::endl;
    if(argc != 6) {
        std::cout << "Insert (just) the target name as a parameter" << std::endl;
        return 0;
    }

    Phi_BINS[0] = -180;
    for(int i = 1; i <= N_Phi; i++) {
        Phi_BINS[i] = Phi_BINS[i-1] + Delta_Phi;
    }
    TStopwatch t;

    std::cout << "Start" << std::endl;

    const TString inputDirectory  = "/work/mbarrial/Data/60/RCFactors/";
    const TString outputDirectory = "/work/mbarrial/Data/60/RCFactors/";

    std::string target = argv[1];
    int nPion = (int)*argv[2] - 48;
    int Q2BinSelect = (int)*argv[3] - 48;
    int NuBinSelect = (int)*argv[4] - 48;
    int ZhBinSelect = (int)*argv[5] - 48;
    //int ZhBinSelect = (int)*argv[5] - 48;
    std::cout << "N PION = " << nPion << std::endl;
    std::cout << "Q2 bin = " << Q2BinSelect << std::endl;
    std::cout << "Nu bin = " << NuBinSelect << std::endl;
    std::cout << "Zh bin = " << ZhBinSelect << std::endl;
    //std::cout << "Zh bin = " << NuBinSelect << std::endl;
    // Creating a array of chars instead of a string to use Form method
    int n = target.length();
    char targetArr[n + 1];
    strcpy(targetArr, target.c_str());

    Double_t f1;
    // Double_t f3;
    Double_t NAZ;
    Double_t m = TMath::Power((kMassNeutron + kMassPion), 2);
    //float Masa = 0.938; // Mass Nucleon (Proton)
    Double_t a1, a2, a3;
    //int sysReturn;

    TRadCor rc;
    a1 = 1;
    a2 = 0;
    a3 = a1/a2;

    if(target == "C")        NAZ = 0.5;
    else if(target == "Fe")  NAZ = 0.5;
    else if(target == "Pb")  NAZ = 82./208.;
    else if(target == "DC")  NAZ = 0.5;
    else if(target == "DFe") NAZ = 0.5;
    else if(target == "DPb") NAZ = 0.5; 
    else NAZ = 0.5;

    TH1F* histRcFactors  = new TH1F("RcFactors", "", N_Phi, -180, 180);

    TFile* fileBins = new TFile(Form(inputDirectory + "Centroid_%s.root", targetArr), "READ");
    TFile* fileHist = new TFile("newphihist.root", "READ");
    gROOT->cd();
    TNtuple* ntupleBins = (TNtuple*) fileBins->Get(Form("Centroid_%i", nPion));
    float Q2, Xb, Zh, Pt, Phi;
    float Q2Bin, NuBin, ZhBin, Pt2Bin;

    ntupleBins->SetBranchAddress("Q2", &Q2);
    ntupleBins->SetBranchAddress("Xb", &Xb);
    ntupleBins->SetBranchAddress("Zh", &Zh);
    ntupleBins->SetBranchAddress("Pt", &Pt);
    ntupleBins->SetBranchAddress("Q2Bin",  &Q2Bin);
    ntupleBins->SetBranchAddress("NuBin",  &NuBin);
    ntupleBins->SetBranchAddress("ZhBin",  &ZhBin);
    ntupleBins->SetBranchAddress("Pt2Bin", &Pt2Bin);


    gROOT->cd();
    TFile* outputFile = new TFile(outputDirectory + Form("RcFactors_%s_%i%i%i_%i.root", 
                        targetArr, (int)Q2BinSelect, (int)NuBinSelect, 
                        (int)ZhBinSelect , nPion), "RECREATE");
    gROOT->cd();
    for(int i = 0; i < ntupleBins->GetEntries(); i++) {

        ntupleBins->GetEntry(i);
        
        std::cout << "loop bin bin: " << " " << (int)Q2Bin << " " << (int)NuBin << " " 
                  << (int)ZhBin << " " << (int)Pt2Bin << std::endl;
        if((int)Q2Bin != Q2BinSelect || (int)NuBin != NuBinSelect ) { continue; } 
        if(ZhBin < 6.1 && (int)ZhBin != ZhBinSelect) { continue; }
        if(ZhBin > 5.9 && ZhBinSelect != 6) { continue; }

        TH1F *hist = (TH1F*) fileHist->Get(Form("PhiDist Q2=%.3f Xb=%.3f Zh=%.3f Pt=%.3f", 
                        Q2, Xb, Zh, Pt));

        if(hist == NULL) {
            delete hist;
            std::cout << "here Null for: " << " " << (int)Q2Bin << " " << (int)NuBin << " "
                      << (int)ZhBin << " " << (int)Pt2Bin << std::endl;
            continue;
        }
        delete hist;
        
        std::cout << "Current bin: " << " " << (int)Q2Bin << " " << (int)NuBin << " " 
                  << (int)ZhBin << " " << (int)Pt2Bin << std::endl;
        for(int PhiCounter = 0 ; PhiCounter < N_Phi ; PhiCounter++) { // Loops in every Phi bin

            Phi = (Phi_BINS[PhiCounter] + Phi_BINS[PhiCounter+1])/2;
            rc.CalculateRCFactor(5.015, Xb, Q2, Zh, Pt, Phi, m, NAZ);
            f1 = rc.GetFactor1();
            // f3 = rc.GetFactor3();
            if(TMath::IsNaN(f1) || f1 == a3) f1 = 0;
            // if(TMath::IsNaN(f3) || f3 == a3) f3 = 0;
            std::cout << "El Factor es: " << f1 << std::endl;
            histRcFactors->SetBinContent(PhiCounter + 1, f1);
        }// End Phi loop

        outputFile->cd();
        histRcFactors->Write(Form("RcFactor_%s_%i%i%i%i_%i", targetArr, (int)Q2Bin, (int)NuBin, 
                    (int)ZhBin, (int)Pt2Bin, nPion));
        gROOT->cd();
        histRcFactors->Reset();

    }
    delete ntupleBins;
    fileBins->Close();
    fileHist->Close();
    outputFile->Close();
    t.Print();
    return 0;
}

