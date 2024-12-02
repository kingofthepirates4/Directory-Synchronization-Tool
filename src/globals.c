// Includes declarations of global variables and functions
#include "dirsync.h"
DirectoryInfo *directoryInfos = NULL; // Pointer to an array of DirectoryInfo structs
size_t directoryCount = 0; // Number of directories processed
RegexPatterns *iregexPatterns = NULL; // Pointer to a RegexPatterns struct
RegexPatterns *oregexPatterns = NULL; // Pointer to a RegexPatterns struct