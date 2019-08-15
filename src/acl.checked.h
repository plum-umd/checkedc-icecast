/* Icecast
 *
 * This program is distributed under the GNU General Public License, version 2.
 * A copy of this license is included with this source.
 *
 * Copyright 2014-2018, Philipp "ph3-der-loewe" Schafft <lion@lion.leolix.org>,
 */

#ifndef __ACL_H__
#define __ACL_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "common/httpp/httpp.h"

#include "icecasttypes.h"
#include "cfgfile.h"

typedef enum acl_policy_tag {
 /* Error on function call */
 ACL_POLICY_ERROR  = -1,
 /* Client is allowed to do operation, go ahead! */
 ACL_POLICY_ALLOW = 0,
 /* Client is not allowed to do so, send error! */
 ACL_POLICY_DENY  = 1
} acl_policy_t;


/* basic functions to work with ACLs */
acl_t * acl_new(void);
acl_t * acl_new_from_xml_node(xmlNodePtr node);

void acl_addref(_Ptr<acl_t> acl);
void acl_release(_Ptr<acl_t> acl);

/* special functions */
int acl_set_ANY_str(acl_t *acl : itype(_Ptr<acl_t> ) , acl_policy_t policy, const char *str, _Ptr<int (_Ptr<acl_t> , acl_policy_t , const char* )> callback);

/* HTTP Method specific functions */
int acl_set_method_str__callback(_Ptr<acl_t> acl, acl_policy_t policy, _Nt_array_ptr<const char> str);
#define acl_set_method_str(acl,policy,str) acl_set_ANY_str((acl), (policy), (str), acl_set_method_str__callback)
acl_policy_t acl_test_method(_Ptr<acl_t> acl, httpp_request_type_e method);

/* admin/ interface specific functions */
int acl_set_admin_str__callbck(_Ptr<acl_t> acl, acl_policy_t policy, _Ptr<const char> str);
#define acl_set_admin_str(acl,policy,str) acl_set_ANY_str((acl), (policy), (str), acl_set_admin_str__callbck)
acl_policy_t acl_test_admin(_Ptr<acl_t> acl, admin_command_id_t command);

/* web/ interface specific functions */
int acl_set_web_policy(acl_t *acl : itype(_Ptr<acl_t> ) , acl_policy_t policy);
acl_policy_t acl_test_web(_Ptr<acl_t> acl);

/* mount specific functons */
int acl_set_max_connection_duration(acl_t *acl : itype(_Ptr<acl_t> ) , time_t duration);
time_t acl_get_max_connection_duration(_Ptr<acl_t> acl);
int acl_set_max_connections_per_user(acl_t *acl : itype(_Ptr<acl_t> ) , size_t limit);
ssize_t acl_get_max_connections_per_user(_Ptr<acl_t> acl);

/* HTTP specific functions */
_Ptr<const ice_config_http_header_t> acl_get_http_headers(_Ptr<acl_t> acl);

#endif
