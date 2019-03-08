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
 * private.h
 * Private definitions, data structures, macros and functions for libartnet
 * Copyright (C) 2004-2007 Simon Newton
 */

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#if !defined(WIN32) && !defined(_MSC_VER)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "artnet.h"
#include "artnet/packets.h"
#include "misc.h"
#include "tod.h"

#ifndef ARTNET_PRIVATE_H
#define ARTNET_PRIVATE_H

// these are defined in artnet.c
extern int ARTNET_PORT;
extern char ARTNET_STRING[];
extern int ARTNET_STRING_SIZE;
extern uint8_t ARTNET_VERSION;
extern uint8_t OEM_HI;
extern uint8_t OEM_LO;
extern char ESTA_HI;
extern char ESTA_LO;
extern uint8_t TTM_BEHAVIOUR_MASK;
extern uint8_t TTM_REPLY_MASK;
extern uint8_t PROGRAM_NO_CHANGE;
extern uint8_t PROGRAM_DEFAULTS;
extern uint8_t PROGRAM_CHANGE_MASK;
extern uint8_t HIGH_NIBBLE;
extern uint8_t LOW_NIBBLE;
extern uint8_t STATUS_PROG_AUTH_MASK;
extern uint8_t PORT_STATUS_LPT_MODE;
extern uint8_t PORT_STATUS_SHORT;
extern uint8_t PORT_STATUS_ERROR;
extern uint8_t PORT_STATUS_DISABLED_MASK;
extern uint8_t ORT_STATUS_MERGE;
extern uint8_t PORT_STATUS_DMX_TEXT;
extern uint8_t PORT_STATUS_DMX_SIP;
extern uint8_t PORT_STATUS_DMX_TEST;
extern uint8_t PORT_STATUS_ACT_MASK;
extern uint8_t PORT_DISABLE_MASK;
extern uint8_t TOD_RESPONSE_FULL;
extern uint8_t TOD_RESPONSE_NAK;
extern uint8_t MIN_PACKET_SIZE;
extern uint8_t MERGE_TIMEOUT_SECONDS;
extern uint8_t FIRMWARE_TIMEOUT_SECONDS;
extern uint8_t RECV_NO_DATA;

#ifndef TRUE
extern int TRUE;
extern int FALSE;
#endif

extern uint16_t LOW_BYTE;
extern uint16_t HIGH_BYTE;

// non artnet specific
#define SA struct sockaddr
#define SI struct in_addr

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

// byte ordering macros
#ifndef bswap_16
#define bswap_16(x)  ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
#endif

// htols : convert short from host to little endian order
#ifdef WIN32
#  define htols(x)  (x)
#else
# ifdef HAVE_ENDIAN_H
#  if BYTE_ORDER == __BIG_ENDIAN
#   define htols(x)   bswap_16 (x)
#  else
#   define htols(x)  (x)
#  endif
# else
#  if BYTE_ORDER == BIG_ENDIAN
#   define htols(x)   bswap_16 (x)
#  else
#   define htols(x)  (x)
#  endif
# endif
#endif

// convert from shorts to bytes and back again
#define short_get_high_byte(x) ((HIGH_BYTE & x) >> 8)
#define short_get_low_byte(x)  (LOW_BYTE & x)

#define bytes_to_short(h,l) ( ((h << 8) & 0xff00) | (l & 0x00FF) );

/*
 * These are enums for fields in packets
 * Ordered by packets they appear in. Some of these appear in the public header file
 * artnet.h if they have to be used by the user.
 *
 * They are all in the form artnet_xxx_code in case they have to be made public some day
 *
 */

// ArtPollReply
//-----------------------------------------------------------------------------

// the node report codes
typedef enum {
  ARTNET_RCDEBUG,
  ARTNET_RCPOWEROK,
  ARTNET_RCPOWERFAIL,
  ARTNET_RCSOCKETWR1,
  ARTNET_RCPARSEFAIL,
  ARTNET_RCUDPFAIL,
  ARTNET_RCSHNAMEOK,
  ARTNET_RCLONAMEOK,
  ARTNET_RCDMXERROR,
  ARTNET_RCDMXUDPFULL,
  ARTNET_RCDMXRXFULL,
  ARTNET_RCSWITCHERR,
  ARTNET_RCCONFIGERR,
  ARTNET_RCDMXSHORT,
  ARTNET_RCFIRMWAREFAIL,
  ARTNET_RCUSERFAIL
} artnet_node_report_code;

// these define the types of node that can exist
// note it's different from artnet_node_type
typedef enum {
  STNODE = 0x00,
  STSERVER = 0x01,
  STMEDIA = 0x02,
  STROUTE = 0x03,
  STBACKUP = 0x04,
  STCONFIG = 0x05
} artnet_node_style_code;

// artnet_port_data in artnet.h

