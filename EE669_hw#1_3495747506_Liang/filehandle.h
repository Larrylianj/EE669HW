//
//  filehandle.h
//
//  Created by Siyang Liang on 2/4/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#ifndef BWT_filehandle_h
#define BWT_filehandle_h

FILE * openFileForRead(char * filePath);
FILE * openFileForWrite(char * filePath);
void closeFile(FILE * fileHandle);

#endif
