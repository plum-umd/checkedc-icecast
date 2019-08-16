/* Icecast
 *
 * This program is distributed under the GNU General Public License, version 2.
 * A copy of this license is included with this source.
 *
 * Copyright 2000-2004, Jack Moffitt <jack@xiph.org>, 
 *                      Michael Smith <msmith@xiph.org>,
 *                      oddsock <oddsock@xiph.org>,
 *                      Karl Heyes <karl@xiph.org>
 *                      and others (see AUTHORS for details).
 * Copyright 2011,      Dave 'justdave' Miller <justdave@mozilla.com>.
 * Copyright 2011-2018, Philipp "ph3-der-loewe" Schafft <lion@lion.leolix.org>,
 */

#ifndef __CFGFILE_H__
#define __CFGFILE_H__

#define CONFIG_EINSANE  -1
#define CONFIG_ENOROOT  -2
#define CONFIG_EBADROOT -3
#define CONFIG_EPARSE   -4

#define MAX_YP_DIRECTORIES 25

#include <libxml/tree.h>
#include "common/thread/thread.h"
#include "common/avl/avl.h"
#include "icecasttypes.h"
#include "compat.h"

#define XMLSTR(str) ((xmlChar *)(str)) 

typedef enum _http_header_type {
    /* static: headers are passed as is to the client. */
    HTTP_HEADER_TYPE_STATIC,
    /* CORS: headers are only sent to the client if it's a CORS request. */
    HTTP_HEADER_TYPE_CORS
} http_header_type;

typedef struct ice_config_http_header_tag {
    /* type of this header. See http_header_type */
    http_header_type type;

    /* name and value of the header */
    _Ptr<char> name;
    _Ptr<char> value;

    /* filters */
    int status;

    /* link to the next list element */
    _Ptr<struct ice_config_http_header_tag> next;
} ice_config_http_header_t;

typedef struct ice_config_dir_tag {
    _Ptr<char> host;
    int touch_interval;
    _Ptr<struct ice_config_dir_tag> next;
} ice_config_dir_t;

struct _config_options {
    _Ptr<char> type;
    _Ptr<char> name;
    _Ptr<char> value;
    _Ptr<config_options_t> next;
};

typedef enum _mount_type {
 MOUNT_TYPE_NORMAL,
 MOUNT_TYPE_DEFAULT
} mount_type;

typedef struct _mount_proxy {
    /* The mountpoint this proxy is used for */
    _Ptr<char> mountname;
    /* The type of the mount point */
    mount_type mounttype;
    /* Filename to dump this stream to (will be appended).
     * NULL to not dump.
     */
    _Ptr<char> dumpfile;
    /* Send contents of file to client before the stream */
    _Ptr<char> intro_filename;
    /* Switch new listener to fallback source when max listeners reached */
    int fallback_when_full;
    /* Max listeners for this mountpoint only.
     * -1 to not limit here (i.e. only use the global limit)
     */
    int max_listeners;
    /* Fallback mountname */
    _Ptr<char> fallback_mount;
    /* When this source arrives, do we steal back
     * clients from the fallback?
     */
    int fallback_override;
    /* Do we permit direct requests of this mountpoint?
     * (or only indirect, through fallbacks)
     */
    int no_mount;
    /* amount to send to a new client if possible, -1 take
     * from global setting
     */
    int burst_size;
    unsigned int queue_size_limit;
    /* Do we list this on the xsl pages */
    int hidden;
    /* source timeout in seconds */
    unsigned int source_timeout;
    /* character set if not utf8 */
    _Ptr<char> charset;
    /* outgoing per-stream metadata interval */
    int mp3_meta_interval;
    /* additional HTTP headers */
    _Ptr<ice_config_http_header_t> http_headers;

    /* maximum history size of played songs */
    ssize_t max_history;

    _Ptr<struct event_registration_tag> event;

    _Ptr<char> cluster_password;
    _Ptr<auth_stack_t> authstack;
    unsigned int max_listener_duration;

    _Ptr<char> stream_name;
    _Ptr<char> stream_description;
    _Ptr<char> stream_url;
    _Ptr<char> stream_genre;
    _Ptr<char> bitrate;
    _Ptr<char> type;
    _Ptr<char> subtype;
    int yp_public;

    _Ptr<struct _mount_proxy> next;
} mount_proxy;

#define ALIAS_FLAG_PREFIXMATCH          0x0001

typedef struct _resource {
    _Ptr<char> source;
    _Ptr<char> destination;
    int port;
    _Ptr<char> bind_address;
    _Ptr<char> listen_socket;
    _Ptr<char> vhost;
    _Ptr<char> module;
    _Ptr<char> handler;
    operation_mode omode;
    unsigned int flags;
    _Ptr<struct _resource> next;
} resource_t;

typedef enum _listener_type_tag {
    LISTENER_TYPE_ERROR,
    LISTENER_TYPE_NORMAL,
    LISTENER_TYPE_VIRTUAL
} listener_type_t;

