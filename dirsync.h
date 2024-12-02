#ifndef DIRSYNC_H
#define DIRSYNC_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utime.h>
#include <regex.h>
#include <getopt.h>
#include <time.h>

#define OPTLIST "aprvi:o:n:"

// Function prototypes

size_t directoryCount;
typedef struct {
    char *name;
    char *path;
    time_t modification;
    mode_t permissions;
} FileInfo;
typedef struct {
    FileInfo *fileInfos; // Pointer to an array of FileInfo structs
    int fileInfoCount;
    char* directorypath;
} DirectoryInfo;

typedef struct {
    char** patterns;  // An array of regex patterns
    size_t count;     // Number of patterns
} RegexPatterns;
void freeFileInfoArray(FileInfo *fileInfos, int count);
extern RegexPatterns *iregexPatterns;
extern RegexPatterns *oregexPatterns;


void parseCommandLineOptions( int argc, char *argv[] );
extern FileInfo *fileInfos;
extern DirectoryInfo *directoryInfos;
extern size_t directoryCount;
int aflag;
int rflag;
int pflag; 
int iflag;
int oflag;
int readindex; 
void processDirectory(const char *dirPath);
void printDirectoryInfo(const DirectoryInfo *dirInfo);


void comparedirectories(const DirectoryInfo *dir1, const DirectoryInfo *dir2);
void comparefilename(const DirectoryInfo *dir1, const DirectoryInfo *dir2);
void addfile(const char *filename, const char *sourcedir, const char *destdir, const DirectoryInfo *dir1);
void processSubDirectory(const char *subdirpath, DirectoryInfo *dir);
void comparefilepath(const DirectoryInfo *dir1, const DirectoryInfo *dir2);
const char *getrelativefilepath(const char *directorypath, const char *filepath);
int subdirectoriesexists(const char *path);
const char *getsubdirectorypath(const char *pathname,int noofsubdirectories, int index);
void comparesubdirectories(const DirectoryInfo *dir, int noofsubdirectories, const char *path, const DirectoryInfo *dir2);
bool subdirLocationExists(const char *subdirectoryPath, const char *checkRelativePath);
const char *getLeftoverPath(const char *pathname, int totalSubdirectories, int index);
 const char* removefirstdirectory(const char *pathname);
 const char* getfirstdirectory(const char *pathname);
const char* adddirectory(const char *pathname, const char *directoryname);
const char *removelastdirectory(const char *pathname);
void syncingwhole(const DirectoryInfo *dir1, const DirectoryInfo *dir2, const char *tobecreated, const char *where);
int createDirectory(const char *directoryName, const char *path);
char *glob2regex(char *glob);
void addPattern(RegexPatterns *patterns, const char *pattern);
void freeRegexPatterns(RegexPatterns *patterns);
RegexPatterns* createRegexPatterns();
int matchesPatterns(const char* string, const RegexPatterns* regexPatterns);
#endif
