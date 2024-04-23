# OLangCompiler
Compiler for the OLang language

# Prerequisite

```shell
sudo apt install llvm
```

# CompileProgram

1. To compile file
2. Execute
   ```shell
   llc ./out.ll
   ```
3. Compile with `clang`
   ```shell
   clang-15 out.s -no-pie -o out
   ```