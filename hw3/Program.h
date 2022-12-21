//
// Created by efrat on 12/18/2022.
//

#ifndef COMPILATION_PROGRAM_H
#define COMPILATION_PROGRAM_H

#include "Scope.h"
#include "Expression.h"
#include "SymbolTable.h"
#include "hw3_output.hpp"
#include "hw3_output.hpp"
#include <iostream>

using namespace std;

class Program {
private:
    vector<Scope> scopes;
    GlobalScope global_scope;
    vector<shared_ptr<Expression>> tmp_exp;
    Program() = default;
public:
    static Program& getInstance() {
        static Program instance = Program();
        return instance;
    }
    static void checkIfMainExists_p() {
        cout << "----------------checkIfMainExist" <<  endl;
        Program& program = Program::getInstance();
        shared_ptr<Func> main_ptr = program.global_scope.getById("main");
        if (!main_ptr || main_ptr->getType() != Ex3_type::VOID || main_ptr->getArgsSize()!= 0 ) {
            output::errorMainMissing();
        }
        cout << "----------------checkIfMainExist end" <<  endl;
    }

    static void addFuncToSymbolTable_p(Ex3_type::Type return_type, const string& id, int line_no, bool add_to_scope) {
        cout << "----------------add func to symbl" <<  endl;
        Program& program = Program::getInstance();
        if (program.isInGlobalScope(id)) {
            output::errorDef(line_no,id);
            exit(0);
        }
        shared_ptr<Func> func_ptr = shared_ptr<Func>(new Func(return_type, id));
        program.addToGlobalScope(func_ptr);
        if (add_to_scope) {
            Scope& curr_scope = program.getLastScope();
            curr_scope.setFunc(func_ptr);
            curr_scope.setMaxOffsetOfSym(0);
        }
        cout << "----------------add func to symbl end" <<  endl;
    }

    static void openGlobalScope_p(const int line_no) {
        cout << "----------------openGlobal" <<  endl;
        Program& program = getInstance();
        //print and printi should be added when the global scope opens
        shared_ptr<Expression> id = shared_ptr<Expression>(new Expression("print", Ex3_type::VOID));
        shared_ptr<Expression> return_type = shared_ptr<Expression>(new Expression(Ex3_type::VOID));
        addFuncToSymbolTable_p(return_type->getType(), id->getName(), line_no, false);
        //print func gets String
        program.addArgToFunc_p( "", Ex3_type::STRING, line_no);

        id = shared_ptr<Expression>(new Expression("printi", Ex3_type::VOID));
        addFuncToSymbolTable_p(return_type->getType(), id->getName(), line_no, false);
        program.addArgToFunc_p("", Ex3_type::INT, line_no);
        cout << "----------------openGlobal end" <<  endl;
    }

    static void closeGlobalScope_p() {
        cout << "----------------closeGlobal" <<  endl;
        Program& program = getInstance();
        output::endScope();
        program.global_scope.printFuncs();
        cout << "----------------closeGlobal  end" <<  endl;
    }

    static void openScope_p(bool is_in_while) {
        cout << "----------------openScope" <<  endl;
        Program& program = getInstance();
        if (program.scopes.size() == 0) {
            program.scopes.emplace_back(0, is_in_while);
            return;
        }
        Scope& scope = program.scopes.back();
        program.scopes.emplace_back(scope.getMaxOffsetOfSym(), is_in_while);
        cout << "----------------openScope -- end" <<  endl;
    }

    static void closeScope_p() {
        cout << "----------------closeScope" <<  endl;
        Program& program = getInstance();
        output::endScope();
        shared_ptr<Func> func = program.scopes.back().getFunc();
        if (func) {
            func->printExpressionsOnly();
        }
        program.scopes.back().printSymTableExps();
        program.scopes.pop_back();
        cout << "----------------closeScope end" <<  endl;
    }

    static void addArgToFunc_p(string name, Ex3_type::Type type, int line_no){
        cout << "----------------add arg to func" <<  endl;
        Program &program = Program::getInstance();
        if(getExpById_p(name)) {
            output::errorDef(line_no, name);
        }
        shared_ptr<Expression> arg = shared_ptr<Expression>(new Expression(name,type));
        shared_ptr<Func> func = program.global_scope.getLastFunc();
        arg->setOffset((func->getArgsSize()+1)*(-1));
        func->addArg(arg);
        cout << "----------------add arg to func end" <<  endl;
    }

    static shared_ptr<Expression> getExpById_p(const string& id){
    cout << "----------------getExpByID" <<  endl;
        Program& program = getInstance();
        for (auto& scope : program.scopes) {
            if (scope.isExist(id)){
                cout << "----------------getExpByID end" <<  endl;
                return scope.get(id);
            }
        }
        cout << "----------------getExpByID end" <<  endl;
        return program.global_scope.isInScopeAndRet(id);
    }

