/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef COMMON_ERRORS_H_
#define COMMON_ERRORS_H_

#include <stdio.h>
#include "common/error_strings.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file  errors.h
 *
 * The following error scheme takes advantage of a 16-bit integer
 * in order to fit in data about an error, its origin and severity
 *
 * bits devision:
 * 1 -> 6 - actual error code (63 errors)
 * 7 -> 8 - error's severity (4 categories)
 * 9 -> 16 - the module which originated the error (255 different modules)
 *
 * M is the module, S the severity and C the actual error code
 *
 *  16      9        1
 * *--------*--------*
 * |MMMMMMMM|SSCCCCCC|
 * *--------*--------*
 *
 */

/* error severity */
#define RC_SEVERITY_MASK 0x00C0
#define RC_SHIFT_SEVERITY 6

#define RC_SEVERITY_FATAL (0x3 << RC_SHIFT_SEVERITY)
#define RC_SEVERITY_MAJOR (0x2 << RC_SHIFT_SEVERITY)
#define RC_SEVERITY_MODERATE (0x1 << RC_SHIFT_SEVERITY)
#define RC_SEVERITY_MINOR (0x0 << RC_SHIFT_SEVERITY)

/* error location (module/component) */
#define RC_MODULE_MASK 0xFF00
#define RC_SHIFT_MODULE 8

#define RC_MODULE_GENERAL (0x01 << RC_SHIFT_MODULE)
#define RC_MODULE_STORAGE (0x02 << RC_SHIFT_MODULE)
#define RC_MODULE_STORAGE_SQL (0x03 << RC_SHIFT_MODULE)
#define RC_MODULE_STORAGE_SQLITE3 (0x04 << RC_SHIFT_MODULE)
#define RC_MODULE_CORE (0x05 << RC_SHIFT_MODULE)
#define RC_MODULE_NODE (0x06 << RC_SHIFT_MODULE)
#define RC_MODULE_NEIGHBOR (0x07 << RC_SHIFT_MODULE)
#define RC_MODULE_CCLIENT (0x08 << RC_SHIFT_MODULE)

#define RC_MODULE_CONSENSUS (0x09 << RC_SHIFT_MODULE)
#define RC_MODULE_CONSENSUS_CW (0x0A << RC_SHIFT_MODULE)
#define RC_MODULE_CONSENSUS_EXIT_PROBABILITIES (0x0B << RC_SHIFT_MODULE)
#define RC_MODULE_CONSENSUS_MT (0x0C << RC_SHIFT_MODULE)
#define RC_MODULE_CONSENSUS_SNAPSHOT (0x0D << RC_SHIFT_MODULE)
#define RC_MODULE_LEDGER_VALIDATOR (0x0E << RC_SHIFT_MODULE)
#define RC_MODULE_CONSENSUS_TIP_SELECTOR (0x0F << RC_SHIFT_MODULE)

#define RC_MODULE_UTILS (0xA1 << RC_SHIFT_MODULE)

#define RC_MODULE_BROADCASTER (0xA2 << RC_SHIFT_MODULE)
#define RC_MODULE_PROCESSOR (0xA3 << RC_SHIFT_MODULE)
#define RC_MODULE_RECEIVER_COMPONENT (0xA4 << RC_SHIFT_MODULE)
#define RC_MODULE_REQUESTER_COMPONENT (0xA5 << RC_SHIFT_MODULE)
#define RC_MODULE_RESPONDER_COMPONENT (0xA6 << RC_SHIFT_MODULE)

#define RC_MODULE_CIRI_CONF (0xA7 << RC_SHIFT_MODULE)
#define RC_MODULE_API (0xA8 << RC_SHIFT_MODULE)
#define RC_MODULE_GOSSIP (0xA9 << RC_SHIFT_MODULE)

#define RC_MODULE_MAM2 (0xB0 << RC_SHIFT_MODULE)

