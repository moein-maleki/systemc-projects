#ifndef MACROS_HH
#define MACROS_HH

#define IMAGE_COLS_PX				512
#define IMAGE_ROWS_PX				512
#define IMAGE_SIZE					(IMAGE_ROWS_PX * IMAGE_COLS_PX)

#define RED							0
#define GREEN						1
#define BLUE						2
#define MAX_NO_COLORS				3

#define RED_CHANNEL_FILE_NAME		"red_pixel.txt"
#define GREEN_CHANNEL_FILE_NAME		"green_pixel.txt"
#define BLUE_CHANNEL_FILE_NAME		"blue_pixel.txt"

#define CLOCK_ACTIVE_DUR_NS			2

#define SEGMENT_ROWS_PX				8
#define SEGMENT_SIZE				(SEGMENT_ROWS_PX * IMAGE_COLS_PX)
#define NUM_OF_SEGMENTS				(IMAGE_ROWS_PX / SEGMENT_ROWS_PX)

#define GRAYSCALE_FILE_NAME			"grayScale_value.txt"
#define FINAL_IMAGE_FILE_NAME		"finalImage_value.txt"
#endif