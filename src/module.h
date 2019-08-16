/* Icecast
 *
 * This program is distributed under the GNU General Public License, version 2.
 * A copy of this license is included with this source.
 *
 * Copyright 2018,      Philipp "ph3-der-loewe" Schafft <lion@lion.leolix.org>,
 */

#ifndef __MODULE_H__
#define __MODULE_H__

#include <libxml/tree.h>

#include "icecasttypes.h"
#include "refobject.h"

typedef void (*module_client_handler_function_t)(module_t *self, client_t *client);
typedef int  (*module_setup_handler_t)(module_t *self, void **userdata);

typedef struct {
    _Ptr<const char> name;
    _Ptr<void (_Ptr<module_t> , _Ptr<client_t> )> cb;
} module_client_handler_t;

REFOBJECT_FORWARD_TYPE(module_container_t);
REFOBJECT_FORWARD_TYPE(module_t);

int module_container_add_module(_Ptr<module_container_t> self, _Ptr<module_t> module);
int module_container_delete_module(_Ptr<module_container_t> self, _Ptr<const char> name);
module_t * module_container_get_module(_Ptr<module_container_t> self, _Ptr<const char> name);
xmlNodePtr module_container_get_modulelist_as_xml(_Ptr<module_container_t> self);

module_t * module_new(_Ptr<const char> name, _Ptr<int (module_t* , void** )> newcb, _Ptr<int (_Ptr<module_t> , void** )> freecb, void* userdata);

int module_add_link(_Ptr<module_t> self, _Nt_array_ptr<const char> type, _Nt_array_ptr<const char> url, _Nt_array_ptr<const char> title);

/* Note: Those functions are not really thread safe as (module_client_handler_t) is not thread safe. This is by design. */
_Ptr<const module_client_handler_t> module_get_client_handler(_Ptr<module_t> self, _Nt_array_ptr<const char> name);
int module_add_client_handler(_Ptr<module_t> self, const module_client_handler_t *handlers, size_t len);

#endif
