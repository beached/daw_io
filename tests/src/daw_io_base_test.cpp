// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_io
//

#include <daw/io/daw_output_stream.h>
#include <daw/io/daw_output_stream_cfile.h>
#include <daw/io/daw_output_stream_concept.h>
#include <daw/io/daw_output_stream_console.h>

#include <iostream>

void foo( daw::io::output_stream &os ) {
	os.print( "Hello {}\n", "Bob" );
}

int main( ) {
	try {
		foo( daw::io::con_err );
	} catch( std::exception &ex ) { std::cout << ex.what( ) << '\n'; }
}
