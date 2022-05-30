#include "info.h"


int isRectColor(sPng *image, int x, int y, int indicator, int *params){
    if(x < 0 || y < 0) return 0;
    if(x >= image->width || y >= image->height) return 0;
    png_byte *row = image->row_pointers[y];
    png_byte *ptr = &(row[x*indicator]);
    for(int i = 0; i < indicator; i++){
        if(ptr[i] != params[i]) return 0;
    }
    return 1;
}

int isRectFill(sPng *image, int width, int height, int indicator, int *params, int x, int y){
    for(int i = y; i < y + height; i++){
        for(int j = x; j < x + width; j++){
            if(!isRectColor(image, j, i, indicator, params)) return 0;
        }
    }
    return 1;
}

int isRectShape(sPng *image, int width, int height, int indicator, int *params, int x, int y){
    for(int j = x; j < x + width - 1; j++){
        if(isRectColor(image, j, y-1, indicator, params)) return 0;
        if(isRectColor(image, j, y+height, indicator, params)) return 0;
    }
    for(int i = y; i < y + height - 1; i++){
        if(isRectColor(image, x - 1, i, indicator, params)) return 0;
        if(isRectColor(image, x + width, i, indicator, params)) return 0;
    }
    return 1;
}

void drawRectFrame(sPng *image, Rect *arr, int *params, int width, int count, int indicator){
    for(int n = 0; n < count; n++){
        int i;
        for(i = arr[n].y - width; i < arr[n].y; i++){
            if(i >= 0){
                png_byte *row = image->row_pointers[i];
                for(int j = arr[n].x - width; j < arr[n].x + arr[n].width + width && j < image->width; j++){
                    if(j >= 0){
                        png_byte *ptr = &(row[j*indicator]);
                        ptr[0] = params[0];
                        ptr[1] = params[1];
                        ptr[2] = params[2];
                        if(indicator == 4) ptr[3] = params[3];
                    }
                }
            }
        }
        for(i; i < arr[n].y + arr[n].height; i++){
            if(i >= 0){
                png_byte *row = image->row_pointers[i];
                for(int j = arr[n].x - width; j < arr[n].x + arr[n].width + width && j < image->width; j++){
                    if(j >= 0 && (j < arr[n].x || j >= arr[n].x + arr[n].width)){
                        png_byte *ptr = &(row[j*indicator]);
                        ptr[0] = params[0];
                        ptr[1] = params[1];
                        ptr[2] = params[2];
                        if(indicator == 4) ptr[3] = params[3];
                    }
                }
            }
        }
        
        for(i; i < arr[n].y + arr[n].height + width && i < image->height; i++){
            if(i >= 0){
                png_byte *row = image->row_pointers[i];
                for(int j = arr[n].x - width; j < arr[n].x + arr[n].width + width && j < image->width; j++){
                    if(j >= 0){
                        png_byte *ptr = &(row[j*indicator]);
                        ptr[0] = params[0];
                        ptr[1] = params[1];
                        ptr[2] = params[2];
                        if(indicator == 4) ptr[3] = params[3];
                    }
                }
            }
        }
    }
}

void rectsPNG(sPng *image, char *rect_color, char *frame_color, char *width){
    int color_indicator = checkColorType(image);
    if(color_indicator == -1) return;

    char colours[10][20] = {"red", "green", "blue", "gray", "yellow", "purple", "white", "black", "cyan", "orange"};
    int index_rect = -1, index_frame = -1;
    
    for(int i = 0; i < 10; i++){
        if(!strcmp(rect_color, colours[i])) index_rect = i;
        if(!strcmp(frame_color, colours[i])) index_frame = i;
    }
    

    int *rects_par = (int *)calloc(4, sizeof(int));
    int *frame_par = (int *)calloc(4, sizeof(int));
    if(!setParams(rects_par, index_rect, rect_color) || !setParams(frame_par, index_frame, frame_color)){
        free(rects_par);
        free(frame_par);
        return;
    }

    int rect_count = 0;
    Rect *arr_rects = (Rect *)calloc(image->height / 2, sizeof(Rect));
    for(int i = 0; i < image->height - 1; i++){
        for(int j = 0; j < image->width - 1; j++){
            if(isRectColor(image, j, i, color_indicator, rects_par) && !isRectColor(image, j-1, i, color_indicator, rects_par) && !isRectColor(image, j, i-1, color_indicator, rects_par)){
                int x = j, y = i;
                int width = 0, height = 0;
                while(isRectColor(image, x, i, color_indicator, rects_par)){
                    ++x;
                    ++width;
                }
                while(isRectColor(image, j, y, color_indicator, rects_par)){
                    ++y;
                    ++height;
                }
                if(isRectFill(image, width, height, color_indicator, rects_par, j, i)){
                    if(isRectShape(image, width, height, color_indicator, rects_par, j, i)){
                        arr_rects[rect_count].x = j;
                        arr_rects[rect_count].y = i;
                        arr_rects[rect_count].height = height;
                        arr_rects[rect_count].width = width;
                        ++rect_count;
                    }
                }
            }
        }
    }
    if(!rect_count){
        puts("There is not any rectangle of this color");
        free(rects_par);
        free(frame_par);
        free(arr_rects);
        return;
    }

    if(!isNum(width)){
        puts("Wrong width");
        free(rects_par);
        free(frame_par);
        free(arr_rects);
        return;
    }

    drawRectFrame(image, arr_rects, frame_par, atoi(width), rect_count, color_indicator);
    free(frame_par);
    free(rects_par);
    free(arr_rects);
}

void rectsGet(int argc, char **argv, int *opt_index, sPng *image, char *filename){
    const char *opts = "c:f:w:?";
    struct option lOpt[] = {
        {"color", required_argument, NULL, 'c'},
        {"framecolor", required_argument, NULL, 'f'},
        {"width", required_argument, NULL, 'w'},
        {NULL, 0, NULL, 0}
    };
    char *rect_color = NULL, *frame_color = NULL, *width = NULL;
    int opt;
    while (-1 != (opt = getopt_long(argc, argv, opts, lOpt, opt_index)))
    {
        switch(opt){
            case 'c':
                rect_color = optarg;
                break;
            case 'f':
                frame_color = optarg;
                break;
            case 'w':
                width = optarg;
                break;
            case '?':
            default:
                printHelp(5);
                writePNG(filename, image);
                exit(0);
                break;
        }
    }
    if(rect_color == NULL || frame_color == NULL || width == NULL){
        puts("You did not choose one or more options of rectangle command");
        printHelp(5);
        writePNG(filename, image);
        exit(0);
    }
    rectsPNG(image, rect_color, frame_color, width);
    if(!writePNG(filename, image)){
        exit(0);
    } 
}