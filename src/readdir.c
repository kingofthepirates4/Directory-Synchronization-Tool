#include "dirsync.h"

void processDirectory(const char *dirPath) {
    DIR *dir = opendir(dirPath); // opens directory

    if (dir == NULL) { // checks for errors in opening directory
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    
    // To read and extract info about the directory
    struct dirent *entry; // creates a dirent struct which will be used to read the directory
    struct stat fileStat; // creates a stat struct which will be used to extract information about the file
    char fullFilePath[1024]; // Adjust the buffer size as needed
    
    // Creates a DirectoryInfo struct and initializes it 
    DirectoryInfo *currentDirInfo = &directoryInfos[directoryCount];
    currentDirInfo->fileInfos = NULL; // Initialize to NULL
    currentDirInfo->fileInfoCount = 0;
    currentDirInfo->directorypath = strdup(dirPath);
   
   
    while ((entry = readdir(dir)) != NULL) {
        
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            // Skip "." and ".." entries to avoid infinite recursion
            continue;
        }
        // If aflag off, skip hidden files
        if (aflag==0 && (entry->d_name[0] == '.' || strcmp(entry->d_name, "..") == 0)) {
            continue;
        }
        // If iflag on, check if file name matches any of the regex patterns

        //stores and prints the full path of the found file
        snprintf(fullFilePath, sizeof(fullFilePath), "%s/%s", dirPath, entry->d_name);
          
        /*      lstat functions fills out file stat struct with information about the file
                        if unsuccessful it shows error and continue to the next file          */

        if (lstat(fullFilePath, &fileStat) == -1) 
        {
            perror("lstat");
            continue; 
        }

         // Check if the entry is a regular file or a directory
        if (S_ISREG(fileStat.st_mode)) {  // Regular file found
            size_t index = currentDirInfo->fileInfoCount; // Get the current index
            if (iflag && matchesPatterns(entry->d_name, iregexPatterns)) {
                continue;
            }
            if (oflag && !matchesPatterns(entry->d_name, oregexPatterns)) {
                continue;
            }
            //  First, allocate memory to store the fileinfo in the array
            
            // Malloc if it's the first file, otherwise realloc
            if (currentDirInfo->fileInfoCount == 0) 
            {
                currentDirInfo->fileInfos = (FileInfo *)malloc(sizeof(FileInfo));
            } 
            else 
            {
                currentDirInfo->fileInfos = (FileInfo *)realloc(currentDirInfo->fileInfos,((currentDirInfo->fileInfoCount) + 1) * sizeof(FileInfo));
            }

            // Check for errors in malloc/realloc
            if (currentDirInfo->fileInfos == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }

            // Get the current fileinfo
            FileInfo *fileInfo = &currentDirInfo->fileInfos[index];
            
            // Store the filename and check for errors
            fileInfo->name = strdup(entry->d_name);
            if (fileInfo->name == NULL) {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
            // Store the filepath and check for errors
            fileInfo->path = strdup(fullFilePath);
            if (fileInfo->path == NULL) {
                perror("strdup");
                exit(EXIT_FAILURE);
            }

            // Store the modification time
            fileInfo->modification = fileStat.st_mtime;

            // Increment the fileinfo count
            currentDirInfo->fileInfoCount++;
            fileInfo->permissions = fileStat.st_mode & 0777;
            printf("File permission: %o\n", fileStat.st_mode & 0777);
            printf("file permission: %o\n", fileInfo->permissions);


            
        } else if (rflag== 1 && S_ISDIR(fileStat.st_mode)) {
            processSubDirectory(fullFilePath, currentDirInfo);

            /*  Since Subdirectories are processed differently ,
                    ( We don't need to create a new directoryinfo struct for them),
                                                we call a different function to process them */
        }
        
    }

    closedir(dir);
}
        

void processSubDirectory(const char *subdirPath, DirectoryInfo *currentDirInfo) {
    DIR *dir = opendir(subdirPath); // opens directory

    if (dir == NULL) { // checks for errors in opening directory
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    
    // To read and extract info about the directory
    struct dirent *entry; // creates a dirent struct which will be used to read the directory
    struct stat fileStat; // creates a stat struct which will be used to extract information about the file
    char fullFilePath[1024]; // Adjust the buffer size as needed


    while ((entry = readdir(dir)) != NULL) {
        
        //Avoids hidden files for now to avoid complication
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
        {
            // Skip "." and ".." entries to avoid infinite recursion
            continue;
        }
        if (aflag==0 && (entry->d_name[0] == '.' || strcmp(entry->d_name, "..") == 0)) {
            continue;
        }
    
        //stores and prints the full path of the found file
        snprintf(fullFilePath, sizeof(fullFilePath), "%s/%s", subdirPath, entry->d_name);
       
        if (lstat(fullFilePath, &fileStat) == -1) {
            perror("lstat");
            continue; // Skip this file and continue with the next one
        }

        // Check if the entry is a regular file or a directory
        if (S_ISREG(fileStat.st_mode)) {
            // Regular file found
            size_t index = currentDirInfo->fileInfoCount; // Get the current index
            if (iflag && matchesPatterns(entry->d_name, iregexPatterns)) {
                continue;
            }
            if (oflag && !matchesPatterns(entry->d_name, oregexPatterns)) {
                continue;
            }
            if (currentDirInfo->fileInfoCount == 0) 
            {
                currentDirInfo->fileInfos = (FileInfo *)malloc(sizeof(FileInfo));
            } 
            else 
            {
                currentDirInfo->fileInfos = (FileInfo *)realloc(currentDirInfo->fileInfos,((currentDirInfo->fileInfoCount) + 1) * sizeof(FileInfo));
            }

            if (currentDirInfo->fileInfos == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }

            FileInfo *fileInfo = &currentDirInfo->fileInfos[index];
            

            fileInfo->name = strdup(entry->d_name);
            if (fileInfo->name == NULL) {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
            fileInfo->path = strdup(fullFilePath);
            if (fileInfo->path == NULL) {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
            fileInfo->modification = fileStat.st_mtime;
            currentDirInfo->fileInfoCount++;
             fileInfo->permissions = fileStat.st_mode & 0777;
            printf("File permission: %o\n", fileStat.st_mode & 0777);
            printf("file permission: %o\n", fileInfo->permissions);
        } 
        else if (rflag== 1 && S_ISDIR(fileStat.st_mode)) 
        {
            processSubDirectory(fullFilePath, currentDirInfo);
            // Recursively processes subdirectories
        }
        
    }

    closedir(dir);
}
