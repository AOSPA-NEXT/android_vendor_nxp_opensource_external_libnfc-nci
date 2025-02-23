/******************************************************************************
 *
 *  Copyright (C) 2010-2014 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
/******************************************************************************
 *
 *  The original Work has been changed by NXP.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  Copyright 2018-2023 NXP
 *
 ******************************************************************************/
/******************************************************************************
 *
 *  This is the private interface file for the NFA HCI.
 *
 ******************************************************************************/
#ifndef NFA_HCI_INT_H
#define NFA_HCI_INT_H

#include <cstdint>
#include <string>
#include "nfa_ee_api.h"
#include "nfa_hci_api.h"
#include "nfa_sys.h"
extern uint8_t HCI_LOOPBACK_DEBUG;

/* NFA HCI DEBUG states */
#define NFA_HCI_DEBUG_ON 0x01
#define NFA_HCI_DEBUG_OFF 0x00
/*****************************************************************************
**  Constants and data types
*****************************************************************************/

#define NFA_HCI_HOST_ID_UICC0 0x02 /* Host ID for UICC 0 */
#if (NXP_EXTNS == TRUE)
/* HCI Host Type length */
#define NFA_HCI_HOST_TYPE_LEN 2

#define NFA_HCI_HOST_ID_DYNAMIC_HOST0 0x80 /*Host ID for prop dyn host 0*/
#define NFA_HCI_HOST_ID_PROP_HOST0 0xC0 /*Host ID for prop eSE 0*/
#define NFA_HCI_MAX_NUM_PROP_HOST 0x03  /* Number of prop EE supported */
#define NFA_HCI_MAX_NUM_DYNAMIC_HOST 0x03 /* Number of Dynamic EE supported */
/* Host ID for UICC 0 */
#define NFA_HCI_FIRST_UICC_HOST NFA_HCI_HOST_ID_UICC0
#define NFA_HCI_LAST_UICC_HOST \
  (NFA_HCI_HOST_ID_UICC0 + NFA_HCI_MAX_NUM_UICC_HOST - 1)
/* Host ID for PROP HOST 0 */
#define NFA_HCI_FIRST_PROP_HOST NFA_HCI_HOST_ID_PROP_HOST0
#define NFA_HCI_LAST_PROP_HOST \
  (NFA_HCI_HOST_ID_PROP_HOST0 + NFA_HCI_MAX_NUM_PROP_HOST - 1)
/* Host ID for DYN HOST 0 */
#define NFA_HCI_FIRST_DYNAMIC_HOST NFA_HCI_HOST_ID_DYNAMIC_HOST0
#define NFA_HCI_LAST_DYNAMIC_HOST \
  (NFA_HCI_HOST_ID_DYNAMIC_HOST0 + NFA_HCI_MAX_NUM_DYNAMIC_HOST - 1)
/* Host ID for eUICC1 */
#define NFA_HCI_EUICC1_HOST (NFA_HCI_FIRST_PROP_HOST + 1)
/* Host ID for eUICC2 */
#define NFA_HCI_EUICC2_HOST (NFA_HCI_FIRST_PROP_HOST + 2)

#define IS_PROP_EUICC_HOST(host_id) \
  ((NFA_HCI_FIRST_PROP_HOST < host_id) && (NFA_HCI_LAST_PROP_HOST >= host_id))

#define IS_PROP_HOST(host_id) \
  ((NFA_HCI_FIRST_PROP_HOST <= host_id) && (NFA_HCI_LAST_PROP_HOST >= host_id))
#define IS_DYNAMIC_HOST(host_id)              \
  ((NFA_HCI_FIRST_DYNAMIC_HOST <= host_id) && \
   (NFA_HCI_LAST_DYNAMIC_HOST >= host_id))
#define IS_VALID_HOST(host_id) \
  (IS_PROP_HOST(host_id) || IS_DYNAMIC_HOST(host_id))

/* Static pipes - ADMIN Pipe, Link Management pipe and Static APDU pipes */
#define NFA_HCI_MAX_NUM_STATIC_PIPES         (2)
#else
/* Maximum pipes that can be created on a generic pipe  */
#define NFA_MAX_PIPES_IN_GENERIC_GATE 0x0F
#endif
/* Lost host specific gate */
#define NFA_HCI_LAST_HOST_SPECIFIC_GATE 0xEF

#define NFA_HCI_SESSION_ID_LEN 8 /* HCI Session ID length */
#if(NXP_EXTNS == TRUE)
/*Default pipe ID for prop host*/
#define NFA_HCI_DEFAULT_ID_MANAGEMENT_PIPE 0x18
/*Default conn pipe ID for prop host eSE*/
#define NFA_HCI_CONN_ESE_PIPE 0x16
/*Default APDUpipe ID for prop host eSE*/
#define NFA_HCI_APDUESE_PIPE 0x19
/*Default APDUpipe ID for prop host eUICCs*/
#define NFA_HCI_APDU_EUICC_PIPE 0x27
/*Default conn pipe ID for prop host eUICC1*/
#define NFA_HCI_CONN_EUICC1_PIPE 0x2B
/*Default conn pipe ID for prop host eUICC2*/
#define NFA_HCI_CONN_EUICC2_PIPE 0x2F
#endif
/* HCI SW Version number                       */
#define NFA_HCI_VERSION_SW 0x090000
/* HCI HW Version number                       */
#define NFA_HCI_VERSION_HW 0x000000
#define NFA_HCI_VENDOR_NAME \
  "HCI" /* Vendor Name                                 */
