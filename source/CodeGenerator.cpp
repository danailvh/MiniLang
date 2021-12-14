#include "CodeGenerator.h"
#include <iomanip>
#include <iostream>

void generateCode(std::shared_ptr<Expression> prog, std::ostream& OS,const std::string& name)
{
	CodeGenerator cg(OS,name);
	//! header
	OS << ".class  public "<< name <<"\n";
	OS << ".super java/lang/Object\n";
	OS << ".method public <init>()V\n";
	OS << "	aload_0\n";
	OS << "	invokenonvirtual java/lang/Object/<init>()V\n";
	OS << "	return\n";
	OS << ".end method\n";
	//! the actual program
	prog->accept(cg);
}

void CodeGenerator::boolify()
{
	int label1 = labelNum;
	int label2 = labelNum + 1;
	labelNum += 2;
	OS << "ifeq label" << label1 << "\n";
	OS << "iconst_1\n";
	OS << "goto label" << label2 << "\n";
	OS << "label" << label1 << ":\n";
	OS << "iconst_0\n";
	OS << "label" << label2 << ":\n";
}

void CodeGenerator::visit(TopStatements* e)
{
	auto prog = e->getProg();
	//visit function definitions
	OS << "\n";
	for (int i = 0; i < e->getFunctionCount() && i < prog.size(); i++)
	{
		prog[i]->accept(*this);
		OS << "\n";
	}

	//visit the rest of the program
	OS << ".method public static main([Ljava/lang/String;)V\n";
	OS << ".limit locals 32\n";
	OS << ".limit stack 32\n";
	for (int i = e->getFunctionCount(); i < prog.size(); i++)
	{
		prog[i]->accept(*this);
	}
	OS << "return\n";
	OS << ".end method\n";
}

char CodeGenerator::typeToLowcase(char t)
{
	if (t == BOOL || t == INT)
		return 'i';
	if (t == FLOAT)
		return 'f';
	if (t == STRING)
		assert("no conversion to and from string type allowed" && 0);
}

void CodeGenerator::visit(VariableDefinitionExpression* e)
{
	int var1 = CR.numOfVars;
	CR.numOfVars++;
	e->getRhs()->accept(*this);
	char lhsType = e->getType();
	char rhsType = e->getRhs()->getType();
	if (lhsType != rhsType)
	{
		OS << typeToLowcase(rhsType) << "2" << typeToLowcase(lhsType) << "\n";
	}
	if (CR.vars.count(e->getVarName()) != 0)
		assert("variable redefinition" && 0);

	CR.vars[e->getVarName()] = var1;
	CR.types[e->getVarName()] = lhsType;

	if (lhsType == BOOL)
		boolify();
	if (lhsType == INT || lhsType == BOOL)
		OS << "istore " << var1 << "\n";
	else if (lhsType == FLOAT)
		OS << "fstore " << var1 << "\n";
	else if (lhsType == STRING)
		OS << "astore " << var1 << "\n";
	else
		assert("typeError" && 0);
}

void CodeGenerator::visit(IdentifierExpression* e)
{
	if (CR.vars.count(e->getName()) != 0)
	{
		if (CR.types[e->getName()] == INT || CR.types[e->getName()] == BOOL)
			OS << "iload " << CR.vars[e->getName()] << "\n";
		else if (CR.types[e->getName()] == FLOAT)
			OS << "fload " << CR.vars[e->getName()] << "\n";
		else if (CR.types[e->getName()] == STRING)
			OS << "aload " << CR.vars[e->getName()] << "\n";
	}
	else
	{
		CR.vars[e->getName()] = CR.numOfVars;
		CR.types[e->getName()] = e->getType();
	}
}

void CodeGenerator::visit(LiteralInteger* e)
{
	OS << "ldc " << e->getValue() << "\n";
}

void CodeGenerator::visit(LiteralFloat* e)
{
	if (ceil(e->getValue()) == e->getValue() || floor(e->getValue()) == e->getValue())
		OS << "ldc " << e->getValue() << ".0\n";
	else
		OS << "ldc " << e->getValue() << "\n";
}

void CodeGenerator::visit(LiteralString* e)
{
	OS << "ldc \"" << e->getValue() << "\"\n";
}

