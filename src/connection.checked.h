/* Icecast
 *
 * This program is distributed under the GNU General Public License, version 2.
 * A copy of this license is included with this source.
 *
 * Copyright 2000-2004, Jack Moffitt <jack@xiph.org, 
 *                      Michael Smith <msmith@xiph.org>,
 *                      oddsock <oddsock@xiph.org>,
 *                      Karl Heyes <karl@xiph.org>
 *                      and others (see AUTHORS for details).
 * Copyright 2014-2018, Philipp "ph3-der-loewe" Schafft <lion@lion.leolix.org>,
 */

#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <sys/types.h>
#include <time_checked.h>

#include "tls.h"

#include "icecasttypes.h"
#include "compat.h"
#include "common/thread/thread.h"
#include "common/net/sock.h"

typedef unsigned long connection_id_t;

struct connection_tag {
    /* Connection ID.
     * This is uniq until it overflows (typically at least 2^32 clients).
     */
    connection_id_t id;

    /* Timestamp of client connecting */
    time_t con_time;
    /* Timestamp of when the client must be disconnected (reached listentime limit) OR 0 for no limit. */
    time_t discon_time;
    /* Bytes sent on this connection */
    uint64_t sent_bytes;

    /* Physical socket the client is connected on */
    sock_t sock;
    /* real and effective listen socket the connect used to connect. */
    listensocket_t *listensocket_real;
    listensocket_t *listensocket_effective;

    /* Is the connection in an error condition? */
    int error;

    /* Current TLS mode and state of the client. */
    tlsmode_t tlsmode;
    tls_t *tls;

    /* I/O Callbacks. Should never be called directly.
     * Use connection_*_bytes() for I/O operations.
     */
    _Ptr<int (_Ptr<connection_t> , _Nt_array_ptr<const void> , size_t )> send;
    _Ptr<int (_Ptr<connection_t> , void* , size_t )> read;

    /* Buffers for putback of data into the connection's read queue. */
    void* readbuffer;
    size_t readbufferlen;

    /* IP Address of the client as seen by the server */
    char *ip;
};

void connection_initialize(void);
void connection_shutdown(void);
void connection_reread_config(_Ptr<ice_config_t> config);
void connection_accept_loop(void);
void connection_setup_sockets(_Ptr<ice_config_t> config);
void connection_close(_Ptr<connection_t> con);
_Ptr<connection_t> connection_create(int sock, listensocket_t *listensocket_real, listensocket_t *listensocket_effective, char *ip);
int connection_complete_source(source_t *source : itype(_Ptr<source_t> ) , int response);
void connection_queue(_Ptr<connection_t> con);
void connection_queue_client(_Ptr<client_t> client);
void connection_uses_tls(_Ptr<connection_t> con);

ssize_t connection_send_bytes(_Ptr<connection_t> con, _Nt_array_ptr<const void> buf, size_t len);
ssize_t connection_read_bytes(_Ptr<connection_t> con, void* buf, size_t len);
int connection_read_put_back(_Ptr<connection_t> con, const void *buf : itype(_Ptr<const void> ) , size_t len);

extern rwlock_t _source_shutdown_rwlock;

#endif  /* __CONNECTION_H__ */
