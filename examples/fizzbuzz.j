.class  public fizzbuzz
.super java/lang/Object
.method public <init>()V
	aload_0
	invokenonvirtual java/lang/Object/<init>()V
	return
.end method

.method public static fizzbuzz(I)V
.limit locals 32
.limit stack 32
ldc 1
istore 2
label0:
iload 2
iload 0
if_icmple label2
iconst_0 
goto label3
label2:
iconst_1
label3:
ifeq label1
iload 2
ldc 3
irem
ldc 0
if_icmpeq label4
iconst_0 
goto label5
label4:
iconst_1
label5:
ifeq label6
iload 2
ldc 5
irem
ldc 0
if_icmpeq label8
iconst_0 
goto label9
label8:
iconst_1
label9:
ifeq label6
iconst_1
goto label7
label6:
iconst_0
label7:
ifeq label10
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "FizzBuzz"
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
goto label11
label10:
iload 2
ldc 3
irem
ldc 0
if_icmpeq label12
iconst_0 
goto label13
label12:
iconst_1
label13:
ifeq label14
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "Fizz"
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
goto label15
label14:
iload 2
ldc 5
irem
ldc 0
if_icmpeq label16
iconst_0 
goto label17
label16:
iconst_1
label17:
ifeq label18
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "Buzz"
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
goto label19
label18:
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 2
invokevirtual java/io/PrintStream/println(I)V
label19:
label15:
label11:
iload 2
ldc 1
iadd
istore 2
goto label0
label1:
return
.end method

.method public static main([Ljava/lang/String;)V
.limit locals 32
.limit stack 32
ldc 15
invokestatic fizzbuzz/fizzbuzz(I)V
return
.end method
