#include "info.h"

void transformGet(int argc, char **argv, int *opt_index, sPng *image, char *filename){
    readPNG(filename, image);
    const char *opts = "f:t:?";
    struct option lOpt[] = {
        {"fromcolor", required_argument, NULL, 'f'},
        {"tocolor", required_argument, NULL, 't'},
        {NULL, 0, NULL, 0}
    };
    char *color1, *color2;
    int opt;
    while(-1 != (opt = getopt_long(argc, argv, opts, lOpt, opt_index))){
        switch (opt)
        {
            case 'f':
                puts("from");
                color1 = optarg;
                printf("%s\n", optarg);
                break;
            case 't':
                puts("to");
                color2 = optarg;
                printf("%s\n", optarg);
                break;
            case '?':
            default:
                printHelp(3);
                break;
        }
    }
    transformPNG(image, color1, color2);
    writePNG(filename, image);
}

void transformPNG(sPng *image, char *color1, char *color2){
    int color_indicator = checkColorType(image);
    if(color_indicator == -1) return;
    
    char colours[10][20] = {"red", "green", "blue", "gray", "yellow", "purple", "white", "black", "cyan", "orange"};
    int index1 = -1, index2 = -1;
    
    for(int i = 0; i < 10; i++){
        if(!strcmp(color1, colours[i])) index1 = i;
        if(!strcmp(color2, colours[i])) index2 = i;
    }
    int *parametrs1 = (int *)calloc(3, sizeof(int));
    int *parametrs2 = (int *)calloc(3, sizeof(int));
    char *c1 = (char *)calloc(strlen(color1) + 1, sizeof(char));
    char *c2 = (char *)calloc(strlen(color2) + 1, sizeof(char));
    strcpy(c1, color1);
    strcpy(c2, color2);
    setParams(parametrs1, index1, c1);
    setParams(parametrs2, index2, c2);
    free(c1);
    free(c2);
    printf("%d %d %d %d\n", parametrs1[0], parametrs1[1], parametrs1[2], index1);
    printf("%d %d %d %d\n", parametrs2[0], parametrs2[1], parametrs2[2], index2);
    for(int i = 0; i < image->height; i++){
        png_byte *row = image->row_pointers[i];
        for(int j = 0; j < image->width; j++){
            png_byte *ptr = &(row[j * color_indicator]);
            if(ptr[0] == parametrs1[0] && ptr[1] == parametrs1[1] && ptr[2] == parametrs1[2]){
                ptr[0] = parametrs2[0];
                ptr[1] = parametrs2[1];
                ptr[2] = parametrs2[2];
            }
        }
    }
    free(parametrs1);
    free(parametrs2);
}
