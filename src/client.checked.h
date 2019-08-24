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
 * Copyright 2011-2018, Philipp "ph3-der-loewe" Schafft <lion@lion.leolix.org>,
 */

/* client.h
 **
 ** client data structions and function definitions
 **
 */
#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "common/httpp/httpp.h"
#include "common/httpp/encoding.h"

#include "icecasttypes.h"
#include "errors.h"
#include "refbuf.h"
#include "module.h"

#define CLIENT_DEFAULT_REPORT_XSL_HTML                  "report-html.xsl"
#define CLIENT_DEFAULT_REPORT_XSL_PLAINTEXT             "report-plaintext.xsl"
#define CLIENT_DEFAULT_ERROR_XSL_HTML                   "error-html.xsl"
#define CLIENT_DEFAULT_ERROR_XSL_PLAINTEXT              "error-plaintext.xsl"
#define CLIENT_DEFAULT_ADMIN_FORMAT                     ADMIN_FORMAT_HTML

typedef enum _document_domain_tag {
    DOCUMENT_DOMAIN_WEB,
    DOCUMENT_DOMAIN_ADMIN
} document_domain_t;

typedef enum _protocol_tag {
    ICECAST_PROTOCOL_HTTP = 0,
    ICECAST_PROTOCOL_SHOUTCAST
} protocol_t;

typedef enum _reuse_tag {
    /* do not reuse */
    ICECAST_REUSE_CLOSE = 0,
    /* reuse */
    ICECAST_REUSE_KEEPALIVE,
    /* Upgrade to TLS */
    ICECAST_REUSE_UPGRADETLS
} reuse_t;

typedef enum {
    CLIENT_SLURP_ERROR,
    CLIENT_SLURP_NEEDS_MORE_DATA,
    CLIENT_SLURP_BUFFER_TO_SMALL,
    CLIENT_SLURP_SUCCESS
} client_slurp_result_t;

struct _client_tag {
    /* mode of operation for this client */
    operation_mode mode;

    /* the client's connection */
    _Ptr<connection_t> con;

    /* Reuse this connection ... */
    reuse_t reuse;

    /* the client's http headers */
    _Ptr<http_parser_t> parser;

    /* Transfer Encoding if any */
    _Ptr<httpp_encoding_t> encoding;

    /* protocol client uses */
    protocol_t protocol;

    /* http request body length
     * -1 for streaming (e.g. chunked), 0 for no body, >0 for NNN bytes
     */
    ssize_t request_body_length;

    /* http request body length read so far */
    size_t request_body_read;

    /* http response code for this client */
    int respcode;

    /* admin command if any. ADMIN_COMMAND_ERROR if not an admin command. */
    admin_command_id_t admin_command;

    /* authentication instances we still need to go thru */
    _Ptr<struct auth_stack_tag> authstack;

    /* Client username */
    char *username;

    /* Client password */
    char *password;

    /* Client role */
    char *role;

    /* active ACL, set as soon as the client is authenticated */
    _Ptr<acl_t> acl;

    /* URI */
    char *uri;

    /* Handler module and function */
    module_t *handler_module;
    _Nt_array_ptr<char> handler_function;

    /* is client getting intro data */
    long intro_offset;

    /* where in the queue the client is */
    _Ptr<refbuf_t> refbuf;

    /* position in first buffer */
    unsigned int pos;

    /* auth used for this client */
    _Ptr<auth_t> auth;

    /* Format-handler-specific data for this client */
    void* format_data;

    /* function to call to release format specific resources */
    _Ptr<void (_Ptr<client_t> )> free_client_data;

    /* write out data associated with client */
    _Ptr<int (_Ptr<client_t> )> write_to_client;

    /* function to check if refbuf needs updating */
    _Ptr<int (_Ptr<source_t> , _Ptr<client_t> )> check_buffer;
};

int client_create(client_t **c_ptr : itype(_Ptr<_Ptr<client_t>> ) , _Ptr<connection_t> con, _Ptr<http_parser_t> parser);
void client_complete(client_t *client : itype(_Ptr<client_t> ) );
void client_destroy(client_t *client : itype(_Ptr<client_t> ) );
void client_send_error_by_id(client_t *client : itype(_Ptr<client_t> ) , icecast_error_id_t id);
void client_send_error_by_uuid(_Ptr<client_t> client, const char *uuid : itype(_Ptr<const char> ) );
void client_send_101(_Ptr<client_t> client, reuse_t reuse);
void client_send_204(_Ptr<client_t> client);
void client_send_426(_Ptr<client_t> client, reuse_t reuse);
void client_send_redirect(_Ptr<client_t> client, _Ptr<const char> uuid, int status, _Nt_array_ptr<const char> location);
void client_send_reportxml(_Ptr<client_t> client, reportxml_t *report : itype(_Ptr<reportxml_t> ) , document_domain_t domain, _Nt_array_ptr<const char> xsl, admin_format_t admin_format_hint, int status, _Nt_array_ptr<const char> location);
reportxml_t * client_get_reportxml(_Ptr<const char> state_definition, _Ptr<const char> state_akindof, _Nt_array_ptr<const char> state_text);
admin_format_t client_get_admin_format_by_content_negotiation(client_t *client : itype(_Ptr<client_t> ) );
int client_send_bytes(client_t *client : itype(_Ptr<client_t> ) , const void *buf : itype(_Nt_array_ptr<const void> ) , unsigned int len);
int client_read_bytes(client_t *client : itype(_Ptr<client_t> ) , void *buf : itype(_Nt_array_ptr<void> ) , unsigned int len);
void client_set_queue(client_t *client : itype(_Ptr<client_t> ) , _Ptr<refbuf_t> refbuf);
ssize_t client_body_read(client_t *client : itype(_Ptr<client_t> ) , void *buf : itype(_Nt_array_ptr<void> ) , size_t len);
int client_body_eof(client_t *client : itype(_Ptr<client_t> ) );
client_slurp_result_t client_body_slurp(_Ptr<client_t> client, void* buf, _Ptr<size_t> len);
client_slurp_result_t client_body_skip(_Ptr<client_t> client);
ssize_t client_get_baseurl(client_t *client : itype(_Ptr<client_t> ) , listensocket_t *listensocket, char *buf : itype(_Nt_array_ptr<char> ) , size_t len, const char *user : itype(_Ptr<const char> ) , const char *pw : itype(_Ptr<const char> ) , _Nt_array_ptr<const char> prefix, const char *suffix0 : itype(_Nt_array_ptr<const char> ) , _Nt_array_ptr<const char> suffix1);

#endif  /* __CLIENT_H__ */
