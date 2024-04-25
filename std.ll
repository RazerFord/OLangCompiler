; ModuleID = 'std.c'
source_filename = "std.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Integer = type { i32*, i32 }
%Real = type { i32*, float }
%Boolean = type { i32*, i8 }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Integer* @UnaryMinusI(%Integer* noundef %0) #0 {
  %2 = alloca %Integer*, align 8
  store %Integer* %0, %Integer** %2, align 8
  %3 = load %Integer*, %Integer** %2, align 8
  %4 = getelementptr inbounds %Integer, %Integer* %3, i32 0, i32 1
  %5 = load i32, i32* %4, align 8
  %6 = sub nsw i32 0, %5
  %7 = load %Integer*, %Integer** %2, align 8
  %8 = getelementptr inbounds %Integer, %Integer* %7, i32 0, i32 1
  store i32 %6, i32* %8, align 8
  %9 = load %Integer*, %Integer** %2, align 8
  ret %Integer* %9
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @toReal(%Integer* noundef %0) #0 {
  %2 = alloca %Integer*, align 8
  %3 = alloca %Real*, align 8
  store %Integer* %0, %Integer** %2, align 8
  %4 = call noalias i8* @malloc(i64 noundef 16) #2
  %5 = bitcast i8* %4 to %Real*
  store %Real* %5, %Real** %3, align 8
  %6 = load %Integer*, %Integer** %2, align 8
  %7 = getelementptr inbounds %Integer, %Integer* %6, i32 0, i32 1
  %8 = load i32, i32* %7, align 8
  %9 = sitofp i32 %8 to float
  %10 = load %Real*, %Real** %3, align 8
  %11 = getelementptr inbounds %Real, %Real* %10, i32 0, i32 1
  store float %9, float* %11, align 8
  %12 = load %Real*, %Real** %3, align 8
  ret %Real* %12
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Integer* @PlusII(%Integer* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Integer*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Integer*
  store %Integer* %7, %Integer** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = add nsw i32 %10, %13
  %15 = load %Integer*, %Integer** %5, align 8
  %16 = getelementptr inbounds %Integer, %Integer* %15, i32 0, i32 1
  store i32 %14, i32* %16, align 8
  %17 = load %Integer*, %Integer** %5, align 8
  ret %Integer* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @PlusIR(%Integer* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Real*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %Real*, %Real** %4, align 8
  %13 = getelementptr inbounds %Real, %Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fadd float %11, %14
  %16 = load %Real*, %Real** %5, align 8
  %17 = getelementptr inbounds %Real, %Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %Real*, %Real** %5, align 8
  ret %Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Integer* @MinusII(%Integer* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Integer*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Integer*
  store %Integer* %7, %Integer** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sub nsw i32 %10, %13
  %15 = load %Integer*, %Integer** %5, align 8
  %16 = getelementptr inbounds %Integer, %Integer* %15, i32 0, i32 1
  store i32 %14, i32* %16, align 8
  %17 = load %Integer*, %Integer** %5, align 8
  ret %Integer* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @MinusIR(%Integer* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Real*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %Real*, %Real** %4, align 8
  %13 = getelementptr inbounds %Real, %Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fsub float %11, %14
  %16 = load %Real*, %Real** %5, align 8
  %17 = getelementptr inbounds %Real, %Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %Real*, %Real** %5, align 8
  ret %Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Integer* @MultII(%Integer* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Integer*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Integer*
  store %Integer* %7, %Integer** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = mul nsw i32 %10, %13
  %15 = load %Integer*, %Integer** %5, align 8
  %16 = getelementptr inbounds %Integer, %Integer* %15, i32 0, i32 1
  store i32 %14, i32* %16, align 8
  %17 = load %Integer*, %Integer** %5, align 8
  ret %Integer* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @MultIR(%Integer* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Real*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %Real*, %Real** %4, align 8
  %13 = getelementptr inbounds %Real, %Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fmul float %11, %14
  %16 = load %Real*, %Real** %5, align 8
  %17 = getelementptr inbounds %Real, %Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %Real*, %Real** %5, align 8
  ret %Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Integer* @DivII(%Integer* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Integer*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Integer*
  store %Integer* %7, %Integer** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sdiv i32 %10, %13
  %15 = load %Integer*, %Integer** %5, align 8
  %16 = getelementptr inbounds %Integer, %Integer* %15, i32 0, i32 1
  store i32 %14, i32* %16, align 8
  %17 = load %Integer*, %Integer** %5, align 8
  ret %Integer* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @DivIR(%Integer* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Real*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %Real*, %Real** %4, align 8
  %13 = getelementptr inbounds %Real, %Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fdiv float %11, %14
  %16 = load %Real*, %Real** %5, align 8
  %17 = getelementptr inbounds %Real, %Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %Real*, %Real** %5, align 8
  ret %Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @LessII(%Integer* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Boolean*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = icmp slt i32 %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %Boolean*, %Boolean** %5, align 8
  %18 = getelementptr inbounds %Boolean, %Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @LessIR(%Integer* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Boolean*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %Real*, %Real** %4, align 8
  %13 = getelementptr inbounds %Real, %Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fcmp olt float %11, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %Boolean*, %Boolean** %5, align 8
  %19 = getelementptr inbounds %Boolean, %Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @LessEqII(%Integer* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Boolean*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = icmp sle i32 %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %Boolean*, %Boolean** %5, align 8
  %18 = getelementptr inbounds %Boolean, %Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @LessEqIR(%Integer* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Boolean*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %Real*, %Real** %4, align 8
  %13 = getelementptr inbounds %Real, %Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fcmp ole float %11, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %Boolean*, %Boolean** %5, align 8
  %19 = getelementptr inbounds %Boolean, %Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @GreaterII(%Integer* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Boolean*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = icmp sgt i32 %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %Boolean*, %Boolean** %5, align 8
  %18 = getelementptr inbounds %Boolean, %Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @GreaterIR(%Integer* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Boolean*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %Real*, %Real** %4, align 8
  %13 = getelementptr inbounds %Real, %Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fcmp ogt float %11, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %Boolean*, %Boolean** %5, align 8
  %19 = getelementptr inbounds %Boolean, %Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @GreaterEqII(%Integer* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Boolean*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = icmp sge i32 %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %Boolean*, %Boolean** %5, align 8
  %18 = getelementptr inbounds %Boolean, %Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @GreaterEqIR(%Integer* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Boolean*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %Real*, %Real** %4, align 8
  %13 = getelementptr inbounds %Real, %Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fcmp oge float %11, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %Boolean*, %Boolean** %5, align 8
  %19 = getelementptr inbounds %Boolean, %Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @EqII(%Integer* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Boolean*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = icmp eq i32 %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %Boolean*, %Boolean** %5, align 8
  %18 = getelementptr inbounds %Boolean, %Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @EqR(%Integer* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Integer*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Boolean*, align 8
  store %Integer* %0, %Integer** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Integer*, %Integer** %3, align 8
  %9 = getelementptr inbounds %Integer, %Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %Real*, %Real** %4, align 8
  %13 = getelementptr inbounds %Real, %Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fcmp oeq float %11, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %Boolean*, %Boolean** %5, align 8
  %19 = getelementptr inbounds %Boolean, %Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @UnaryMinusR(%Real* noundef %0) #0 {
  %2 = alloca %Real*, align 8
  store %Real* %0, %Real** %2, align 8
  %3 = load %Real*, %Real** %2, align 8
  %4 = getelementptr inbounds %Real, %Real* %3, i32 0, i32 1
  %5 = load float, float* %4, align 8
  %6 = fneg float %5
  %7 = load %Real*, %Real** %2, align 8
  %8 = getelementptr inbounds %Real, %Real* %7, i32 0, i32 1
  store float %6, float* %8, align 8
  %9 = load %Real*, %Real** %2, align 8
  ret %Real* %9
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Integer* @toInteger(%Real* noundef %0) #0 {
  %2 = alloca %Real*, align 8
  %3 = alloca %Integer*, align 8
  store %Real* %0, %Real** %2, align 8
  %4 = call noalias i8* @malloc(i64 noundef 16) #2
  %5 = bitcast i8* %4 to %Integer*
  store %Integer* %5, %Integer** %3, align 8
  %6 = load %Real*, %Real** %2, align 8
  %7 = getelementptr inbounds %Real, %Real* %6, i32 0, i32 1
  %8 = load float, float* %7, align 8
  %9 = fptosi float %8 to i32
  %10 = load %Integer*, %Integer** %3, align 8
  %11 = getelementptr inbounds %Integer, %Integer* %10, i32 0, i32 1
  store i32 %9, i32* %11, align 8
  %12 = load %Integer*, %Integer** %3, align 8
  ret %Integer* %12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @PlusRR(%Real* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Real*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Real*, %Real** %4, align 8
  %12 = getelementptr inbounds %Real, %Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fadd float %10, %13
  %15 = load %Real*, %Real** %5, align 8
  %16 = getelementptr inbounds %Real, %Real* %15, i32 0, i32 1
  store float %14, float* %16, align 8
  %17 = load %Real*, %Real** %5, align 8
  ret %Real* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @PlusRI(%Real* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Real*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fadd float %10, %14
  %16 = load %Real*, %Real** %5, align 8
  %17 = getelementptr inbounds %Real, %Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %Real*, %Real** %5, align 8
  ret %Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @MinusRR(%Real* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Real*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Real*, %Real** %4, align 8
  %12 = getelementptr inbounds %Real, %Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fsub float %10, %13
  %15 = load %Real*, %Real** %5, align 8
  %16 = getelementptr inbounds %Real, %Real* %15, i32 0, i32 1
  store float %14, float* %16, align 8
  %17 = load %Real*, %Real** %5, align 8
  ret %Real* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @MinusRI(%Real* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Real*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fsub float %10, %14
  %16 = load %Real*, %Real** %5, align 8
  %17 = getelementptr inbounds %Real, %Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %Real*, %Real** %5, align 8
  ret %Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @MultRR(%Real* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Real*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Real*, %Real** %4, align 8
  %12 = getelementptr inbounds %Real, %Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fmul float %10, %13
  %15 = load %Real*, %Real** %5, align 8
  %16 = getelementptr inbounds %Real, %Real* %15, i32 0, i32 1
  store float %14, float* %16, align 8
  %17 = load %Real*, %Real** %5, align 8
  ret %Real* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @MultRI(%Real* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Real*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fmul float %10, %14
  %16 = load %Real*, %Real** %5, align 8
  %17 = getelementptr inbounds %Real, %Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %Real*, %Real** %5, align 8
  ret %Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @DivRR(%Real* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Real*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Real*, %Real** %4, align 8
  %12 = getelementptr inbounds %Real, %Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fdiv float %10, %13
  %15 = load %Real*, %Real** %5, align 8
  %16 = getelementptr inbounds %Real, %Real* %15, i32 0, i32 1
  store float %14, float* %16, align 8
  %17 = load %Real*, %Real** %5, align 8
  ret %Real* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Real* @DivRI(%Real* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Real*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Real*
  store %Real* %7, %Real** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fdiv float %10, %14
  %16 = load %Real*, %Real** %5, align 8
  %17 = getelementptr inbounds %Real, %Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %Real*, %Real** %5, align 8
  ret %Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @LessRR(%Real* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Boolean*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Real*, %Real** %4, align 8
  %12 = getelementptr inbounds %Real, %Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fcmp olt float %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %Boolean*, %Boolean** %5, align 8
  %18 = getelementptr inbounds %Boolean, %Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @LessRI(%Real* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Boolean*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fcmp olt float %10, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %Boolean*, %Boolean** %5, align 8
  %19 = getelementptr inbounds %Boolean, %Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @LessEqRR(%Real* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Boolean*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Real*, %Real** %4, align 8
  %12 = getelementptr inbounds %Real, %Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fcmp ole float %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %Boolean*, %Boolean** %5, align 8
  %18 = getelementptr inbounds %Boolean, %Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @LessEqRI(%Real* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Boolean*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fcmp ole float %10, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %Boolean*, %Boolean** %5, align 8
  %19 = getelementptr inbounds %Boolean, %Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @GreaterRR(%Real* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Boolean*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Real*, %Real** %4, align 8
  %12 = getelementptr inbounds %Real, %Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fcmp ogt float %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %Boolean*, %Boolean** %5, align 8
  %18 = getelementptr inbounds %Boolean, %Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @GreaterRI(%Real* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Boolean*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fcmp ogt float %10, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %Boolean*, %Boolean** %5, align 8
  %19 = getelementptr inbounds %Boolean, %Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @GreaterEqRR(%Real* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Boolean*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Real*, %Real** %4, align 8
  %12 = getelementptr inbounds %Real, %Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fcmp oge float %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %Boolean*, %Boolean** %5, align 8
  %18 = getelementptr inbounds %Boolean, %Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @GreaterEqRI(%Real* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Boolean*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fcmp oge float %10, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %Boolean*, %Boolean** %5, align 8
  %19 = getelementptr inbounds %Boolean, %Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @EqRR(%Real* noundef %0, %Real* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Real*, align 8
  %5 = alloca %Boolean*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Real* %1, %Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Real*, %Real** %4, align 8
  %12 = getelementptr inbounds %Real, %Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fcmp oeq float %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %Boolean*, %Boolean** %5, align 8
  %18 = getelementptr inbounds %Boolean, %Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @EqRI(%Real* noundef %0, %Integer* noundef %1) #0 {
  %3 = alloca %Real*, align 8
  %4 = alloca %Integer*, align 8
  %5 = alloca %Boolean*, align 8
  store %Real* %0, %Real** %3, align 8
  store %Integer* %1, %Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Real*, %Real** %3, align 8
  %9 = getelementptr inbounds %Real, %Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %Integer*, %Integer** %4, align 8
  %12 = getelementptr inbounds %Integer, %Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fcmp oeq float %10, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %Boolean*, %Boolean** %5, align 8
  %19 = getelementptr inbounds %Boolean, %Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Integer* @toIntegerB(%Boolean* noundef %0) #0 {
  %2 = alloca %Boolean*, align 8
  %3 = alloca %Integer*, align 8
  store %Boolean* %0, %Boolean** %2, align 8
  %4 = call noalias i8* @malloc(i64 noundef 16) #2
  %5 = bitcast i8* %4 to %Integer*
  store %Integer* %5, %Integer** %3, align 8
  %6 = load %Boolean*, %Boolean** %2, align 8
  %7 = getelementptr inbounds %Boolean, %Boolean* %6, i32 0, i32 1
  %8 = load i8, i8* %7, align 8
  %9 = sext i8 %8 to i32
  %10 = load %Integer*, %Integer** %3, align 8
  %11 = getelementptr inbounds %Integer, %Integer* %10, i32 0, i32 1
  store i32 %9, i32* %11, align 8
  %12 = load %Integer*, %Integer** %3, align 8
  ret %Integer* %12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @Or(%Boolean* noundef %0, %Boolean* noundef %1) #0 {
  %3 = alloca %Boolean*, align 8
  %4 = alloca %Boolean*, align 8
  %5 = alloca %Boolean*, align 8
  store %Boolean* %0, %Boolean** %3, align 8
  store %Boolean* %1, %Boolean** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Boolean*, %Boolean** %3, align 8
  %9 = getelementptr inbounds %Boolean, %Boolean* %8, i32 0, i32 1
  %10 = load i8, i8* %9, align 8
  %11 = sext i8 %10 to i32
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %19, label %13

13:                                               ; preds = %2
  %14 = load %Boolean*, %Boolean** %4, align 8
  %15 = getelementptr inbounds %Boolean, %Boolean* %14, i32 0, i32 1
  %16 = load i8, i8* %15, align 8
  %17 = sext i8 %16 to i32
  %18 = icmp ne i32 %17, 0
  br label %19

19:                                               ; preds = %13, %2
  %20 = phi i1 [ true, %2 ], [ %18, %13 ]
  %21 = zext i1 %20 to i32
  %22 = trunc i32 %21 to i8
  %23 = load %Boolean*, %Boolean** %5, align 8
  %24 = getelementptr inbounds %Boolean, %Boolean* %23, i32 0, i32 1
  store i8 %22, i8* %24, align 8
  %25 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %25
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @And(%Boolean* noundef %0, %Boolean* noundef %1) #0 {
  %3 = alloca %Boolean*, align 8
  %4 = alloca %Boolean*, align 8
  %5 = alloca %Boolean*, align 8
  store %Boolean* %0, %Boolean** %3, align 8
  store %Boolean* %1, %Boolean** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Boolean*, %Boolean** %3, align 8
  %9 = getelementptr inbounds %Boolean, %Boolean* %8, i32 0, i32 1
  %10 = load i8, i8* %9, align 8
  %11 = sext i8 %10 to i32
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %13, label %19

13:                                               ; preds = %2
  %14 = load %Boolean*, %Boolean** %4, align 8
  %15 = getelementptr inbounds %Boolean, %Boolean* %14, i32 0, i32 1
  %16 = load i8, i8* %15, align 8
  %17 = sext i8 %16 to i32
  %18 = icmp ne i32 %17, 0
  br label %19

19:                                               ; preds = %13, %2
  %20 = phi i1 [ false, %2 ], [ %18, %13 ]
  %21 = zext i1 %20 to i32
  %22 = trunc i32 %21 to i8
  %23 = load %Boolean*, %Boolean** %5, align 8
  %24 = getelementptr inbounds %Boolean, %Boolean* %23, i32 0, i32 1
  store i8 %22, i8* %24, align 8
  %25 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %25
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @Xor(%Boolean* noundef %0, %Boolean* noundef %1) #0 {
  %3 = alloca %Boolean*, align 8
  %4 = alloca %Boolean*, align 8
  %5 = alloca %Boolean*, align 8
  store %Boolean* %0, %Boolean** %3, align 8
  store %Boolean* %1, %Boolean** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %Boolean*
  store %Boolean* %7, %Boolean** %5, align 8
  %8 = load %Boolean*, %Boolean** %3, align 8
  %9 = getelementptr inbounds %Boolean, %Boolean* %8, i32 0, i32 1
  %10 = load i8, i8* %9, align 8
  %11 = sext i8 %10 to i32
  %12 = load %Boolean*, %Boolean** %4, align 8
  %13 = getelementptr inbounds %Boolean, %Boolean* %12, i32 0, i32 1
  %14 = load i8, i8* %13, align 8
  %15 = sext i8 %14 to i32
  %16 = xor i32 %11, %15
  %17 = trunc i32 %16 to i8
  %18 = load %Boolean*, %Boolean** %5, align 8
  %19 = getelementptr inbounds %Boolean, %Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %Boolean*, %Boolean** %5, align 8
  ret %Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %Boolean* @Not(%Boolean* noundef %0) #0 {
  %2 = alloca %Boolean*, align 8
  store %Boolean* %0, %Boolean** %2, align 8
  %3 = load %Boolean*, %Boolean** %2, align 8
  %4 = getelementptr inbounds %Boolean, %Boolean* %3, i32 0, i32 1
  %5 = load i8, i8* %4, align 8
  %6 = icmp ne i8 %5, 0
  %7 = xor i1 %6, true
  %8 = zext i1 %7 to i32
  %9 = trunc i32 %8 to i8
  %10 = load %Boolean*, %Boolean** %2, align 8
  %11 = getelementptr inbounds %Boolean, %Boolean* %10, i32 0, i32 1
  store i8 %9, i8* %11, align 8
  %12 = load %Boolean*, %Boolean** %2, align 8
  ret %Boolean* %12
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
