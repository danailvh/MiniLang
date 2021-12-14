.class  public fibonacci_iter
.super java/lang/Object
.method public <init>()V
	aload_0
	invokenonvirtual java/lang/Object/<init>()V
	return
.end method

.method public static fibonacci(I)I
.limit locals 32
.limit stack 32
ldc 1
istore 1
ldc 1
istore 2
ldc 3
istore 3
label0:
iload 3
iload 0
if_icmple label2
iconst_0 
goto label3
label2:
iconst_1
label3:
ifeq label1
iload 1
iload 2
iadd
istore 2
iload 2
iload 1
isub
istore 1
iload 3
ldc 1
iadd
istore 3
goto label0
label1:
iload 2
ireturn
.end method

.method public static main([Ljava/lang/String;)V
.limit locals 32
.limit stack 32
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "Enter integer"
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
iconst_0
istore 1
label4:
getstatic java/lang/System/in Ljava/io/InputStream;
invokevirtual java/io/InputStream/read()I
istore 2
iload 2
ldc 10
isub
ifeq label5
iload 2
ldc 13
isub
ifeq label5
iload 2
ldc 48
isub
iload 1
ldc 10
imul
iadd
istore 1
goto label4
label5:
getstatic java/lang/System/in Ljava/io/InputStream;
invokevirtual java/io/InputStream/read()I
iload 1
istore 0
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "Your fibonacci number is :"
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 0
invokestatic fibonacci_iter/fibonacci(I)I
invokevirtual java/io/PrintStream/println(I)V
return
.end method
