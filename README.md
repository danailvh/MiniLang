# MiniLang
A compiler for my toy-language

The target of the compiler is an assembler for the JVM called Jasmin(http://jasmin.sourceforge.net/guide.html). <br>
We can feed the generated .j file to Jasmin and get the .class file which we can run on the JVM.

<b> Example program with their .j and .class files can be found in the examples folder.</b>

<b> How to run : </b>

1.From the command line run Minilang.exe (windows only)
first argument is the file containing the source code
second (and optional) argument is the name of the generated file "Program.j" by default.

./Minilang source.txt 

2.The other option is to open the .sln file with Visual Studio and write the code in the test.txt file
The output will be in solution/Program.j 

<b>The grammer for Minilang can be found in the MinilangGrammar.txt file. </b>