// ArtAddress
//-----------------------------------------------------------------------------

// artnet_port_command_code in artnet.h

// ArtFirmwareMaster
//-----------------------------------------------------------------------------

// defines contents of the firmware packet
typedef enum  {
  ARTNET_FIRMWARE_FIRMFIRST = 0x00,
  ARTNET_FIRMWARE_FIRMCONT = 0x01,
  ARTNET_FIRMWARE_FIRMLAST = 0x02,
  ARTNET_FIRMWARE_UBEAFIRST = 0x03,
  ARTNET_FIRMWARE_UBEACONT = 0x04,
  ARTNET_FIRMWARE_UBEALAST = 0x05,
} artnet_firmware_type_code;

// ArtFirmwareReply
//-----------------------------------------------------------------------------

// artnet_firmware_status_code in artnet.h



// End packet enums
//-----------------------------------------------------------------------------


/*
 * Data structures to manage callbacks
 */

// packet callbacks have a function and some user data
typedef struct {
  int (*fh)(artnet_node n, void *p, void *data);
  void *data;
} callback_t;

/*
 * the dmx callback is triggered when a dmx packet arrives
 */
typedef struct {
  int (*fh)(artnet_node n, int portid, void *data);
  void *data;
} dmx_callback_t;

/*
 * firmware callback is triggered when a firmware recieve has been completed sucessfully
 */
typedef struct {
  int (*fh)(artnet_node n, int ubea, uint16_t *data, int length, void *d);
  void *data;
} firmware_callback_t;

/*
 * called when a node is remote programmed
 */
typedef struct {
  int (*fh)(artnet_node n, void *d);
  void *data;
} program_callback_t;

/*
 * called when a node receives rdm data
 */
typedef struct {
  int (*fh)(artnet_node n, int address, uint8_t *rdm, int length, void *d);
  void *data;
} rdm_callback_t;


// struct to store callbacks
typedef struct {
  callback_t recv;
  callback_t send;
  callback_t poll;
  callback_t reply;
  callback_t dmx;
  callback_t address;
  callback_t input;
  callback_t todrequest;
  callback_t toddata;
  callback_t todcontrol;
  callback_t rdm;
  callback_t ipprog;
  callback_t firmware;
  callback_t firmware_reply;
  dmx_callback_t dmx_c;
  firmware_callback_t firmware_c;
  program_callback_t program_c;
  rdm_callback_t  rdm_c;
  dmx_callback_t rdm_init_c;
  dmx_callback_t rdm_tod_c;
} node_callbacks_t;


// End callback structures
//-----------------------------------------------------------------------------

/*
 * Begin port structures
 */



// first a generic port
typedef struct {
  uint8_t addr;        // the port address
  uint8_t default_addr;    // the address set by the hardware
  uint8_t net_ctl;      // if the port address is under network control
  uint8_t status;        // status of the port
  uint8_t enabled;      // true if the port has had it's address set, this is internal only,
                // it's not used by the ArtNet protocol, otherwise the node keeps
                // picking up packets for the 0x00 port
  tod_t tod;
} g_port_t;

/**
 * struct to represent an input port
 * input ports need to keep track of sequence numbers
 * (this doesn't seem to be used though)
 */
typedef struct {
  g_port_t port;
  uint8_t seq;
} input_port_t;


/**
 * For output ports we need to track if they merge in HTP or LTP modes
 */
typedef enum {
  ARTNET_MERGE_HTP,
  ARTNET_MERGE_LTP
} merge_t;

/**
 * struct to represent an output port
 *
 * output ports can merge data from two sources in either HTP
 * (highest takes precedence) or LPT (lowest takes precedence) mode
 *
 * we need to store:
 *   o The data from each source
 *   o The ip of the source
 *   o The time the data was recv'ed
 *   o the tod table, length and max length
 */
typedef struct {
  g_port_t port;
  int  length;        // the length of the data THAT HAS CHANGED since the last dmx packet
  uint8_t enabled;    // true if the port has had it's address set, this is internal only,
                      // it's not used by the ArtNet protocol, otherwise the node keeps
                      // picking up packets for the 0x00 port
  uint8_t  data[ARTNET_DMX_LENGTH]; // output data
  merge_t merge_mode; // for merging
  uint8_t dataA[ARTNET_DMX_LENGTH];
  uint8_t dataB[ARTNET_DMX_LENGTH];
  time_t timeA;
  time_t timeB;
  SI ipA;
  SI ipB;
} output_port_t;

// use defines to hide the inner structures
#define port_addr port.addr
#define port_default_addr port.default_addr
#define port_net_ctl port.net_ctl
#define port_status port.status
#define port_enabled port.enabled
#define port_tod port.tod

// End port structures
//-----------------------------------------------------------------------------


/*
 * We use a linked list to keep track of nodes on the network
 * Here be the structures
 */

