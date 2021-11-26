#include "CodeGenerator.h" 
#include <fstream>
#include <sstream>
#include <iostream>

std::shared_ptr<Expression> getAST(std::string code)
{
	Lexer L;
	auto tokens = L.getTokens(code);
	auto ident = L.getIdentation();
	Parser P;
	return P.parse(tokens, ident);
}

void printTokens(const std::string& code)
{
	Lexer L;
	auto tokens = L.getTokens(code);
	L.print();
}

void generateJasmin(std::string code, std::ostream& file,const std::string& fname)
{
	std::string name = fname.substr(0, fname.find("."));
	auto ast = getAST(code);
	generateCode(ast, file, name);
}

int main(int argc, char* argv[])
{
	/*
		When running from VS
		write your code in test.txt and run
		results will be in solution/Program.j
	*/

	std::string code = "";
	std::string outputFileName = "Program.j";
	
	//first argument is the source code file 
	//second (optional) argument is the outputfile
	std::ifstream t;
	if (argc > 1)
		t.open(argv[1]);
	else
		t.open("test.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
	code = buffer.str();
	if (argc > 2)
		outputFileName = argv[2];

	std::ofstream file(outputFileName);

	//printTokens(code);
	generateJasmin(code, file, outputFileName);
}