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

#ifndef LIBGIT2XX__REPOSITORY_HXX
#define LIBGIT2XX__REPOSITORY_HXX

#include "object.h++"
#include <git2.h>
#include <string>
#include <iostream>

namespace libgit2xx {
    class repository {
    private:
        git_repository *repo;

    public:
        repository(const std::string& path)
        {
            int error = git_repository_open(&repo, path.c_str());
            if (error != 0) {
                const git_error *e = giterr_last();
                std::cerr << "libgit2xx::repository::repository(): " << e->message << "\n";
                abort();
            }
        }

    public:
        object revparse_single(const std::string& committish)
        {
            object out(repo);
            int error = git_revparse_single(out.ptrptr(), repo, committish.c_str());
            if (error != 0) {
                const git_error *e = giterr_last();
                std::cerr << "libgit2xx::repository::revparse_single(): " << e->message << "\n";
                abort();
            }
            return out;
        }
    };
}

#endif
