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

	class c_file_write_exception : public c_file_io_exception {
		std::size_t m_bytes_written;

	public:
		explicit inline c_file_write_exception( int ec, std::size_t bytes_written ) noexcept
		  : c_file_io_exception( ec )
		  , m_bytes_written( bytes_written ) {}

		[[nodiscard]] inline std::size_t bytes_written( ) const noexcept {
			return m_bytes_written;
		}
	};
} // namespace daw::io
