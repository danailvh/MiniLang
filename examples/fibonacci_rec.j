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
ldc 20
invokestatic fibonacci_rec/fibonacci(I)I
invokevirtual java/io/PrintStream/println(I)V
return
.end method
