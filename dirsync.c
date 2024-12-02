#include "dirsync.h"

int main(int argc, char *argv[]){
    
    // Check and validate input
    if (argc < 2) {
    fprintf(stderr, "Usage: %s <directory_path> ...\n", argv[0]);
    exit(EXIT_FAILURE); 
    }

    iregexPatterns = createRegexPatterns();
    oregexPatterns = createRegexPatterns();
    
    // Parse command-line options
    parseCommandLineOptions(argc, argv);
      // Adjust argc and argv after parsing options
    argc -= optind;
    argv += optind;


    printf("Regex Patterns:\n");
    for (int i = 0; i < oregexPatterns->count; i++) {
        printf("[%d]: %s\n", i, oregexPatterns->patterns[i]);
    }
    // Allocate space for directoryInfo based on the number of directories
    directoryInfos = (DirectoryInfo *)malloc(argc * sizeof(DirectoryInfo));
    if (directoryInfos == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Call processDirectory for each directory and populate the directoryInfos array
    for (int a = 0; a < argc; ++a) {
        processDirectory(argv[a]);
        directoryCount++;    
    }

    // For debugging
    for (size_t i = 0; i < directoryCount; i++) 
    {
        printDirectoryInfo(&directoryInfos[i]);
    }

    // Start Comparing and identifying files that need to be copied
    // After identification, comparedirectories function will then call addfile() or to copy the files and directories
    for (int i = 0; i <  (int)directoryCount; i++) {
        for (int j = i + 1; j < (int)directoryCount; j++) {
            // Call it again with reversed arguments
            comparedirectories(&directoryInfos[i], &directoryInfos[j]); // Identifies files in dir1 that need to be copied to dir2
            comparedirectories(&directoryInfos[j], &directoryInfos[i]);  // Identifies files in dir2 that need to be copied to dir1
        }
    }

    //Free allocated memory for FileInfo and DirectoryInfo structs
    for (size_t i = 0; i < directoryCount; i++) {
        freeFileInfoArray(directoryInfos[i].fileInfos, directoryInfos[i].fileInfoCount);
    }
    for (size_t i = 0; i < directoryCount; i++) {
        free(directoryInfos[i].directorypath);
    }
    // Free the array itself
    free(directoryInfos);
    freeRegexPatterns(iregexPatterns);
    freeRegexPatterns(oregexPatterns);

}

// Function to free the FileInfo array (To be incoporated as part of main function later)
void freeFileInfoArray(FileInfo *fileInfos, int count) {
   
    if (fileInfos == NULL) {
        return; // Nothing to free
    }

    for (int i = 0; i < count; i++) {
        free(fileInfos[i].name);
        free(fileInfos[i].path);
    }

    free(fileInfos);
}

// For debugging
void printDirectoryInfo(const DirectoryInfo *dirInfo) {
    printf("Directory Path: %s\n", dirInfo->directorypath);
    printf("File Count: %d\n", dirInfo->fileInfoCount);

    for (int i = 0; i < dirInfo->fileInfoCount; i++) {
        const FileInfo *fileInfo = &dirInfo->fileInfos[i];
        printf("File Name: %s\n", fileInfo->name);
        printf("File Path: %s\n", fileInfo->path);
        printf("Modification Time: %s", ctime(&fileInfo->modification));
        // Add more details as needed
    }

    printf("\n"); // Add a separator between directory info
}

RegexPatterns* createRegexPatterns() {
    RegexPatterns* regexPatterns = (RegexPatterns*)malloc(sizeof(RegexPatterns));
    if (regexPatterns == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    regexPatterns->patterns = NULL;
    regexPatterns->count = 0;
    return regexPatterns;
}

void addPattern(RegexPatterns* regexPatterns, const char* pattern) {
    regexPatterns->patterns = (char**)realloc(regexPatterns->patterns, (regexPatterns->count + 1) * sizeof(char*));
    if (regexPatterns->patterns == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    regexPatterns->patterns[regexPatterns->count] = strdup(pattern);
    if (regexPatterns->patterns[regexPatterns->count] == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    regexPatterns->count++;
}

void freeRegexPatterns(RegexPatterns* regexPatterns) {
    if (regexPatterns != NULL) {
        for (size_t i = 0; i < regexPatterns->count; i++) {
            free(regexPatterns->patterns[i]);
        }
        free(regexPatterns->patterns);
        free(regexPatterns);
    }
}
