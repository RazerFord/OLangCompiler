; ModuleID = 'Main'
source_filename = "Main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"

%__VTableInteger = type { float (%Real*)*, %Real* (%Real*)*, %Integer* (%Real*)*, %Real* (%Real*, %Real*)*, %Real* (%Real*, %Integer*)*, %Real* (%Real*, %Real*)*, %Real* (%Real*, %Integer*)*, %Real* (%Real*, %Real*)*, %Real* (%Real*, %Integer*)*, %Real* (%Real*, %Real*)*, %Real* (%Real*, %Integer*)*, %Boolean* (%Real*, %Real*)*, %Boolean* (%Real*, %Integer*)*, %Boolean* (%Real*, %Real*)*, %Boolean* (%Real*, %Integer*)*, %Boolean* (%Real*, %Real*)*, %Boolean* (%Real*, %Integer*)*, %Boolean* (%Real*, %Real*)*, %Boolean* (%Real*, %Integer*)*, %Boolean* (%Real*, %Real*)*, %Boolean* (%Real*, %Integer*)*, %Real* (%Integer*)* }
%Real = type { %__VTableReal*, float }
%__VTableReal = type { i32 (%Integer*)*, %Integer* (%Integer*)*, %Real* (%Integer*)*, %Integer* (%Integer*, %Integer*)*, %Real* (%Integer*, %Real*)*, %Integer* (%Integer*, %Integer*)*, %Real* (%Integer*, %Real*)*, %Integer* (%Integer*, %Integer*)*, %Real* (%Integer*, %Real*)*, %Integer* (%Integer*, %Integer*)*, %Real* (%Integer*, %Real*)*, %Boolean* (%Integer*, %Integer*)*, %Boolean* (%Integer*, %Real*)*, %Boolean* (%Integer*, %Integer*)*, %Boolean* (%Integer*, %Real*)*, %Boolean* (%Integer*, %Integer*)*, %Boolean* (%Integer*, %Real*)*, %Boolean* (%Integer*, %Integer*)*, %Boolean* (%Integer*, %Real*)*, %Boolean* (%Integer*, %Integer*)*, %Boolean* (%Integer*, %Real*)*, %Integer* (%Real*)* }
%Integer = type { %__VTableInteger*, i32 }
%Boolean = type { %__VTableBoolean*, i1 }
%__VTableBoolean = type { i1 (%Boolean*)*, %Integer* (%Boolean*)*, %Boolean* (%Boolean*, %Boolean*)*, %Boolean* (%Boolean*, %Boolean*)*, %Boolean* (%Boolean*, %Boolean*)*, %Boolean* (%Boolean*)* }
%__VTableIntegerArray = type { %Integer* (%IntegerArray*)*, %Integer* (%IntegerArray*, %Integer*)*, void (%IntegerArray*, %Integer*, %Integer*)* }
%IntegerArray = type { %__VTableIntegerArray*, %Integer, %Integer }
%__VTableSortClassTest = type { void (%SortClassTest*)* }
%SortClassTest = type { %__VTableSortClassTest* }

