# Test files

It is a common practice to do unit tests of each class before you integrate it into the project to validate its operation.
In this folder, you can create your own unit test files to validate the operation of your components.

> It might be a good idea to also take some notes about the tests since you are required to 
  report these in the final report.

# Unit Tests

## Test of MyClass

**Involved Classes:**

**Test File:**

**Results:**

Tests can be run in VS code configurations or in terminal with
```
g++ -std=c++17 -Iinclude $(find src -name '*.cpp' ! -name 'main.cpp') $(find tests -name '*.cpp') -o build/tests -lsfml-graphics -lsfml-window -lsfml-system

./build/tests
```
Or similarly using valgrind
```
valgrind ./build/tests
```