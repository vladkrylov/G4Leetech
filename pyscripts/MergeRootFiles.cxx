void MergeRootFiles()
{
    const char *TreeName = "T";
    const int numberOfFiles = 24;
    char* mergeFilenames[numberOfFiles] = {
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun010.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun015.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun023.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun003.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun022.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun020.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun014.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun011.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun012.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun005.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun013.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun018.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun009.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun001.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun004.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun002.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun017.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun000.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun008.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun019.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun006.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun021.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun016.root",
"/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/LeetechRun007.root"};

    TFile **f = new TFile*[numberOfFiles];
    TTree **t = new TTree*[numberOfFiles];
    TList treeList;

    for (int i = 0; i < numberOfFiles; ++i) {
		f[i] = new TFile(mergeFilenames[i]);
    	t[i] = (TTree *)f[i]->Get(TreeName);
    	treeList.Add(t[i]);
	}

    TFile resultFile("/home/vlad/5g4work/LeetechRuns/B_field_csan_Target=2mm_5.0 MeV_B=310 G/Result.root", "RECREATE");

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
