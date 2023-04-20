// This code apply the two acceptance factors to the data and
// generate a PhiPq histogram for each bin of Q2, Nu, Zh, Pt2
// The hadrons variables used were calculate taking the vectorial
// sum of the hadrons momentum and using it as hadron momentum for the event
// For simultion use the tuple generate by the code VecSumSimul.cpp
// For data the tuple generate by the code VecSum.cpp
// It can be compile with
// g++ -Wall -fPIC -I./include `root-config --cflags` AccCorrection.cpp -o ./bin/AccCorrection `root-config --glibs` ./include/Acc.h
// For the target name use (C,Fe,Pb) for the solids targets and (DC,DFe,DPb) for the liquid target

#include "Acc.h"

int main(int argc, char* argv[]) { 
    
    // Set Pt2 Binning
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

    int m;
    TString fileDataName;
    // Select the data for the chosen solid target
    if(targetArr[0] == 'D') {
        m = 2;
        char solidTarget[n];
        for(int i = 0; i < n; i++){
            solidTarget[i] = targetArr[i+1];
        }
        fileDataName = Form(dataDirectory + "VecSum_%s.root", solidTarget);
    } else{
        m = n+1;
        fileDataName = Form(dataDirectory + "VecSum_%s.root", targetArr);
    }

    // Select the target of the simultion
    char simulTarget[m];
    if(targetArr[0] == 'D') {
        simulTarget[0] = 'D';
        simulTarget[1] = '\0';
    } else{
        for(int i = 0; i < m; i++){
            simulTarget[i] = targetArr[i];
        }
    }
    // Open the input and output files
    TFile* fileData   = new TFile(fileDataName,"READ");
    TFile* fileSimul  = new TFile(Form(dataDirectory + "VecSumSimul_%s.root", simulTarget),
            "READ");
    TFile* fileOutput = new TFile(outputDirectory + 
            Form("corr_data_Phi_%i%i_%s.root", Q2Bin, NuBin, targetArr), "RECREATE");
    gROOT->cd();

    // Create some variables to use inside the for loops
    TString tupleDataName;
    TCut Q2Cut, NuCut, ZhCut, Pt2Cut, VCData, cutsData;
    TCut Q2Cut_gen, NuCut_gen, ZhCut_gen, Pt2Cut_gen, cutsSimul_gen, GenCut; 
    TCut Q2Cut_rec, NuCut_rec, ZhCut_rec, Pt2Cut_rec, cutsSimul_rec, RecCut;
    TCut YCCut = "TMath::Abs(YC)<1.4";
    // Select liquid or solid target
    if(targetArr[0] == 'D') { VCData  = "VC_TM == 1.";}
    else {VCData  = "VC_TM == 2.";}
    std::cout << Form("Simul target %s, Target %s", simulTarget, targetArr) << " and " << VCData << std::endl;

    // Create all the necessary histograms
    TH1F* histDetected    = new TH1F("Detected",     "", N_Phi, -180, 180);
    TH1F* histTotDetected = new TH1F("TotDetected",  "", N_Phi, -180, 180);
    TH1F* histThrown      = new TH1F("Thrown",       "", N_Phi, -180, 180);
    TH1F* histData        = new TH1F("Data",         "", N_Phi, -180, 180);
    TH1F* histFinalFactor = new TH1F("FinalFactor",  "", N_Phi, -180, 180);
    TH1F* histDataCorr2   = new TH1F("DataCorr2",    "", N_Phi, -180, 180);

    // Store the sum of the weights A.K.A the erros (in the other histograms if save it by other methods)
    histData->Sumw2();
    histThrown->Sumw2();
    histTotDetected->Sumw2();

    for(int gen = 1; gen <= N_PION ; gen++) { // Loops in every number of generated pions

        GenCut    = Form("Gen == %f", (float)gen);
        RecCut    = Form("Rec == %f", (float)gen);
        //GenDecCut = GenCut||RecCut;

        for(int ZhCounter = 0; ZhCounter < N_Zh; ZhCounter++) { 

            std::cout << "Bin selected: " << gen << Q2Bin << NuBin << ZhCounter << std::endl;

            // Select the cuts for each bin
            Q2Cut       = Form("Q2>%f&&Q2<%f", Q2_BINS[Q2Bin], Q2_BINS[Q2Bin+1]);
            NuCut       = Form("Nu>%f&&Nu<%f", Nu_BINS[NuBin], Nu_BINS[NuBin+1]);
            ZhCut       = Form("Zh>%f&&Zh<%f", Zh_BINS[ZhCounter], Zh_BINS[ZhCounter+1]);

            Q2Cut_gen   = Form("Q2_gen>%f&&Q2_gen<%f", Q2_BINS[Q2Bin], Q2_BINS[Q2Bin+1]);
            NuCut_gen   = Form("Nu_gen>%f&&Nu_gen<%f", Nu_BINS[NuBin], Nu_BINS[NuBin+1]);
            ZhCut_gen   = Form("Zh_gen>%f&&Zh_gen<%f", Zh_BINS[ZhCounter],
                                                        Zh_BINS[ZhCounter+1]);

            Q2Cut_rec   = Form("Q2_rec>%f&&Q2_rec<%f", Q2_BINS[Q2Bin], Q2_BINS[Q2Bin+1]);
            NuCut_rec   = Form("Nu_rec>%f&&Nu_rec<%f", Nu_BINS[NuBin], Nu_BINS[NuBin+1]);
            ZhCut_rec   = Form("Zh_rec>%f&&Zh_rec<%f", Zh_BINS[ZhCounter], 
                                                        Zh_BINS[ZhCounter+1]);

            cutsData  = Q2Cut&&NuCut&&ZhCut&&YCCut&&VCData;
            cutsSimul_gen = Q2Cut_gen&&NuCut_gen&&ZhCut_gen&&GenCut;
            cutsSimul_rec = Q2Cut_rec&&NuCut_rec&&ZhCut_rec&&RecCut;

            TNtuple* ntupleData  = (TNtuple*) fileData->Get(Form("ntuple_%i_pion", gen));
            TNtuple* ntupleSimul_gen = (TNtuple*) fileSimul->Get("ntuple_sim_gen");
            TNtuple* ntupleSimul_rec = (TNtuple*) fileSimul->Get("ntuple_sim_rec");

            // Apply the cuts to the ntuples to increces the efficiency
            ntupleData->Draw(">>listData", cutsData);
            ntupleSimul_gen->Draw(">>listSimul_gen", cutsSimul_gen);
            ntupleSimul_rec->Draw(">>listSimul_rec", cutsSimul_rec);

            TEventList* evntData = (TEventList*) gDirectory->Get("listData");
            TEventList* evntSimul_gen = (TEventList*) gDirectory->Get("listSimul_gen");
            TEventList* evntSimul_rec = (TEventList*) gDirectory->Get("listSimul_rec");

            ntupleData->SetEventList(evntData);
            ntupleSimul_gen->SetEventList(evntSimul_gen);
            ntupleSimul_rec->SetEventList(evntSimul_rec);

            for(int Pt2Counter = 0; Pt2Counter < N_Pt2; Pt2Counter++) { // Loops in every Pt2 bin

                // Select the Pt2 bin
                Pt2Cut      = Form("Pt2>%f&&Pt2<%f", Pt2_BINS[Pt2Counter], 
                                    Pt2_BINS[Pt2Counter+1]);
                Pt2Cut_gen  = Form("Pt2_gen>%f&&Pt2_gen<%f", Pt2_BINS[Pt2Counter], 
                                    Pt2_BINS[Pt2Counter+1]);
                Pt2Cut_rec  = Form("Pt2_rec>%f&&Pt2_rec<%f", Pt2_BINS[Pt2Counter],
                                    Pt2_BINS[Pt2Counter+1]);

                ntupleData->Project("Data", "PhiPQ", Pt2Cut);
                // If there isn't any event in data skip this bin
                if(EmptyHist(histData) == 1){ continue; }
                ntupleSimul_rec->Project("Detected",    "PhiPQ_rec", Pt2Cut_rec&&GenCut);
                if(EmptyHist(histDetected) == 1){ continue; }
                ntupleSimul_gen->Project("Thrown",      "PhiPQ_gen", Pt2Cut_gen);
                ntupleSimul_rec->Project("TotDetected", "PhiPQ_rec", Pt2Cut_rec);

                histFinalFactor->Divide(histThrown, histTotDetected, 1, 1, "B");

                // Apply the correction factors
                histDataCorr2->Divide(histDataCorr, histFinalFactor, 1, 1);

                // Save the histograms in the output file
                fileOutput->cd();

                histData->Write(Form("Data_%s_%i%i%i%i_%i",            targetArr, Q2Bin,
                            NuBin, ZhCounter, Pt2Counter, gen));
                histDataCorr2->Write(Form("DataCorr2_%s_%i%i%i%i_%i",  targetArr, Q2Bin, 
                            NuBin, ZhCounter, Pt2Counter, gen));
                histFinalFactor->Write(Form("FinalFactor_%s_%i%i%i%i_%i", targetArr, Q2Bin, 
                            NuBin, ZhCounter, Pt2Counter, gen));

                gROOT->cd();

                // Set the histograms values to 0
                histData->Reset();
                histDataCorr2->Reset();
                histThrown->Reset();
                histTotDetected->Reset();
                histFinalFactor->Reset();

            } // End Pt2 loop
            
            delete ntupleData;
            delete ntupleSimul_gen;
            delete ntupleSimul_rec;
            delete evntData;
            delete evntSimul_gen;
            delete evntSimul_rec;

        } // End Zh loop
    } // End pion number loop

    fileData->Close();
    fileSimul->Close();
    fileOutput->Close();
    t.Print();
    delete histTotDetected ;
    delete histThrown      ;
    delete histData        ;
    delete histDataCorr2   ;
    delete histFinalFactor ;
    return 0;

}