    static void addToScope_p(Ex3_type::Type type, const string& name) {
        cout << "----------------addToScope_p " <<  endl;
        shared_ptr<Expression> exp = shared_ptr<Expression>(new Expression(name, type));
        Program& program = Program::getInstance();
        Scope& curr_scope = program.scopes.back();
        curr_scope.insertToSymbolTable(exp);
        cout << "----------------addToScope_p  end" <<  endl;
    }

    static void wrongFuncArgs_p(const string& id, const vector<shared_ptr<Expression>>& args, int line_no) {
        cout << "----------------wrongFuncArgs_p " <<  endl;
        vector<string> type_names;
        type_names.reserve(args.size());
        for (const shared_ptr<Expression>& exp : args) {
            type_names.push_back(exp->getTypeAsString());
        }
        output::errorPrototypeMismatch(line_no, id, type_names);
        cout << "----------------wrongFuncArgs_p   end" <<  endl;
    }

    static shared_ptr<Expression> funcCallHandler_p(const string& id, int line_no) {
        cout << "----------------funcCallHandler_p " <<  endl;
        Program& program = getInstance();
        shared_ptr<Func> func = program.global_scope.getById(id);
        if (func) {
            if (func->getArgsSize() != program.tmp_exp.size())
                wrongFuncArgs_p(id, func->getArgs(), line_no);
            for (int i = 0; i < func->getArgsSize(); i++) {
                if (!(func->getArgByIndex(i)->getType(), program.tmp_exp[i]->getType()) ||
                    ( func->getArgByIndex(i)->getType() == Ex3_type::BYTE && program.tmp_exp[i]->getType() == Ex3_type::INT))
                    wrongFuncArgs_p(id, func->getArgs(), line_no);
            }
            program.tmp_exp.clear();
            return func;
        }
        output::errorUndefFunc(line_no, id);
        return nullptr;
        cout << "----------------funcCallHandler_p  end" <<  endl;
    }

    static void addArgForFuncCall_p(const shared_ptr<Expression>& arg) {
        Program& program = getInstance();
        program.tmp_exp.insert(program.tmp_exp.begin(), arg);
    }

    static shared_ptr<Expression> selectType_p(Ex3_type::Type type1, Ex3_type::Type type2) {
        if (type1 == Ex3_type::BYTE && type2 == Ex3_type::BYTE)
            return shared_ptr<Expression>(new Expression("", Ex3_type::BYTE));
        else
            return shared_ptr<Expression>(new Expression("", Ex3_type::INT));
    }

    bool isInGlobalScope(const string id){
        cout << "----------------isInGlobalScope and end " <<  endl;
        return this->global_scope.isInScope(id);
    }

    void addToGlobalScope(shared_ptr<Func> func_ptr){
        cout << "----------------addToGlobalScope and end " <<  endl;
        this->global_scope.addFunc(func_ptr);
    }

    Scope& getLastScope(){
        cout << "----------------getLastScope and end " <<  endl;
        return this->scopes.back();
    }

    static void checkSameType_p(Ex3_type::Type type1, Ex3_type::Type type2, int line_no) {
        // std::cout << "line " << line_num << ": " << type1 << " " << type2 << std::endl;
        if (type1 == type2)
            return;
        if (type1 <= Ex3_type::NUM and type2 <= Ex3_type::NUM)
            return;
        output::errorMismatch(line_no);
    }

    static shared_ptr<Expression> checkIdAndReturn_p(const string name, int line_no){
        cout << "----------------checkIdAndReturn_p  " <<  endl;
        shared_ptr<Expression> exp = Program::getExpById_p(name);
        if (!exp || exp->isFunc()) {
            output::errorUndef(line_no, name);
        }
        cout << "----------------checkIdAndReturn_p  end" <<  endl;
        return exp;
    }

    static void checkIsInVoidScope_p(int line_no) {
        Program& program = Program::getInstance();
        for (auto scope = program.scopes.rbegin(); scope != program.scopes.rend(); scope++) {
            if (scope->getFunc() and scope->getFunc()->getType() == Ex3_type::VOID)
                return;
        }
        output::errorMismatch(line_no);
    }

    static void checkRetType_p(Ex3_type::Type type, int line_no) {
        Program& program = Program::getInstance();
        Scope& curr_scope = program.scopes.back();
        for (auto scope = program.scopes.rbegin(); scope != program.scopes.rend(); scope++) {
            shared_ptr<Func> func = scope->getFunc();
            if (func) {
                checkSameType_p(func->getType(), type, line_no);
                if (type == Ex3_type::VOID)
                    output::errorMismatch(line_no);
            }
            return;
        }
        output::errorMismatch(line_no);
    }

    static void checkIsWhileScope_p(bool is_break, int line_no) {
        Program& program = Program::getInstance();
        for (auto scope = program.scopes.rbegin(); scope != program.scopes.rend(); scope++) {
            if (scope->isInWhile())
                return;
        }
        if (is_break)
            output::errorUnexpectedBreak(line_no);
        else
            output::errorUnexpectedContinue(line_no);
    }

