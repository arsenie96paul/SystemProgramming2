#ifndef _USER_H
#define _USER_H
#include <keyboard.h>

void User_ConsoleWriteCharacter(unsigned char c); 
void User_ConsoleWriteString(char* str); 
void User_ConsoleWriteInt(unsigned int i, unsigned int base); 
void User_ConsoleClearScreen(const uint8_t c); 
void User_ConsoleGotoXY(unsigned int x, unsigned int y); 

// VGA
void User_CleanScreen(const uint8_t c);
void User_Set_Pixel(unsigned int x, unsigned int y, uint8_t c);
void User_Unfilled_Rectangle(int *x, uint8_t c);
void User_Filled_Rectangle(int *x, uint8_t c);
void User_Filled_Triangle(int *x, uint8_t c);
void User_Unfilled_Triangle(int *x, uint8_t c);
void User_Draw_Line (int *x, uint8_t c, int l);
void User_Unfilled_Circle (int *x, uint8_t c);
void User_Filled_Circle (int *x, uint8_t c);
void User_Draw_Filled_Hex(int*x, uint8_t c);
void User_Draw_Unfilled_Hex(int*x, uint8_t c);

#endif