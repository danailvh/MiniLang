#pragma once
#include "Parser.h"
#include <ostream>

class CodeGenerator : public Visitor
{
public:
	CodeGenerator(std::ostream& OS,std::string name) : OS(OS), fileName(name), labelNum(0)
	{
		Frame f;
		frames.push(f);
	}

	void visit(LiteralInteger* e);
	void visit(LiteralFloat* e);
	void visit(LiteralString* e);
	void visit(LiteralBoolean* e);
	void visit(IdentifierExpression* e);
	void visit(UnaryExpression* e);
	void visit(BinaryExpression* e);
	void visit(VariableDefinitionExpression* e);
	void visit(AssignmentExpression* e);
	void visit(TopStatements* e);
	void visit(IfStatement* e);
	void visit(WhileStatement* e);
	void visit(BlockStatement* e);
	void visit(FunctionDeclaration* e);
	void visit(FunctionCall* e);
private:
	void boolify();
	void comparisons(std::string operation,char type);
	std::string typeToLetter(char type);
	char typeToLowcase(char type);
	struct Frame
	{
		Frame() : numOfVars(0) {};
		std::unordered_map<std::string, unsigned> vars;
		std::unordered_map<std::string, char> types;
		int numOfVars;
	};
	int labelNum;
	#define CR frames.top()
	std::stack<Frame> frames;
	std::stack<std::pair<std::string, std::string>> breakContinue;
	std::ostream& OS;
	std::string fileName;
};

void generateCode(std::shared_ptr<Expression> prog, std::ostream& OS,const std::string& name="Program");
