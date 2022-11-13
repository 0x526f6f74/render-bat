<p align="center">
    <img src="https://cdn.discordapp.com/attachments/1018900693312090152/1018900722777079928/main.png" width="512">
    <br>
    <br>
    <i>Render Bat Style Guide</i>
    <br>
    <hr>
</p>

# Render Bat Style Guide

## Table of Contents

- [C++ specific style](#c-specific-style)
- [Naming](#naming)
- [Formatting](#formatting)

## C++ specific style

- Use `int` as your main type for integers.
- Use `const` as much as possible on member functions.
- Avoid declaring primitive function parameters as `const`.
- Avoid `noexcept` and `[[nodiscard]]`.

## Naming

- C++ source files should end in `.cc` and header files in `.h`.
- Variables, functions and namespaces: `snake_case` *(try to keep namespace names to one word)*
- Constants and macros: `MACRO_CASE`
- Classes and structs: `PascalCase`

## Formatting

- All source files are formatted using [**Clang Format**](https://clang.llvm.org/docs/ClangFormat.html) according to `.clang-format`.
