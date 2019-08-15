/* Icecast
 *
 * This program is distributed under the GNU General Public License, version 2.
 * A copy of this license is included with this source.
 *
 * Copyright 2018,      Philipp "ph3-der-loewe" Schafft <lion@lion.leolix.org>,
 */

#ifndef __LISTENSOCKET_H__
#define __LISTENSOCKET_H__

#include "icecasttypes.h"
#include "refobject.h"
#include "cfgfile.h"

REFOBJECT_FORWARD_TYPE(listensocket_container_t);

listensocket_container_t *  listensocket_container_new(void);
int listensocket_container_configure(_Ptr<listensocket_container_t> self, _Ptr<const ice_config_t> config);
int listensocket_container_configure_and_setup(_Ptr<listensocket_container_t> self, _Ptr<const ice_config_t> config);
int listensocket_container_setup(_Ptr<listensocket_container_t> self);
_Ptr<connection_t> listensocket_container_accept(_Ptr<listensocket_container_t> self, int timeout);
int listensocket_container_set_sockcount_cb(_Ptr<listensocket_container_t> self, _Ptr<void (size_t , void* )> cb, void* userdata);
ssize_t listensocket_container_sockcount(_Ptr<listensocket_container_t> self);

REFOBJECT_FORWARD_TYPE(listensocket_t);

int listensocket_refsock(listensocket_t *self : itype(_Ptr<listensocket_t> ) );
int listensocket_unrefsock(listensocket_t *self : itype(_Ptr<listensocket_t> ) );
_Ptr<connection_t> listensocket_accept(listensocket_t *self, _Ptr<listensocket_container_t> container);
_Ptr<const listener_t> listensocket_get_listener(listensocket_t *self : itype(_Ptr<listensocket_t> ) );
int listensocket_release_listener(listensocket_t *self : itype(_Ptr<listensocket_t> ) );
listener_type_t listensocket_get_type(listensocket_t *self : itype(_Ptr<listensocket_t> ) );

#endif
