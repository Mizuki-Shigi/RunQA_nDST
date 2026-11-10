#include <vector>
#include <map>
using namespace std;

// 平均ヒストグラムを作る関数
TH1* MakeAverageHist(
    const string& histName,
    const vector<int>& runs,
    const string& indir
)
{
    TH1* hmean = nullptr;
    int nUsed = 0;
    double nevts = 0.0;

    for (int run : runs)
    {
        string infile = indir + to_string(run) + "_merged.root";
        TFile* fin = TFile::Open(infile.c_str(), "READ");

        if (!fin || fin->IsZombie())
        {
            cout << "Cannot open: " << infile << endl;
            if (fin) delete fin;
            continue;
        }

        TH1* h = dynamic_cast<TH1*>(fin->Get(histName.c_str()));
        TH1* hevt = dynamic_cast<TH1*>(fin->Get("h_zvtx"));
        double runNevts = hevt->GetEntries();
        nevts += runNevts;
        cout << run <<":"<<runNevts<<endl;

        if (hmean == nullptr)
        {
            // 最初に見つかったヒストグラムをコピーする
            hmean = dynamic_cast<TH1*>(h->Clone(histName.c_str()));
            hmean->SetDirectory(nullptr);
        }
        else
        {
            hmean->Add(h);
        }
        nUsed++;

        fin->Close();
        delete fin;
    }
    cout<<"All evnt: "<<nevts<<endl;

    if (nUsed == 0)
    {
        cout << "No histograms found: " << histName << endl;
        return nullptr;
    }

    hmean->Scale(1.0 / nevts);
    return hmean;
}

// RUNごとのばらつきを計算する関数
TH1* Makestd(
    const string& histName,
    const vector<int>& runs,
    const string& indir_nomali
)
{
    TH1* hmean = nullptr;
    TH1* hsum2 = nullptr;
    int nUsed = 0;

    for (int run : runs)
    {
        string infile_nomali = indir_nomali + "nomali_" + to_string(run) + "_merged.root";
        TFile* fin_nomali = TFile::Open(infile_nomali.c_str(), "READ");

        if (!fin_nomali || fin_nomali->IsZombie())
        {
            cout << "Cannot open: " << infile_nomali << endl;
            if (fin_nomali) delete fin_nomali;
            continue;
        }

        TH1* h = dynamic_cast<TH1*>(fin_nomali->Get(histName.c_str()));

        if (hmean == nullptr)
        {
            // 最初に見つかったヒストグラムをコピーする
            hmean = dynamic_cast<TH1*>(h->Clone(histName.c_str()));
            hmean->SetDirectory(nullptr);
            hsum2 = dynamic_cast<TH1*>(h->Clone(histName.c_str()));
            hsum2->SetDirectory(nullptr);

            for (int bin = 0; bin <= h->GetNbinsX() + 1; bin++)
            {
                double value = h->GetBinContent(bin);
                hsum2->SetBinContent(bin, value * value);
            }
        }
        else
        {
            hmean->Add(h);
            for (int bin = 0; bin <= h->GetNbinsX() + 1; bin++)
            {
                double value = h->GetBinContent(bin);
                double sum2 = hsum2->GetBinContent(bin);
                hsum2->SetBinContent(bin, sum2 + value * value);
            }
        }
        nUsed++;

        fin_nomali->Close();
        delete fin_nomali;
    }

    if (nUsed == 0)
    {
        cout << "No histograms found: " << histName << endl;
        return nullptr;
    }

    hmean->Scale(1.0 / nUsed);

    TH1* hstd = (TH1*)hmean->Clone((histName + "_std").c_str());
    hstd->SetDirectory(nullptr);

    for (int bin = 0; bin <= hmean->GetNbinsX() + 1; bin++)
    {
        double mean = hmean->GetBinContent(bin);
        double mean2 = hsum2->GetBinContent(bin) / nUsed;

        double variance = mean2 - mean * mean;

        // 浮動小数点の誤差による負値を防ぐ
        if (variance < 0)
        {
            variance = 0;
        }

        hstd->SetBinContent(bin, sqrt(variance));
        hstd->SetBinError(bin, 0);
    }
    delete hmean;
    delete hsum2;

    return hstd;
}

TH1* MakeRatioHist(
    const string& histName,
    int run,
    TH1* hmean,
    const string& indir_nomali
)
{
    string infile = indir_nomali + "nomali_" + to_string(run) + "_merged.root";
    TFile* fin_nomali = TFile::Open(infile.c_str(), "READ");

    if (!fin_nomali || fin_nomali->IsZombie())
    {
        cerr << "Cannot open " << infile << endl;

        if (fin_nomali) delete fin_nomali;
        return nullptr;
    }

    // 元のヒストグラムを取得
    TH1* h = (TH1*)fin_nomali->Get(histName.c_str());
    // 元のヒストグラムをコピー
    string ratioName = histName + "_" + to_string(run);

    TH1* hratio = (TH1*)h->Clone(ratioName.c_str());
    hratio->SetDirectory(nullptr);

    // 平均ヒストグラムでbinごとに割る
    hratio->Divide(hmean);

    // 入力ファイルを閉じる
    fin_nomali->Close();
    delete fin_nomali;

    return hratio;
}