/* error code module specific */
#define RC_ERRORCODE_MASK 0x003F

/* Macros for Severity of error code */
#define RC_GET_SEVERITY(x) ((x)&RC_SEVERITY_MASK)

/* Macros for module of error code */
#define RC_GET_MODULE(x) ((x)&RC_MODULE_MASK)

/* Macros for Error code module specific */
#define RC_GET_ERRORCODE(x) ((x)&RC_ERRORCODE_MASK)

#define RC_RESOLVE_FORMAT_STR "M=0x%02X, E=0x%02X, S=0x%X (0x%04X)"

#define err_bind(expr)       \
  if (RC_OK != (e = (expr))) \
    continue;                \
  else

#define err_guard(expr, err) \
  if (!(expr)) {             \
    e = (err);               \
    continue;                \
  } else

#define ERR_BIND_RETURN(expr, e) \
  if (RC_OK != (e = (expr)))     \
    return e;                    \
  else

#define ERR_BIND_GOTO(expr, e, label) \
  if (RC_OK != (e = (expr)))          \
    goto label;                       \
  else

#define ERR_GUARD_RETURN(expr, err, e) \
  if (!(expr)) {                       \
    e = (err);                         \
    return e;                          \
  } else

#define ERR_GUARD_GOTO(expr, err, e, label) \
  if (!(expr)) {                            \
    e = (err);                              \
    goto label;                             \
  } else

/** Return Codes */
enum retcode_t {
  // Success (equal to zero)
  RC_OK = 0,
  RC_STORAGE_OOM = 0x01 | RC_MODULE_STORAGE | RC_SEVERITY_MAJOR,

  RC_NULL_PARAM = 0x01 | RC_MODULE_GENERAL | RC_SEVERITY_MAJOR,
  RC_OOM = 0x02 | RC_MODULE_GENERAL | RC_SEVERITY_MAJOR,
  RC_FAILED_THREAD_SPAWN = 0x03 | RC_MODULE_GENERAL | RC_SEVERITY_FATAL,
  RC_FAILED_THREAD_JOIN = 0x04 | RC_MODULE_GENERAL | RC_SEVERITY_MODERATE,
  RC_STILL_RUNNING = 0x05 | RC_MODULE_GENERAL | RC_SEVERITY_MODERATE,

  // Storage SQLite3 Module
  RC_SQLITE3_FAILED_OPEN_DB =
      0x01 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_FATAL,
  RC_SQLITE3_FAILED_INSERT_DB =
      0x02 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_CREATE_INDEX_DB =
      0x03 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_WRITE_STATEMENT =
      0x04 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_SELECT_DB =
      0x05 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_NO_PATH_FOR_DB_SPECIFIED =
      0x06 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_FATAL,
  RC_SQLITE3_FAILED_NOT_IMPLEMENTED =
      0x07 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_START_TRANSACTION =
      0x08 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_END_TRANSACTION =
      0x09 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_BINDING =
      0x0A | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_PREPARED_STATEMENT =
      0x0B | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_FINALIZE =
      0x0C | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_STEP = 0x0D | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_BEGIN =
      0x0E | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_END = 0x0F | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_MAJOR,
  RC_SQLITE3_FAILED_ROLLBACK =
      0x10 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_FATAL,
  RC_SQLITE3_FAILED_CONFIG =
      0x11 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_FATAL,
  RC_SQLITE3_FAILED_INITIALIZE =
      0x12 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_FATAL,
  RC_SQLITE3_FAILED_SHUTDOWN =
      0x13 | RC_MODULE_STORAGE_SQLITE3 | RC_SEVERITY_FATAL,

  // Storage SQL Module
  RC_SQL_FAILED_WRITE_STATEMENT =
      0x01 | RC_MODULE_STORAGE_SQL | RC_SEVERITY_MAJOR,

