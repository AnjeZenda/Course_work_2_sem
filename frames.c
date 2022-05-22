#include "info.h"

int findMaxDegree(int num){
    if(num < 1){
        return 0;
    }
    int x = 1;
    while(x <= num){
        x *= 3;
    }
    return x / 3;
}

void fillPixel(png_bytep *square, int x, int width, int *params, int indicator){
    for(int i = 0; i < width; i++){
        png_byte *row = square[i];
        for(int j = x; j < x + width; j++){
            png_byte *ptr = &(row[j * indicator]);
            ptr[0] = params[0];
            ptr[1] = params[1];
            ptr[2] = params[2];
            if(indicator == 4) ptr[3] = params[3];
        }
    }
}

void fillSquares(png_bytep *square, int gx, int width, int *params, int indicator){
    if (width == 1)
        return;
    int square_width = width / 3;
    for (int y = 0; y < width; y += square_width) {
        for (int x = 0; x < width; x += square_width) { 
            if (x == y && x == square_width) {
                fillPixel(square + y, gx + x, square_width, params, indicator);
            } else { 
                fillSquares(square + y, gx + x, square_width, params, indicator);
            }
        }
    }
}

png_bytep *drawSquare(int width, struct Png *image, int *params, int indicator){
    int maxDeg = findMaxDegree(width);

    png_bytep *buf = (png_bytep *)calloc(maxDeg, sizeof(png_bytep));
    for(int i = 0; i < maxDeg; i++)
        buf[i] = (png_byte *)calloc(1, png_get_rowbytes(image->png_ptr, image->info_ptr));

    fillSquares(buf, 0, maxDeg, params, indicator);
    return buf;
}

void drawFractal(struct Png *image, char *color, char *width, int indicator){
    char colours[10][20] = {"red", "green", "blue", "gray", "yellow", "purple", "white", "black", "cyan", "orange"};
    int ind = -1;
    for(int i = 0; i < 10; i++)
        if(!strcmp(color, colours[i])) ind = i;
    int *parametrs = (int *)calloc(4, sizeof(int));
    char *c = (char *)calloc(strlen(color) + 1, sizeof(char));
    strcpy(c, color);
    setParams(parametrs, ind, c);
    free(c);
    printf("%d %d %d %d\n", parametrs[0], parametrs[1], parametrs[2], parametrs[3]);
    if(!isNum(width)) return;
    int i_width = atoi(width);

    int maxDeg = findMaxDegree(i_width);    
    if(maxDeg == 0){
        free(parametrs);
        return;
    }
    png_bytep *square = drawSquare(i_width, image, parametrs, indicator);
    for (int i = 0; i < image->height; i++){
        png_byte *row = image->row_pointers[i];
        png_byte *sq_row = square[i % maxDeg];
        for(int j = 0; j < image->width; j++){
            png_byte *ptr = &(row[j*indicator]);
            png_byte *sq_ptr = &(sq_row[(j % maxDeg) * indicator]);
            if(i < i_width){
                ptr[0] = sq_ptr[0];
                ptr[1] = sq_ptr[1];
                ptr[2] = sq_ptr[2];
                if(indicator == 4) ptr[3] = sq_ptr[3];
            }
            else if(i >= i_width && i < image->height - i_width && (j < i_width || j >= image->width - i_width)){
                ptr[0] = sq_ptr[0];
                ptr[1] = sq_ptr[1];
                ptr[2] = sq_ptr[2];
                if(indicator == 4) ptr[3] = sq_ptr[3];
            }
            else if(i >= image->height - i_width){
                ptr[0] = sq_ptr[0];
                ptr[1] = sq_ptr[1];
                ptr[2] = sq_ptr[2];
                if(indicator == 4) ptr[3] = sq_ptr[3];
            }
        }
    }
    

    free(square);
    free(parametrs);
}


void drawCommon(sPng *image, char *color, char *width, int indicator){
    char colours[10][20] = {"red", "green", "blue", "gray", "yellow", "purple", "white", "black", "brown", "orange"};
    int ind = -1;
    for(int i = 0; i < 10; i++)
        if(!strcmp(color, colours[i])) ind = i;
    int *parametrs = (int *)calloc(4, sizeof(int));
    setParams(parametrs, ind, color);
    if(!isNum(width)) return;
    
    int i_width = atoi(width);
    int y;
    printf("%d %d %d\n", parametrs[0], parametrs[1], parametrs[2]);
    for(y = 0; y < i_width; y++){
        png_byte *row = image->row_pointers[y];
        for(int j = 0; j < image->width; j++){
            png_byte *ptr = &(row[j * indicator]);
            ptr[0] = parametrs[0];
            ptr[1] = parametrs[1];
            ptr[2] = parametrs[2];
            if(indicator == 4) ptr[3] = parametrs[3];
        }
    }
    for(y; y < image->height - i_width; y++){
        png_byte *row = image->row_pointers[y];
        for(int j = 0; j < i_width; j++){
            png_byte *ptr = &(row[j * indicator]);
            ptr[0] = parametrs[0];
            ptr[1] = parametrs[1];
            ptr[2] = parametrs[2];
            if(indicator == 4) ptr[3] = parametrs[3];
        }
        for(int j = image->width - i_width; j < image->width; j++){
            png_byte *ptr = &(row[j * indicator]);
            ptr[0] = parametrs[0];
            ptr[1] = parametrs[1];
            ptr[2] = parametrs[2];
            if(indicator == 4) ptr[3] = parametrs[3];
        }
    }
    for(y; y < image->height; y++){
        png_byte *row = image->row_pointers[y];
        for(int j = 0; j < image->width; j++){
            png_byte *ptr = &(row[j * indicator]);
            ptr[0] = parametrs[0];
            ptr[1] = parametrs[1];
            ptr[2] = parametrs[2];
            if(indicator == 4) ptr[3] = parametrs[3];
        }
    }
    free(parametrs);
}

