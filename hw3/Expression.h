//
// Created by efrat on 12/17/2022.
//

#ifndef COMPILATION_EXPRESSION_H
#define COMPILATION_EXPRESSION_H

#include <memory>
#include <vector>
#include <string>
#include "hw3_output.hpp"
using namespace std;

namespace Ex3_type {
    enum Type {
        INT, BYTE, NUM, VOID, BOOL, STRING, NONE
    };
}

class Expression {
protected:
    string name;
    int offset;
    bool is_func;
    Ex3_type::Type type;
public:
    Expression(): name(""),offset(0),is_func(false){}

    Expression(string name, Ex3_type::Type type){
        this->name= name;
        this->type = type;
    }

    explicit Expression(Ex3_type::Type type) : type(type) {}

    explicit Expression(string name) {
        this->name= name;
    }

    bool isFunc(){
        return this->is_func;
    }

    void setOffset(int offset){
        this->offset = offset;
    }

    string getName(){
        return this->name;
    }

    Ex3_type::Type getType(){
        return this->type;
    }

    void setType(Ex3_type::Type type){
        this->type = type;
    }

    string getTypeAsString() const {
        switch (this->type) {
            case Ex3_type::INT:
                return "INT";
            case Ex3_type::BYTE:
                return "BYTE";
            case Ex3_type::NUM:
                return "NUM";
            case Ex3_type::VOID:
                return "VOID";
            case Ex3_type::BOOL:
                return "BOOL";
            case Ex3_type::STRING:
                return "STRING";
            case Ex3_type::NONE:
                return "NONE";
            default:
                return "ERROR";
        }
    }

    virtual void print() {
        output::printID(name, offset, this->getTypeAsString());
    }
};


class Func : public Expression {
private:
    vector<shared_ptr<Expression>> args;
public:
    Func(Ex3_type::Type ret_type, string id) : Expression(id, ret_type) {
        this->is_func = true;
        this->offset = 0;
    }

    shared_ptr<Expression> getArgById(const string id){
        for (auto const &arg : this->args) {
            if (arg->getName() == id) return arg;
        }
        return nullptr;
    }

    int getArgsSize(){
        return args.size();
    }

    vector<shared_ptr<Expression>> getArgs(){
        return this->args;
    }

    shared_ptr<Expression> getArgByIndex(int i){
        return args[i];
    }

    void addArg(shared_ptr<Expression> arg){
        this->args.push_back(arg);
    }

    void print() override {
        vector<string> args_types_names;
        for (const auto& exp : args) {
            args_types_names.push_back(exp->getTypeAsString());
        }
        output::printID(name, offset, output::makeFunctionType(this->getTypeAsString(), args_types_names));
    }

    void printExpressionsOnly(){
        for (auto & exp : args) {
            exp->print();
        }
    }
};

#endif //COMPILATION_EXPRESSION_H