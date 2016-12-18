/*
 * Copyright (C) 2016 Palmer Dabbelt
 *   <palmer@dabbelt.com>
 *
 * This file is part of gnuchangelog.
 * 
 * gnuchangelog is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gnuchangelog is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with gnuchangelog.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LBIGIT2XX__OBJECT_HXX
#define LIBGIT2XX__OBJECT_HXX

#include <git2.h>

namespace libgit2xx {
	class time
    {
    private:
        git_time_t t;

    public:
        time(const git_time_t& _t)
        : t(_t)
        {}

    public:
        time_t as_time_t(void) const
        {
            /* FIXME: Is this actually right? */
            return (time_t)t;
        }

        time_t *as_time_t_ptr(void) const
        {
            /* FIXME: Is this actually right? */
            return (time_t *)&t;
        }
    };
}

#endif