/* Model ID                                    */
#define NFA_HCI_MODEL_ID 00
/* HCI Version                                 */
#define NFA_HCI_VERSION 90

/* NFA HCI states */
#define NFA_HCI_STATE_DISABLED 0x00 /* HCI is disabled  */
/* HCI performing Initialization sequence */
#define NFA_HCI_STATE_STARTUP 0x01
/* HCI is waiting for initialization of other host in the network */
#define NFA_HCI_STATE_WAIT_NETWK_ENABLE 0x02
/* HCI is waiting to handle api commands  */
#define NFA_HCI_STATE_IDLE 0x03
/* HCI is waiting for response to command sent */
#define NFA_HCI_STATE_WAIT_RSP 0x04
/* Removing all pipes prior to removing the gate */
#define NFA_HCI_STATE_REMOVE_GATE 0x05
/* Removing all pipes and gates prior to deregistering the app */
#define NFA_HCI_STATE_APP_DEREGISTER 0x06
#define NFA_HCI_STATE_RESTORE 0x07 /* HCI restore */
/* HCI is waiting for initialization of other host in the network after restore
 */
#define NFA_HCI_STATE_RESTORE_NETWK_ENABLE 0x08
#if(NXP_EXTNS == TRUE)
/*NFCEE Recovery codes*/

/*HCI_CLEAR_ALL_PIPE event received*/
#define NFCEE_HCI_NOTIFY_ALL_PIPE_CLEARED       0x01
/*NFCEE Unrecoverable status notification received*/
#define NFCEE_UNRECOVERABLE_ERRROR              0x10
/*NFCEE Discovery notification received*/
#define NFCEE_REINIT                            0x02
/*NFCEE removed notification received*/
#define NFCEE_REMOVED_NTF                       0x04
/*NFCEE Initialization completed status notification received*/
#define NFCEE_INIT_COMPLETED                    0x08
/*NFCEE unrecoverable error handling triggered*/
#define NFCEE_RECOVERY_IN_PROGRESS 0x20
#define NFA_HCI_MAX_RSP_WAIT_TIME 0x0C
/* After the reception of WTX, maximum response timeout value is 30 sec */
#define NFA_HCI_CHAIN_PKT_RSP_TIMEOUT 30000
/* Wait time to give response timeout to application if WTX not received*/
#define NFA_HCI_WTX_RESP_TIMEOUT 3000
/* time out for wired mode response after RF deativation */
#define NFA_HCI_DWP_RSP_WAIT_TIMEOUT 2000
/* time out for wired session aborted(0xE6 ntf) due to SWP switched to UICC*/
#define NFA_HCI_DWP_SESSION_ABORT_TIMEOUT 5000
/* delay between session ID poll to check if the reset host is initilized or not
 */
#define NFA_HCI_SESSION_ID_POLL_DELAY 50
/* retry count for session ID poll*/
#define NFA_HCI_MAX_SESSION_ID_RETRY_CNT 0x0A
/* NFCEE disc timeout default value in sec*/
#define NFA_HCI_NFCEE_DISC_TIMEOUT 0x02
/*  NXP specific events */
/* Event to read the number of NFCEE configured in NFCC*/
#define NFA_HCI_GET_NUM_NFCEE_CONFIGURED 0xF1
/* Event to read the session ID of all the Secure Element*/
#define NFA_HCI_READ_SESSIONID 0xF2
/* Event to start ETSI 12 configuration*/
#define NFA_HCI_INIT_NFCEE_CONFIG 0xF3
/* NFCEE ETSI 12 configuration complete*/
#define NFA_HCI_NFCEE_CONFIG_COMPLETE 0xF9

#define NFA_HCI_EVT_SW_PROC_LATENCY                                            \
  1000 /* Time taken for the event sent from ese / uicc HOST to reach Terminal \
          host and begin processing */

#define NF_HCI_PRINT_BUFF_SIZE 100
#endif

typedef uint8_t tNFA_HCI_STATE;

/* NFA HCI PIPE states */
#define NFA_HCI_PIPE_CLOSED 0x00 /* Pipe is closed */
#define NFA_HCI_PIPE_OPENED 0x01 /* Pipe is opened */

#define NFA_HCI_INVALID_INX 0xFF
#if(NXP_EXTNS == TRUE)
/* General purpose APDU Gate event definitions */
#define NFA_HCI_EVT_CMD_APDU                0x10

/* General purpose APDU App Gate event definitions */
#define NFA_HCI_EVT_RSP_APDU                0x10

#define NFA_HCI_INVALID_PIPE                0xFF

#define NFA_HCI_APP_HANDLE_NONE             0x00    /* for special gates that are allocated internally by NFA-HCI (without owner) */
#define NFA_HCI_APP_NAME_NONE               "NFA_HCI"
#endif
typedef uint8_t tNFA_HCI_COMMAND;
typedef uint8_t tNFA_HCI_RESPONSE;

