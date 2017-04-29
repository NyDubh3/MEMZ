#pragma once
#include "memz.h"

extern const char *sites[];
extern const char *sounds[];

extern const size_t nSites;
extern const size_t nSounds;

#ifndef CLEAN
extern const unsigned char code1[];
extern const unsigned char code2[];

extern const size_t code1_len;
extern const size_t code2_len;

extern const unsigned char msg[];
extern const char *msgs[];

extern const size_t msg_len;
extern const size_t nMsgs;
#endif