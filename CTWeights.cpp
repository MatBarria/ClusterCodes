// This code apply the two acceptance factors to the half the of the simulation to do a closure test
// generate a PhiPq histogram for each bin of Q2, Nu, Zh, Pt2
// The hadrons variables used were calculate taking the vectorial
// sum of the hadrons momentum and using it as hadron momentum for the event
// For simultion use the tuple generate by the code VecSumSimul.cpp
// The simulation are splited in two by groups by the program SlipHalf.cpp
// It can be compile with
// g++ -Wall -fPIC -I./include `root-config --cflags` CTWeights.cpp -o ../bin/CTWeights `root-config --glibs` ./include/Acc.h
// For the target name use (C, Fe, Pb, D)

#include "Acc.h"

int main(int argc, char* argv[]) { 

    Pt2_BINS[0] = 0.;
    for(int i = 1; i < N_Pt2; i++) {
        Pt2_BINS[i] = Pt2_BINS[i-1] + Delta_Pt2;
    }

    if(argc != 4) {
        std::cout << "Incorrect number of arguments" << std::endl;  
        return 1;
    } 

    TStopwatch t;

    std::cout << "Start" << std::endl;

    std::string target = argv[1];
    int n = target.length();
    char targetArr[n + 1];
    strcpy(targetArr, target.c_str());	

    int Q2Bin = (int)*argv[2] - 48;
    int NuBin = (int)*argv[3] - 48;

    //outputDirectory = outputDirectory + "Systematic/CT/";

    TFile* fileData   = new TFile(Form(dataDirectory + "SimulTuple_%s_2.root", targetArr), "READ");
    TFile* fileSimul  = new TFile(Form(dataDirectory + "SimulTuple_%s_1.root", targetArr), "READ");
    TFile* fileOutput = new TFile(Form(outputDirectory + "Weights_%s_%i%i.root", targetArr, Q2Bin, NuBin), "RECREATE");
    gROOT->cd();

    // Create some variables to use inside the for loops
    TString tupleDataName;
    TCut Q2CutGen, NuCutGen, ZhCutGen, Pt2CutGen, cutsGen, GenCut, RecCut, GenDecCut;
    TCut Q2CutRec, NuCutRec, ZhCutRec, Pt2CutRec, cutsRec;
    // Select liquid or solid target
    // Create all the necessary histogram
    TH1F* histDetected    = new TH1F("Detected",     "", N_Phi, -180, 180);
    TH1F* histTotDetected = new TH1F("TotDetected",  "", N_Phi, -180, 180);
    TH1F* histThrown      = new TH1F("Thrown",       "", N_Phi, -180, 180);
    TH1F* histDataRec     = new TH1F("DataRec",      "", N_Phi, -180, 180);
    TH1F* histDataGen     = new TH1F("DataGen",      "", N_Phi, -180, 180);
    TH1F* histFalPos      = new TH1F("FalPosFactor", "", N_Phi, -180, 180);
    TH1F* histAccFactors  = new TH1F("AccFactor",    "", N_Phi, -180, 180);
    TH1F* histDataCorr    = new TH1F("DataCorr",     "", N_Phi, -180, 180);
    TH1F* histDataCorr2   = new TH1F("DataCorr2",    "", N_Phi, -180, 180);
    TH1F* histWeight      = new TH1F("Weights",      "", N_Phi, -180, 180);

    // Store the sum of the weights A.K.A the erros (in the other histograms if save it by other methods)
    histDataRec->Sumw2();
    histThrown->Sumw2();
    histTotDetected->Sumw2();
    histDetected->Sumw2();

    for(int gen = 1; gen <= N_PION ; gen++) { // Loops in every number of generated pions

        GenCut    = Form("Gen == %f", (float)gen);
        RecCut    = Form("Rec == %f", (float)gen);
        GenDecCut = GenCut||RecCut;

        for(int ZhCounter = 0; ZhCounter < N_Zh; ZhCounter++) { // Loops in every Zh bin

            std::cout << "Bin selected: " << gen << Q2Bin << NuBin << ZhCounter << std::endl;

            // Select the cuts for each bin
            Q2CutGen   = Form("Q2_gen>%f&&Q2_gen<%f", Q2_BINS[Q2Bin], Q2_BINS[Q2Bin+1]);
            NuCutGen   = Form("Nu_gen>%f&&Nu_gen<%f", Nu_BINS[NuBin], Nu_BINS[NuBin+1]);
            Q2CutRec   = Form("Q2_rec>%f&&Q2_rec<%f", Q2_BINS[Q2Bin], Q2_BINS[Q2Bin+1]);
            NuCutRec   = Form("Nu_rec>%f&&Nu_rec<%f", Nu_BINS[NuBin], Nu_BINS[NuBin+1]);
            ZhCutGen   = Form("Zh_gen>%f&&Zh_gen<%f", Zh_BINS[ZhCounter], Zh_BINS[ZhCounter+1]);
            ZhCutRec   = Form("Zh_rec>%f&&Zh_rec<%f", Zh_BINS[ZhCounter], Zh_BINS[ZhCounter+1]);

            cutsGen  = Q2CutGen&&NuCutGen&&ZhCutGen&&GenCut;
            cutsRec  = Q2CutRec&&NuCutRec&&ZhCutRec&&RecCut;


            TNtuple* ntupleDataGen  = (TNtuple*) fileData->Get("ntuple_sim_gen");
            TNtuple* ntupleDataRec  = (TNtuple*) fileData->Get("ntuple_sim_rec");
            TNtuple* ntupleSimulGen = (TNtuple*) fileSimul->Get("ntuple_sim_gen");
            TNtuple* ntupleSimulRec = (TNtuple*) fileSimul->Get("ntuple_sim_rec");

            // Apply the cuts to the ntuples to increces the efficiency
            ntupleDataGen->Draw(">>listDataGen", cutsGen);
            ntupleDataRec->Draw(">>listDataRec", cutsRec);
            ntupleSimulGen->Draw(">>listSimulGen", cutsGen);
            ntupleSimulRec->Draw(">>listSimulRec", cutsRec);
            TEventList* evntDataRec  = (TEventList*) gDirectory->Get("listDataRec");
            TEventList* evntDataGen  = (TEventList*) gDirectory->Get("listDataGen");
            TEventList* evntSimulRec = (TEventList*) gDirectory->Get("listSimulRec");
            TEventList* evntSimulGen = (TEventList*) gDirectory->Get("listSimulGen");
            ntupleDataRec->SetEventList(evntDataRec);
            ntupleDataGen->SetEventList(evntDataGen);
            ntupleSimulRec->SetEventList(evntSimulRec);
            ntupleSimulGen->SetEventList(evntSimulGen);

            for(int Pt2Counter = 0; Pt2Counter < N_Pt2; Pt2Counter++) { // Loops in every Pt2 bin

                // Select the Pt2 bin
                Pt2CutGen = Form("Pt2_gen>%f&&Pt2_gen<%f", Pt2_BINS[Pt2Counter], 
                                    Pt2_BINS[Pt2Counter+1]);
                Pt2CutRec = Form("Pt2_rec>%f&&Pt2_rec<%f", Pt2_BINS[Pt2Counter], 
                                    Pt2_BINS[Pt2Counter+1]);

                ntupleDataRec->Project("DataRec", "PhiPQ_rec", Pt2CutRec);
                ntupleDataGen->Project("DataGen", "PhiPQ_gen", Pt2CutGen);
                if(EmptyHist(histDataRec) == 1){ continue; } // If there isn't any event in data skip this bin
                ntupleSimulRec->Project("Detected",    "PhiPQ_rec", Pt2CutRec&&GenCut);
                ntupleSimulGen->Project("Thrown",      "PhiPQ_gen", Pt2CutGen);
                ntupleSimulRec->Project("TotDetected", "PhiPQ_rec", Pt2CutRec);

                // Calculate the Acceptance factor
                histAccFactors->Divide(histDetected, histThrown, 1, 1, "B");
                histFalPos->Divide(histDetected, histTotDetected, 1, 1, "B");
                // Check that the acceptance factors are smaller than one
                // Apply the correction factors
                histDataCorr->Divide(histDataRec, histAccFactors, 1, 1);
                histDataCorr2->Multiply(histDataCorr, histFalPos, 1, 1);

                histWeight->Divide(histDataCorr2, histDataGen, 1, 1);
                // Save the histograms in the output file

                fileOutput->cd();

                if(targetArr[0] != 'D') { 
                    histDataCorr2->Write(Form("DataCorr2_%s_%i%i%i%i_%i",  targetArr, Q2Bin, 
                                NuBin, ZhCounter, Pt2Counter, gen));
                    histDataGen->Write(Form("DataGen_%s_%i%i%i%i_%i",    targetArr, Q2Bin,
                                NuBin, ZhCounter, Pt2Counter, gen));
                    histWeight->Write(Form("Weight_%s_%i%i%i%i_%i",      targetArr, Q2Bin,
                                NuBin, ZhCounter, Pt2Counter, gen));
                    // Save the histograms in the output file
                } else {
                    histDataCorr2->Write(Form("DataCorr2_%sC_%i%i%i%i_%i",  targetArr, Q2Bin,
                                NuBin, ZhCounter, Pt2Counter, gen));
                    histDataGen->Write(Form("DataGen_%sC_%i%i%i%i_%i",    targetArr, Q2Bin,
                                NuBin, ZhCounter, Pt2Counter, gen));
                    histWeight->Write(Form("Weight_%sC_%i%i%i%i_%i",      targetArr, Q2Bin,
                                NuBin, ZhCounter, Pt2Counter, gen));
                    histDataCorr2->Write(Form("DataCorr2_%sFe_%i%i%i%i_%i", targetArr, Q2Bin,
                                NuBin, ZhCounter, Pt2Counter, gen));
                    histDataGen->Write(Form("DataGen_%sFe_%i%i%i%i_%i",   targetArr, Q2Bin,
                                NuBin, ZhCounter, Pt2Counter, gen));
                    histWeight->Write(Form("Weight_%sFe_%i%i%i%i_%i",     targetArr, Q2Bin,
                                NuBin, ZhCounter, Pt2Counter, gen));
                    histDataCorr2->Write(Form("DataCorr2_%sPb_%i%i%i%i_%i", targetArr, Q2Bin,
                                NuBin, ZhCounter, Pt2Counter, gen));
                    histDataGen->Write(Form("DataGen_%sPb_%i%i%i%i_%i",   targetArr, Q2Bin,
                                NuBin, ZhCounter, Pt2Counter, gen));
                    histWeight->Write(Form("Weight_%sPb_%i%i%i%i_%i",     targetArr, Q2Bin,
                                NuBin, ZhCounter, Pt2Counter, gen));
                }

                gROOT->cd();

                // Set the histograms values to 0
                histDataRec->Reset();
                histDataGen->Reset();
                histDataCorr2->Reset();
                histDataCorr->Reset();
                histFalPos->Reset();
                histAccFactors->Reset();
                histThrown->Reset();
                histDetected->Reset();
                histTotDetected->Reset();

            } // End Pt2 loop

            delete ntupleDataRec;
            delete ntupleDataGen;
            delete ntupleSimulRec;
            delete ntupleSimulGen;
            delete evntDataRec;
            delete evntSimulRec;
            delete evntSimulGen;

        } // End Zh loop
    } // End pion number loop

    fileData->Close();
    fileSimul->Close();
    fileOutput->Close();
    t.Print();
    delete histDetected    ;
    delete histTotDetected ;
    delete histThrown      ;
    delete histDataRec     ;
    delete histDataGen     ;
    delete histFalPos      ;
    delete histAccFactors  ;
    delete histDataCorr    ;
    delete histDataCorr2   ;
    return 0;

}
