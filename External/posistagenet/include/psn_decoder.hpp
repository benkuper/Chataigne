//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 VYV Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
**/
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#ifndef PSN_DECODER_HPP
#define PSN_DECODER_HPP

#include "psn_defs.hpp"
#include <algorithm>
#include <functional>
#include <map>
#include <string>

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace psn
{

class psn_decoder
{
public:
    struct info_t
    {
        packet_header header ;
        ::std::string system_name ;
        ::std::map< int , ::std::string > tracker_names ;
    } ;

    struct data_t
    {
        packet_header header ;
        tracker_map trackers ;
    } ;

public :
    psn_decoder( void ) ;

    bool decode( const char * packet , size_t packet_size ) ;

    const info_t & get_info( void ) const { return info_ ; }
    const data_t & get_data( void ) const { return data_ ; }

private:
    typedef ::psn::packet< const char > packet_t ;
    typedef ::std::function< bool( packet_t , const chunk_header & ) > decode_child_t ;

    // Info packet
    bool decode_info( packet_t packet , const chunk_header & header ) ;
    bool decode_info_header( packet_t packet ) ;
    bool decode_info_tracker_list( packet_t packet , const chunk_header & header ) ;
    bool decode_info_tracker( packet_t packet , const chunk_header & header ) ;

    // Data packet
    bool decode_data( packet_t packet , const chunk_header & header ) ;
    bool decode_data_header( packet_t packet ) ;
    bool decode_data_tracker_list( packet_t packet , const chunk_header & header ) ;
    bool decode_data_tracker( packet_t packet , const chunk_header & header ) ;

    // Generic
    template< typename type >
    bool decode_type( packet_t packet , type & result ) ;
    bool decode_string( packet_t packet , const chunk_header & header , ::std::string & result ) ;
    bool decode_children( packet_t packet , const chunk_header & header , const decode_child_t & decode_child ) ;

private:
    size_t info_packet_count_ ;
    info_t info_ ;
    info_t info_to_commit_ ;
    
    size_t data_packet_count_ ;
    data_t data_ ;
    data_t data_to_commit_ ;
} ;

} // namespace psn

#endif