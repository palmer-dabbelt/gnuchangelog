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

#include <libgit2++/repository.h++>
#include <libgit2++/diff.h++>
#include <stdlib.h>
#include <sstream>

static auto first_line = [](const auto& str) {
   std::stringstream ss(str);
   std::string cut;
   std::getline(ss, cut, '\n');
   return cut;
};

int main(int argc, const char **argv)
{
    static auto yyyy_mm_dd = [](const auto& time) {
        char buf[32];
        strftime(buf, 32, "%Y-%m-%d", localtime(time.as_time_t_ptr()));
        return std::string(buf);
    };

    if (argc != 2) {
        std::cerr << argv[0] << " <parent>: Prints GNU-style ChangeLogs from git\n";
        std::cerr << "  parent: The commit to stop at\n";
        return 1;
    }

	auto repo = libgit2xx::repository(".");

    auto stop_at_object = repo.revparse_single(argv[1]);
    auto stop_at = stop_at_object.commit_lookup();

	auto commit_object = repo.revparse_single("HEAD");
    auto commit = commit_object.commit_lookup();
    while (stop_at.id() != commit.id()) {
        if (commit.has_parent() == false)
            return 0;
        auto parent = commit.parent();

        auto diff = libgit2xx::diff(commit, parent);

        std::cout 
            << yyyy_mm_dd(commit.time())
            << " "
            << commit.author().name
            << " <"
            << commit.author().email
            << ">"
            << "\n\n";

        diff.foreach_hunk(
            [&](const auto& file, const auto& header)
            {
                auto floh = first_line(header);

                if (floh.find_last_of("@") == std::string::npos || floh.find_last_of("@") + 2 >= floh.size()) {
                    std::cout
                        << "\t* "
                        << file
                        << " ("
                        << "UNKNOWN"
                        << "): "
                        << first_line(commit.message())
                        << "\n";
                    return;
                }
                auto without_ats = floh.substr(floh.find_last_of("@") + 2);

                if (without_ats.find_first_of("(") == std::string::npos) {
                    std::cout
                        << "\t* "
                        << file
                        << " ("
                        << "UNKNOWN"
                        << "): "
                        << first_line(commit.message())
                        << "\n";
                    return;
                }
                auto without_args = without_ats.substr(0, without_ats.find_first_of("("));

                auto function = 
                    without_args.find_last_of(" ") == std::string::npos
                    ? without_args
                    : without_args.substr(without_args.find_last_of(" ") + 1);

                std::cout
                    << "\t* "
                    << file
                    << " ("
                    << function
                    << "): "
                    << first_line(commit.message())
                    << "\n";
                return;

            }
        );

        std::cout << "\n";

        commit = commit.parent();
    }

	return 1;
}