@__VTableInteger = global %__VTableInteger { float (%Real*)* @get.15, %Real* (%Real*)* @UnaryMinus.16, %Integer* (%Real*)* @toInteger, %Real* (%Real*, %Real*)* @Plus.17, %Real* (%Real*, %Integer*)* @Plus.18, %Real* (%Real*, %Real*)* @Minus.19, %Real* (%Real*, %Integer*)* @Minus.20, %Real* (%Real*, %Real*)* @Mult.21, %Real* (%Real*, %Integer*)* @Mult.22, %Real* (%Real*, %Real*)* @Div.23, %Real* (%Real*, %Integer*)* @Div.24, %Boolean* (%Real*, %Real*)* @Less.25, %Boolean* (%Real*, %Integer*)* @Less.26, %Boolean* (%Real*, %Real*)* @LessEqual.27, %Boolean* (%Real*, %Integer*)* @LessEqual.28, %Boolean* (%Real*, %Real*)* @Greater.29, %Boolean* (%Real*, %Integer*)* @Greater.30, %Boolean* (%Real*, %Real*)* @GreaterEqual.31, %Boolean* (%Real*, %Integer*)* @GreaterEqual.32, %Boolean* (%Real*, %Real*)* @Equal.33, %Boolean* (%Real*, %Integer*)* @Equal.34, %Real* (%Integer*)* @toReal }
@__VTableReal = global %__VTableReal { i32 (%Integer*)* @get, %Integer* (%Integer*)* @UnaryMinus, %Real* (%Integer*)* @toReal, %Integer* (%Integer*, %Integer*)* @Plus, %Real* (%Integer*, %Real*)* @Plus.3, %Integer* (%Integer*, %Integer*)* @Minus, %Real* (%Integer*, %Real*)* @Minus.4, %Integer* (%Integer*, %Integer*)* @Mult, %Real* (%Integer*, %Real*)* @Mult.5, %Integer* (%Integer*, %Integer*)* @Div, %Real* (%Integer*, %Real*)* @Div.6, %Boolean* (%Integer*, %Integer*)* @Less, %Boolean* (%Integer*, %Real*)* @Less.7, %Boolean* (%Integer*, %Integer*)* @LessEqual, %Boolean* (%Integer*, %Real*)* @LessEqual.8, %Boolean* (%Integer*, %Integer*)* @Greater, %Boolean* (%Integer*, %Real*)* @Greater.9, %Boolean* (%Integer*, %Integer*)* @GreaterEqual, %Boolean* (%Integer*, %Real*)* @GreaterEqual.10, %Boolean* (%Integer*, %Integer*)* @Equal, %Boolean* (%Integer*, %Real*)* @Equal.11, %Integer* (%Real*)* @toInteger }
@__VTableBoolean = global %__VTableBoolean { i1 (%Boolean*)* @get.38, %Integer* (%Boolean*)* @toInteger.39, %Boolean* (%Boolean*, %Boolean*)* @Or, %Boolean* (%Boolean*, %Boolean*)* @And, %Boolean* (%Boolean*, %Boolean*)* @Xor, %Boolean* (%Boolean*)* @Not }
@__VTableIntegerArray = global %__VTableIntegerArray { %Integer* (%IntegerArray*)* @getLength, %Integer* (%IntegerArray*, %Integer*)* @get.41, void (%IntegerArray*, %Integer*, %Integer*)* @set }
@__VTableSortClassTest = global %__VTableSortClassTest { void (%SortClassTest*)* @main }
@0 = private unnamed_addr constant [10 x i8] c"size: %d\0A\00", align 1
@1 = private unnamed_addr constant [8 x i8] c"%d: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

declare i8* @malloc(i64)

define %Integer* @"<init>"(%Integer* %0) {
entry:
  ret %Integer* %0
}

define %Integer* @"<init>.1"(%Integer* %0, %Integer* %i) {
entry:
  %1 = getelementptr inbounds %Integer, %Integer* %0, i32 0, i32 1
  %2 = getelementptr inbounds %Integer, %Integer* %i, i32 0, i32 1
  %3 = load i32, i32* %2, align 4
  store i32 %3, i32* %1, align 4
  ret %Integer* %0
}

define %Integer* @"<init>.2"(%Integer* %0, i32 %i) {
entry:
  %i1 = alloca i32, align 4
  store i32 %i, i32* %i1, align 4
  %1 = getelementptr inbounds %Integer, %Integer* %0, i32 0, i32 1
  %2 = load i32, i32* %i1, align 4
  store i32 %2, i32* %1, align 4
  ret %Integer* %0
}

define i32 @get(%Integer* %0) {
entry:
  %1 = getelementptr inbounds %Integer, %Integer* %0, i32 0, i32 1
  %2 = load i32, i32* %1, align 4
  ret i32 %2
}

define %Integer* @UnaryMinus(%Integer* %0) {
entry:
  %1 = call %Integer* @UnaryMinusI(%Integer* %0)
  ret %Integer* %1
}