/* NFA HCI Internal events */
enum {
  NFA_HCI_API_REGISTER_APP_EVT =
      NFA_SYS_EVT_START(NFA_ID_HCI), /* Register APP with HCI */
  NFA_HCI_API_DEREGISTER_APP_EVT,    /* Deregister an app from HCI */
  NFA_HCI_API_GET_APP_GATE_PIPE_EVT, /* Get the list of gate and pipe associated
                                        to the application */
  NFA_HCI_API_ALLOC_GATE_EVT, /* Allocate a dyanmic gate for the application */
  NFA_HCI_API_DEALLOC_GATE_EVT, /* Deallocate a previously allocated gate to the
                                   application */
  NFA_HCI_API_GET_HOST_LIST_EVT,   /* Get the list of Host in the network */
  NFA_HCI_API_GET_REGISTRY_EVT,    /* Get a registry entry from a host */
  NFA_HCI_API_SET_REGISTRY_EVT,    /* Set a registry entry on a host */
  NFA_HCI_API_CREATE_PIPE_EVT,     /* Create a pipe between two gates */
  NFA_HCI_API_OPEN_PIPE_EVT,       /* Open a pipe */
  NFA_HCI_API_CLOSE_PIPE_EVT,      /* Close a pipe */
  NFA_HCI_API_DELETE_PIPE_EVT,     /* Delete a pipe */
  NFA_HCI_API_ADD_STATIC_PIPE_EVT, /* Add a static pipe */
  NFA_HCI_API_SEND_CMD_EVT,        /* Send command via pipe */
  NFA_HCI_API_SEND_RSP_EVT,        /* Application Response to a command */
#if(NXP_EXTNS == TRUE)
  NFA_HCI_API_SEND_APDU_EVT,  /* Send Command APDU on APDU Pipe */
  NFA_HCI_API_ABORT_APDU_EVT, /* Abort the Command APDU sent on APDU Pipe */
#endif
  NFA_HCI_API_SEND_EVENT_EVT, /* Send event via pipe */
  NFA_HCI_RSP_NV_READ_EVT,    /* Non volatile read complete event */
  NFA_HCI_RSP_NV_WRITE_EVT,   /* Non volatile write complete event */
  NFA_HCI_RSP_TIMEOUT_EVT, /* Timeout to response for the HCP Command packet */
  NFA_HCI_CHECK_QUEUE_EVT
};

#define NFA_HCI_FIRST_API_EVENT NFA_HCI_API_REGISTER_APP_EVT
#define NFA_HCI_LAST_API_EVENT NFA_HCI_API_SEND_EVENT_EVT
#if(NXP_EXTNS == TRUE)
typedef uint16_t tNFA_HCI_INT_EVT;
#endif
/* Internal event structures.
**
** Note, every internal structure starts with a NFC_HDR and an app handle
*/

/* data type for NFA_HCI_API_REGISTER_APP_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  char app_name[NFA_MAX_HCI_APP_NAME_LEN + 1];
  tNFA_HCI_CBACK* p_cback;
  bool b_send_conn_evts;
} tNFA_HCI_API_REGISTER_APP;

/* data type for NFA_HCI_API_DEREGISTER_APP_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  char app_name[NFA_MAX_HCI_APP_NAME_LEN + 1];
} tNFA_HCI_API_DEREGISTER_APP;

/* data type for NFA_HCI_API_GET_APP_GATE_PIPE_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
} tNFA_HCI_API_GET_APP_GATE_PIPE;

/* data type for NFA_HCI_API_ALLOC_GATE_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  uint8_t gate;
} tNFA_HCI_API_ALLOC_GATE;

/* data type for NFA_HCI_API_DEALLOC_GATE_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  uint8_t gate;
} tNFA_HCI_API_DEALLOC_GATE;

/* data type for NFA_HCI_API_GET_HOST_LIST_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  tNFA_STATUS status;
} tNFA_HCI_API_GET_HOST_LIST;

/* data type for NFA_HCI_API_GET_REGISTRY_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  uint8_t pipe;
  uint8_t reg_inx;
} tNFA_HCI_API_GET_REGISTRY;

/* data type for NFA_HCI_API_SET_REGISTRY_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  uint8_t pipe;
  uint8_t reg_inx;
  uint8_t size;
  uint8_t data[NFA_MAX_HCI_CMD_LEN];
} tNFA_HCI_API_SET_REGISTRY;

/* data type for NFA_HCI_API_CREATE_PIPE_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  tNFA_STATUS status;
  uint8_t source_gate;
  uint8_t dest_host;
  uint8_t dest_gate;
} tNFA_HCI_API_CREATE_PIPE_EVT;

/* data type for NFA_HCI_API_OPEN_PIPE_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  tNFA_STATUS status;
  uint8_t pipe;
} tNFA_HCI_API_OPEN_PIPE_EVT;

/* data type for NFA_HCI_API_CLOSE_PIPE_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  tNFA_STATUS status;
  uint8_t pipe;
} tNFA_HCI_API_CLOSE_PIPE_EVT;

/* data type for NFA_HCI_API_DELETE_PIPE_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  tNFA_STATUS status;
  uint8_t pipe;
} tNFA_HCI_API_DELETE_PIPE_EVT;

/* data type for NFA_HCI_API_ADD_STATIC_PIPE_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  tNFA_STATUS status;
  uint8_t host;
  uint8_t gate;
  uint8_t pipe;
} tNFA_HCI_API_ADD_STATIC_PIPE_EVT;

/* data type for NFA_HCI_API_SEND_EVENT_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  uint8_t pipe;
  uint8_t evt_code;
  uint16_t evt_len;
  uint8_t* p_evt_buf;
  uint16_t rsp_len;
  uint8_t* p_rsp_buf;
  uint16_t rsp_timeout;
} tNFA_HCI_API_SEND_EVENT_EVT;

/* data type for NFA_HCI_API_SEND_CMD_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  uint8_t pipe;
  uint8_t cmd_code;
  uint16_t cmd_len;
  uint8_t data[NFA_MAX_HCI_CMD_LEN];
} tNFA_HCI_API_SEND_CMD_EVT;

/* data type for NFA_HCI_RSP_NV_READ_EVT */
typedef struct {
  NFC_HDR hdr;
  uint8_t block;
  uint16_t size;
  tNFA_STATUS status;
} tNFA_HCI_RSP_NV_READ_EVT;

