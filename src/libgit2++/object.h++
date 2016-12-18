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

#ifndef LIBGIT2XX__OBJECT_HXX
#define LIBGIT2XX__OBJECT_HXX

namespace libgit2xx {
    class repository;
}

#include "commit.h++"
#include <git2.h>
#include <iostream>

namespace libgit2xx {
	class object {
        friend class repository;

    private:
        git_repository *repo;
        git_object *obj;

    protected:
        object(git_repository *_repo)
        : repo(_repo),
          obj(nullptr)
        {}

        git_object **ptrptr(void) { return &obj; }

    public:
        commit commit_lookup(void)
        {
            commit out;
            auto error = git_commit_lookup(out.ptrptr(), repo, git_object_id(obj));
            if (error != 0) {
                const git_error *e = giterr_last();
                std::cerr << "libgit2xx::object::commit_lookup(): " << e->message << "\n";
                abort();
            }
            return out;
        }

    };
}

#endif
