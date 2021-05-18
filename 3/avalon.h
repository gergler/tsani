//==============================================================================
//
// Title:       avalon_lib.h
// Purpose:     A short description of the interface.
//
// Created on:  16.03.2021 at 10:47:10 by Gennady Kuzin.
// Copyright:   NSU. All Rights Reserved.
//
//==============================================================================

#ifndef __avalon_lib_H__
#define __avalon_lib_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

void Init(int adr, int subadr);
void Write(int adr, int subadr, int data);
int Read(int adr, int subadr);
void DACcode(double u);
void DAC(double code); 
void ACP(int* data1);
void SETI(void);
void IACK(void);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __avalon_lib_H__ */
