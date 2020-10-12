/*
 * Port number allocation for CanSerial
 *
 *  Copyright (C) 2019 Eug Krashtan <eug.krashtan@gmail.com>
 *  This file may be distributed under the terms of the GNU GPLv3 license.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "cansock.h"
#include "portnumber.h"

typedef struct {
	uint16_t port;
	uint8_t uuid[CAN_UUID_SIZE];
} tPnKeep;

static int pn_len = 0;
static int max_pn = 0;
static tPnKeep *dict;

// power of two
static int dictsize = 2;

static void printuuid(FILE* stream, uint8_t *u)
{
    int i;
    for (i=0; i< CAN_UUID_SIZE; i++) {
        if (i>0)
            fprintf(stream, ":");
        fprintf(stream, "%02X", u[i]);
    }
}

static void addnum(uint16_t p, uint8_t *u)
{
    if(pn_len+1 >= dictsize) {
        // Increase allocated space
        dictsize *= 2;
        dict = realloc(dict, sizeof(tPnKeep) * dictsize);
        if (!dict) {
            fprintf(stderr, "realloc failed!\n");
            exit(1);
        }
    }
    memcpy(dict[pn_len].uuid, u, CAN_UUID_SIZE);
    dict[pn_len].port = p;
    pn_len++;

    // max port for automatic allocation
    if(max_pn<p) max_pn = p;
}

void PnInit(void)
{
    FILE *fp;
    dict = malloc(sizeof(tPnKeep) * dictsize);
    if (!dict) {
        fprintf(stderr, "malloc failed!\n");
        exit(1);
    }
}


uint16_t PnGetNumber(uint8_t* u)
{
    for (int i=0; i<pn_len; i++) {
        if (memcmp(dict[i].uuid, u, CAN_UUID_SIZE) == 0) {
            return dict[i].port;
        }
    }
    addnum(max_pn+1,u);
    return max_pn; // already incremented value in previous call
}


