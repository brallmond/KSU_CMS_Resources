void NanoAODTemplate(char* filename) {

    // Author Braden Allmond August 1st 2022
    // The purpose of this file is to serve as a minimal introduction to
    // working with ROOT files. This macro was originally made to produce
    // a turn-on plot from NanoAOD data and was repurposed and
    // further commented to illustrate useful and common ROOT techniques.

    // execute with this
    // root -l 'NanoAODTemplate.C("anything")'

    // TChain info https://root.cern.ch/doc/master/classTChain.html
    TChain tree("Events");

    tree.Add("/eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/STEAM/anayak/2022NanoAOD/SingleMuonV1/Files1/nano_aod_0.root");
    tree.Add("/eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/STEAM/anayak/2022NanoAOD/SingleMuonV1/Files1/nano_aod_1.root");
    tree.Add("/eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/STEAM/anayak/2022NanoAOD/SingleMuonV1/Files1/nano_aod_10.root");
    //alternative way of opening an available file (not using TChain)
    //TFile *_file0 = TFile::Open(filename);
    //TTree* tree = (TTree*)_file0->Get("Events");

    // TCuts defining object requirements
    // exactly analgous to
    // tree->Draw("run", "nTau>0 && Tau_eta<=2.1", "goff")
    // on commandline
    // These are separated between offline cuts, trigger cuts, and then combinations 

    TCut userGoodTau = "nTau>0 && Tau_eta<=2.1";
    TCut userOfflineTauID = "Tau_idDeepTau2017v2p1VSjet>=16 && Tau_idDeepTau2017v2p1VSmu>=8 && Tau_idDeepTau2017v2p1VSe>=2";
    TCut userGoodMuon = "nMuon>0 && Muon_eta<=2.1 && Muon_pt>=27";

    TCut userPassIsoMu27 = "HLT_IsoMu27>0";
    TCut userPassTauPOGCrossTrig = "HLT_IsoMu24_eta2p1_MediumDeepTauPFTauHPS30_L2NN_eta2p1_CrossL1>0 && Tau_pt>=35"; //Jaime's

    TCut goodObjectCuts = userGoodTau && userOfflineTauID && userGoodMuon;
    TCut numeratorCuts = goodObjectCuts && userPassTauPOGCrossTrig;
    TCut denominatorCuts = goodObjectCuts && userPassIsoMu27;

    // print the number of events passing baseline requirements
    // this way you know the number of events your graphs are
    // displaying. Gives a good idea of statistics
    //trigger info
    double passTauPOG = tree.Draw("run", userPassTauPOGCrossTrig, "goff");
    double passIsoMu27 = tree.Draw("run", userPassIsoMu27, "goff");

    std::cout << "pass Tau POG Cross Trig " << passTauPOG << std::endl;
    std::cout << "pass IsoMu27 " << passIsoMu27 << std::endl;

    // selection defined, now make plots
    long nEvents = tree.Draw("run", "run>0", "goff");
    std::cout << "nEvents " << nEvents << std::endl;

    // this sets error bars correctly 
    TH1::SetDefaultSumw2();

    // create a canvas object to draw to and later save
    TCanvas *c1 = new TCanvas("c1", "", 600, 400);
    // set the optional statistics box (normally in top right hand corner) off
    gStyle->SetOptStat(kFALSE);

    // manually set number of bins and bin edges
    // there is a way to do this automatically in pyroot
    const Int_t NBINS = 10;
    Double_t edges[NBINS + 1] = {0.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 200.0};

    // create three histogram objects containing floats for later use
    TH1F* h_num = new TH1F("h_num", "", NBINS, edges); 
    TH1F* h_den = new TH1F("h_den", "", NBINS, edges);
    TH1F* h_ratio = new TH1F("h_ratio", "", NBINS, edges);

    // from the Tau_pt branch of the tree (from TChain), fill the h_num TH1F with events
    // passing the numeratorCuts TCut
    // similarly for the h_den
    tree.Draw("Tau_pt >> h_num", numeratorCuts, "goff");
    tree.Draw("Tau_pt >> h_den", denominatorCuts, "goff");
    // again, print event numbers to get an idea of statistics used
    double numCount = tree.Draw("Tau_pt >> h_num", numeratorCuts, "goff");
    double denCount = tree.Draw("Tau_pt >> h_den", denominatorCuts, "goff");
    std::cout << numCount << '\t' << "numerator Events" << std::endl;
    std::cout << denCount << '\t' << "denominator Events" << std::endl;

    // assign a clone of h_num to the h_ratio TH1F
    h_ratio = (TH1F*)h_num->Clone();
    // divide the clone of the numerator by the denominator
    // thus making a ratio (while still preserving both the num and denom)
    h_ratio->Divide(h_den);

   
    h_ratio->SetTitle("Offline Tau pT Efficiency");
    h_ratio->Draw("E");
    //h_ratio->GetYaxis()->SetRangeUser(0.,1.1);

    // save plot
    c1->Print("turn-on-plot.png", "png");


}
