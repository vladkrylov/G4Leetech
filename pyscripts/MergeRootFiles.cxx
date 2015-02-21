void MergeRootFiles()
{
    const char *TreeName = "T";
    const int numberOfFiles = 23;
    char* mergeFilenames[numberOfFiles] = {
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun010.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun015.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun023.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun003.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun022.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun020.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun014.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun011.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun012.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun005.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun013.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun018.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun009.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun001.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun004.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun002.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun017.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun008.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun019.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun006.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun021.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun016.root",
"/home/vlad/5g4work/LeetechRuns/new_geometry_test/LeetechRun007.root"};

    TFile **f = new TFile*[numberOfFiles];
    TTree **t = new TTree*[numberOfFiles];
    TList treeList;

    for (int i = 0; i < numberOfFiles; ++i) {
		f[i] = new TFile(mergeFilenames[i]);
    	t[i] = (TTree *)f[i]->Get(TreeName);
    	treeList.Add(t[i]);
	}

    TFile resultFile("/home/vlad/5g4work/LeetechRuns/new_geometry_test/Result.root", "RECREATE");

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
