/* Icecast
 *
 *  Copyright 2000-2004 Jack Moffitt <jack@xiph.org, 
 *                      Michael Smith <msmith@xiph.org>,
 *                      oddsock <oddsock@xiph.org>,
 *                      Karl Heyes <karl@xiph.org>
 *                      and others (see AUTHORS for details).
 *  Copyright 2012-2018 Philipp "ph3-der-loewe" Schafft <lion@lion.leolix.org>
 *
 *  This program is distributed under the GNU General Public License, version 2.
 *  A copy of this license is included with this source.
 */

#ifndef __UTIL_H__
#define __UTIL_H__

/* for FILE* */
#include <stdio_checked.h>

#include "common/net/sock.h"
#include "icecasttypes.h"

#define UNKNOWN_CONTENT 0
#define XSLT_CONTENT    1
#define HTML_CONTENT    2

#define READ_ENTIRE_HEADER 1
#define READ_LINE 0

#define MAX_LINE_LEN 512

int util_timed_wait_for_fd(int fd, int timeout);
int util_read_header(int sock, _Array_ptr<char> buff, unsigned long len, int entire);
int util_check_valid_extension(const char *uri : itype(_Nt_array_ptr<const char> ) );
_Nt_array_ptr<char> util_get_extension(const char *path : itype(_Nt_array_ptr<const char> ) );
char * util_get_path_from_uri(_Nt_array_ptr<char> uri);
char * util_get_path_from_normalised_uri(const char *uri : itype(_Nt_array_ptr<const char> ) );
char *util_normalise_uri(const char *uri : itype(_Nt_array_ptr<const char> ) ) : itype(_Nt_array_ptr<char> ) ;
_Nt_array_ptr<char> util_base64_encode(_Array_ptr<const char> data, size_t len);
_Nt_array_ptr<char> util_base64_decode(const char *data);
_Nt_array_ptr<char> util_bin_to_hex(_Array_ptr<unsigned char> data, int len);

typedef enum _util_hostcheck_tag {
    HOSTCHECK_ERROR = -1,
    HOSTCHECK_SANE  = 0,
    HOSTCHECK_NOT_FQDN,
    HOSTCHECK_IS_LOCALHOST,
    HOSTCHECK_IS_IPV4,
    HOSTCHECK_IS_IPV6,
    HOSTCHECK_BADCHAR
} util_hostcheck_type;

util_hostcheck_type util_hostcheck(const char *hostname : itype(_Nt_array_ptr<const char> ) );

int util_str_to_bool(const char *str);
int util_str_to_loglevel(const char *str);
int util_str_to_int(const char *str : itype(_Nt_array_ptr<const char> ) , const int default_value);
unsigned int util_str_to_unsigned_int(const char *str : itype(_Nt_array_ptr<const char> ) , const unsigned int default_value);

char *util_url_unescape(const char *src : itype(_Nt_array_ptr<const char> ) ) : itype(_Nt_array_ptr<char> ) ;
char *util_url_escape(const char *src) : itype(_Nt_array_ptr<char> ) ;

/* Function to build up a HTTP header.
 * out is the pointer to storage.
 * len is the total length of out.
 * offset is the offset in out we should start writing at if offset >= 0.
 * In this case the 'used' length is len-offset.
 * If offset is -1 we append at the end of out.
 * In this case the used length is len-strlen(out).
 * cache is a bool. If set allow the client to cache the content (static data).
 * if unset we will send no-caching headers.
 * status is the HTTP status code. If -1 no HTTP status header is generated.
 * statusmsg is the status header text. If NULL a default one
 * is used for the given status code. This is ignored if status is -1.
 * contenttype is the value for the Content-Type header.
 * if NULL no such header is generated.
 * charset is the charset for the object. If NULL no header is generated
 * or default is used.
 * datablock is random data added to the request.
 * If datablock is non NULL the end-of-header is appended as well as this datablock.
 * If datablock is NULL no end-of-header nor any data is appended.
 * Returns the number of bytes written or -1 on error.
 */
ssize_t util_http_build_header(char *out : itype(_Nt_array_ptr<char> ) , size_t len, ssize_t offset, int cache, int status, _Nt_array_ptr<const char> statusmsg, const char *contenttype : itype(_Ptr<const char> ) , const char *charset : itype(_Ptr<const char> ) , _Ptr<const char> datablock, source_t *source : itype(_Ptr<source_t> ) , client_t *client : itype(_Ptr<client_t> ) );

const char * util_http_select_best(const char *input : itype(_Nt_array_ptr<const char> ) , const char *first, ...);

typedef struct icecast_kv_tag {
    char *key;
    char *value;
} icecast_kv_t;

typedef struct icecast_kva_tag {
    void** _tofree;
    size_t kvlen;
    size_t indexlen;
    size_t *index;
    icecast_kv_t *kv;
} icecast_kva_t;

_Ptr<icecast_kva_t> util_parse_http_cn(_Nt_array_ptr<const char> cnstr);
void util_kva_free(_Ptr<icecast_kva_t> kva);

/* String dictionary type, without support for NULL keys, or multiple
 * instances of the same key */
typedef struct _util_dict {
    _Nt_array_ptr<char> key;
    char *val;
    _Ptr<struct _util_dict> next;
} util_dict;

_Ptr<util_dict> util_dict_new(void);
void util_dict_free(_Ptr<util_dict> dict);
/* dict, key must not be NULL. */
int util_dict_set(_Ptr<util_dict> dict, _Nt_array_ptr<const char> key, const char *val : itype(_Nt_array_ptr<const char> ) );
const char * util_dict_get(_Ptr<util_dict> dict, _Nt_array_ptr<const char> key);
char * util_dict_urlencode(_Ptr<util_dict> dict, char delim);

#ifndef HAVE_LOCALTIME_R
struct tm *localtime_r(const time_t *timep, struct tm *result);
#endif
char *util_conv_string(const char *string, const char *in_charset, const char *out_charset) : itype(_Nt_array_ptr<char> ) ;

int get_line(_Ptr<FILE> file, char *buf : itype(_Nt_array_ptr<char> ) , size_t siz);

int util_replace_string(char **dst : itype(_Ptr<_Nt_array_ptr<char>> ) , const char *src : itype(_Nt_array_ptr<const char> ) );
int util_strtolower(char *str : itype(_Array_ptr<char> ) );
#endif  /* __UTIL_H__ */
