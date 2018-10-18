#include "queryparser.h"

#include <QMessageBox>

QueryParser::QueryParser(std::string query, std::vector<std::vector<node *> *> *nodes, size_t currTreeIndex)
{
    this->query = query;
    this->nodes = nodes;
    this->roots = nullptr;
    this->wholeTree = false;
    this->currTreeIndex = currTreeIndex;
}

QueryParser::QueryParser(std::string query, std::vector<edge *> *roots)
{
    this->query = query;
    this->nodes = nullptr;
    this->roots = roots;
    this->wholeTree = true;
}

void QueryParser::Parse(std::vector<int> *positions)
{
    if (query == "eq") {
        Eq(positions);
    } else if (query == "diff") {
        Diff(positions);
    }
    else if (query == "test") {
            Test(positions);
        }
    else if (query.find("sim") == 0) {
        Sim(positions);
    } else {
        QMessageBox msgBox;
        msgBox.setText("Unrecognized query!");
        msgBox.exec();
    }
}

void QueryParser::Eq(std::vector<int> *positions)
{
    positions->clear();
    for (size_t i = 0; i < this->nodes->size(); i++) {
        size_t matchCount = 0;
        for (size_t y = 0; y < this->nodes->at(i)->size(); y++) {
            node *curr = this->nodes->at(i)->at(y);
            node *refNode = nodes->at(this->currTreeIndex)->at(y);
            if (curr->l_prime[0] == refNode->l_prime[0]) {
                matchCount++;
            }
        }
        if (matchCount == this->nodes->at(i)->size())
            positions->push_back(static_cast<int>(i));
    }
}

void QueryParser::Diff(std::vector<int> *positions)
{
    positions->clear();
    for (size_t i = 0; i < this->nodes->size(); i++) {
        size_t matchCount = 0;
        for (size_t y = 0; y < this->nodes->at(i)->size(); y++) {
            node *curr = this->nodes->at(i)->at(y);
            node *refNode = nodes->at(this->currTreeIndex)->at(y);
            if (curr->l_prime[0] == refNode->l_prime[0]) {
                matchCount++;
            }
        }
        if (matchCount < this->nodes->at(i)->size())
            positions->push_back(static_cast<int>(i));
    }
}

void QueryParser::Sim(std::vector<int> *positions)
{

}

void QueryParser::Test(std::vector<int> *positions)
{
    positions->clear();
    positions->push_back(20);
    positions->push_back(40);
}
