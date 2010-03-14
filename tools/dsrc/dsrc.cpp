/**
 *    Copyright 2009-09-15 DuzySoft.com, by Zhan Xin-Ming (Duzy Chan)
 *    All rights reserved by Zhan Xin-Ming (Duzy Chan)
 *    Email: <duzy@duzy.info, duzy.chan@gmail.com>
 *
 *    $Id$
 *
 **/

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string>
#include <vector>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ds/resource/item.hpp>
#include <ds/debug.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem/operations.hpp>

//#define dsD(x) std::clog<<__FILE__<<":"<<__LINE__<<": "<<x<<std::endl;
#define DSRC_NS "http://www.duzysoft.com/resource"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

namespace detail
{
  struct file
  {
    std::string name;
    std::string rename;
  };//struct file

  struct files
  {
    std::string prefix;
    std::vector<file> items;
  };//struct files

  struct resource
  {
    bool is_valid;
    std::string version;
    std::vector<detail::files> items;
    fs::path path;
  };//struct resource

  static inline bool is_node( xmlNodePtr p, const char *name )
  {
    return xmlStrcmp( p->name, (const xmlChar*)name ) == 0;
  }//is_node()

  static bool parse_node_files( resource & result, xmlDocPtr doc,
                                xmlNsPtr ns, xmlNodePtr cur )
  {
    assert( is_node(cur, "files") );

    detail::files res;
    {
      xmlChar * str( xmlGetProp(cur, (const xmlChar*)"prefix") );
      if ( str ) res.prefix = (const char*)str;
    }

    cur = cur->xmlChildrenNode;
    while( cur ) {
      if ( xmlIsBlankNode(cur) ) { cur = cur->next; continue; }
      if ( cur->ns != ns ) {
        dsD("node '"<<cur->name<<"' of the wrong namespace, ignored");
        continue;
      }
      if ( is_node(cur, "file") ) {
        xmlChar *str( xmlNodeListGetString(doc, cur->xmlChildrenNode, 1) );
        if ( str == NULL ) { dsD("file: empty file name."); continue; }

        fs::path p( result.path / (const char*)str );
        if ( !fs::exists( p ) ) {
          std::clog<<"file:0: '"<<p<<"' not found!"<<std::endl;
          return false;
        }
        if ( fs::is_directory( p ) ) {
          std::clog<<"file:0: '"<<p<<"' is not a file!"<<std::endl;
          return false;
        }

        xmlChar * rn( xmlGetProp(cur, (const xmlChar*)"rename") );
        detail::file f = { (const char*)str, rn ? (const char*)rn : "" };
        res.items.push_back( f );

        if ( rn ) std::clog<<"file: "<<str<<" -> "<<rn<<std::endl;
        else std::clog<<"file: "<<str<<std::endl;
      }
      cur = cur->next;
    }//!< for-each( 'file' node )
    if ( !res.items.empty() ) {
      result.items.push_back( res );
      return true;
    }
    return false;
  }

  static bool parse_resource_file( resource & result, const char *filename )
  {
    result.is_valid = false;

    xmlDocPtr doc( xmlParseFile(filename) );
    if ( doc == NULL ) {
      dsD("xmlParseFile()");
      return false;
    }

    xmlNodePtr cur( xmlDocGetRootElement( doc ) );
    if ( cur == NULL ) {
      dsD("xmlDocGetRootElement()");
      return false;
    }
    
    xmlNsPtr ns( xmlSearchNsByHref(doc, cur, (const xmlChar*)DSRC_NS) );
    if ( ns == NULL ) {
      xmlFreeDoc( doc );
      dsD("xmlSearchNsByHref(" DSRC_NS ")");
      std::clog<< "resource: Invalid namespace, should be '"DSRC_NS"'"
               << std::endl;
      return false;
    }

    if ( !is_node(cur, "resource") ) {
      xmlFreeDoc( doc );
      dsD("root node != 'resource'");
      return false;
    }

    result.version.clear();
    {
      xmlChar *sVer( xmlGetProp(cur, (const xmlChar*)"version") );
      if ( sVer == NULL ) {
        dsD("resource: No 'version' attribute.");
      }
      else {
        result.version = (const char*)sVer;
      }
    }

    result.items.clear();
    cur = cur->xmlChildrenNode;
    while( cur ) {
      if ( xmlIsBlankNode(cur) ) { cur = cur->next; continue; }
      if ( cur->ns != ns ) {
        dsL("node '"<<cur->name<<"' of the wrong namespace, ignored");
        continue;
      }
      if ( is_node(cur, "files") ) {
        if ( !parse_node_files( result, doc, ns, cur ) )
          { dsD("fail parse_node_files()"); return false; }
      }
      cur = cur->next;
    }//!< for-each( child node )

    xmlFreeDoc( doc );
    return true;
  }//parse_resource_file()

