#ifndef PARSE_H_
#define PARSE_H_

#include "register.h"
#include <string.h>

void parse_args(int argc, char *argv[]);

void file_parse();

void set_zero(BYTE k, instr_st *p);

int getROBtag();
void fillROB(int ROBtag,BYTE i);
void fillIQ();
void fillEX(BYTE i, instr_st* ptr);
void wakeup(int wakeup_dest);
void fillWB(int i, int tot);


void fetch();
void decode();
void Rename();
void regread();
void dispatch();
void issue();
void execute();
void writeback();
void retire();

void init_status();
void print_bundle(instr_st *ptr, char ch);
void print_bundleEX(instr_st *ptr, char ch);
void printIQ();
void printROB();
void display_scope();

BYTE AdvanceCycle();

//void display();

#endif /* PARSE_H_ */