//
// Created by efrat on 12/17/2022.
//

#ifndef COMPILATION_EXPRESSION_H
#define COMPILATION_EXPRESSION_H

#include <memory>
#include <vector>
using namespace std;

enum Type {
    INT, BYTE, NUM, VOID, BOOL, STRING, NONE
};

class Expression {
protected:
    string name;
    int offset;
    bool is_func;
    Type type;
public:
    Expression(): name(""),offset(0),is_func(false){}

    Expression(string name, Type type){
        name.copy(this->name, name.size());
        this->type = type;
    }

    explicit Expression(Type type) : type(type) {}

    explicit Expression(string name) {
        name.copy(this->name, name.size());
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

    Type getType(){
        return this->type;
    }

    string getTypeAsString() const {
        switch (this->type) {
            case INT:
                return "INT";
                break;
            case BYTE:
                return "BYTE";
                break;
            case NUM:
                return "NUM";
                break;
            case VOID:
                return "VOID";
                break;
            case BOOL:
                return "BOOL";
                break;
            case STRING:
                return "STRING";
                break;
            case NONE:
                return "NONE";
                break;
            default:
                return "ERROR";
                break;
        }
    }

    virtual void print() {
        output::printID(name, offset, this->getTypeAsString());
    }
};


class Func : public Expression {
    vector<shared_ptr<Expression>> args;
public:
    Func(Type ret_type, string id) : Expression(id, ret_type) {
        this->is_func = true;
        this->offset = 0;
    }
    shared_ptr<Expression> getArgById(const string id){
        for (auto const &arg : this->args) {
            if (arg->name == id) return arg;
        }
        return nullptr;
    }
    int getArgsSize(){
        return args.size();
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