  //======================================================================

  struct os_t
  {
    std::ostream & os;
    int line_count;

    struct dumped_item
    {
      int offset;
      int size;
    };//struct dumped_item

    typedef std::map<std::string, dumped_item> dumped_files_t;
    dumped_files_t dumped_files;

    os_t( std::ostream & os_ ) : os( os_ ), line_count( 0 ) {}

    void new_line()
    {
      line_count = 0;
      os << "\n";
    }
  };//struct os_t

  typedef unsigned char byte_t;
  static int dump_byte( os_t & o, byte_t b )
  {
    if ( o.line_count == 0 ) o.os<<"  ";
    if ( o.os<<"0x"<<std::hex<<(unsigned)b<<"," ) {
      if ( 14 < ++o.line_count ) {
        o.line_count = 0;
        o.os<<std::endl;
      }
      return 1;
    }
    dsD("can't dump byte: '"<<std::hex<<(unsigned)b<<"'");
    return 0;
  }//dump_byte()

  static int dump_bytes( os_t & o, const byte_t * bs, std::size_t len )
  {
    const byte_t * p( bs );
    const byte_t * const end( bs + len );
    for(; p != end; ++p) if ( dump_byte( o, *p ) < 1 ) return 0;
    assert( p - bs == len );
    return len;
  }

  static inline int dump_int( os_t & o, int n )
  {
    const byte_t * bs( reinterpret_cast<const byte_t *>(&n) );
    return dump_bytes( o, bs, sizeof(n) );
  }//dump_int()

  static int dump_string( os_t & o, const std::string & s )
  {
    int len( dump_int( o, s.size() ) );
    o.os<<" ";

    const byte_t * bs( reinterpret_cast<const byte_t *>(s.c_str()) );
    len += dump_bytes( o, bs, s.size() + 1/* 1-byte of '\0' EOS */ );
    return len;
  }

  static int dump_file( os_t & o, const fs::path & path, const file & fn, int offset )
  {
    if ( !fn.rename.empty() )
      o.os<<"  //!<     "<<fn.rename<<"\n"
          <<"  //!<     -> "<<fn.name<<"\n";
    else
      o.os<<"  //!<     "<<fn.name<<"\n";

    int len( 0 );

    /** If a dumped file of the same name existed, we just make an alias
     *  on the dumped file.
     */
    os_t::dumped_files_t::const_iterator it( o.dumped_files.find( fn.name ) );
    if ( it != o.dumped_files.end() ) {
      len = dump_byte( o, byte_t(ds::resource::item_type_link) );

      o.os<<" "; //!< make a space
      if ( fn.rename.empty() ) len += dump_string( o, fn.name );
      else len += dump_string( o, fn.rename );

      o.os<<" "; //!< make a space
      len += dump_int( o, it->second.offset );
      return len;
    }

    /** If the file is never dumped, we record it as dumped file.
     */
    os_t::dumped_item & rec( o.dumped_files[ fn.name ] );
    rec.offset = offset;

    len = dump_byte( o, byte_t(ds::resource::item_type_file) );
    o.os<<" "; //!< make a space
    if ( fn.rename.empty() ) len += dump_string( o, fn.name );
    else len += dump_string( o, fn.rename );

    //std::ifstream ifs( fn.name.c_str(), std::ios::binary );
    std::ifstream ifs( (path/fn.name).file_string().c_str(), std::ios::binary );
    if ( !ifs ) { dsD("can't open '"<<fn.name<<"' for input."); return 0; }

    ifs.seekg( 0, std::ios::end );
    int fileLen( rec.size = ifs.tellg() );
    o.os<<" ";
    {
      int n( dump_int( o, fileLen ) );
      if ( n != sizeof(fileLen) ) {
        dsD("dump_int(): "<<n);
        return 0;
      }
      len += n;
    }

    o.os<<" ";
    std::string buf( 4096, '\0' );
    ifs.seekg( 0, std::ios::beg );
    {
      while( ifs ) {
        ifs.read( &buf[0], buf.size() );
        len += dump_bytes( o, (byte_t*)&buf[0], ifs.gcount() );
      }
    }

    return len;
  }

