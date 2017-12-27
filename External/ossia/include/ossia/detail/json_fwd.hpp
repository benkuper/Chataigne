#pragma once

namespace rapidjson
{
class CrtAllocator;
template <typename Encoding, typename Allocator>
class GenericStringBuffer;
template<typename CharType>
struct UTF8;
using StringBuffer = GenericStringBuffer<UTF8<char>, CrtAllocator>;
}
