/*
 * serialCommands.c
 *
 *  Created on: 3 Nov 2018
 *      Author: sven
 */

#include "serialCommands.h"
#include "stm32f1xx_hal.h"

#include <string.h>
#include <stdint.h>

extern char UART_Out_Buffer[];
extern uint16_t UART_Out_Cnt;

void sendCmd(char *cmd) {
	strcpy(UART_Out_Buffer, cmd);
	UART_Out_Cnt = strlen(cmd);
}

void cmdJogX(double amount) {
	char cmd[64];
	snprintf(cmd, 64, "JOG_X %f\n", amount);
	sendCmd(cmd);
}

void cmdJogY(double amount) {
	char cmd[64];
	snprintf(cmd, 64, "JOG_Y %f\n", amount);
	sendCmd(cmd);
}

void cmdJogZ(double amount) {
	char cmd[64];
	snprintf(cmd, 64, "JOG_Z %f\n", amount);
	sendCmd(cmd);
}

void cmdExtrude(double amount) {
	char cmd[64];
	snprintf(cmd, 64, "EXTRUDE %f\n", amount);
	sendCmd(cmd);
}

void cmdHome() {
	char cmd[64];
	snprintf(cmd, 64, "HOME\n");
	sendCmd(cmd);
}

void cmdUnload() {
	char cmd[64];
	snprintf(cmd, 64, "UNLOAD\n");
	sendCmd(cmd);
}

void cmdLoad() {
	char cmd[64];
	snprintf(cmd, 64, "LOAD\n");
	sendCmd(cmd);
}

void cmdSetBedTemp(uint16_t t) {
	char cmd[64];
	snprintf(cmd, 64, "SET_BED_TEMP %d\n", t);
	sendCmd(cmd);
}

void cmdSetToolTemp(uint16_t t) {
	char cmd[64];
	snprintf(cmd, 64, "SET_TOOL_TEMP %d\n", t);
	sendCmd(cmd);
}

void cmdSetFolder(char* folder) {
	char cmd[64];
	snprintf(cmd, 64, "SET_FOLDER %s\n", folder);
	sendCmd(cmd);
}

void cmdGetFolderEntries(uint16_t no_from, uint16_t no_to) {
	char cmd[64];
	snprintf(cmd, 64, "GET_FOLDER_ENTRIES %d %d\n", no_from, no_to);
	sendCmd(cmd);
}

void cmdSelectFolderEntry(uint16_t no) {
	char cmd[64];
	snprintf(cmd, 64, "SEL_FOLDER_ENTRY %d\n", no);
	sendCmd(cmd);
}

void cmdInit() {
	sendCmd("INIT\n");
}


void cmdPrint(){
	sendCmd("PRINT\n");
}

void cmdPause() {
	sendCmd("PAUSE\n");
}

void cmdResume() {
	sendCmd("RESUME\n");
}

void cmdCancel(){
	sendCmd("CANCEL\n");
}
