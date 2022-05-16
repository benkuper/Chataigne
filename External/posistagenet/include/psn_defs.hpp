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

#ifndef PSN_DEFS_HPP
#define PSN_DEFS_HPP

#include <cstdint>
#include <map>
#include <string>

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace psn
{
    
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PSN Version
const uint8_t HIGH_VERSION            = 2 ;
const uint8_t LOW_VERSION             = 0 ;

// Default UDP Port and Multicast Address
const uint16_t DEFAULT_UDP_PORT       = 56565 ;
const ::std::string DEFAULT_UDP_MULTICAST_ADDR( "236.10.10.10" ) ;

// Max UDP Packet Size
const uint16_t MAX_UDP_PACKET_SIZE    = 1500 ;

// PSN Info
const uint16_t INFO_PACKET            = 0x6756 ;
const uint16_t INFO_PACKET_HEADER     = 0x0000 ;
const uint16_t INFO_SYSTEM_NAME       = 0x0001 ;
const uint16_t INFO_TRACKER_LIST      = 0x0002 ;
const uint16_t INFO_TRACKER_NAME      = 0x0000 ;

// PSN Data
const uint16_t DATA_PACKET            = 0x6755 ;
const uint16_t DATA_PACKET_HEADER     = 0x0000 ;
const uint16_t DATA_TRACKER_LIST      = 0x0001 ;
const uint16_t DATA_TRACKER_POS       = 0x0000 ;
const uint16_t DATA_TRACKER_SPEED     = 0x0001 ;
const uint16_t DATA_TRACKER_ORI       = 0x0002 ;
const uint16_t DATA_TRACKER_STATUS    = 0x0003 ;
const uint16_t DATA_TRACKER_ACCEL     = 0x0004 ;
const uint16_t DATA_TRACKER_TRGTPOS   = 0x0005 ;
const uint16_t DATA_TRACKER_TIMESTAMP = 0x0006 ;
    
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// float3
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
struct float3
{
    float3( float px = 0 , float py = 0 , float pz = 0 ) 
        : x( px ) , y( py ) , z( pz )
    {}

    float x , y , z ;
} ;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// tracker
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
struct tracker
{
    tracker( uint16_t id = 0 , const ::std::string & name = "" )
        : id_( id )
        , name_( name )
        , status_( 0 )
        , timestamp_( 0 )
        , fields_( 0 )
    {
        set_field( DATA_TRACKER_POS ) ; // Default field
    }

    void set_id( uint16_t id ) { id_ = id ; }
    uint16_t get_id( void ) const { return id_ ; }

    void set_name( const ::std::string & name ) { name_ = name ; }
    const ::std::string & get_name( void ) const { return name_ ; }

    void set_pos( const float3 & pos ) { pos_ = pos ; set_field( DATA_TRACKER_POS ) ; }
    float3 get_pos( void ) const { return pos_ ; }
    bool is_pos_set( void ) const { return is_field_set( DATA_TRACKER_POS ) ; }
    
    void set_speed( const float3 & speed ) { speed_ = speed ; set_field( DATA_TRACKER_SPEED ) ; }
    float3 get_speed( void ) const { return speed_ ; }
    bool is_speed_set( void ) const { return is_field_set( DATA_TRACKER_SPEED ) ; }
    
    void set_ori( const float3 & ori ) { ori_ = ori ; set_field( DATA_TRACKER_ORI ) ; }
    float3 get_ori( void ) const { return ori_ ; }
    bool is_ori_set( void ) const { return is_field_set( DATA_TRACKER_ORI ) ; }
    
    void set_status( float status ) { status_ = status ; set_field( DATA_TRACKER_STATUS ) ; }
    float get_status( void ) const { return status_ ; }
    bool is_status_set( void ) const { return is_field_set( DATA_TRACKER_STATUS ) ; }
    
    void set_accel( const float3 & accel ) { accel_ = accel ; set_field( DATA_TRACKER_ACCEL ) ; }
    float3 get_accel( void ) const { return accel_ ; }
    bool is_accel_set( void ) const { return is_field_set( DATA_TRACKER_ACCEL ) ; }
    
    void set_target_pos( const float3 & target_pos ) { target_pos_ = target_pos ; set_field( DATA_TRACKER_TRGTPOS ) ; }
    float3 get_target_pos( void ) const { return target_pos_ ; }
    bool is_target_pos_set( void ) const { return is_field_set( DATA_TRACKER_TRGTPOS ) ; }
    
    void set_timestamp( uint64_t timestamp_usec ) { timestamp_ = timestamp_usec ; set_field( DATA_TRACKER_TIMESTAMP ) ; }
    uint64_t get_timestamp( void ) const { return timestamp_ ; }
    bool is_timestamp_set( void ) const { return is_field_set( DATA_TRACKER_TIMESTAMP ) ; }

private:
    void set_field( int field ) { fields_ |= 1 << field ; }
    void unset_field( int field ) { fields_ &= ~( 1 << field ) ; }
    bool is_field_set( int field ) const { return ( fields_ & ( 1 << field ) ) > 0 ; }

    uint16_t id_ ;
    ::std::string name_ ;

    float3 pos_ ;
    float3 speed_ ;
    float3 ori_ ;
    float status_ ;
    float3 accel_ ;
    float3 target_pos_ ;
    uint64_t timestamp_ ;

    uint32_t fields_ ;
} ;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// tracker_map
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
typedef ::std::map< uint16_t , tracker > tracker_map ; // map< id , tracker >

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// chunk_header
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
struct chunk_header
{
    chunk_header( void ) : id( 0 ) , data_len( 0 ) , has_subchunks( 0 ) {}

    uint32_t    id            : 16 ;   // chunk ID
    uint32_t    data_len      : 15 ;   // length of chunk data or sub chunks
    uint32_t    has_subchunks : 1 ;    // does it contain other chunks
} ;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// packet_header
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
class packet_header
{
public :
    packet_header( void )
        : timestamp_usec( 0 )
        , version_high( 0 )
        , version_low( 0 )
        , frame_id( 0 )
        , frame_packet_count( 0 )
    {}

    uint64_t timestamp_usec ;
    uint8_t  version_high ;
    uint8_t  version_low ;
    uint8_t  frame_id ;
    uint8_t  frame_packet_count ;  
} ;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// packet
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
template< typename type >
struct packet
{
    packet( type * b , size_t s )
        : buffer( b )
        , size( s )
    {}

    void apply_offset( size_t offset )
    {
        offset = ::std::min( size , offset ) ;
        buffer += offset ;
        size -= offset ;
    }

    template< typename cast_type >
    cast_type * cast_to( void )
    {
        if ( size < sizeof( cast_type ) )
            return nullptr ;

        return reinterpret_cast< cast_type * >( buffer ) ;
    }

    type * buffer ;
    size_t size ;
} ;

} // namespace psn

#endif