define %Real* @toReal(%Integer* %0) {
entry:
  %1 = call %Real* @toRealI(%Integer* %0)
  ret %Real* %1
}

define %Integer* @Plus(%Integer* %0, %Integer* %i) {
entry:
  %1 = call %Integer* @PlusII(%Integer* %0, %Integer* %i)
  ret %Integer* %1
}

define %Real* @Plus.3(%Integer* %0, %Real* %r) {
entry:
  %1 = call %Real* @PlusIR(%Integer* %0, %Real* %r)
  ret %Real* %1
}

define %Integer* @Minus(%Integer* %0, %Integer* %i) {
entry:
  %1 = call %Integer* @MinusII(%Integer* %0, %Integer* %i)
  ret %Integer* %1
}

define %Real* @Minus.4(%Integer* %0, %Real* %r) {
entry:
  %1 = call %Real* @MinusIR(%Integer* %0, %Real* %r)
  ret %Real* %1
}

define %Integer* @Mult(%Integer* %0, %Integer* %i) {
entry:
  %1 = call %Integer* @MultII(%Integer* %0, %Integer* %i)
  ret %Integer* %1
}

define %Real* @Mult.5(%Integer* %0, %Real* %r) {
entry:
  %1 = call %Real* @MultIR(%Integer* %0, %Real* %r)
  ret %Real* %1
}

define %Integer* @Div(%Integer* %0, %Integer* %i) {
entry:
  %1 = call %Integer* @DivII(%Integer* %0, %Integer* %i)
  ret %Integer* %1
}

define %Real* @Div.6(%Integer* %0, %Real* %r) {
entry:
  %1 = call %Real* @DivIR(%Integer* %0, %Real* %r)
  ret %Real* %1
}

define %Boolean* @Less(%Integer* %0, %Integer* %i) {
entry:
  %1 = call %Boolean* @LessII(%Integer* %0, %Integer* %i)
  ret %Boolean* %1
}

define %Boolean* @Less.7(%Integer* %0, %Real* %r) {
entry:
  %1 = call %Boolean* @LessIR(%Integer* %0, %Real* %r)
  ret %Boolean* %1
}

define %Boolean* @LessEqual(%Integer* %0, %Integer* %i) {
entry:
  %1 = call %Boolean* @LessEqII(%Integer* %0, %Integer* %i)
  ret %Boolean* %1
}

define %Boolean* @LessEqual.8(%Integer* %0, %Real* %r) {
entry:
  %1 = call %Boolean* @LessEqIR(%Integer* %0, %Real* %r)
  ret %Boolean* %1
}

define %Boolean* @Greater(%Integer* %0, %Integer* %i) {
entry:
  %1 = call %Boolean* @GreaterII(%Integer* %0, %Integer* %i)
  ret %Boolean* %1
}

define %Boolean* @Greater.9(%Integer* %0, %Real* %r) {
entry:
  %1 = call %Boolean* @GreaterIR(%Integer* %0, %Real* %r)
  ret %Boolean* %1
}

define %Boolean* @GreaterEqual(%Integer* %0, %Integer* %i) {
entry:
  %1 = call %Boolean* @GreaterEqII(%Integer* %0, %Integer* %i)
  ret %Boolean* %1
}

define %Boolean* @GreaterEqual.10(%Integer* %0, %Real* %r) {
entry:
  %1 = call %Boolean* @GreaterEqIR(%Integer* %0, %Real* %r)
  ret %Boolean* %1
}

define %Boolean* @Equal(%Integer* %0, %Integer* %i) {
entry:
  %1 = call %Boolean* @EqII(%Integer* %0, %Integer* %i)
  ret %Boolean* %1
}

define %Boolean* @Equal.11(%Integer* %0, %Real* %r) {
entry:
  %1 = call %Boolean* @EqIR(%Integer* %0, %Real* %r)
  ret %Boolean* %1
}

define %Real* @"<init>.12"(%Real* %0) {
entry:
  ret %Real* %0
}

