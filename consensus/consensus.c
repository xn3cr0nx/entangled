/*
 * Copyright (c) 2018 IOTA Stiftung
 * https://github.com/iotaledger/entangled
 *
 * Refer to the LICENSE file for licensing information
 */

#include "consensus/consensus.h"
#include "utils/logger_helper.h"

#define CONSENSUS_LOGGER_ID "consensus"

static logger_id_t logger_id;

retcode_t iota_consensus_init(iota_consensus_t *const consensus, tangle_t *const tangle,
                              transaction_requester_t *const transaction_requester, tips_cache_t *const tips) {
  retcode_t ret = RC_OK;

  logger_id = logger_helper_enable(CONSENSUS_LOGGER_ID, LOGGER_DEBUG, true);

  log_info(logger_id, "Initializing bundle validator\n");
  if ((ret = iota_consensus_bundle_validator_init()) != RC_OK) {
    log_critical(logger_id, "Initializing bundle validator failed\n");
    return ret;
  }

  log_info(logger_id, "Initializing cumulative weight rating calculator\n");
  if ((ret = iota_consensus_cw_rating_init(&consensus->cw_rating_calculator, DEFAULT_TIP_SELECTION_CW_CALC_IMPL)) !=
      RC_OK) {
    log_critical(logger_id, "Initializing cumulative weight rating calculator failed\n");
    return ret;
  }

  log_info(logger_id, "Initializing entry point selector\n");
  if ((ret = iota_consensus_entry_point_selector_init(&consensus->entry_point_selector,
                                                      &consensus->milestone_tracker)) != RC_OK) {
    log_critical(logger_id, "Initializing entry point selector failed\n");
    return ret;
  }

  log_info(logger_id, "Initializing exit probability randomizer\n");
  if ((ret = iota_consensus_ep_randomizer_init(&consensus->ep_randomizer, &consensus->conf,
                                               DEFAULT_TIP_SELECTION_EP_RAND_IMPL)) != RC_OK) {
    log_critical(logger_id, "Initializing exit probability randomizer failed\n");
    return ret;
  }

  log_info(logger_id, "Initializing exit probability transaction validator\n");
  if ((ret = iota_consensus_exit_prob_transaction_validator_init(
           &consensus->conf, &consensus->milestone_tracker, &consensus->ledger_validator,
           &consensus->exit_prob_transaction_validator)) != RC_OK) {
    log_critical(logger_id, "Initializing exit probability transaction validator failed\n");
    return ret;
  }

  log_info(logger_id, "Initializing snapshot\n");
  if ((ret = iota_snapshot_init(&consensus->snapshot, &consensus->conf)) != RC_OK) {
    log_critical(logger_id, "Initializing snapshot failed\n");
    return ret;
  }

  log_info(logger_id, "Initializing transaction solidifier\n");
  if ((ret = iota_consensus_transaction_solidifier_init(&consensus->transaction_solidifier, &consensus->conf,
                                                        transaction_requester, tips)) != RC_OK) {
    log_critical(logger_id, "Initializing transaction solidifier failed\n");
    return ret;
  }

  log_info(logger_id, "Initializing milestone tracker\n");
  if ((ret = iota_milestone_tracker_init(&consensus->milestone_tracker, &consensus->conf, &consensus->snapshot,
                                         &consensus->ledger_validator, &consensus->transaction_solidifier)) != RC_OK) {
    log_critical(logger_id, "Initializing milestone tracker failed\n");
    return ret;
  }

  log_info(logger_id, "Initializing tip selector\n");
  if ((ret = iota_consensus_tip_selector_init(&consensus->tip_selector, &consensus->conf,
                                              &consensus->cw_rating_calculator, &consensus->entry_point_selector,
                                              &consensus->ep_randomizer, &consensus->exit_prob_transaction_validator,
                                              &consensus->ledger_validator, &consensus->milestone_tracker)) != RC_OK) {
    log_critical(logger_id, "Initializing tip selector failed\n");
    return ret;
  }

  log_info(logger_id, "Initializing transaction validator\n");
  if ((ret = iota_consensus_transaction_validator_init(&consensus->transaction_validator, &consensus->conf)) != RC_OK) {
    log_critical(logger_id, "Initializing transaction validator failed\n");
    return ret;
  }

  log_info(logger_id, "Initializing ledger validator\n");
  if ((ret = iota_consensus_ledger_validator_init(&consensus->ledger_validator, tangle, &consensus->conf,
                                                  &consensus->milestone_tracker)) != RC_OK) {
    log_critical(logger_id, "Initializing ledger validator failed\n");
    return ret;
  }

  return ret;
}

