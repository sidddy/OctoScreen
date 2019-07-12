#ifndef SERIALCOMMANDS_H_
#define SERIALCOMMANDS_H_

#include <stdint.h>

void cmdJogX();
void cmdJogY();
void cmdJogZ();
void cmdExtrude();
void cmdHome();
void cmdSetBedTemp(uint16_t t);
void cmdSetToolTemp(uint16_t t);
void cmdSetFolder(char* folder);
void cmdGetFolderEntries(uint16_t no_from, uint16_t no_to);
void cmdSelectFolderEntry(uint16_t no);
void cmdInit();
void cmdPrint();
void cmdPause();
void cmdResume();
void cmdCancel();

#endif /* SERIALCOMMANDS_H_ */


