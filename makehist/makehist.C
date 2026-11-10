#include <vector>
#include <cmath>
#include <string>

using namespace std;

void makehist
(
      //const std::string infile  = "/sphenix/tg/tg01/commissioning/INTT/work/hachiya/SiCaloTrack/data/run2pp/53741/ana_53741_00000.root"
      const std::string infile  = "/sphenix/tg/tg01/commissioning/INTT/work/hachiya/SiCaloTrack/data/run3pp/79508/ana_79508_00000.root"
    , const std::string outdir = "/sphenix/tg/tg01/commissioning/INTT/work/mizuki/RunQA/makehist/result/data_run3pp/"
)
{ 
    //defined hist
    //EMCal Hit
    TH1F *h_EMCal_phi = new TH1F("h_EMCal_phi","EMCal phi; phi[rad]; counts",100,-4,4);
    TH1F *h_EMCal_z = new TH1F("h_EMCal_z","EMCal z; z[cm]; counts",80,-160,160);
    TH2F *h2_EMCal_phi_z = new TH2F("h2_EMCal_phi_z","EMCal phi vs z; phi[rad]; z[cm]",100,-4,4,100,-150,150);
    //TopoCalo hist
    TH1F *h_TopoClus_phi = new TH1F("h_TopoClus_phi","TopoClus phi; phi[rad]; counts",100,-4,4);
    TH1F *h_TopoClus_z = new TH1F("h_TopoClus_z","TopoClus z; z[cm]; counts",100,-300,300);
    TH2F *h2_TopoClus_phi_z = new TH2F("h2_TopoClus_phi_z","TopoClus phi vs z; phi[rad]; z[cm]",100,-4,4,100,-300,300);
    //MVTX Hit 
    TH1F *h_AllMVTX_Inner_phi = new TH1F("h_AllMVTX_Inner_phi","AllMVTX Inner phi; phi[rad]; counts",100,-4,4);
    TH1F *h_AllMVTX_Inner_z = new TH1F("h_AllMVTX_Inner_z","AllMVTX Inner z; z[cm]; counts",100,-15,15);
    TH2F *h2_AllMVTX_Inner_phi_z = new TH2F("h2_AllMVTX_Inner_phi_z","AllMVTX Inner phi vs z; phi[rad]; z[cm]",100,-4,4,100,-15,15);
    TH1F *h_AllMVTX_Middle_phi = new TH1F("h_AllMVTX_Middle_phi","AllMVTX Middle phi; phi[rad]; counts",100,-4,4);
    TH1F *h_AllMVTX_Middle_z = new TH1F("h_AllMVTX_Middle_z","AllMVTX Middle z; z[cm]; counts",100,-15,15);
    TH2F *h2_AllMVTX_Middle_phi_z = new TH2F("h2_AllMVTX_Middle_phi_z","AllMVTX Middle phi vs z; phi[rad]; z[cm]",100,-4,4,100,-15,15);
    TH1F *h_AllMVTX_Outer_phi = new TH1F("h_AllMVTX_Outer_phi","AllMVTX Outer phi; phi[rad]; counts",100,-4,4);
    TH1F *h_AllMVTX_Outer_z = new TH1F("h_AllMVTX_Outer_z","AllMVTX Outer z; z[cm]; counts",100,-15,15);
    TH2F *h2_AllMVTX_Outer_phi_z = new TH2F("h2_AllMVTX_Outer_phi_z","AllMVTX Outer phi vs z; phi[rad]; z[cm]",100,-4,4,100,-15,15);
    //INTT Hit
    TH1F *h_AllINTT_Inner_phi = new TH1F("h_AllINTT_Inner_phi","AllINTT Inner phi; phi[rad]; counts",100,-4,4);
    TH1F *h_AllINTT_Inner_z = new TH1F("h_AllINTT_Inner_z","AllINTT Inner z; z[cm]; counts",30,-25,25);
    TH2F *h2_AllINTT_Inner_phi_z = new TH2F("h2_AllINTT_Inner_phi_z","AllINTT Inner phi vs z; phi[rad]; z[cm]",100,-4,4,30,-25,25);
    TH1F *h_AllINTT_Outer_phi = new TH1F("h_AllINTT_Outer_phi","AllINTT Outer phi; phi[rad]; counts",100,-4,4);
    TH1F *h_AllINTT_Outer_z = new TH1F("h_AllINTT_Outer_z","AllINTT Outer z; z[cm]; counts",30,-25,25);
    TH2F *h2_AllINTT_Outer_phi_z = new TH2F("h2_AllINTT_Outer_phi_z","AllINTT Outer phi vs z; phi[rad]; z[cm]",100,-4,4,30,-25,25);
    TH1F *h_AllINTT_Inner_zm_phi = new TH1F("h_AllINTT_Inner_zm_phi","AllINTT Inner zm phi; phi[rad]; counts",100,-4,4);
    TH1F *h_AllINTT_Outer_zm_phi = new TH1F("h_AllINTT_Outer_zm_phi","AllINTT Outer zm phi; phi[rad]; counts",100,-4,4);
    TH1F *h_AllINTT_Inner_zp_phi = new TH1F("h_AllINTT_Inner_zp_phi","AllINTT Inner zp phi; phi[rad]; counts",100,-4,4);
    TH1F *h_AllINTT_Outer_zp_phi = new TH1F("h_AllINTT_Outer_zp_phi","AllINTT Outer zp phi; phi[rad]; counts",100,-4,4);
    TH2F *h2_AllINTT_Inner_zm_phi_evt = new TH2F("h2_AllINTT_Inner_zm_phi_evt","AllINTT Inner zm phi evt; evt; phi[rad]",10000,0,10000000,100,-4,4);
    TH2F *h2_AllINTT_Outer_zm_phi_evt = new TH2F("h2_AllINTT_Outer_zm_phi_evt","AllINTT Outer zm phi evt; evt; phi[rad]",10000,0,10000000,100,-4,4);
    TH2F *h2_AllINTT_Inner_zp_phi_evt = new TH2F("h2_AllINTT_Inner_zp_phi_evt","AllINTT Inner zp phi evt; evt; phi[rad]",10000,0,10000000,100,-4,4);
    TH2F *h2_AllINTT_Outer_zp_phi_evt = new TH2F("h2_AllINTT_Outer_zp_phi_evt","AllINTT Outer zp phi evt; evt; phi[rad]",10000,0,10000000,100,-4,4);
    TH1F *h_seed_evt = new TH1F("h_seed_evt","h_seed_evt; evt; counts",10000,0,10000000);
    //MVTX Hit 
    TH1F *h_MVTX_Inner_phi = new TH1F("h_MVTX_Inner_phi","MVTX Inner phi; phi[rad]; counts",100,-4,4);
    TH1F *h_MVTX_Inner_z = new TH1F("h_MVTX_Inner_z","MVTX Inner z; z[cm]; counts",100,-15,15);
    TH2F *h2_MVTX_Inner_phi_z = new TH2F("h2_MVTX_Inner_phi_z","MVTX Inner phi vs z; phi[rad]; z[cm]",100,-4,4,100,-15,15);
    TH1F *h_MVTX_Middle_phi = new TH1F("h_MVTX_Middle_phi","MVTX Middle phi; phi[rad]; counts",100,-4,4);
    TH1F *h_MVTX_Middle_z = new TH1F("h_MVTX_Middle_z","MVTX Middle z; z[cm]; counts",100,-15,15);
    TH2F *h2_MVTX_Middle_phi_z = new TH2F("h2_MVTX_Middle_phi_z","MVTX Middle phi vs z; phi[rad]; z[cm]",100,-4,4,100,-15,15);
    TH1F *h_MVTX_Outer_phi = new TH1F("h_MVTX_Outer_phi","MVTX Outer phi; phi[rad]; counts",100,-4,4);
    TH1F *h_MVTX_Outer_z = new TH1F("h_MVTX_Outer_z","MVTX Outer z; z[cm]; counts",100,-15,15);
    TH2F *h2_MVTX_Outer_phi_z = new TH2F("h2_MVTX_Outer_phi_z","MVTX Outer phi vs z; phi[rad]; z[cm]",100,-4,4,100,-15,15);
    //INTT Hit
    TH1F *h_INTT_Inner_phi = new TH1F("h_INTT_Inner_phi","INTT Inner phi; phi[rad]; counts",100,-4,4);
    TH1F *h_INTT_Inner_z = new TH1F("h_INTT_Inner_z","INTT Inner z; z[cm]; counts",30,-25,25);
    TH2F *h2_INTT_Inner_phi_z = new TH2F("h2_INTT_Inner_phi_z","INTT Inner phi vs z; phi[rad]; z[cm]",100,-4,4,30,-25,25);
    TH1F *h_INTT_Outer_phi = new TH1F("h_INTT_Outer_phi","INTT Outer phi; phi[rad]; counts",100,-4,4);
    TH1F *h_INTT_Outer_z = new TH1F("h_INTT_Outer_z","INTT Outer z; z[cm]; counts",30,-25,25);
    TH2F *h2_INTT_Outer_phi_z = new TH2F("h2_INTT_Outer_phi_z","INTT Outer phi vs z; phi[rad]; z[cm]",100,-4,4,30,-25,25);
    TH1F *h_zvtx = new TH1F("h_zvtx","zvtx; [cm]; counts",100,-20,20);
    //evtTree
    TH1F *h_trkz0 = new TH1F("h_trkz0","h_trkz0; trk_z0;count",100,-25,25);
    
    //output
    TString fname = infile.c_str();
    cout<<"infile : "<<fname<<endl;
 
    //file open
    TFile *file = TFile::Open(fname, "READ");
    TString fname_cout = gSystem->BaseName(fname);
    if(!file || file->IsZombie())
    {
        cout << "cannot open " << fname_cout << endl;
        //continue;
        return;
    }

    fname_cout.Prepend("qahist_");
    
    std::string outfile = outdir + fname_cout.Data();
    //string outfile = "/sphenix/tg/tg01/commissioning/INTT/work/mizuki/Jpsi/RunQA/makehist/condor/data/test.root";
    cout<<"outfile : "<<outfile<<endl;

    TFile *fout = new TFile(outfile.c_str(),"RECREATE");

    //caloTree
    TTree *caloTree = (TTree *)file->Get("caloTree");
    vector<float> *calo_phi = nullptr;
    vector<float> *calo_z = nullptr;

    caloTree->SetBranchAddress("phi", &calo_phi);
    caloTree->SetBranchAddress("z", &calo_z);

    Long64_t nentries = caloTree->GetEntries();
    for(Long64_t evt = 0; evt < nentries; evt++)
    {
        caloTree->GetEntry(evt);

        for(size_t i = 0; i < calo_phi->size(); i++)
        {   
            h_EMCal_phi->Fill(calo_phi->at(i));
            h_EMCal_z->Fill(calo_z->at(i));
            h2_EMCal_phi_z->Fill( calo_phi->at(i),calo_z->at(i) );
        }
    }
    cout << "EMCal " << fname_cout << endl;

    //TopoClusTree
    TTree *TopoClusTree = (TTree *)file->Get("TopoClusTree");
    vector<float> *Topo_phi = nullptr;
    vector<float> *Topo_z = nullptr;

    TopoClusTree->SetBranchAddress("clus_phi", &Topo_phi);
    TopoClusTree->SetBranchAddress("clus_z", &Topo_z);

    nentries = TopoClusTree->GetEntries();
    for(Long64_t evt = 0; evt < nentries; evt++)
    {
        TopoClusTree->GetEntry(evt);

        for(size_t i = 0; i < Topo_phi->size(); i++)
        {   
            h_TopoClus_phi->Fill( Topo_phi->at(i) );
            h_TopoClus_z->Fill( Topo_z->at(i) );
            h2_TopoClus_phi_z->Fill( Topo_phi->at(i), Topo_z->at(i) );
        }
    }
    cout << "Topo " << fname_cout << endl;

    //SiClusAllTree
    TTree *SiClusAllTree = (TTree *)file->Get("SiClusAllTree");

    int AllSiclus_evt;
    vector<float> *AllSiclus_x = nullptr;
    vector<float> *AllSiclus_y = nullptr;
    vector<float> *AllSiclus_z = nullptr;
    vector<int> *AllSiclus_layer = nullptr;

    SiClusAllTree->SetBranchAddress("evt", &AllSiclus_evt);
    SiClusAllTree->SetBranchAddress("Siclus_x", &AllSiclus_x);
    SiClusAllTree->SetBranchAddress("Siclus_y", &AllSiclus_y);
    SiClusAllTree->SetBranchAddress("Siclus_z", &AllSiclus_z);
    SiClusAllTree->SetBranchAddress("Siclus_layer", &AllSiclus_layer);

    nentries = SiClusAllTree->GetEntries();
    for(Long64_t evt = 0; evt < nentries; evt++)
    {
        SiClusAllTree->GetEntry(evt);

        for(size_t i = 0; i < AllSiclus_x->size(); i++)
        {   
            float phi = std::atan2(AllSiclus_y->at(i),AllSiclus_x->at(i)); 
            int layer = AllSiclus_layer->at(i);

            if(layer == 0)
            {
                h_AllMVTX_Inner_phi->Fill(phi);
                h_AllMVTX_Inner_z->Fill(AllSiclus_z->at(i));
                h2_AllMVTX_Inner_phi_z->Fill(phi,AllSiclus_z->at(i));
            }
            else if(layer == 1)
            {
                h_AllMVTX_Middle_phi->Fill(phi);
                h_AllMVTX_Middle_z->Fill(AllSiclus_z->at(i));
                h2_AllMVTX_Middle_phi_z->Fill(phi,AllSiclus_z->at(i));
            }
            else if(layer == 2)
            {
                h_AllMVTX_Outer_phi->Fill(phi);
                h_AllMVTX_Outer_z->Fill(AllSiclus_z->at(i));
                h2_AllMVTX_Outer_phi_z->Fill(phi,AllSiclus_z->at(i));
            }
            else if(layer == 3 || layer == 4)
            {
                h_AllINTT_Inner_phi->Fill(phi);
                h_AllINTT_Inner_z->Fill(AllSiclus_z->at(i));
                h2_AllINTT_Inner_phi_z->Fill(phi,AllSiclus_z->at(i));

                if(AllSiclus_z->at(i) < 0)
                {
                    h_AllINTT_Inner_zm_phi->Fill(phi);
                    h2_AllINTT_Inner_zm_phi_evt->Fill(AllSiclus_evt,phi);
                }
                if(AllSiclus_z->at(i) > 0)
                {
                    h_AllINTT_Inner_zp_phi->Fill(phi);
                    h2_AllINTT_Inner_zp_phi_evt->Fill(AllSiclus_evt,phi);
                }
            }
            else if(layer == 5 || layer == 6)
            {
                h_AllINTT_Outer_phi->Fill(phi);
                h_AllINTT_Outer_z->Fill(AllSiclus_z->at(i));
                h2_AllINTT_Outer_phi_z->Fill(phi,AllSiclus_z->at(i));
                if(AllSiclus_z->at(i) < 0)
                {
                    h_AllINTT_Outer_zm_phi->Fill(phi);
                    h2_AllINTT_Outer_zm_phi_evt->Fill(AllSiclus_evt,phi);
                }
                if(AllSiclus_z->at(i) > 0)
                {
                    h_AllINTT_Outer_zp_phi->Fill(phi);
                    h2_AllINTT_Outer_zp_phi_evt->Fill(AllSiclus_evt,phi);
                }
            }                
        }
        h_seed_evt->Fill(evt);
    }
    cout << "SiclusAll" << fname_cout << endl;

    //SiClus
    TTree *SiClusTree = (TTree *)file->Get("SiClusTree");

    int Siclus_evt;
    vector<float> *Siclus_x = nullptr;
    vector<float> *Siclus_y = nullptr;
    vector<float> *Siclus_z = nullptr;
    vector<int> *Siclus_layer = nullptr;

    SiClusTree->SetBranchAddress("evt", &Siclus_evt);
    SiClusTree->SetBranchAddress("Siclus_x", &Siclus_x);
    SiClusTree->SetBranchAddress("Siclus_y", &Siclus_y);
    SiClusTree->SetBranchAddress("Siclus_z", &Siclus_z);
    SiClusTree->SetBranchAddress("Siclus_layer", &Siclus_layer);

    nentries = SiClusTree->GetEntries();

    for(Long64_t evt = 0; evt < nentries; evt++)
    {
        SiClusTree->GetEntry(evt);

        for(size_t i = 0; i < Siclus_x->size(); i++)
        {   
            float phi = std::atan2(Siclus_y->at(i),Siclus_x->at(i)); 
            int layer = Siclus_layer->at(i);

            if(layer == 0)
            {
                h_MVTX_Inner_phi->Fill(phi);
                h_MVTX_Inner_z->Fill(Siclus_z->at(i));
                h2_MVTX_Inner_phi_z->Fill(phi,Siclus_z->at(i));
            }
            else if(layer == 1)
            {
                h_MVTX_Middle_phi->Fill(phi);
                h_MVTX_Middle_z->Fill(Siclus_z->at(i));
                h2_MVTX_Middle_phi_z->Fill(phi,Siclus_z->at(i));
            }
            else if(layer == 2)
            {
                h_MVTX_Outer_phi->Fill(phi);
                h_MVTX_Outer_z->Fill(Siclus_z->at(i));
                h2_MVTX_Outer_phi_z->Fill(phi,Siclus_z->at(i));
            }
            else if(layer == 3 || layer == 4)
            {
                h_INTT_Inner_phi->Fill(phi);
                h_INTT_Inner_z->Fill(Siclus_z->at(i));
                h2_INTT_Inner_phi_z->Fill(phi,Siclus_z->at(i));
            }
            else if(layer == 5 || layer == 6)
            {
                h_INTT_Outer_phi->Fill(phi);
                h_INTT_Outer_z->Fill(Siclus_z->at(i));
                h2_INTT_Outer_phi_z->Fill(phi,Siclus_z->at(i));
            }                
        }
    }
    cout << "Siclus" << fname_cout << endl;

    //treckTree
    TTree *trackTree = (TTree *)file->Get("trackTree");

    vector<float> *sicalo_z0 = nullptr;
    trackTree->SetBranchAddress("z0", &sicalo_z0);
        
    nentries = trackTree->GetEntries();
    for(Long64_t evt = 0; evt < nentries; evt++)
    {   
        trackTree->GetEntry(evt);

        for(size_t i = 0; i < sicalo_z0->size(); i++)
        {   
            h_trkz0->Fill(sicalo_z0->at(i));
        }
    }
    cout << "track " << fname_cout << endl;

    //evtTree
    TTree *evtTree = (TTree *)file->Get("evtTree");
    float zvtx;
    evtTree->SetBranchAddress("zvtx", &zvtx);
    
    nentries = evtTree->GetEntries();
    for(Long64_t evt = 0; evt < nentries; evt++)
    {
        evtTree->GetEntry(evt);
        h_zvtx->Fill(zvtx); 
    }
    cout << "evt " << fname_cout << endl;

    //EMCal Hit
    h_EMCal_phi->Write();
    h_EMCal_z->Write();
    h2_EMCal_phi_z->Write();
    //Topo Hit
    h_TopoClus_phi->Write();
    h_TopoClus_z->Write();
    h2_TopoClus_phi_z->Write();
    //MVTX Inner Hit
    h_MVTX_Inner_phi->Write();
    h_MVTX_Inner_z->Write();
    h2_MVTX_Inner_phi_z->Write();
    //MVTX Middle Hit
    h_MVTX_Middle_phi->Write();
    h_MVTX_Middle_z->Write();
    h2_MVTX_Middle_phi_z->Write();
    //MVTX Outer Hit
    h_MVTX_Outer_phi->Write();
    h_MVTX_Outer_z->Write();
    h2_MVTX_Outer_phi_z->Write();
    //INTT Inner Hit
    h_INTT_Inner_phi->Write();
    h_INTT_Inner_z->Write();
    h2_INTT_Inner_phi_z->Write();
    //INTT Outer Hit
    h_INTT_Outer_phi->Write();
    h_INTT_Outer_z->Write();
    h2_INTT_Outer_phi_z->Write();
    //MVTX Inner Hit
    h_AllMVTX_Inner_phi->Write();
    h_AllMVTX_Inner_z->Write();
    h2_AllMVTX_Inner_phi_z->Write();
    //MVTX Middle Hit
    h_AllMVTX_Middle_phi->Write();
    h_AllMVTX_Middle_z->Write();
    h2_AllMVTX_Middle_phi_z->Write();
    //MVTX Outer Hit
    h_AllMVTX_Outer_phi->Write();
    h_AllMVTX_Outer_z->Write();
    h2_AllMVTX_Outer_phi_z->Write();
    //INTT Inner Hit
    h_AllINTT_Inner_phi->Write();
    h_AllINTT_Inner_z->Write();
    h2_AllINTT_Inner_phi_z->Write();
    //INTT Outer Hit
    h_AllINTT_Outer_phi->Write();
    h_AllINTT_Outer_z->Write();
    h2_AllINTT_Outer_phi_z->Write();
    //INTT northsouth
    h_AllINTT_Inner_zm_phi->Write();
    h_AllINTT_Outer_zm_phi->Write();
    h_AllINTT_Inner_zp_phi->Write();
    h_AllINTT_Outer_zp_phi->Write();
    h2_AllINTT_Inner_zm_phi_evt->Write();
    h2_AllINTT_Outer_zm_phi_evt->Write();
    h2_AllINTT_Inner_zp_phi_evt->Write();
    h2_AllINTT_Outer_zp_phi_evt->Write();
    h_seed_evt->Write();
    //zvtx
    h_zvtx->Write();
    h_trkz0->Write();

    fout->Close();
    
   cout << outfile << "All oK!!!!"<<endl; 
}
