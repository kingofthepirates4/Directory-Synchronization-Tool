#include "dirsync.h"

// Given the filename, the directory to copy from and the directory to paste to, it copies the file
void addfile(const char *filename, const char *sourcedir, const char *destdir, const DirectoryInfo *dir1) {
    printf("Adding file '%s' from Directory 1 to Directory 2\n", filename);
    const char *sourceFilePath = sourcedir; 
    char destFilePath[1024];
    const char* sourcedir1 = getsubdirectorypath(sourcedir,1,1);
    
    // Construct full source and destination file paths

    snprintf(destFilePath, sizeof(destFilePath), "%s/%s", destdir, filename);
    struct stat sourceStat, destStat;
    // Get file attributes of the source and destination files
    if (stat(destFilePath, &destStat) == -1) {
        // Destination file doesn't exist; create it by copying the source file
        FILE *srcFile, *destFile;
        char buffer[1024];
        size_t bytesRead;
        
        // Open the source file for reading
        srcFile = fopen(sourceFilePath, "rb");
        if (srcFile == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        
        // Open the destination file for writing
        destFile = fopen(destFilePath, "wb");
        if (destFile == NULL) {
            perror("fopen");
            fclose(srcFile);
            exit(EXIT_FAILURE);
        }
        
        // Copy the contents of the source file to the destination file
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcFile)) > 0) {
            fwrite(buffer, 1, bytesRead, destFile);
        }
        
        // Close both files
        fclose(srcFile);
        fclose(destFile);
        if (pflag==0){
            if (chmod(destFilePath, 0666) == -1) {
                perror("chmod");
                exit(EXIT_FAILURE);
            }
            printf("New file permissions: %o\n", destStat.st_mode & 0777);
        }
        if (pflag==1){
            
            if (chmod(destFilePath, dir1->fileInfos[readindex].permissions) == -1) {
                perror("chmod");
                exit(EXIT_FAILURE);
                
            }
            printf("New file permissions: %o\n", destStat.st_mode & 0777);
        }
        printf("FILE PERMISSION: %o\n", dir1->fileInfos[readindex].permissions);
           if (stat(destFilePath, &destStat) == 0) {
    } else {
        perror("stat");
    }
        
    }
    printf("File '%s' added successfully.\n", filename);
}


int createDirectory(const char *directoryName, const char *path) {
    // Construct the full path by concatenating the provided path and directory name
    char fullPath[256];  // Adjust the size as needed
    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, directoryName);

    // Use the mkdir function to create the directory
    if (mkdir(fullPath, 0755) == 0) {
        printf("Directory '%s' created successfully.\n", fullPath);
        return 1;  // Success
    } else {

        perror("Subdirectory alreadys exists");
        return 0;  // Failure
    }
}

const char* getfirstdirectory(const char *pathname){
    char *firstdirectory = strdup(pathname);
    char *slash = strchr(firstdirectory, '/');
    if (slash != NULL) {
        *slash = '\0';
    }
    return firstdirectory;
}
const char* removefirstdirectory(const char *pathname) {
    const char *start = strchr(pathname, '/');
    const char *end = pathname + strlen(pathname) - 1;

    if (start == NULL) {
        // No '/' found, return the whole string
        return strdup(pathname);
    }

    while (*end == '/' && end > start) {
        end--;
    }

    int length = end - start;

    if (length <= 0) {
        // If the result length is not positive, return NULL or an empty string, depending on your requirements
        return NULL;
    }

    char *result = (char*)malloc(length + 1);
    strncpy(result, start + 1, length);
    result[length] = '\0';

    return result;
}




const char* adddirectory(const char *pathname, const char *directoryname){
    size_t lengthofdestinationpath = strlen(pathname) + strlen(directoryname) + 2;
    char *destinationp = (char *)malloc(lengthofdestinationpath);
    snprintf(destinationp, lengthofdestinationpath, "%s/%s", pathname, directoryname);
    return destinationp;
}

  
const char *getsubdirectorypath(const char *pathname,int noofsubdirectories, int index) {
    int len = strlen(pathname);
    int count = 0;
    int lastSlashIndex = -1;
    // Iterate through the pathname in reverse to find the last slash
    for (int i = len - 1; i >= 0; i--) {
        if (pathname[i] == '/') {
            count++;
            lastSlashIndex = i;

            if (count == index) {
                
                break;
            }
        }
    }
    // Find the start of the subdirectory path
    int startIndex = lastSlashIndex;
    //Create a copy of the subdirectory path
    char *copy = strdup(pathname);
    copy[startIndex] = '\0';

    return copy;
}



bool subdirLocationExists(const char *subdirectoryPath, const char *checkRelativePath) {
    if (subdirectoryPath == NULL || checkRelativePath == NULL) {
        // Invalid input
        return false;
    }

    size_t subdirectoryLen = strlen(subdirectoryPath);
    size_t checkLen = strlen(checkRelativePath);

    if (checkLen < subdirectoryLen) {
        // The check path is shorter, so it can't start with the subdirectory path
        return false;
    }

    // Compare the subdirectoryPath with the beginning of checkRelativePath
    if (strncmp(subdirectoryPath, checkRelativePath, subdirectoryLen) == 0) {
        // The checkRelativePath starts with subdirectoryPath or is the same
        return true;
    }

    return false;
}
    

const char *getrelativefilepath(const char *directorypath, const char *filepath) {
    // Find the lengths of both paths
    size_t dir_len = strlen(directorypath);
    size_t file_len = strlen(filepath);

    // Initialize variables to track the common prefix length
    size_t common_len = 0;

    // Find the length of the common prefix
    while (common_len < dir_len && common_len < file_len && directorypath[common_len] == filepath[common_len]) {
        common_len++;
    }

    // Calculate the relative path length
    size_t relative_len = file_len - common_len;

    // Allocate memory for the relative path
    char *relativepath = (char *)malloc(relative_len + 1);

    // Copy the remaining part of the filepath to the relative path
    strcpy(relativepath, filepath + (common_len+1));

    return relativepath;
}

int subdirectoriesexists(const char *path) {
    // Initialize a counter for path separators
    int separatorCount = 0;
    // Iterate through each character in the path
    for (size_t i = 0; i < strlen(path); i++) {
        if (path[i] == '/') {
            separatorCount++;
        }
    }
    // If there is at least one path separator, subdirectories exist
    return separatorCount;
}


const char *getLeftoverPath(const char *pathname, int noofsubdirectories, int index) {
    int len = strlen(pathname);
    int slashCount = 0;
    int startIndex = -1;

    // Find the last slash in the pathname
    for (int i = len - 1; i >= 0; i--) {
        if (pathname[i] == '/') {
            slashCount++;
            if (slashCount == index) {
                startIndex = i;
                break;  // Stop the loop after finding startIndex
            }
        }
    }

    // If startIndex is still -1, it means there are not enough slashes in the pathname
    if (startIndex == -1) {
        return NULL; // Indicate an error or handle it as needed
    }

    // Return the substring from startIndex to the end of the string
    return &pathname[startIndex + 1]; // +1 to exclude the slash
}


