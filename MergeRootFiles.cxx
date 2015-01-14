void MergeRootFiles(int numberOfFiles)
{
    using namespace std;

    const char *TreeName = "T";
    
    std::string filename;

    TFile **f = new TFile*[numberOfFiles];
    TTree **t = new TTree*[numberOfFiles];
    TList treeList;

    for (int i = 0; i < numberOfFiles; ++i) {
    	filename = "000micromegas_Run.root";
		f[i] = new TFile(filename.c_str());
    	t[i] = (TTree *)f[i]->Get(TreeName);
    	treeList.Add(t[i]);
	}

    TFile resultFile("Result.root", "RECREATE");

    cout << "Merging trees..." << endl;
    TTree::MergeTrees(&treeList);
    resultFile.Write();
    resultFile.Close();
    exit(0);
}
