/************************** Start of filehandle.c *************************
 *
 * This utility file contains basic file handles
 *
 * A valid file path should be apply to this functions
 *
 * Created by Siyang Liang on 2/6/14.
 *  Copyright (c) 2014 SiyangLiang. All rights reserved.
 *
 *
 */

#include <stdio.h>

// Open file for reading according to the file path
FILE * openFileForRead(char * filePath)
{
    
    FILE * fp;
    fp = fopen(filePath, "rb");
    
    if (NULL == fp) {
        
        printf("Fail to open the file, check the file path!\n");
        
    }else{
        
        printf("Open file:\n %s \nsuccessfully\n",filePath);
        
    }
    
    return fp;
    
}


// Open file for writing according to the file path
FILE * openFileForWrite(char * filePath)
{
    
    FILE * fp;
    fp = fopen(filePath, "wb");
    
    if (NULL == fp) {
        
        printf("Fail to open the file, check the file path!\n");
        
    }else{
        
        printf("Open file:\n %s \nsuccessfully\n",filePath);
        
    }
    
    return fp;
    
}


// Close file
void closeFile(FILE * fileHandle)
{
    
    fclose(fileHandle);
    
    printf("Close file!\n");
    
}
