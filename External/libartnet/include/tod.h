/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * tod.h
 * Header file for tod.h
 * Copyright (C) 2004-2005 Simon Newton
 */

#ifndef ARTNET_TOD_H
#define ARTNET_TOD_H

#include <stdint.h>
#include "common.h"

enum { ARTNET_TOD_INITIAL_SIZE = 100 };
enum { ARTNET_TOD_INCREMENT = 50 };

/*
 * table of devices
 */
typedef struct {
  uint8_t *data;
  int length;
  int max_length;
} tod_t;


extern int add_tod_uid(tod_t *tod, uint8_t uid[ARTNET_RDM_UID_WIDTH]);
extern int remove_tod_uid(tod_t *tod, uint8_t uid[ARTNET_RDM_UID_WIDTH]);
extern int flush_tod(tod_t *tod);
extern int reset_tod(tod_t *tod);

#endif