  static std::string get_var_name( const std::string & fn )
  {
    std::size_t n( fn.rfind('/') );
    if ( n == fn.npos ) n = fn.rfind('\\');
    if ( n == fn.npos ) n = 0;
    std::string s( fn.substr(n, fn.size()-n) );
    if ( (n = fn.rfind('.')) != fn.npos ) s = s.substr( 0, n );
    for( n = 0; n < s.size(); ++n )
      if ( !std::isalnum( s[n] ) ) s[n] = '_';

    std::ostringstream oss;
    oss<<"g_ds_resdata_"<<fn.size()<<"_"<<s;
    return oss.str();
  }

  static bool generate_resource_source( resource & res, const std::string & out )
  {
    std::ofstream ofs( out.c_str() );
    if ( !ofs ) { dsD("can't open "<<out<<" for output"); return false; }

    std::string varName( get_var_name(out) );

    ofs
      <<"/**\n"
      <<" *    ds::resource object code\n"
      <<" *\n"
      <<" **/\n"
      <<"\n"
      <<"#include <ds/debug.hpp>\n"
      <<"#include <ds/resource/item.hpp>\n"
      <<"\n"
      <<"static const unsigned char "<<varName<<"[] = {"
      ;

    typedef std::vector<int> offsets_t;
    typedef std::vector<offsets_t> file_list_t;
    file_list_t fi;

    os_t os( ofs );

    ofs<<"\n  //!< Version: \n";
    int ver( ds::resource::compiled_resource_version_1 );
    int offset( dump_int( os, ver ) );
    os.new_line();

    //!<
    //!< print data bytes of files
    //!<
    fi.resize( res.items.size() );
    for( int i=0; i < res.items.size(); ++i ) {

      detail::files & fl( res.items[i] );
      fi[i].resize( fl.items.size() );

      ofs<<"\n  //!< prefix: "<<fl.prefix<<"\n";
      for( int j=0; j < fl.items.size(); ++j ) {
        if ( !fl.items[j].rename.empty() )
          std::clog<<"dump:"<<offset<<": "<<fl.prefix<<fl.items[j].rename
                   <<" -> "<<fl.items[j].name<<std::endl;
        else
          std::clog<<"dump:"<<offset<<": "<<fl.prefix<<fl.items[j].name
                   <<" -> "<<fl.items[j].name<<std::endl;

        fi[i][j] = offset; //!< the offset
        offset += dump_file( os, res.path, fl.items[j], offset );
        os.new_line();
      }//!< for-each( file )

    }//!< for-each( detail::files )

    os.new_line();

    int indexOffset( offset );
    assert( fi.size() == res.items.size() );

    ofs<<"  //!< count: "<<fi.size()<<"\n";
    offset += dump_int( os, fi.size() );
    os.new_line();
    //!<
    //!< print index table
    //!<
    for( int i=0; i < fi.size(); ++i ) {
      assert( fi[i].size() == res.items[i].items.size() );

      offsets_t & offs( fi[i] );
      detail::files & fl( res.items[i] );

      ofs<<"  //!<  index: "<<fl.prefix<<"\n";
      os.line_count = 0;
      offset += dump_string( os, fl.prefix );
      os.new_line();

      offset += dump_int( os, offs.size() );
      os.new_line();

      for( int j=0; j < offs.size(); ++j ) {
        std::clog<<"offset:"<<i<<":"<<j<<":"<<offs[j]<<std::endl;

        offset += dump_int( os, offs[j] );
        ofs<<" ";
      }//!< for-each( offset )
      os.new_line();
    }//!< for-each( file index )

    os.new_line();
    ofs<<"  //!< Index offset\n";
    offset += dump_int( os, indexOffset );
    os.new_line();

    ofs
      <<"}; //!< size: "<<std::dec<<offset<<" ("<<std::hex<<offset<<")"
      <<"\n\n"
      <<"DS_RESOURCE_REGISTER_COMPILED( "<<varName<<" )\n"
      ;
    return true;
  }//generate_resource_source()

}//namespace detail

