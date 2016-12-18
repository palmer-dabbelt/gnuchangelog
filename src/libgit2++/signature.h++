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

#ifndef LIBGIT2XX__SIGNATURE_HXX
#define LIBGIT2XX__SIGNATURE_HXX

#include "time.h++"
#include <string>

namespace libgit2xx {
	class signature {
    public:
        const std::string name;
        const std::string email;
        const time when;

    public:
        signature(
            const std::string& _name,
            const std::string& _email,
            const time& _when
        ) : name(_name),
            email(_email),
            when(_when)
        {}

        signature(const git_signature *sig)
        : name(sig->name),
          email(sig->email),
          when(sig->when)
        {}
    };
}

#endif
