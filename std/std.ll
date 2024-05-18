; ModuleID = 'std.c'
source_filename = "std.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.Integer = type { i32*, i32 }
%struct.Real = type { i32*, float }
%struct.Boolean = type { i32*, i8 }
%struct.IntegerArray = type { i32*, %struct.Integer*, %struct.Integer* }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Integer* @UnaryMinusI(%struct.Integer* noundef %0) #0 {
  %2 = alloca %struct.Integer*, align 8
  store %struct.Integer* %0, %struct.Integer** %2, align 8
  %3 = load %struct.Integer*, %struct.Integer** %2, align 8
  %4 = getelementptr inbounds %struct.Integer, %struct.Integer* %3, i32 0, i32 1
  %5 = load i32, i32* %4, align 8
  %6 = sub nsw i32 0, %5
  %7 = load %struct.Integer*, %struct.Integer** %2, align 8
  %8 = getelementptr inbounds %struct.Integer, %struct.Integer* %7, i32 0, i32 1
  store i32 %6, i32* %8, align 8
  %9 = load %struct.Integer*, %struct.Integer** %2, align 8
  ret %struct.Integer* %9
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @toRealI(%struct.Integer* noundef %0) #0 {
  %2 = alloca %struct.Integer*, align 8
  %3 = alloca %struct.Real*, align 8
  store %struct.Integer* %0, %struct.Integer** %2, align 8
  %4 = call noalias i8* @malloc(i64 noundef 16) #2
  %5 = bitcast i8* %4 to %struct.Real*
  store %struct.Real* %5, %struct.Real** %3, align 8
  %6 = load %struct.Integer*, %struct.Integer** %2, align 8
  %7 = getelementptr inbounds %struct.Integer, %struct.Integer* %6, i32 0, i32 1
  %8 = load i32, i32* %7, align 8
  %9 = sitofp i32 %8 to float
  %10 = load %struct.Real*, %struct.Real** %3, align 8
  %11 = getelementptr inbounds %struct.Real, %struct.Real* %10, i32 0, i32 1
  store float %9, float* %11, align 8
  %12 = load %struct.Real*, %struct.Real** %3, align 8
  ret %struct.Real* %12
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Integer* @PlusII(%struct.Integer* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Integer*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Integer*
  store %struct.Integer* %7, %struct.Integer** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = add nsw i32 %10, %13
  %15 = load %struct.Integer*, %struct.Integer** %5, align 8
  %16 = getelementptr inbounds %struct.Integer, %struct.Integer* %15, i32 0, i32 1
  store i32 %14, i32* %16, align 8
  %17 = load %struct.Integer*, %struct.Integer** %5, align 8
  ret %struct.Integer* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @PlusIR(%struct.Integer* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %struct.Real*, %struct.Real** %4, align 8
  %13 = getelementptr inbounds %struct.Real, %struct.Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fadd float %11, %14
  %16 = load %struct.Real*, %struct.Real** %5, align 8
  %17 = getelementptr inbounds %struct.Real, %struct.Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Integer* @MinusII(%struct.Integer* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Integer*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Integer*
  store %struct.Integer* %7, %struct.Integer** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sub nsw i32 %10, %13
  %15 = load %struct.Integer*, %struct.Integer** %5, align 8
  %16 = getelementptr inbounds %struct.Integer, %struct.Integer* %15, i32 0, i32 1
  store i32 %14, i32* %16, align 8
  %17 = load %struct.Integer*, %struct.Integer** %5, align 8
  ret %struct.Integer* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @MinusIR(%struct.Integer* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %struct.Real*, %struct.Real** %4, align 8
  %13 = getelementptr inbounds %struct.Real, %struct.Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fsub float %11, %14
  %16 = load %struct.Real*, %struct.Real** %5, align 8
  %17 = getelementptr inbounds %struct.Real, %struct.Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Integer* @MultII(%struct.Integer* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Integer*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Integer*
  store %struct.Integer* %7, %struct.Integer** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = mul nsw i32 %10, %13
  %15 = load %struct.Integer*, %struct.Integer** %5, align 8
  %16 = getelementptr inbounds %struct.Integer, %struct.Integer* %15, i32 0, i32 1
  store i32 %14, i32* %16, align 8
  %17 = load %struct.Integer*, %struct.Integer** %5, align 8
  ret %struct.Integer* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @MultIR(%struct.Integer* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %struct.Real*, %struct.Real** %4, align 8
  %13 = getelementptr inbounds %struct.Real, %struct.Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fmul float %11, %14
  %16 = load %struct.Real*, %struct.Real** %5, align 8
  %17 = getelementptr inbounds %struct.Real, %struct.Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Integer* @DivII(%struct.Integer* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Integer*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Integer*
  store %struct.Integer* %7, %struct.Integer** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sdiv i32 %10, %13
  %15 = load %struct.Integer*, %struct.Integer** %5, align 8
  %16 = getelementptr inbounds %struct.Integer, %struct.Integer* %15, i32 0, i32 1
  store i32 %14, i32* %16, align 8
  %17 = load %struct.Integer*, %struct.Integer** %5, align 8
  ret %struct.Integer* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @DivIR(%struct.Integer* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %struct.Real*, %struct.Real** %4, align 8
  %13 = getelementptr inbounds %struct.Real, %struct.Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fdiv float %11, %14
  %16 = load %struct.Real*, %struct.Real** %5, align 8
  %17 = getelementptr inbounds %struct.Real, %struct.Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @LessII(%struct.Integer* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = icmp slt i32 %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %18 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @LessIR(%struct.Integer* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %struct.Real*, %struct.Real** %4, align 8
  %13 = getelementptr inbounds %struct.Real, %struct.Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fcmp olt float %11, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %19 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @LessEqII(%struct.Integer* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = icmp sle i32 %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %18 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @LessEqIR(%struct.Integer* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %struct.Real*, %struct.Real** %4, align 8
  %13 = getelementptr inbounds %struct.Real, %struct.Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fcmp ole float %11, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %19 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @GreaterII(%struct.Integer* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = icmp sgt i32 %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %18 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @GreaterIR(%struct.Integer* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %struct.Real*, %struct.Real** %4, align 8
  %13 = getelementptr inbounds %struct.Real, %struct.Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fcmp ogt float %11, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %19 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @GreaterEqII(%struct.Integer* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = icmp sge i32 %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %18 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @GreaterEqIR(%struct.Integer* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %struct.Real*, %struct.Real** %4, align 8
  %13 = getelementptr inbounds %struct.Real, %struct.Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fcmp oge float %11, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %19 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @EqII(%struct.Integer* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = icmp eq i32 %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %18 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @EqIR(%struct.Integer* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Integer*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Integer* %0, %struct.Integer** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Integer*, %struct.Integer** %3, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sitofp i32 %10 to float
  %12 = load %struct.Real*, %struct.Real** %4, align 8
  %13 = getelementptr inbounds %struct.Real, %struct.Real* %12, i32 0, i32 1
  %14 = load float, float* %13, align 8
  %15 = fcmp oeq float %11, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %19 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @UnaryMinusR(%struct.Real* noundef %0) #0 {
  %2 = alloca %struct.Real*, align 8
  store %struct.Real* %0, %struct.Real** %2, align 8
  %3 = load %struct.Real*, %struct.Real** %2, align 8
  %4 = getelementptr inbounds %struct.Real, %struct.Real* %3, i32 0, i32 1
  %5 = load float, float* %4, align 8
  %6 = fneg float %5
  %7 = load %struct.Real*, %struct.Real** %2, align 8
  %8 = getelementptr inbounds %struct.Real, %struct.Real* %7, i32 0, i32 1
  store float %6, float* %8, align 8
  %9 = load %struct.Real*, %struct.Real** %2, align 8
  ret %struct.Real* %9
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Integer* @toIntegerR(%struct.Real* noundef %0) #0 {
  %2 = alloca %struct.Real*, align 8
  %3 = alloca %struct.Integer*, align 8
  store %struct.Real* %0, %struct.Real** %2, align 8
  %4 = call noalias i8* @malloc(i64 noundef 16) #2
  %5 = bitcast i8* %4 to %struct.Integer*
  store %struct.Integer* %5, %struct.Integer** %3, align 8
  %6 = load %struct.Real*, %struct.Real** %2, align 8
  %7 = getelementptr inbounds %struct.Real, %struct.Real* %6, i32 0, i32 1
  %8 = load float, float* %7, align 8
  %9 = fptosi float %8 to i32
  %10 = load %struct.Integer*, %struct.Integer** %3, align 8
  %11 = getelementptr inbounds %struct.Integer, %struct.Integer* %10, i32 0, i32 1
  store i32 %9, i32* %11, align 8
  %12 = load %struct.Integer*, %struct.Integer** %3, align 8
  ret %struct.Integer* %12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @PlusRR(%struct.Real* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Real*, %struct.Real** %4, align 8
  %12 = getelementptr inbounds %struct.Real, %struct.Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fadd float %10, %13
  %15 = load %struct.Real*, %struct.Real** %5, align 8
  %16 = getelementptr inbounds %struct.Real, %struct.Real* %15, i32 0, i32 1
  store float %14, float* %16, align 8
  %17 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @PlusRI(%struct.Real* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fadd float %10, %14
  %16 = load %struct.Real*, %struct.Real** %5, align 8
  %17 = getelementptr inbounds %struct.Real, %struct.Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @MinusRR(%struct.Real* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Real*, %struct.Real** %4, align 8
  %12 = getelementptr inbounds %struct.Real, %struct.Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fsub float %10, %13
  %15 = load %struct.Real*, %struct.Real** %5, align 8
  %16 = getelementptr inbounds %struct.Real, %struct.Real* %15, i32 0, i32 1
  store float %14, float* %16, align 8
  %17 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @MinusRI(%struct.Real* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fsub float %10, %14
  %16 = load %struct.Real*, %struct.Real** %5, align 8
  %17 = getelementptr inbounds %struct.Real, %struct.Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @MultRR(%struct.Real* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Real*, %struct.Real** %4, align 8
  %12 = getelementptr inbounds %struct.Real, %struct.Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fmul float %10, %13
  %15 = load %struct.Real*, %struct.Real** %5, align 8
  %16 = getelementptr inbounds %struct.Real, %struct.Real* %15, i32 0, i32 1
  store float %14, float* %16, align 8
  %17 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @MultRI(%struct.Real* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fmul float %10, %14
  %16 = load %struct.Real*, %struct.Real** %5, align 8
  %17 = getelementptr inbounds %struct.Real, %struct.Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @DivRR(%struct.Real* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Real*, %struct.Real** %4, align 8
  %12 = getelementptr inbounds %struct.Real, %struct.Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fdiv float %10, %13
  %15 = load %struct.Real*, %struct.Real** %5, align 8
  %16 = getelementptr inbounds %struct.Real, %struct.Real* %15, i32 0, i32 1
  store float %14, float* %16, align 8
  %17 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Real* @DivRI(%struct.Real* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Real*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Real*
  store %struct.Real* %7, %struct.Real** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fdiv float %10, %14
  %16 = load %struct.Real*, %struct.Real** %5, align 8
  %17 = getelementptr inbounds %struct.Real, %struct.Real* %16, i32 0, i32 1
  store float %15, float* %17, align 8
  %18 = load %struct.Real*, %struct.Real** %5, align 8
  ret %struct.Real* %18
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @LessRR(%struct.Real* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Real*, %struct.Real** %4, align 8
  %12 = getelementptr inbounds %struct.Real, %struct.Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fcmp olt float %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %18 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @LessRI(%struct.Real* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fcmp olt float %10, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %19 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @LessEqRR(%struct.Real* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Real*, %struct.Real** %4, align 8
  %12 = getelementptr inbounds %struct.Real, %struct.Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fcmp ole float %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %18 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @LessEqRI(%struct.Real* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fcmp ole float %10, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %19 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @GreaterRR(%struct.Real* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Real*, %struct.Real** %4, align 8
  %12 = getelementptr inbounds %struct.Real, %struct.Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fcmp ogt float %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %18 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @GreaterRI(%struct.Real* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fcmp ogt float %10, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %19 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @GreaterEqRR(%struct.Real* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Real*, %struct.Real** %4, align 8
  %12 = getelementptr inbounds %struct.Real, %struct.Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fcmp oge float %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %18 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @GreaterEqRI(%struct.Real* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fcmp oge float %10, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %19 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @EqRR(%struct.Real* noundef %0, %struct.Real* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Real*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Real* %1, %struct.Real** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Real*, %struct.Real** %4, align 8
  %12 = getelementptr inbounds %struct.Real, %struct.Real* %11, i32 0, i32 1
  %13 = load float, float* %12, align 8
  %14 = fcmp oeq float %10, %13
  %15 = zext i1 %14 to i32
  %16 = trunc i32 %15 to i8
  %17 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %18 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %17, i32 0, i32 1
  store i8 %16, i8* %18, align 8
  %19 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %19
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @EqRI(%struct.Real* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.Real*, align 8
  %4 = alloca %struct.Integer*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Real* %0, %struct.Real** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Real*, %struct.Real** %3, align 8
  %9 = getelementptr inbounds %struct.Real, %struct.Real* %8, i32 0, i32 1
  %10 = load float, float* %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %4, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sitofp i32 %13 to float
  %15 = fcmp oeq float %10, %14
  %16 = zext i1 %15 to i32
  %17 = trunc i32 %16 to i8
  %18 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %19 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Integer* @toIntegerB(%struct.Boolean* noundef %0) #0 {
  %2 = alloca %struct.Boolean*, align 8
  %3 = alloca %struct.Integer*, align 8
  store %struct.Boolean* %0, %struct.Boolean** %2, align 8
  %4 = call noalias i8* @malloc(i64 noundef 16) #2
  %5 = bitcast i8* %4 to %struct.Integer*
  store %struct.Integer* %5, %struct.Integer** %3, align 8
  %6 = load %struct.Boolean*, %struct.Boolean** %2, align 8
  %7 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %6, i32 0, i32 1
  %8 = load i8, i8* %7, align 8
  %9 = sext i8 %8 to i32
  %10 = load %struct.Integer*, %struct.Integer** %3, align 8
  %11 = getelementptr inbounds %struct.Integer, %struct.Integer* %10, i32 0, i32 1
  store i32 %9, i32* %11, align 8
  %12 = load %struct.Integer*, %struct.Integer** %3, align 8
  ret %struct.Integer* %12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @OrB(%struct.Boolean* noundef %0, %struct.Boolean* noundef %1) #0 {
  %3 = alloca %struct.Boolean*, align 8
  %4 = alloca %struct.Boolean*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Boolean* %0, %struct.Boolean** %3, align 8
  store %struct.Boolean* %1, %struct.Boolean** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Boolean*, %struct.Boolean** %3, align 8
  %9 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %8, i32 0, i32 1
  %10 = load i8, i8* %9, align 8
  %11 = sext i8 %10 to i32
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %19, label %13

13:                                               ; preds = %2
  %14 = load %struct.Boolean*, %struct.Boolean** %4, align 8
  %15 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %14, i32 0, i32 1
  %16 = load i8, i8* %15, align 8
  %17 = sext i8 %16 to i32
  %18 = icmp ne i32 %17, 0
  br label %19

19:                                               ; preds = %13, %2
  %20 = phi i1 [ true, %2 ], [ %18, %13 ]
  %21 = zext i1 %20 to i32
  %22 = trunc i32 %21 to i8
  %23 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %24 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %23, i32 0, i32 1
  store i8 %22, i8* %24, align 8
  %25 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %25
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @AndB(%struct.Boolean* noundef %0, %struct.Boolean* noundef %1) #0 {
  %3 = alloca %struct.Boolean*, align 8
  %4 = alloca %struct.Boolean*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Boolean* %0, %struct.Boolean** %3, align 8
  store %struct.Boolean* %1, %struct.Boolean** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Boolean*, %struct.Boolean** %3, align 8
  %9 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %8, i32 0, i32 1
  %10 = load i8, i8* %9, align 8
  %11 = sext i8 %10 to i32
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %13, label %19

13:                                               ; preds = %2
  %14 = load %struct.Boolean*, %struct.Boolean** %4, align 8
  %15 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %14, i32 0, i32 1
  %16 = load i8, i8* %15, align 8
  %17 = sext i8 %16 to i32
  %18 = icmp ne i32 %17, 0
  br label %19

19:                                               ; preds = %13, %2
  %20 = phi i1 [ false, %2 ], [ %18, %13 ]
  %21 = zext i1 %20 to i32
  %22 = trunc i32 %21 to i8
  %23 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %24 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %23, i32 0, i32 1
  store i8 %22, i8* %24, align 8
  %25 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %25
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @XorB(%struct.Boolean* noundef %0, %struct.Boolean* noundef %1) #0 {
  %3 = alloca %struct.Boolean*, align 8
  %4 = alloca %struct.Boolean*, align 8
  %5 = alloca %struct.Boolean*, align 8
  store %struct.Boolean* %0, %struct.Boolean** %3, align 8
  store %struct.Boolean* %1, %struct.Boolean** %4, align 8
  %6 = call noalias i8* @malloc(i64 noundef 16) #2
  %7 = bitcast i8* %6 to %struct.Boolean*
  store %struct.Boolean* %7, %struct.Boolean** %5, align 8
  %8 = load %struct.Boolean*, %struct.Boolean** %3, align 8
  %9 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %8, i32 0, i32 1
  %10 = load i8, i8* %9, align 8
  %11 = sext i8 %10 to i32
  %12 = load %struct.Boolean*, %struct.Boolean** %4, align 8
  %13 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %12, i32 0, i32 1
  %14 = load i8, i8* %13, align 8
  %15 = sext i8 %14 to i32
  %16 = xor i32 %11, %15
  %17 = trunc i32 %16 to i8
  %18 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  %19 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %18, i32 0, i32 1
  store i8 %17, i8* %19, align 8
  %20 = load %struct.Boolean*, %struct.Boolean** %5, align 8
  ret %struct.Boolean* %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Boolean* @NotB(%struct.Boolean* noundef %0) #0 {
  %2 = alloca %struct.Boolean*, align 8
  store %struct.Boolean* %0, %struct.Boolean** %2, align 8
  %3 = load %struct.Boolean*, %struct.Boolean** %2, align 8
  %4 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %3, i32 0, i32 1
  %5 = load i8, i8* %4, align 8
  %6 = icmp ne i8 %5, 0
  %7 = xor i1 %6, true
  %8 = zext i1 %7 to i32
  %9 = trunc i32 %8 to i8
  %10 = load %struct.Boolean*, %struct.Boolean** %2, align 8
  %11 = getelementptr inbounds %struct.Boolean, %struct.Boolean* %10, i32 0, i32 1
  store i8 %9, i8* %11, align 8
  %12 = load %struct.Boolean*, %struct.Boolean** %2, align 8
  ret %struct.Boolean* %12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.IntegerArray* @initArray(%struct.IntegerArray* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.IntegerArray*, align 8
  %4 = alloca %struct.Integer*, align 8
  store %struct.IntegerArray* %0, %struct.IntegerArray** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %5 = load %struct.Integer*, %struct.Integer** %4, align 8
  %6 = getelementptr inbounds %struct.Integer, %struct.Integer* %5, i32 0, i32 1
  %7 = load i32, i32* %6, align 8
  %8 = sext i32 %7 to i64
  %9 = mul i64 16, %8
  %10 = call noalias i8* @malloc(i64 noundef %9) #2
  %11 = bitcast i8* %10 to %struct.Integer*
  %12 = load %struct.IntegerArray*, %struct.IntegerArray** %3, align 8
  %13 = getelementptr inbounds %struct.IntegerArray, %struct.IntegerArray* %12, i32 0, i32 1
  store %struct.Integer* %11, %struct.Integer** %13, align 8
  %14 = load %struct.Integer*, %struct.Integer** %4, align 8
  %15 = load %struct.IntegerArray*, %struct.IntegerArray** %3, align 8
  %16 = getelementptr inbounds %struct.IntegerArray, %struct.IntegerArray* %15, i32 0, i32 2
  store %struct.Integer* %14, %struct.Integer** %16, align 8
  %17 = load %struct.IntegerArray*, %struct.IntegerArray** %3, align 8
  ret %struct.IntegerArray* %17
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.Integer* @getElement(%struct.IntegerArray* noundef %0, %struct.Integer* noundef %1) #0 {
  %3 = alloca %struct.IntegerArray*, align 8
  %4 = alloca %struct.Integer*, align 8
  store %struct.IntegerArray* %0, %struct.IntegerArray** %3, align 8
  store %struct.Integer* %1, %struct.Integer** %4, align 8
  %5 = load %struct.IntegerArray*, %struct.IntegerArray** %3, align 8
  %6 = getelementptr inbounds %struct.IntegerArray, %struct.IntegerArray* %5, i32 0, i32 1
  %7 = load %struct.Integer*, %struct.Integer** %6, align 8
  %8 = load %struct.Integer*, %struct.Integer** %4, align 8
  %9 = getelementptr inbounds %struct.Integer, %struct.Integer* %8, i32 0, i32 1
  %10 = load i32, i32* %9, align 8
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %7, i64 %11
  ret %struct.Integer* %12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @setElement(%struct.IntegerArray* noundef %0, %struct.Integer* noundef %1, %struct.Integer* noundef %2) #0 {
  %4 = alloca %struct.IntegerArray*, align 8
  %5 = alloca %struct.Integer*, align 8
  %6 = alloca %struct.Integer*, align 8
  %7 = alloca %struct.Integer*, align 8
  store %struct.IntegerArray* %0, %struct.IntegerArray** %4, align 8
  store %struct.Integer* %1, %struct.Integer** %5, align 8
  store %struct.Integer* %2, %struct.Integer** %6, align 8
  %8 = load %struct.IntegerArray*, %struct.IntegerArray** %4, align 8
  %9 = getelementptr inbounds %struct.IntegerArray, %struct.IntegerArray* %8, i32 0, i32 1
  %10 = load %struct.Integer*, %struct.Integer** %9, align 8
  %11 = load %struct.Integer*, %struct.Integer** %5, align 8
  %12 = getelementptr inbounds %struct.Integer, %struct.Integer* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 8
  %14 = sext i32 %13 to i64
  %15 = getelementptr inbounds %struct.Integer, %struct.Integer* %10, i64 %14
  store %struct.Integer* %15, %struct.Integer** %7, align 8
  %16 = load %struct.Integer*, %struct.Integer** %6, align 8
  %17 = getelementptr inbounds %struct.Integer, %struct.Integer* %16, i32 0, i32 1
  %18 = load i32, i32* %17, align 8
  %19 = load %struct.Integer*, %struct.Integer** %7, align 8
  %20 = getelementptr inbounds %struct.Integer, %struct.Integer* %19, i32 0, i32 1
  store i32 %18, i32* %20, align 8
  ret void
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
