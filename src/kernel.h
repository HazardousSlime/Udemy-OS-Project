#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

void kernel_main();
void print(const char* str);
void puts(const char* str);
//void problem_escape();

#endif //KERNEL_H
