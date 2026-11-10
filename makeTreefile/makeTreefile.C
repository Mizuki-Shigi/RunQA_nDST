#include <vector>
#include <cmath>
#include <string>

using namespace std;

void makeTreefile
(
      const std::string infile  = "/sphenix/tg/tg01/commissioning/INTT/work/hachiya/SiCaloTrack/data/run2pp/53741/ana_53741_00000.root"
    , const std::string outdir = "/sphenix/tg/tg01/commissioning/INTT/work/mizuki/RunQA/makeTreefile/result/data/"
)
{
  TString fname = infile.c_str();

  TFile *file = TFile::Open(fname, "READ");
  TString fname_cout = gSystem->BaseName(fname);
  if(!file || file->IsZombie())
  {
      cout << "cannot open " << fname_cout << endl;
      //continue;
      return;
  }else cout << "open : "<< fname_cout << endl;

  fname_cout.Prepend("qa_");

  string outfile = outdir + fname_cout.Data();
  cout<<"outfile : "<<outfile<<endl;

  TFile *fout = new TFile(outfile.c_str(),"RECREATE");

  // Treeを作成
  TTree *AllSiTree = new TTree("SiClusAll", "SiclusAll");

  // Branch用変数
  int out_evt;

  vector<float> out_Siclus_x;
  vector<float> out_Siclus_y;
  vector<float> out_Siclus_z;
  vector<int>   out_Siclus_layer;
  //vector<float> out_Siclus_phi;

  AllSiTree->Branch("evt", &out_evt);
  AllSiTree->Branch("Siclus_x", &out_Siclus_x);
  AllSiTree->Branch("Siclus_y", &out_Siclus_y);
  AllSiTree->Branch("Siclus_z", &out_Siclus_z);
  AllSiTree->Branch("Siclus_layer", &out_Siclus_layer);
  //AllSiTree->Branch("Siclus_phi", &out_Siclus_phi);
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

  Long64_t nentries = SiClusAllTree->GetEntries();
  for(Long64_t evt = 0; evt < nentries; evt++)
  {
    out_Siclus_x.clear();
    out_Siclus_y.clear();
    out_Siclus_z.clear();
    out_Siclus_layer.clear();
    //out_Siclus_phi.clear();
    SiClusAllTree->GetEntry(evt);

    out_evt = AllSiclus_evt;

    out_Siclus_x = *AllSiclus_x;
    out_Siclus_y = *AllSiclus_y;
    out_Siclus_z = *AllSiclus_z;
    out_Siclus_layer = *AllSiclus_layer;

    for(size_t i = 0; i < AllSiclus_x->size(); i++)
    {   
        float phi = std::atan2(AllSiclus_y->at(i),AllSiclus_x->at(i)); 
        int layer = AllSiclus_layer->at(i);
        //out_Siclus_phi.push_back(phi);
    }        
    AllSiTree->Fill();        
  }

  cout << "Fill SiclusAll" << endl;

  AllSiTree->Write();
  fout->Close();

  cout << outfile << "All oK!!!!"<<endl; 
}

