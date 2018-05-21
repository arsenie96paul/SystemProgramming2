
// Funtions used to maniplate pixels, in order to clean screan or draw shapes
void CleanScreen(uint8_t _colour);
void Set_Pixel(int x, int y, uint8_t _colour);
void Unfilled_Rectangle( int *parameters, uint8_t _colour);
void Filled_Rectangle( int *parameters, uint8_t _colour);
void Draw_Line(int *parameters, uint8_t _colour, int line_tipe);
void Unfilled_Circle (int *parameters, uint8_t _colour);
void Filled_Circle ( int *parameters, uint8_t _colour);
void Draw_Unfilled_Hex(int*parameters, uint8_t _colour);
void Draw_Filled_Hex(int*parameters, uint8_t _colour); 
void Unfilled_Triangle ( int *parameters, uint8_t _colour );
void Filled_Triangle ( int *parameters, uint8_t _colour );