define %Real* @"<init>.13"(%Real* %0, float %r) {
entry:
  %r1 = alloca float, align 4
  store float %r, float* %r1, align 4
  %1 = getelementptr inbounds %Real, %Real* %0, i32 0, i32 1
  %2 = load float, float* %r1, align 4
  store float %2, float* %1, align 4
  ret %Real* %0
}

define %Real* @"<init>.14"(%Real* %0, %Real* %r) {
entry:
  %1 = getelementptr inbounds %Real, %Real* %0, i32 0, i32 1
  %2 = getelementptr inbounds %Real, %Real* %r, i32 0, i32 1
  %3 = load float, float* %2, align 4
  store float %3, float* %1, align 4
  ret %Real* %0
}

define float @get.15(%Real* %0) {
entry:
  %1 = getelementptr inbounds %Real, %Real* %0, i32 0, i32 1
  %2 = load float, float* %1, align 4
  ret float %2
}

define %Real* @UnaryMinus.16(%Real* %0) {
entry:
  %1 = call %Real* @UnaryMinusR(%Real* %0)
  ret %Real* %1
}

define %Integer* @toInteger(%Real* %0) {
entry:
  %1 = call %Integer* @toIntegerR(%Real* %0)
  ret %Integer* %1
}

define %Real* @Plus.17(%Real* %0, %Real* %r) {
entry:
  %1 = call %Real* @PlusRR(%Real* %0, %Real* %r)
  ret %Real* %1
}

define %Real* @Plus.18(%Real* %0, %Integer* %i) {
entry:
  %1 = call %Real* @PlusRI(%Real* %0, %Integer* %i)
  ret %Real* %1
}

define %Real* @Minus.19(%Real* %0, %Real* %r) {
entry:
  %1 = call %Real* @MinusRR(%Real* %0, %Real* %r)
  ret %Real* %1
}

define %Real* @Minus.20(%Real* %0, %Integer* %i) {
entry:
  %1 = call %Real* @MinusRI(%Real* %0, %Integer* %i)
  ret %Real* %1
}

define %Real* @Mult.21(%Real* %0, %Real* %r) {
entry:
  %1 = call %Real* @MultRR(%Real* %0, %Real* %r)
  ret %Real* %1
}

define %Real* @Mult.22(%Real* %0, %Integer* %i) {
entry:
  %1 = call %Real* @MultRI(%Real* %0, %Integer* %i)
  ret %Real* %1
}

define %Real* @Div.23(%Real* %0, %Real* %r) {
entry:
  %1 = call %Real* @DivRR(%Real* %0, %Real* %r)
  ret %Real* %1
}

define %Real* @Div.24(%Real* %0, %Integer* %i) {
entry:
  %1 = call %Real* @DivRI(%Real* %0, %Integer* %i)
  ret %Real* %1
}

define %Boolean* @Less.25(%Real* %0, %Real* %r) {
entry:
  %1 = call %Boolean* @LessRR(%Real* %0, %Real* %r)
  ret %Boolean* %1
}

define %Boolean* @Less.26(%Real* %0, %Integer* %i) {
entry:
  %1 = call %Boolean* @LessRI(%Real* %0, %Integer* %i)
  ret %Boolean* %1
}

define %Boolean* @LessEqual.27(%Real* %0, %Real* %r) {
entry:
  %1 = call %Boolean* @LessEqRR(%Real* %0, %Real* %r)
  ret %Boolean* %1
}

define %Boolean* @LessEqual.28(%Real* %0, %Integer* %i) {
entry:
  %1 = call %Boolean* @LessEqRI(%Real* %0, %Integer* %i)
  ret %Boolean* %1
}

define %Boolean* @Greater.29(%Real* %0, %Real* %r) {
entry:
  %1 = call %Boolean* @GreaterRR(%Real* %0, %Real* %r)
  ret %Boolean* %1
}

define %Boolean* @Greater.30(%Real* %0, %Integer* %i) {
entry:
  %1 = call %Boolean* @GreaterRI(%Real* %0, %Integer* %i)
  ret %Boolean* %1
}

