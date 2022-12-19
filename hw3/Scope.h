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
    //shared_ptr<hw3_utils::Func> func = nullptr;
public:
    Scope& operator=(const Scope&) = delete;
    Scope(int offset, bool is_in_while) : is_in_while(is_in_while){
        this->symbols.max_offset = offset;
    }

    bool isExist(const string& id) {
        if (this->sym_table.isExist(id)){
            return true;
        }
        //hw3_utils::FuncPtr f_ptr = this->func;
        //if (this->func == nullptr) {
        //    return false;
        //}
        //return f_ptr->args[ID] != nullptr;
    }

    shared_ptr<Expression> get(const string& id) {
        shared_ptr<Expression> exp = this->sym_table.get(id);
        if (!exp){
            exp = this->func->getArgById(id);
        }
        return exp;
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
