#include "info.h"

int isNum(char *str){
    int len = strlen(str);
    for(int i = 0; i < len; i++){
        if(!isdigit(str[i]))
            return 0;
    }
    return 1;
}

void setParams(int *parametrs, int ind, char *colour){
    char *color = (char *)calloc(strlen(colour) + 1, sizeof(char));
    if(!color) return;
    strcpy(color, colour);
    int i = 0;
    char *ptr;
    parametrs[3] = 255;
    switch (ind)
    {
        case -1:
            i = 0;
            ptr = strtok(color, ",");
            while(ptr){
                parametrs[i++] = atoi(ptr);
                ptr = strtok(NULL, ",");
            }
            break;
        case 0:
            parametrs[0] = 255;
            break;
        case 1:
            parametrs[1] = 255;
            break;
        case 2:
            parametrs[2] = 255;
            break;
        case 3:
            parametrs[0] = 190;
            parametrs[1] = 190;
            parametrs[2] = 190;
            break;
        case 4:
            parametrs[0] = 255;
            parametrs[1] = 255;
            break;
        case 5:
            parametrs[0] = 160;
            parametrs[1] = 32;
            parametrs[2] = 240;
            break;
        case 6:
            parametrs[0] = 255;
            parametrs[1] = 255;
            parametrs[2] = 255;
            break;
        case 7:
            parametrs[0] = 0;
            parametrs[1] = 0;
            parametrs[2] = 0;
            break;
        case 8:
            parametrs[0] = 0;
            parametrs[1] = 255;
            parametrs[2] = 255;
            break;
        case 9:
            parametrs[0] = 255;
            parametrs[1] = 165;
            break;
        default:
            break;
    }
    free(color);
}

void printHelp(int arg){
    puts("-? -h --help\n");
    if(arg == 1 || !arg){
        puts("the program has the following structre:\n\t<name_of_executable_file> <names_of_options> <file_name>");
        puts("\tfile must have png format -> <file_name.png>");
        puts("-s --standart - to learn standart colors that are used in the function transform and the frames that are used in the function frame");
        puts("");
    }
    if(arg == 2 || !arg){
        puts("-c --copy - copy pixels from one area to another");
        puts("\t-l --leftup <value>,<value> - coordinates of upper-lefе corner");
        puts("\t-r --rightlow <value>,<value> - coordinates of lower-right corner");
        puts("\t-e --endleftup <value>,<value> - coordinates of the upper-left corner where the program paste copied area");
        puts("");
    }
    if(arg == 3 || !arg){
        puts("-t --transform - change one color to another in a given area");
        puts("\t -f --fromcolor - the color to change");
        puts("\t -t --tocolor - the color to change the current color");
        puts("");
    }
    if(arg == 4 || !arg){
        puts("-f --frame - make a frame aroud the picture");
        puts("\t -t --type - type of pattern");
        puts("\t -c --color - color of pattern");
        puts("\t -w --width - width of pattern");
        puts("");
    }

    if(arg == 5 || !arg){
        puts("-r --rects - find rectangles and make the frame around them");
        puts("\t -c --color - color of rectanges");
        puts("\t -f --framecolor - color of frame");
        puts("\t -w -width - width of frame");
        puts("");
    }
    if(arg == 6){
        puts("There is no any PNG files");
        puts("");
    }
}

void readPNG(char *filename, sPng *image){
    FILE *fp = fopen(filename, "rb");
    char header[8];
    if(!fp) {fclose(fp); exit(0);};
    
    fread(header, 1, 8, fp);

    if(png_sig_cmp(header, 0, 8)) {fclose(fp); exit(0);}
    
    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!image->png_ptr) {fclose(fp); exit(0);}
    
    image->info_ptr = png_create_info_struct(image->png_ptr);
    if(!image->info_ptr) {fclose(fp); exit(0);}

    if(setjmp(png_jmpbuf(image->png_ptr))) {fclose(fp); exit(0);}

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);
    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);
    image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);

    if(setjmp(png_jmpbuf(image->png_ptr))) {fclose(fp); exit(0);}

    image->row_pointers = (png_bytep *)calloc(image->height, sizeof(png_bytep));
    for(int i = 0; i < image->height; i++){
        image->row_pointers[i] = (png_byte *)malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));
    }

    png_read_image(image->png_ptr, image->row_pointers);


    fclose(fp);
}

void writePNG(char *filename, sPng *image){
    FILE *fout = fopen(filename, "wb");
    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!image->png_ptr){fclose(fout); exit(0);}
    image->info_ptr = png_create_info_struct(image->png_ptr);

    png_init_io(image->png_ptr, fout);
    
    if(setjmp(png_jmpbuf(image->png_ptr))){fclose(fout); exit(0);}
    
    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height, image->bit_depth, image->color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(image->png_ptr, image->info_ptr);

    if(setjmp(png_jmpbuf(image->png_ptr))){fclose(fout); exit(0);}

    png_write_image(image->png_ptr, image->row_pointers);

    if(setjmp(png_jmpbuf(image->png_ptr))){fclose(fout); exit(0);}

    png_write_end(image->png_ptr, NULL);
    for(int i = 0; i < image->height; i++){
        free(image->row_pointers[i]);
    }
    free(image->row_pointers);
    fclose(fout);
}

int checkColorType(sPng *image){
    switch (png_get_color_type(image->png_ptr, image->info_ptr))
    {
        case PNG_COLOR_TYPE_PALETTE:
            puts("Your type is PALETTE, but expected RGB or RGBA"); // if the picture is not RGB or RGBA drop the program
            return -1;
            break;
        case PNG_COLOR_TYPE_GA:
            puts("Your type is GA, but expected RGB or RGBA");
            return -1;
            break;
        case PNG_COLOR_TYPE_GRAY:
            puts("Your type is GRAY, but expected RGB or RGBA");
            return -1;
            break;
        case PNG_COLOR_TYPE_RGB:
            return 3;
            break;
        case PNG_COLOR_TYPE_RGBA:
            return 4;
            break;
        default:
            break;
    }
}
