#include "info.h"


void copyAreaPNG(sPng *image, char *left_up, char *right_down, char *destination){
    int color_indicator = checkColorType(image);
    if(color_indicator == -1) return;
    
    char *start = (char *)calloc(strlen(left_up) + 1, sizeof(char));
    char *end = (char *)calloc(strlen(right_down) + 1, sizeof(char));
    char *dest = (char *)calloc(strlen(destination) + 1, sizeof(char));
    strcpy(start, left_up);
    strcpy(end, right_down);
    strcpy(dest, destination);
    int startx, starty, endx, endy, destx, desty;
    char *ptr1 = strtok(start, ","), *ptr2 = strtok(NULL, ",");
    
    if(isNum(ptr1) && isNum(ptr2)){ 
        startx = atoi(ptr1);
        starty = atoi(ptr2);
    }
    else{
        puts("Start point has wrong arguments");
        free(start);
        free(end);
        free(dest);
        return;
    }
    ptr1 = strtok(end, ","), ptr2 = strtok(NULL, ",");
    if(isNum(ptr1) && isNum(ptr2)){ 
        endx = atoi(ptr1);
        endy = atoi(ptr2);
    }
    else{
        puts("End point has wrong arguments");
        free(start);
        free(end);
        free(dest);
        return;
    }
    ptr1 = strtok(dest, ","), ptr2 = strtok(NULL, ",");
    if(isNum(ptr1) && isNum(ptr2)){ 
        destx = atoi(ptr1);
        desty = atoi(ptr2);
    }
    else{
        puts("Destination point has wrong arguments");
        free(start);
        free(end);
        free(dest);
        return;
    }

    free(start);
    free(end);
    free(dest);

    if(starty < 0 || starty >= image->height || startx < 0 || startx >= image->width){
        puts("Wrong position start");
        return;
    }

    if(endy < 0 || endy >= image->height || endx < 0 || endx >= image->width){
        puts("Wrong position end");
        return;
    }

    if(desty < 0 || desty >= image->height || destx < 0 || destx >= image->width){
        puts("Wrong position destination");
        return;
    }

    if(startx >= endx || starty >= endy){
        puts("Start positions bigger then end ones");
        puts("   Start pos    End pos");
        if(startx >= endx) printf("x: %d    >      %d\n", startx, endx);
        if(starty >= endy) printf("y: %d    >      %d\n", starty, endy);
        return;
    }

    png_bytep *buf = (png_bytep *)calloc(endy - starty,sizeof(png_bytep));
    for(int i = 0; i < endy - starty; i++)
        buf[i] = (png_byte *)calloc(1, png_get_rowbytes(image->png_ptr, image->info_ptr));

    for(int i = starty; i < image->height && i < endy; i++){
        png_byte *row = image->row_pointers[i];
        png_byte *bufr = buf[i-starty];
        for(int j = startx; j < image->width && j < endx; j++){
            png_byte *ptr = &(row[j * color_indicator]);
            png_byte *buf_ptr = &(bufr[(j - startx) * color_indicator]);
            buf_ptr[0] = ptr[0];
            buf_ptr[1] = ptr[1];
            buf_ptr[2] = ptr[2];

            if(color_indicator == 4) buf_ptr[3] = ptr[3];
            
        }
    }
    for(int i = desty; i < image->height && i < endy - starty + desty; i++){
        png_byte *row = image->row_pointers[i];
        png_byte *bufr = buf[i-desty];
        for(int j = destx; j < image->width && j < endx - startx + destx; j++){
            png_byte *ptr = &(row[j*color_indicator]);
            png_byte *buf_ptr = &(bufr[(j-destx) * color_indicator]);
            ptr[0] = buf_ptr[0];
            ptr[1] = buf_ptr[1];
            ptr[2] = buf_ptr[2];

            if(color_indicator == 4)ptr[3] = buf_ptr[3];

        }
    }
    for(int i = 0; i < endy - starty; i++){
        free(buf[i]);
    }
    free(buf);
}


void copyAreaGet(int argc, char **argv, int *opt_index, sPng *image, char *filename){
    if(!readPNG(filename, image)){
        exit(0);
    }
    const char *opts = "l:r:e:?";
    struct option lOpt[] = {
        {"leftup", required_argument, NULL, 'l'},
        {"rightlow", required_argument, NULL, 'r'},
        {"endleftup", required_argument, NULL, 'e'},
        {NULL, 0, NULL, 0}
    };
    int opt;
    char *start, *end, *dest;
    while(-1 != (opt = getopt_long(argc, argv, opts, lOpt, opt_index))){
        switch (opt)
        {
            case 'l':
                start = optarg;
                break;
            case 'r':
                end = optarg;
                break;
            case 'e':
                dest = optarg;
                break;
            case '?':
            default:
                printHelp(2);
                break;
        }
    }
    copyAreaPNG(image, start, end, dest);
    if(!writePNG(filename, image)){
        exit(0);
    } 
}
