/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
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
 *  Copyright 2019-2020, 2023 NXP
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
 *
 ******************************************************************************/
#ifndef NFC_TARGET_H
#define NFC_TARGET_H

#include "data_types.h"

#ifdef BUILDCFG
#include "buildcfg.h"
#endif

/* Include common GKI definitions used by this platform */
#include "bt_types.h" /* This must be defined AFTER buildcfg.h */
#include "gki_target.h"

#ifndef USERIAL_DEBUG
#define USERIAL_DEBUG FALSE
#endif

/******************************************************************************
**
** GKI Mail Box and Timer
**
******************************************************************************/

/* Mailbox event mask for NFC stack */
#ifndef NFC_MBOX_EVT_MASK
#define NFC_MBOX_EVT_MASK (TASK_MBOX_0_EVT_MASK)
#endif

/* Mailbox ID for NFC stack */
#ifndef NFC_MBOX_ID
#define NFC_MBOX_ID (TASK_MBOX_0)
#endif

/* Mailbox event mask for NFA */
#ifndef NFA_MBOX_EVT_MASK
#define NFA_MBOX_EVT_MASK (TASK_MBOX_2_EVT_MASK)
#endif

/* Mailbox ID for NFA */
#ifndef NFA_MBOX_ID
#define NFA_MBOX_ID (TASK_MBOX_2)
#endif

/* GKI timer id used for protocol timer in NFC stack */
#ifndef NFC_TIMER_ID
#define NFC_TIMER_ID (TIMER_0)
#endif

/* GKI timer event mask used for protocol timer in NFC stack */
#ifndef NFC_TIMER_EVT_MASK
#define NFC_TIMER_EVT_MASK (TIMER_0_EVT_MASK)
#endif

/* GKI timer id used for quick timer in NFC stack */
#ifndef NFC_QUICK_TIMER_ID
#define NFC_QUICK_TIMER_ID (TIMER_1)
#endif

/* GKI timer event mask used for quick timer in NFC stack */
#ifndef NFC_QUICK_TIMER_EVT_MASK
#define NFC_QUICK_TIMER_EVT_MASK (TIMER_1_EVT_MASK)
#endif

/* GKI timer id used for protocol timer in NFA */
#ifndef NFA_TIMER_ID
#define NFA_TIMER_ID (TIMER_2)
#endif

/* GKI timer event mask used for protocol timer in NFA */
#ifndef NFA_TIMER_EVT_MASK
#define NFA_TIMER_EVT_MASK (TIMER_2_EVT_MASK)
#endif

/******************************************************************************
**
** GKI Buffer Pools
**
******************************************************************************/

/* NCI command/notification/data */
#ifndef NFC_NCI_POOL_ID
#define NFC_NCI_POOL_ID GKI_POOL_ID_2
#endif

#ifndef NFC_NCI_POOL_BUF_SIZE
#define NFC_NCI_POOL_BUF_SIZE GKI_BUF2_SIZE
#endif

/* Reader/Write commands (NCI data payload) */
#ifndef NFC_RW_POOL_ID
#define NFC_RW_POOL_ID GKI_POOL_ID_2
#endif

#ifndef NFC_RW_POOL_BUF_SIZE
#define NFC_RW_POOL_BUF_SIZE GKI_BUF2_SIZE
#endif

/* Card Emulation responses (NCI data payload) */
#ifndef NFC_CE_POOL_ID
#define NFC_CE_POOL_ID GKI_POOL_ID_2
#endif

#ifndef NFC_CE_POOL_BUF_SIZE
#define NFC_CE_POOL_BUF_SIZE GKI_BUF2_SIZE
#endif

/* NCI msg pool for HAL (for shared NFC/HAL GKI)*/
#ifndef NFC_HAL_NCI_POOL_ID
#define NFC_HAL_NCI_POOL_ID NFC_NCI_POOL_ID
#endif

#ifndef NFC_HAL_NCI_POOL_BUF_SIZE
#define NFC_HAL_NCI_POOL_BUF_SIZE NFC_NCI_POOL_BUF_SIZE
#endif

