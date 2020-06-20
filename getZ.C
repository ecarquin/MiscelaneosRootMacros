void getZ()
{

  //Do hadd on all background samples

  //Open root files
  TFile *b=TFile::Open("background.root","READ");
  TFile *s=TFile::Open("signal.root","READ");

  //Get corresponding histograms
  TH1F *h_b = (TH1F*)b->Get("/NOSYS/hist/SS_ee_nominal_ht");
  TH1F *h_f = (TH1F*)b->Get("/NOSYS/hist/SS_ee_nominal_ht_fakes");
  TH1F *h_s = (TH1F*)s->Get("/NOSYS/hist/SS_ee_nominal_ht");

  //Sum histogram with fakes to background histogram to get the total background
  h_b->Add(h_f);

  //For simplicity rebin files to have 100 histograms instead of 5000
  h_b->Rebin(50);
  h_s->Rebin(50);

  //Calculate S/Sqrt(b), stop if there is less than 1 signal event

  //Start with HT cut = 400 GeV;
  float htcut=400.;

  //Define a 1D histogram to store the significance found for each cut 
  TH1F *h_Z=new TH1F("h_z","Significance",100,0,5000);

  //Loop through the htcut
  while(htcut<5000){
    Int_t bin=h_s->FindBin(htcut);
    cout << "Starting bin is: " << bin << " " << h_s->GetBinLowEdge(bin) << endl;
    float S=0;
    float B=0;
    for(UInt_t ibin=bin;ibin<h_s->GetNbinsX();ibin++){
      S+=h_s->GetBinContent(ibin);
      B+=h_b->GetBinContent(ibin);
      cout<< "Signal: " << S << " Background: " << B << endl;
    }
    if(S<1.) break;
    float Z=S/sqrt(B);
    cout << "Z= " << Z << endl;
    h_Z->SetBinContent(bin,Z);
    htcut+=50;
  }

  //Create a canvas to draw the histogram
  TCanvas *c=new TCanvas("c","",800,600);
  h_Z->Draw();
  c->SaveAs("Z.png");
  
}