  // Core Module
  RC_CORE_NULL_CORE = 0x01 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_DATABASE_INIT = 0x02 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_DATABASE_DESTROY =
      0x03 | RC_MODULE_CORE | RC_SEVERITY_MODERATE,
  RC_CORE_FAILED_MILESTONE_TRACKER_INIT =
      0x04 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_MILESTONE_TRACKER_START =
      0x05 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_MILESTONE_TRACKER_STOP =
      0x06 | RC_MODULE_CORE | RC_SEVERITY_MODERATE,
  RC_CORE_FAILED_MILESTONE_TRACKER_DESTROY =
      0x07 | RC_MODULE_CORE | RC_SEVERITY_MODERATE,
  RC_CORE_FAILED_NODE_INIT = 0x08 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_NODE_START = 0x09 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_NODE_STOP = 0x0A | RC_MODULE_CORE | RC_SEVERITY_MODERATE,
  RC_CORE_FAILED_NODE_DESTROY = 0x0B | RC_MODULE_CORE | RC_SEVERITY_MODERATE,
  RC_CORE_FAILED_API_INIT = 0x0C | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_API_START = 0x0D | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_API_STOP = 0x0E | RC_MODULE_CORE | RC_SEVERITY_MODERATE,
  RC_CORE_FAILED_API_DESTROY = 0x0F | RC_MODULE_CORE | RC_SEVERITY_MODERATE,
  RC_CORE_FAILED_SNAPSHOT_INIT = 0x10 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_SNAPSHOT_DESTROY =
      0x11 | RC_MODULE_CORE | RC_SEVERITY_MODERATE,
  RC_CORE_FAILED_LEDGER_VALIDATOR_INIT =
      0x12 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_LEDGER_VALIDATOR_DESTROY =
      0x13 | RC_MODULE_CORE | RC_SEVERITY_MODERATE,
  RC_CORE_STILL_RUNNING = 0x14 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_CONSENSUS_INIT = 0x15 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_CONSENSUS_START = 0x16 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_CONSENSUS_STOP = 0x17 | RC_MODULE_CORE | RC_SEVERITY_FATAL,
  RC_CORE_FAILED_CONSENSUS_DESTROY = 0x18 | RC_MODULE_CORE | RC_SEVERITY_FATAL,

  // Node Module
  RC_NODE_NULL_NODE = 0x01 | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_NULL_CORE = 0x02 | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_NEIGHBORS_INIT = 0x03 | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_NEIGHBORS_DESTROY = 0x04 | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_BROADCASTER_INIT = 0x05 | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_BROADCASTER_START = 0x06 | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_BROADCASTER_STOP =
      0x07 | RC_MODULE_NODE | RC_SEVERITY_MODERATE,
  RC_NODE_FAILED_BROADCASTER_DESTROY =
      0x08 | RC_MODULE_NODE | RC_SEVERITY_MODERATE,
  RC_NODE_FAILED_PROCESSOR_INIT = 0x09 | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_PROCESSOR_START = 0x0A | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_PROCESSOR_STOP = 0x0B | RC_MODULE_NODE | RC_SEVERITY_MODERATE,
  RC_NODE_FAILED_PROCESSOR_DESTROY =
      0x0C | RC_MODULE_NODE | RC_SEVERITY_MODERATE,
  RC_NODE_FAILED_RECEIVER_INIT = 0x0D | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_RECEIVER_START = 0x0E | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_RECEIVER_STOP = 0x0F | RC_MODULE_NODE | RC_SEVERITY_MODERATE,
  RC_NODE_FAILED_RECEIVER_DESTROY =
      0x10 | RC_MODULE_NODE | RC_SEVERITY_MODERATE,
  RC_NODE_FAILED_RESPONDER_INIT = 0x13 | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_RESPONDER_START = 0x14 | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_RESPONDER_STOP = 0x15 | RC_MODULE_NODE | RC_SEVERITY_MODERATE,
  RC_NODE_FAILED_RESPONDER_DESTROY =
      0x16 | RC_MODULE_NODE | RC_SEVERITY_MODERATE,
  RC_NODE_FAILED_REQUESTER_INIT = 0x17 | RC_MODULE_NODE | RC_SEVERITY_FATAL,
  RC_NODE_FAILED_REQUESTER_DESTROY =
      0x18 | RC_MODULE_NODE | RC_SEVERITY_MODERATE,
  RC_NODE_STILL_RUNNING = 0x19 | RC_MODULE_NODE | RC_SEVERITY_FATAL,

