.class  public other
.super java/lang/Object
.method public <init>()V
	aload_0
	invokenonvirtual java/lang/Object/<init>()V
	return
.end method

.method public static func1(F)F
.limit locals 32
.limit stack 32
ldc 3.5
fstore 2
fload 0
fload 2
fmul
ldc 1.0
ldc 123.2
fmul
fadd
freturn
.end method

.method public static func2(F)F
.limit locals 32
.limit stack 32
ldc 3.5
fstore 2
fload 0
fload 2
ldc 1.0
fadd
ldc 123.2
fmul
fmul
freturn
.end method

.method public static main([Ljava/lang/String;)V
.limit locals 32
.limit stack 32
ldc 1.2
fstore 1
ldc 10000
i2f
fstore 2
ldc "good"
astore 3
ldc "bad"
astore 4
fload 1
invokestatic other/func1(F)F
fload 1
invokestatic other/func2(F)F
fcmpg
ifne label0
iconst_0
goto label1
label0:
iconst_1
label1:
ifeq label2
getstatic java/lang/System/out Ljava/io/PrintStream;
aload 3
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
goto label3
label2:
getstatic java/lang/System/out Ljava/io/PrintStream;
aload 4
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
label3:
fload 2
fload 1
fcmpg
ifgt label4
iconst_0
goto label5
label4:
iconst_1
label5:
ifeq label6
getstatic java/lang/System/out Ljava/io/PrintStream;
aload 3
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
goto label7
label6:
fload 2
fload 1
fcmpg
ifeq label8
iconst_0
goto label9
label8:
iconst_1
label9:
ifeq label10
getstatic java/lang/System/out Ljava/io/PrintStream;
aload 4
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
goto label11
label10:
getstatic java/lang/System/out Ljava/io/PrintStream;
aload 3
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
label11:
label7:
getstatic java/lang/System/out Ljava/io/PrintStream;
fload 1
invokestatic other/func1(F)F
invokevirtual java/io/PrintStream/println(F)V
getstatic java/lang/System/out Ljava/io/PrintStream;
fload 1
invokestatic other/func2(F)F
invokevirtual java/io/PrintStream/println(F)V
iconst_0
ifeq label12
iconst_1
ifeq label12
iconst_1
goto label13
label12:
iconst_0
label13:
ifeq label14
getstatic java/lang/System/out Ljava/io/PrintStream;
aload 4
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
goto label15
label14:
iconst_0
ifeq label16
iconst_0
ifeq label16
iconst_1
goto label17
label16:
iconst_0
label17:
ifeq label18
getstatic java/lang/System/out Ljava/io/PrintStream;
aload 4
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
goto label19
label18:
iconst_0
ifne label20
iconst_0
ifne label20
iconst_0
goto label21
label20:
iconst_1
label21:
ifeq label22
getstatic java/lang/System/out Ljava/io/PrintStream;
aload 4
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
goto label23
label22:
iconst_1
ifne label24
iconst_0
ifne label24
iconst_0
goto label25
label24:
iconst_1
label25:
ifeq label26
iconst_1
ifeq label26
iconst_1
goto label27
label26:
iconst_0
label27:
ifeq label28
getstatic java/lang/System/out Ljava/io/PrintStream;
aload 3
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
goto label29
label28:
label29:
label23:
label19:
label15:
ldc 100
ldc 3
ldc 4
imul
ldc 8
ldc 2
idiv
iadd
iadd
istore 5
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 5
invokevirtual java/io/PrintStream/println(I)V
ldc 1
istore 6
label30:
iload 6
ldc 256
if_icmple label32
iconst_0 
goto label33
label32:
iconst_1
label33:
ifeq label31
getstatic java/lang/System/out Ljava/io/PrintStream;
iload 6
invokevirtual java/io/PrintStream/println(I)V
iload 6
ldc 2
imul
istore 6
goto label30
label31:
getstatic java/lang/System/out Ljava/io/PrintStream;
ldc "the end"
invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
return
.end method
