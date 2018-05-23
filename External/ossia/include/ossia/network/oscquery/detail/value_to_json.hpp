#pragma once
#include <ossia/detail/json.hpp>
#include <ossia/network/value/value.hpp>
#include <oscpack/osc/OscTypes.h>

namespace ossia
{
namespace oscquery
{
namespace detail
{

// TODO base64 encode
struct value_to_json
{
  rapidjson::Writer<rapidjson::StringBuffer>& writer;
  void operator()(impulse) const
  {
    writer.Null();
  }
  void operator()(int v) const
  {
    writer.Int(v);
  }
  void operator()(float v) const
  {
    writer.Double(v);
  }
  void operator()(bool v) const
  {
    writer.Bool(v);
  }
  void operator()(char v) const
  {
    write_json(writer, v);
  }
  void operator()(const std::string& v) const
  {
    // TODO handle base 64
    // bool b = Base64::Encode(get<coppa::Generic>(val).buf, &out);
    writer.String(v);
  }

  template <std::size_t N>
  void operator()(const std::array<float, N>& vec) const
  {
    writer.StartArray();
    for (std::size_t i = 0; i < N; i++)
    {
      writer.Double(vec[i]);
    }
    writer.EndArray();
  }

  void operator()(const std::vector<ossia::value>& vec) const
  {
    writer.StartArray();
    for (const auto& sub : vec)
    {
      sub.apply(*this);
    }
    writer.EndArray();
  }
  void operator()() const
  {
    throw std::runtime_error("value_to_json: no type");
  }
};

struct value_to_json_value
{
  rapidjson::Document::AllocatorType& allocator;
  rapidjson::Value operator()(impulse) const
  {
    return rapidjson::Value{};
  }
  rapidjson::Value operator()(int v) const
  {
    return rapidjson::Value{v};
  }
  rapidjson::Value operator()(float v) const
  {
    return rapidjson::Value{v};
  }
  rapidjson::Value operator()(bool v) const
  {
    return rapidjson::Value{v};
  }
  rapidjson::Value operator()(char v) const
  {
    return rapidjson::Value{&v, 1, allocator};
  } // 1-char string
  rapidjson::Value operator()(const std::string& v) const
  {
    return rapidjson::Value{v, allocator};
  }

  template <std::size_t N>
  rapidjson::Value operator()(const std::array<float, N>& vec) const
  {
    rapidjson::Value v(rapidjson::kArrayType);
    for (std::size_t i = 0; i < N; i++)
    {
      v.PushBack(vec[i], allocator);
    }
    return v;
  }

  rapidjson::Value operator()(const std::vector<ossia::value>& vec) const
  {
    rapidjson::Value v(rapidjson::kArrayType);
    for (std::size_t i = 0; i < vec.size(); i++)
    {
      v.PushBack(vec[i].apply(*this), allocator);
    }
    return v;
  }
  rapidjson::Value operator()() const
  {
    throw std::runtime_error("value_to_json_value: no type");
  }
};

struct json_to_value
{
  const rapidjson::Value& val;
  ossia::string_view& typetags;
  int& typetag_cursor;
  bool operator()(impulse) const
  {
    typetag_cursor++;
    return val.IsNull();
  }

  bool operator()(int& res) const
  {
    typetag_cursor++;

    bool b = val.IsInt();
    if (b)
      res = val.GetInt();
    return b;
  }

  bool operator()(float& res) const
  {
    typetag_cursor++;

    bool b = val.IsNumber();
    if (b)
      res = (float)val.GetDouble();
    return b;
  }

  bool operator()(bool& res) const
  {
    typetag_cursor++;

    bool b = val.IsBool();
    if (b)
      res = val.GetBool();
    return b;
  }

  bool operator()(char& res) const
  {
    typetag_cursor++;

    bool b = val.IsString() && val.GetStringLength() > 0;
    if (b)
      res = val.GetString()[0];
    return b;
  }

  bool operator()(std::string& res) const
  {
    typetag_cursor++;
    // TODO handle base 64
    // bool b = Base64::Encode(get<coppa::Generic>(val).buf, &out);

    bool b = val.IsString();
    if (b)
      res = std::string(val.GetString(), val.GetStringLength());
    return b;
  }

  template <std::size_t N>
  bool operator()(std::array<float, N>& res) const
  {
    typetag_cursor += N;
    bool b = val.IsArray();
    if (b)
    {
      auto arr = val.GetArray();
      if (arr.Size() == N)
      {
        for (int i = 0; i < (int)N; i++)
        {
          if (arr[i].IsNumber())
          {
            res[i] = arr[i].GetDouble();
          }
          else
          {
            b = false;
            break;
          }
        }
      }
      else
      {
        b = false;
      }
    }
    return b;
  }