/* data type for NFA_HCI_RSP_NV_WRITE_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_STATUS status;
} tNFA_HCI_RSP_NV_WRITE_EVT;

/* data type for NFA_HCI_API_SEND_RSP_EVT */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
  uint8_t pipe;
  uint8_t response;
  uint8_t size;
  uint8_t data[NFA_MAX_HCI_RSP_LEN];
} tNFA_HCI_API_SEND_RSP_EVT;

/* common data type for internal events */
typedef struct {
  NFC_HDR hdr;
  tNFA_HANDLE hci_handle;
} tNFA_HCI_COMM_DATA;

#if(NXP_EXTNS == TRUE)

#define NFA_HCI_FL_EE_ENABLING 0x01
#define NFA_HCI_FL_EE_ENABLED  0x02
#define NFA_HCI_FL_EE_NONE     0x00
/* Loopback gate control block */
typedef struct
{
  uint32_t              pipe_inx_mask;                  /* Bit 0 == pipe inx 0, etc */
} tNFA_HCI_LOOP_BACK_GATE_INFO;

/* Identity management gate control block */
typedef struct
{
  uint32_t              pipe_inx_mask;                  /* Bit 0 == pipe inx 0, etc */
} tNFA_HCI_ID_MNGMNT_GATE_INFO;

/* dest host control block */
typedef struct
{
    uint8_t                       num_gates;
    uint8_t                       gate_id[NFA_HCI_MAX_GATE_CB];
    uint8_t                       host_id;
    tNFA_HCI_APDU_PIPE_REG_INFO   apdu_pipe_reg_info;
} tNFA_HCI_HOST_INFO;

typedef struct
{
    uint8_t host_id;
    uint8_t reset_cfg;
}tNFA_HCI_RESET_HOST_INFO;

/* Admin pipe control block */
typedef struct
{
    tNFA_HCI_PIPE_STATE state;                              /* State of Pipe '01' */
    uint8_t               session_id[NFA_HCI_SESSION_ID_LEN]; /* Session ID of the host network */
} tNFA_HCI_ADMIN_PIPE_INFO;

/* Link management pipe control block */
typedef struct
{
    tNFA_HCI_PIPE_STATE state;                      /* State of Pipe '00' */
    uint16_t              rec_errors;                 /* Receive errors */
} tNFA_HCI_LINK_MGMNT_PIPE_INFO;

/* data type for NFA_HCI_API_SEND_APDU_EVT */
typedef struct
{
    NFC_HDR             hdr;
    tNFA_HANDLE         hci_handle;
    uint8_t             host_id;
    uint16_t            cmd_apdu_len;
    uint8_t             *p_cmd_apdu;
    uint16_t            rsp_apdu_buf_size;
    uint8_t             *p_rsp_apdu_buf;
    uint32_t             rsp_timeout;
} tNFA_HCI_API_SEND_APDU_EVT;

/* data type for tNFA_HCI_API_ABORT_APDU_EVT */
typedef struct
{
    NFC_HDR             hdr;
    tNFA_HANDLE         hci_handle;
    uint8_t             host_id;
    uint32_t             rsp_timeout;
} tNFA_HCI_API_ABORT_APDU_EVT;

/* HCP reassembly info on pipe */
typedef struct
{
    TIMER_LIST_ENT            rsp_timer;              /* Timer to avoid indefinitely waiting for event on the pipe */
    tNFA_HANDLE               pipe_user;              /* The app that is waiting for Rsp or Event on the pipe */
    uint8_t                   recv_inst_type;         /* Instruction type of incoming message on the pipe */
    uint8_t                   recv_inst;              /* Instruction of incoming message on the pipe */
    uint16_t                  msg_rx_len;             /* For segmentation - length of the combined message */
    uint16_t                  max_msg_rx_len;         /* Maximum reassembled incoming message size */
    bool                      reassembling;           /* Reassembling HCP on the pipe */
    bool                      reassembly_failed;      /* Reassembly of HCP failed on the pipe */
    bool                      w4_cmd_rsp;             /* Flag indicating if Waiting for response to command sent on pipe */
    bool                      w4_rsp_apdu_evt;        /* Flag indicating if Waiting for ETSI_HCI_EVT_R_APDU event on pipe */
    bool                      w4_atr_evt;             /* Flag indicating if Waiting for ETSI_HCI_EVT_ATR on the pipe */
    uint16_t                  rsp_buf_size;           /* Size of response buffer */
    uint8_t                   *p_rsp_buf;             /* Buffer to hold response for the event sent */
    uint8_t                   cmd_inst_sent;          /* Instruction of the command sent on the pipe */
    uint8_t                   cmd_inst_param_sent;    /* Instruction parameter of the command sent on the pipe */
    uint32_t                   rsp_timeout;
} tNFA_HCI_PIPE_CMDRSP_INFO;