#if (NXP_EXTNS == TRUE)
/* WIRED Mode pool */
#ifndef NFC_WIRED_POOL_ID
#define NFC_WIRED_POOL_ID GKI_POOL_ID_4
#endif

#ifndef NFC_WIRED_POOL_SIZE
#define NFC_WIRED_POOL_SIZE GKI_BUF4_SIZE
#endif

#endif
/******************************************************************************
**
** NCI Transport definitions
**
******************************************************************************/
/* offset of the first NCI packet in buffer for outgoing */
#ifndef NCI_MSG_OFFSET_SIZE
#define NCI_MSG_OFFSET_SIZE 1
#endif

/* Restore NFCC baud rate to default on shutdown if NFC_UpdateBaudRate was
 * called */
#ifndef NFC_RESTORE_BAUD_ON_SHUTDOWN
#define NFC_RESTORE_BAUD_ON_SHUTDOWN TRUE
#endif

/******************************************************************************
**
** NCI
**
******************************************************************************/
#define NCI_VERSION_UNKNOWN 0x00
#define NCI_VERSION_1_0 0x10
#define NCI_VERSION_2_0 0x20
#ifndef NCI_VERSION
#define NCI_VERSION NCI_VERSION_2_0
#endif
#define NCI_CORE_RESET_RSP_LEN(X) (((X) >= NCI_VERSION_2_0) ? (0x01) : (0x03))

/* TRUE I2C patch is needed */
#ifndef NFC_I2C_PATCH_INCLUDED
#define NFC_I2C_PATCH_INCLUDED TRUE /* NFC-Android uses this!!! */
#endif

/******************************************************************************
**
** NFC
**
******************************************************************************/

/* Define to TRUE to include Broadcom Vendor Specific implementation */
#ifndef NFC_BRCM_VS_INCLUDED
#define NFC_BRCM_VS_INCLUDED TRUE
#endif

/* Define to TRUE if compling for NFC Reader/Writer Only mode */
#ifndef NFC_RW_ONLY
#define NFC_RW_ONLY FALSE
#endif

/* Timeout for receiving response to NCI command */
#ifndef NFC_CMD_CMPL_TIMEOUT
#define NFC_CMD_CMPL_TIMEOUT 2
#endif

/* Timeout for waiting on data credit/NFC-DEP */
#ifndef NFC_DEACTIVATE_TIMEOUT
#define NFC_DEACTIVATE_TIMEOUT 2
#endif

/* the maximum number of Vendor Specific callback functions allowed to be
 * registered. 1-14 */
#ifndef NFC_NUM_VS_CBACKS
#define NFC_NUM_VS_CBACKS 3
#endif

/* the maximum number of NCI connections allowed. 1-14 */
#ifndef NCI_MAX_CONN_CBS
#define NCI_MAX_CONN_CBS 4
#endif

/* Maximum number of NCI commands that the NFCC accepts without needing to wait
 * for response */
#ifndef NCI_MAX_CMD_WINDOW
#define NCI_MAX_CMD_WINDOW 1
#endif

/* Define to TRUE to include the NFCEE related functionalities */
#ifndef NFC_NFCEE_INCLUDED
#define NFC_NFCEE_INCLUDED TRUE
#endif

/* the maximum number of NFCEE interface supported */
#ifndef NFC_MAX_EE_INTERFACE
#define NFC_MAX_EE_INTERFACE 3
#endif

/* the maximum number of NFCEE information supported. */
#ifndef NFC_MAX_EE_INFO
#define NFC_MAX_EE_INFO 8
#endif

/* the maximum number of NFCEE TLVs supported */
#ifndef NFC_MAX_EE_TLVS
#define NFC_MAX_EE_TLVS 1
#endif

/* the maximum size of NFCEE TLV list supported */
#ifndef NFC_MAX_EE_TLV_SIZE
#define NFC_MAX_EE_TLV_SIZE 150
#endif