  bool handleVecElement(
      const rapidjson::Value& elt, std::vector<ossia::value>& res) const
  {
    if ((int)typetags.size() > typetag_cursor)
    {
      switch (typetags[typetag_cursor])
      {
        case oscpack::TypeTagValues::INFINITUM_TYPE_TAG:
        {
          ossia::impulse i;
          if (!json_to_value{elt, typetags, typetag_cursor}(i))
            return false;

          res.push_back(i);
          return true;
        }
        case oscpack::TypeTagValues::INT32_TYPE_TAG:
        {
          int32_t i{};
          if (!json_to_value{elt, typetags, typetag_cursor}(i))
            return false;

          res.push_back(i);
          return true;
        }
        case oscpack::TypeTagValues::FLOAT_TYPE_TAG:
        {
          float i{};
          if (!json_to_value{elt, typetags, typetag_cursor}(i))
            return false;

          res.push_back(i);
          return true;
        }
        case oscpack::TypeTagValues::CHAR_TYPE_TAG:
        {
          char i{};
          if (!json_to_value{elt, typetags, typetag_cursor}(i))
            return false;

          res.push_back(i);
          return true;
        }

        case oscpack::TypeTagValues::TRUE_TYPE_TAG:
        case oscpack::TypeTagValues::FALSE_TYPE_TAG:
        {
          bool i{};
          if (!json_to_value{elt, typetags, typetag_cursor}(i))
            return false;

          res.push_back(i);
          return true;
        }

        case oscpack::TypeTagValues::STRING_TYPE_TAG:
        case oscpack::TypeTagValues::SYMBOL_TYPE_TAG:
        {
          std::string i;
          if (!json_to_value{elt, typetags, typetag_cursor}(i))
            return false;

          res.push_back(std::move(i));
          return true;
        }

        case oscpack::TypeTagValues::ARRAY_BEGIN_TYPE_TAG:
        {
          std::vector<ossia::value> i;
          ++typetag_cursor; // We skip the '['
          if (!json_to_value{elt, typetags, typetag_cursor}(i))
            return false;

          ++typetag_cursor; // We skip the ']'
          res.push_back(std::move(i));
          return true;
        }
        case oscpack::TypeTagValues::ARRAY_END_TYPE_TAG:
        default:
        {
          // We should never end up here
          return false;
        }
      }
    }
    else
    {
      return false;
    }
  }

  bool operator()(std::vector<ossia::value>& res) const
  {
    // TODO read from the typetag
    bool b = val.IsArray();
    if (b)
    {
      auto arr = val.GetArray();
      for (const auto& elt : arr)
      {
        if (!handleVecElement(elt, res))
          return false;
      }
    }
    return b;
  }

  bool operator()() const
  {
    throw std::runtime_error("json_to_value: no type");
  }
};

inline ossia::value ReadValue(const rapidjson::Value& val)
{
  switch (val.GetType())
  {
    case rapidjson::kNumberType:
    {
      if (val.IsInt())
        return val.GetInt();
      else if (val.IsUint())
        return (int)val.GetUint();
      // There is also int64 and uint64 but we'll get a better approximation
      // with double
      else
        return val.GetDouble();
    }
    case rapidjson::kFalseType:
      return false;
    case rapidjson::kTrueType:
      return true;

    case rapidjson::kArrayType:
    {
      std::vector<ossia::value> tpl;
      tpl.reserve(val.Size());
      for (auto& elt : val.GetArray())
      {
        tpl.push_back(ReadValue(elt));
      }
      return tpl;
    }

    case rapidjson::kStringType:
      return get_string(val);

    case rapidjson::kObjectType:
    case rapidjson::kNullType:
    default:
      return ossia::impulse{};
  }
}

struct json_to_value_unchecked
{
  const rapidjson::Value& val;
  void operator()(impulse) const
  {
  }

  void operator()(int& res) const
  {
    if (val.IsInt())
      res = val.GetInt();
  }

  void operator()(float& res) const
  {
    if (val.IsNumber())
      res = (float)val.GetDouble();
  }

  void operator()(bool& res) const
  {
    if (val.IsBool())
      res = val.GetBool();
  }

  void operator()(char& res) const
  {
    if (val.IsString() && val.GetStringLength() > 0)
      res = val.GetString()[0];
  }

  void operator()(std::string& res) const
  {
    // TODO handle base 64
    // bool b = Base64::Encode(get<coppa::Generic>(val).buf, &out);

    if (val.IsString())
      res = get_string(val);
  }

  template <std::size_t N>
  void operator()(std::array<float, N>& res) const
  {
    if (val.IsArray())
    {
      auto arr = val.GetArray();
      if (arr.Size() == N)
      {
        for (int i = 0; i < (int)N; i++)
        {
          res[i] = arr[i].GetDouble();
        }
      }
    }
  }

  void operator()(std::vector<ossia::value>& res) const
  {
    if (val.IsArray())
    {
      res.clear();
      auto arr = val.GetArray();
      for (const auto& elt : arr)
      {
        res.push_back(ReadValue(elt));
      }
    }
  }

  void operator()() const
  {
    throw std::runtime_error("json_to_value_unchecked: no type");
  }
};
}
}
}
