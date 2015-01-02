void UniteRoots()
{
    using namespace std;

    const char *TreeName = "T";
    
    TList tree_list;
    std::string filename;

    Int_t total_events = 0;

    TFile *f1 = new TFile("000micromegas_Run.root");
    TFile *f2 = new TFile("001micromegas_Run.root");
    TFile *f3 = new TFile("002micromegas_Run.root");
    TFile *f4 = new TFile("003micromegas_Run.root");

//    if(TTree *tree = (TTree *)f->Get(TreeName))
    TTree *tree1 = (TTree *)f1->Get(TreeName);
    tree_list.Add(tree1);
    TTree *tree2 = (TTree *)f2->Get(TreeName);
    tree_list.Add(tree2);
    TTree *tree3 = (TTree *)f3->Get(TreeName);
    tree_list.Add(tree3);
    TTree *tree4 = (TTree *)f4->Get(TreeName);
    tree_list.Add(tree4);

    //total_events += (Int_t )tree->GetEntries();


    //cout << "Opening output file: " << outfile << endl;
    TFile output("United.root", "RECREATE");

    cout << "Merging trees...patience..." << endl;
    TTree::MergeTrees(&tree_list);
    output.Write();
    output.Close();
    exit();
}
