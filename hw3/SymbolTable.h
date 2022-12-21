//
// Created by efrat on 12/17/2022.
//

#ifndef COMPILATION_SYMBOLTABLE_H
#define COMPILATION_SYMBOLTABLE_H

#include <memory>
#include <vector>
#include <string>
#include "Expression.h"

using namespace std;

class SymbolTable {
private:
    int max_offset;
    vector<shared_ptr<Expression>> expressions;
public:
    SymbolTable& operator=(const SymbolTable&) = delete;

    bool isExist(const string& id){
        for (auto const &exp : this->expressions) {
            if (exp->getName() == id){
                return true;
            }
        }
        return false;
    }

    shared_ptr<Expression> get(const string& id) {
        for (auto const &exp : this->expressions) {
            if (exp->getName() == id){
                return exp;
            }
        }
        return nullptr;
    }

    void insert(const shared_ptr<Expression>& expression_ptr){
        //func offset always zero
        if (expression_ptr->isFunc())
            expression_ptr->setOffset(0);
        else
            expression_ptr->setOffset(this->max_offset);
        this->max_offset++; //check the ++ if its function? maybe it need to be inside the else
        this->expressions.push_back(expression_ptr);
    }

    void setMaxOffset(const int offset){
        this->max_offset = offset;
    }

    int getMaxOffset(){
        return this->max_offset;
    }

    void print(){
        for (auto const& exp : expressions) {
            exp->print();
        }
    }

};

#endif //COMPILATION_SYMBOLTABLE_H