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
 * artnet.h
 * Interface to libartnet
 * Copyright (C) 2004-2007 Simon Newton
 */

#ifndef ARTNET_HEADER_H
#define ARTNET_HEADER_H

#include <stdint.h>
// order is important here for osx
#include <sys/types.h>

#if !defined(WIN32) && !defined(_MSC_VER)
#include <sys/select.h>
#else
#include <winsock2.h>
typedef unsigned long in_addr_t;
#endif

#include <artnet/common.h>

/* the external storage class is "extern" in UNIX; in MSW it's ugly. */
#ifndef EXTERN
#ifdef MSW
#define EXTERN __declspec(dllexport) extern
#else
#define EXTERN extern
#endif /* MSW */
#endif

#ifdef __cplusplus
extern "C" {
#endif

EXTERN int ARTNET_ADDRESS_NO_CHANGE;

/**
 * An enum for setting the behaviour of a port.
 * Ports can either input data (DMX -> ArtNet) or
 * output (ArtNet -> DMX) data.
 */
typedef enum {
  ARTNET_ENABLE_INPUT = 0x40,  /**< Enables the input for this port */
  ARTNET_ENABLE_OUTPUT = 0x80  /**< Enables the output for this port */
} artnet_port_settings_t;


typedef enum {
  ARTNET_PC_NONE = 0x00,
  ARTNET_PC_CANCEL = 0x01,
  ARTNET_PC_LED_NORMAL = 0x02,
  ARTNET_PC_LED_MUTE = 0x03,
  ARTNET_PC_LED_LOCATE = 0x04,
  ARTNET_PC_RESET = 0x05,
  ARTNET_PC_MERGE_LTP_O = 0x10,
  ARTNET_PC_MERGE_LTP_1 = 0x11,
  ARTNET_PC_MERGE_LTP_2 = 0x12,
  ARTNET_PC_MERGE_LTP_3 = 0x13,
  ARTNET_PC_MERGE_HTP_0 = 0x50,
  ARTNET_PC_MERGE_HTP_1 = 0x51,
  ARTNET_PC_MERGE_HTP_2 = 0x52,
  ARTNET_PC_MERGE_HTP_3 = 0x53,
  ARTNET_PC_CLR_0 = 0x93,
  ARTNET_PC_CLR_1 = 0x93,
  ARTNET_PC_CLR_2 = 0x93,
  ARTNET_PC_CLR_3 = 0x93,
} artnet_port_command_t;


/*
 * An enum for the type of data transmitted on a port.
 * As far as I know, only DMX-512 is supported
 */
typedef enum  {
  ARTNET_PORT_DMX = 0x00,    /**< Data is DMX-512 */
  ARTNET_PORT_MIDI = 0x01,  /**< Data is MIDI */
  ARTNET_PORT_AVAB = 0x02,  /**< Data is Avab */
  ARTNET_PORT_CMX = 0x03,    /**< Data is Colortran CMX */
  ARTNET_PORT_ADB = 0x04,    /**< Data is ABD 62.5 */
  ARTNET_PORT_ARTNET = 0x05  /**< Data is ArtNet */
} artnet_port_data_code;


// defines the status of the firmware transfer
typedef enum  {
  ARTNET_FIRMWARE_BLOCKGOOD = 0x00,
  ARTNET_FIRMWARE_ALLGOOD = 0x01,
  ARTNET_FIRMWARE_FAIL = 0xff,
} artnet_firmware_status_code;

// tod actions
typedef enum  {
  ARTNET_TOD_FULL = 0x00,
  ARTNET_TOD_FLUSH = 0x01,
} artnet_tod_command_code;


/**
 * An enum for refering to a particular input or output port.
 */
typedef enum {
  ARTNET_INPUT_PORT = 1,    /**< The input port */
  ARTNET_OUTPUT_PORT,      /**< The output port */
} artnet_port_dir_t;


/*
 * Enum describing the type of node
 */
typedef enum {
  ARTNET_SRV,      /**< An ArtNet server (transmitts DMX data) */
  ARTNET_NODE,    /**< An ArtNet node   (dmx reciever) */
  ARTNET_MSRV,    /**< A Media Server */
  ARTNET_ROUTE,    /**< No Effect currently */
  ARTNET_BACKUP,    /**< No Effect currently */
  ARTNET_RAW      /**< Raw Node - used for diagnostics */
} artnet_node_type;


/*
 * Enum for the talk-to-me value
 * These values can be &'ed togeather, so for example to set private replies
 * and auto replying use :
 *   (ARTNET_TTM_PRIVATE & ARTNET_TTM_AUTO)
 */
typedef enum {
  ARTNET_TTM_DEFAULT = 0xFF,  /**< default, ArtPollReplies are broadcast, and nodes won't send a ArtPollReply when conditions change */
  ARTNET_TTM_PRIVATE = 0xFE,  /**< ArtPollReplies aren't broadcast */
  ARTNET_TTM_AUTO = 0xFD    /**< ArtPollReplies are send when node conditions chang */
} artnet_ttm_value_t;

/**
 * Enums for the application defined handlers
 */
typedef enum {
  ARTNET_RECV_HANDLER,    /**< Called on reciept of any ArtNet packet */
  ARTNET_SEND_HANDLER,    /**< Called on transmission of any ArtNet packet */
  ARTNET_POLL_HANDLER,    /**< Called on reciept of an ArtPoll packet */
  ARTNET_REPLY_HANDLER,    /**< Called on reciept of an ArtPollReply packet */
  ARTNET_DMX_HANDLER,      /**< Called on reciept of an ArtDMX packet */
  ARTNET_ADDRESS_HANDLER,    /**< Called on reciept of an ArtAddress packet */
  ARTNET_INPUT_HANDLER,    /**< Called on reciept of an ArtInput packet */
  ARTNET_TOD_REQUEST_HANDLER,  /**< Called on reciept of an ArtTodRequest packet */
  ARTNET_TOD_DATA_HANDLER,  /**< Called on reciept of an ArtTodData packet */
  ARTNET_TOD_CONTROL_HANDLER,  /**< Called on reciept of an ArtTodControl packet */
  ARTNET_RDM_HANDLER,      /**< Called on reciept of an ArtRdm packet */
  ARTNET_IPPROG_HANDLER,    /**< Called on reciept of an ArtIPProg packet */
  ARTNET_FIRMWARE_HANDLER,  /**< Called on reciept of an ArtFirmware packet */
  ARTNET_FIRMWARE_REPLY_HANDLER,  /**< Called on reciept of an ArtFirmwareReply packet */
} artnet_handler_name_t;


/*
 * Describes a remote ArtNet node that has been discovered
 */
typedef struct artnet_node_entry_s {
  uint8_t ip[ARTNET_IP_SIZE];  /**< The IP address, Network byte ordered*/
  int16_t ver;          /**< The firmware version */
  int16_t sub;          /**< The subnet address */
  int16_t oem;          /**< The OEM value */
  uint8_t ubea;          /**< The UBEA version */
  uint8_t status;
  uint8_t etsaman[ARTNET_ESTA_SIZE];        /**< The ESTA Manufacturer code */
  uint8_t shortname[ARTNET_SHORT_NAME_LENGTH];  /**< The short node name */
  uint8_t longname[ARTNET_LONG_NAME_LENGTH];  /**< The long node name */
  uint8_t nodereport[ARTNET_REPORT_LENGTH];  /**< The node report */
  int16_t numbports;        /**< The number of ports */
  uint8_t porttypes[ARTNET_MAX_PORTS];    /**< The type of ports */
  uint8_t goodinput[ARTNET_MAX_PORTS];
  uint8_t goodoutput[ARTNET_MAX_PORTS];
  uint8_t swin[ARTNET_MAX_PORTS];
  uint8_t swout[ARTNET_MAX_PORTS];
  uint8_t swvideo;
  uint8_t swmacro;
  uint8_t swremote;
  uint8_t style;
  uint8_t mac[ARTNET_MAC_SIZE];        /**< The MAC address of the node */
} artnet_node_entry_t;

/** A pointer to an artnet_node_entry_t */
typedef artnet_node_entry_t *artnet_node_entry;

typedef struct {
  char short_name[ARTNET_SHORT_NAME_LENGTH];
  char long_name[ARTNET_LONG_NAME_LENGTH];
  uint8_t subnet;
  uint8_t in_ports[ARTNET_MAX_PORTS];
  uint8_t out_ports[ARTNET_MAX_PORTS];
} artnet_node_config_t;


/** The local ArtNet node */
typedef void *artnet_node;

/** A list of remote ArtNet nodes */
typedef void *artnet_node_list;

#if !defined(WIN32) && !defined(_MSC_VER)
typedef int artnet_socket_t;
#else
typedef SOCKET artnet_socket_t;
#endif

// node control functions
EXTERN artnet_node artnet_new(const char *ip, int verbose);
EXTERN int artnet_setoem(artnet_node vn, uint8_t hi, uint8_t lo);
EXTERN int artnet_setesta(artnet_node vn, char hi, char lo);
EXTERN int artnet_set_bcast_limit(artnet_node vn, int limit);
EXTERN int artnet_start(artnet_node n);
EXTERN int artnet_read(artnet_node n, int timeout);
EXTERN int artnet_stop(artnet_node n);
EXTERN int artnet_destroy(artnet_node n);

int artnet_join(artnet_node vn1, artnet_node vn2);

// handler functions
// these need to be cleaned up into a generic interface
EXTERN int artnet_set_handler(artnet_node vn,
  artnet_handler_name_t handler,
  int (*fh)(artnet_node n, void *pp, void *d),
  void* data);
EXTERN int artnet_set_dmx_handler(artnet_node vn,
  int (*fh)(artnet_node n, int port, void *d),
  void *data);
EXTERN int artnet_set_program_handler(artnet_node vn,
  int (*fh)(artnet_node n, void *d),
  void *data);
EXTERN int artnet_set_firmware_handler(artnet_node vn,
  int (*fh)(artnet_node n, int ubea, uint16_t *data, int length, void *d),
  void *data);
EXTERN int artnet_set_rdm_handler(artnet_node vn,
  int (*fh)(artnet_node n, int address, uint8_t *rdm, int length, void *d),
  void *data);
EXTERN int artnet_set_rdm_initiate_handler(artnet_node vn,
  int (*fh)(artnet_node n, int port, void *d),
  void *data);
EXTERN int artnet_set_rdm_tod_handler(artnet_node vn,
  int (*fh)(artnet_node n, int port, void *d),
  void *data);

// send functions
EXTERN int artnet_send_poll(artnet_node n,
  const char *ip,
  artnet_ttm_value_t talk_to_me);
EXTERN int artnet_send_poll_reply(artnet_node n);
EXTERN int artnet_send_dmx(artnet_node n,
  int port_id,
  int16_t length,
  const uint8_t *data);
EXTERN int artnet_raw_send_dmx(artnet_node vn,
  uint8_t uni,
  int16_t length,
  const uint8_t *data);
EXTERN int artnet_send_address(artnet_node n,
  artnet_node_entry e,
  const char *shortName,
  const char *longName,
  uint8_t inAddr[ARTNET_MAX_PORTS],
  uint8_t outAddr[ARTNET_MAX_PORTS],
  uint8_t subAddr,
  artnet_port_command_t cmd);
EXTERN int artnet_send_input(artnet_node n,
  artnet_node_entry e,
  uint8_t settings[ARTNET_MAX_PORTS]);
EXTERN int artnet_send_firmware(artnet_node vn,
  artnet_node_entry e,
  int ubea,
  uint16_t *data,
  int length,
  int (*fh)(artnet_node n, artnet_firmware_status_code code, void *d),
  void *user_data);
EXTERN int artnet_send_firmware_reply(artnet_node vn,
  artnet_node_entry e,
  artnet_firmware_status_code code);

// rdm functions
EXTERN int artnet_send_tod_request(artnet_node vn);
EXTERN int artnet_send_tod_control(artnet_node vn,
  uint8_t address,
  artnet_tod_command_code action);
EXTERN int artnet_send_tod_data(artnet_node vn, int port);
EXTERN int artnet_send_rdm(artnet_node vn,
  uint8_t address,
  uint8_t *data,
  int length);
EXTERN int artnet_add_rdm_device(artnet_node vn,
  int port,
  uint8_t uid[ARTNET_RDM_UID_WIDTH]);
EXTERN int artnet_add_rdm_devices(artnet_node vn,
  int port,
  uint8_t *uid,
  int count);
EXTERN int artnet_remove_rdm_device(artnet_node vn,
  int port,
  uint8_t uid[ARTNET_RDM_UID_WIDTH]);

// recv functions
EXTERN uint8_t *artnet_read_dmx(artnet_node n, int port_id, int *length);

// state changing functions
EXTERN int artnet_set_node_type(artnet_node n, artnet_node_type type);
EXTERN int artnet_set_short_name(artnet_node vn, const char *name);
EXTERN int artnet_set_long_name(artnet_node n, const char *name);

//port manipulation functions
EXTERN int artnet_set_port_type(artnet_node n,
                                int id,
                                artnet_port_settings_t settings,
                                artnet_port_data_code data);
EXTERN int artnet_set_port_addr(artnet_node n,
                                int id,
                                artnet_port_dir_t dir,
                                uint8_t addr);
EXTERN int artnet_set_subnet_addr(artnet_node n, uint8_t subnet);
EXTERN int artnet_get_universe_addr(artnet_node n,
                                    int id,
                                    artnet_port_dir_t dir);

//node list functions
EXTERN artnet_node_list artnet_get_nl(artnet_node n);
EXTERN artnet_node_entry artnet_nl_first(artnet_node_list nl);
EXTERN artnet_node_entry artnet_nl_next(artnet_node_list nl);
EXTERN int artnet_nl_get_length(artnet_node_list nl);

// misc
EXTERN int artnet_dump_config(artnet_node n);
EXTERN int artnet_get_config(artnet_node n, artnet_node_config_t *config);
EXTERN artnet_socket_t artnet_get_sd(artnet_node n);
EXTERN int artnet_set_fdset(artnet_node vn, fd_set *fdset);

EXTERN char *artnet_strerror();

#ifdef __cplusplus
}
#endif

#endif