define %Boolean* @GreaterEqual.31(%Real* %0, %Real* %r) {
entry:
  %1 = call %Boolean* @GreaterEqRR(%Real* %0, %Real* %r)
  ret %Boolean* %1
}

define %Boolean* @GreaterEqual.32(%Real* %0, %Integer* %i) {
entry:
  %1 = call %Boolean* @GreaterEqRI(%Real* %0, %Integer* %i)
  ret %Boolean* %1
}

define %Boolean* @Equal.33(%Real* %0, %Real* %r) {
entry:
  %1 = call %Boolean* @EqRR(%Real* %0, %Real* %r)
  ret %Boolean* %1
}

define %Boolean* @Equal.34(%Real* %0, %Integer* %i) {
entry:
  %1 = call %Boolean* @EqRI(%Real* %0, %Integer* %i)
  ret %Boolean* %1
}

define %Boolean* @"<init>.35"(%Boolean* %0) {
entry:
  ret %Boolean* %0
}

define %Boolean* @"<init>.36"(%Boolean* %0, i1 %b) {
entry:
  %b1 = alloca i1, align 1
  store i1 %b, i1* %b1, align 1
  %1 = getelementptr inbounds %Boolean, %Boolean* %0, i32 0, i32 1
  %2 = load i1, i1* %b1, align 1
  store i1 %2, i1* %1, align 1
  ret %Boolean* %0
}

define %Boolean* @"<init>.37"(%Boolean* %0, %Boolean* %b) {
entry:
  %1 = getelementptr inbounds %Boolean, %Boolean* %0, i32 0, i32 1
  %2 = getelementptr inbounds %Boolean, %Boolean* %b, i32 0, i32 1
  %3 = load i1, i1* %2, align 1
  store i1 %3, i1* %1, align 1
  ret %Boolean* %0
}

define i1 @get.38(%Boolean* %0) {
entry:
  %1 = getelementptr inbounds %Boolean, %Boolean* %0, i32 0, i32 1
  %2 = load i1, i1* %1, align 1
  ret i1 %2
}

define %Integer* @toInteger.39(%Boolean* %0) {
entry:
  %1 = call %Integer* @toIntegerB(%Boolean* %0)
  ret %Integer* %1
}

define %Boolean* @Or(%Boolean* %0, %Boolean* %b) {
entry:
  %1 = call %Boolean* @OrB(%Boolean* %0, %Boolean* %b)
  ret %Boolean* %1
}

define %Boolean* @And(%Boolean* %0, %Boolean* %b) {
entry:
  %1 = call %Boolean* @AndB(%Boolean* %0, %Boolean* %b)
  ret %Boolean* %1
}

define %Boolean* @Xor(%Boolean* %0, %Boolean* %b) {
entry:
  %1 = call %Boolean* @XorB(%Boolean* %0, %Boolean* %b)
  ret %Boolean* %1
}

define %Boolean* @Not(%Boolean* %0) {
entry:
  %1 = call %Boolean* @NotB(%Boolean* %0)
  ret %Boolean* %1
}

define %IntegerArray* @"<init>.40"(%IntegerArray* %0, %Integer* %length) {
entry:
  %1 = call %IntegerArray* @initArray(%IntegerArray* %0, %Integer* %length)
  %2 = getelementptr inbounds %IntegerArray, %IntegerArray* %0, i32 0, i32 2
  %3 = load %Integer, %Integer* %length, align 8
  store %Integer %3, %Integer* %2, align 8
  ret %IntegerArray* %0
}

define %Integer* @getLength(%IntegerArray* %0) {
entry:
  %1 = getelementptr inbounds %IntegerArray, %IntegerArray* %0, i32 0, i32 2
  ret %Integer* %1
}

define %Integer* @get.41(%IntegerArray* %0, %Integer* %ind) {
entry:
  %1 = call %Integer* @getElement(%IntegerArray* %0, %Integer* %ind)
  ret %Integer* %1
}

define void @set(%IntegerArray* %0, %Integer* %ind, %Integer* %value) {
entry:
  call void @setElement(%IntegerArray* %0, %Integer* %ind, %Integer* %value)
  ret void
}