/* firstly we have the potential to do a firmware transfer to any node,
 * this struct keeps the information such as how much data has been transfered
 * and the address of the peer. It's also used for receiving firmware
 */
typedef struct {
  uint16_t *data;
  int      bytes_current;
  int      bytes_total;
  struct   in_addr peer;
  int      ubea;
  time_t   last_time;
  int      expected_block;
  int      (*callback)(artnet_node n, artnet_firmware_status_code code, void *d);
  void     *user_data;
} firmware_transfer_t;

/*
 * The node entry in the LL. It contains the public entry, as well as some stuff
 * we don't want public like firmware
 */
typedef struct node_entry_private_s {
  artnet_node_entry_t pub;
  struct node_entry_private_s *next;
  firmware_transfer_t firmware;
  SI ip;  // don't rely on the ip address that the node
          // sends, they could be faking it. This is the ip that
          // the pollreply was sent from
} node_entry_private_t;

/**
 * The node list stores a pointer to the first, last and current
 * entries.
 */
typedef struct {
  node_entry_private_t *first;
  node_entry_private_t *current;
  node_entry_private_t *last;
  int length;
} node_list_t;


// End node list structures
//-----------------------------------------------------------------------------


// the status of the node
typedef enum {
  ARTNET_OFF,
  ARTNET_STANDBY,
  ARTNET_ON
} node_status_t;


// struct to hold the state of the node
typedef struct {
  artnet_node_type node_type;
  node_status_t mode;
  SI reply_addr;
  SI ip_addr;
  SI bcast_addr;
  uint8_t hw_addr[ARTNET_MAC_SIZE];
  uint8_t default_subnet;
  uint8_t subnet_net_ctl;
  int send_apr_on_change;
  int ar_count;
  int verbose;
  char short_name[ARTNET_SHORT_NAME_LENGTH];
  char long_name[ARTNET_LONG_NAME_LENGTH];
  char report[ARTNET_REPORT_LENGTH];
  uint8_t subnet;
  uint8_t oem_hi;
  uint8_t oem_lo;
  uint8_t esta_hi;
  uint8_t esta_lo;
  int bcast_limit; // the number of nodes after which we change to bcast
  artnet_node_report_code report_code;
} node_state_t;


typedef struct {
  struct artnet_node_s *peer;        // peer if we've joined a group
  int   master;
} node_peering_t;


/**
 * The main node structure
 */
typedef struct artnet_node_s{
  artnet_socket_t sd;      // the two sockets
  node_state_t state;      // the state struct
  node_callbacks_t callbacks;  // the callbacks struct
  struct ports_s {
    uint8_t  types[ARTNET_MAX_PORTS];    // type of port
    input_port_t in[ARTNET_MAX_PORTS];   // input ports
    output_port_t out[ARTNET_MAX_PORTS]; // output ports
  } ports;
  artnet_reply_t ar_temp;       // buffered artpoll reply packet
  node_list_t node_list;        // node list
  firmware_transfer_t firmware; // firmware details
  node_peering_t peering;       // peer if we've joined a group
} artnet_node_t;


typedef artnet_node_t *node;

/*
 * Function definitions follow
 */

// exported from artnet.c
node_entry_private_t *find_private_entry( node n, artnet_node_entry e);
void check_timeouts(node n);
node_entry_private_t *find_entry_from_ip(node_list_t *nl, SI ip);
int artnet_nl_update(node_list_t *nl, artnet_packet reply);


// exported from receive.c
int handle(node n, artnet_packet p);
int16_t get_type(artnet_packet p);
void reset_firmware_upload(node n);


// exported from transmit.c
int artnet_tx_poll(node n, const char *ip,  artnet_ttm_value_t ttm);
int artnet_tx_poll_reply(node n, int reply);
int artnet_tx_tod_data(node n, int id);
int artnet_tx_firmware_reply(node n, in_addr_t ip, artnet_firmware_status_code code);
int artnet_tx_firmware_packet(node n, firmware_transfer_t *firm );
int artnet_tx_tod_request(node n);
int artnet_tx_tod_control(node n, uint8_t address, artnet_tod_command_code action);
int artnet_tx_rdm(node n, uint8_t address, uint8_t *data, int length);
int artnet_tx_build_art_poll_reply(node n);


// exported from network.c
int artnet_net_recv(node n, artnet_packet p, int block);
int artnet_net_send(node n, artnet_packet p);
int artnet_net_set_non_block(node n);
int artnet_net_init(node n, const char *ip);
int artnet_net_start(node n);
int artnet_net_close(artnet_socket_t sock);
int artnet_net_join(node n1, node n2);
int artnet_net_set_fdset(node n, fd_set *fdset);
int artnet_net_inet_aton(const char *ip_address, struct in_addr *address);
const char *artnet_net_last_error();

#endif
