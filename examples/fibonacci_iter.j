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
istore 2
ldc 1
istore 3
ldc 3
istore 4
label0:
iload 4
iload 0
if_icmple label2
iconst_0 
goto label3
label2:
iconst_1
label3:
ifeq label1
iload 2
iload 3
iadd
istore 3
iload 3
iload 2
isub
istore 2
iload 4
ldc 1
iadd
istore 4
goto label0
label1:
iload 3
ireturn
.end method

.method public static main([Ljava/lang/String;)V
.limit locals 32
.limit stack 32
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc 20
invokestatic fibonacci_iter/fibonacci(I)I
invokevirtual java/io/PrintStream/println(I)V
return
.end method