define void @main(%SortClassTest* %0) {
entry:
  %1 = call i8* @malloc(i64 ptrtoint (%IntegerArray* getelementptr (%IntegerArray, %IntegerArray* null, i64 1) to i64))
  %2 = bitcast i8* %1 to %IntegerArray*
  %3 = getelementptr inbounds %IntegerArray, %IntegerArray* %2, i32 0, i32 0
  store %__VTableIntegerArray* @__VTableIntegerArray, %__VTableIntegerArray** %3, align 8
  %4 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %5 = bitcast i8* %4 to %Integer*
  %6 = getelementptr inbounds %Integer, %Integer* %5, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %6, align 8
  %7 = getelementptr inbounds %Integer, %Integer* %5, i32 0, i32 1
  store i32 0, i32* %7, align 4
  %8 = call %Integer* @"<init>"(%Integer* %5)
  %9 = load %Integer, %Integer* %5, align 8
  %10 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %11 = bitcast i8* %10 to %Integer*
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %12, align 8
  %13 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  store i32 0, i32* %13, align 4
  %14 = call %Integer* @"<init>"(%Integer* %11)
  %15 = load %Integer, %Integer* %11, align 8
  %16 = getelementptr inbounds %IntegerArray, %IntegerArray* %2, i32 0, i32 1
  store %Integer %9, %Integer* %16, align 8
  %17 = getelementptr inbounds %IntegerArray, %IntegerArray* %2, i32 0, i32 2
  store %Integer %15, %Integer* %17, align 8
  %18 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %19 = bitcast i8* %18 to %Integer*
  %20 = getelementptr inbounds %Integer, %Integer* %19, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %20, align 8
  %21 = getelementptr inbounds %Integer, %Integer* %19, i32 0, i32 1
  store i32 0, i32* %21, align 4
  %22 = call %Integer* @"<init>.2"(%Integer* %19, i32 5)
  %23 = call %IntegerArray* @"<init>.40"(%IntegerArray* %2, %Integer* %19)
  %24 = getelementptr inbounds %IntegerArray, %IntegerArray* %2, i32 0, i32 2
  %25 = call i32 @get(%Integer* %24)
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %25)
  %27 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %28 = bitcast i8* %27 to %Integer*
  %29 = getelementptr inbounds %Integer, %Integer* %28, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %29, align 8
  %30 = getelementptr inbounds %Integer, %Integer* %28, i32 0, i32 1
  store i32 0, i32* %30, align 4
  %31 = call %Integer* @"<init>.2"(%Integer* %28, i32 0)
  %32 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %33 = bitcast i8* %32 to %Integer*
  %34 = getelementptr inbounds %Integer, %Integer* %33, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %34, align 8
  %35 = getelementptr inbounds %Integer, %Integer* %33, i32 0, i32 1
  store i32 0, i32* %35, align 4
  %36 = call %Integer* @"<init>.2"(%Integer* %33, i32 123)
  call void @set(%IntegerArray* %2, %Integer* %28, %Integer* %33)
  %37 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %38 = bitcast i8* %37 to %Integer*
  %39 = getelementptr inbounds %Integer, %Integer* %38, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %39, align 8
  %40 = getelementptr inbounds %Integer, %Integer* %38, i32 0, i32 1
  store i32 0, i32* %40, align 4
  %41 = call %Integer* @"<init>.2"(%Integer* %38, i32 1)
  %42 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %43 = bitcast i8* %42 to %Integer*
  %44 = getelementptr inbounds %Integer, %Integer* %43, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %44, align 8
  %45 = getelementptr inbounds %Integer, %Integer* %43, i32 0, i32 1
  store i32 0, i32* %45, align 4
  %46 = call %Integer* @"<init>.2"(%Integer* %43, i32 10)
  call void @set(%IntegerArray* %2, %Integer* %38, %Integer* %43)
  %47 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %48 = bitcast i8* %47 to %Integer*
  %49 = getelementptr inbounds %Integer, %Integer* %48, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %49, align 8
  %50 = getelementptr inbounds %Integer, %Integer* %48, i32 0, i32 1
  store i32 0, i32* %50, align 4
  %51 = call %Integer* @"<init>.2"(%Integer* %48, i32 2)
  %52 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %53 = bitcast i8* %52 to %Integer*
  %54 = getelementptr inbounds %Integer, %Integer* %53, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %54, align 8
  %55 = getelementptr inbounds %Integer, %Integer* %53, i32 0, i32 1
  store i32 0, i32* %55, align 4
  %56 = call %Integer* @"<init>.2"(%Integer* %53, i32 22)
  call void @set(%IntegerArray* %2, %Integer* %48, %Integer* %53)
  %57 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %58 = bitcast i8* %57 to %Integer*
  %59 = getelementptr inbounds %Integer, %Integer* %58, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %59, align 8
  %60 = getelementptr inbounds %Integer, %Integer* %58, i32 0, i32 1
  store i32 0, i32* %60, align 4
  %61 = call %Integer* @"<init>.2"(%Integer* %58, i32 3)
  %62 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %63 = bitcast i8* %62 to %Integer*
  %64 = getelementptr inbounds %Integer, %Integer* %63, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %64, align 8
  %65 = getelementptr inbounds %Integer, %Integer* %63, i32 0, i32 1
  store i32 0, i32* %65, align 4
  %66 = call %Integer* @"<init>.2"(%Integer* %63, i32 1)
  call void @set(%IntegerArray* %2, %Integer* %58, %Integer* %63)
  %67 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %68 = bitcast i8* %67 to %Integer*
  %69 = getelementptr inbounds %Integer, %Integer* %68, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %69, align 8
  %70 = getelementptr inbounds %Integer, %Integer* %68, i32 0, i32 1
  store i32 0, i32* %70, align 4
  %71 = call %Integer* @"<init>.2"(%Integer* %68, i32 4)
  %72 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %73 = bitcast i8* %72 to %Integer*
  %74 = getelementptr inbounds %Integer, %Integer* %73, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %74, align 8
  %75 = getelementptr inbounds %Integer, %Integer* %73, i32 0, i32 1
  store i32 0, i32* %75, align 4
  %76 = call %Integer* @"<init>.2"(%Integer* %73, i32 23)
  call void @set(%IntegerArray* %2, %Integer* %68, %Integer* %73)
  %77 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %78 = bitcast i8* %77 to %Integer*
  %79 = getelementptr inbounds %Integer, %Integer* %78, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %79, align 8
  %80 = getelementptr inbounds %Integer, %Integer* %78, i32 0, i32 1
  store i32 0, i32* %80, align 4
  %81 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %82 = bitcast i8* %81 to %Integer*
  %83 = getelementptr inbounds %Integer, %Integer* %82, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %83, align 8
  %84 = getelementptr inbounds %Integer, %Integer* %82, i32 0, i32 1
  store i32 0, i32* %84, align 4
  %85 = call %Integer* @"<init>.2"(%Integer* %82, i32 0)
  %86 = call %Integer* @"<init>.1"(%Integer* %78, %Integer* %82)
  %87 = call %Integer* @getLength(%IntegerArray* %2)
  %88 = call %Boolean* @Less(%Integer* %78, %Integer* %87)
  %89 = getelementptr inbounds %Boolean, %Boolean* %88, i32 0, i32 1
  %90 = load i1, i1* %89, align 1
  br i1 %90, label %loop, label %loopend

