void MergeRootFiles()
{
    const char *TreeName = "Hits";
    const int numberOfFiles = 4;
    char* mergeFilenames[numberOfFiles] = {
"/home/vlad/10g4work/LeetechRuns/scan/d=4mm_E=3500Mev_B=100G/leetech_t3.root",
"/home/vlad/10g4work/LeetechRuns/scan/d=4mm_E=3500Mev_B=100G/leetech_t2.root",
"/home/vlad/10g4work/LeetechRuns/scan/d=4mm_E=3500Mev_B=100G/leetech_t0.root",
"/home/vlad/10g4work/LeetechRuns/scan/d=4mm_E=3500Mev_B=100G/leetech_t1.root"};

    TFile **f = new TFile*[numberOfFiles];
    TTree **t = new TTree*[numberOfFiles];
    TList treeList;

    for (int i = 0; i < numberOfFiles; ++i) {
		f[i] = new TFile(mergeFilenames[i]);
    	t[i] = (TTree *)f[i]->Get(TreeName);
    	treeList.Add(t[i]);
	}

    TFile resultFile("/home/vlad/10g4work/LeetechRuns/scan/d=4mm_E=3500Mev_B=100G/Result.root", "RECREATE");

    TTree::MergeTrees(&treeList);
    resultFile.Write();
    resultFile.Close();

    for (int i = 0; i < numberOfFiles; ++i) {
    	delete t[i];
    	delete f[i];
    }
    delete[] t;
    delete[] f;

    exit(0);
}
