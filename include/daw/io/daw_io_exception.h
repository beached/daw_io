// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/
//

#pragma once

#include <exception>

namespace daw::io {
	class c_file_io_exception : public std::exception {
		int m_error_code;

	public:
		explicit inline c_file_io_exception( int ec ) noexcept
		  : m_error_code( ec ) {}

		[[nodiscard]] char const *what( ) const noexcept override;

		[[nodiscard]] inline int error_code( ) const noexcept {
			return m_error_code;
		}
	};
} // namespace daw::io
