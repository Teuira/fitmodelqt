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
    }
    else if (query == "diff") {
        Diff(positions);
    }
    else if (query.find("sim") == 0) {
        std::string val = query.substr(4, query.length() - 4);
        std::cout << val << std::endl;
        Sim(positions, std::stoi(val));
    }
    else if (query.find("has w1") == 0) {
           HasWx(positions, 0.0);
    }
    else if (query.find("has w2") == 0) {
           HasWx(positions, 0.5);
    }
    else if (query.find("has w3") == 0) {
        HasWx(positions, 1.0);
    }
    else if (query.find("from w1 to w2") == 0) {
        FromXToY(positions, 0.0, 0.5);
    }
    else if (query.find("from w1 to w3") == 0) {
        FromXToY(positions, 0.0, 1.0);
    }
    else if (query.find("from w2 to w1") == 0) {
        FromXToY(positions, 0.5, 0.0);
    }
    else if (query.find("from w2 to w3") == 0) {
        FromXToY(positions, 0.5, 1.0);
    }
    else if (query.find("from w3 to w1") == 0) {
        FromXToY(positions, 1.0, 0.0);
    }
    else if (query.find("from w3 to w2") == 0) {
        FromXToY(positions, 1.0, 0.5);
    }
    else {
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

void QueryParser::Sim(std::vector<int> *positions, int threshold)
{
    positions->clear();
    for (size_t i = 0; i < this->nodes->size(); i++) {
        int matchCount = 0;
        for (size_t y = 0; y < this->nodes->at(i)->size(); y++) {
            node *curr = this->nodes->at(i)->at(y);
            node *refNode = nodes->at(this->currTreeIndex)->at(y);
            if (curr->l_prime[0] == refNode->l_prime[0]) {
                matchCount++;
            }
        }
        if (static_cast<int>(this->nodes->at(i)->size()) - matchCount < threshold)
            positions->push_back(static_cast<int>(i));
    }
}

void QueryParser::HasWx(std::vector<int> *positions, double wVal)
{
    positions->clear();
    for (size_t i = 0; i < this->nodes->size(); i++) {
        for (size_t y = 0; y < this->nodes->at(i)->size(); y++) {
            node *curr = this->nodes->at(i)->at(y);
            if (curr->l_prime[0] == wVal) {
                positions->push_back(static_cast<int>(i));
                break;
            }
        }
    }
}

void QueryParser::FromXToY(std::vector<int> *positions, double xVal, double yVal)
{
    positions->clear();
    for (size_t i = 0; i < this->nodes->size(); i++) {
        for (size_t y = 0; y < this->nodes->at(i)->size(); y++) {
            node *curr = this->nodes->at(i)->at(y);
            node *child1 = curr->v[1];
            node *child2 = curr->v[2];
            if (curr->l_prime[0] == xVal) {
                if (child1) {
                    if (child1->l_prime[0] == yVal) {
                        positions->push_back(static_cast<int>(i));
                        break;
                    }
                }
                if (child2) {
                    if (child2->l_prime[0] == yVal) {
                        positions->push_back(static_cast<int>(i));
                        break;
                    }
                }
            }
        }
    }
}