void CodeGenerator::visit(LiteralBoolean* e)
{
	if (e->getValue())
		OS << "iconst_1" << "\n";
	else
		OS << "iconst_0" << "\n";
}

void CodeGenerator::comparisons(std::string operation, char type)
{
	std::string op;
	if (operation == "EqualEqual")
		op = "eq";
	else if (operation == "NotEqual")
		op = "ne";
	else if (operation == "MoreThan")
		op = "gt";
	else if (operation == "MoreEqual")
		op = "ge";
	else if (operation == "LessThan")
		op = "lt";
	else if (operation == "LessEqual")
		op = "le";

	if (type == FLOAT)
	{
		if (operation == "MoreThan" || operation == "MoreEqual" || operation == "EqualEqual" || operation == "NotEqual")
			OS << "fcmpg\n";
		else if (operation == "LessThan" || operation == "LessEqual")
			OS << "fcmpl\n";

		int label1 = labelNum;
		int label2 = labelNum + 1;
		labelNum += 2;
		OS << "if" << op << " label" << label1 << "\n";
		OS << "iconst_0\n";
		OS << "goto label" << label2 << "\n";
		OS << "label" << label1 << ":\n";
		OS << "iconst_1\n";
		OS << "label" << label2 << ":\n";
		return;
	}

	int label1 = labelNum;
	int label2 = labelNum + 1;
	labelNum += 2;
	OS << "if_icmp" << op << " label" << label1 << "\n";
	OS << "iconst_0 \n";
	OS << "goto label" << label2 << "\n";
	OS << "label" << label1 << ":\n";
	OS << "iconst_1\n";
	OS << "label" << label2 << ":\n";

}

void CodeGenerator::visit(BinaryExpression* e)
{
	//visit left
	//visit right
	//use OP on left and right which should be on the top of the stack
	auto op = e->getOp();
	e->getLhs()->accept(*this);

	if (op == "and")
	{
		int label1 = labelNum;
		int label2 = labelNum + 1;
		labelNum += 2;
		OS << "ifeq label" << label1 << "\n";
		e->getRhs()->accept(*this);
		OS << "ifeq label" << label1 << "\n";
		OS << "iconst_1\n";
		OS << "goto label" << label2 << "\n";
		OS << "label" << label1 << ":\n";
		OS << "iconst_0\n";
		OS << "label" << label2 << ":\n";
		return;
	}
	else if (op == "or")
	{
		int label1 = labelNum;
		int label2 = labelNum + 1;
		labelNum += 2;
		OS << "ifne label" << label1 << "\n";
		e->getRhs()->accept(*this);
		OS << "ifne label" << label1 << "\n";
		OS << "iconst_0\n";
		OS << "goto label" << label2 << "\n";
		OS << "label" << label1 << ":\n";
		OS << "iconst_1\n";
		OS << "label" << label2 << ":\n";
		return;
	}

	e->getRhs()->accept(*this);

	char letter;
	if (e->getType() == INT || e->getType() == BOOL)
		letter = 'i';
	else if (e->getType() == FLOAT)
		letter = 'f';
	else if (e->getType() == STRING)
		letter = 'a';
	else
		assert("type error in BinExpr" && 0);

	if (op == "Plus")
		OS << letter << "add\n";
	else if (op == "Minus")
		OS << letter << "sub\n";
	else if (op == "Times")
		OS << letter << "mul\n";
	else if (op == "Divide")
		OS << letter << "div\n";
	else if (op == "Mod")
		OS << letter << "rem\n";
	else if (op == "EqualEqual" || op == "LessThan" || op == "LessEqual" || op == "MoreEqual" || op == "MoreThan" || op == "NotEqual")
		comparisons(op, e->getLhs()->getType());

	else
		assert("error in BinExpr" && 0);


}

