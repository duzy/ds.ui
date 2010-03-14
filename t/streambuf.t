/**                                                               -*- c++ -*-
 *
 */

#include <ds/resource/streambuf.hpp>
//#define BOOST_TEST_MODULE streambuf 
//#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <istream>
#include <fstream>
#include <iostream>

namespace fs = boost::filesystem;

static std::string read_all( std::istream & str )
{
  BOOST_CHECK( str );

  str.seekg( 0, str.end );
  std::size_t sz( str.tellg() );
  str.seekg( 0, str.beg );

  BOOST_CHECK( 0 < sz );
  if ( 0 < sz ) {
    std::size_t len( 0 );
    std::string s( sz, '\0' );
    do {
      str.read( &s[len], sz-len );
      len += str.gcount();
    } while( len < sz && str );
    return s;
  }

  return std::string();
}

static std::string read_all( const std::string & fn )
{
  std::ifstream ifs( fn.c_str(), std::ios::binary );
  return read_all( ifs );
}

BOOST_AUTO_TEST_CASE( streambuf )
{
  const std::string test_buffer( "foobar" );
  ds::resource::register_resource( ":/images/test.txt", &test_buffer[0], test_buffer.size() );

  bool be_false( ds::resource::register_resource( ":/images/test.txt", 0, 0 ) );
  BOOST_CHECK( be_false == false );

  ds::resource::item res( ":/images/test.txt" );
  BOOST_CHECK( res.data() != NULL );
  
  ds::resource::streambuf buf( ":/images/test.txt" );
  BOOST_CHECK( buf.is_valid() );

  std::istream is( &buf );
  std::string s;
  is >> s;
  BOOST_CHECK( s == test_buffer );
}

BOOST_AUTO_TEST_CASE( dsrc )
{
  std::string dsrc( "tools/dsrc/dsrc" );
  std::string f( "t/dsrc_test_file.txt" );
# ifdef WIN32
  dsrc += ".exe";
# endif
  BOOST_CHECK( fs::exists(dsrc) );
  BOOST_CHECK( fs::exists(f) );

  ds::resource::item res( "/dsrc_test_file.txt" );
  ds::resource::item res2( "/foo.txt" );
  BOOST_CHECK( res.data() != NULL );
  BOOST_CHECK( res2.data() != NULL );

  ds::resource::streambuf buf( "/dsrc_test_file.txt" );
  ds::resource::streambuf buf2( "/dsrc_test_file.txt" );
  BOOST_CHECK( buf.is_valid() );
  BOOST_CHECK( buf2.is_valid() );

  {
    std::istream is( &buf );
    std::string s1( read_all( is ) );
    std::string s2( read_all( f ) );
    BOOST_CHECK( s1 == s2 );
  }
  {
    std::istream is( &buf2 );
    std::string s1( read_all( is ) );
    std::string s2( read_all( f ) );
    BOOST_CHECK( s1 == s2 );
  }
}
