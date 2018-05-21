#include <console.h>
#include <keyboard.h>
#include <pixels.h>

void User_ConsoleWriteCharacter(unsigned char c)
{
	asm volatile("movl $1, %%eax\n\t"
				 "movzx %0, %%ebx\n\t"
				 "int $0x80\n"
				 : : "b"(c)
				);
}

void User_ConsoleWriteString(char* str)
{
	asm volatile("xor %%eax, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "int $0x80\n"
				 : : "b"(str)
				);
}

void User_ConsoleWriteInt(unsigned int i, unsigned int base)
{
	asm volatile("movl $2, %%eax\n\t"
				 "movl %0, %%ecx\n\t"
				 "movl %1, %%ebx\n\t"
				 "int $0x80\n"
				 : : "c"(base), "b"(i)
				);
}

void User_ConsoleClearScreen(const uint8_t c)
{
	asm volatile("movl $3, %%eax\n\t"
				 "movzx %0, %%ebx\n\t"
				 "int $0x80\n"
				 : : "b"(c)
				);
}

void User_ConsoleGotoXY(unsigned int x, unsigned int y) 
{
	asm volatile("movl $4, %%eax\n\t"
				 "movl %0, %%ecx\n\t"
				 "movl %1, %%ebx\n\t"
				 "int $0x80\n"
				 : : "c"(y), "b"(x)
				);
}

// INTRERUPT 81
void User_CleanScreen(const uint8_t c)
{
	asm volatile("movl $0, %%eax\n\t"
				 "movzx %0, %%ebx\n\t"
				 "int $0x81\n"
				 : : "b"(c)
				);
}

void User_Set_Pixel(unsigned int x, unsigned int y, uint8_t c) 
{
	asm volatile("movl $1, %%eax\n\t"
				 "movl %0, %%ecx\n\t"
				 "movl %1, %%ebx\n\t"
				 "movzx %2, %%edx\n\t"
				 "int $0x81\n"
				 : : "c"(x), "b"(y), "d"(c)
				);
}

void User_Unfilled_Rectangle(int *x, uint8_t c) 
{
	asm volatile("movl $2, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "int $0x81\n"
				 : : "b"(x), "c"(c)
				);
}

void User_Filled_Rectangle(int *x, uint8_t c) 
{
	asm volatile("movl $3, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "int $0x81\n"
				 : : "b"(x), "c"(c)
				);
}

void User_Filled_Triangle(int *x, uint8_t c) 
{
	asm volatile("movl $4, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "int $0x81\n"
				 : : "b"(x), "c"(c)
				);
}


void User_Unfilled_Triangle(int *x, uint8_t c) 
{
	asm volatile("movl $5, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "int $0x81\n"
				 : : "b"(x), "c"(c)
				);
}

void User_Draw_Line(int *x, uint8_t c, int l) 
{
	asm volatile("movl $6, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "movl %2, %%edx\n\t"
				 "int $0x81\n"
				 : : "b"(x), "c"(c), "d"(l)
				);
}

void User_Unfilled_Circle(int *x, uint8_t c) 
{
	asm volatile("movl $7, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "int $0x81\n"
				 : : "b"(x), "c"(c)
				);
}

void User_Filled_Circle(int *x, uint8_t c) 
{
	asm volatile("movl $8, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "int $0x81\n"
				 : : "b"(x), "c"(c)
				);
}

void User_Draw_Filled_Hex(int *x, uint8_t c) 
{
	asm volatile("movl $9, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "int $0x81\n"
				 : : "b"(x), "c"(c)
				);
}

void User_Draw_Unfilled_Hex(int *x, uint8_t c) 
{
	asm volatile("movl $10, %%eax\n\t"
				 "leal (%0), %%ebx\n\t"
				 "movzx %1, %%ecx\n\t"
				 "int $0x81\n"
				 : : "b"(x), "c"(c)
				);
}


