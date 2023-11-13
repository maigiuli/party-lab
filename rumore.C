#include "TH2F.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLatex.h"
#include "TString.h"
#include "TCanvas.h"
#include <fstream>
#include <iostream>
#include <string>

void rumore() {
    // Crea un istogramma 2D con 6 righe e 4 colonne.
    TCanvas* c1 = new TCanvas("c1", "Canvas for Heatmap", 800, 600);
    c1->SetRightMargin(0.15);
    TH2F *h2 = new TH2F("h2", "Rumore dei canali", 4, 0, 4, 6, 0, 6);
    
    ifstream infile("rumore.txt");
    float value;
    TLatex latex;
    latex.SetTextAlign(22); // centra il testo

    for (int j=6; j>=1; j--) { 
        for (int i=1; i<=4; i++) {
            if (infile >> value) {
                h2->SetBinContent(i, j, value);
            
            }
        }
    }
    infile.close();

    const Int_t nRGBs = 5;
    Double_t stops[nRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[nRGBs]   = { 0.50, 0.50, 1.00, 1.00, 1.00 };
    Double_t green[nRGBs] = { 0.50, 1.00, 1.00, 0.60, 0.50 };
    Double_t blue[nRGBs]  = { 1.00, 1.00, 0.50, 0.40, 0.50 };
    TColor::CreateGradientColorTable(nRGBs, stops, red, green, blue, 255);
    gStyle->SetNumberContours(255);
    
    // Disegna l'istogramma.
    h2->SetZTitle("mV");
    gStyle->SetOptStat(0);
    h2->Draw("COLZ");

    for (int j=6; j>=1; j--) { 
        for (int i=1; i<=4; i++) {
            value = h2->GetBinContent(i, j);
            TString label;
            switch (i) {
                case 1: label = "A"; break;
                case 2: label = "B"; break;
                case 3: label = "C"; break;
                case 4: label = "D"; break;
            }
            label += std::to_string(j).c_str();

            if (value == 0) {
                label = "no";
            }
            latex.DrawLatex(i-0.5, j-0.5, label); 
        }
    }
    c1->Update(); 
    c1->SaveAs("heatmap.pdf");
    delete c1;
}
