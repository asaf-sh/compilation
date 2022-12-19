//
// Created by efrat on 12/17/2022.
//

#ifndef COMPILATION_SYMBOLTABLE_H
#define COMPILATION_SYMBOLTABLE_H

#include <memory>
#include <vector>
#include <string>
#include "Expression.h"
#include "Program.h"


using namespace std;

class SymbolTable {
    int max_offset;
    vector<shared_ptr<Expression>> expressions;
public:
    SymbolTable& operator=(const SymbolTable&) = delete;
    bool isExist(const string& id){
        return this->table[id] != nullptr;
    }
    shared_ptr<Expression> get(const string& id) {
        for (auto const &exp : this->expressions) {
            if (exp->name == id){
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
    static void addFuncToSymbolTable(Type return_type, const string& id, int line_no, bool add_to_scope) {
        Program& program = Program::getInstance();
        if (program.isInGlobalScope(id)) {
            //error def
            //exit(0);
        }
        shared_ptr<Func> func_ptr = shared_ptr<Func>(new Func(return_type, id));
        program.addToGlobalScope(func_ptr);
        if (add_to_scope) {
            //Scope& curr_scope = program.scopes.back();
            //curr_scope.func = func_ptr;
            //curr_scope.symbols.max_offset = 0;
        }
    }
};

#endif //COMPILATION_SYMBOLTABLE_H