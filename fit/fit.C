#include <vector>
#include <string>

using namespace std;

void fit
(
    const string std_indir =
    "/sphenix/tg/tg01/commissioning/INTT/work/mizuki/RunQA/makeaverage/result",

    const string ratio_indir =
    "/sphenix/tg/tg01/commissioning/INTT/work/mizuki/RunQA/makeaverage/result",

    const string outdir =
    "/sphenix/tg/tg01/commissioning/INTT/work/mizuki/RunQA/fit/result/fit",

    const string logdir = "/sphenix/tg/tg01/commissioning/INTT/work/mizuki/RunQA/fit/result/logs"
)
{

    vector<int> runnums = {
        53018,
        53046,
        53741,
        53742,
        53743,
        53744,
        53756,
        53783,
        53871,
        53876,
        53877,
        53879
    };

    vector<string> hname{

        "h_MVTX_Inner_phi",
        "h_MVTX_Middle_phi",
        "h_MVTX_Outer_phi",
        "h_MVTX_Inner_z",
        "h_MVTX_Middle_z",
        "h_MVTX_Outer_z",
        "h_INTT_Inner_phi",
        "h_INTT_Outer_phi",
        "h_INTT_Inner_z",
        "h_INTT_Outer_z",
        "h_AllMVTX_Inner_phi",
        "h_AllMVTX_Middle_phi",
        "h_AllMVTX_Outer_phi",
        "h_AllMVTX_Inner_z",
        "h_AllMVTX_Middle_z",
        "h_AllMVTX_Outer_z",
        "h_AllINTT_Inner_phi",
        "h_AllINTT_Outer_phi",
        "h_AllINTT_Inner_z",
        "h_AllINTT_Outer_z",
        "h_EMCal_phi",
        "h_EMCal_z",
        "h_TopoClus_phi",
        "h_TopoClus_z"
    };

    TString outpdf = TString(outdir) + "/chi2ndf.pdf";
    TLatex latex;
    latex.SetTextSize(0.025);
    latex.SetTextAngle(45);

    TCanvas *c = new TCanvas("c", "Canvas", 1200, 800);
    gStyle->SetOptStat(0);

    TFile *fin_std  = TFile::Open(Form("%s/average.root",std_indir.c_str()));
    if(!fin_std || fin_std->IsZombie())
    {
        cout << "Cannot open file!" << endl;
        return;
    }
    cout << "file opened" << endl;

    for(int i = 0; i<hname.size(); i++)
    {

        TFile *fin_ratio  = TFile::Open(Form("%s/%s.root",ratio_indir.c_str(),hname[i].c_str()));
        if(!fin_ratio || fin_ratio->IsZombie())
        {
            cout << "Cannot open file!" << endl;
            continue;
        }
        cout << "file opened" << endl;

        TFile* fout = TFile::Open(Form("%s/%s.root", outdir.c_str(), hname[i].c_str()),  "RECREATE" );
        TString log = Form("%s/%s_log.txt",logdir.c_str(),hname[i].c_str());
        //TString log = Form("%s/test_%s_log.txt",logdir.c_str(),hname[i].c_str());

        ofstream logout(log.Data());

        TGraph *gr = new TGraph();
        gr->SetTitle(Form("%s_chi2/ndf",hname[i].c_str()));
        //gr->GetXaxis()->SetTitle("runnum");
        gr->GetYaxis()->SetTitle("#chi^{2}/ndf");
        gr->GetXaxis()->SetLabelOffset(999);
        gr->GetXaxis()->SetTickLength(0);
        gr->SetMarkerStyle(20);
        gr->SetMarkerSize(1.2);

        logout << "Input file : "<< Form("%s.root",hname[i].c_str())<< endl;

        TH1 *h_std = (TH1*)fin_std->Get(Form("%s_std",hname[i].c_str()));
        if (!h_std) 
        {
            cout << "Cannot find histogram: "
                << hname[i] << "_std" << endl;
            fin_ratio->Close();
            delete fin_ratio;
            continue;
        }

        TIter next(fin_ratio->GetListOfKeys());
        TKey *key;
        int l = 0;

        while((key=(TKey*)next()))
        {
            TObject *obj = key->ReadObj();

            string name = obj->GetName();
            if(!obj->InheritsFrom("TH1"))continue;

            TH1 *h_ratio = (TH1*)obj->Clone();
            h_ratio->Fit("pol0","Q");

            TF1* fpol0 = h_ratio->GetFunction("pol0");

            double p0   = fpol0->GetParameter(0);
            double err  = fpol0->GetParError(0);
            double NDF  = fpol0->GetNDF();
            fout->cd();
            h_ratio->Write();

            logout << "Histogram : " << h_ratio->GetName() << endl;
            logout << "  p0        = " << p0 << " +/- " << err << endl;
            logout << "  NDF  :"  << NDF <<endl;
            logout << "----------------------------------" << endl;

            double chi2_sum = 0.0;

            for (int bin = 1; bin <= h_ratio->GetNbinsX(); bin++) 
            {
                double count = h_ratio->GetBinContent(bin);
                double bin_err = h_ratio->GetBinError(bin);
                double std = h_std->GetBinContent(bin);
                double chi2 = (count - p0)*(count - p0);
                double sigma = bin_err*bin_err + std*std;

                logout << "bin " << bin << " : count = " << count <<endl;
                logout << " bin_err = " << bin_err << endl;
                logout << " std = " << std << endl;

                if(sigma <=0 )continue;
                chi2_sum += chi2/sigma;
                logout << " err_total = " << sigma << endl;
            }

            logout << "----------------------------------" << endl;
            logout << "chi2 = " << chi2_sum << endl;
            logout << "chi2/NDF = " << chi2_sum/NDF << endl;
            logout << "----------------------------------" << endl;

            gr->SetPoint(l,l,chi2_sum/NDF);
            l++;
            delete h_ratio;
        }

        gr->GetXaxis()->SetLimits(-0.5, runnums.size()-0.5);

        if(i == 0)c->Print((outpdf + "[").Data());

        c->cd();
        //c->SetLogy();
        c->Clear();
        gr->Draw("AP");
        gPad->Update();

        double ymin = gPad->GetUymin();
        double ymax = gPad->GetUymax();

        for (int j = 0; j < runnums.size(); j++) {
            latex.DrawLatex(j,
                            ymin - 0.08*(ymax-ymin),
                            Form("%d", runnums[j]));
        }

        c->Print(outpdf.Data());

        if(i == hname.size()-1)
            {
                
                c->Print((outpdf + "]").Data());
            }

        fin_ratio->Close();
        delete fin_ratio;
        fout->Close();
        delete fout;
        delete gr;
        cout<< hname[i] << "All done!!" <<endl;
    }
        fin_std->Close();
        delete fin_std;
        cout<< "All ok!!!!!!" <<endl;
}

