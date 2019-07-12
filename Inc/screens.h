/*
 * screens.h
 *
 *  Created on: 2 Nov 2018
 *      Author: sven
 */

#ifndef SCREENS_H_
#define SCREENS_H_

void initScreens();

void setMessage(char* msg);

void setTempBedActual(float val);
void setTempBedTarget(float val);
void setTempToolActual(float val);
void setTempToolTarget(float val);

void setZHeight(float z);
void setProgress(float p);
void setPrintTime(uint16_t t);
void setPrintTimeLeft(uint16_t t);
void setJob(char* name);
void setFolder(char* name);
void setFolderCount(uint16_t c);
void setFolderEntry(uint16_t no, char* name);
void setStatus(char* status);
void rotateLabels();

#endif /* SCREENS_H_ */