  // Neighbor Module
  RC_NEIGHBOR_NULL_NEIGHBOR = 0x01 | RC_MODULE_NEIGHBOR | RC_SEVERITY_FATAL,
  RC_NEIGHBOR_NULL_URI = 0x02 | RC_MODULE_NEIGHBOR | RC_SEVERITY_FATAL,
  RC_NEIGHBOR_NULL_HASH = 0x03 | RC_MODULE_NEIGHBOR | RC_SEVERITY_FATAL,
  RC_NEIGHBOR_NULL_PACKET = 0x04 | RC_MODULE_NEIGHBOR | RC_SEVERITY_FATAL,
  RC_NEIGHBOR_NULL_NODE = 0x05 | RC_MODULE_NEIGHBOR | RC_SEVERITY_FATAL,
  RC_NEIGHBOR_FAILED_URI_PARSING =
      0x06 | RC_MODULE_NEIGHBOR | RC_SEVERITY_MAJOR,
  RC_NEIGHBOR_INVALID_PROTOCOL = 0x07 | RC_MODULE_NEIGHBOR | RC_SEVERITY_MAJOR,
  RC_NEIGHBOR_INVALID_HOST = 0x08 | RC_MODULE_NEIGHBOR | RC_SEVERITY_MAJOR,
  RC_NEIGHBOR_FAILED_SEND = 0x09 | RC_MODULE_NEIGHBOR | RC_SEVERITY_MODERATE,
  RC_NEIGHBOR_FAILED_REQUESTER = 0x0A | RC_MODULE_NEIGHBOR | RC_SEVERITY_FATAL,
  RC_NEIGHBOR_FAILED_ENDPOINT_INIT =
      0x0B | RC_MODULE_NEIGHBOR | RC_SEVERITY_FATAL,
  RC_NEIGHBOR_FAILED_ENDPOINT_DESTROY =
      0x0C | RC_MODULE_NEIGHBOR | RC_SEVERITY_FATAL,
  RC_NEIGHBOR_ALREADY_PAIRED = 0x0D | RC_MODULE_NEIGHBOR | RC_SEVERITY_MODERATE,
  RC_NEIGHBOR_NOT_PAIRED = 0x0E | RC_MODULE_NEIGHBOR | RC_SEVERITY_MODERATE,

