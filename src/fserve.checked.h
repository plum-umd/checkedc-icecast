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
 * Copyright 2018,      Philipp "ph3-der-loewe" Schafft <lion@lion.leolix.org>,
 */

#ifndef __FSERVE_H__
#define __FSERVE_H__

#include <stdio_checked.h>

#include "icecasttypes.h"

typedef void (*fserve_callback_t)(client_t *, void *);

typedef struct _fserve_t
{
    _Ptr<client_t> client;

    _Ptr<FILE> file;
    int ready;
    _Ptr<void (_Ptr<client_t> , void* )> callback;
    void* arg;
    _Ptr<struct _fserve_t> next;
} fserve_t;

void fserve_initialize(void);
void fserve_shutdown(void);
int fserve_client_create(_Ptr<client_t> httpclient);
int fserve_add_client(_Ptr<client_t> client, FILE *file : itype(_Ptr<FILE> ) );
void fserve_add_client_callback(_Ptr<client_t> client, _Ptr<void (_Ptr<client_t> , void* )> callback, void* arg);
_Nt_array_ptr<char> fserve_content_type(const char *path);
void fserve_recheck_mime_types(_Ptr<ice_config_t> config);


#endif


