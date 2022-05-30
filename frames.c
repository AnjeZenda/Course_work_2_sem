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

    if(!setParams(parametrs, ind, color)){
        free(parametrs);
        return;
    }
    if(!isNum(width)) {
        free(parametrs);
        puts("Wrong width");
        return;
    }

    int i_width = atoi(width);

    if(i_width * 2 > image->height || i_width * 2 > image->width){
        free(parametrs);
        puts("Too big width");
        return;
    }

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
    char colours[10][20] = {"red", "green", "blue", "gray", "yellow", "purple", "white", "black", "cyan", "orange"};
    int ind = -1;
    for(int i = 0; i < 10; i++)
        if(!strcmp(color, colours[i])) ind = i;
    int *parametrs = (int *)calloc(4, sizeof(int));
    
    if(!setParams(parametrs, ind, color)){
        free(parametrs);
        return;
    }
    if(!isNum(width)) {
        free(parametrs);
        puts("Wrong width");
        return;
    }
    
    
    int i_width = atoi(width);

    if(i_width * 2 > image->height || i_width * 2 > image->width){
        free(parametrs);
        puts("Too big width");
        return;
    }

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
    char colours[10][20] = {"red", "green", "blue", "gray", "yellow", "purple", "white", "black", "cyan", "orange"};
    int ind = -1;
    for(int i = 0; i < 10; i++)
        if(!strcmp(color, colours[i])) ind = i;
    int *parametrs = (int *)calloc(4, sizeof(int));
    
    if(!setParams(parametrs, ind, color)){
        free(parametrs);
        return;
    }
    if(!isNum(width)) {
        free(parametrs);
        puts("Wrong width");
        return;
    }
    int i_width = atoi(width);

    if(i_width * 2 > image->height || i_width * 2 > image->width){
        free(parametrs);
        puts("Too big width");
        return;
    }

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

void BresenhamAlgorithm(sPng *image, int x1, int y1, int x2, int y2, int *params, int indicator){
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;

    png_byte *ptry = image->row_pointers[y2];
    png_byte *ptrx = &(ptry[x2*indicator]);
    
    for(int i = 0; i < indicator; i++) 
        ptrx[i] = params[i];
    
    while (x1 != x2 || y1 != y2){
        png_byte *row = image->row_pointers[y1];
        png_byte *ptr = &(row[x1*indicator]);
    
        for(int i = 0; i < indicator; i++) 
            ptr[i] = params[i];
        int error2 = error * 2;
        if(error2 > -deltaY){
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX){
            error += deltaX;
            y1 += signY;
        }
    }
    
}

void drawTunnel(sPng *image, char *color, char *width, int indicator){
    char colours[10][20] = {"red", "green", "blue", "gray", "yellow", "purple", "white", "black", "cyan", "orange"};
    int ind = -1;
    for(int i = 0; i < 10; i++)
        if(!strcmp(color, colours[i])) ind = i;
    int *parametrs = (int *)calloc(4, sizeof(int));
    if(!setParams(parametrs, ind, color)){
        free(parametrs);
        return;
    }

    if(!isNum(width)){
        free(parametrs);
        puts("Wrong width");
        return;
    }
    int i_width = atoi(width);


    if(image->height < 2 * i_width || image->width < 2 * i_width){
        free(parametrs);
        puts("Too big width");
        return;
    }
    int small_x = image->width - 2 * i_width;
    int small_y = image->height - 2 * i_width;

    int line_width = (image->width/50) ? (image->width/50) : 2;
    int space_width = (image->width/200) ? (image->width/200) : 2;

    for(int y1 = 0; y1 < image->height; ++y1){
        if(y1 % line_width >= space_width){
            int y2 = (int)(small_y * (double)y1/image->height);
            BresenhamAlgorithm(image, 0, y1, i_width - 1, y2 + i_width, parametrs, indicator);
            BresenhamAlgorithm(image, image->width - 1, y1, image->width - i_width, y2 + i_width, parametrs, indicator);

        }
    }

    for(int x1 = 0; x1 < image->width; ++x1){
        if(x1 % line_width > space_width){
            int x2 = (int)(small_x * (double)x1/image->width);
            BresenhamAlgorithm(image, x1, 0, x2 + i_width, i_width - 1, parametrs, indicator);
            BresenhamAlgorithm(image, x1, image->height - 1, x2 + i_width, image->height - i_width, parametrs, indicator);
        }
    }
    free(parametrs);
}

void makeFramePNG(sPng *image, char *frame_type, char *width, char *color){
    int color_indicator = checkColorType(image);
    if(color_indicator == -1){
        puts("Wrong color indicator expected RGB or RGBA");
        return;
    }
    char frames[4][20] = {"common", "fractal", "tunnel", "chess"};
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
            drawTunnel(image, color, width, color_indicator);
            break;
        case 3:
            drawChess(image, color, width, color_indicator);
            break;
        default:
            break;
    }
}

void makeFrameGet(int argc, char **argv, int *opt_index, sPng *image, char *filename){
    const char *opts = "t:c:w:?";
    struct option lOpt[] = {
        {"type", required_argument, NULL, 't'},
        {"color", required_argument, NULL, 'c'},
        {"width", required_argument, NULL, 'w'},
        {NULL, 0, NULL, 0}
    };
    char *frame_type = NULL, *width = NULL, *color = NULL;
    int opt;
    while (-1 != (opt = getopt_long(argc, argv, opts, lOpt, opt_index)))
    {
        switch(opt){
            case 't':
                frame_type = optarg;
                break;
            case 'c':
                color = optarg;
                break;
            case 'w':
                width = optarg;
                break;
            case '?':
            default:
                printHelp(4);
                writePNG(filename, image);
                exit(0);
                break;
        }
    }
    if(frame_type == NULL || color == NULL || width == NULL){
        puts("You did not choose one or more options of frames command");
        printHelp(5);
        writePNG(filename, image);
        exit(0);
    }
    makeFramePNG(image, frame_type, width, color);
    if(!writePNG(filename, image)){
        exit(0);
    }   
}

