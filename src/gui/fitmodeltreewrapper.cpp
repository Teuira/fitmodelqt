#include "fitmodeltreewrapper.h"
extern "C" {
#include "fitmodel/utilities.h"
}
#include <fstream>

FitModelTreeWrapper::FitModelTreeWrapper(std::string treeName, std::string newick)
{
    this->treeName = treeName;
    this->newick = newick;

    int i;
    int b_root_num = 0;

    this->tree = Read_Tree((char*)newick.c_str());
    For(i, 2 * this->tree->n_otu - 2) {
        if (this->tree->noeud[i]->check_branch > -1) {
            b_root_num = this->tree->noeud[i]->b[this->tree->noeud[i]->check_branch]->num;
            break;
        }
    }
    this->root = this->tree->t_edges[b_root_num];
    tree->fp_tree = nullptr;
    this->w = Make_Tdraw_Struct(tree);
    Draw_Tree(this->root, this->w, this->tree);

}

FitModelTreeWrapper::~FitModelTreeWrapper()
{
    Free_Tree(this->tree);
}

std::string FitModelTreeWrapper::GetName()
{
    return this->treeName;
}

arbre *FitModelTreeWrapper::GetTree()
{
    return this->tree;
}

edge *FitModelTreeWrapper::GetRoot()
{
    return this->root;
}

tdraw *FitModelTreeWrapper::GetDrawStruct()
{
    return this->w;
}

std::vector<FitModelTreeWrapper *> *FitModelTreeWrapper::LoadForest(std::string path)
{
    std::vector<FitModelTreeWrapper *> *res = new std::vector<FitModelTreeWrapper *>;
    std::ifstream infile(path);
    std::string line;

    while(std::getline(infile, line)) {
        size_t delimPos = line.find('=');
        std::string treeName = line.substr(0, delimPos - 1);
        std::string newick = line.substr(delimPos + 2, line.length() - delimPos);
        res->push_back(new FitModelTreeWrapper(treeName, newick));
    }
    return res;
}
