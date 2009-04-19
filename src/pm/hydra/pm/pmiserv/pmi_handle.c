/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *  (C) 2008 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "hydra.h"
#include "pmi_handle.h"
#include "pmi_handle_v1.h"
#include "pmi_handle_v2.h"

HYD_Handle handle;
HYD_PMCD_pmi_pg_t *pg_list = NULL;

struct HYD_PMCD_pmi_handle *HYD_PMCD_pmi_v1;
struct HYD_PMCD_pmi_handle *HYD_PMCD_pmi_v2;

static HYD_Status allocate_kvs(HYD_PMCD_pmi_kvs_t ** kvs, int pgid)
{
    HYD_Status status = HYD_SUCCESS;

    HYDU_FUNC_ENTER();

    HYDU_MALLOC(*kvs, HYD_PMCD_pmi_kvs_t *, sizeof(HYD_PMCD_pmi_kvs_t), status);
    HYDU_snprintf((*kvs)->kvs_name, MAXNAMELEN, "kvs_%d_%d", (int) getpid(), pgid);
    (*kvs)->key_pair = NULL;

  fn_exit:
    HYDU_FUNC_EXIT();
    return status;

  fn_fail:
    goto fn_exit;
}


static HYD_Status create_pg(HYD_PMCD_pmi_pg_t ** pg, int pgid)
{
    HYD_Status status = HYD_SUCCESS;

    HYDU_FUNC_ENTER();

    HYDU_MALLOC(*pg, HYD_PMCD_pmi_pg_t *, sizeof(HYD_PMCD_pmi_pg_t), status);
    (*pg)->id = pgid;
    (*pg)->num_procs = 0;
    (*pg)->barrier_count = 0;
    (*pg)->process = NULL;

    status = allocate_kvs(&(*pg)->kvs, pgid);
    HYDU_ERR_POP(status, "unable to allocate kvs space\n");

    (*pg)->next = NULL;

  fn_exit:
    HYDU_FUNC_EXIT();
    return status;

  fn_fail:
    goto fn_exit;
}


static HYD_Status free_pmi_process_list(HYD_PMCD_pmi_process_t * process_list)
{
    HYD_PMCD_pmi_process_t *process, *tmp;
    HYD_Status status = HYD_SUCCESS;

    HYDU_FUNC_ENTER();

    process = process_list;
    while (process) {
        tmp = process->next;
        HYDU_FREE(process);
        process = tmp;
    }

    HYDU_FUNC_EXIT();
    return status;
}


static HYD_Status free_pmi_kvs_list(HYD_PMCD_pmi_kvs_t * kvs_list)
{
    HYD_PMCD_pmi_kvs_pair_t *key_pair, *tmp;
    HYD_Status status = HYD_SUCCESS;

    HYDU_FUNC_ENTER();

    key_pair = kvs_list->key_pair;
    while (key_pair) {
        tmp = key_pair->next;
        HYDU_FREE(key_pair);
        key_pair = tmp;
    }
    HYDU_FREE(kvs_list);

    HYDU_FUNC_EXIT();
    return status;
}


HYD_Status HYD_PMCD_pmi_init(void)
{
    struct HYD_Partition *partition;
    struct HYD_Partition_exec *exec;
    int num_procs;
    HYD_Status status = HYD_SUCCESS;

    HYDU_FUNC_ENTER();

    /* Find the number of processes in the PG */
    num_procs = 0;
    for (partition = handle.partition_list; partition && partition->exec_list;
         partition = partition->next)
        for (exec = partition->exec_list; exec; exec = exec->next)
            num_procs += exec->proc_count;

    status = create_pg(&pg_list, 0);
    HYDU_ERR_POP(status, "unable to create pg\n");
    pg_list->num_procs = num_procs;

  fn_exit:
    HYDU_FUNC_EXIT();
    return status;

  fn_fail:
    goto fn_exit;
}


HYD_Status HYD_PMCD_pmi_finalize(void)
{
    HYD_PMCD_pmi_pg_t *pg, *tmp;
    HYD_Status status = HYD_SUCCESS;

    HYDU_FUNC_ENTER();

    pg = pg_list;
    while (pg) {
        tmp = pg->next;

        status = free_pmi_process_list(pg->process);
        HYDU_ERR_POP(status, "unable to free process list\n");

        status = free_pmi_kvs_list(pg->kvs);
        HYDU_ERR_POP(status, "unable to free kvs list\n");

        HYDU_FREE(pg);
        pg = tmp;
    }

  fn_exit:
    HYDU_FUNC_EXIT();
    return status;

  fn_fail:
    goto fn_exit;
}
