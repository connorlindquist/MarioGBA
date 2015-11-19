//Connor Lindquist
#include "myLib.h"
#include "font.h"
#include "text.h"
u16 *videoBuffer = (u16 *) 0x6000000;

u16 __key_curr = 0, __key_prev = 0;

// A function to set pixel (r, c) to the color passed in
void setPixel(int r, int c, u16 color) {
	videoBuffer[OFFSET(r, c, 240)] = color;
}

//Draws rectangle at r,c
void drawRect(int r, int c, int width, int height, u16 color) {
	// we're basically looping through pixel by pixel
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			setPixel(r + i, c + j, color);
		}
	}
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

//Draws image with DMA
void drawImage3(int r, int c, int width, int height, const u16* image) {
	for (int x = 0; x < height; x++) {
		DMA[3].src = &image[OFFSET(x, 0, width)];
		DMA[3].dst = &videoBuffer[OFFSET(r + x, c, 240)];
		DMA[3].cnt = (width) | DMA_ON;
	}
}

//Draws rectangle with DMA
void drawRect3(int r, int c, int width, int height, u16 color) {
	for (int x = 0; x < height; x++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(r + x, c, 240)];
		DMA[3].cnt = (width) | DMA_SOURCE_FIXED | DMA_ON;
	}
}

//Draws char on screen
void drawChar(int row, int col, char ch, u16 color){
	int r,c;
	for(r=0; r<8; r++){
		for(c=0; c<6; c++){
			if(fontdata_6x8[OFFSET(r,c,6) + 48*ch]){
				setPixel(r + row, c + col, color);
			}	
		}
	}
}

//Draws string on screen
void drawString(int row, int col, char *str, u16 color){
	int chars_drawn = 0;
	while(*str){
		drawChar(row, col + 6*chars_drawn++, *str++, color);
	}
}

void draw_image_portion(int destination_row, int destination_col, int source_row, int source_col, int source_width, int source_height, const u16* image, int image_width) {
	for (int x = 0; x < source_height; x++) {
		DMA[3].src = &image[OFFSET(source_row + x, source_col, image_width)];
		DMA[3].dst = &videoBuffer[OFFSET(destination_row + x, destination_col, image_width)];
		DMA[3].cnt = (source_width) | DMA_ON;
	}
}