#endif
/* union of all event data types */
typedef union {
  NFC_HDR hdr;
  tNFA_HCI_COMM_DATA comm;

  /* API events */
  tNFA_HCI_API_REGISTER_APP app_info; /* Register/Deregister an application */
  tNFA_HCI_API_GET_APP_GATE_PIPE get_gate_pipe_list; /* Get the list of gates
                                                        and pipes created for
                                                        the application */
  tNFA_HCI_API_ALLOC_GATE
      gate_info; /* Allocate a dynamic gate to the application */
  tNFA_HCI_API_DEALLOC_GATE
      gate_dealloc; /* Deallocate the gate allocated to the application */
  tNFA_HCI_API_CREATE_PIPE_EVT create_pipe;         /* Create a pipe */
  tNFA_HCI_API_OPEN_PIPE_EVT open_pipe;             /* Open a pipe */
  tNFA_HCI_API_CLOSE_PIPE_EVT close_pipe;           /* Close a pipe */
  tNFA_HCI_API_DELETE_PIPE_EVT delete_pipe;         /* Delete a pipe */
  tNFA_HCI_API_ADD_STATIC_PIPE_EVT add_static_pipe; /* Add a static pipe */
  tNFA_HCI_API_GET_HOST_LIST
      get_host_list; /* Get the list of Host in the network */
  tNFA_HCI_API_GET_REGISTRY get_registry; /* Get a registry entry on a host */
  tNFA_HCI_API_SET_REGISTRY set_registry; /* Set a registry entry on a host */
  tNFA_HCI_API_SEND_CMD_EVT send_cmd;     /* Send a event on a pipe to a host */
  tNFA_HCI_API_SEND_RSP_EVT
      send_rsp; /* Response to a command sent on a pipe to a host */
  tNFA_HCI_API_SEND_EVENT_EVT send_evt; /* Send a command on a pipe to a host */

  /* Internal events */
  tNFA_HCI_RSP_NV_READ_EVT nv_read;   /* Read Non volatile data */
  tNFA_HCI_RSP_NV_WRITE_EVT nv_write; /* Write Non volatile data */
#if(NXP_EXTNS == TRUE)
  tNFA_HCI_API_SEND_APDU_EVT          send_apdu;                      /* Send Command APDU on APDU pipe */
  tNFA_HCI_API_ABORT_APDU_EVT         abort_apdu;                     /* Abort the Command APDU sent on the pipe */
#endif
} tNFA_HCI_EVENT_DATA;

/*****************************************************************************
**  control block
*****************************************************************************/

/* Dynamic pipe control block */
typedef struct {
  uint8_t pipe_id;                /* Pipe ID */
  tNFA_HCI_PIPE_STATE pipe_state; /* State of the Pipe */
  uint8_t local_gate;             /* local gate id */
  uint8_t dest_host; /* Peer host to which this pipe is connected */
  uint8_t dest_gate; /* Peer gate to which this pipe is connected */
} tNFA_HCI_DYN_PIPE;

/* Dynamic gate control block */
typedef struct {
  uint8_t gate_id;        /* local gate id */
  tNFA_HANDLE gate_owner; /* NFA-HCI handle assigned to the application which
                             owns the gate */
  uint32_t pipe_inx_mask; /* Bit 0 == pipe inx 0, etc */
} tNFA_HCI_DYN_GATE;

/* Admin gate control block */
typedef struct {
  tNFA_HCI_PIPE_STATE pipe01_state; /* State of Pipe '01' */
  uint8_t
      session_id[NFA_HCI_SESSION_ID_LEN]; /* Session ID of the host network */
} tNFA_ADMIN_GATE_INFO;

/* Link management gate control block */
typedef struct {
  tNFA_HCI_PIPE_STATE pipe00_state; /* State of Pipe '00' */
  uint16_t rec_errors;              /* Receive errors */
} tNFA_LINK_MGMT_GATE_INFO;

