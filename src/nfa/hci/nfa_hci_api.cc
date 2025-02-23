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
 *  Copyright 2018-2020 NXP
 *
 ******************************************************************************/
/******************************************************************************
 *
 *  NFA interface to HCI
 *
 ******************************************************************************/
#include "nfa_hci_api.h"

#include <android-base/logging.h>
#include <android-base/stringprintf.h>

#include "nfa_hci_defs.h"
#include "nfa_hci_int.h"

using android::base::StringPrintf;

/*******************************************************************************
**
** Function         NFA_HciRegister
**
** Description      This function will register an application with hci and
**                  returns an application handle and provides a mechanism to
**                  register a callback with HCI to receive NFA HCI event
**                  notification. When the application is registered (or if an
**                  error occurs), the app will be notified with
**                  NFA_HCI_REGISTER_EVT. Previous session information including
**                  allocated gates, created pipes and pipes states will be
**                  returned as part of tNFA_HCI_REGISTER data.
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciRegister(char* p_app_name, tNFA_HCI_CBACK* p_cback,
                            bool b_send_conn_evts) {
  tNFA_HCI_API_REGISTER_APP* p_msg;
  uint8_t app_name_len;

  if (p_app_name == nullptr) {
    LOG(VERBOSE) << StringPrintf("Invalid Application name");
    return (NFA_STATUS_FAILED);
  }

  if (p_cback == nullptr) {
    LOG(VERBOSE) << StringPrintf(
        "Application should provide callback function to "
        "register!");
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf("Application Name: %s", p_app_name);

  app_name_len = (uint8_t)strlen(p_app_name);

  /* Register the application with HCI */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      (p_app_name != nullptr) && (app_name_len <= NFA_MAX_HCI_APP_NAME_LEN) &&
      ((p_msg = (tNFA_HCI_API_REGISTER_APP*)GKI_getbuf(
            sizeof(tNFA_HCI_API_REGISTER_APP))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_REGISTER_APP_EVT;

    /* Save application name and callback */
    memset(p_msg->app_name, 0, sizeof(p_msg->app_name));
    strlcpy(p_msg->app_name, p_app_name, NFA_MAX_HCI_APP_NAME_LEN);
    p_msg->p_cback = p_cback;
    p_msg->b_send_conn_evts = b_send_conn_evts;

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }

  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciGetGateAndPipeList
**
** Description      This function will get the list of gates allocated to the
**                  application and list of dynamic pipes created by the
**                  application. The app will be notified with
**                  NFA_HCI_GET_GATE_PIPE_LIST_EVT. List of allocated dynamic
**                  gates to the application and list of pipes created by the
**                  application will be returned as part of
**                  tNFA_HCI_GET_GATE_PIPE_LIST data.
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciGetGateAndPipeList(tNFA_HANDLE hci_handle) {
  tNFA_HCI_API_GET_APP_GATE_PIPE* p_msg;

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf("hci_handle:0x%04x", hci_handle);

  /* Register the application with HCI */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      ((p_msg = (tNFA_HCI_API_GET_APP_GATE_PIPE*)GKI_getbuf(
            sizeof(tNFA_HCI_API_GET_APP_GATE_PIPE))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_GET_APP_GATE_PIPE_EVT;
    p_msg->hci_handle = hci_handle;

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }

  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciDeregister
**
** Description      This function is called to deregister an application
**                  from HCI. The app will be notified by NFA_HCI_DEREGISTER_EVT
**                  after deleting all the pipes owned by the app and
**                  deallocating all the gates allocated to the app or if an
**                  error occurs. Even if deregistration fails, the app has to
**                  register again to provide a new cback function.
**
** Returns          NFA_STATUS_OK if the application is deregistered
**                  successfully
**                  NFA_STATUS_FAILED otherwise

*******************************************************************************/
tNFA_STATUS NFA_HciDeregister(char* p_app_name) {
  tNFA_HCI_API_DEREGISTER_APP* p_msg;
  int xx;
  uint8_t app_name_len;

  if (p_app_name == nullptr) {
    LOG(VERBOSE) << StringPrintf("Invalid Application");
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf("Application Name: %s", p_app_name);
  app_name_len = (uint8_t)strlen(p_app_name);

  if (app_name_len > NFA_MAX_HCI_APP_NAME_LEN) return (NFA_STATUS_FAILED);

  /* Find the application registration */
  for (xx = 0; xx < NFA_HCI_MAX_APP_CB; xx++) {
    if ((nfa_hci_cb.cfg.reg_app_names[xx][0] != 0) &&
        (!strncmp(p_app_name, &nfa_hci_cb.cfg.reg_app_names[xx][0],
                  app_name_len)))
      break;
  }

  if (xx == NFA_HCI_MAX_APP_CB) {
    LOG(ERROR) << StringPrintf("Application Name: %s  NOT FOUND", p_app_name);
    return (NFA_STATUS_FAILED);
  }

  /* Deregister the application with HCI */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      ((p_msg = (tNFA_HCI_API_DEREGISTER_APP*)GKI_getbuf(
            sizeof(tNFA_HCI_API_DEREGISTER_APP))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_DEREGISTER_APP_EVT;

    memset(p_msg->app_name, 0, sizeof(p_msg->app_name));
    strlcpy(p_msg->app_name, p_app_name, NFA_MAX_HCI_APP_NAME_LEN);

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }

  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciAllocGate
**
** Description      This function will allocate the gate if any specified or an
**                  available generic gate for the app to provide an entry point
**                  for a particular service to other host or to establish
**                  communication with other host. When the gate is
**                  allocated (or if an error occurs), the app will be notified
**                  with NFA_HCI_ALLOCATE_GATE_EVT with the gate id. The
**                  allocated Gate information will be stored in non volatile
**                  memory.
**
** Returns          NFA_STATUS_OK if this API started
**                  NFA_STATUS_FAILED if no generic gate is available
**
*******************************************************************************/
tNFA_STATUS NFA_HciAllocGate(tNFA_HANDLE hci_handle, uint8_t gate) {
  tNFA_HCI_API_ALLOC_GATE* p_msg;

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  if ((gate) && ((gate < NFA_HCI_FIRST_HOST_SPECIFIC_GENERIC_GATE) ||
                 (gate > NFA_HCI_LAST_PROP_GATE) ||
                 (gate == NFA_HCI_CONNECTIVITY_GATE))) {
    LOG(VERBOSE) << StringPrintf("Cannot allocate gate:0x%02x", gate);
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf("hci_handle:0x%04x, Gate:0x%02x", hci_handle,
                             gate);

  /* Request HCI to allocate gate to the application */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      ((p_msg = (tNFA_HCI_API_ALLOC_GATE*)GKI_getbuf(
            sizeof(tNFA_HCI_API_ALLOC_GATE))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_ALLOC_GATE_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->gate = gate;

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }
  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciDeallocGate
**
** Description      This function will release the specified gate that was
**                  previously allocated to the application. When the generic
**                  gate is released (or if an error occurs), the app will be
**                  notified with NFA_HCI_DEALLOCATE_GATE_EVT with the gate id.
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciDeallocGate(tNFA_HANDLE hci_handle, uint8_t gate) {
  tNFA_HCI_API_DEALLOC_GATE* p_msg;

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  if ((gate < NFA_HCI_FIRST_HOST_SPECIFIC_GENERIC_GATE) ||
      (gate > NFA_HCI_LAST_PROP_GATE) || (gate == NFA_HCI_CONNECTIVITY_GATE)) {
    LOG(VERBOSE) << StringPrintf("Cannot deallocate the gate:0x%02x", gate);
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf("hci_handle:0x%04x, gate:0x%02X", hci_handle,
                             gate);

  /* Request HCI to deallocate the gate that was previously allocated to the
   * application */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      ((p_msg = (tNFA_HCI_API_DEALLOC_GATE*)GKI_getbuf(
            sizeof(tNFA_HCI_API_DEALLOC_GATE))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_DEALLOC_GATE_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->gate = gate;

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }
  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciGetHostList
**
** Description      This function will request the host controller to return the
**                  list of hosts that are present in the host network. When
**                  host controller responds with the host list (or if an error
**                  occurs), the app will be notified with NFA_HCI_HOST_LIST_EVT
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciGetHostList(tNFA_HANDLE hci_handle) {
  tNFA_HCI_API_GET_HOST_LIST* p_msg;

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf("hci_handle:0x%04x", hci_handle);

  /* Request HCI to get list of host in the hci network */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      ((p_msg = (tNFA_HCI_API_GET_HOST_LIST*)GKI_getbuf(
            sizeof(tNFA_HCI_API_GET_HOST_LIST))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_GET_HOST_LIST_EVT;
    p_msg->hci_handle = hci_handle;

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }

  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciCreatePipe
**
** Description      This function is called to create a dynamic pipe with the
**                  specified host. When the dynamic pipe is created (or
**                  if an error occurs), the app will be notified with
**                  NFA_HCI_CREATE_PIPE_EVT with the pipe id. If a pipe exists
**                  between the two gates passed as argument and if it was
**                  created earlier by the calling application then the pipe
**                  id of the existing pipe will be returned and a new pipe
**                  will not be created. After successful creation of pipe,
**                  registry entry will be created for the dynamic pipe and
**                  all information related to the pipe will be stored in non
**                  volatile memory.
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciCreatePipe(tNFA_HANDLE hci_handle, uint8_t source_gate_id,
                              uint8_t dest_host, uint8_t dest_gate) {
  tNFA_HCI_API_CREATE_PIPE_EVT* p_msg;
  uint8_t xx;

  LOG(VERBOSE) << StringPrintf(
      "hci_handle:0x%04x, source gate:0x%02X, "
      "destination host:0x%02X , destination gate:0x%02X",
      hci_handle, source_gate_id, dest_host, dest_gate);

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  if ((source_gate_id < NFA_HCI_FIRST_HOST_SPECIFIC_GENERIC_GATE) ||
      (source_gate_id > NFA_HCI_LAST_PROP_GATE)) {
    LOG(VERBOSE) << StringPrintf("Invalid local Gate:0x%02x", source_gate_id);
    return (NFA_STATUS_FAILED);
  }

  if (((dest_gate < NFA_HCI_FIRST_HOST_SPECIFIC_GENERIC_GATE) &&
       (dest_gate != NFA_HCI_LOOP_BACK_GATE) &&
       (dest_gate != NFA_HCI_IDENTITY_MANAGEMENT_GATE)) ||
      (dest_gate > NFA_HCI_LAST_PROP_GATE)) {
    LOG(VERBOSE) << StringPrintf("Invalid Destination Gate:0x%02x", dest_gate);
    return (NFA_STATUS_FAILED);
  }

  for (xx = 0; xx < NFA_HCI_MAX_HOST_IN_NETWORK; xx++)
#if(NXP_EXTNS == TRUE)
    if (nfa_hci_cb.active_host[xx] == dest_host) break;

  if (xx == NFA_HCI_MAX_HOST_IN_NETWORK) {
#else
    if (nfa_hci_cb.inactive_host[xx] == dest_host) break;

  if (xx != NFA_HCI_MAX_HOST_IN_NETWORK) {
#endif
    LOG(VERBOSE) << StringPrintf("NFA_HciCreatePipe (): Host not active:0x%02x",
                               dest_host);
    return (NFA_STATUS_FAILED);
  }

  /* Request HCI to create a pipe between two specified gates */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      (!nfa_hci_cb.b_low_power_mode) &&
      ((p_msg = (tNFA_HCI_API_CREATE_PIPE_EVT*)GKI_getbuf(
            sizeof(tNFA_HCI_API_CREATE_PIPE_EVT))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_CREATE_PIPE_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->source_gate = source_gate_id;
    p_msg->dest_host = dest_host; /* Host id of the destination host */
    p_msg->dest_gate = dest_gate; /* Gate id of the destination gate */

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }
  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciOpenPipe
**
** Description      This function is called to open a dynamic pipe.
**                  When the dynamic pipe is opened (or
**                  if an error occurs), the app will be notified with
**                  NFA_HCI_OPEN_PIPE_EVT with the pipe id.
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciOpenPipe(tNFA_HANDLE hci_handle, uint8_t pipe) {
  tNFA_HCI_API_OPEN_PIPE_EVT* p_msg;

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  if ((pipe < NFA_HCI_FIRST_DYNAMIC_PIPE) ||
      (pipe > NFA_HCI_LAST_DYNAMIC_PIPE)) {
    LOG(VERBOSE) << StringPrintf("Invalid Pipe:0x%02x", pipe);
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf("hci_handle:0x%04x, pipe:0x%02X", hci_handle,
                             pipe);

  /* Request HCI to open a pipe if it is in closed state */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      (!nfa_hci_cb.b_low_power_mode) &&
      ((p_msg = (tNFA_HCI_API_OPEN_PIPE_EVT*)GKI_getbuf(
            sizeof(tNFA_HCI_API_OPEN_PIPE_EVT))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_OPEN_PIPE_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->pipe = pipe; /* Pipe ID of the pipe to open */

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }
  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciGetRegistry
**
** Description      This function requests a peer host to return the desired
**                  registry field value for the gate that the pipe is on.
**
**                  When the peer host responds,the app is notified with
**                  NFA_HCI_GET_REG_RSP_EVT or
**                  if an error occurs in sending the command the app will be
**                  notified by NFA_HCI_CMD_SENT_EVT
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciGetRegistry(tNFA_HANDLE hci_handle, uint8_t pipe,
                               uint8_t reg_inx) {
  tNFA_HCI_API_GET_REGISTRY* p_msg;

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  if (pipe < NFA_HCI_FIRST_DYNAMIC_PIPE) {
    LOG(VERBOSE) << StringPrintf("Invalid Pipe:0x%02x", pipe);
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf("hci_handle:0x%04x  Pipe: 0x%02x", hci_handle,
                             pipe);

  /* Request HCI to get list of gates supported by the specified host */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      ((p_msg = (tNFA_HCI_API_GET_REGISTRY*)GKI_getbuf(
            sizeof(tNFA_HCI_API_GET_REGISTRY))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_GET_REGISTRY_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->pipe = pipe;
    p_msg->reg_inx = reg_inx;

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }

  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciSendCommand
**
** Description      This function is called to send a command on a pipe created
**                  by the application.
**                  The app will be notified by NFA_HCI_CMD_SENT_EVT if an error
**                  occurs.
**                  When the peer host responds,the app is notified with
**                  NFA_HCI_RSP_RCVD_EVT
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciSendCommand(tNFA_HANDLE hci_handle, uint8_t pipe,
                               uint8_t cmd_code, uint16_t cmd_size,
                               uint8_t* p_data) {
  tNFA_HCI_API_SEND_CMD_EVT* p_msg;

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  if (pipe < NFA_HCI_FIRST_DYNAMIC_PIPE) {
    LOG(VERBOSE) << StringPrintf("Invalid Pipe:0x%02x", pipe);
    return (NFA_STATUS_FAILED);
  }

  if ((cmd_size && (p_data == nullptr)) || (cmd_size > NFA_MAX_HCI_CMD_LEN)) {
    LOG(VERBOSE) << StringPrintf("Invalid cmd size:0x%02x", cmd_size);
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf("hci_handle:0x%04x, pipe:0x%02x  Code: 0x%02x",
                             hci_handle, pipe, cmd_code);

  /* Request HCI to post event data on a particular pipe */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      ((p_msg = (tNFA_HCI_API_SEND_CMD_EVT*)GKI_getbuf(
            sizeof(tNFA_HCI_API_SEND_CMD_EVT))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_SEND_CMD_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->pipe = pipe;
    p_msg->cmd_code = cmd_code;
    p_msg->cmd_len = cmd_size;

    if (cmd_size) memcpy(p_msg->data, p_data, cmd_size);

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }

  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciSendEvent
**
** Description      This function is called to send any event on a pipe created
**                  by the application.
**                  The app will be notified by NFA_HCI_EVENT_SENT_EVT
**                  after successfully sending the event on the specified pipe
**                  or if an error occurs. The application should wait for this
**                  event before releasing event buffer passed as argument.
**                  If the app is expecting a response to the event then it can
**                  provide response buffer for collecting the response. If it
**                  provides a response buffer it can also provide response
**                  timeout indicating maximum timeout for the response.
**                  Maximum of NFA_MAX_HCI_EVENT_LEN bytes APDU can be received
**                  using internal buffer if no response buffer is provided by
**                  the application. The app will be notified by
**                  NFA_HCI_EVENT_RCVD_EVT after receiving the response event
**                  or on timeout if app provided response buffer and response
**                  timeout. If response buffer and response timeout is provided
**                  by the application, it should wait for this event before
**                  releasing the response buffer. If the application did not
**                  provide response timeout then it should not release the
**                  response buffer until it receives NFA_HCI_EVENT_RCVD_EVT or
**                  after timeout it sends next event on the same pipe
**                  and receives NFA_HCI_EVENT_SENT_EVT for that event.
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciSendEvent(tNFA_HANDLE hci_handle, uint8_t pipe,
                             uint8_t evt_code, uint16_t evt_size,
                             uint8_t* p_data, uint16_t rsp_size,
                             uint8_t* p_rsp_buf, uint16_t rsp_timeout) {
  tNFA_HCI_API_SEND_EVENT_EVT* p_msg;

  LOG(VERBOSE) << StringPrintf("hci_handle:0x%04x, pipe:0x%02x  Code: 0x%02x",
                             hci_handle, pipe, evt_code);

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  if (pipe < NFA_HCI_FIRST_DYNAMIC_PIPE) {
    LOG(VERBOSE) << StringPrintf("Invalid Pipe:0x%02x", pipe);
    return (NFA_STATUS_FAILED);
  }

  if (evt_size && (p_data == nullptr)) {
    LOG(VERBOSE) << StringPrintf("Invalid Event size:0x%02x", evt_size);
    return (NFA_STATUS_FAILED);
  }

  if (rsp_size && (p_rsp_buf == nullptr)) {
    LOG(VERBOSE) << StringPrintf(
        "No Event buffer, but invalid event buffer size "
        ":%u",
        rsp_size);
    return (NFA_STATUS_FAILED);
  }

  /* Request HCI to post event data on a particular pipe */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      ((p_msg = (tNFA_HCI_API_SEND_EVENT_EVT*)GKI_getbuf(
            sizeof(tNFA_HCI_API_SEND_EVENT_EVT))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_SEND_EVENT_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->pipe = pipe;
    p_msg->evt_code = evt_code;
    p_msg->evt_len = evt_size;
    p_msg->p_evt_buf = p_data;
    p_msg->rsp_len = rsp_size;
    p_msg->p_rsp_buf = p_rsp_buf;
    p_msg->rsp_timeout = rsp_timeout;

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }

  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciClosePipe
**
** Description      This function is called to close a dynamic pipe.
**                  When the dynamic pipe is closed (or
**                  if an error occurs), the app will be notified with
**                  NFA_HCI_CLOSE_PIPE_EVT with the pipe id.
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciClosePipe(tNFA_HANDLE hci_handle, uint8_t pipe) {
  tNFA_HCI_API_CLOSE_PIPE_EVT* p_msg;

  LOG(VERBOSE) << StringPrintf("hci_handle:0x%04x, pipe:0x%02X", hci_handle,
                             pipe);

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  if ((pipe < NFA_HCI_FIRST_DYNAMIC_PIPE) ||
      (pipe > NFA_HCI_LAST_DYNAMIC_PIPE)) {
    LOG(VERBOSE) << StringPrintf("Invalid Pipe:0x%02x", pipe);
    return (NFA_STATUS_FAILED);
  }

  /* Request HCI to close a pipe if it is in opened state */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      (!nfa_hci_cb.b_low_power_mode) &&
      ((p_msg = (tNFA_HCI_API_CLOSE_PIPE_EVT*)GKI_getbuf(
            sizeof(tNFA_HCI_API_CLOSE_PIPE_EVT))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_CLOSE_PIPE_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->pipe = pipe;

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }
  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciDeletePipe
**
** Description      This function is called to delete a particular dynamic pipe.
**                  When the dynamic pipe is deleted (or if an error occurs),
**                  the app will be notified with NFA_HCI_DELETE_PIPE_EVT with
**                  the pipe id. After successful deletion of pipe, registry
**                  entry will be deleted for the dynamic pipe and all
**                  information related to the pipe will be deleted from non
**                  volatile memory.
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciDeletePipe(tNFA_HANDLE hci_handle, uint8_t pipe) {
  tNFA_HCI_API_DELETE_PIPE_EVT* p_msg;

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  if ((pipe < NFA_HCI_FIRST_DYNAMIC_PIPE) ||
      (pipe > NFA_HCI_LAST_DYNAMIC_PIPE)) {
    LOG(VERBOSE) << StringPrintf("Invalid Pipe:0x%02x", pipe);
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf("hci_handle:0x%04x, pipe:0x%02X", hci_handle,
                             pipe);

  /* Request HCI to delete a pipe created by the application identified by hci
   * handle */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      (!nfa_hci_cb.b_low_power_mode) &&
      ((p_msg = (tNFA_HCI_API_DELETE_PIPE_EVT*)GKI_getbuf(
            sizeof(tNFA_HCI_API_DELETE_PIPE_EVT))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_DELETE_PIPE_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->pipe = pipe;

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }
  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciAddStaticPipe
**
** Description      This function is called to add a static pipe for sending
**                  7816 APDUs. When the static pipe is added (or if an error
**                  occurs), the app will be notified with
**                  NFA_HCI_ADD_STATIC_PIPE_EVT with the status.
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFA_STATUS NFA_HciAddStaticPipe(tNFA_HANDLE hci_handle, uint8_t host,
                                 uint8_t gate, uint8_t pipe) {
  tNFA_HCI_API_ADD_STATIC_PIPE_EVT* p_msg;
  uint8_t xx;

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf("Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  for (xx = 0; xx < NFA_HCI_MAX_HOST_IN_NETWORK; xx++)
#if(NXP_EXTNS == TRUE)
    if (nfa_hci_cb.active_host[xx] == host) break;

  if (xx == NFA_HCI_MAX_HOST_IN_NETWORK) {
#else

    if (nfa_hci_cb.inactive_host[xx] == host) break;

  if (xx != NFA_HCI_MAX_HOST_IN_NETWORK) {
#endif
    LOG(VERBOSE) << StringPrintf(
        "NFA_HciAddStaticPipe (): Host not active:0x%02x", host);
    return (NFA_STATUS_FAILED);
  }

  if (gate <= NFA_HCI_LAST_HOST_SPECIFIC_GATE) {
    LOG(VERBOSE) << StringPrintf("Invalid Gate:0x%02x", gate);
    return (NFA_STATUS_FAILED);
  }

  if (pipe <= NFA_HCI_LAST_DYNAMIC_PIPE) {
    LOG(VERBOSE) << StringPrintf("Invalid Pipe:0x%02x", pipe);
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf("hci_handle:0x%04x, pipe:0x%02X", hci_handle,
                             pipe);

  /* Request HCI to delete a pipe created by the application identified by hci
   * handle */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      ((p_msg = (tNFA_HCI_API_ADD_STATIC_PIPE_EVT*)GKI_getbuf(
            sizeof(tNFA_HCI_API_ADD_STATIC_PIPE_EVT))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_ADD_STATIC_PIPE_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->host = host;
    p_msg->gate = gate;
    p_msg->pipe = pipe;

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }
  /* Unable to add static pipe */
  return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciDebug
**
** Description      Debug function.
**
*******************************************************************************/
void NFA_HciDebug(uint8_t action, uint8_t size, uint8_t* p_data) {
  int xx;
  tNFA_HCI_DYN_GATE* pg = nfa_hci_cb.cfg.dyn_gates;
  tNFA_HCI_DYN_PIPE* pp = nfa_hci_cb.cfg.dyn_pipes;
  NFC_HDR* p_msg;
  uint8_t* p;

  switch (action) {
    case NFA_HCI_DEBUG_DISPLAY_CB:
      LOG(VERBOSE) << StringPrintf("NFA_HciDebug  Host List:");
      for (xx = 0; xx < NFA_HCI_MAX_APP_CB; xx++) {
        if (nfa_hci_cb.cfg.reg_app_names[xx][0] != 0) {
          LOG(VERBOSE) << StringPrintf("              Host Inx:  %u   Name: %s",
                                     xx, &nfa_hci_cb.cfg.reg_app_names[xx][0]);
        }
      }

      LOG(VERBOSE) << StringPrintf("NFA_HciDebug  Gate List:");
      for (xx = 0; xx < NFA_HCI_MAX_GATE_CB; xx++, pg++) {
        if (pg->gate_id != 0) {
          LOG(VERBOSE) << StringPrintf(
              "              Gate Inx: %x  ID: 0x%02x  Owner: 0x%04x  "
              "PipeInxMask: 0x%08x",
              xx, pg->gate_id, pg->gate_owner, pg->pipe_inx_mask);
        }
      }

      LOG(VERBOSE) << StringPrintf("NFA_HciDebug  Pipe List:");
      for (xx = 0; xx < NFA_HCI_MAX_PIPE_CB; xx++, pp++) {
        if (pp->pipe_id != 0) {
          LOG(VERBOSE) << StringPrintf(
              "              Pipe Inx: %x  ID: 0x%02x  State: %u  "
              "LocalGate: "
              "0x%02x  Dest Gate: 0x%02x  Host: 0x%02x",
              xx, pp->pipe_id, pp->pipe_state, pp->local_gate, pp->dest_gate,
              pp->dest_host);
        }
      }
      break;

    case NFA_HCI_DEBUG_SIM_HCI_EVENT:
      p_msg = (NFC_HDR*)GKI_getpoolbuf(NFC_RW_POOL_ID);
      if (p_msg != nullptr) {
        p = (uint8_t*)(p_msg + 1);

        p_msg->event = NFA_HCI_CHECK_QUEUE_EVT;
        p_msg->len = size;
        p_msg->offset = 0;

        memcpy(p, p_data, size);

        nfa_sys_sendmsg(p_msg);
      }
      break;

    case NFA_HCI_DEBUG_ENABLE_LOOPBACK:
      LOG(VERBOSE) << StringPrintf("NFA_HciDebug  HCI_LOOPBACK_DEBUG = TRUE");
      HCI_LOOPBACK_DEBUG = NFA_HCI_DEBUG_ON;
      break;

    case NFA_HCI_DEBUG_DISABLE_LOOPBACK:
      LOG(VERBOSE) << StringPrintf("NFA_HciDebug  HCI_LOOPBACK_DEBUG = FALSE");
      HCI_LOOPBACK_DEBUG = NFA_HCI_DEBUG_OFF;
      break;
  }
}

/*******************************************************************************
**
** Function         NFA_HciSetRegistry
**
** Description      This function requests a peer host to set the desired
**                  registry field value for the gate that the pipe is on.
**
**                  When the peer host responds,the app is notified with
**                  NFA_HCI_SET_REG_RSP_EVT or
**                  if an error occurs in sending the command the app will be
**                  notified by NFA_HCI_CMD_SENT_EVT
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
extern tNFA_STATUS NFA_HciSetRegistry(tNFA_HANDLE hci_handle, uint8_t pipe,
                                      uint8_t reg_inx, uint8_t data_size,
                                      uint8_t* p_data) {
  tNFA_HCI_API_SET_REGISTRY* p_msg;

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf(
        "NFA_HciSetRegistry (): Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  if (pipe < NFA_HCI_FIRST_DYNAMIC_PIPE) {
    LOG(VERBOSE) << StringPrintf("NFA_HciSetRegistry (): Invalid Pipe:0x%02x",
                               pipe);
    return (NFA_STATUS_FAILED);
  }

  if ((data_size == 0) || (p_data == nullptr) ||
      (data_size > NFA_MAX_HCI_CMD_LEN)) {
    LOG(VERBOSE) << StringPrintf(
        "NFA_HciSetRegistry (): Invalid data size:0x%02x", data_size);
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf(
      "NFA_HciSetRegistry (): hci_handle:0x%04x  Pipe: 0x%02x", hci_handle,
      pipe);

  /* Request HCI to get list of gates supported by the specified host */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      ((p_msg = (tNFA_HCI_API_SET_REGISTRY*)GKI_getbuf(
            sizeof(tNFA_HCI_API_SET_REGISTRY))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_SET_REGISTRY_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->pipe = pipe;
    p_msg->reg_inx = reg_inx;
    p_msg->size = data_size;

    memcpy(p_msg->data, p_data, data_size);
    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }

  return (NFA_STATUS_FAILED);
}
/*******************************************************************************
**
** Function         NFA_HciSendResponse
**
** Description      This function is called to send a response on a pipe created
**                  by the application.
**                  The app will be notified by NFA_HCI_RSP_SENT_EVT if an error
**                  occurs.
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
extern tNFA_STATUS NFA_HciSendResponse(tNFA_HANDLE hci_handle, uint8_t pipe,
                                       uint8_t response, uint8_t data_size,
                                       uint8_t* p_data) {
  tNFA_HCI_API_SEND_RSP_EVT* p_msg;

  if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI) {
    LOG(VERBOSE) << StringPrintf(
        "NFA_HciSendResponse (): Invalid hci_handle:0x%04x", hci_handle);
    return (NFA_STATUS_FAILED);
  }

  if (pipe < NFA_HCI_FIRST_DYNAMIC_PIPE) {
    LOG(VERBOSE) << StringPrintf("NFA_HciSendResponse (): Invalid Pipe:0x%02x",
                               pipe);
    return (NFA_STATUS_FAILED);
  }

  if ((data_size && (p_data == nullptr)) || (data_size > NFA_MAX_HCI_RSP_LEN)) {
    LOG(VERBOSE) << StringPrintf(
        "NFA_HciSendResponse (): Invalid data size:0x%02x", data_size);
    return (NFA_STATUS_FAILED);
  }

  LOG(VERBOSE) << StringPrintf(
      "NFA_HciSendResponse (): hci_handle:0x%04x  Pipe: 0x%02x  Response: "
      "0x%02x",
      hci_handle, pipe, response);

  /* Request HCI to get list of gates supported by the specified host */
  if ((nfa_hci_cb.hci_state != NFA_HCI_STATE_DISABLED) &&
      ((p_msg = (tNFA_HCI_API_SEND_RSP_EVT*)GKI_getbuf(
            sizeof(tNFA_HCI_API_SEND_RSP_EVT))) != nullptr)) {
    p_msg->hdr.event = NFA_HCI_API_SEND_RSP_EVT;
    p_msg->hci_handle = hci_handle;
    p_msg->response = response;
    p_msg->size = data_size;

    if (data_size) memcpy(p_msg->data, p_data, data_size);

    nfa_sys_sendmsg(p_msg);
    return (NFA_STATUS_OK);
  }

  return (NFA_STATUS_FAILED);
}

#if(NXP_EXTNS == TRUE)
/*******************************************************************************
**
** Function         NFA_HciSendApdu
**
** Description      This function is called to send APDU commands to one of
**                  the host that acts as a server APDU host and to receive
**                  response from that host on APDU pipe.
**
**                  The application will be notified by NFA_HCI_CMD_APDU_SENT_EVT
**                  after successfully sending the APDU command on APDU pipe of
**                  the specified host or if an error occurs. The application
**                  should wait for this event before releasing APDU command
**                  buffer passed as an argument. The application should provide
**                  response APDU buffer for collecting the response APDU.
**                  The application will be notified by NFA_HCI_RSP_APDU_RCVD_EVT
**                  after receiving the response APDU or on timeout if timeout
**                  can be automatically detected on the pipe (If APDU Pipe is
**                  connected to APDU Gate implemented as per spec ETSI TS 102
**                  622 V12.1.0) on destination host.
**                  If a response buffer is provided by the application, it
**                  should wait for NFA_HCI_RSP_APDU_RCVD_EVT event or call
**                  NFA_HciAbortApdu and wait for NFA_HCI_APDU_ABORTED_EVT
**                  before releasing the response APDU buffer.
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFC_STATUS NFA_HciSendApdu (tNFA_HANDLE  hci_handle,
                             uint8_t        host_id,
                             uint16_t       cmd_apdu_len,
                             uint8_t        *p_cmd_apdu,
                             uint16_t       rsp_apdu_buf_size,
                             uint8_t        *p_rsp_apdu_buf,
                             uint32_t        rsp_timeout)
{
    tNFA_HCI_API_SEND_APDU_EVT *p_msg;

    if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI)
    {
    LOG(VERBOSE) << StringPrintf("NFA_HciSendApdu (): Invalid hci_handle:0x%04x",
                               hci_handle);
    return (NFA_STATUS_FAILED);
    }
    if (!nfa_hciu_is_active_host (host_id))
    {
    LOG(VERBOSE) << StringPrintf("NFA_HciAbortApdu (): Host not active:0x%02x",
                               host_id);
    return (NFA_STATUS_FAILED);
    }

    if (!cmd_apdu_len || (p_cmd_apdu == nullptr))
    {
    LOG(VERBOSE) << StringPrintf(
        "NFA_HciSendApdu (): Invalid Command APDU size:0x%02x", cmd_apdu_len);
    return (NFA_STATUS_FAILED);
    }

    if (rsp_apdu_buf_size && (p_rsp_apdu_buf == nullptr))
    {
    LOG(VERBOSE) << StringPrintf(
        "NFA_HciSendApdu (): No Rsp APDU buf, but invalid Rsp APDU buf size "
        ":%u",
        rsp_apdu_buf_size);

    return (NFA_STATUS_FAILED);
    }

    if (nfa_hci_cb.hci_state == NFA_HCI_STATE_DISABLED)
    {
    LOG(VERBOSE) << StringPrintf("NFA_HciSendApdu (): Invalid HCI State[0x%02x]",
                               nfa_hci_cb.hci_state);
    return (NFA_STATUS_FAILED);
    }

    LOG(VERBOSE) << StringPrintf(
        "NFA_HciSendApdu(): hci_handle:0x%04x, Server APDU host:0x%02x",
        hci_handle, host_id);

    p_msg = (tNFA_HCI_API_SEND_APDU_EVT *) GKI_getbuf (sizeof (tNFA_HCI_API_SEND_APDU_EVT));


    /* Request HCI to Send data on APDU pipe connected to specified UICC host */
    if (p_msg != nullptr)
    {
        p_msg->hdr.event         = NFA_HCI_API_SEND_APDU_EVT;
        p_msg->hci_handle        = hci_handle;
        p_msg->host_id           = host_id;
        p_msg->cmd_apdu_len      = cmd_apdu_len;
        p_msg->p_cmd_apdu        = p_cmd_apdu;
        p_msg->rsp_apdu_buf_size = rsp_apdu_buf_size;
        p_msg->p_rsp_apdu_buf    = p_rsp_apdu_buf;
        p_msg->rsp_timeout       = rsp_timeout;

        nfa_sys_sendmsg (p_msg);
        return (NFA_STATUS_OK);
    }

    return (NFA_STATUS_FAILED);
}

/*******************************************************************************
**
** Function         NFA_HciAbortApdu
**
** Description      This function is called to stop waiting for response APDU
**                  for the command APDU sent by the application on the APDU pipe
**                  of the specified host. The application will be notified by
**                  NFA_HCI_APDU_ABORTED_EVT after releasing response buffer
**                  provided with NFA_HciSendApdu for collecting the response
**                  APDU and releasing the APDU pipe for sending next command
**                  APDU on the pipe.
**                  If APDU server host supports EVT_ABORT then the server host
**                  is also notified to Abort the APDU command and wait for
**                  it to confirm with EVT_ATR that it stopped processing the
**                  command APDU before releasing the response buffer and
**                  reporting NFA_HCI_APDU_ABORTED_EVT to the application
**
** Returns          NFA_STATUS_OK if successfully initiated
**                  NFA_STATUS_FAILED otherwise
**
*******************************************************************************/
tNFC_STATUS NFA_HciAbortApdu (tNFA_HANDLE hci_handle, uint8_t host_id, uint32_t rsp_timeout)
{
    tNFA_HCI_API_ABORT_APDU_EVT *p_msg;

    if ((NFA_HANDLE_GROUP_MASK & hci_handle) != NFA_HANDLE_GROUP_HCI)
    {
        LOG(VERBOSE) << StringPrintf(
            "NFA_HciAbortApdu (): Invalid hci_handle:0x%04x", hci_handle);
        return (NFA_STATUS_FAILED);
    }

    if (!nfa_hciu_is_active_host (host_id))
    {
        LOG(VERBOSE) << StringPrintf(
            "NFA_HciAbortApdu (): Host not active:0x%02x", host_id);
        return (NFA_STATUS_FAILED);
    }

    if (nfa_hci_cb.hci_state == NFA_HCI_STATE_DISABLED)
    {
        LOG(VERBOSE) << StringPrintf(
            "NFA_HciAbortApdu (): Invalid HCI State[0x%02x]",
            nfa_hci_cb.hci_state);
        return (NFA_STATUS_FAILED);
    }

    LOG(VERBOSE) << StringPrintf(
        "NFA_HciAbortApdu (): hci_handle:0x%04x, host:0x%02X", hci_handle,
        host_id);

    p_msg = (tNFA_HCI_API_ABORT_APDU_EVT *) GKI_getbuf (sizeof (tNFA_HCI_API_ABORT_APDU_EVT));

    /* Request HCI to Abort the command APDU sent on APDU pipe */
    if (p_msg != nullptr)
    {
        p_msg->hdr.event    = NFA_HCI_API_ABORT_APDU_EVT;
        p_msg->hci_handle   = hci_handle;
        p_msg->host_id      = host_id;
        p_msg->rsp_timeout  = rsp_timeout;
        nfa_sys_sendmsg (p_msg);
        return (NFA_STATUS_OK);
    }
    /* Unable to Abort command APDU */
    return (NFA_STATUS_FAILED);
}
#endif