loop:                                             ; preds = %loop, %entry
  %91 = call i32 @get(%Integer* %78)
  %92 = call %Integer* @get.41(%IntegerArray* %2, %Integer* %78)
  %93 = call i32 @get(%Integer* %92)
  %94 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @1, i32 0, i32 0), i32 %91, i32 %93)
  %95 = call i8* @malloc(i64 ptrtoint (%Integer* getelementptr (%Integer, %Integer* null, i64 1) to i64))
  %96 = bitcast i8* %95 to %Integer*
  %97 = getelementptr inbounds %Integer, %Integer* %96, i32 0, i32 0
  store %__VTableInteger* @__VTableInteger, %__VTableInteger** %97, align 8
  %98 = getelementptr inbounds %Integer, %Integer* %96, i32 0, i32 1
  store i32 0, i32* %98, align 4
  %99 = call %Integer* @"<init>.2"(%Integer* %96, i32 1)
  %100 = call %Integer* @Plus(%Integer* %78, %Integer* %96)
  %101 = load %Integer, %Integer* %100, align 8
  store %Integer %101, %Integer* %78, align 8
  %102 = call %Integer* @getLength(%IntegerArray* %2)
  %103 = call %Boolean* @Less(%Integer* %78, %Integer* %102)
  %104 = getelementptr inbounds %Boolean, %Boolean* %103, i32 0, i32 1
  %105 = load i1, i1* %104, align 1
  br i1 %105, label %loop, label %loopend