/* Identity management gate control block */
typedef struct {
  uint32_t pipe_inx_mask;  /* Bit 0 == pipe inx 0, etc */
  uint16_t version_sw;     /* Software version number */
  uint16_t version_hw;     /* Hardware version number */
  uint8_t vendor_name[20]; /* Vendor name */
  uint8_t model_id;        /* Model ID */
  uint8_t hci_version;     /* HCI Version */
} tNFA_ID_MGMT_GATE_INFO;
#if(NXP_EXTNS == TRUE)
/* Internal flags */
/* sub system is being disabled */
#define NFA_HCI_FL_DISABLING 0x01
#define NFA_HCI_FL_NV_CHANGED 0x02 /* NV Ram changed */
#endif
/* NFA HCI control block */
typedef struct {
#if(NXP_EXTNS == TRUE)
  uint32_t m_wtx_count;
#endif
  tNFA_HCI_STATE hci_state; /* state of the HCI */
  uint8_t num_nfcee;        /* Number of NFCEE ID Discovered */
  tNFA_EE_INFO ee_info[NFA_HCI_MAX_HOST_IN_NETWORK]; /*NFCEE ID Info*/
  uint8_t num_ee_dis_req_ntf; /* Number of ee discovery request ntf received */
  uint8_t num_hot_plug_evts;  /* Number of Hot plug events received after ee
                                 discovery disable ntf */
#if(NXP_EXTNS == TRUE)
  tNFA_HCI_RESET_HOST_INFO reset_host[NFA_HCI_MAX_HOST_IN_NETWORK]; /* List of host reseting */
  uint8_t curr_nfcee;                              /* NFCEE to which terminal host is configuring */
  uint8_t next_nfcee_idx;                          /* Index of NFCEE which is configured*/
  uint8_t active_host[NFA_HCI_MAX_HOST_IN_NETWORK]; /* active host in the
                                                         host network */
  uint8_t se_apdu_gate_support;
  uint8_t uicc_etsi_support;
#else
  uint8_t inactive_host[NFA_HCI_MAX_HOST_IN_NETWORK]; /* Inactive host in the
                                                         host network */
  uint8_t reset_host[NFA_HCI_MAX_HOST_IN_NETWORK]; /* List of host reseting */
#endif
  bool b_low_power_mode;  /* Host controller in low power mode */
  bool b_hci_new_sessionId; /* Command sent to set a new session Id */
  bool b_hci_netwk_reset; /* Command sent to reset HCI Network */
  bool w4_hci_netwk_init; /* Wait for other host in network to initialize */
  TIMER_LIST_ENT timer;   /* Timer to avoid indefinitely waiting for response */
  uint8_t conn_id;        /* Connection ID */
  uint8_t buff_size;      /* Connection buffer size */
  bool nv_read_cmplt;     /* NV Read completed */
  bool nv_write_needed;   /* Something changed - NV write is needed */
  bool assembling;        /* Set true if in process of assembling a message  */
  bool assembly_failed;   /* Set true if Insufficient buffer to Reassemble
                             incoming message */
  bool w4_rsp_evt;        /* Application command sent on HCP Event */
  tNFA_HANDLE
      app_in_use; /* Index of the application that is waiting for response */
  uint8_t local_gate_in_use;  /* Local gate currently working with */
  uint8_t remote_gate_in_use; /* Remote gate currently working with */
  uint8_t remote_host_in_use; /* The remote host to which a command is sent */
  uint8_t pipe_in_use;        /* The pipe currently working with */
  uint8_t param_in_use;       /* The registry parameter currently working with */
  tNFA_HCI_COMMAND cmd_sent;  /* The last command sent */
  bool ee_disc_cmplt;         /* EE Discovery operation completed */
  bool ee_disable_disc;       /* EE Discovery operation is disabled */
#if(NXP_EXTNS == TRUE)
  tNFA_HCI_EVENT_SENT evt_sent;
  tNFA_HCI_PIPE_CMDRSP_INFO       dyn_pipe_cmdrsp_info[NFA_HCI_MAX_PIPE_CB]; /* Reassembly buffer and reassembly state info for dynamic pipes */
  uint8_t                         static_pipe[NFA_HCI_MAX_NUM_STATIC_PIPES]; /* Static pipes, Proprietary static pipes */
  tNFA_HCI_PIPE_CMDRSP_INFO       static_pipe_cmdrsp_info[NFA_HCI_MAX_NUM_STATIC_PIPES]; /* Reassembly buffer and reassembly state info for static pipe */
  uint8_t                         app_param[NFA_HCI_MAX_APP_CB + 1];
  tNFA_HCI_ADMIN_PIPE_INFO        admin_pipe;
  tNFA_HCI_LINK_MGMNT_PIPE_INFO   link_mgmnt_pipe;
  tNFA_HCI_LOOP_BACK_GATE_INFO    loop_back_gate;
  tNFA_HCI_ID_MNGMNT_GATE_INFO    id_mngmnt_gate;
#endif
  uint16_t msg_len;     /* For segmentation - length of the combined message */
  uint16_t max_msg_len; /* Maximum reassembled message size */
  uint8_t msg_data[NFA_MAX_HCI_EVENT_LEN]; /* For segmentation - the combined
                                              message data */
  uint8_t* p_msg_data; /* For segmentation - reassembled message */
  uint8_t type;        /* Instruction type of incoming message */
  uint8_t inst;        /* Instruction of incoming message */

  BUFFER_Q hci_api_q;            /* Buffer Q to hold incoming API commands */
  BUFFER_Q hci_host_reset_api_q; /* Buffer Q to hold incoming API commands to a
                                    host that is reactivating */
  tNFA_HCI_CBACK* p_app_cback[NFA_HCI_MAX_APP_CB]; /* Callback functions
                                                      registered by the
                                                      applications */
  uint16_t rsp_buf_size; /* Maximum size of APDU buffer */
  uint8_t* p_rsp_buf;    /* Buffer to hold response to sent event */
  struct                 /* Persistent information for Device Host */
  {
    char reg_app_names[NFA_HCI_MAX_APP_CB][NFA_MAX_HCI_APP_NAME_LEN + 1];

    tNFA_HCI_DYN_GATE dyn_gates[NFA_HCI_MAX_GATE_CB];
    tNFA_HCI_DYN_PIPE dyn_pipes[NFA_HCI_MAX_PIPE_CB];
#if(NXP_EXTNS == TRUE)
    tNFA_HCI_HOST_INFO host[NFA_HCI_MAX_HOST_IN_NETWORK];
#endif
    bool b_send_conn_evts[NFA_HCI_MAX_APP_CB];
    tNFA_ADMIN_GATE_INFO admin_gate;
    tNFA_LINK_MGMT_GATE_INFO link_mgmt_gate;
    tNFA_ID_MGMT_GATE_INFO id_mgmt_gate;
  } cfg;

} tNFA_HCI_CB;

