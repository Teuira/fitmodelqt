#ifndef QUERYPARSER_H
#define QUERYPARSER_H

extern "C" {
#include "fitmodel/utilities.h"
}
#include <iostream>
#include <vector>

class QueryParser
{
public:
    QueryParser(std::string query, std::vector<std::vector<node *> *> *nodes, size_t currTreeIndex);
    QueryParser(std::string query, std::vector<edge *> *roots);
    void Parse(std::vector<int> *positions);

private:
    std::string query;
    bool wholeTree;
    std::vector<std::vector<node *> *> *nodes;
    std::vector<edge *> *roots;
    size_t currTreeIndex;
    void Eq(std::vector<int> *positions);
    void Diff(std::vector<int> *positions);
    void Sim(std::vector<int> *positions);
    void Test(std::vector<int> *positions);

};

#endif // QUERYPARSER_H
