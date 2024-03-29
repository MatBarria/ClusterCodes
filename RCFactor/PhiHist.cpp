// This program take the histogram generated by AccCorrection.cpp and apply a fit funcion to it
// It can be compile with
// g++ -Wall -fPIC -I./include `root-config --cflags` PhiHist.cpp -o ./bin/PhiHist `root-config --glibs` ./include/Acc_Rc.h
// For the target name use (C,Fe,Pb) for the solids targets and (DC,DFe,DPb) for the liquid target

#include <fstream>
#include <iostream>
#include <string>
#include "Acc_Rc.h"
#include "TString.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"


const TString inputDirectory  = "/work/mbarrial/Data/60/RCFactors/";
const TString outputDirectory = "/work/mbarrial/Data/60/RCFactors/";

int PhiPQ(std::string target);

int main(){

    std::cout << "PhiPQ C" << std::endl;
    PhiPQ("C");
    std::cout << "PhiPQ Fe" << std::endl;
    PhiPQ("Fe");
    std::cout << "PhiPQ Pb" << std::endl;
    PhiPQ("Pb");
    std::cout << "PhiPQ DC" << std::endl;
    PhiPQ("DC");
    std::cout << "PhiPQ DFe" << std::endl;
    PhiPQ("DFe");
    std::cout << "PhiPQ DPb" << std::endl;
    PhiPQ("DPb");

    return 0;
}


int PhiPQ(std::string target) {

    TStopwatch t;

    std::cout << "Start" << std::endl;
    int n = target.length();
    char targetArr[n + 1];
    strcpy(targetArr, target.c_str());

    float Q2, Xb, Zh, Pt;
    float Q2Bin, NuBin, ZhBin, Pt2Bin;
    float A, Ac, Acc;
    float AErr, AcErr, AccErr;
    float ChiSQ, ndf;
    //float Masa = 0.938; // Nucleon Mass (Proton)
    int empty;
    int minBins = 3; // Min number of bins no empty bins that the histogram must have
    //TFile* inputFile = new TFile(inputDirectory + "corr_data_Phi.root", "READ");
    TFile* inputFile = new TFile(inputDirectory + "corr_data_Phi_Evnt.root", "READ");
    TFile* binsFile  = new TFile(inputDirectory + Form("Centroid_%s.root", targetArr), "READ");
    TF1 *func = new TF1("fit", 
            "[0]+TMath::Cos(x*TMath::Pi()/180)*[1]+TMath::Cos(2*x*TMath::Pi()/180)*[2]");

    for(int nPion = 1; nPion <= N_PION; nPion++) { // Loops in every number of pion
        TFile* outputFile = new TFile(outputDirectory + Form("%snewphihist%i.root", targetArr,
                    nPion), 
                "RECREATE");
        gROOT->cd();
        TNtuple* newntuple = new TNtuple("AAcAcc_data", "AAcAcc_data", 
                "Q2:Xb:Zh:Pt:A:AErr:Ac:AcErr:Acc:AccErr:ChiSQ:ndf:GF");

        TNtuple *binsTuple = (TNtuple*) binsFile->Get(Form("Centroid_%i", nPion));


        binsTuple->SetBranchAddress("Q2",&Q2);
        binsTuple->SetBranchAddress("Xb",&Xb);
        binsTuple->SetBranchAddress("Zh",&Zh);
        binsTuple->SetBranchAddress("Pt",&Pt);
        binsTuple->SetBranchAddress("Q2Bin",&Q2Bin);
        binsTuple->SetBranchAddress("NuBin",&NuBin);
        binsTuple->SetBranchAddress("ZhBin",&ZhBin);
        binsTuple->SetBranchAddress("Pt2Bin",&Pt2Bin);

        for(int i = 0; i < binsTuple->GetEntries(); i++) {
            binsTuple->GetEntry(i);	
            //std::cout << "Bin selected: " << nPion << Q2Bin << NuBin << ZhBin << std::endl;
            TH1F* histPhi = (TH1F*) inputFile->Get(Form("DataCorr2_%s_%i%i%i%i_%i", targetArr,
                        (int)Q2Bin, (int)NuBin, (int)ZhBin, (int)Pt2Bin, (int)nPion));
            if(histPhi == NULL) { continue; }
            if(EmptyHist(histPhi) == 1) { continue; }
            empty = 0;
            // Check that there is enough not empty bins in the histogram
            for(int j = 1; j <= N_Phi; j++) {
                if(histPhi->GetBinContent(j) == 0) { empty++; }
            }
            if(empty <= (N_Phi - minBins)){

                histPhi->Fit(func, "q");
                outputFile->cd();
                histPhi->Write(Form("PhiDist Q2=%.3f Xb=%.3f Zh=%.3f Pt=%.3f", Q2, Xb, Zh, Pt));
                gROOT->cd();
                // Fit the funtion to the histogram
                if(func->GetNDF() != 0) {

                    ChiSQ  = func->GetChisquare();
                    ndf    = func->GetNDF();
                    A      = func->GetParameter(0);
                    AErr   = func->GetParError(0);
                    Ac     = func->GetParameter(1);
                    AcErr  = func->GetParError(1);
                    Acc    = func->GetParameter(2);
                    AccErr = func->GetParError(2);
                    // Save the constan in a tuple to be use to calculate the RCfactors
                    newntuple->Fill(Q2, Xb, Zh, Pt, A, AErr, Ac, AcErr, Acc, AccErr, ChiSQ, ndf, 
                            ChiSQ/ndf);
                }
            }
        }
        delete binsTuple;

        outputFile->cd();
        newntuple->Write();
        gROOT->cd();
        newntuple->Delete();
        outputFile->Close();
    }// End number pion event loop

    inputFile->Close();
    t.Print();
    return 0;

}
