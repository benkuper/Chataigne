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

#ifndef PSN_ENCODER_HPP
#define PSN_ENCODER_HPP

#include "psn_defs.hpp"
#include <algorithm>
#include <cstdint>
#include <list>
#include <string>

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace psn
{

class psn_encoder
{
public:
    psn_encoder( const ::std::string & system_name = "" ) ;

    ::std::list< ::std::string > encode_info( const tracker_map & trackers , uint64_t timestamp_usec ) ;
    ::std::list< ::std::string > encode_data( const tracker_map & trackers , uint64_t timestamp_usec ) ;

    uint8_t get_last_info_frame_id( void ) const { return info_frame_id ; }
    uint8_t get_last_data_frame_id( void ) const { return data_frame_id ; }

private:
    typedef ::psn::packet< char > packet_t ;

    chunk_header * fill_chunk_header( packet_t & packet , uint16_t id , bool has_subchunks , size_t data_len ) ;
    packet_header * fill_packet_header( packet_t & packet , uint16_t chunk_id , uint8_t frame_id , uint64_t timestamp_usec ) ;

    template< typename type >
    bool fill_tracker_field( packet_t & packet , uint16_t id , type const & value ) ;
    bool fill_string( packet_t & packet , uint16_t id , const ::std::string & str ) ;

private:
    ::std::string system_name_ ;

    uint8_t info_frame_id ;
    uint8_t data_frame_id ;
} ;

} // namespace psn

#endif
