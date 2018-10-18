#ifndef TREE_H
#define TREE_H

#include <string>
#include <stack>
#include <vector>

struct Node {
    std::string name;
    double dist;
    double val2;
    double totDist;
    int topoDist;
    Node *parent;
    std::stack<Node*> *children;
    std::vector<Node*> *childrens;
    double vPosLeaf;
};

class Tree
{
public:
    Tree(std::string name, std::string newick);
    ~Tree();
    Node* GetFarthestLeaf();
    size_t GetLeafCount();
    std::string GetName();
    std::string GetNewick();
    std::vector<Node*> *GetNonLeaves();
    std::vector<Node*> *GetLeaves();
    void PrintAllNodes();
    void PrintFarthestLeaf();
    void PrintLeavesBarChart(bool topo = false, size_t maxChar = 80);
    static void PrintNode(Node *node);
    static std::vector<Tree*> *LoadForest(std::string path);
private:
    std::string treeName;
    std::string newick;
    void CreateNodes(std::string newick);
    void CalcLengthsFillVectors(Node *node, int level, double initLen);
    Node *root;
    std::vector<Node*> *nodes;
    std::vector<Node*> *leaves;
    std::vector<Node*> *nonLeaves;
};

#endif // TREE_H