void CodeGenerator::visit(UnaryExpression* e)
{
	std::string letter;
	if (e->getOp() == "print" || e->getOp() == "Minus")
	{
		if (e->getType() == INT || e->getType() == BOOL)
			letter = "I";
		else if (e->getType() == FLOAT)
			letter = "F";
		else if (e->getType() == STRING)
			letter = "Ljava/lang/String;";
		else
			assert("type error in print" && 0);
	}

	if (e->getOp() == "print")
	{
		OS << "getstatic java/lang/System/out Ljava/io/PrintStream;\n";
		e->getLhs()->accept(*this);
		OS << "invokevirtual java/io/PrintStream/println(" << letter << ")V\n";
	}
	else if (e->getOp() == "not")
	{
		e->getLhs()->accept(*this);
		int label1 = labelNum;
		int label2 = labelNum + 1;
		labelNum += 2;
		OS << "ifeq label" << label1 << "\n";
		OS << "iconst_0\n";
		OS << "goto label" << label2 << "\n";
		OS << "label" << label1 << ":\n";
		OS << "iconst_1\n";
		OS << "label" << label2 << ":\n";
	}
	else if (e->getOp() == "Minus")
	{
		e->getLhs()->accept(*this);
		if (letter == "I")
			OS << "ineg\n";
		else if (letter == "F")
			OS << "fneg\n";
	}
	else if (e->getOp() == "return")
	{
		auto returnType = e->getType();
		std::string l = "";
		if (e->getLhs())
			e->getLhs()->accept(*this);
		if (returnType == INT || returnType == BOOL)
			l = "i";
		else if (returnType == FLOAT)
			l = "f";
		else if (returnType == STRING)
			l = "a";

		OS << l << "return\n";
	}
	else if (e->getOp() == "continue")
	{
		if (breakContinue.empty())
			assert("continue outside of loop" && 0);
		OS << "goto " << breakContinue.top().second << "\n";
	}
	else if (e->getOp() == "break")
	{
		if (breakContinue.empty())
			assert("break outside of loop" && 0);
		OS << "goto " << breakContinue.top().first << "\n";
	}
	else if (e->getOp() == "readi")
	{
		int label1 = labelNum;
		int label2 = labelNum + 1;
		int var1 = CR.numOfVars;
		int var2 = CR.numOfVars + 1;
		CR.numOfVars += 2;
		labelNum += 2;
		//var1 starts at 0 and digits are added 1 by 1 
		OS << "iconst_0\n";
		OS << "istore " << var1 << "\n";
		OS << "label" << label1 << ":\n";
		OS << "getstatic java/lang/System/in Ljava/io/InputStream;\n";
		OS << "invokevirtual java/io/InputStream/read()I\n";
		OS << "istore " << var2 << "\n";
		OS << "iload " << var2 << "\n";
		// 10 and 13 are newline and carriage return checks 
		// when we see either it means input is over and we return the final number
		OS << "ldc 10\n"; 
		OS << "isub\n";
		OS << "ifeq label" << label2 << "\n";
		OS << "iload " << var2 << "\n";
		OS << "ldc 13\n";
		OS << "isub\n";
		OS << "ifeq label" << label2 << "\n";
		OS << "iload " << var2 << "\n";
		// java/io/InputStream/read() reads a byte 
		// substract 48 to get the digit
		OS << "ldc 48\n";
		OS << "isub\n";
		// multiply the result so far by 10 and add the new digit
		OS << "iload " << var1 << "\n";
		OS << "ldc 10\n";
		OS << "imul\n";
		OS << "iadd\n";
		OS << "istore " << var1 << "\n";
		OS << "goto label" << label1 << "\n";

		OS << "label" << label2 << ":\n";
		// after we are done with the number read 1 more byte which would be a nl
		// so that we can read more than once per program
		OS << "getstatic java/lang/System/in Ljava/io/InputStream;\n";
		OS << "invokevirtual java/io/InputStream/read()I\n";
		OS << "iload " << var1 << "\n";
	}
	else
		assert("nonexistant unary expression" && 0);
}

