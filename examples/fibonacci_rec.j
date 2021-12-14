.class  public fibonacci_rec
.super java/lang/Object
.method public <init>()V
	aload_0
	invokenonvirtual java/lang/Object/<init>()V
	return
.end method

.method public static fibonacci(I)I
.limit locals 32
.limit stack 32
iload 0
ldc 3
if_icmplt label0
iconst_0 
goto label1
label0:
iconst_1
label1:
ifeq label2
ldc 1
ireturn
goto label3
label2:
label3:
iload 0
ldc 1
isub
invokestatic fibonacci_rec/fibonacci(I)I
iload 0
ldc 2
isub
invokestatic fibonacci_rec/fibonacci(I)I
iadd
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
invokestatic fibonacci_rec/fibonacci(I)I
invokevirtual java/io/PrintStream/println(I)V
return
.end method
