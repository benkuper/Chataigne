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
 * misc.h
 * Header file for misc.c
 * Copyright (C) 2004-2005 Simon Newton
 */

#ifndef ARTNET_MISC_H
#define ARTNET_MISC_H

#include <errno.h>
#include <string.h>

extern char artnet_errstr[256];
void artnet_error(const char *fmt, ...);
int32_t artnet_misc_nbytes_to_32(uint8_t bytes[4]);
void artnet_misc_int_to_bytes(int data, uint8_t *bytes);

// check if the node is null and return an error
#define check_nullnode(node) if (node == NULL) { \
  artnet_error("%s : argument 1 (artnet_node) was null" , __FUNCTION__ ); \
  return ARTNET_EARG; \
}

#define artnet_error_malloc() artnet_error("%s : malloc error %s" , __FUNCTION__, strerror(errno) ) 
#define artnet_error_realloc() artnet_error("%s : realloc error %s" , __FUNCTION__, strerror(errno) ) 
#define artnet_error_nullnode() artnet_error("%s : argument 1 (artnet_node) was null" , __FUNCTION__ ) 

#endif
