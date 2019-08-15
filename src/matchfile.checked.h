/* Icecast
 *
 * This program is distributed under the GNU General Public License, version 2.
 * A copy of this license is included with this source.
 *
 * Copyright 2015,      Philipp "ph3-der-loewe" Schafft <lion@lion.leolix.org>
 */

#ifndef __MATCHFILE_H__
#define __MATCHFILE_H__

struct matchfile_tag;
typedef struct matchfile_tag matchfile_t;

matchfile_t * matchfile_new(_Nt_array_ptr<const char> filename);
int matchfile_addref(_Ptr<matchfile_t> file);
int matchfile_release(matchfile_t *file : itype(_Ptr<matchfile_t> ) );
int matchfile_match(matchfile_t *file, const char *key);

/* returns 1 for allow or pass and 0 for deny */
int matchfile_match_allow_deny(_Ptr<matchfile_t> allow, _Ptr<matchfile_t> deny, _Ptr<const char> key);

#endif  /* __MATCHFILE_H__ */
