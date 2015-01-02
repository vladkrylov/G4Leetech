{
	
  gROOT->Reset();
  gROOT->SetStyle("Plain");
TChain * nt_V1=new TChain("T");
TCanvas *a = new TCanvas("a","Z",5,85,800,800);
/*
a->Divide(3,2);
a->cd(1);

nt_V1->Add("micromegas_VisA1.root");
nt_V1->Draw("Energy","","", 914, 0);
//nt_V1->Draw("Energy:Theta","","", 54, 0);
//nt_V1->Draw("Energy:PosXY","","", 25, 0);

a->cd(2);
nt_V1->Add("micromegas_VisA2.root");
nt_V1->Draw("Energy","","", 914, 0);
//nt_V1->Draw("Energy:Theta","","", 54, 0);
//nt_V1->Draw("Energy:PosXY","","", 25, 0);

a->cd(3);
nt_V1->Add("micromegas_VisA3.root");
nt_V1->Draw("Energy","","", 914, 0);
//nt_V1->Draw("Energy:Theta","","", 54, 0);
//nt_V1->Draw("Energy:PosXY","","", 25, 0);

a->cd(4);
nt_V1->Add("micromegas_VisA4.root");
nt_V1->Draw("Energy","","", 914, 0);
//nt_V1->Draw("Energy:Theta","","", 54, 0);
//nt_V1->Draw("Energy:PosXY","","", 25, 0);

a->cd(5);
nt_V1->Add("micromegas_VisA5.root");
nt_V1->Draw("Energy","","", 914, 0);
//nt_V1->Draw("Energy:Theta","","", 54, 0);
//nt_V1->Draw("Energy:PosXY","","", 25, 0);

a->cd(6);
nt_V1->Add("micromegas_VisA6.root");
nt_V1->Draw("Energy","","", 914, 0);
//nt_V1->Draw("Energy:Theta","","", 54, 0);
//nt_V1->Draw("Energy:PosXY","","", 25, 0);
*/
nt_V1->Add("mic.Cal.0.2mm.200.root");
//nt_V1->Draw("Theta","Ptot>0.9 && Ptot<1.1","", 502772, 0);
	//TF1 *x1P300 = new TF1("xP","8*3.14*1*16*sin(x)*sin(x)",0,3.14);
const int nbin = 100;
TH1F * Theta1 = new TH1F("Theta1","Theta" ,nbin, 0, 100);
nt_V1->Draw("Theta>>Theta1","Ptot>0.9 && Ptot<1.1");

float x[nbin];
	float y[nbin];
	float si;
for(int i=0; i<nbin;i++)
	 {
		si = 1-(float)sin((double)i/180*3.14)*(float)sin((double)i/180*3.14);
		y[i]= (float)Theta1->GetBinContent(i)*(float)si;
		x[i]=(float) i;
		cout<<Theta1->GetBinContent(i)<<endl;
	}

TGraph *cc = new TGraph(nbin, x, y);
cc.Draw("A*L");

}