  // Cclient Module
  // json create object error, might OOM.
  RC_CCLIENT_JSON_CREATE = 0x01 | RC_MODULE_CCLIENT | RC_SEVERITY_FATAL,
  // json parsing error, might the wrong format
  RC_CCLIENT_JSON_PARSE = 0x02 | RC_MODULE_CCLIENT | RC_SEVERITY_MAJOR,
  // Out of Memory
  RC_CCLIENT_OOM = 0x03 | RC_MODULE_CCLIENT | RC_SEVERITY_FATAL,
  // HTTP service error
  RC_CCLIENT_HTTP = 0x04 | RC_MODULE_CCLIENT | RC_SEVERITY_MAJOR,
  // errors during issuing a HTTP request
  RC_CCLIENT_HTTP_REQ = 0x05 | RC_MODULE_CCLIENT | RC_SEVERITY_MAJOR,
  // http response error
  RC_CCLIENT_HTTP_RES = 0x06 | RC_MODULE_CCLIENT | RC_SEVERITY_MAJOR,
  // has error messages in the response
  RC_CCLIENT_RES_ERROR = 0x07 | RC_MODULE_CCLIENT | RC_SEVERITY_MODERATE,
  // json parsing error, key no found in the json object
  RC_CCLIENT_JSON_KEY = 0x08 | RC_MODULE_CCLIENT | RC_SEVERITY_MINOR,
  RC_CCLIENT_FLEX_TRITS = 0x09 | RC_MODULE_CCLIENT | RC_SEVERITY_MODERATE,
  RC_CCLIENT_NULL_PTR = 0x0A | RC_MODULE_CCLIENT | RC_SEVERITY_MAJOR,
  RC_CCLIENT_UNIMPLEMENTED = 0x0B | RC_MODULE_CCLIENT | RC_SEVERITY_MAJOR,
  RC_CCLIENT_INVALID_SECURITY = 0x0C | RC_MODULE_CCLIENT | RC_SEVERITY_MINOR,
  RC_CCLIENT_HOST_NOT_FOUND = 0x0D | RC_MODULE_CCLIENT | RC_SEVERITY_MAJOR,
  RC_CCLIENT_TX_DESERIALIZE_FAILED =
      0x0E | RC_MODULE_CCLIENT | RC_SEVERITY_MODERATE,
  RC_CCLIENT_INSUFFICIENT_BALANCE =
      0x0F | RC_MODULE_CCLIENT | RC_SEVERITY_MINOR,
  RC_CCLIENT_POW_FAILED = 0x10 | RC_MODULE_CCLIENT | RC_SEVERITY_MINOR,
  RC_CCLIENT_INVALID_TRANSFER = 0x11 | RC_MODULE_CCLIENT | RC_SEVERITY_MODERATE,

  // Consensus module
  RC_CONSENSUS_NOT_IMPLEMENTED = 0x01 | RC_MODULE_CONSENSUS | RC_SEVERITY_MAJOR,
  RC_CONSENSUS_CW_FAILED_IN_DFS_FROM_DB =
      0x02 | RC_MODULE_CONSENSUS_CW | RC_SEVERITY_MAJOR,
  RC_CONSENSUS_CW_FAILED_IN_LIGHT_DFS =
      0x03 | RC_MODULE_CONSENSUS_CW | RC_SEVERITY_MAJOR,
  RC_CONSENSUS_OOM = 0x04 | RC_MODULE_CONSENSUS | RC_SEVERITY_MAJOR,
  RC_CONSENSUS_NULL_PTR = 0x05 | RC_MODULE_CONSENSUS | RC_SEVERITY_FATAL,
  RC_CONSENSUS_EXIT_PROBABILITIES_INVALID_ENTRYPOINT =
      0x06 | RC_MODULE_CONSENSUS_EXIT_PROBABILITIES | RC_SEVERITY_MAJOR,
  RC_CONSENSUS_EXIT_PROBABILITIES_MISSING_RATING =
      0x07 | RC_MODULE_CONSENSUS_EXIT_PROBABILITIES | RC_SEVERITY_MODERATE,

  RC_CONSENSUS_NULL_BUNDLE_PTR =
      0x08 | RC_MODULE_CONSENSUS_CW | RC_SEVERITY_MAJOR,

  // Utils module
  RC_UTILS_FAILED_REMOVE_FILE = 0x01 | RC_MODULE_UTILS | RC_SEVERITY_MAJOR,
  RC_UTILS_FAILED_TO_COPY_FILE = 0x02 | RC_MODULE_UTILS | RC_SEVERITY_MAJOR,
  RC_UTILS_FAILED_TO_OPEN_FILE = 0x03 | RC_MODULE_UTILS | RC_SEVERITY_MAJOR,
  RC_UTILS_FAILED_TO_DIGEST_FILE = 0x04 | RC_MODULE_UTILS | RC_SEVERITY_MAJOR,
  RC_UTILS_OOM = 0x05 | RC_MODULE_UTILS | RC_SEVERITY_MAJOR,
  RC_UTILS_INVALID_SIG_FILE = 0x06 | RC_MODULE_UTILS | RC_SEVERITY_MAJOR,
  RC_UTILS_INVALID_LOGGER_VERSION = 0x07 | RC_MODULE_UTILS | RC_SEVERITY_MAJOR,

