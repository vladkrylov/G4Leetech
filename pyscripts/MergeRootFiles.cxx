void MergeRootFiles()
{
    const char *TreeName = "Hits";
    const int numberOfFiles = 24;
    char* mergeFilenames[numberOfFiles] = {
"/home/vlad/10g4work/LeetechRuns/test/leetech_t11.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t19.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t16.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t14.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t3.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t4.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t18.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t13.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t2.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t12.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t20.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t22.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t0.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t23.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t21.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t6.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t10.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t9.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t7.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t5.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t17.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t8.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t1.root",
"/home/vlad/10g4work/LeetechRuns/test/leetech_t15.root"};

    TFile **f = new TFile*[numberOfFiles];
    TTree **t = new TTree*[numberOfFiles];
    TList treeList;

    for (int i = 0; i < numberOfFiles; ++i) {
		f[i] = new TFile(mergeFilenames[i]);
    	t[i] = (TTree *)f[i]->Get(TreeName);
    	treeList.Add(t[i]);
	}

    TFile resultFile("/home/vlad/10g4work/LeetechRuns/test/Result.root", "RECREATE");

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