void makeaverage()
{
    string indir =
        "/sphenix/tg/tg01/commissioning/INTT/work/mizuki/"
        "RunQA/makehist/result/merged_data/";
    string indir_nomali =
        "/sphenix/tg/tg01/commissioning/INTT/work/mizuki/"
        "RunQA/makenomali/result/data/";
    string outdir = "/sphenix/tg/tg01/commissioning/INTT/work/mizuki/RunQA/makeaverage/result/";
    string outfile = outdir + "average.root";

    TFile* fout = TFile::Open(outfile.c_str(), "RECREATE");

    map<string, vector<int>> runConfig = {
        // {"h_EMCal_phi", {53018, 53046, 53741, 53742}},
        // {"h_EMCal_z", {53018, 53046, 53741}},
        // {"h_TopoClus_phi", {53741, 53742, 53743}},
        // {"h_TopoClus_z", {53018, 53046, 53741, 53742, 53743}},

        // {"h_MVTX_Inner_phi", {53018, 53046, 53741, 53742}},
        // {"h_MVTX_Inner_z", {53018, 53046, 53741}},
        // {"h_MVTX_Middle_phi", {53741, 53742, 53743}},
        // {"h_MVTX_Middle_z", {53018, 53046, 53741, 53742, 53743}},
        // {"h_MVTX_Outer_phi", {53018, 53046, 53741, 53742}},
        // {"h_MVTX_Outer_z", {53018, 53046, 53741}},

        // {"h_INTT_Inner_phi", {53741, 53742, 53743}},
        // {"h_INTT_Inner_z", {53018, 53046, 53741, 53742, 53743}},
        // {"h_INTT_Outer_phi", {53018, 53046, 53741, 53742}},
        // {"h_INTT_Outer_z", {53018, 53046, 53741}},

        // {"h_AllMVTX_Inner_phi", {53741, 53742, 53743}},
        // {"h_AllMVTX_Inner_z", {53018, 53046, 53741, 53742, 53743}},
        // {"h_AllMVTX_Middle_phi", {53018, 53046, 53741, 53742}},
        // {"h_AllMVTX_Middle_z", {53018, 53046, 53741}},
        // {"h_AllMVTX_Outer_phi", {53741, 53742, 53743}},
        // {"h_AllMVTX_Outer_z", {53018, 53046, 53741, 53742, 53743}},

        {"h_AllINTT_Inner_phi", {53046,53741,53742,53783,53871,53877}},
        // {"h_AllINTT_Inner_z", {53018, 53046, 53741, 53742, 53743}},
        // {"h_AllINTT_Outer_phi", {53018, 53046, 53741, 53742}},
        // {"h_AllINTT_Outer_z", {53018, 53046, 53741}},

        // {"h_zvtx", {53018, 53046, 53741, 53742}},
        // {"h_trkz0", {53018, 53046, 53741}}
    };

    vector<int> allRuns = 
    {
        53018, 53046, 53741, 53742, 53743, 53744, 53756, 53783, 53871, 53876, 53877, 53879
    };

    // 設定したヒストグラムを順番に処理
    for (const auto& item : runConfig)
    {
        const string& histName = item.first;
        const vector<int>& runs = item.second;

        cout << "Processing: " << histName << endl;

        TH1* hmean = MakeAverageHist(histName, runs, indir);
        if (!hmean)
        {
            cerr << "Failed to make average: " << histName << endl;

            delete hmean;
            continue;
        }

        TH1* hstd = Makestd(histName, runs, indir_nomali);
        if (!hstd)
        {
            cerr << "Failed to make std: " << histName << endl;

            delete hstd;
            continue;
        }

        string outfile = outdir  + histName + ".root";

        TFile* fratio = TFile::Open(outfile.c_str(), "RECREATE");

        for (int run : allRuns)
        {
            TH1* hratio = MakeRatioHist(
                histName,
                run,
                hmean,
                indir_nomali
            );

            fratio->cd();
            hratio->Write();

            delete hratio;
        }
        fratio->Close();
        delete fratio;

        fout->cd();
        hmean->Write();
        hstd->Write();
        delete hmean;
        delete hstd;
    }

    fout->Close();
    delete fout;

    cout << "All ok!!!!!!" << endl;
}