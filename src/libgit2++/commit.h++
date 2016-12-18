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

#ifndef LIBGIT2XX__COMMIT_HXX
#define LIBGIT2XX__COMMIT_HXX

namespace libgit2xx {
	class object;
}

#include "time.h++"
#include "signature.h++"
#include <git2.h>
#include <iostream>

namespace libgit2xx {
	class commit {
        friend class object;
        friend class diff;

    private:
        git_repository *repo;
        git_commit *c;

    protected:
        commit(git_repository *_repo)
        : repo(_repo),
          c(nullptr)
        {}

        git_commit *ptr(void) const { return c; }
        git_commit **ptrptr(void) { return &c; }

    public:
        typename libgit2xx::time time(void) const
        {
            auto out = git_commit_time(c);
            return out;
        }

        bool has_parent(void) const
        {
            commit parent(repo);
            auto error = git_commit_parent(parent.ptrptr(), c, 0);
            return error == 0;
        }

        commit parent(void) const
        {
            commit parent(repo);
            auto error = git_commit_parent(parent.ptrptr(), c, 0);
            if (error != 0) {
                const git_error *e = giterr_last();
                std::cerr << "libgit2xx::commit::parent() " << e->message << "\n";
                abort();
            }
            return parent;
        }

        const std::string message(void) const
        {
            return git_commit_message(c);
        }

        const signature committer(void) const
        {
            return signature(git_commit_committer(c));
        }

        const git_oid *id(void) const
        {
            return git_commit_id(c);
        }
    };
}

#endif
