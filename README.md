# OLangCompiler
Compiler for the OLang language

# Prerequisite

```shell
sudo apt install llvm
sudo apt install clang-15
```

# CompileProgram

1. To compile file. Use command 
   ```shell
   ./olang "PROGRAM" "OUTFILE"
   PROGRAM - program file path
   OUTFILE - output file path
   ```
2. Execute
   ```shell
   llc "OUTFILE"
   OUTFILE - output file path
   ```
   The file "OUTFILE.s" will appear
3. Compile with `clang`
   ```shell
   clang++-15 "OUTFILE.s" ./std/std.ll -no-pie -o "EXECUTABLE"
   OUTFILE.s - output file after compilation
   EXECUTABLE - executable file
   ```

# Example

```shell
./olang "/home/razerford/Рабочий стол/OLangCompiler/tests/1_test.olg" "out"
llc ./out
clang++-15 out.s ./std/std.ll -no-pie -o out
./out
```