/* Maximum time to discover NFCEE */
#ifndef NFA_EE_DISCV_TIMEOUT_VAL
#define NFA_EE_DISCV_TIMEOUT_VAL 3000
#endif

/* Number of times reader/writer should attempt to resend a command on failure
 */
#ifndef RW_MAX_RETRIES
#define RW_MAX_RETRIES 5
#endif

/* RW NDEF Support */
#ifndef RW_NDEF_INCLUDED
#define RW_NDEF_INCLUDED TRUE
#endif

/* RW Type 1 Tag timeout for each API call, in ms */
#ifndef RW_T1T_TOUT_RESP
#define RW_T1T_TOUT_RESP 100
#endif

/* CE Type 2 Tag timeout for controller command, in ms */
#ifndef CE_T2T_TOUT_RESP
#define CE_T2T_TOUT_RESP 1000
#endif

/* RW Type 2 Tag timeout for each API call, in ms */
#ifndef RW_T2T_TOUT_RESP
/* Android requires 150 instead of 100 for presence-check*/
#define RW_T2T_TOUT_RESP 150
#endif

/* RW Type 2 Tag timeout for each API call, in ms */
#ifndef RW_T2T_SEC_SEL_TOUT_RESP
#define RW_T2T_SEC_SEL_TOUT_RESP 10
#endif

/* RW Type 3 Tag timeout for each API call, in ms */
#ifndef RW_T3T_TOUT_RESP
/* NFC-Android will use 100 instead of 75 for T3t presence-check */
#define RW_T3T_TOUT_RESP 100
#endif

/* CE Type 3 Tag maximum response timeout index (for check and update, used in
 * SENSF_RES) */
#ifndef CE_T3T_MRTI_C
#define CE_T3T_MRTI_C 0xFF
#endif
#ifndef CE_T3T_MRTI_U
#define CE_T3T_MRTI_U 0xFF
#endif

/* Default maxblocks for CE_T3T UPDATE/CHECK operations */
#ifndef CE_T3T_DEFAULT_UPDATE_MAXBLOCKS
#define CE_T3T_DEFAULT_UPDATE_MAXBLOCKS 3
#endif

#ifndef CE_T3T_DEFAULT_CHECK_MAXBLOCKS
#define CE_T3T_DEFAULT_CHECK_MAXBLOCKS 3
#endif

/* CE Type 4 Tag, Frame Waiting time Integer */
#ifndef CE_T4T_ISO_DEP_FWI
#define CE_T4T_ISO_DEP_FWI 7
#endif

/* RW Type 4 Tag timeout for each API call, in ms */
#ifndef RW_T4T_TOUT_RESP
#define RW_T4T_TOUT_RESP 1000
#endif

/* CE Type 4 Tag timeout for update file, in ms */
#ifndef CE_T4T_TOUT_UPDATE
#define CE_T4T_TOUT_UPDATE 1000
#endif

/* CE Type 4 Tag, mandatory NDEF File ID */
#ifndef CE_T4T_MANDATORY_NDEF_FILE_ID
#define CE_T4T_MANDATORY_NDEF_FILE_ID 0x1000
#endif

/* CE Type 4 Tag, max number of AID supported */
#ifndef CE_T4T_MAX_REG_AID
#define CE_T4T_MAX_REG_AID 4
#endif

/* Sub carrier */
#ifndef RW_I93_FLAG_SUB_CARRIER
#define RW_I93_FLAG_SUB_CARRIER I93_FLAG_SUB_CARRIER_SINGLE
#endif

/* Data rate for 15693 command/response */
#ifndef RW_I93_FLAG_DATA_RATE
#define RW_I93_FLAG_DATA_RATE I93_FLAG_DATA_RATE_HIGH
#endif

/* RW Mifare Classic Tag timeout for each API call, in ms */
#ifndef RW_MFC_TOUT_RESP
#define RW_MFC_TOUT_RESP 300
#endif

/* TRUE, to include Card Emulation related test commands */
#ifndef CE_TEST_INCLUDED
#define CE_TEST_INCLUDED FALSE
#endif

