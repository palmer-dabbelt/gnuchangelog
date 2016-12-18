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

#ifndef LIBGIT2XX__DIFF_HXX
#define LIBGIT2XX__DIFF_HXX

#include "object.h++"
#include "commit.h++"
#include <git2.h>
#include <cassert>
#include <cstring>
#include <functional>

namespace libgit2xx {
	class diff {
    private:
        git_diff *d;

    public:
        diff(const commit& source, const commit& target)
        {
            assert(source.repo == target.repo);
            auto repo = source.repo;

            git_tree *source_tree;
            git_commit_tree(&source_tree, source.ptr());
            git_tree *target_tree;
            git_commit_tree(&target_tree, target.ptr());

            auto error = git_diff_tree_to_tree(&d, repo, source_tree, target_tree, NULL);
            if (error != 0) {
                const git_error *e = giterr_last();
                std::cerr << "libgit2xx::diff::diff " << e->message << "\n";
                abort();
            }
        }

    private:
        std::function<void(const std::string& filename)> foreach_file_function;
        static int foreach_file_helper(
            const git_diff_delta *delta,
            float progress __attribute__((unused)),
            void *payload
        )
        {
            auto d = (diff *)payload;
            auto old_path = delta->old_file.path;
            auto new_path = delta->new_file.path;
            assert(std::strcmp(old_path, new_path) == 0);
            d->foreach_file_function(new_path);
            return 0;
        }

    public:
        void foreach_file(std::function<void(const std::string& filename)> f)
        {
            foreach_file_function = f;
            git_diff_foreach(
                d,
                foreach_file_helper,
                nullptr,
                nullptr,
                nullptr,
                this
            );
            
        }
    };
}

#endif
