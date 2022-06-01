#include "info.h"

int isNum(char *str){
    int len = strlen(str);
    for(int i = 0; i < len; i++){
        if(!isdigit(str[i]))
            return 0;
    }
    return 1;
}

int setParams(int *parametrs, int ind, char *colour){
    char *color = (char *)calloc(strlen(colour) + 1, sizeof(char));
    if(!color) return 0;
    strcpy(color, colour);
    int i = 0, num;
    char *ptr;
    parametrs[3] = 255;
    switch (ind)
    {
        case -1:
            i = 0;
            ptr = strtok(color, ",");
            while(ptr){
                if(!isNum(ptr)){
                    free(color);
                    puts("Wrong color");
                    return 0;
                }
                num = atoi(ptr);
                if(num > 255 || num < 0){
                    free(color);
                    puts("Wrong argument of RGB/RGBA code");
                    printf("%d %c %d\n", num, num>255?'>':'<', num>255?255:0);
                    return 0;

                }
                parametrs[i++] = num;
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
            parametrs[0] = 150;
            parametrs[1] = 150;
            parametrs[2] = 150;
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
    return 1;
}

void printHelp(int arg){
    puts("-? -h --help\n");
    if(arg == 1 || !arg){
        puts("The program has the following structure:\n\t<name_of_executable_file> <names_of_options> <file_name>");
        puts("\tFile must have png format -> <file_name.png>");
        puts("-s --standart - to learn standart colors that are used in the function transform and the frames that are used in the function frame");
        puts("If you want to use all functions correctly write the file name in the end of your commands");
        puts("If you want to enter the code of color in RGB or RGBA the structure must be <Red_Component>,<Green_Component>,<Blue_Component>,<Alpha_Component>\n(The last one uses if you have RGBA picture)");
        puts("");
    }
    if(arg == 2 || !arg){
        puts("-c --copy - copy pixels from one area to another");
        puts("\t-l --leftup <x_axis>,<y_axis> - coordinates of upper-left corner");
        puts("\t-r --rightlow <x_axis>,<y_axis> - coordinates of lower-right corner");
        puts("\t-e --endleftup <x_axis>,<y_axis> - coordinates of the upper-left corner where the program paste copied area");
        puts("");
        puts("WARNING: y axis directed from top to bottom\nupper-left corner has coordinates (0,0) and lower-left corner has coordinates (0, height of picture - 1)");
        puts("");
    }
    if(arg == 3 || !arg){
        puts("-t --transform - change one color to another in a given area");
        puts("\t -f --fromcolor <value>- the color to change");
        puts("\t -t --tocolor <value> - the color to change the current color");
        puts("");
    }
    if(arg == 4 || !arg){
        puts("-f --frame - make a frame aroud the picture");
        puts("\t -t --type <value> - type of pattern");
        puts("\t -c --color <value> - color of pattern");
        puts("\t -w --width <value> - width of pattern");
        puts("");
    }

    if(arg == 5 || !arg){
        puts("-r --rects - find rectangles and make the frame around them");
        puts("\t -c --color <value> - color of rectanges");
        puts("\t -f --framecolor <value> - color of frame");
        puts("\t -w --width <value> - width of frame");
        puts("");
    }
    if(arg == 6){
        puts("There is no any PNG files");
        puts("");
    }
}

int readPNG(char *filename, sPng *image){
    FILE *fp = fopen(filename, "rb");
    char header[8];
    if(!fp) {
        puts("Could not read file");
        return 0;
    };
    
    fread(header, 1, 8, fp);

    if(png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        puts("Could not reead header");
        return 0;
    }
    
    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!image->png_ptr) {
        fclose(fp);
        puts("Could not create read struct");
        return 0;
    }
    
    image->info_ptr = png_create_info_struct(image->png_ptr);
    if(!image->info_ptr) {
        png_destroy_read_struct(&image->png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        puts("Could not create info struct");
        return 0;
    }

    if(setjmp(png_jmpbuf(image->png_ptr))) {
        png_destroy_read_struct(&image->png_ptr, &image->info_ptr, (png_infopp)NULL);
        fclose(fp);
        puts("Something went wrong");
        return 0;
    }

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);
    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);
    image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);

    if(setjmp(png_jmpbuf(image->png_ptr))) {
        png_destroy_read_struct(&image->png_ptr, &image->info_ptr, (png_infopp)NULL);
        fclose(fp);
        puts("Something went wrong");
        return 0;
    }

    image->row_pointers = (png_bytep *)calloc(image->height, sizeof(png_bytep));
    for(int i = 0; i < image->height; i++){
        image->row_pointers[i] = (png_byte *)malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));
    }

    png_read_image(image->png_ptr, image->row_pointers);

    return 1;
    fclose(fp);
}

int writePNG(char *filename, sPng *image){
    FILE *fout = fopen(filename, "wb");

    if(!fout){
        puts("Could not read file");
        return 0;
    }

    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if(!image->png_ptr){
        fclose(fout);
        png_destroy_write_struct(&image->png_ptr, (png_infopp)NULL);
        return 0;
    }
    
    image->info_ptr = png_create_info_struct(image->png_ptr);

    png_init_io(image->png_ptr, fout);
    
    if(setjmp(png_jmpbuf(image->png_ptr))){
            fclose(fout);
            png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
            return 0;
        }
    
    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height, image->bit_depth, image->color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(image->png_ptr, image->info_ptr);

    if(setjmp(png_jmpbuf(image->png_ptr))){
        fclose(fout);
        png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
        return 0;
    }

    png_write_image(image->png_ptr, image->row_pointers);

    if(setjmp(png_jmpbuf(image->png_ptr))){
        fclose(fout);
        png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
        return 0;
    }

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

void info(sPng *image){
    puts("Standart colors");
    puts("\t*black\n\t*yellow\n\t*orange\n\t*white\n\t*green\n\t*blue\n\t*cyan\n\t*gray\n\t*red\n\t*purple");
    puts("");
    puts("Standart frames");
    puts("\t*common\n\t*fractal\n\t*tunnel\n\t*chess");
    int color;
    switch (checkColorType(image))
    {
    case -1:
        exit(0);
        break;
    case 3:
        color = 3;
        break;
    case 4:
        color = 4;
        break;
    default:
        break;
    }
    printf("Color type - %s\n", color == 3 ? "RGB" : "RGBA");
    printf("Color depth - %d\n", image->bit_depth);
    printf("Height of the picture - %d\n", image->height);
    printf("Width of the picture - %d\n", image->width);
}
