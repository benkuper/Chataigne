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

#ifndef PSN_ENCODER_IMPL_HPP
#define PSN_ENCODER_IMPL_HPP

#include "psn_encoder.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace psn
{

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline psn_encoder::
psn_encoder( const ::std::string & system_name /*= ""*/ )
    : system_name_( system_name )
    , info_frame_id( 0 )
    , data_frame_id( 0 )
{
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::std::list< ::std::string >
inline psn_encoder::
encode_info( const tracker_map & trackers , uint64_t timestamp_usec )
{
    ::std::list< ::std::string > packets ;
    auto tracker_it = trackers.begin() ;
    size_t packet_count_offset = 0 ;

    info_frame_id++ ;

    while ( tracker_it != trackers.end() )
    {
        char buffer[ MAX_UDP_PACKET_SIZE ] ;
        packet_t packet( buffer , MAX_UDP_PACKET_SIZE ) ;

        // Main chunk header
        chunk_header * main_chunk = fill_chunk_header( packet , INFO_PACKET , true , 0 /* to be computed*/ ) ;
        if ( !main_chunk ) break ;

        // Packet header
        packet_header * packet_header = fill_packet_header( packet , INFO_PACKET_HEADER , info_frame_id , timestamp_usec ) ;
        if ( !packet_header ) break ;
        packet_count_offset = (char *)&packet_header->frame_packet_count - buffer ;

        // System name
        if ( !fill_string( packet , INFO_SYSTEM_NAME , system_name_ ) )
            break ;

        // Tracker list
        chunk_header * tracker_list_chunk = fill_chunk_header( packet , INFO_TRACKER_LIST , true , 0 /* to be computed*/ ) ;
        if ( !tracker_list_chunk ) break ;

        // Trackers
        while ( tracker_it != trackers.end() )
        {
            packet_t backup_packet = packet ; // Used to backtrack if there is not enough space to encode the tracker
            const ::psn::tracker & tracker = tracker_it->second ;

            // Tracker chunk
            chunk_header * tracker_chunk = fill_chunk_header( packet , tracker_it->first , true , 0 /* to be computed*/ ) ;
            
            if ( !tracker_chunk )
            {
                packet = backup_packet ;
                break ;
            }

            // Tracker name
            if ( !fill_string( packet , INFO_TRACKER_NAME , tracker.get_name() ) )
            {
                packet = backup_packet ;
                break ;
            }

            tracker_chunk->data_len = packet.buffer - ( (char *)tracker_chunk + sizeof( chunk_header ) ) ;
            tracker_list_chunk->data_len += tracker_chunk->data_len + sizeof( chunk_header ) ;

            ++tracker_it ;
        }

        main_chunk->data_len = packet.buffer - ( buffer + sizeof( chunk_header ) ) ;

        packets.push_back( ::std::string( buffer , MAX_UDP_PACKET_SIZE - packet.size ) ) ;
    }

    // Apply packet count
    for ( auto it = packets.begin() ; it != packets.end() ; ++it )
        if ( it->size() > packet_count_offset )
            (*it)[ packet_count_offset ] = (char)packets.size() ;

    return packets ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::std::list< ::std::string >
inline psn_encoder::
encode_data( const tracker_map & trackers , uint64_t timestamp_usec ) 
{
    ::std::list< ::std::string > packets ;
    auto tracker_it = trackers.begin() ;
    size_t packet_count_offset = 0 ;

    data_frame_id++ ;

    while ( tracker_it != trackers.end() )
    {
        char buffer[ MAX_UDP_PACKET_SIZE ] ;
        packet_t packet( buffer , MAX_UDP_PACKET_SIZE ) ;

        // Main chunk header
        chunk_header * main_chunk = fill_chunk_header( packet , DATA_PACKET , true , 0 /*to be computed*/ ) ;
        if ( !main_chunk ) break ;

        // Packet header
        packet_header * packet_header = fill_packet_header( packet , DATA_PACKET_HEADER , data_frame_id , timestamp_usec ) ;
        if ( !packet_header ) break ;
        packet_count_offset = (char *)&packet_header->frame_packet_count - buffer ;

        // Tracker list
        chunk_header * tracker_list_chunk = fill_chunk_header( packet , DATA_TRACKER_LIST , true , 0 /*to be computed*/ ) ;
        if ( !tracker_list_chunk ) break ;

        // Trackers
        while ( tracker_it != trackers.end() )
        {
            packet_t backup_packet = packet ; // Used to backtrack if there is not enough space to encode the tracker
            const ::psn::tracker & tracker = tracker_it->second ;

            // Tracker chunk
            chunk_header * tracker_chunk = fill_chunk_header( packet , tracker_it->first , true , 0 /*to be computed*/ ) ;
            
            if ( !tracker_chunk )
            {
                packet = backup_packet ;
                break ;
            }

            // Tracker fields
            if ( tracker.is_pos_set()        && !fill_tracker_field( packet , DATA_TRACKER_POS ,       tracker.get_pos() ) ||
                 tracker.is_speed_set()      && !fill_tracker_field( packet , DATA_TRACKER_SPEED ,     tracker.get_speed() ) ||
                 tracker.is_ori_set()        && !fill_tracker_field( packet , DATA_TRACKER_ORI ,       tracker.get_ori() ) ||
                 tracker.is_status_set()     && !fill_tracker_field( packet , DATA_TRACKER_STATUS ,    tracker.get_status() ) ||
                 tracker.is_accel_set()      && !fill_tracker_field( packet , DATA_TRACKER_ACCEL ,     tracker.get_accel() ) ||
                 tracker.is_target_pos_set() && !fill_tracker_field( packet , DATA_TRACKER_TRGTPOS ,   tracker.get_target_pos() ) ||
                 tracker.is_timestamp_set()  && !fill_tracker_field( packet , DATA_TRACKER_TIMESTAMP , tracker.get_timestamp() ) )
            {
                packet = backup_packet ;
                break ;
            }

            tracker_chunk->data_len = packet.buffer - ( (char *)tracker_chunk + sizeof( chunk_header ) ) ;
            tracker_list_chunk->data_len += tracker_chunk->data_len + sizeof( chunk_header ) ;
            ++tracker_it ;
        }

        main_chunk->data_len = packet.buffer - ( buffer + sizeof( chunk_header ) ) ;

        packets.push_back( ::std::string( buffer , MAX_UDP_PACKET_SIZE - packet.size ) ) ;
    }

    // Apply packet count
    for ( auto it = packets.begin() ; it != packets.end() ; ++it )
        if ( it->size() > packet_count_offset )
            (*it)[ packet_count_offset ] = (char)packets.size() ;

    return packets ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline chunk_header *
psn_encoder::
fill_chunk_header( packet_t & packet , uint16_t id , bool has_subchunks , size_t data_len )
{
    chunk_header * chunk = packet.cast_to< chunk_header >() ;

    if ( !chunk )
        return nullptr ;

    chunk->id  = id ;
    chunk->has_subchunks = has_subchunks ? 1 : 0 ;
    chunk->data_len = data_len ;
    packet.apply_offset( sizeof( chunk_header ) ) ;

    return chunk ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline packet_header *
psn_encoder::
fill_packet_header( packet_t & packet , uint16_t chunk_id , uint8_t frame_id , uint64_t timestamp_usec )
{
    // Chunk header
    if ( !fill_chunk_header( packet , chunk_id , false , sizeof( packet_header ) ) )
        return nullptr ;

    // Packet header
    packet_header * header = packet.cast_to< packet_header >() ;
    
    if ( !header )
        return nullptr ;

    header->frame_id = frame_id ;
    header->timestamp_usec = timestamp_usec ;
    header->version_high = HIGH_VERSION ;
    header->version_low = LOW_VERSION ;
    packet.apply_offset( sizeof( packet_header ) ) ;

    return header ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template< typename type >
inline bool
psn_encoder::
fill_tracker_field( packet_t & packet , uint16_t id , type const & value )
{
    // Chunk header
    if ( !fill_chunk_header( packet , id , false , sizeof( type ) ) )
        return false ;

    // Field
    auto data = packet.cast_to< type >() ;
    
    if ( !data )
        return false ;

    *data = value ;
    packet.apply_offset( sizeof( type ) ) ;

    return true ;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline bool
psn_encoder::
fill_string( packet_t & packet , uint16_t id , const ::std::string & str )
{
    // Chunk header
    if ( !fill_chunk_header( packet , id , false , str.length() ) )
        return false ;

    // String
    if ( packet.size < str.length() )
        return false ;

    for ( size_t i = 0 ; i < str.length() ; ++i )
        packet.buffer[ i ] = str[ i ] ;

    packet.apply_offset( str.length() ) ;

    return true ;
}


} // namespace psn

#endif
