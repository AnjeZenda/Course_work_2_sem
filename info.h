#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <png.h>

typedef struct Png{
    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep *row_pointers;
}sPng;

typedef struct Rect{
    int x, y, height, width;
}Rect;

/*----------HELPING FUNCTIONS----------*/

void printHelp(int arg);
int isNum(char *str);
void setParams(int *parametrs, int ind, char *color);
int checkColorType(sPng *image);

/*----------HELPING FUNCTIONS----------*/

/*----------PNG FUNCTIONS----------*/

void readPNG(char *filename, sPng *image);
void writePNG(char *filename, sPng *image);
void copyAreaPNG(sPng *image, char *left_up, char *right_down, char *destination);
void transformPNG(sPng *image, char *color1, char *color2);
void makeFramePNG(sPng *image, char *frame_type, char *width, char *color);
void rectsPNG(sPng *image, char *rect_color, char *frame_color, char *width);

/*----------PNG FUNCTIONS----------*/

/*----------GETOPT FUNCTIONS----------*/

void copyAreaGet(int argc, char **argv, int *opt_index, sPng *image, char *filename);
void transformGet(int argc, char **argv, int *opt_index, sPng *image, char *filename);
void makeFrameGet(int argc, char **argv, int *opt_index, sPng *image, char *filename);
void rectsGet(int argc, char **argv, int *opt_index, sPng *image, char *filename);

/*----------GETOPT FUNCTIONS----------*/