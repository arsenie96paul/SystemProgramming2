#include <string.h>
#include <console.h>
#include <hal.h>
#include <exception.h>
#include "physicalmemorymanager.h"
#include "virtualmemorymanager.h"
#include "bootinfo.h"
#include <keyboard.h>
#include <sysapi.h>
#include <user.h>
#include <vgamodes.h>
#include <pixels.h> 

// Start PIXEL
uint8_t *video = (uint8_t*)0xA0000; 

// Resolution
int width = 320;
int height = 200;

// Clean Screen Video 
void CleanScreen(const uint8_t _colour)
{
	//uint8_t colour = 0;	// Colour = black
	
	for ( int i = 0; i <height; i++) 
		for ( int j = 0; j<width; j++)
		{
			*(video + i*width +j )= _colour;
		}
}

//Set PIXEL
void Set_Pixel(int x, int y, uint8_t _colour)
{
	*(video + y*width +x )= _colour;
}

// Draw Unfilled Rectangle
void Unfilled_Rectangle( int *parameters, uint8_t _colour)
{
	int TLx = parameters[0];
	int TLy = parameters[1];
	int w = parameters[2];
	int h = parameters[3];

	// Bottom Right Corner
	int BRx = TLx + w;
	int BRy = TLy + h;
	
	// From Top-Left to Botoom-Right
	for ( int i = TLx; i <= BRx; i++)
		for (int j = TLy; j< BRy; j ++)
		{
			// Set pixel in mirror for each side
			Set_Pixel(TLx,j,_colour); // Left Vertical
			Set_Pixel(BRx,j,_colour); // Right Vertical
			
			Set_Pixel(i,TLy,_colour); // Top Horizontal
			Set_Pixel(i,BRy,_colour); // Botoom Horizontal
		}
}

// Draw Filled Rectangle
void Filled_Rectangle( int *parameters, uint8_t _colour)
{
	int TLx = parameters[0];
	int TLy = parameters[1];
	int w = parameters[2];
	int h = parameters[3];

	// Bottom Right Corner
	int BRx = TLx + w;
	int BRy = TLy + h;
	
	// From Top-Left to Botoom-Right 
	for ( int i = TLx; i < BRx; i++)
		for (int j = TLy; j< BRy; j ++)
		{
			Set_Pixel(i,j,_colour);
		}
}

// Draw Unfilled Triangle
void Unfilled_Triangle ( int *parameters, uint8_t _colour )
{
	int Lx = parameters[0];
	int By = parameters[1]; 
	int radius = parameters[2];

	// Coordinates
	int Rx = Lx + radius;		// Right x
	int Mx = Lx + radius/2;		// Middle x
	int Ty  = By - radius/2;	// Top y

	// 2 Up lines
	for ( int i = Ty ; i < By; i++)
	{
		int temp = i - Ty;
		Set_Pixel(Mx-temp,i,_colour);
		Set_Pixel(Mx+temp,i,_colour);
	}

	// Bottom line
	for (int i = Lx ; i <= Rx; i ++)
		Set_Pixel (i,By,_colour);
}


// Draw Filled Triangle
void Filled_Triangle ( int *parameters, uint8_t _colour )
{
	int Lx = parameters[0];
	int By = parameters[1]; 
	int radius = parameters[2];

	// Coordinates
	int Rx = Lx + radius;		// Right x
	int Mx = Lx + radius/2;		// Middle x
	int Ty  = By - radius/2;	// Top y

	// From top to bottom
	for ( int i = Ty ; i <= By; i++)
	{
		int temp = i - Ty;
		for (int j = 0 ; j < temp  ; j ++)
		{
			Set_Pixel(Mx-j,i,_colour);
			Set_Pixel(Mx+j,i,_colour);
		}
	}
}

// Bresenham algorithm
void Draw_Line(int *parameters, uint8_t _colour, int line_tipe)
{
	int x0 = parameters[0];
	int y0 = parameters[1];
	int x1 = parameters[2];
	int y1 = parameters[3];

	int inc1;
	int inc2;
	int e;

	int dx = x1 - x0;
	int dy = y1 - y0;

	// abs(dx) && abs(dy)
	if(dx < 0) dx = -dx;
	if(dy < 0) dy = -dy;

	int xPlus;
	int yPlus;
 
	// check points position ( to increment or decrement )
	if(x1 < x0) 
	{
		xPlus = -1;
	}
	else xPlus = 1;

	if(y1 < y0) 
	{
		yPlus = -1;
	}
	else yPlus = 1;

	int x=x0;
	int y=y0;

	if(dx > dy)
	{
		e = 2 * dy - dx;

		inc1 = 2 * (dy -dx);
		inc2 = 2 * dy;

		for(int i = 0; i < dx; i++)
		{
			if(e >= 0)
			{
				y += yPlus;
				e += inc1;
			}
			else e += inc2;
			
			x = x + xPlus;

			// normal line
			if (line_tipe == 0) 
			{
				Set_Pixel(x,y, _colour);
			}
			
			// doted line
			else if (line_tipe == 1) 
			{
				if(x%2 == 0)
				{
					Set_Pixel(x,y, _colour);
				}
			}

			// dashed line
			else if (line_tipe == 2) 
			{
				if(x%5 != 0)
				{
					Set_Pixel(x,y, _colour);
				}
			}
		}
	}
	else
	{
		e = 2 * dx - dy;

		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;

		for(int i = 0; i < dy; i++)
		{
			if(e >= 0)
			{
				x += xPlus;
				e += inc1;
			}
			else e += inc2;

			y = y + yPlus;

			// normal line
			if (line_tipe == 0) 
			{
				Set_Pixel(x,y, _colour);
			}
			
			// doted line
			else if (line_tipe == 1) 
			{
				if(x%2 == 0)
				{
					Set_Pixel(x,y, _colour);
				}
			}

			// dashed line
	        else if (line_tipe == 2) 
			{
				if(x%5 != 0)
				{
					Set_Pixel(x,y, _colour);
				}
			}
		}
	}
}