void drawChess(sPng *image, char *color, char *width, int indicator){
    char colours[10][20] = {"red", "green", "blue", "gray", "yellow", "purple", "white", "black", "brown", "orange"};
    int ind = -1;
    for(int i = 0; i < 10; i++)
        if(!strcmp(color, colours[i])) ind = i;
    int *parametrs = (int *)calloc(4, sizeof(int));
    setParams(parametrs, ind, color);

    if(!isNum(width)) return;
    int i_width = atoi(width);
    int y;
    for(y = 0; y < i_width; y++){
        png_byte *row = image->row_pointers[y];
        for(int x = 0; x < image->width; x++){
            png_byte *ptr = &(row[indicator*x]);
            if(y % 16 <= 7 && x % 16 <= 7){
                ptr[0] = parametrs[0];
                ptr[1] = parametrs[1];
                ptr[2] = parametrs[2];
                if(indicator == 4) ptr[3] = parametrs[3];
            }
            else if(y % 16 > 7 && x % 16 > 7){
                ptr[0] = parametrs[0];
                ptr[1] = parametrs[1];
                ptr[2] = parametrs[2];
                if(indicator == 4) ptr[3] = parametrs[3];
            }
        }
    }
    for(y; y < image->height - i_width; y++){
        png_byte *row = image->row_pointers[y];
        for(int x = 0; x < image->width; x++){
            png_byte *ptr = &(row[indicator*x]);
            if(x < i_width || x >= image->width - i_width){
                if(y % 16 <= 7 && x % 16 <= 7){
                    ptr[0] = parametrs[0];
                    ptr[1] = parametrs[1];
                    ptr[2] = parametrs[2];
                    if(indicator == 4) ptr[3] = parametrs[3];
                }
                else if(y % 16 > 7 && x % 16 > 7){
                    ptr[0] = parametrs[0];
                    ptr[1] = parametrs[1];
                    ptr[2] = parametrs[2];
                    if(indicator == 4) ptr[3] = parametrs[3];
                }
            }
        }
    }
    for(y; y < image->height; y++){
        png_byte *row = image->row_pointers[y];
        for(int x = 0; x < image->width; x++){
            png_byte *ptr = &(row[indicator*x]);
            if(y % 16 <= 7 && x % 16 <= 7){
                ptr[0] = parametrs[0];
                ptr[1] = parametrs[1];
                ptr[2] = parametrs[2];
                if(indicator == 4) ptr[3] = parametrs[3];
            }
            else if(y % 16 > 7 && x % 16 > 7){
                ptr[0] = parametrs[0];
                ptr[1] = parametrs[1];
                ptr[2] = parametrs[2];
                if(indicator == 4) ptr[3] = parametrs[3];
            }
        }
    }
    free(parametrs);
}



void makeFramePNG(sPng *image, char *frame_type, char *width, char *color){
    int color_indicator = checkColorType(image);
    if(color_indicator == -1) return;
    char frames[4][20] = {"common", "fractal", "lines", "chess"};
    int flag = -1;
    for(int i = 0; i < 4; i++){
        if(!strcmp(frame_type, frames[i])){
            flag = i;
            break;
        }
    }
    switch (flag)
    {
        case -1:
            puts("No such frame");
            break;
        case 0:
            drawCommon(image, color, width, color_indicator);
            break;
        case 1:
            drawFractal(image, color, width, color_indicator);
            break;
        case 2:
            //drawLines(image, color, width, color_indicator);
            puts("While nothing");
            break;
        case 3:
            drawChess(image, color, width, color_indicator);
            break;
        default:
            break;
    }
}

void makeFrameGet(int argc, char **argv, int *opt_index, sPng *image, char *filename){
    readPNG(filename, image);
    const char *opts = "t:c:w:?";
    struct option lOpt[] = {
        {"type", required_argument, NULL, 't'},
        {"color", required_argument, NULL, 'c'},
        {"width", required_argument, NULL, 'w'},
        {NULL, 0, NULL, 0}
    };
    char *frame_type, *width, *color;
    int opt;
    while (-1 != (opt = getopt_long(argc, argv, opts, lOpt, opt_index)))
    {
        switch(opt){
            case 't':
                puts("type");
                frame_type = optarg;
                break;
            case 'c':
                puts("color");
                color = optarg;
                break;
            case 'w':
                puts("width");
                width = optarg;
                break;
            case '?':
            default:
                printHelp(4);
                break;
        }
    }
    makeFramePNG(image, frame_type, width, color);
    writePNG(filename, image);   
}

