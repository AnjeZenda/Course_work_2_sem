#include "info.h"

int main(int argc, char **argv){
    const char *opts = "ctfrqhs?";
    char file_name[50];
    if(argc == 1){
        printHelp(argc);
    }
    else if(argc == 2){
        printHelp(0);
        exit(0);
    }
    else if (!strstr(argv[argc-1], ".png")){
        printHelp(6);
        exit(0);
    }
    else if(argc >= 3){
        strcpy(file_name, argv[--argc]);
    }
    sPng image;
    struct option longOpt[] = {
        {"copy", no_argument, NULL, 'c'},
        {"transform", no_argument, NULL, 't'},
        {"frame", no_argument, NULL, 'f'},
        {"rects", no_argument, NULL, 'r'},
        {"standart", no_argument, NULL, 's'},
        {"help", no_argument, NULL, 'h'},
        {"quit", no_argument, NULL, 'q'},
        {NULL, 0, NULL, 0}
    };
    int opt, opt_index;
    while((opt = getopt_long(argc, argv, opts, longOpt, &opt_index)) != -1){
        
        switch(opt){
            case 'c':
                copyAreaGet(argc, argv, &opt_index, &image, file_name);
                break;
            case 't':
                transformGet(argc, argv, &opt_index, &image, file_name);
                break;
            case 'f':
                makeFrameGet(argc, argv, &opt_index, &image, file_name);
                break;
            case 'r':
                rectsGet(argc, argv, &opt_index, &image, file_name);
                break;
            case 'h':
                printHelp(0);
                break;
            case 'q':
                exit(0);
                break;
            case 's':
                puts("red, green, blue, gray, yellow, purple, white, black, cyan, orange");
                puts("common, fractal, chess, tunnel");
                break;
            case '?':
            default:
                printHelp(0);
                break;
        }
    }
    return 0;
}