/* Quick Timer */
#ifndef QUICK_TIMER_TICKS_PER_SEC
#define QUICK_TIMER_TICKS_PER_SEC 100 /* 10ms timer */
#endif

/******************************************************************************
**
** NFA
**
******************************************************************************/
#if (NXP_EXTNS == TRUE)
/* Maximum number of AID entries per target_handle  */
#ifndef NFA_EE_MAX_AID_ENTRIES
#define NFA_EE_MIN_AID_SIZE (5)
#define NFA_EE_MIN_AID_ENTRY_SIZE (NFA_EE_MIN_AID_SIZE + 4)
#define NFA_EE_MAX_AID_ENTRIES (51)
#endif
#endif

/* Maximum Idle time (no hcp) to wait for EE DISC REQ Ntf(s) */
#ifndef NFA_HCI_NETWK_INIT_IDLE_TIMEOUT
#define NFA_HCI_NETWK_INIT_IDLE_TIMEOUT 1000
#endif

#ifndef NFA_HCI_MAX_HOST_IN_NETWORK
#define NFA_HCI_MAX_HOST_IN_NETWORK 0x06
#endif

/* Max number of Application that can be registered to NFA-HCI */
#ifndef NFA_HCI_MAX_APP_CB
#define NFA_HCI_MAX_APP_CB 0x05
#endif

/* Max number of HCI gates that can be created */
#ifndef NFA_HCI_MAX_GATE_CB
#define NFA_HCI_MAX_GATE_CB 0x06
#endif

/* Max number of HCI pipes that can be created for the whole system */
#ifndef NFA_HCI_MAX_PIPE_CB
#define NFA_HCI_MAX_PIPE_CB 0x0A
#endif

/* Timeout for waiting for the response to HCP Command packet */
#ifndef NFA_HCI_RESPONSE_TIMEOUT
#define NFA_HCI_RESPONSE_TIMEOUT 1000
#endif

/* Default poll duration (may be over-ridden using NFA_SetRfDiscoveryDuration)
 */
#ifndef NFA_DM_DISC_DURATION_POLL
#define NFA_DM_DISC_DURATION_POLL 500 /* Android requires 500 */
#endif

/* Automatic NDEF detection (when not in exclusive RF mode) */
#ifndef NFA_DM_AUTO_DETECT_NDEF
#define NFA_DM_AUTO_DETECT_NDEF FALSE /* !!!!! NFC-Android needs FALSE */
#endif

/* Automatic NDEF read (when not in exclusive RF mode) */
#ifndef NFA_DM_AUTO_READ_NDEF
#define NFA_DM_AUTO_READ_NDEF FALSE /* !!!!! NFC-Android needs FALSE */
#endif

/* Automatic NDEF presence check (when not in exclusive RF mode) */
#ifndef NFA_DM_AUTO_PRESENCE_CHECK
#define NFA_DM_AUTO_PRESENCE_CHECK FALSE /* Android requires FALSE */
#endif

/* Presence check option: 0x01: use sleep/wake for none-NDEF ISO-DEP tags */
#ifndef NFA_DM_PRESENCE_CHECK_OPTION
/* !!!!! Android needs value 3 */
#define NFA_DM_PRESENCE_CHECK_OPTION 0x03
#endif

/* Maximum time to wait for presence check response */
#ifndef NFA_DM_MAX_PRESENCE_CHECK_TIMEOUT
#define NFA_DM_MAX_PRESENCE_CHECK_TIMEOUT 500
#endif

/* Default delay to auto presence check after sending raw frame */
#ifndef NFA_DM_DEFAULT_PRESENCE_CHECK_START_DELAY
#define NFA_DM_DEFAULT_PRESENCE_CHECK_START_DELAY 750
#endif

/* Timeout for reactivation of Kovio bar code tag (presence check) */
#ifndef NFA_DM_DISC_TIMEOUT_KOVIO_PRESENCE_CHECK
#define NFA_DM_DISC_TIMEOUT_KOVIO_PRESENCE_CHECK (1000)
#endif

