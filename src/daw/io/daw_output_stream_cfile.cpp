// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_io
//

#include "daw/io/daw_output_stream_cfile.h"
#include "daw/io/daw_io_exception.h"

#include <cassert>
#include <cstddef>
#include <cstdio>

namespace daw::io {
	cfile_output_stream::cfile_output_stream( std::string const &path, std::string const &mode )
	  : m_file( fopen( path.c_str( ), mode.c_str( ) ) ) {
		if( not m_file ) {
			auto const err = errno;
			throw c_file_io_exception{ err };
		}
	}

	void cfile_output_stream::close( ) {
		assert( static_cast<bool>( m_file ) );
		auto f = m_file.release( );
		if( int ret = std::fclose( f ); ret != 0 ) {
			throw c_file_io_exception{ EOF };
		}
	}

	void cfile_output_stream::flush( ) {
		assert( static_cast<bool>( m_file ) );
		if( int ret = std::fflush( m_file.get( ) ); ret != 0 ) {
			throw c_file_io_exception{ EOF };
		}
	}

	void cfile_output_stream::write( std::byte byte ) {
		assert( static_cast<bool>( m_file ) );
		if( int ret = std::fputc( static_cast<int>( byte ), m_file.get( ) ); ret != 0 ) {
			auto err = ferror( m_file.get( ) );
			throw c_file_io_exception{ err };
		}
	}

	void cfile_output_stream::write( std::span<std::byte const> data ) {
		assert( static_cast<bool>( m_file ) );
		auto const sz = data.size( );
		if( auto ret = std::fwrite( data.data( ), 1, data.size( ), m_file.get( ) ); ret != sz ) {
			auto err = ferror( m_file.get( ) );
			throw c_file_io_exception{ err };
		}
	}

	[[nodiscard]] bool cfile_output_stream::can_write( ) const noexcept {
		return static_cast<bool>( m_file );
	}

	void c_file_impl::c_file_closer::operator( )( FILE *f ) const noexcept {
		if( not f ) {
			return;
		}
		(void)std::fclose( f );
	}
} // namespace daw::io