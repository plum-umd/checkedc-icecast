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

#ifndef __AUTH_H__
#define __AUTH_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "common/thread/thread.h"
#include "common/httpp/httpp.h"

#include "icecasttypes.h"
#include "cfgfile.h"

/* implemented */
#define AUTH_TYPE_ANONYMOUS       "anonymous"
#define AUTH_TYPE_STATIC          "static"
#define AUTH_TYPE_LEGACY_PASSWORD "legacy-password"
#define AUTH_TYPE_URL             "url"
#define AUTH_TYPE_HTPASSWD        "htpasswd"
#define AUTH_TYPE_ENFORCE_AUTH    "enforce-auth"

#define MAX_ADMIN_COMMANDS 32

typedef enum
{
    /* XXX: ??? */
    AUTH_UNDEFINED,
    /* user authed successfull */
    AUTH_OK,
    /* user authed failed */
    AUTH_FAILED,
    /* session got terminated */
    AUTH_RELEASED,
    /* XXX: ??? */
    AUTH_FORBIDDEN,
    /* No match for given username or other identifier found */
    AUTH_NOMATCH,
    /* status codes for database changes */
    AUTH_USERADDED,
    AUTH_USEREXISTS,
    AUTH_USERDELETED
} auth_result;

typedef enum {
    /* The slot is not used */
    AUTH_MATCHTYPE_UNUSED,
    /* Match on this slot */
    AUTH_MATCHTYPE_MATCH,
    /* Do not match on this slot */
    AUTH_MATCHTYPE_NOMATCH
} auth_matchtype_t;

typedef enum {
    /* Used internally by auth system. */
    AUTH_ALTER_NOOP = 0,
    /* Internal rewrite of URI */
    AUTH_ALTER_REWRITE,
    /* Redirect to another location. */
    AUTH_ALTER_REDIRECT,
    /* See some other resource */
    AUTH_ALTER_REDIRECT_SEE_OTHER,
    /* This resource is currently located elsewhere */
    AUTH_ALTER_REDIRECT_TEMPORARY,
    /* This resource is now located at new location */
    AUTH_ALTER_REDIRECT_PERMANENT,
    /* Send an error report to the client */
    AUTH_ALTER_SEND_ERROR
} auth_alter_t;

typedef struct auth_client_tag auth_client;
struct auth_client_tag {
    _Ptr<client_t> client;
    _Ptr<auth_result (_Ptr<auth_t> , _Ptr<auth_client> )> process;
    _Ptr<void (_Ptr<client_t> , _Ptr<void (_Ptr<client_t> , void* , auth_result )> , void* )> on_no_match;
    _Ptr<void (_Ptr<client_t> , void* , auth_result )> on_result;
    void* userdata;
    void* authbackend_userdata;
    auth_alter_t  alter_client_action;
    char         *alter_client_arg;
    auth_client  *next;
};


struct auth_tag
{
    /* unique ID */
    unsigned long id;

    /* URL for any kind of UI used to configure this or NULL. */
    char *management_url;

    _Nt_array_ptr<char> mount;

    /* filters */
    auth_matchtype_t filter_method[13];
    auth_matchtype_t filter_web_policy;
    auth_matchtype_t filter_admin_policy;
    struct (anonymous struct at /home/hasantouma/plum-umd-icecast/checkedc-icecast/src/auth.h:115:5) filter_admin[32];

    struct {
        auth_matchtype_t type;
        _Nt_array_ptr<char> origin;
    } *filter_origin;
    size_t filter_origin_len;
    auth_matchtype_t filter_origin_policy;

    /* permissions */
    auth_matchtype_t permission_alter[7];

    /* whether authenticate_client() and release_client() will return immediate.
     * Setting this will result in no thread being started for this.
     */
    int immediate;

    /* Authenticate using the given username and password */
    _Ptr<auth_result (_Ptr<auth_client> )> authenticate_client;
    _Ptr<auth_result (_Ptr<auth_client> )> release_client;

    /* auth state-specific free call */
    _Ptr<void (_Ptr<auth_t> )> free;

    auth_result (*adduser)(auth_t *auth, const char *username, const char *password);
    auth_result (*deleteuser)(auth_t *auth, const char *username);
    auth_result (*listuser)(auth_t *auth, xmlNodePtr srcnode);

    mutex_t lock;
    int running;
    size_t refcount;

    thread_type *thread;

    /* per-auth queue for clients */
    auth_client *head, **tailp;
    int pending_count;

    void *state;
    char *type;
    _Ptr<char> unique_tag;

    /* acl to set on succsessful auth */
    _Ptr<acl_t> acl;
    /* role name for later matching, may be NULL if no role name was given in config */
    char  *role;

    /* HTTP headers to send to clients using this role */
    _Ptr<ice_config_http_header_t> http_headers;
};

/* prototypes for auths that do not need own header file */
int auth_get_anonymous_auth(_Ptr<auth_t> authenticator, config_options_t *options);
int auth_get_static_auth(_Ptr<auth_t> authenticator, config_options_t *options);
int auth_get_url_auth(_Ptr<auth_t> authenticator, config_options_t *options);
int auth_get_htpasswd_auth(_Ptr<auth_t> authenticator, config_options_t *options);
int auth_get_enforce_auth_auth(_Ptr<auth_t> authenticator, config_options_t *options);

/* prototypes for auth.c */
void auth_initialise(void);
void auth_shutdown(void);

auth_result auth_str2result(const char *str);

auth_t *auth_get_authenticator(xmlNodePtr node) : itype(_Ptr<auth_t> ) ;
void auth_release(auth_t *authenticator : itype(_Ptr<auth_t> ) );
void auth_addref(auth_t *authenticator : itype(_Ptr<auth_t> ) );

int auth_release_client(_Ptr<client_t> client);

void auth_stack_add_client(_Ptr<auth_stack_t> stack, _Ptr<client_t> client, _Ptr<void (_Ptr<client_t> , void* , auth_result )> on_result, void* userdata);

int auth_alter_client(auth_t *auth : itype(_Ptr<auth_t> ) , _Ptr<auth_client> auth_user, auth_alter_t action, const char *arg : itype(_Nt_array_ptr<const char> ) );
auth_alter_t auth_str2alter(const char *str);

void auth_stack_release(_Ptr<auth_stack_t> stack);
void auth_stack_addref(_Ptr<auth_stack_t> stack);
int auth_stack_next(_Ptr<_Ptr<auth_stack_t>> stack); /* returns -1 on error, 0 on success, +1 if no next element is present */
int auth_stack_push(_Ptr<_Ptr<auth_stack_t>> stack, auth_t *auth : itype(_Ptr<auth_t> ) );
int auth_stack_append(_Ptr<auth_stack_t> stack, _Ptr<auth_stack_t> tail);
auth_t *auth_stack_get(_Ptr<auth_stack_t> stack) : itype(_Ptr<auth_t> ) ;
auth_t *auth_stack_getbyid(_Ptr<auth_stack_t> stack, unsigned long id) : itype(_Ptr<auth_t> ) ;
_Ptr<acl_t> auth_stack_get_anonymous_acl(_Ptr<auth_stack_t> stack, httpp_request_type_e method);

#endif
