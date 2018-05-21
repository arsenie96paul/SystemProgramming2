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
void Unfilled_Rectangle( int TLx, int TLy, int w, int h, uint8_t _colour)
{
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
void Filled_Rectangle( int TLx, int TLy, int w, int h, uint8_t _colour)
{
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

// Draw Filled Triangle
void Filled_Triangle ( int Lx, int By, int radius, uint8_t _colour )
{
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
void Draw_Line(int x0,int y0,int x1,int y1, uint8_t _colour, int line_tipe)
{
	int inc1;
	int inc2;
	int e;

	int dx = x1 - x0;
	int dy = y1 - y0;

	// abs(dx) && abs(dy)
	if(dx < 0) dx = -dx;
	if(dy < 0) dy = -dy;

	int incx = 1;
	int incy = 1;

	// check points position
	if(x1 < x0) incx = -1;
	if(y1 < y0) incy = -1;

	int x=x0;
	int y=y0;

	if(dx > dy)
	{
		e = 2*dy - dx;

		inc1 = 2*( dy -dx);
		inc2 = 2*dy;

		for(int i = 0; i < dx; i++)
		{
			if(e >= 0)
			{
				y += incy;
				e += inc1;
			}
			else e += inc2;
			
			x = x + incx;

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
		e = 2*dx - dy;

		inc1 = 2*( dx - dy);
		inc2 = 2*dx;

		for(int i = 0; i < dy; i++)
		{
			if(e >= 0)
			{
				x += incx;
				e += inc1;
			}
			else e += inc2;

			y = y + incy;

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
void Unfilled_Circle (int xC, int yC, int radius, uint8_t _colour)
{
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
void Filled_Circle ( int xC, int yC, int radius, uint8_t _colour)
{
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
void Draw_Unfilled_Hex(int TLx, int TLy , int w, int h, uint8_t _colour) 
{	
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
void Draw_Filled_Hex(int TLx, int TLy , int w, int h, uint8_t _colour) 
{	
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

// Draw Unfilled Triangle
void Unfilled_Triangle ( int Lx, int By, int radius, uint8_t _colour )
{
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



// // Draw a line 
// void Draw_Line(int x1, int y1, int x2, int y2, uint8_t _colour, int tipe )
// {
// 	// same y ( horizontal line)
// 	if ( y1 == y2)
// 	{
// 		// Check start point
// 		if ( x1 < x2)
// 		{
// 			for ( int i = x1; i <=x2;i++)
// 			{
// 				// Check if straight, dot or dash
// 				if ( tipe == 0) 
// 					Set_Pixel(i, y1, _colour);
// 				if ( tipe == 1) 
// 					if ( i%2==0 )
// 						Set_Pixel(i, y1, _colour);
// 				if ( tipe == 2) 
// 					if ( i%3 != 0 )
// 						Set_Pixel(i, y1, _colour);
// 			}
// 		}
// 		else if ( x2 < x1)
// 		{
// 			for ( int i = x2; i <=x1;i++)
// 			{
// 				// Check if straight, dot or dash
// 				if ( tipe == 0) 
// 					Set_Pixel(i, y1, _colour);
// 				if ( tipe == 1) 
// 					if ( i%2==0 )
// 						Set_Pixel(i, y1, _colour);
// 				if ( tipe == 2) 
// 					if ( i%3 != 0 )
// 						Set_Pixel(i, y1, _colour);
// 			}
// 		}
// 	}

// 	// same x ( vertical line )
// 	if ( x1 == x2)
// 	{
// 		// Check start point
// 		if (y1 < y2)
// 		{
// 			for ( int i = y1; i <=y2;i++)
// 			{
// 				// Check if straight, dot or dash
// 				if ( tipe == 0) 
// 					Set_Pixel(x1, i, _colour);
// 				if ( tipe == 1) 
// 					if ( i%2==0 )
// 					Set_Pixel(x1, i, _colour);
// 				if ( tipe == 2) 
// 					if ( i%3 != 0 )
// 						Set_Pixel(x1, i, _colour);
// 			}
// 		}
// 		else if ( y2 < y1)
// 		{
// 			for ( int i = y2; i <=y1;i++)
// 			{
// 				// Check if straight, dot or dash
// 				if ( tipe == 0) 
// 					Set_Pixel(x1, i, _colour);
// 				if ( tipe == 1) 
// 					if ( i%2==0 )
// 					Set_Pixel(x1, i, _colour);
// 				if ( tipe == 2) 
// 					if ( i%3 != 0 )
// 						Set_Pixel(x1, i, _colour);
// 			}
// 		}
// 	}
	
// 	// Check same point
// 	if (x1 == x2 && y1 == y2)
// 	{
// 		Set_Pixel (x1,y1, _colour);
// 	}

// 	// Coordinates different (Bresenham)
// 	if (x1 != x2 && y1 != y2)
// 	{
// 		int dx;
// 		int dy;
		
// 		// abs (dx)
// 		if ( x2 > x1) dx = x2 - x1;
// 		else dx = x1 - x2;

// 		// abs(dy)
// 		if (y2 > y1) dy = y2 - y1;
// 		else dy= y1 - y2;

// 		int x = x1;
// 		int y = y1;
	
// 		int temp = 2 * dy - dx;
// 		int end;
	
// 		while(x < x2)
// 		{
// 			if(temp < 0)
// 			{
// 				temp = temp + 2 * dy;
// 			}
// 			else
// 			{
// 				y = y + 1;
// 				temp = temp + 2 * (dy - dx);
// 			}

// 			// Check if straight, dot or dash
// 			if ( tipe == 0) 
// 				Set_Pixel(x, y, _colour);
// 			if ( tipe == 1) 
// 				if ( x%2==0 )
// 					Set_Pixel(x, y, _colour);
// 			if ( tipe == 2) 
// 				if ( x%3 != 0 )
// 					Set_Pixel(x, y, _colour);

// 			x++;
// 		}
// 	}
// }