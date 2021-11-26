#pragma once
#include <assert.h>
#include "Expressions.h"

class Visitor
{
public:
	virtual void visit(LiteralInteger* e) = 0;
	virtual void visit(LiteralFloat* e) = 0;
	virtual void visit(LiteralString* e) = 0;
	virtual void visit(LiteralBoolean* e) = 0;
	virtual void visit(IdentifierExpression* e) = 0; 
	virtual void visit(UnaryExpression* e) = 0;
	virtual void visit(BinaryExpression* e) = 0;
	virtual void visit(VariableDefinitionExpression* e) = 0;
	virtual void visit(AssignmentExpression* e) = 0;
	virtual void visit(TopStatements* e) = 0;
	virtual void visit(IfStatement* e) = 0;
	virtual void visit(WhileStatement* e) = 0;
	virtual void visit(BlockStatement* e) = 0;
	virtual void visit(FunctionDeclaration* e) = 0;
	virtual void visit(FunctionCall* e) = 0;
	virtual void visit(Expression* e) { assert("Parsing Error" && 0); }

	virtual ~Visitor() {}
};