typedef struct _listener_t {
    _Ptr<struct _listener_t> next;
    _Ptr<char> id;
    _Ptr<char> on_behalf_of;
    listener_type_t type;
    int port;
    int so_sndbuf;
    int listen_backlog;
    _Ptr<char> bind_address;
    int shoutcast_compat;
    _Ptr<char> shoutcast_mount;
    tlsmode_t tls;
    _Ptr<auth_stack_t> authstack;
} listener_t;

typedef struct _config_tls_context {
    _Ptr<char> cert_file;
    _Ptr<char> key_file;
    _Ptr<char> cipher_list;
} config_tls_context_t;

typedef struct {
    _Ptr<char> server;
    int port;
    _Ptr<char> mount;
    _Ptr<char> username;
    _Ptr<char> password;
    _Ptr<char> bind;
    int mp3metadata;
} relay_config_upstream_t;

typedef struct {
    _Ptr<char> localmount;
    int on_demand;
    size_t upstreams;
    _Ptr<relay_config_upstream_t> upstream;
    relay_config_upstream_t upstream_default;
} relay_config_t;

struct ice_config_tag {
    _Ptr<char> config_filename;

    _Ptr<char> location;
    _Ptr<char> admin;

    int client_limit;
    int source_limit;
    int body_size_limit;
    unsigned int queue_size_limit;
    unsigned int burst_size;
    int client_timeout;
    int header_timeout;
    int source_timeout;
    int body_timeout;
    int fileserve;
    int on_demand; /* global setting for all relays */

    _Ptr<char> shoutcast_mount;
    _Ptr<char> shoutcast_user;
    _Ptr<auth_stack_t> authstack;

    _Ptr<struct event_registration_tag> event;

    int touch_interval;
    _Ptr<ice_config_dir_t> dir_list;

    _Ptr<char> hostname;
    int sane_hostname;
    int port;
    _Ptr<char> mimetypes_fn;

    _Ptr<listener_t> listen_sock;
    unsigned int listen_sock_count;

    _Ptr<char> master_server;
    int master_server_port;
    int master_update_interval;
    _Ptr<char> master_username;
    _Ptr<char> master_password;

    _Ptr<ice_config_http_header_t> http_headers;

    /* is TLS supported by the server? */
    int tls_ok;

    size_t relay_length;
    _Ptr<_Ptr<relay_config_t>> relay;

    _Ptr<mount_proxy> mounts;

    _Ptr<char> server_id;
    _Ptr<char> base_dir;
    _Ptr<char> log_dir;
    _Ptr<char> pidfile;
    _Ptr<char> null_device;
    _Ptr<char> banfile;
    _Ptr<char> allowfile;
    _Ptr<char> webroot_dir;
    _Nt_array_ptr<char> adminroot_dir;
    _Ptr<resource_t> resources;
    _Ptr<reportxml_database_t> reportxml_db;

    _Ptr<char> access_log;
    _Ptr<char> error_log;
    _Ptr<char> playlist_log;
    int loglevel;
    int logsize;
    int logarchive;

    config_tls_context_t tls_context;

    int chroot;
    int chuid;
    _Ptr<char> user;
    _Ptr<char> group;
    _Ptr<_Ptr<char>> yp_url;
    _Ptr<int> yp_url_timeout;
    _Ptr<int> yp_touch_interval;
    size_t num_yp_directories;
};

typedef struct {
    rwlock_t config_lock;
    mutex_t relay_lock;
} ice_config_locks;

void config_initialize(void);
void config_shutdown(void);

operation_mode config_str_to_omode(const char *str);

void config_reread_config(void);
int config_parse_file(const char *filename, _Ptr<ice_config_t> configuration);
int config_initial_parse_file(_Nt_array_ptr<const char> filename);
int config_parse_cmdline(int arg, _Ptr<_Ptr<char>> argv);
void config_set_config(_Ptr<ice_config_t> config);
listener_t * config_clear_listener(listener_t *listener : itype(_Ptr<listener_t> ) );
void config_clear(_Ptr<ice_config_t> c);
mount_proxy * config_find_mount(_Ptr<ice_config_t> config, const char *mount, mount_type type);

listener_t * config_copy_listener_one(_Ptr<const listener_t> listener);

config_options_t * config_parse_options(xmlNodePtr node);
void config_clear_options(config_options_t *options);

void config_parse_http_headers(xmlNodePtr node, ice_config_http_header_t **http_headers : itype(_Ptr<ice_config_http_header_t*> ) );
void config_clear_http_header(ice_config_http_header_t *header);

int config_rehash(void);

_Ptr<ice_config_locks> config_locks(void);

_Ptr<ice_config_t> config_get_config(void);
_Ptr<ice_config_t> config_grab_config(void);
void config_release_config(void);

/* To be used ONLY in one-time startup code */
_Ptr<ice_config_t> config_get_config_unlocked(void);

#endif  /* __CFGFILE_H__ */
