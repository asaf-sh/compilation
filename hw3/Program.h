//
// Created by efrat on 12/18/2022.
//

#ifndef COMPILATION_PROGRAM_H
#define COMPILATION_PROGRAM_H

#include "Scope.h"
#include "Expression.h."
#include "SymbolTable.h"
#include "hw3_output.hpp"

using namespace std;

class Program {
    vector<Scope> scopes;
    GlobalScope global_scope;
    vector<shared_ptr<Expression>> tmp_exp;
    Program() = default;
public:
    static Program& getInstance() {
        static Program instance = Prog();
        return instance;
    }

    static void checkIfMainExists() {
        Program& program = Program::getInstance();
        shared_ptr<Func> main_ptr = program.global_scope.getById("main");
        if (!main_ptr || main_ptr->getType() != VOID || main_ptr->getArgsSize()!= 0 ) {
           // error Main is Missing
        }
    }

    static void openGlobalScope(const int line_no) {
        Program& program = getInstance();
        //print and printi should be added when the global scope opens
        shared_ptr<Expression> id = shared_ptr<Expression>(new Expression("print", VOID));
        shared_ptr<Expression> return_type = shared_ptr<Expression>(new Expression(VOID));
        SymbolTable::addFuncToSymbolTable(return_type->getType(), id->getName(), line_no, false);
        //print func gets String
        program.addArgToFunc( "", STRING, line_no);

        id = shared_ptr<Expression>((new Expression("printi", VOID));
        SymbolTable::addFuncToSymbolTable(return_type->getType(), id->getName(), line_no, false);
        program.addArgToFunc("", INT, line_no);
    }

    static void closeGlobalScope() {
        Program& program = getInstance();
        output::endScope();
        program.global_scope.printFuncs();
    }

    static void openScope(bool is_in_while) {
        Program& program = getInstance();
        if (program.scopes.size() == 0) {
            program.scopes.emplace_back(0, is_in_while);
            return;
        }
        Scope& scope = program.scopes.back();
        program.scopes.emplace_back(scope.getMaxOffsetOfSym(), is_in_while);
    }

    static void closeScope() {
        Program& program = getInstance();
        output::endScope();
        shared_ptr<Func> func = program.scopes.back().getFunc();
        if (func) {
            func->printExpressionsOnly();
        }
        program.scopes.back().printSymTableExps();
        program.scopes.pop_back();
    }

    static void addArgToFunc(string name, Type type, int line_no){
        Program &program = Program::getInstance();
        if(this->getExpById(name)) {
            //error def
        }
        shared_ptr<Expression> arg = shared_ptr<Expression>(new Expression(name,type));
        shared_ptr<Func> func = program.global_scope->getLastFunc();
        arg->setOffset((func->getArgsSize()+1)*(-1));
        func->addArg(arg);
    }

    shared_ptr<Expression> getExpById(const string& id) {
        for (auto& scope : scopes) {
            if (scope.isInScope(id)){
                return scope.get(id);
            }
        }
        return global_scope.isInScope(id);
    }

    bool isInGlobalScope(const string id){
        return this->global_scope.isInScope(id);
    }

    void addToGlobalScope(shared_ptr<Func> func_ptr){
        this->global_scope->addFunc(func_ptr);
    }

    Scope& getLastScope(){
        return this->scopes.back();
    }
};

#endif //COMPILATION_PROGRAM_H
