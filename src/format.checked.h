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

/* format.h
 **
 ** format plugin header
 **
 */
#ifndef __FORMAT_H__
#define __FORMAT_H__

#include <vorbis/codec.h>

#include "icecasttypes.h"
#include "client.h"
#include "refbuf.h"
#include "cfgfile.h"
#include "common/httpp/httpp.h"

typedef enum _format_type_tag
{
    FORMAT_ERROR, /* No format, source not processable */
    FORMAT_TYPE_OGG,
    FORMAT_TYPE_EBML,
    FORMAT_TYPE_GENERIC
} format_type_t;

typedef struct _format_plugin_tag
{
    format_type_t type;

    /* we need to know the mount to report statistics */
    _Ptr<char> mount;

    _Nt_array_ptr<const char> contenttype;
    _Nt_array_ptr<char> charset;
    uint64_t    read_bytes;
    uint64_t    sent_bytes;

    _Ptr<_Ptr<refbuf_t> (_Ptr<source_t> )> get_buffer;
    _Ptr<int (_Ptr<client_t> )> write_buf_to_client;
    _Ptr<void (_Ptr<source_t> , _Ptr<refbuf_t> )> write_buf_to_file;
    _Ptr<int (_Ptr<source_t> , _Ptr<client_t> )> create_client_data;
    _Ptr<void (_Ptr<struct _format_plugin_tag> , _Nt_array_ptr<const char> , _Nt_array_ptr<const char> , _Ptr<const char> )> set_tag;
    _Ptr<void (_Ptr<struct _format_plugin_tag> )> free_plugin;
    _Ptr<void (_Ptr<client_t> , _Ptr<struct _format_plugin_tag> , _Ptr<mount_proxy> )> apply_settings;

    /* meta data */
    vorbis_comment vc;

    /* for internal state management */
    void *_state;
} format_plugin_t;

format_type_t format_get_type(_Nt_array_ptr<const char> contenttype);
char *format_get_mimetype(format_type_t type);
int format_get_plugin(format_type_t type, _Ptr<source_t> source);

int format_generic_write_to_client(_Ptr<client_t> client);
int format_advance_queue(_Ptr<source_t> source, _Ptr<client_t> client);
int format_check_http_buffer(_Ptr<source_t> source, _Ptr<client_t> client);
int format_check_file_buffer(_Ptr<source_t> source, _Ptr<client_t> client);


void format_send_general_headers(format_plugin_t *format, 
        source_t *source, client_t *client);

void format_set_vorbiscomment(_Ptr<format_plugin_t> plugin, const char *tag, const char *value);

#endif  /* __FORMAT_H__ */

