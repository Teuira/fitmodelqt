#ifndef FITMODELTREEWRAPPER_H
#define FITMODELTREEWRAPPER_H

#include <iostream>
#include <vector>

extern "C" {
#include "fitmodel/config.h"
#include "fitmodel/utilities.h"
#include "fitmodel/draw.h"
#include "fitmodel/free.h"
}

class FitModelTreeWrapper
{
public:
    FitModelTreeWrapper(std::string treeName, std::string newick);
    ~FitModelTreeWrapper();
    static std::vector<FitModelTreeWrapper *> *LoadForest(std::string path);
    std::string GetName();
    arbre *GetTree();
    edge *GetRoot();
    tdraw *GetDrawStruct();

private:
    std::string treeName;
    std::string newick;
    arbre *tree;
    edge *root;
    tdraw *w;
};


#endif // FITMODELTREEWRAPPER_H