  // Broadcaster module

  RC_BROADCASTER_FAILED_PUSH_QUEUE =
      0x01 | RC_MODULE_BROADCASTER | RC_SEVERITY_MINOR,
  RC_BROADCASTER_STILL_RUNNING =
      0x02 | RC_MODULE_BROADCASTER | RC_SEVERITY_FATAL,

  // Processor component module
  RC_PROCESSOR_INVALID_TRANSACTION =
      0x01 | RC_MODULE_PROCESSOR | RC_SEVERITY_MODERATE,
  RC_PROCESSOR_INVALID_REQUEST =
      0x02 | RC_MODULE_PROCESSOR | RC_SEVERITY_MODERATE,

  // Receiver component module
  RC_RECEIVER_COMPONENT_NULL_STATE =
      0x01 | RC_MODULE_RECEIVER_COMPONENT | RC_SEVERITY_FATAL,
  RC_RECEIVER_COMPONENT_NULL_NODE =
      0x02 | RC_MODULE_RECEIVER_COMPONENT | RC_SEVERITY_FATAL,
  RC_RECEIVER_COMPONENT_FAILED_THREAD_SPAWN =
      0x03 | RC_MODULE_RECEIVER_COMPONENT | RC_SEVERITY_FATAL,
  RC_RECEIVER_COMPONENT_FAILED_THREAD_JOIN =
      0x04 | RC_MODULE_RECEIVER_COMPONENT | RC_SEVERITY_MODERATE,
  RC_RECEIVER_COMPONENT_STILL_RUNNING =
      0x05 | RC_MODULE_RECEIVER_COMPONENT | RC_SEVERITY_FATAL,
  RC_RECEIVER_COMPONENT_INVALID_PROCESSOR =
      0x06 | RC_MODULE_RECEIVER_COMPONENT | RC_SEVERITY_FATAL,

  // Requester component module
  RC_REQUESTER_COMPONENT_NULL_STATE =
      0x01 | RC_MODULE_REQUESTER_COMPONENT | RC_SEVERITY_FATAL,
  RC_REQUESTER_COMPONENT_NULL_TANGLE =
      0x02 | RC_MODULE_REQUESTER_COMPONENT | RC_SEVERITY_FATAL,
  RC_REQUESTER_COMPONENT_NULL_HASH =
      0x03 | RC_MODULE_REQUESTER_COMPONENT | RC_SEVERITY_FATAL,
  RC_REQUESTER_COMPONENT_FAILED_INIT_LIST =
      0x04 | RC_MODULE_REQUESTER_COMPONENT | RC_SEVERITY_FATAL,
  RC_REQUESTER_COMPONENT_FAILED_DESTROY_LIST =
      0x05 | RC_MODULE_REQUESTER_COMPONENT | RC_SEVERITY_MODERATE,
  RC_REQUESTER_COMPONENT_FAILED_ADD_LIST =
      0x06 | RC_MODULE_REQUESTER_COMPONENT | RC_SEVERITY_MINOR,
  RC_REQUESTER_COMPONENT_FAILED_REMOVE_LIST =
      0x07 | RC_MODULE_REQUESTER_COMPONENT | RC_SEVERITY_MINOR,

  // Responder component module
  RC_RESPONDER_COMPONENT_FAILED_TX_PROCESSING =
      0x0D | RC_MODULE_RESPONDER_COMPONENT | RC_SEVERITY_MINOR,
  RC_RESPONDER_COMPONENT_FAILED_REQ_PROCESSING =
      0x0E | RC_MODULE_RESPONDER_COMPONENT | RC_SEVERITY_MINOR,
  RC_RESPONDER_COMPONENT_INVALID_TX =
      0x0F | RC_MODULE_RESPONDER_COMPONENT | RC_SEVERITY_MODERATE,

