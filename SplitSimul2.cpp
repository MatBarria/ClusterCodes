// It can be compile with
// g++ -Wall -fpic -I./include `root-config --cflags` SplitSimul2.cpp -o ./bin/SplitSimul `root-config --glibs` ./include/Binning.h
// For the target name use (D,C,Fe,Pb)

#include <iostream>
#include <string>
#include "TString.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TStopwatch.h"
#include "Binning.h"
#include "TROOT.h"

int main(int argc, char* argv[]) {

    if(argc != 2) {
        std::cout << "Insert (just) the target name and the run as a parameters" << std::endl;
        return 0;
    }

    TStopwatch t;

    // For the Target name use (D,C,Fe,Pb)
    std::string target = argv[1];
    // Creating a array of chars instead of a string to use Form method
    int n = target.length();
    char targetArr[n + 1];
    strcpy(targetArr, target.c_str());

    TFile *simFile = new TFile(Form(dataDirectory + "VecSumSimul_%s.root", targetArr), "READ");
    //TFile *simFile = new TFile(Form(dataDirectory + "SimulTuple_%s.root", targetArr), "READ");

    const char* VarListGen = "Gen:Rec:Q2_gen:Nu_gen:Zh_gen:Pt2_gen:PhiPQ_gen";
    const char* VarListRec = "Gen:Rec:Q2_rec:Nu_rec:Zh_rec:Pt2_rec:PhiPQ_rec";
    float *varsGen = new Float_t[7];
    float *varsRec = new Float_t[7];

    TNtuple *simulTupleGen = (TNtuple*) simFile->Get("ntuple_sim_gen");
    TNtuple *simulTupleRec = (TNtuple*) simFile->Get("ntuple_sim_rec");
    if(simulTupleGen == NULL || simulTupleRec == NULL) {std::cout <<"la ptm, que paso ahora\n";}

    simulTupleGen->SetBranchAddress("Gen"       , &varsGen[0]);
    simulTupleGen->SetBranchAddress("Rec"       , &varsGen[1]);
    simulTupleGen->SetBranchAddress("Q2_gen"    , &varsGen[2]);
    simulTupleGen->SetBranchAddress("Nu_gen"    , &varsGen[3]);
    simulTupleGen->SetBranchAddress("Zh_gen"    , &varsGen[4]);
    simulTupleGen->SetBranchAddress("Pt2_gen"   , &varsGen[5]);
    simulTupleGen->SetBranchAddress("PhiPQ_gen" , &varsGen[6]);
    simulTupleRec->SetBranchAddress("Gen"       , &varsRec[0]);
    simulTupleRec->SetBranchAddress("Rec"       , &varsRec[1]);
    simulTupleRec->SetBranchAddress("Q2_rec"    , &varsRec[2]);
    simulTupleRec->SetBranchAddress("Nu_rec"    , &varsRec[3]);
    simulTupleRec->SetBranchAddress("Zh_rec"    , &varsRec[4]);
    simulTupleRec->SetBranchAddress("Pt2_rec"   , &varsRec[5]);
    simulTupleRec->SetBranchAddress("PhiPQ_rec" , &varsRec[6]);

    TFile *SecondHalfFile = new TFile(dataDirectory + Form("SimulTuple_%s_2.root", targetArr)
                , "RECREATE");
    gROOT->cd();

    TNtuple *SHtupleGen = new TNtuple("SHntuple_sim_gen", "", VarListGen);
    TNtuple *SHtupleRec = new TNtuple("SHntuple_sim_rec", "", VarListRec);

    int limit = (int)(simulTupleRec->GetEntries()/2);
    std::cout << "Start from: " << limit << " until: " << 2*limit << std::endl;

    for(int i = limit; i < 2*limit ; i++) { 

        //std::cout << "Entry : " << (int)(simulTupleRec->GetEntries()/2)+i-1<< std::endl;

        simulTupleGen->GetEntry(i-1);
        simulTupleRec->GetEntry(i-1);
        //std::cout << "Q2: " << varsGen[2]<< std::endl;
        SHtupleGen->Fill(varsGen);
        SHtupleRec->Fill(varsRec);
    }

    SecondHalfFile->cd();
    SHtupleGen->Write("ntuple_sim_gen");
    SHtupleRec->Write("ntuple_sim_rec");
    gROOT->cd();

    delete simulTupleGen;
    delete simulTupleRec;
    SecondHalfFile->Close(); 
    simFile->Close();
    delete SHtupleGen;
    delete SHtupleRec;

    t.Print();

    return 0;

}