    static void checkValidByte_p(const shared_ptr<Expression>& exp, int line_no) {
        int e_val = stoi(exp->getName());
        if (e_val > 255)
            output::errorByteTooLarge(line_no, exp->getName());
    }

    static shared_ptr<Expression> checkBool_p(const shared_ptr<Expression>& exp, int line_no) {
        checkSameType_p(exp->getType(), Ex3_type::BOOL, line_no);
        return shared_ptr<Expression>(new Expression(Ex3_type::BOOL));
    }


    static void checkIfIdAlreadyExist_p(const string &id, int line_no) {
        cout << "----------------checkIfIdAlreadyExist_p  " <<  endl;
        if (Program::getExpById_p(id)) {
            output::errorDef(line_no, id);
        }
        cout << "----------------checkIfIdAlreadyExist_p  end" <<  endl;
    }

    static void checkType_p(const Ex3_type::Type& type1, const Ex3_type::Type& type2, int line_no) {
        cout << "----------------checkType_p  " <<  endl;
        if (type1 == type2)
            return;
        if (type1 == Ex3_type::INT && type2 == Ex3_type::BYTE)
            return;
        output::errorMismatch(line_no);
    }

    static void checkCast_p(Ex3_type::Type type1, Ex3_type::Type type2, int line_no) {
        if (type1 == type2)
            return;
        if (type1 <= Ex3_type::NUM && type2 <= Ex3_type::NUM)
            return;
        output::errorMismatch(line_no);
    }
};

//functions to functions
namespace ProgramFunctions{
    static void checkIfMainExists() {
       Program::checkIfMainExists_p();
    }

    static void addFuncToSymbolTable(Ex3_type::Type return_type, const string& id, int line_no, bool add_to_scope=true) {
        Program::addFuncToSymbolTable_p(return_type,id,line_no,add_to_scope);
    }

    static void openGlobalScope(const int line_no) {
        Program::openGlobalScope_p(line_no);
    }

    static void closeGlobalScope() {
        Program::closeGlobalScope_p();
    }

    static void openScope(bool is_in_while=false) {
        Program::openScope_p(is_in_while);
    }

    static void closeScope() {
        Program::closeScope_p();
    }

    static void addArgToFunc(string name, Ex3_type::Type type, int line_no){
        Program::addArgToFunc_p(name,type,line_no);
    }

    static shared_ptr<Expression> getExpById(const string& id) {
        return Program::getExpById_p(id);
    }

    static void addToScope(Ex3_type::Type type, const string& name) {
        Program::addToScope_p(type,name);
    }

    static void wrongFuncArgs(const string& id, const vector<shared_ptr<Expression>>& args, int line_no) {
        Program::wrongFuncArgs_p(id,args,line_no);
    }

    static shared_ptr<Expression> funcCallHandler(const string& id, int line_no) {
       return Program::funcCallHandler_p(id,line_no);
    }

    static void addArgForFuncCall(const shared_ptr<Expression>& arg) {
        Program::addArgForFuncCall_p(arg);
    }

    static shared_ptr<Expression> selectType(Ex3_type::Type type1, Ex3_type::Type type2) {
        return Program::selectType_p(type1,type2);
    }

    static void checkSameType(Ex3_type::Type type1, Ex3_type::Type type2, int line_no) {
        Program::checkSameType_p(type1,type2,line_no);
    }

    static shared_ptr<Expression> checkIdAndReturn(const string name, int line_no){
        return Program::checkIdAndReturn_p(name,line_no);
    }

    static void checkIsInVoidScope(int line_no) {
        Program::checkIsInVoidScope_p(line_no);
    }

    static void checkRetType(Ex3_type::Type type, int line_no) {
        Program::checkRetType_p(type,line_no);
    }

    static void checkIsWhileScope(bool is_break, int line_no) {
        Program::checkIsWhileScope_p(is_break,line_no);
    }

    static void checkValidByte(const shared_ptr<Expression>& exp, int line_no) {
        Program::checkValidByte_p(exp,line_no);
    }

    static shared_ptr<Expression> checkBool(const shared_ptr<Expression>& exp, int line_no) {
        return Program::checkBool_p(exp,line_no);
    }

    static void checkIfIdAlreadyExist(const string &id, int line_no) {
        Program::checkIfIdAlreadyExist_p(id,line_no);
    }

    static void checkType(const Ex3_type::Type& type1, const Ex3_type::Type& type2, int line_no) {
        Program::checkType_p(type1,type2,line_no);
    }


    static void checkCast(Ex3_type::Type type1, Ex3_type::Type type2, int line_no) {
        Program::checkCast_p(type1,type2,line_no);
    }
};







#endif //COMPILATION_PROGRAM_H