/**
 *  |------------------------------|
 *  |    4-bytes version number    |
 *  |------------------------------|
 *  |                              |
 *  |       data of files:         |
 *  |                              |
 *  |   0xXXXXX1: images/a.png     |
 *  |   0xXXXXX2: images/b.png     |
 *  |   0xXXXXX3: info/a.txt       |
 *  |   0xXXXXX4: info/b.txt       |
 *  |                              |
 *  |------------------------------|
 *  |   index: count=2 (4-bytes)   |
 *  |       files: /               |
 *  |         int: size            |
 *  |         file: 0xXXXXX1       |
 *  |         file: 0xXXXXX2       |
 *  |       files: /test01         |
 *  |         int: size            |
 *  |         file: 0xXXXXX3       |
 *  |         file: 0xXXXXX4       |
 *  |------------------------------|
 *  |     4-bytes index offset     |
 *  |------------------------------|
 *
 *  Files are in this format:
 *     [ 1-byte entry type ]
 *     + <name>
 *     + [4-bytes length -> N] + [N-bytes data]
 *
 *  Index entries are in this format:
 *     [ 1-byte entry type ]
 *     + <name:prefix>
 *     + [ 4-bytes list size ]
 *     + [ list of offsets, 4-bytes for each ]
 *
 *  Names are in this format:
 *     [4-bytes length -> N] + [N-bytes name]
 */

int main( int ac, char** av )
{
  fs::path exePath( av[0], fs::native );
#if 0
  fs::path & p( exePath );
  std::clog<<"***** boost::path *****"<<std::endl;
  std::clog<<"  raw:"<<av[0]<<std::endl;
  std::clog<<"  intial_path: "<<fs::initial_path()<<std::endl;
  std::clog<<"  current_path: "<<fs::current_path()<<std::endl;
  std::clog<<"  path: "<<p<<std::endl;
  std::clog<<"  directory_string: "<<p.directory_string()<<std::endl;
  std::clog<<"  root_path: "<<p.root_path()<<std::endl;
  std::clog<<"  root_name: "<<p.root_name()<<std::endl;
  std::clog<<"  root_directory: "<<p.root_directory()<<std::endl;
  std::clog<<"  relative_path: "<<p.relative_path()<<std::endl;
  std::clog<<"  parent_path: "<<p.parent_path()<<std::endl;
  std::clog<<"  branch_path: "<<p.branch_path()<<std::endl;
  std::clog<<"  leaf: "<<p.leaf()<<std::endl;
  std::clog<<"  stem: "<<p.stem()<<std::endl;
  std::clog<<"  filename: "<<p.stem()<<std::endl;
  std::clog<<"***** boost::path end *****"<<std::endl;
#endif

  //!< Do not generate nodes for formatting spaces
  xmlKeepBlanksDefault( 0 );

  po::variables_map vm;
  try {
    po::options_description desc( "Generic" );
    desc.add_options()
      ( "version,v", "Prints the version info." )
      ( "help,h", "Prints the usage message." )
      ( "output,o", po::value<std::string>(), "Place the output into a file." )
      ( "input,i", po::value<std::string>(), "Specify the input file." )
      ;
    po::positional_options_description p;
    p.add( "input", -1 );
    po::store( po::command_line_parser(ac, av).
               options(desc).positional(p).run(), vm );
    po::notify( vm );

    if ( vm.count("help") ) {
      std::cout<<desc<<std::endl;
      return 0;
    }
  }
  catch( const std::exception & e ) {
    std::cout<<e.what()<<std::endl;
    return 0;
  }

  if ( vm.count("version") ) {
    std::cout<<"dscr, version 1.0"<<std::endl;
    return 0;
  }

  std::string in;
  if ( vm.count("input") <= 0 ) {
    std::clog<<"Input file required."<<std::endl;
    return 0;
  }
  in = vm[ "input" ].as<std::string>();

  std::string out( "a.cpp" );
  if ( vm.count("output") ) {
    out = vm[ "output" ].as<std::string>();
  }

  fs::path inPath( in );
  detail::resource res;
  res.path = inPath.parent_path();

  if ( !detail::parse_resource_file( res, in.c_str() ) ) {
    dsD("parse 'test.xml' error");
    goto cleanup;
  }
  if ( !detail::generate_resource_source( res, out ) ) {
    dsD("error: generate resource source");
    goto cleanup;
  }

 cleanup:
  //!< Cleanup everything before quitting
  xmlCleanupParser();
  return 0;
}
