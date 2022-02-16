// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/
//

#pragma once

#include "daw_output_stream.h"

#include <cstdio>
#include <memory>
#include <string>

namespace daw::io {
	namespace c_file_impl {
		struct c_file_closer {
			c_file_closer( ) = default;
			void operator( )( FILE *f ) const noexcept;
		};
	} // namespace c_file_impl
	/// A non-owning c-file io wrapper for the OutputStream concept
	class cfile_output_stream : public output_stream {
		std::unique_ptr<FILE, c_file_impl::c_file_closer> m_file{ };

	public:
		cfile_output_stream( ) = default;
		cfile_output_stream( std::string const &path, std::string const &mode );

		void close( ) override;
		void flush( ) override;

		void write( std::byte byte ) override;
		void write( std::span<std::byte const> data ) override;

		[[nodiscard]] bool can_write( ) const noexcept override;
	};
} // namespace daw::io