  // Gossip module
  RC_GOSSIP_SET_PACKET_TRANSACTION_FAILED =
      0x01 | RC_MODULE_GOSSIP | RC_SEVERITY_MODERATE,
  RC_GOSSIP_SET_PACKET_REQUEST_FAILED =
      0x02 | RC_MODULE_GOSSIP | RC_SEVERITY_MODERATE,

  // Conf Module
  RC_CIRI_CONF_NULL_CONF = 0x01 | RC_MODULE_CIRI_CONF | RC_SEVERITY_FATAL,
  RC_CIRI_CONF_INVALID_ARGUMENTS =
      0x02 | RC_MODULE_CIRI_CONF | RC_SEVERITY_FATAL,
  RC_CIRI_CONF_FILE_NOT_FOUND = 0x03 | RC_MODULE_CIRI_CONF | RC_SEVERITY_FATAL,
  RC_CIRI_CONF_PARSER_ERROR = 0x04 | RC_MODULE_CIRI_CONF | RC_SEVERITY_FATAL,

  // Milestone tracker Module
  RC_CONSENSUS_MT_NULL_SELF = 0x01 | RC_MODULE_CONSENSUS_MT | RC_SEVERITY_FATAL,
  RC_CONSENSUS_MT_NULL_TANGLE =
      0x02 | RC_MODULE_CONSENSUS_MT | RC_SEVERITY_FATAL,
  RC_CONSENSUS_MT_FAILED_THREAD_SPAWN =
      0x03 | RC_MODULE_CONSENSUS_MT | RC_SEVERITY_FATAL,
  RC_CONSENSUS_MT_FAILED_THREAD_JOIN =
      0x04 | RC_MODULE_CONSENSUS_MT | RC_SEVERITY_MODERATE,
  RC_CONSENSUS_MT_STILL_RUNNING =
      0x05 | RC_MODULE_CONSENSUS_MT | RC_SEVERITY_FATAL,
  RC_CONSENSUS_MT_OOM = 0x06 | RC_MODULE_CONSENSUS_MT | RC_SEVERITY_MAJOR,

  // CIRI API Module
  RC_API_SERIALIZER_NOT_IMPLEMENTED = 0x01 | RC_MODULE_API | RC_SEVERITY_FATAL,
  RC_API_MAX_GET_TRYTES = 0x02 | RC_MODULE_API | RC_SEVERITY_MODERATE,
  RC_API_FIND_TRANSACTIONS_NO_INPUT =
      0x3 | RC_MODULE_API | RC_SEVERITY_MODERATE,
  RC_API_MAX_FIND_TRANSACTIONS = 0x4 | RC_MODULE_API | RC_SEVERITY_MODERATE,
  RC_API_INVALID_DEPTH_INPUT = 0x05 | RC_MODULE_API | RC_SEVERITY_MODERATE,
  RC_API_INVALID_SUBTANGLE_STATUS = 0x06 | RC_MODULE_API | RC_SEVERITY_MODERATE,
  RC_API_TAIL_MISSING = 0x07 | RC_MODULE_API | RC_SEVERITY_MODERATE,
  RC_API_NOT_TAIL = 0x08 | RC_MODULE_API | RC_SEVERITY_MODERATE,