/* Max number of NDEF type handlers that can be registered (including the
 * default handler) */
#ifndef NFA_NDEF_MAX_HANDLERS
#define NFA_NDEF_MAX_HANDLERS 8
#endif

/* Maximum number of listen entries configured/registered with
 * NFA_CeConfigureUiccListenTech, */
/* NFA_CeRegisterFelicaSystemCodeOnDH, or NFA_CeRegisterT4tAidOnDH */
#ifndef NFA_CE_LISTEN_INFO_MAX
#define NFA_CE_LISTEN_INFO_MAX 6
#endif

#ifndef NFA_SNEP_INCLUDED
/* Android must use FALSE to exclude SNEP */
#define NFA_SNEP_INCLUDED FALSE
#endif

/* Max acceptable length */
#ifndef NFA_SNEP_DEFAULT_SERVER_MAX_NDEF_SIZE
#define NFA_SNEP_DEFAULT_SERVER_MAX_NDEF_SIZE 500000
#endif

/* Max number of SNEP server/client and data link connection */
#ifndef NFA_SNEP_MAX_CONN
#define NFA_SNEP_MAX_CONN 6
#endif

/* Max number data link connection of SNEP default server*/
#ifndef NFA_SNEP_DEFAULT_MAX_CONN
#define NFA_SNEP_DEFAULT_MAX_CONN 3
#endif

/* MIU for SNEP              */
#ifndef NFA_SNEP_MIU
#define NFA_SNEP_MIU 1980 /* Modified for NFC-A */
#endif

/* Receiving Window for SNEP */
#ifndef NFA_SNEP_RW
#define NFA_SNEP_RW 2 /* Modified for NFC-A */
#endif

/* Max number of NFCEE supported */
#ifndef NFA_EE_MAX_EE_SUPPORTED
/* Modified for NFC-A until we add dynamic support */
/*Including T4T NFCEE by incrementing 1*/
#define NFA_EE_MAX_EE_SUPPORTED 6
#endif

/* Maximum number of AID entries per target_handle  */
#ifndef NFA_EE_MAX_AID_ENTRIES
#define NFA_EE_MAX_AID_ENTRIES (32)
#endif
#if (NXP_EXTNS == TRUE)
/* Maximum number of AID entries per target_handle  */
#ifndef NFA_EE_MAX_APDU_PATTERN_ENTRIES
#define NFA_EE_MAX_APDU_PATTERN_ENTRIES (5)
#endif
#endif
/* Maximum number of callback functions can be registered through
 * NFA_EeRegister() */
#ifndef NFA_EE_MAX_CBACKS
#define NFA_EE_MAX_CBACKS (3)
#endif

#ifndef NFA_DTA_INCLUDED
#define NFA_DTA_INCLUDED TRUE
#endif

/*****************************************************************************
**  Define HAL_WRITE depending on whether HAL is using shared GKI resources
**  as the NFC stack.
*****************************************************************************/
#ifndef HAL_WRITE
#define HAL_WRITE(p)                                                  \
  {                                                                   \
    nfc_cb.p_hal->write((p)->len, (uint8_t*)((p) + 1) + (p)->offset); \
    GKI_freebuf(p);                                                   \
  }

#ifdef NFC_HAL_SHARED_GKI

/* NFC HAL Included if NFC_NFCEE_INCLUDED */
#if (NFC_NFCEE_INCLUDED == TRUE)

#ifndef NFC_HAL_HCI_INCLUDED
#define NFC_HAL_HCI_INCLUDED TRUE
#endif
#else /* NFC_NFCEE_INCLUDED == TRUE */
#ifndef NFC_HAL_HCI_INCLUDED
#define NFC_HAL_HCI_INCLUDED FALSE
#endif

#endif /* NFC_NFCEE_INCLUDED == FALSE */

#endif /* NFC_HAL_SHARED_GKI */

#endif /* HAL_WRITE */

#endif /* NFC_TARGET_H */