/*****************************************************************************
**  External variables
*****************************************************************************/

/* NFA HCI control block */
extern tNFA_HCI_CB nfa_hci_cb;

/*****************************************************************************
**  External functions
*****************************************************************************/

/* Functions in nfa_hci_main.c
*/
extern void nfa_hci_init(void);
extern void nfa_hci_proc_nfcc_power_mode(uint8_t nfcc_power_mode);
extern void nfa_hci_dh_startup_complete(void);
extern void nfa_hci_startup_complete(tNFA_STATUS status);
extern void nfa_hci_startup(void);
extern void nfa_hci_restore_default_config(uint8_t* p_session_id);
#if(NXP_EXTNS == TRUE)
extern bool nfa_hci_enable_one_nfcee(void);
extern void nfa_hci_release_transceive(uint8_t host_id, uint8_t status);
#else
extern void nfa_hci_enable_one_nfcee(void);
#endif
/* Action functions in nfa_hci_act.c
*/
extern void nfa_hci_check_pending_api_requests(void);
extern void nfa_hci_check_api_requests(void);
extern void nfa_hci_handle_admin_gate_cmd(uint8_t* p_data, uint16_t data_len);
extern void nfa_hci_handle_admin_gate_rsp(uint8_t* p_data, uint8_t data_len);
extern void nfa_hci_handle_admin_gate_evt();
extern void nfa_hci_handle_link_mgm_gate_cmd(uint8_t* p_data,
                                             uint16_t data_len);
extern void nfa_hci_handle_dyn_pipe_pkt(uint8_t pipe, uint8_t* p_data,
                                        uint16_t data_len);
extern void nfa_hci_handle_pipe_open_close_cmd(tNFA_HCI_DYN_PIPE* p_pipe);
extern void nfa_hci_api_dealloc_gate(tNFA_HCI_EVENT_DATA* p_evt_data);
extern void nfa_hci_api_deregister(tNFA_HCI_EVENT_DATA* p_evt_data);

/* Utility functions in nfa_hci_utils.c
*/
extern tNFA_HCI_DYN_GATE* nfa_hciu_alloc_gate(uint8_t gate_id,
                                              tNFA_HANDLE app_handle);
extern tNFA_HCI_DYN_GATE* nfa_hciu_find_gate_by_gid(uint8_t gate_id);
extern tNFA_HCI_DYN_GATE* nfa_hciu_find_gate_by_owner(tNFA_HANDLE app_handle);
extern tNFA_HCI_DYN_GATE* nfa_hciu_find_gate_with_nopipes_by_owner(
    tNFA_HANDLE app_handle);
extern tNFA_HCI_DYN_PIPE* nfa_hciu_find_pipe_by_pid(uint8_t pipe_id);
extern tNFA_HCI_DYN_PIPE* nfa_hciu_find_pipe_by_owner(tNFA_HANDLE app_handle);
extern tNFA_HCI_DYN_PIPE* nfa_hciu_find_active_pipe_by_owner(
    tNFA_HANDLE app_handle);
extern tNFA_HCI_DYN_PIPE* nfa_hciu_find_pipe_on_gate(uint8_t gate_id);
extern tNFA_HANDLE nfa_hciu_get_gate_owner(uint8_t gate_id);
extern bool nfa_hciu_check_pipe_between_gates(uint8_t local_gate,
                                              uint8_t dest_host,
                                              uint8_t dest_gate);
extern bool nfa_hciu_is_active_host(uint8_t host_id);
extern bool nfa_hciu_is_host_reseting(uint8_t host_id);
extern bool nfa_hciu_is_no_host_resetting(void);
extern tNFA_HCI_DYN_PIPE* nfa_hciu_find_active_pipe_on_gate(uint8_t gate_id);
extern tNFA_HANDLE nfa_hciu_get_pipe_owner(uint8_t pipe_id);
extern uint8_t nfa_hciu_count_open_pipes_on_gate(tNFA_HCI_DYN_GATE* p_gate);
extern uint8_t nfa_hciu_count_pipes_on_gate(tNFA_HCI_DYN_GATE* p_gate);
#if(NXP_EXTNS == TRUE)
extern tNFA_STATUS nfa_hciu_asmbl_dyn_pipe_pkt(uint8_t* p_data,
                                               uint8_t data_len);
#endif
extern tNFA_HCI_RESPONSE nfa_hciu_add_pipe_to_gate(uint8_t pipe,
                                                   uint8_t local_gate,
                                                   uint8_t dest_host,
                                                   uint8_t dest_gate);
