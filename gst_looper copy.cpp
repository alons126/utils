#include <TFile.h>
#include <TIterator.h>
#include <TObjArray.h>
#include <TSystem.h>
#include <TTree.h>

#include <iostream>

#include "gst.h"

void gst_looper(const char *filePattern = "*.root") {
    // Get the list of files matching the pattern
    TObjArray *fileList = gSystem->GetDirList(filePattern);

    if (fileList->GetEntries() == 0) {
        std::cerr << "No files found matching pattern: " << filePattern << std::endl;
        return;
    }

    // Loop over the files
    TIterator *iter = fileList->MakeIterator();
    TObject *obj = nullptr;

    while ((obj = iter->Next())) {
        const char *fileName = obj->GetName();

        // Open the ROOT file
        TFile *file = TFile::Open(fileName);
        if (!file || file->IsZombie()) {
            std::cerr << "Error opening file: " << fileName << std::endl;
            continue;
        }

        std::cout << "Processing file: " << fileName << std::endl;

        // Access a TTree from the file (change "tree_name" to the actual TTree
        // name)
        TTree *tree = (TTree *)file->Get("tree_name");
        if (!tree) {
            std::cerr << "Error: TTree not found in file: " << fileName << std::endl;
            file->Close();
            continue;
        }

        // Loop over the entries in the TTree
        Long64_t nEntries = tree->GetEntries();
        for (Long64_t entry = 0; entry < nEntries; ++entry) {
            tree->GetEntry(entry);

            // Here you can perform any analysis on the data in the TTree
            // For example, print an entry's value from a branch (assuming a branch
            // named "branch_name") Double var; tree->SetBranchAddress("branch_name",
            // &var); std::cout << "Value: " << var << std::endl;
        }

        // Close the file after processing
        file->Close();
    }

    // Clean up the iterator and file list
    delete iter;
    delete fileList;
}