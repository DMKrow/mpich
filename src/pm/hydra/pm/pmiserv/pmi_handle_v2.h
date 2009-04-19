/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *  (C) 2008 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#ifndef PMI_HANDLE_V2_H_INCLUDED
#define PMI_HANDLE_V2_H_INCLUDED

#include "pmi_handle.h"

extern struct HYD_PMCD_pmi_handle *HYD_PMCD_pmi_v2;

/* PMI handles */
char *HYD_PMCD_pmi_handle_v2_parser(char *buf, char *args[]);
HYD_Status HYD_PMCD_pmi_handle_v2_initack(int fd, char *args[]);
HYD_Status HYD_PMCD_pmi_handle_v2_get_maxes(int fd, char *args[]);
HYD_Status HYD_PMCD_pmi_handle_v2_get_appnum(int fd, char *args[]);
HYD_Status HYD_PMCD_pmi_handle_v2_get_my_kvsname(int fd, char *args[]);
HYD_Status HYD_PMCD_pmi_handle_v2_barrier_in(int fd, char *args[]);
HYD_Status HYD_PMCD_pmi_handle_v2_put(int fd, char *args[]);
HYD_Status HYD_PMCD_pmi_handle_v2_get(int fd, char *args[]);
HYD_Status HYD_PMCD_pmi_handle_v2_get_usize(int fd, char *args[]);
HYD_Status HYD_PMCD_pmi_handle_v2_finalize(int fd, char *args[]);

#endif /* PMI_HANDLE_V2_H_INCLUDED */