retcode_t iota_consensus_start(iota_consensus_t *const consensus, tangle_t *const tangle) {
  retcode_t ret = RC_OK;

  log_info(logger_id, "Starting milestone tracker\n");
  if ((ret = iota_milestone_tracker_start(&consensus->milestone_tracker, tangle)) != RC_OK) {
    log_critical(logger_id, "Starting milestone tracker failed\n");
    return ret;
  }

  log_info(logger_id, "Starting transaction solidifier\n");
  if ((ret = iota_consensus_transaction_solidifier_start(&consensus->transaction_solidifier)) != RC_OK) {
    log_critical(logger_id, "Starting transaction solidifier failed\n");
    return ret;
  }

  return ret;
}

retcode_t iota_consensus_stop(iota_consensus_t *const consensus) {
  retcode_t ret = RC_OK;

  log_info(logger_id, "Stopping milestone tracker\n");
  if ((ret = iota_milestone_tracker_stop(&consensus->milestone_tracker)) != RC_OK) {
    log_critical(logger_id, "Stopping milestone tracker failed\n");
  }

  log_info(logger_id, "Stopping transaction solidifier\n");
  if ((ret = iota_consensus_transaction_solidifier_stop(&consensus->transaction_solidifier)) != RC_OK) {
    log_critical(logger_id, "Stopping transaction solidifier failed\n");
  }

  return ret;
}

retcode_t iota_consensus_destroy(iota_consensus_t *const consensus) {
  retcode_t ret = RC_OK;

  log_info(logger_id, "Destroying bundle validator\n");
  if ((ret = iota_consensus_bundle_validator_destroy()) != RC_OK) {
    log_error(logger_id, "Destroying bundle validator failed\n");
  }

  log_info(logger_id, "Destroying cumulative weight rating calculator\n");
  if ((ret = iota_consensus_cw_rating_destroy(&consensus->cw_rating_calculator)) != RC_OK) {
    log_error(logger_id, "Destroying cumulative weight rating calculator failed\n");
  }

  log_info(logger_id, "Destroying entry point selector\n");
  if ((ret = iota_consensus_entry_point_selector_destroy(&consensus->entry_point_selector)) != RC_OK) {
    log_error(logger_id, "Destroying entry point selector failed\n");
  }

  log_info(logger_id, "Destroying exit probability randomizer\n");
  if ((ret = iota_consensus_ep_randomizer_destroy(&consensus->ep_randomizer)) != RC_OK) {
    log_error(logger_id, "Destroying exit probability randomizer failed\n");
  }

  log_info(logger_id, "Destroying exit probability transaction validator\n");
  if ((ret = iota_consensus_exit_prob_transaction_validator_destroy(&consensus->exit_prob_transaction_validator)) !=
      RC_OK) {
    log_error(logger_id, "Destroying exit probability transaction validator failed\n");
  }

  log_info(logger_id, "Destroying ledger validator\n");
  if ((ret = iota_consensus_ledger_validator_destroy(&consensus->ledger_validator)) != RC_OK) {
    log_error(logger_id, "Destroying ledger validator failed\n");
  }

  log_info(logger_id, "Destroying milestone tracker\n");
  if ((ret = iota_milestone_tracker_destroy(&consensus->milestone_tracker)) != RC_OK) {
    log_error(logger_id, "Destroying milestone tracker failed\n");
  }

  log_info(logger_id, "Destroying transaction solidifier\n");
  if ((ret = iota_consensus_transaction_solidifier_destroy(&consensus->transaction_solidifier)) != RC_OK) {
    log_error(logger_id, "Destroying transaction solidifier failed\n");
  }

  log_info(logger_id, "Destroying snapshot\n");
  if ((ret = iota_snapshot_destroy(&consensus->snapshot)) != RC_OK) {
    log_error(logger_id, "Destroying snapshot failed\n");
  }

  log_info(logger_id, "Destroying tip selector\n");
  if ((ret = iota_consensus_tip_selector_destroy(&consensus->tip_selector))) {
    log_error(logger_id, "Destroying tip selector failed\n");
  }

  log_info(logger_id, "Destroying transaction validator\n");
  if ((ret = iota_consensus_transaction_validator_destroy(&consensus->transaction_validator)) != RC_OK) {
    log_error(logger_id, "Destroying transaction validator failed\n");
  }

  logger_helper_release(logger_id);

  return ret;
}
