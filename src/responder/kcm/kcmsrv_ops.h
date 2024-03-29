/*
   SSSD

   KCM Server - private header file

   Copyright (C) Red Hat, 2016

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __KCMSRV_OPS_H__
#define __KCMSRV_OPS_H__

#include "config.h"

#include <dhash.h>
#include <sys/types.h>
#include "util/sss_iobuf.h"
#include "responder/kcm/kcmsrv_pvt.h"

/* The initial packet size, which can later be grown up to KCM_PACKET_MAX_SIZE.
 * The initial size is a trade off that is expected to best serve most of the
 * cases (typical credentials size).
 */
#define KCM_PACKET_INITIAL_SIZE 4096

/* The maximum length of a request or reply as defined by the RPC
 * protocol. This is the same constant size as MIT KRB5 uses
 * This limit comes from:
 * https://github.com/krb5/krb5/blob/c20251dafd6120fa08c76b19315cb9deb1a1b24e/src/lib/krb5/ccache/cc_kcm.c#L54
 */
#define KCM_PACKET_MAX_SIZE 10*1024*1024

struct kcm_op;
struct kcm_op *kcm_get_opt(uint16_t opcode);
const char *kcm_opt_name(struct kcm_op *op);

struct kcm_conn_data {
    /* Credentials obtained by GET_CRED_UUID_LIST. We use to improve performance
     * by avoiding ccache lookups in GET_CRED_BY_UUID. */
    hash_table_t *creds;
};

struct tevent_req *kcm_cmd_send(TALLOC_CTX *mem_ctx,
                                struct tevent_context *ev,
                                struct kcm_ops_queue_ctx *qctx,
                                struct kcm_resp_ctx *kcm_data,
                                struct kcm_conn_data *conn_data,
                                struct cli_creds *client,
                                struct kcm_data *input,
                                struct kcm_op *op);
errno_t kcm_cmd_recv(TALLOC_CTX *mem_ctx,
                     struct tevent_req *req,
                     struct sss_iobuf **_reply);

#endif /* __KCMSRV_OPS_H__ */
