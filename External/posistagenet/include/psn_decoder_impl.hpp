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

#ifndef PSN_DECODER_IMPL_HPP
#define PSN_DECODER_IMPL_HPP

#include "psn_decoder.hpp"
#include <utility>

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace psn
{
    
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline psn_decoder::
psn_decoder( void )
    : info_packet_count_( 0 )
    , data_packet_count_( 0 )
{
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_decoder::
decode( const char * packet_ptr , size_t packet_size )
{
    packet_t packet( packet_ptr , packet_size ) ;
    
    auto header = packet.cast_to< const chunk_header >() ;

    if ( !header )
        return false ;

    packet.apply_offset( sizeof( chunk_header ) ) ;

    switch ( header->id )
    {
    case INFO_PACKET: return decode_info( packet , *header ) ;
    case DATA_PACKET: return decode_data( packet , *header ) ;
    }

    return true ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_decoder::
decode_info( packet_t packet , const chunk_header & header )
{
    bool success = decode_children( packet , header ,
        [this]( packet_t packet , const chunk_header & child_header )
        {
            switch ( child_header.id )
            {
            case INFO_PACKET_HEADER: return decode_info_header( packet ) ;
            case INFO_SYSTEM_NAME:   return decode_string( packet , child_header , info_to_commit_.system_name ) ;
            case INFO_TRACKER_LIST:  return decode_info_tracker_list( packet , child_header ) ;
            }
            return true ;
        } ) ;

    if ( ++info_packet_count_ >= info_to_commit_.header.frame_packet_count )
    {
        info_ = ::std::move( info_to_commit_ ) ;
        info_to_commit_ = info_t() ;
        info_packet_count_ = 0 ;
    }

    return success ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_decoder::
decode_info_header( packet_t packet )
{
    uint8_t last_frame_id = data_to_commit_.header.frame_id ;
    bool expect_new_frame = ( data_packet_count_ == 0 ) ;

    if ( !decode_type( packet , info_to_commit_.header ) )
        return false ;

    // Backup solution in case frame_packet_count is bad or we missed a packet
    if ( info_to_commit_.header.frame_id != last_frame_id && !expect_new_frame )
    {
        info_ = info_to_commit_ ;
        info_packet_count_ = 0 ;
    }
                
    return true ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_decoder::
decode_info_tracker_list( packet_t packet , const chunk_header & header )
{
    return decode_children( packet , header ,
        [this]( packet_t packet , const chunk_header & child_header )
        {
            return decode_info_tracker( packet , child_header ) ;
        } ) ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_decoder::
decode_info_tracker( packet_t packet , const chunk_header & header )
{
    ::std::string & tracker_name = info_to_commit_.tracker_names[ header.id ] ;

    return decode_children( packet , header ,
        [&]( packet_t packet , const chunk_header & child_header )
        {
            if ( child_header.id == INFO_TRACKER_NAME )
                return decode_string( packet , child_header , tracker_name ) ;
            return true ;
        } ) ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_decoder::
decode_data( packet_t packet , const chunk_header & header )
{
    bool success = decode_children( packet , header ,
        [this]( packet_t packet , const chunk_header & child_header )
        {
            switch ( child_header.id )
            {
            case DATA_PACKET_HEADER: return decode_data_header( packet ) ;
            case DATA_TRACKER_LIST:  return decode_data_tracker_list( packet , child_header ) ;
            }
            return true ;
        } ) ;

    if ( ++data_packet_count_ >= data_to_commit_.header.frame_packet_count )
    {
        data_ = ::std::move( data_to_commit_ ) ;
        data_to_commit_.trackers.clear() ;
        data_packet_count_ = 0 ;
    }

    return success ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_decoder::
decode_data_header( packet_t packet )
{
    uint8_t last_frame_id = data_to_commit_.header.frame_id ;
    bool expect_new_frame = ( data_packet_count_ == 0 ) ;
                
    if ( !decode_type( packet , data_to_commit_.header ) )
        return false ;

    // Backup solution in case frame_packet_count is bad or we missed a packet
    if ( data_to_commit_.header.frame_id != last_frame_id && !expect_new_frame )
    {
        data_ = data_to_commit_ ;
        data_packet_count_ = 0 ;
    }

    return true ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_decoder::
decode_data_tracker_list( packet_t packet , const chunk_header & header )
{
    return decode_children( packet , header ,
        [this]( packet_t packet , const chunk_header & child_header )
        {
            return decode_data_tracker( packet , child_header ) ;
        } ) ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_decoder::
decode_data_tracker( packet_t packet , const chunk_header & header )
{
    tracker & tracker = data_to_commit_.trackers[ header.id ] ;
    tracker = ::psn::tracker( header.id , info_.tracker_names[ header.id ] ) ;

    return decode_children( packet , header ,
        [&]( packet_t packet , const chunk_header & child_header )
        {
            bool success = true ;
            switch( child_header.id )
            {
            case DATA_TRACKER_POS:       { float3 val ;   success = decode_type( packet , val ) ; tracker.set_pos( val ) ; break ; }
            case DATA_TRACKER_SPEED:     { float3 val ;   success = decode_type( packet , val ) ; tracker.set_speed( val ) ; break ; }
            case DATA_TRACKER_ORI:       { float3 val ;   success = decode_type( packet , val ) ; tracker.set_ori( val ) ; break ; }
            case DATA_TRACKER_STATUS:    { float val ;    success = decode_type( packet , val ) ; tracker.set_status( val ) ; break ; }
            case DATA_TRACKER_ACCEL:     { float3 val ;   success = decode_type( packet , val ) ; tracker.set_accel( val ) ; break ; }
            case DATA_TRACKER_TRGTPOS:   { float3 val ;   success = decode_type( packet , val ) ; tracker.set_target_pos( val ) ; break ; }
            case DATA_TRACKER_TIMESTAMP: { uint64_t val ; success = decode_type( packet , val ) ; tracker.set_timestamp( val ) ; break ; }
            }
            return success ;
        } ) ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template< typename type >
inline bool
psn_decoder::
decode_type( packet_t packet , type & result )
{
    if ( auto data = packet.cast_to< const type >() )
    {
        result = *data ;
        return true ;
    }

    return false ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_decoder::
decode_string( packet_t packet , const chunk_header & header , ::std::string & result )
{
    if ( header.data_len > packet.size )
        return false ;

    result.assign( packet.buffer , header.data_len ) ;
    return true ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_decoder::
decode_children( packet_t packet , const chunk_header & header , const decode_child_t & decode_child )
{
    size_t decoded_size = 0 ;

    while( decoded_size < header.data_len )
    {
        auto child_header = packet.cast_to< const chunk_header >() ;

        if ( !child_header )
            return false ;

        packet.apply_offset( sizeof( chunk_header ) ) ;

        if ( !decode_child( packet , *child_header ) )
            return false ;

        packet.apply_offset( child_header->data_len ) ;
        decoded_size += child_header->data_len + sizeof( chunk_header ) ;
    }

    return true ;
}

} // namespace psn

#endif
