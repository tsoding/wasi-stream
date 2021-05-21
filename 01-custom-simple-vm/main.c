// Copyright 2021 Alexey Kutepov <reximkut@gmail.com>

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    INST_PUSH,
    INST_ADD,
    INST_PRINT
} Inst_Type;

typedef struct {
    Inst_Type type;
    int operand;
} Inst;

Inst program[] = {
    {.type = INST_PUSH, .operand = 35},
    {.type = INST_PUSH, .operand = 34},
    {.type = INST_ADD},
    {.type = INST_PRINT},
};
#define PROGRAM_SIZE (sizeof(program) / sizeof(program[0]))

#define STACK_CAPACITY 1024
int stack[STACK_CAPACITY];
size_t stack_size = 0;

void stack_push(int value)
{
    assert(stack_size < STACK_CAPACITY);
    stack[stack_size++] = value;
}

int stack_pop(void)
{
    assert(stack_size > 0);
    return stack[--stack_size];
}

void save_program_to_file(const char *file_path)
{
    FILE *f = fopen(file_path, "wb");
    fwrite(program, sizeof(program[0]), PROGRAM_SIZE, f);
    fclose(f);
}

int main()
{
    save_program_to_file("program.vm");

    for (size_t ip = 0; ip < PROGRAM_SIZE; ++ip) {
        switch (program[ip].type) {
        case INST_PUSH:
            stack_push(program[ip].operand);
            break;
        case INST_ADD: {
            int a = stack_pop();
            int b = stack_pop();
            stack_push(a + b);
        }
        break;
        case INST_PRINT:
            printf("%d\n", stack_pop());
            break;
        default:
            assert(0 && "Invalid instruction");
        }
    }

    return 0;
}