  // Snapshot Module
  RC_SNAPSHOT_NULL_SELF =
      0x01 | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_FATAL,
  RC_SNAPSHOT_NULL_STATE =
      0x02 | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_MODERATE,
  RC_SNAPSHOT_NULL_HASH =
      0x03 | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_MODERATE,
  RC_SNAPSHOT_NULL_BALANCE =
      0x04 | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_MODERATE,
  RC_SNAPSHOT_FILE_NOT_FOUND =
      0x05 | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_FATAL,
  RC_SNAPSHOT_INVALID_FILE =
      0x06 | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_FATAL,
  RC_SNAPSHOT_INVALID_SUPPLY =
      0x07 | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_FATAL,
  RC_SNAPSHOT_INCONSISTENT_SNAPSHOT =
      0x08 | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_FATAL,
  RC_SNAPSHOT_OOM = 0x09 | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_FATAL,
  RC_SNAPSHOT_INCONSISTENT_PATCH =
      0x0A | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_MAJOR,
  RC_SNAPSHOT_BALANCE_NOT_FOUND =
      0x0B | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_MODERATE,
  RC_SNAPSHOT_INVALID_SIGNATURE =
      0x0C | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_FATAL,
  RC_SNAPSHOT_FAILED_JSON_PARSING =
      0x0D | RC_MODULE_CONSENSUS_SNAPSHOT | RC_SEVERITY_FATAL,

  // Ledger Validator Module
  RC_LEDGER_VALIDATOR_NULL_PTR =
      0x01 | RC_MODULE_LEDGER_VALIDATOR | RC_SEVERITY_FATAL,
  RC_LEDGER_VALIDATOR_INVALID_TRANSACTION =
      0x02 | RC_MODULE_LEDGER_VALIDATOR | RC_SEVERITY_MAJOR,
  RC_LEDGER_VALIDATOR_OOM =
      0x03 | RC_MODULE_LEDGER_VALIDATOR | RC_SEVERITY_FATAL,
  RC_LEDGER_VALIDATOR_COULD_NOT_LOAD_MILESTONE =
      0x04 | RC_MODULE_LEDGER_VALIDATOR | RC_SEVERITY_MAJOR,
  RC_LEDGER_VALIDATOR_INCONSISTENT_DELTA =
      0x05 | RC_MODULE_LEDGER_VALIDATOR | RC_SEVERITY_FATAL,
  RC_LEDGER_VALIDATOR_TRANSACTION_NOT_SOLID =
      0x06 | RC_MODULE_LEDGER_VALIDATOR | RC_SEVERITY_FATAL,

  // Tip Selector Module
  RC_TIP_SELECTOR_TIPS_NOT_CONSISTENT =
      0x01 | RC_MODULE_CONSENSUS_TIP_SELECTOR | RC_SEVERITY_MODERATE,
  RC_TIP_SELECTOR_REFERENCE_TOO_OLD =
      0x02 | RC_MODULE_CONSENSUS_TIP_SELECTOR | RC_SEVERITY_MODERATE,

  // MAM2 Module
  RC_MAM2_BUFFER_TOO_SMALL = 0x01 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_INVALID_ARGUMENT = 0x02 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_INVALID_VALUE = 0x03 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_NEGATIVE_VALUE = 0x04 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_INTERNAL_ERROR = 0x05 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_NOT_IMPLEMENTED = 0x06 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_PB3_EOF = 0x07 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_PB3_BAD_ONEOF = 0x08 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_PB3_BAD_OPTIONAL = 0x09 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_PB3_BAD_REPEATED = 0x0A | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_PB3_BAD_MAC = 0x0B | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_PB3_BAD_SIG = 0x0C | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_PB3_BAD_EKEY = 0x0D | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_PB3_SIZE_T_NOT_SUPPORTED =
      0x0E | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_CHANNEL_NOT_FOUND = 0x0F | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_ENDPOINT_NOT_FOUND = 0x10 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_VERSION_NOT_SUPPORTED = 0x11 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_CHANNEL_NOT_TRUSTED = 0x12 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_ENDPOINT_NOT_TRUSTED = 0x13 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_KEYLOAD_IRRELEVANT = 0x14 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
  RC_MAM2_KEYLOAD_OVERLOADED = 0x15 | RC_MODULE_MAM2 | RC_SEVERITY_MODERATE,
};

typedef enum retcode_t retcode_t;

const char* error_2_string(retcode_t err);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_ERRORS_H_