extern tNFA_HCI_RESPONSE nfa_hciu_add_pipe_to_static_gate(uint8_t local_gate,
                                                          uint8_t pipe_id,
                                                          uint8_t dest_host,
                                                          uint8_t dest_gate);

extern tNFA_HCI_RESPONSE nfa_hciu_release_pipe(uint8_t pipe_id);
extern void nfa_hciu_release_gate(uint8_t gate);
extern void nfa_hciu_remove_all_pipes_from_host(uint8_t host);
extern uint8_t nfa_hciu_get_allocated_gate_list(uint8_t* p_gate_list);

extern void nfa_hciu_send_to_app(tNFA_HCI_EVT event, tNFA_HCI_EVT_DATA* p_evt,
                                 tNFA_HANDLE app_handle);
extern void nfa_hciu_send_to_all_apps(tNFA_HCI_EVT event,
                                      tNFA_HCI_EVT_DATA* p_evt);
extern void nfa_hciu_send_to_apps_handling_connectivity_evts(
    tNFA_HCI_EVT event, tNFA_HCI_EVT_DATA* p_evt);

extern tNFA_STATUS nfa_hciu_send_close_pipe_cmd(uint8_t pipe);
extern tNFA_STATUS nfa_hciu_send_delete_pipe_cmd(uint8_t pipe);
extern tNFA_STATUS nfa_hciu_send_clear_all_pipe_cmd(void);
extern tNFA_STATUS nfa_hciu_send_open_pipe_cmd(uint8_t pipe);
extern tNFA_STATUS nfa_hciu_send_get_param_cmd(uint8_t pipe, uint8_t index);
extern tNFA_STATUS nfa_hciu_send_create_pipe_cmd(uint8_t source_gate,
                                                 uint8_t dest_host,
                                                 uint8_t dest_gate);
extern tNFA_STATUS nfa_hciu_send_set_param_cmd(uint8_t pipe, uint8_t index,
                                               uint8_t length, uint8_t* p_data);
extern tNFA_STATUS nfa_hciu_send_msg(uint8_t pipe_id, uint8_t type,
                                     uint8_t instruction, uint16_t pkt_len,
                                     uint8_t* p_pkt);

extern std::string nfa_hciu_instr_2_str(uint8_t type);
extern std::string nfa_hciu_get_event_name(uint16_t event);
extern std::string nfa_hciu_get_state_name(uint8_t state);
extern char* nfa_hciu_get_type_inst_names(uint8_t pipe, uint8_t type,
                                          uint8_t inst, char* p_buff,
                                          const uint8_t max_buff_size);
extern std::string nfa_hciu_evt_2_str(uint8_t pipe_id, uint8_t evt);
#if(NXP_EXTNS == TRUE)
extern tNFA_STATUS nfa_hciu_send_raw_cmd(uint8_t param_len, uint8_t* p_data,
                                         tNFA_VSC_CBACK* p_cback);
extern void fa_hciu_set_server_apdu_host_not_ready (tNFA_HCI_DYN_GATE *p_gate);
extern void nfa_hciu_reset_gate_list_of_host (uint8_t host_id);
extern void nfa_hciu_update_gate_list_of_host (uint8_t host_id, uint8_t num_gates, uint8_t *p_gates);
extern void nfa_hciu_reset_apdu_pipe_registry_info_of_host (uint8_t host_id);
extern tNFA_HCI_DYN_PIPE  *nfa_hciu_find_id_pipe_for_host (uint8_t host_id);
extern tNFA_HCI_DYN_PIPE  *nfa_hciu_find_dyn_apdu_pipe_for_host (uint8_t host_id);
extern tNFA_HCI_DYN_PIPE  *nfa_hciu_find_dyn_conn_pipe_for_host (uint8_t host_id);
extern tNFA_HCI_PIPE_CMDRSP_INFO *nfa_hciu_get_pipe_cmdrsp_info (uint8_t pipe);
extern uint8_t  nfa_hciu_find_server_apdu_gate_for_host (uint8_t host_id);
extern tNFA_HCI_APDU_PIPE_REG_INFO *nfa_hciu_find_apdu_pipe_registry_info_for_host (uint8_t host_id);
extern bool nfa_hci_check_set_apdu_pipe_ready_for_next_host ();
extern void nfa_hciu_set_server_apdu_host_not_ready (tNFA_HCI_DYN_GATE *p_gate);
extern void nfa_hciu_update_host_list (uint8_t data_len, uint8_t *p_host_list);
extern void nfa_hciu_add_host_resetting(uint8_t host_id, uint8_t reset_cfg);
extern void nfa_hciu_clear_host_resetting(uint8_t host_id, uint8_t reset_cfg);
extern void nfa_hci_handle_pending_host_reset();
extern void nfa_hci_handle_control_evt(tNFC_CONN_EVT event,tNFC_CONN* p_data);
extern bool nfa_hciu_check_host_resetting(uint8_t host_id, uint8_t reset_type);
extern void nfa_hciu_check_n_clear_host_resetting(uint8_t host_id, uint8_t reset_type);
#endif
#define VERBOSE_BUFF_SIZE 100
#endif /* NFA_HCI_INT_H */
