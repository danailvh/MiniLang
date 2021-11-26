#pragma once
#include "Lexer.h"
#include <memory>

class Visitor;

#define VOID 0
#define BOOL 1
#define INT 2
#define STRING 3
#define FLOAT 4

class Expression
{
	char type;
public:
	Expression(char type = 0) : type(type) {};
	virtual void accept(Visitor& vis) = 0;
	virtual char getType() { return type; }
	//TODO: line + cols for error messages later on
	virtual ~Expression() {}
};

class LiteralInteger : public Expression
{
	int value;
public:
	LiteralInteger(int i) : value(i) {};
	void accept(Visitor& vis);
	int getValue();
	char getType() { return INT; }
};

class LiteralFloat : public Expression
{
	double value;
public:
	LiteralFloat(double i) : value(i) {};
	void accept(Visitor& vis);
	double getValue();
	char getType() { return FLOAT; }
};

class LiteralString : public Expression
{
	std::string value;
public:
	LiteralString(std::string i) : value(i) {};
	void accept(Visitor& vis);
	std::string getValue();
	char getType() { return STRING; }
};

class LiteralBoolean : public Expression
{
	bool value;
public:
	LiteralBoolean(bool i) : value(i) {};
	void accept(Visitor& vis);
	bool getValue();
	char getType() { return BOOL; }
};

class IdentifierExpression : public Expression
{
	std::string name;
public:
	IdentifierExpression(std::string i, char t) : name(i), Expression(t) {};
	void accept(Visitor& vis);
	std::string getName();
};

class VariableDefinitionExpression : public Expression
{
	std::string lhs;
	std::shared_ptr<Expression> rhs;
public:
	VariableDefinitionExpression(std::string lh, std::shared_ptr<Expression> rh, char t) : lhs(lh), rhs(rh), Expression(t) {};
	void accept(Visitor& vis);
	std::string getVarName();
	std::shared_ptr<Expression> getRhs();
};

class UnaryExpression : public Expression
{
	std::string op;
	std::shared_ptr<Expression> lhs;
public:
	UnaryExpression(std::string i, std::shared_ptr<Expression> lh, char t) : op(i), lhs(lh), Expression(t) {};
	void accept(Visitor& vis);
	std::string getOp();
	std::shared_ptr<Expression> getLhs();
};

class BinaryExpression : public Expression
{
	std::string op;
	std::shared_ptr<Expression> lhs;
	std::shared_ptr<Expression> rhs;
public:
	BinaryExpression(std::string i, std::shared_ptr<Expression> lh, std::shared_ptr<Expression> rh,char t) :
		op(i), lhs(lh), rhs(rh), Expression(t) {};
	void accept(Visitor& vis);
	std::string getOp();
	std::shared_ptr<Expression> getLhs();
	std::shared_ptr<Expression> getRhs();
};

class AssignmentExpression : public Expression
{
	std::string opName;
	std::string varName;
	std::shared_ptr<Expression> rhs;
public:
	AssignmentExpression(std::string i, std::string varName, std::shared_ptr<Expression> rh, char t) :
		opName(i), varName(varName), rhs(rh), Expression(t) {};
	void accept(Visitor& vis);
	std::string getName();
	std::string getVarName();
	std::shared_ptr<Expression> getRhs();
};

class TopStatements : public Expression
{
	std::vector<std::shared_ptr<Expression>> prog;
	int functionCount;
public:
	TopStatements(std::vector<std::shared_ptr<Expression>>& p, int fc) : prog(p), functionCount(fc) {};
	void accept(Visitor& vis);
	std::vector<std::shared_ptr<Expression>> getProg();
	int getFunctionCount();
};

class BlockStatement : public Expression
{
	std::vector<std::shared_ptr<Expression>> block;
public:
	BlockStatement(std::vector<std::shared_ptr<Expression>>& p) : block(p) {};
	void accept(Visitor& vis);
	std::vector<std::shared_ptr<Expression>> getBlock();
};

class WhileStatement : public Expression
{
	std::shared_ptr<Expression> cond;
	std::shared_ptr<Expression> body;
public:
	WhileStatement(std::shared_ptr<Expression> cond, std::shared_ptr<Expression> body) : cond(cond), body(body) {};
	void accept(Visitor& vis);
	std::shared_ptr<Expression> getCond();
	std::shared_ptr<Expression> getBody();
};

class IfStatement : public Expression
{
	std::string ifType; //if or elif or else
	std::shared_ptr<Expression> cond;
	std::shared_ptr<Expression> body;
	std::shared_ptr<Expression> orelse;
public:
	IfStatement(std::string t, std::shared_ptr<Expression> cond, std::shared_ptr<Expression> body, std::shared_ptr<Expression> el) :
		ifType(t), cond(cond), body(body), orelse(el) {};
	void accept(Visitor& vis);
	std::string getIfType();
	std::shared_ptr<Expression> getCond();
	std::shared_ptr<Expression> getBody();
	std::shared_ptr<Expression> getOrelse();
};

class FunctionDeclaration : public Expression
{
	std::string name;
	std::vector<std::shared_ptr<Expression>> args;
	std::shared_ptr<Expression> body;
public:
	FunctionDeclaration(std::string name, std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<Expression> body, char t) : 
		name(name), args(args), body(body), Expression(t) {};
	void accept(Visitor& vis);
	std::string getName();
	std::vector<std::shared_ptr<Expression>> getArgs();
	std::shared_ptr<Expression> getBody();
};

class FunctionCall : public Expression
{
	std::string name;
	std::vector<std::shared_ptr<Expression>> args;
public:
	FunctionCall(std::string name, std::vector<std::shared_ptr<Expression>> args, char t) : name(name), args(args), Expression(t) {};
	void accept(Visitor& vis);
	std::string getName();
	std::vector<std::shared_ptr<Expression>> getArgs();
};