loopend:                                          ; preds = %loop, %entry
  ret void
}

declare %Integer* @UnaryMinusI(%Integer*)

declare %Real* @toRealI(%Integer*)

declare %Integer* @PlusII(%Integer*, %Integer*)

declare %Real* @PlusIR(%Integer*, %Real*)

declare %Integer* @MinusII(%Integer*, %Integer*)

declare %Real* @MinusIR(%Integer*, %Real*)

declare %Integer* @MultII(%Integer*, %Integer*)

declare %Real* @MultIR(%Integer*, %Real*)

declare %Integer* @DivII(%Integer*, %Integer*)

declare %Real* @DivIR(%Integer*, %Real*)

declare %Boolean* @LessII(%Integer*, %Integer*)

declare %Boolean* @LessIR(%Integer*, %Real*)

declare %Boolean* @LessEqII(%Integer*, %Integer*)

declare %Boolean* @LessEqIR(%Integer*, %Real*)

declare %Boolean* @GreaterII(%Integer*, %Integer*)

declare %Boolean* @GreaterIR(%Integer*, %Real*)

declare %Boolean* @GreaterEqII(%Integer*, %Integer*)

declare %Boolean* @GreaterEqIR(%Integer*, %Real*)

declare %Boolean* @EqII(%Integer*, %Integer*)

declare %Boolean* @EqIR(%Integer*, %Real*)

declare %Real* @UnaryMinusR(%Real*)

declare %Integer* @toIntegerR(%Real*)

declare %Real* @PlusRR(%Real*, %Real*)

declare %Real* @PlusRI(%Real*, %Integer*)

declare %Real* @MinusRR(%Real*, %Real*)

declare %Real* @MinusRI(%Real*, %Integer*)

declare %Real* @MultRR(%Real*, %Real*)

declare %Real* @MultRI(%Real*, %Integer*)

declare %Real* @DivRR(%Real*, %Real*)

declare %Real* @DivRI(%Real*, %Integer*)

declare %Boolean* @LessRR(%Real*, %Real*)

declare %Boolean* @LessRI(%Real*, %Integer*)

declare %Boolean* @LessEqRR(%Real*, %Real*)

declare %Boolean* @LessEqRI(%Real*, %Integer*)

declare %Boolean* @GreaterRR(%Real*, %Real*)

declare %Boolean* @GreaterRI(%Real*, %Integer*)

declare %Boolean* @GreaterEqRR(%Real*, %Real*)

declare %Boolean* @GreaterEqRI(%Real*, %Integer*)

declare %Boolean* @EqRR(%Real*, %Real*)

declare %Boolean* @EqRI(%Real*, %Integer*)

declare %Integer* @toIntegerB(%Boolean*)

declare %Boolean* @OrB(%Boolean*, %Boolean*)

declare %Boolean* @AndB(%Boolean*, %Boolean*)

declare %Boolean* @XorB(%Boolean*, %Boolean*)

declare %Boolean* @NotB(%Boolean*)

declare %IntegerArray* @initArray(%IntegerArray*, %Integer*)

declare %Integer* @getElement(%IntegerArray*, %Integer*)

declare void @setElement(%IntegerArray*, %Integer*, %Integer*)