// Draw Unfilled Circle
void Unfilled_Circle (int *parameters, uint8_t _colour)
{
	int xC = parameters[0];
	int yC = parameters[1];
	int radius = parameters[2];

	int err = 2-2*radius; 
	int max = -radius;
	int min = 0; 
   
    while (max < 0)
	{
		int temp1;
		int temp2;
		radius = err;
		
		// Set pixel in mirror for each side
		Set_Pixel (xC+min, yC+max,_colour); // Right top corner
		Set_Pixel (xC-max, yC+min,_colour); // Right down corner
		
		Set_Pixel (xC-min, yC-max,_colour); // Left down corner
		Set_Pixel (xC+max, yC-min,_colour); // Left top corner

		if (radius >  max) 
		{
			max++;
			temp1 = max * 2 + 1; 
			err += temp1;
		}
		
		if (radius <= min) 
		{
			min++;			
			temp2 = min * 2 + 1; 
			err += temp2;
		}
    }
}

// Draw Filled Circle
void Filled_Circle (int *parameters, uint8_t _colour)
{
	int xC = parameters[0];
	int yC = parameters[1];
	int radius = parameters[2];

	int min = -radius;
	
	for(int y=min; y<=radius; y++)
		for(int x=min; x<=radius; x++)
		{
			if(x*x+y*y <= radius*radius)
			{
				Set_Pixel(xC+x, yC+y, _colour);
			}
		}
}

// Draw Unfilled Hexagon
void Draw_Unfilled_Hex(int *parameters, uint8_t _colour) 
{	
	int TLx = parameters[0]; 
	int TLy = parameters[1];
	int w = parameters[2];
	int h = parameters[3];

	// Bottom Right Corner
	int BRx = TLx + w;
	int BRy = TLy + h;
		
	// From Top-Left to Botoom-Right: Create body ( Rectangle )
	for ( int i = TLx; i <= BRx; i++)
		for (int j = TLy; j< BRy; j ++)
		{
			// Set pixel in mirror for each side
			Set_Pixel(TLx,j,_colour); // Left Vertical
			Set_Pixel(BRx,j,_colour); // Right Vertical
		}
			
	// Extend ( Create triangles up and down )
	int extended = w/2;			// extended height
	int eY = TLy - extended;	// For top
	int eY2 = BRy + extended;	// For bottom
	int eX = TLx + w/2;
		
	// Upper triangle
	for ( int i = eY ; i < TLy; i++)
	{
		int temp = i - eY;
		Set_Pixel(eX-temp,i,_colour);
		Set_Pixel(eX+temp,i,_colour);
	}

	// Bottom triangle
	for ( int i = BRy ; i <= eY2; i++)
	{
		int temp = i - eY2;
		Set_Pixel(eX-temp,i,_colour);
		Set_Pixel(eX+temp,i,_colour);
	}
}

// Draw Filled Hexagon
void Draw_Filled_Hex(int*parameters, uint8_t _colour) 
{	
	int TLx = parameters[0]; 
	int TLy = parameters[1];
	int w = parameters[2];
	int h = parameters[3];

	// Bottom Right Corner
	int BRx = TLx + w;
	int BRy = TLy + h;
		
	// From Top-Left to Botoom-Right: Create body ( Rectangle )
	for ( int i = TLx+1; i < BRx; i++)
		for (int j = TLy; j< BRy; j ++)
		{
			Set_Pixel(i,j,_colour);
		}
		
	// Extend ( Create triangles up and down )
	int extended = w/2;			// Extended height
	int eY = TLy - extended;	// For top
	int eY2 = BRy + extended;	// For bottom
	int eX = TLx + w/2;			
	
	// Upper triangle
	for ( int i = eY ; i <= TLy; i++)
	{
		int temp = i - eY;

		for (int j = 0 ; j < temp  ; j ++)
		{
			Set_Pixel(eX-j,i,_colour);
			Set_Pixel(eX+j,i,_colour);
		}
	}
	// Bottom triangle
	for ( int i = eY2 ; i >= BRy; i--)
	{
		int temp = eY2-i ;
		
		for (int j = 0 ; j < temp  ; j ++)
		{
			Set_Pixel(eX-j,i,_colour);
			Set_Pixel(eX+j,i,_colour);
		}
	}
}