void CodeGenerator::visit(AssignmentExpression* e)
{
	char letter;
	char rhsType = e->getType();
	char lhsType = CR.types[e->getVarName()];

	if (lhsType == INT || lhsType == BOOL)
		letter = 'i';
	else if (lhsType == FLOAT)
		letter = 'f';
	else if (lhsType == STRING)
		letter = 'a';
	

	if (e->getName() == "Equal")
	{
		e->getRhs()->accept(*this);
		if (rhsType == BOOL || lhsType == BOOL)
			boolify();
		if (rhsType != lhsType)
			OS << typeToLowcase(rhsType) << "2" << typeToLowcase(lhsType) << "\n";
		OS << letter << "store " << CR.vars[e->getVarName()] << "\n";
		return;
	}

	OS << letter << "load " << CR.vars[e->getVarName()] << "\n";
	e->getRhs()->accept(*this);
	if (rhsType != lhsType)
		OS << typeToLowcase(rhsType) << "2" << typeToLowcase(lhsType) << "\n";

	if (e->getName() == "PlusEqual")
		OS << letter << "add" << "\n";
	else if (e->getName() == "MinusEqual")
		OS << letter << "sub" << "\n";
	else if (e->getName() == "TimesEqual")
		OS << letter << "mul" << "\n";
	else if (e->getName() == "DivideEqual")
		OS << letter << "div" << "\n";
	else
		assert("error in assignement expression" && 0);
	if (e->getType() == BOOL)
		boolify();
	OS << letter << "store " << CR.vars[e->getVarName()] << "\n";
}

void CodeGenerator::visit(BlockStatement* e)
{
	auto block = e->getBlock();
	for (int i = 0; i < block.size(); i++)
	{
		block[i]->accept(*this);
	}
}


void CodeGenerator::visit(IfStatement* e)
{
	if (e->getIfType() == "else")
	{
		e->getBody()->accept(*this);
		return;
	}
	e->getCond()->accept(*this);
	int label1 = labelNum;
	int label2 = labelNum + 1;
	labelNum += 2;
	OS << "ifeq label" << label1 << "\n";
	e->getBody()->accept(*this);
	OS << "goto label" << label2 << "\n";
	OS << "label" << label1 << ":\n";
	if (e->getOrelse() != nullptr)
		e->getOrelse()->accept(*this);
	OS << "label" << label2 << ":\n";

}

void CodeGenerator::visit(WhileStatement* e)
{
	int label1 = labelNum;
	int label2 = labelNum + 1;
	labelNum += 2;
	breakContinue.push({ "label" + std::to_string(label2),"label" + std::to_string(label1) });
	OS << "label" << label1 << ":\n";
	e->getCond()->accept(*this);
	OS << "ifeq label" << label2 << "\n";
	e->getBody()->accept(*this);
	OS << "goto label" << label1 << "\n";
	OS << "label" << label2 << ":\n";
	breakContinue.pop();
}

std::string CodeGenerator::typeToLetter(char type)
{
	std::string letter;

	if (type == INT || type == BOOL)
		letter = "I";
	else if (type == FLOAT)
		letter = "F";
	else if (type == STRING)
		letter = "Ljava/lang/String;";
	else if (type == 0)
		letter = "V";
	else
		assert("unsupported type as a function argument" && 0);

	return letter;
}

void CodeGenerator::visit(FunctionDeclaration* e)
{
	auto begin = frames.top();
	auto args = e->getArgs();

	std::string arTypes = "";
	std::string returnType = typeToLetter(e->getType());
	for (int i = 0; i < args.size(); i++)
	{
		arTypes += typeToLetter(args[i]->getType());
		args[i]->accept(*this);
		CR.numOfVars++;
	}

	if (e->getType() == STRING)
		returnType = "Ljava/lang/String;";

	OS << ".method public static " << e->getName() << "(" << arTypes << ")" << returnType << "\n";
	OS << ".limit locals 32\n";
	OS << ".limit stack 32\n";
	e->getBody()->accept(*this);
	OS << ".end method\n";
	frames.top() = begin;
}

void CodeGenerator::visit(FunctionCall* e)
{
	auto args = e->getArgs();
	std::string arTypes;
	std::string returnType = typeToLetter(e->getType());
	for (int i = 0; i < args.size(); i++)
		arTypes += typeToLetter(args[i]->getType());

	for (int i = 0; i < args.size(); i++)
		args[i]->accept(*this);
	Frame F;
	frames.push(F);
	OS << "invokestatic "<< fileName <<"/" << e->getName() << "(" << arTypes << ")" << returnType << "\n";
	frames.pop();
}