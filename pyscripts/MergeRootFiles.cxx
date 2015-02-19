void MergeRootFiles()
{
    const char *TreeName = "T";
    const int numberOfFiles = 8;
    char* mergeFilenames[numberOfFiles] = {
"/home/vlad/5g4work/LeetechRuns/new_test5/LeetechRun003.root",
"/home/vlad/5g4work/LeetechRuns/new_test5/LeetechRun005.root",
"/home/vlad/5g4work/LeetechRuns/new_test5/LeetechRun001.root",
"/home/vlad/5g4work/LeetechRuns/new_test5/LeetechRun004.root",
"/home/vlad/5g4work/LeetechRuns/new_test5/LeetechRun002.root",
"/home/vlad/5g4work/LeetechRuns/new_test5/LeetechRun000.root",
"/home/vlad/5g4work/LeetechRuns/new_test5/LeetechRun006.root",
"/home/vlad/5g4work/LeetechRuns/new_test5/LeetechRun007.root"};

    TFile **f = new TFile*[numberOfFiles];
    TTree **t = new TTree*[numberOfFiles];
    TList treeList;

    for (int i = 0; i < numberOfFiles; ++i) {
		f[i] = new TFile(mergeFilenames[i]);
    	t[i] = (TTree *)f[i]->Get(TreeName);
    	treeList.Add(t[i]);
	}

    TFile resultFile("/home/vlad/5g4work/LeetechRuns/new_test5/Result.root", "RECREATE");

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
