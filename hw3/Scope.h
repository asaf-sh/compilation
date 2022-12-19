//
// Created by efrat on 12/18/2022.
//

#ifndef COMPILATION_SCOPE_H
#define COMPILATION_SCOPE_H

#include <vector>
#include <string>
#include <memory>
#include "SymbolTable.h"
#include "Expression.h"
using namespace std;


class Scope {
    SymbolTable sym_table;
    bool is_in_while = false;
    shared_ptr<Func> function = nullptr;
public:
    Scope& operator=(const Scope&) = delete;

    Scope(int offset, bool is_in_while) : is_in_while(is_in_while){
        this->symbols.max_offset = offset;
    }

    bool isExist(const string& id) {
        if (this->sym_table.isExist(id)){
            return true;
        }
;
        if (this->function == nullptr) {
            return false;
        }
        return this->function->getArgById(id) != nullptr;
    }

    shared_ptr<Expression> get(const string& id) {
        shared_ptr<Expression> exp = this->sym_table.get(id);
        if (!exp){
            exp = this->func->getArgById(id);
        }
        return exp;
    }

    void setMaxOffsetOfSym(const int offset){
        this->sym_table->setMaxOffset(offset);
    }

    int getMaxOffsetOfSym(){
        return this->sym_table->getMaxOffset();
    }

    shared_ptr<Func> getFunc(){
        return this->function;
    }

    void printSymTableExps(){
        this->sym_table->print();
    }

};


class GlobalScope {
    vector<shared_ptr<Func>> functions;
public:

    GlobalScope& operator=(const GlobalScope&) = delete;

    bool isInScope(const string& id) {
        for (const auto& func : this->functions) {
            if (func->name == id) {
                return true;
            }
        }
        return false;
    }

    shared_ptr<Func>getById(const string& id) {
        for (auto func : this->functions) {
            if (func->getName() == id){
                return func;
            }
        }
        return nullptr;
    }

    void addFunc(shared_ptr<Func> func_ptr){
        this->functions->push_back(func_ptr);
    }

    void printFuncs(){
        for (const auto& func : functions) {
            func->print();
        }
    }
};
#endif //COMPILATION_SCOPE_H
