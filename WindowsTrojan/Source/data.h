#pragma once
#include "memz.h"

extern const char *sites[];
extern const char *sounds[];

extern const size_t nSites;
extern const size_t nSounds;

#ifndef CLEAN
extern const char STAGE1_MBR[];
extern const char STAGE2_MBR[];

extern const size_t STAGE1_MBR_LEN;
extern const size_t STAGE2_MBR_LEN;

extern const unsigned char msg[];
extern const char *msgs[];

extern const size_t msg_len;
extern const size_t nMsgs;
#endif