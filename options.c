#include "dirsync.h"

// Function to parse command-line options
void parseCommandLineOptions(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, OPTLIST)) != -1) {
        switch (opt) {
            case 'a':
                aflag = 1;
                break;
            case 'r':
                rflag = 1;
                break;
            case 'p':
                pflag = 1;
                break;
            case 'i':
                iflag = 1;
                char *iregexvalue = glob2regex(optarg);
                addPattern(iregexPatterns, iregexvalue);
                break;
            case 'o':
                oflag = 1;
                char *oregexvalue = glob2regex(optarg);
                addPattern(oregexPatterns, oregexvalue);
                break;
            
        default:

                exit(EXIT_FAILURE);
        }
    }
}
