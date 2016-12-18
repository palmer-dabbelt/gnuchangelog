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

#include <stdlib.h>
#include <libgit2++/repository.h++>
#include <libgit2++/diff.h++>

int main(int argc, const char **argv __attribute__((unused)))
{
    static auto yyyy_mm_dd = [](const auto& time) {
        char buf[32];
        strftime(buf, 32, "%Y-%m-%d", localtime(time.as_time_t_ptr()));
        return std::string(buf);
    };

	if (argc != 1) abort();

	auto repo = libgit2xx::repository(".");
	auto commit_object = repo.revparse_single("HEAD");
    auto commit = commit_object.commit_lookup();
    while (true) {
        if (commit.has_parent() == false)
            return 0;
        auto parent = commit.parent();

        std::cout 
            << yyyy_mm_dd(commit.time())
            << " "
            << commit.committer().name
            << " <"
            << commit.committer().email
            << ">"
            << "\n\n";
        auto diff = libgit2xx::diff(commit, parent);
        auto message = commit.message();
        diff.foreach_file(
            [&](const auto& file)
            {
                std::cout << "\t* " << file << ": " << message << "";
                message = "Likewise.\n";
            }
        );

        std::cout << "\n";

        commit = commit.parent();
    }

	return 1;
}
