#include "Expressions.h"
#include "Visitor.h"

void LiteralInteger::accept(Visitor& vis)
{
	vis.visit(this);
}
int LiteralInteger::getValue()
{
	return value;
}

void LiteralFloat::accept(Visitor& vis)
{
	vis.visit(this);
}
double LiteralFloat::getValue()
{
	return value;
}

void LiteralString::accept(Visitor& vis)
{
	vis.visit(this);
}
std::string LiteralString::getValue()
{
	return value;
}

void LiteralBoolean::accept(Visitor& vis)
{
	vis.visit(this);
}
bool LiteralBoolean::getValue()
{
	return value;
}

void IdentifierExpression::accept(Visitor& vis)
{
	vis.visit(this);
}
std::string IdentifierExpression::getName()
{
	return name;
}

void UnaryExpression::accept(Visitor& vis)
{
	vis.visit(this);
}
std::string UnaryExpression::getOp()
{
	return op;
}
std::shared_ptr<Expression> UnaryExpression::getLhs()
{
	return lhs;
}

void BinaryExpression::accept(Visitor& vis)
{
	vis.visit(this);
}
std::string BinaryExpression::getOp()
{
	return op;
}
std::shared_ptr<Expression> BinaryExpression::getLhs()
{
	return lhs;
}
std::shared_ptr<Expression> BinaryExpression::getRhs()
{
	return rhs;
}

void VariableDefinitionExpression::accept(Visitor& vis)
{
	vis.visit(this);
}
std::string VariableDefinitionExpression::getVarName()
{
	return lhs;
}
std::shared_ptr<Expression> VariableDefinitionExpression::getRhs()
{
	return rhs;
}

void AssignmentExpression::accept(Visitor& vis)
{
	vis.visit(this);
}
std::string AssignmentExpression::getName()
{
	return opName;
}
std::string AssignmentExpression::getVarName()
{
	return varName;
}
std::shared_ptr<Expression> AssignmentExpression::getRhs()
{
	return rhs;
}

void TopStatements::accept(Visitor& vis)
{
	vis.visit(this);
}
std::vector<std::shared_ptr<Expression>> TopStatements::getProg()
{
	return prog;
}
int TopStatements::getFunctionCount()
{
	return functionCount;
}
void IfStatement::accept(Visitor& vis)
{
	vis.visit(this);
}
std::string IfStatement::getIfType()
{
	return ifType;
}
std::shared_ptr<Expression> IfStatement::getCond()
{
	return cond;
}
std::shared_ptr<Expression> IfStatement::getBody()
{
	return body;
}
std::shared_ptr<Expression> IfStatement::getOrelse()
{
	return orelse;
}

void WhileStatement::accept(Visitor& vis)
{
	vis.visit(this);
}
std::shared_ptr<Expression> WhileStatement::getCond()
{
	return cond;
}
std::shared_ptr<Expression> WhileStatement::getBody()
{
	return body;
}
void BlockStatement::accept(Visitor& vis)
{
	vis.visit(this);
}
std::vector<std::shared_ptr<Expression>> BlockStatement::getBlock()
{
	return block;
}

void FunctionDeclaration::accept(Visitor& vis)
{
	vis.visit(this);
}
std::string FunctionDeclaration::getName()
{
	return name;
}
std::vector<std::shared_ptr<Expression>> FunctionDeclaration::getArgs()
{
	return args;
}
std::shared_ptr<Expression> FunctionDeclaration::getBody()
{
	return body;
}

void FunctionCall::accept(Visitor& vis)
{
	vis.visit(this);
}
std::string FunctionCall::getName()
{
	return name;
}
std::vector<std::shared_ptr<Expression>> FunctionCall::getArgs()
{
	return args;
}