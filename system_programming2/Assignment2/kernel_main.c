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
#include <umode.h>

// This is a dummy __main.  For some reason, gcc puts in a call to 
// __main from main, so we just include a dummy.
 
BootInfo *	_bootInfo;
 
void __main() {}

void InitialisePhysicalMemory()
{
	// Initialise the physical memory manager
	// We place the memory bit map in the next available block of memory after the kernel.
	// Note that all memory addresses here are physical memory addresses since we are dealing
	// with management of physical memory

	uint32_t memoryMapAddress = 0x100000 + _bootInfo->KernelSize;
	if (memoryMapAddress % PMM_GetBlockSize() != 0)
	{
		// Make sure that the memory map is going to be aligned on a block boundary
		memoryMapAddress = (memoryMapAddress / PMM_GetBlockSize() + 1) * PMM_GetBlockSize();
	}
	uint32_t sizeOfMemoryMap = PMM_Initialise(_bootInfo, memoryMapAddress);

	// We now need to mark various regions as unavailable
	
	// Mark first page as unavailable (so that a null pointer is invalid)
	PMM_MarkRegionAsUnavailable(0x00, PMM_GetBlockSize());

	// Mark memory used by kernel as unavailable
	PMM_MarkRegionAsUnavailable(0x100000, _bootInfo->KernelSize);

	// Mark memory used by memory map as unavailable
	PMM_MarkRegionAsUnavailable(PMM_GetMemoryMap(), sizeOfMemoryMap);

	// Reserve two blocks for the stack and make unavailable (the stack is set at 0x90000 in boot loader)
	uint32_t stackSize = PMM_GetBlockSize() * 2;
	PMM_MarkRegionAsUnavailable(_bootInfo->StackTop - stackSize, stackSize);
	
	// Reserve two block for the kernel stack and make unavailable 
	PMM_MarkRegionAsUnavailable(0x80000 - stackSize, stackSize);
}
void Initialise()
{
	ConsoleClearScreen(0x1F); 
	ConsoleWriteString("UODOS 32-bit Kernel. Kernel size is ");
	ConsoleWriteInt(_bootInfo->KernelSize, 10);
	ConsoleWriteString(" bytes\n");
	HAL_Initialise();
	InitialisePhysicalMemory();
	VMM_Initialise();
	KeyboardInstall(33); 
	//InitialiseSysCalls();
	InitialiseUModes();
}

// Ring 0 or 3
void SelectRing(int ring)
{
	// if ring = 0 then move to vga mode, clean the screen and draw all my shapes
	if ( ring == 0)
	{
		VGA_SetGraphicsMode(320, 200, 1);	// Switch to Graphics Mode
		
		// Parameters
		int URect[] = {180,20,50,30};
		int FRect[] = {250,20,50,24};
		int FTrinagle[] = {120, 150, 50};
		int UTriangle[] = {120, 100, 50};
		int Line[] = {5,5,50,15};
		int Line2[] = {50,20, 10,50};
		int Line3[] = { 70, 70, 10, 80};
		int UCircle[] = {290,80, 20};
		int FCircle[] = {290,130,20};
		int FHex[] = {200,140,50,25};
		int UHex[] = {200,80,30,15};

		// Call Functions
		CleanScreen(0);
		Set_Pixel(100,7,15);	
		Draw_Line(Line,9,0);// case 0 - normal line
		Draw_Line(Line2,3,1);// case 1 - dotted line
		Draw_Line(Line3,2,2);// case 2 - dashed line
		
		// Filled Shapes
		Filled_Rectangle( FRect,5);
		Filled_Triangle(FTrinagle, 4);
		Filled_Circle(FCircle,11);
		Draw_Filled_Hex(FHex,3);

		// Unfilled Shapes
		Unfilled_Rectangle( URect, 1);
		Unfilled_Triangle(UTriangle, 3); 
		Unfilled_Circle(UCircle,2);
		Draw_Unfilled_Hex(UHex,10);
	}
	// if ring = 3, turn on the user mode, turn on the vga mode and draw my shapes
	if (ring == 3)
	{
		HAL_EnterUserMode();					// Turn on User mode
		VGA_SetGraphicsMode(320, 200, 1);		// Switch to Graphics Mode
	
		// Parameters
		int URect[] = {180,20,50,30};
		int FRect[] = {250,20,50,24};
		int FTrinagle[] = {120, 150, 50};
		int UTriangle[] = {120, 100, 50};
		int Line[] = {5,5,50,15};
		int Line2[] = {55,20, 10,50};
		int Line3[] = { 55, 30, 10, 80};
 		int UCircle[] = {290,80, 20};
		int FCircle[] = {290,130,20};
		int FHex[] = {200,140,50,25};
		int UHex[] = {200,80,30,15};
	
		// Call Functions
		User_CleanScreen(0);
		User_Set_Pixel(100,7,15);	
		User_Draw_Line(Line,9,0); // case 0 - normal line
		User_Draw_Line(Line2,7,1);// case 1 - dot line
		User_Draw_Line(Line3,2,2);// case 2 - dash line
		
		// Filled Shapes
		User_Filled_Rectangle( FRect,1);
		User_Filled_Triangle(FTrinagle, 2);
		User_Filled_Circle(FCircle,7);
		User_Draw_Filled_Hex(FHex,11);
	
		// Unfilled Shapes
		User_Unfilled_Rectangle( URect, 4);
		User_Unfilled_Triangle(UTriangle, 5); 
		User_Unfilled_Circle(UCircle,8);
		User_Draw_Unfilled_Hex(UHex,10);
	}
	// if not ring 0 or 3, display the message
	else 
	{
		ConsoleWriteString("This is not a valid ring! Please call ring 0 or 3!");
	}
}

// Main program
void main(BootInfo * bootInfo) 
{
	_bootInfo = bootInfo;
	Initialise();
	SelectRing(3);			//It can run in ring 0 or 3, for another call it will display a text message

	while(1);
}
