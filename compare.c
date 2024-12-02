#include "dirsync.h"
// The main function that compares two directories
// Decides how to compare the directories based on the flags
void comparedirectories(const DirectoryInfo *dir1, const DirectoryInfo *dir2) {
    //If r flag given , it compares subdirectories and top-level position
    if (rflag==1){
    comparefilepath(dir1, dir2);
    }
    //If no r flag given, it compares filenames & modification time
    if (rflag==0){
        comparefilename(dir1, dir2);          
    }
}

//  Simple filename + modification time co
//  Given two directories, it compares the file names and modification time
//  Finds out the files that are in directory 1 but not in directory 2
//  Finds out if the files in directory 1 are newer than the ones in directory 2
// If the file is not found in the other directory (in the iteration of second for loop), it is added to the other directory
void comparefilename(const DirectoryInfo *dir1, const DirectoryInfo *dir2){
    // Compares filenames & modification time (from source directory to destination directory) by running two for loops for comparison
    for (int i = 0; i < dir1->fileInfoCount; i++) {
        const char *filename1 = dir1->fileInfos[i].name;
        int found = 0;
        for (int j = 0; j < dir2->fileInfoCount; j++) {
            const char *filename2 = dir2->fileInfos[j].name;
            if (strcmp(filename1, filename2) == 0) {        
                found = 1;;
                const time_t modification2 = dir2->fileInfos[j].modification;
                const time_t modification1 = dir1->fileInfos[i].modification;
                if (modification1 > modification2) {
                    printf("File '%s' in %s is newer than the one in %s\n", filename1, dir1->directorypath, dir2->directorypath);
                    //Addfile(filename,sourcedir, destdir)
                    // Doubt!!! Does it create a new file and remove older file or overwrite the original one?
                } 
            }
        } // If the file is not found in the other directory (in the iteration of second for loop), it is added to the other directory
        if (!found) {
            printf("File '%s' found in %s but not in %s\n", filename1, dir1->directorypath, dir2->directorypath);
            const char* sourcedir = dir1->directorypath;
            const char* destdir = dir2->directorypath; 
            addfile(filename1, sourcedir, destdir, dir1);
        }
    }
}

void comparefilepath(const DirectoryInfo *dir1, const DirectoryInfo *dir2){
    for (int i = 0; i < dir1->fileInfoCount; i++) {
        const char *filepath1 = dir1->fileInfos[i].path;
        const char *relativepath1 = getrelativefilepath(dir1->directorypath, filepath1);
        printf("filepath1: %s\n", filepath1);
        printf("relativepath1: %s\n", relativepath1);
        int completepathfound = 0;;    
        // Check 1: Does the fullfilepath match in both directories?
                    for (int j = 0; j < dir2->fileInfoCount; j++) {
                        
                        const char *filepath2 = dir2->fileInfos[j].path;
                        const char *relativepath2 = getrelativefilepath(dir2->directorypath, filepath2);
                        
                        if (strcmp(relativepath1, relativepath2) == 0) {
                            completepathfound = 1;
                            break;
                        }
                        free((void*)relativepath2);
                    }
                    //Yay!!! The file exists in both directories. Proceed to next one.
                    if (completepathfound == 1) {
                        printf("File '%s' found in %s and also in %s\n", filepath1, dir1->directorypath, dir2->directorypath);
                        
                    }
                    // If it doesn't exist, then either a file or a file system needs to be created
                    else{
                        printf("Not the exact same filepath found, but files or directories must be created\n");
                        int noofsubdirectories = subdirectoriesexists(relativepath1);
                       
                        if (noofsubdirectories >0){
                            //Call subdirectories comparison function
                            comparesubdirectories(dir1, noofsubdirectories, relativepath1, dir2);
                        }
                        else{
                            printf("Only a file needs to be created since no top level processing stuff\n");
                            const char *nameoffile = getsubdirectorypath(relativepath1, 1, 1);
                            printf("relativefilepath: %s\n", relativepath1);
                            printf("nameoffile: %s\n", nameoffile);
                            addfile(nameoffile, dir1->directorypath, dir2->directorypath, dir1);
                        }
                    }
                    
        //             free((void *)relativepath1);
        // free((void *)filepath1);
    }
    
}

void comparesubdirectories(const DirectoryInfo *dir1, int noofsubdirectories, const char *path, const DirectoryInfo *dir2){
 
    const char *pathcopy = strdup(path);
    const char* tobecreated = getfirstdirectory(pathcopy);
    
    pathcopy = removefirstdirectory(pathcopy);
    const char* destination= dir2->directorypath;
    
    for (int i = 1; i <= (noofsubdirectories); i++){

        if (createDirectory(tobecreated, destination)==0) {

            destination = adddirectory(destination, tobecreated);
            tobecreated = getfirstdirectory(pathcopy);
            
            
            printf("Directory '%s' already exists in %s\n", tobecreated, destination);
        }
        else{
            printf("Subdirectory created\n");
            destination = adddirectory(destination, tobecreated);
            tobecreated = getfirstdirectory(pathcopy);
            printf("tobecreated: %s\n", tobecreated);
            printf("destination: %s\n", destination);
            
        }
    }
    
    const char* nameoffile = getLeftoverPath(path, 1,1);
    printf("nameoffile: %s\n", nameoffile);
    printf("destination: %s\n", destination);
    char resultpath[1024];
    snprintf(resultpath, sizeof(resultpath), "%s/%s", dir1->directorypath, path);
    addfile(nameoffile, resultpath, destination, dir1);
    printf("ends successfully\n"); 
}


  