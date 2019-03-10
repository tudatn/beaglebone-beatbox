/*
 * support.h
 *	Module provides support (general) functions to all modules.
 *
 *  Created on: Jun 8, 2018
 *      Author: tudatn
 */

#ifndef SUPPORT_H_
#define SUPPORT_H_

/*
 * support functions to read/ write from/to files
 */
void readFromFile(char *fileName, char *readValue);

char* getFileName(int nth, char *generalPath, char *subFile, char *returnedFilePath);

void writeToFile(char *fileName, const char *content);

#endif /* SUPPORT_H_ */
