#include <stdbool.h>
#include "kernel.h"
#include "memory/memory.h"

void assert_func(bool cond, const char *file, int line, const char* func){
	if(cond)
		return;
	print(file);
	char line_str[12];
	print(":"); print(itoa(line, line_str));
	print(": "); print(func);
	puts(": Assertion failed.");
	print("====END====");
	while(1);
}
