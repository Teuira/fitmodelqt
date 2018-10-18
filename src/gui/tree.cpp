#include "tree.h"

#include <algorithm>
#include <iostream>
#include <fstream>

//#define PRINT_DEBUG

Tree::Tree(std::string name, std::string newick)
{
    this->treeName = name;
    this->newick = newick;
    this->nodes = new std::vector<Node*>;
    this->leaves = new std::vector<Node*>;
    this->nonLeaves = new std::vector<Node*>;

    newick += ":0::0";
    CreateNodes(newick);
#ifdef PRINT_DEBUG
    std::cout << this->root->name << ": 0.0" << std::endl;
#endif
    this->root->topoDist = 0;
    this->root->parent = nullptr;
    CalcLengthsFillVectors(this->root, 1, 0.0);
}

Tree::~Tree() {
    for (size_t i = 0;i < this->nodes->size(); i++) {
        if (this->nodes->at(i)->children != nullptr)
            delete this->nodes->at(i)->children;
        delete this->nodes->at(i)->childrens;
        delete this->nodes->at(i);
    }
    delete this->nodes;
    delete this->leaves;
    delete this->nonLeaves;
}

Node* Tree::GetFarthestLeaf()
{
    Node *res = this->root;
    for (size_t i = 0; i < this->leaves->size(); i++) {
        Node *currLeave = this->leaves->at(i);
        if (currLeave->totDist > res->totDist)
            res = currLeave;
    }
    return res;
}

size_t Tree::GetLeafCount()
{
    return this->leaves->size();
}

std::string Tree::GetName()
{
    return this->treeName;
}

std::string Tree::GetNewick()
{
    return this->newick;
}

std::vector<Node *> *Tree::GetNonLeaves()
{
    return this->nonLeaves;
}

std::vector<Node*> *Tree::GetLeaves()
{
    return this->leaves;
}

void Tree::PrintAllNodes()
{
    for (size_t i = 0; i < this->nodes->size(); i++) {
        Tree::PrintNode(this->nodes->at(i));
    }
}

void Tree::PrintFarthestLeaf()
{
    PrintNode(GetFarthestLeaf());
}

void Tree::PrintLeavesBarChart(bool topo, size_t maxChar)
{
    if (topo)
        for (size_t i = 0; i < this->leaves->size(); i++) {
            Node *currLeave = this->leaves->at(i);
            std::cout << std::string(currLeave->topoDist, '-') << currLeave->name << std::endl;
        }
    else {
        double maxLen = GetFarthestLeaf()->totDist;
        // totDist : maxLen = x : maxChar
        for (size_t i = 0; i < this->leaves->size(); i++) {
            Node *currLeave = this->leaves->at(i);
            std::cout << std::string(currLeave->totDist * (double)maxChar / maxLen, '-') << currLeave->name << std::endl;
        }
    }
}

void Tree::PrintNode(Node *node)
{
    std::cout << "NODE START" << std::endl;
    std::cout << "Node name: " << node->name << std::endl;
    std::cout << "Distance: " << node->totDist << std::endl;
    std::cout << "NODE END" << std::endl;
}

std::vector<Tree*> *Tree::LoadForest(std::string path)
{
    std::vector<Tree*> *res = new std::vector<Tree*>;
    std::ifstream infile(path);
    std::string line;

    while(std::getline(infile, line)) {
        size_t delimPos = line.find('=');
        std::string treeName = line.substr(0, delimPos - 1);
        std::string newick = line.substr(delimPos + 2, line.length() - delimPos);
        res->push_back(new Tree(treeName, newick));
    }
    return res;
}

void Tree::CreateNodes(std::string newick) {
    std::stack<std::stack<Node*>*> *stack = new std::stack<std::stack<Node*>*>;
    std::stack<Node*> *root = new std::stack<Node*>;
    std::stack<Node*> *node = root;
    Node *child = nullptr;

    for (int i = (int)newick.length() - 1; i >=0; i--) {
        switch(newick[i]) {
        case ')':
            // ')' => begin child node
            stack->push(node);
            child->children = new std::stack<Node*>;
            node = child->children;
            break;

        case '(':
            // '(' => end of child node
            node = stack->top();
            stack->pop();
            break;

        case ' ':
        case ',':
            // ',' and '' => separators (ignored)
            break;

        default:
            // assume all other characters are node names
            // Gets attributes
            int i2 = i;
            std::string token = "";
            token += (char)newick[i];
            while (i2 >= 0) {
                i2--;
                char curr = (char)newick[i2];
                if (curr == ' ' || curr == ',' || curr == '(' || curr == ')') {
                    break;
                }
                token += curr;
            }
            i = ++i2;
            std::reverse(token.begin(), token.end());
            size_t nameEnd = token.find_first_of(':');
            std::string name = token.substr(0, nameEnd);
            size_t lenEnd = token.find_first_of(':', nameEnd + 1);
            std::string len = token.substr(nameEnd + 1, lenEnd - nameEnd - 1);
            std::string value2 = token.substr(lenEnd+2);
            child = new Node;
            child->name = name;
            child->children = nullptr;
            child->dist = std::stod(len);
            child->val2 = std::stod(value2);
            node->push(child);
            break;
        }
    }
    this->root = root->top();
}

void Tree::CalcLengthsFillVectors(Node *node, int level, double initLen) {
    static int vPosLeaf = 0;
    this->nodes->push_back(node);
    if (node->name != ";" && node->name != "") {
        node->vPosLeaf = vPosLeaf++;
        this->leaves->push_back(node);
    }
    else {
        if (node->name == "")
            this->nonLeaves->push_back(node);
    }
    node->childrens = new std::vector<Node*>;
    if (node->children == nullptr)
        return;

    while (!node->children->empty()) {
        Node* curr = node->children->top();
        curr->parent = node;
        node->childrens->push_back(curr);
        node->children->pop();
        curr->topoDist = level;
        curr->totDist = curr->dist + initLen;
        std::string len = "";
        for (int i = 0; i < level; i++) len += '-';
#ifdef PRINT_DEBUG
        std::cout << len << curr->name << ": " << curr->totDist << std::endl;
#endif
        CalcLengthsFillVectors(curr, level + 1, curr->totDist);
    }
}
