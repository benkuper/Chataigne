struct angle_u
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::degree_u m_value0;

    ossia::radian_u m_value1;

    dummy_t m_dummy;
    Impl() : m_dummy{}
    {
    }
    ~Impl()
    {
    }
  };

  enum Type : int8_t
  {
    Type0,
    Type1,
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
      default:
        break;
    }
  }
  Impl m_impl;
  Type m_type;

public:
  static const constexpr auto npos = Npos;
  int which() const
  {
    return m_type;
  }

  operator bool() const
  {
    return m_type != npos;
  }
  template <typename T>
  const T* target() const;
  template <typename T>
  T* target();
  template <typename T>
  const T& get() const;
  template <typename T>
  T& get();

  template <typename T>
  static Type matching_type();
  angle_u() : m_type{Npos}
  {
  }
  ~angle_u()
  {
    destruct_impl();
  }
  angle_u(ossia::degree_u v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::degree_u{v};
  }
  angle_u(ossia::radian_u v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::radian_u{v};
  }
  angle_u(const angle_u& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::degree_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::radian_u{other.m_impl.m_value1};
        break;
      default:
        break;
    }
  }
  angle_u(angle_u&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::degree_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::radian_u{std::move(other.m_impl.m_value1)};
        break;
      default:
        break;
    }
  }
  angle_u& operator=(const angle_u& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::degree_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::radian_u{other.m_impl.m_value1};
        break;
      default:
        break;
    }
    return *this;
  }
  angle_u& operator=(angle_u&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::degree_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::radian_u{std::move(other.m_impl.m_value1)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::degree_u* angle_u::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::radian_u* angle_u::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::degree_u* angle_u::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::radian_u* angle_u::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::degree_u& angle_u::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::radian_u& angle_u::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::degree_u& angle_u::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::radian_u& angle_u::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const angle_u& var)
{
  switch (var.m_type)
  {
    case angle_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case angle_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, angle_u& var)
{
  switch (var.m_type)
  {
    case angle_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case angle_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, angle_u&& var)
{
  switch (var.m_type)
  {
    case angle_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case angle_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const angle_u& var)
{
  switch (var.m_type)
  {
    case angle_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case angle_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, angle_u& var)
{
  switch (var.m_type)
  {
    case angle_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case angle_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, angle_u&& var)
{
  switch (var.m_type)
  {
    case angle_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case angle_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    default:
      return functor();
  }
}
inline bool operator==(const angle_u& lhs, const angle_u& rhs)
{
  return (lhs.m_type == rhs.m_type);
}
inline bool operator!=(const angle_u& lhs, const angle_u& rhs)
{
  return (lhs.m_type != rhs.m_type);
}
inline bool operator==(const angle_u& lhs, const ossia::degree_u& rhs)
{
  return (lhs.m_type == angle_u::Type::Type0);
}
inline bool operator==(const ossia::degree_u& lhs, const angle_u& rhs)
{
  return (rhs.m_type == angle_u::Type::Type0);
}
inline bool operator!=(const angle_u& lhs, const ossia::degree_u& rhs)
{
  return (lhs.m_type != angle_u::Type::Type0);
}
inline bool operator!=(const ossia::degree_u& lhs, const angle_u& rhs)
{
  return (rhs.m_type != angle_u::Type::Type0);
}
inline bool operator==(const angle_u& lhs, const ossia::radian_u& rhs)
{
  return (lhs.m_type == angle_u::Type::Type1);
}
inline bool operator==(const ossia::radian_u& lhs, const angle_u& rhs)
{
  return (rhs.m_type == angle_u::Type::Type1);
}
inline bool operator!=(const angle_u& lhs, const ossia::radian_u& rhs)
{
  return (lhs.m_type != angle_u::Type::Type1);
}
inline bool operator!=(const ossia::radian_u& lhs, const angle_u& rhs)
{
  return (rhs.m_type != angle_u::Type::Type1);
}
struct color_u
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::argb_u m_value0;

    ossia::rgba_u m_value1;

    ossia::rgb_u m_value2;

    ossia::bgr_u m_value3;

    ossia::argb8_u m_value4;

    ossia::hsv_u m_value5;

    ossia::cmy8_u m_value6;

    ossia::xyz_u m_value7;

    dummy_t m_dummy;
    Impl() : m_dummy{}
    {
    }
    ~Impl()
    {
    }
  };

  enum Type : int8_t
  {
    Type0,
    Type1,
    Type2,
    Type3,
    Type4,
    Type5,
    Type6,
    Type7,
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
      default:
        break;
    }
  }
  Impl m_impl;
  Type m_type;

public:
  static const constexpr auto npos = Npos;
  int which() const
  {
    return m_type;
  }

  operator bool() const
  {
    return m_type != npos;
  }
  template <typename T>
  const T* target() const;
  template <typename T>
  T* target();
  template <typename T>
  const T& get() const;
  template <typename T>
  T& get();

  template <typename T>
  static Type matching_type();
  color_u() : m_type{Npos}
  {
  }
  ~color_u()
  {
    destruct_impl();
  }
  color_u(ossia::argb_u v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::argb_u{v};
  }
  color_u(ossia::rgba_u v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::rgba_u{v};
  }
  color_u(ossia::rgb_u v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::rgb_u{v};
  }
  color_u(ossia::bgr_u v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::bgr_u{v};
  }
  color_u(ossia::argb8_u v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::argb8_u{v};
  }
  color_u(ossia::hsv_u v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::hsv_u{v};
  }
  color_u(ossia::cmy8_u v) : m_type{Type6}
  {
    new (&m_impl.m_value6) ossia::cmy8_u{v};
  }
  color_u(ossia::xyz_u v) : m_type{Type7}
  {
    new (&m_impl.m_value7) ossia::xyz_u{v};
  }
  color_u(const color_u& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::argb_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::rgba_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::rgb_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::bgr_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::argb8_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::hsv_u{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::cmy8_u{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::xyz_u{other.m_impl.m_value7};
        break;
      default:
        break;
    }
  }
  color_u(color_u&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::argb_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::rgba_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::rgb_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::bgr_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::argb8_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::hsv_u{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::cmy8_u{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::xyz_u{std::move(other.m_impl.m_value7)};
        break;
      default:
        break;
    }
  }
  color_u& operator=(const color_u& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::argb_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::rgba_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::rgb_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::bgr_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::argb8_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::hsv_u{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::cmy8_u{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::xyz_u{other.m_impl.m_value7};
        break;
      default:
        break;
    }
    return *this;
  }
  color_u& operator=(color_u&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::argb_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::rgba_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::rgb_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::bgr_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::argb8_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::hsv_u{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::cmy8_u{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::xyz_u{std::move(other.m_impl.m_value7)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::argb_u* color_u::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::rgba_u* color_u::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::rgb_u* color_u::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::bgr_u* color_u::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::argb8_u* color_u::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::hsv_u* color_u::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::cmy8_u* color_u::target() const
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline const ossia::xyz_u* color_u::target() const
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline ossia::argb_u* color_u::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::rgba_u* color_u::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::rgb_u* color_u::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::bgr_u* color_u::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::argb8_u* color_u::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::hsv_u* color_u::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::cmy8_u* color_u::target()
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline ossia::xyz_u* color_u::target()
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline const ossia::argb_u& color_u::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::rgba_u& color_u::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::rgb_u& color_u::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::bgr_u& color_u::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::argb8_u& color_u::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::hsv_u& color_u::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::cmy8_u& color_u::get() const
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::xyz_u& color_u::get() const
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::argb_u& color_u::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::rgba_u& color_u::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::rgb_u& color_u::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::bgr_u& color_u::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::argb8_u& color_u::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::hsv_u& color_u::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::cmy8_u& color_u::get()
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::xyz_u& color_u::get()
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const color_u& var)
{
  switch (var.m_type)
  {
    case color_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case color_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case color_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case color_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case color_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case color_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case color_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case color_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, color_u& var)
{
  switch (var.m_type)
  {
    case color_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case color_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case color_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case color_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case color_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case color_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case color_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case color_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, color_u&& var)
{
  switch (var.m_type)
  {
    case color_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case color_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case color_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case color_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case color_u::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case color_u::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case color_u::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case color_u::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const color_u& var)
{
  switch (var.m_type)
  {
    case color_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case color_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case color_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case color_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case color_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case color_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case color_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case color_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, color_u& var)
{
  switch (var.m_type)
  {
    case color_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case color_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case color_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case color_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case color_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case color_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case color_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case color_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, color_u&& var)
{
  switch (var.m_type)
  {
    case color_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case color_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case color_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case color_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case color_u::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case color_u::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case color_u::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case color_u::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    default:
      return functor();
  }
}
inline bool operator==(const color_u& lhs, const color_u& rhs)
{
  return (lhs.m_type == rhs.m_type);
}
inline bool operator!=(const color_u& lhs, const color_u& rhs)
{
  return (lhs.m_type != rhs.m_type);
}
inline bool operator==(const color_u& lhs, const ossia::argb_u& rhs)
{
  return (lhs.m_type == color_u::Type::Type0);
}
inline bool operator==(const ossia::argb_u& lhs, const color_u& rhs)
{
  return (rhs.m_type == color_u::Type::Type0);
}
inline bool operator!=(const color_u& lhs, const ossia::argb_u& rhs)
{
  return (lhs.m_type != color_u::Type::Type0);
}
inline bool operator!=(const ossia::argb_u& lhs, const color_u& rhs)
{
  return (rhs.m_type != color_u::Type::Type0);
}
inline bool operator==(const color_u& lhs, const ossia::rgba_u& rhs)
{
  return (lhs.m_type == color_u::Type::Type1);
}
inline bool operator==(const ossia::rgba_u& lhs, const color_u& rhs)
{
  return (rhs.m_type == color_u::Type::Type1);
}
inline bool operator!=(const color_u& lhs, const ossia::rgba_u& rhs)
{
  return (lhs.m_type != color_u::Type::Type1);
}
inline bool operator!=(const ossia::rgba_u& lhs, const color_u& rhs)
{
  return (rhs.m_type != color_u::Type::Type1);
}
inline bool operator==(const color_u& lhs, const ossia::rgb_u& rhs)
{
  return (lhs.m_type == color_u::Type::Type2);
}
inline bool operator==(const ossia::rgb_u& lhs, const color_u& rhs)
{
  return (rhs.m_type == color_u::Type::Type2);
}
inline bool operator!=(const color_u& lhs, const ossia::rgb_u& rhs)
{
  return (lhs.m_type != color_u::Type::Type2);
}
inline bool operator!=(const ossia::rgb_u& lhs, const color_u& rhs)
{
  return (rhs.m_type != color_u::Type::Type2);
}
inline bool operator==(const color_u& lhs, const ossia::bgr_u& rhs)
{
  return (lhs.m_type == color_u::Type::Type3);
}
inline bool operator==(const ossia::bgr_u& lhs, const color_u& rhs)
{
  return (rhs.m_type == color_u::Type::Type3);
}
inline bool operator!=(const color_u& lhs, const ossia::bgr_u& rhs)
{
  return (lhs.m_type != color_u::Type::Type3);
}
inline bool operator!=(const ossia::bgr_u& lhs, const color_u& rhs)
{
  return (rhs.m_type != color_u::Type::Type3);
}
inline bool operator==(const color_u& lhs, const ossia::argb8_u& rhs)
{
  return (lhs.m_type == color_u::Type::Type4);
}
inline bool operator==(const ossia::argb8_u& lhs, const color_u& rhs)
{
  return (rhs.m_type == color_u::Type::Type4);
}
inline bool operator!=(const color_u& lhs, const ossia::argb8_u& rhs)
{
  return (lhs.m_type != color_u::Type::Type4);
}
inline bool operator!=(const ossia::argb8_u& lhs, const color_u& rhs)
{
  return (rhs.m_type != color_u::Type::Type4);
}
inline bool operator==(const color_u& lhs, const ossia::hsv_u& rhs)
{
  return (lhs.m_type == color_u::Type::Type5);
}
inline bool operator==(const ossia::hsv_u& lhs, const color_u& rhs)
{
  return (rhs.m_type == color_u::Type::Type5);
}
inline bool operator!=(const color_u& lhs, const ossia::hsv_u& rhs)
{
  return (lhs.m_type != color_u::Type::Type5);
}
inline bool operator!=(const ossia::hsv_u& lhs, const color_u& rhs)
{
  return (rhs.m_type != color_u::Type::Type5);
}
inline bool operator==(const color_u& lhs, const ossia::cmy8_u& rhs)
{
  return (lhs.m_type == color_u::Type::Type6);
}
inline bool operator==(const ossia::cmy8_u& lhs, const color_u& rhs)
{
  return (rhs.m_type == color_u::Type::Type6);
}
inline bool operator!=(const color_u& lhs, const ossia::cmy8_u& rhs)
{
  return (lhs.m_type != color_u::Type::Type6);
}
inline bool operator!=(const ossia::cmy8_u& lhs, const color_u& rhs)
{
  return (rhs.m_type != color_u::Type::Type6);
}
inline bool operator==(const color_u& lhs, const ossia::xyz_u& rhs)
{
  return (lhs.m_type == color_u::Type::Type7);
}
inline bool operator==(const ossia::xyz_u& lhs, const color_u& rhs)
{
  return (rhs.m_type == color_u::Type::Type7);
}
inline bool operator!=(const color_u& lhs, const ossia::xyz_u& rhs)
{
  return (lhs.m_type != color_u::Type::Type7);
}
inline bool operator!=(const ossia::xyz_u& lhs, const color_u& rhs)
{
  return (rhs.m_type != color_u::Type::Type7);
}
struct distance_u
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::meter_u m_value0;

    ossia::kilometer_u m_value1;

    ossia::decimeter_u m_value2;

    ossia::centimeter_u m_value3;

    ossia::millimeter_u m_value4;

    ossia::micrometer_u m_value5;

    ossia::nanometer_u m_value6;

    ossia::picometer_u m_value7;

    ossia::inch_u m_value8;

    ossia::foot_u m_value9;

    ossia::mile_u m_value10;

    dummy_t m_dummy;
    Impl() : m_dummy{}
    {
    }
    ~Impl()
    {
    }
  };

  enum Type : int8_t
  {
    Type0,
    Type1,
    Type2,
    Type3,
    Type4,
    Type5,
    Type6,
    Type7,
    Type8,
    Type9,
    Type10,
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
      default:
        break;
    }
  }
  Impl m_impl;
  Type m_type;

public:
  static const constexpr auto npos = Npos;
  int which() const
  {
    return m_type;
  }

  operator bool() const
  {
    return m_type != npos;
  }
  template <typename T>
  const T* target() const;
  template <typename T>
  T* target();
  template <typename T>
  const T& get() const;
  template <typename T>
  T& get();

  template <typename T>
  static Type matching_type();
  distance_u() : m_type{Npos}
  {
  }
  ~distance_u()
  {
    destruct_impl();
  }
  distance_u(ossia::meter_u v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::meter_u{v};
  }
  distance_u(ossia::kilometer_u v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::kilometer_u{v};
  }
  distance_u(ossia::decimeter_u v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::decimeter_u{v};
  }
  distance_u(ossia::centimeter_u v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::centimeter_u{v};
  }
  distance_u(ossia::millimeter_u v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::millimeter_u{v};
  }
  distance_u(ossia::micrometer_u v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::micrometer_u{v};
  }
  distance_u(ossia::nanometer_u v) : m_type{Type6}
  {
    new (&m_impl.m_value6) ossia::nanometer_u{v};
  }
  distance_u(ossia::picometer_u v) : m_type{Type7}
  {
    new (&m_impl.m_value7) ossia::picometer_u{v};
  }
  distance_u(ossia::inch_u v) : m_type{Type8}
  {
    new (&m_impl.m_value8) ossia::inch_u{v};
  }
  distance_u(ossia::foot_u v) : m_type{Type9}
  {
    new (&m_impl.m_value9) ossia::foot_u{v};
  }
  distance_u(ossia::mile_u v) : m_type{Type10}
  {
    new (&m_impl.m_value10) ossia::mile_u{v};
  }
  distance_u(const distance_u& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::meter_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::kilometer_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::decimeter_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::centimeter_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::millimeter_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::micrometer_u{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::nanometer_u{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::picometer_u{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::inch_u{other.m_impl.m_value8};
        break;
      case Type::Type9:
        new (&m_impl.m_value9) ossia::foot_u{other.m_impl.m_value9};
        break;
      case Type::Type10:
        new (&m_impl.m_value10) ossia::mile_u{other.m_impl.m_value10};
        break;
      default:
        break;
    }
  }
  distance_u(distance_u&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::meter_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::kilometer_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::decimeter_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::centimeter_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::millimeter_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::micrometer_u{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6)
            ossia::nanometer_u{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::picometer_u{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::inch_u{std::move(other.m_impl.m_value8)};
        break;
      case Type::Type9:
        new (&m_impl.m_value9) ossia::foot_u{std::move(other.m_impl.m_value9)};
        break;
      case Type::Type10:
        new (&m_impl.m_value10)
            ossia::mile_u{std::move(other.m_impl.m_value10)};
        break;
      default:
        break;
    }
  }
  distance_u& operator=(const distance_u& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::meter_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::kilometer_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::decimeter_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::centimeter_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::millimeter_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::micrometer_u{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::nanometer_u{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::picometer_u{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::inch_u{other.m_impl.m_value8};
        break;
      case Type::Type9:
        new (&m_impl.m_value9) ossia::foot_u{other.m_impl.m_value9};
        break;
      case Type::Type10:
        new (&m_impl.m_value10) ossia::mile_u{other.m_impl.m_value10};
        break;
      default:
        break;
    }
    return *this;
  }
  distance_u& operator=(distance_u&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::meter_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::kilometer_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::decimeter_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::centimeter_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::millimeter_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::micrometer_u{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6)
            ossia::nanometer_u{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::picometer_u{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::inch_u{std::move(other.m_impl.m_value8)};
        break;
      case Type::Type9:
        new (&m_impl.m_value9) ossia::foot_u{std::move(other.m_impl.m_value9)};
        break;
      case Type::Type10:
        new (&m_impl.m_value10)
            ossia::mile_u{std::move(other.m_impl.m_value10)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::meter_u* distance_u::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::kilometer_u* distance_u::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::decimeter_u* distance_u::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::centimeter_u* distance_u::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::millimeter_u* distance_u::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::micrometer_u* distance_u::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::nanometer_u* distance_u::target() const
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline const ossia::picometer_u* distance_u::target() const
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline const ossia::inch_u* distance_u::target() const
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline const ossia::foot_u* distance_u::target() const
{
  if (m_type == Type9)
    return &m_impl.m_value9;
  return nullptr;
}
template <>
inline const ossia::mile_u* distance_u::target() const
{
  if (m_type == Type10)
    return &m_impl.m_value10;
  return nullptr;
}
template <>
inline ossia::meter_u* distance_u::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::kilometer_u* distance_u::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::decimeter_u* distance_u::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::centimeter_u* distance_u::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::millimeter_u* distance_u::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::micrometer_u* distance_u::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::nanometer_u* distance_u::target()
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline ossia::picometer_u* distance_u::target()
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline ossia::inch_u* distance_u::target()
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline ossia::foot_u* distance_u::target()
{
  if (m_type == Type9)
    return &m_impl.m_value9;
  return nullptr;
}
template <>
inline ossia::mile_u* distance_u::target()
{
  if (m_type == Type10)
    return &m_impl.m_value10;
  return nullptr;
}
template <>
inline const ossia::meter_u& distance_u::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::kilometer_u& distance_u::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::decimeter_u& distance_u::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::centimeter_u& distance_u::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::millimeter_u& distance_u::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::micrometer_u& distance_u::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::nanometer_u& distance_u::get() const
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::picometer_u& distance_u::get() const
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::inch_u& distance_u::get() const
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::foot_u& distance_u::get() const
{
  if (m_type == Type9)
    return m_impl.m_value9;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::mile_u& distance_u::get() const
{
  if (m_type == Type10)
    return m_impl.m_value10;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::meter_u& distance_u::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::kilometer_u& distance_u::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::decimeter_u& distance_u::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::centimeter_u& distance_u::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::millimeter_u& distance_u::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::micrometer_u& distance_u::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::nanometer_u& distance_u::get()
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::picometer_u& distance_u::get()
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::inch_u& distance_u::get()
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::foot_u& distance_u::get()
{
  if (m_type == Type9)
    return m_impl.m_value9;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::mile_u& distance_u::get()
{
  if (m_type == Type10)
    return m_impl.m_value10;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const distance_u& var)
{
  switch (var.m_type)
  {
    case distance_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case distance_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case distance_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case distance_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case distance_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case distance_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case distance_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case distance_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    case distance_u::Type::Type8:
      return functor(var.m_impl.m_value8);
    case distance_u::Type::Type9:
      return functor(var.m_impl.m_value9);
    case distance_u::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, distance_u& var)
{
  switch (var.m_type)
  {
    case distance_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case distance_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case distance_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case distance_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case distance_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case distance_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case distance_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case distance_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    case distance_u::Type::Type8:
      return functor(var.m_impl.m_value8);
    case distance_u::Type::Type9:
      return functor(var.m_impl.m_value9);
    case distance_u::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, distance_u&& var)
{
  switch (var.m_type)
  {
    case distance_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case distance_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case distance_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case distance_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case distance_u::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case distance_u::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case distance_u::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case distance_u::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case distance_u::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    case distance_u::Type::Type9:
      return functor(std::move(var.m_impl.m_value9));
    case distance_u::Type::Type10:
      return functor(std::move(var.m_impl.m_value10));
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const distance_u& var)
{
  switch (var.m_type)
  {
    case distance_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case distance_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case distance_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case distance_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case distance_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case distance_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case distance_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case distance_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    case distance_u::Type::Type8:
      return functor(var.m_impl.m_value8);
    case distance_u::Type::Type9:
      return functor(var.m_impl.m_value9);
    case distance_u::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, distance_u& var)
{
  switch (var.m_type)
  {
    case distance_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case distance_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case distance_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case distance_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case distance_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case distance_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case distance_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case distance_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    case distance_u::Type::Type8:
      return functor(var.m_impl.m_value8);
    case distance_u::Type::Type9:
      return functor(var.m_impl.m_value9);
    case distance_u::Type::Type10:
      return functor(var.m_impl.m_value10);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, distance_u&& var)
{
  switch (var.m_type)
  {
    case distance_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case distance_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case distance_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case distance_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case distance_u::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case distance_u::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case distance_u::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case distance_u::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case distance_u::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    case distance_u::Type::Type9:
      return functor(std::move(var.m_impl.m_value9));
    case distance_u::Type::Type10:
      return functor(std::move(var.m_impl.m_value10));
    default:
      return functor();
  }
}
inline bool operator==(const distance_u& lhs, const distance_u& rhs)
{
  return (lhs.m_type == rhs.m_type);
}
inline bool operator!=(const distance_u& lhs, const distance_u& rhs)
{
  return (lhs.m_type != rhs.m_type);
}
inline bool operator==(const distance_u& lhs, const ossia::meter_u& rhs)
{
  return (lhs.m_type == distance_u::Type::Type0);
}
inline bool operator==(const ossia::meter_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type == distance_u::Type::Type0);
}
inline bool operator!=(const distance_u& lhs, const ossia::meter_u& rhs)
{
  return (lhs.m_type != distance_u::Type::Type0);
}
inline bool operator!=(const ossia::meter_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type != distance_u::Type::Type0);
}
inline bool operator==(const distance_u& lhs, const ossia::kilometer_u& rhs)
{
  return (lhs.m_type == distance_u::Type::Type1);
}
inline bool operator==(const ossia::kilometer_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type == distance_u::Type::Type1);
}
inline bool operator!=(const distance_u& lhs, const ossia::kilometer_u& rhs)
{
  return (lhs.m_type != distance_u::Type::Type1);
}
inline bool operator!=(const ossia::kilometer_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type != distance_u::Type::Type1);
}
inline bool operator==(const distance_u& lhs, const ossia::decimeter_u& rhs)
{
  return (lhs.m_type == distance_u::Type::Type2);
}
inline bool operator==(const ossia::decimeter_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type == distance_u::Type::Type2);
}
inline bool operator!=(const distance_u& lhs, const ossia::decimeter_u& rhs)
{
  return (lhs.m_type != distance_u::Type::Type2);
}
inline bool operator!=(const ossia::decimeter_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type != distance_u::Type::Type2);
}
inline bool operator==(const distance_u& lhs, const ossia::centimeter_u& rhs)
{
  return (lhs.m_type == distance_u::Type::Type3);
}
inline bool operator==(const ossia::centimeter_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type == distance_u::Type::Type3);
}
inline bool operator!=(const distance_u& lhs, const ossia::centimeter_u& rhs)
{
  return (lhs.m_type != distance_u::Type::Type3);
}
inline bool operator!=(const ossia::centimeter_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type != distance_u::Type::Type3);
}
inline bool operator==(const distance_u& lhs, const ossia::millimeter_u& rhs)
{
  return (lhs.m_type == distance_u::Type::Type4);
}
inline bool operator==(const ossia::millimeter_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type == distance_u::Type::Type4);
}
inline bool operator!=(const distance_u& lhs, const ossia::millimeter_u& rhs)
{
  return (lhs.m_type != distance_u::Type::Type4);
}
inline bool operator!=(const ossia::millimeter_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type != distance_u::Type::Type4);
}
inline bool operator==(const distance_u& lhs, const ossia::micrometer_u& rhs)
{
  return (lhs.m_type == distance_u::Type::Type5);
}
inline bool operator==(const ossia::micrometer_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type == distance_u::Type::Type5);
}
inline bool operator!=(const distance_u& lhs, const ossia::micrometer_u& rhs)
{
  return (lhs.m_type != distance_u::Type::Type5);
}
inline bool operator!=(const ossia::micrometer_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type != distance_u::Type::Type5);
}
inline bool operator==(const distance_u& lhs, const ossia::nanometer_u& rhs)
{
  return (lhs.m_type == distance_u::Type::Type6);
}
inline bool operator==(const ossia::nanometer_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type == distance_u::Type::Type6);
}
inline bool operator!=(const distance_u& lhs, const ossia::nanometer_u& rhs)
{
  return (lhs.m_type != distance_u::Type::Type6);
}
inline bool operator!=(const ossia::nanometer_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type != distance_u::Type::Type6);
}
inline bool operator==(const distance_u& lhs, const ossia::picometer_u& rhs)
{
  return (lhs.m_type == distance_u::Type::Type7);
}
inline bool operator==(const ossia::picometer_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type == distance_u::Type::Type7);
}
inline bool operator!=(const distance_u& lhs, const ossia::picometer_u& rhs)
{
  return (lhs.m_type != distance_u::Type::Type7);
}
inline bool operator!=(const ossia::picometer_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type != distance_u::Type::Type7);
}
inline bool operator==(const distance_u& lhs, const ossia::inch_u& rhs)
{
  return (lhs.m_type == distance_u::Type::Type8);
}
inline bool operator==(const ossia::inch_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type == distance_u::Type::Type8);
}
inline bool operator!=(const distance_u& lhs, const ossia::inch_u& rhs)
{
  return (lhs.m_type != distance_u::Type::Type8);
}
inline bool operator!=(const ossia::inch_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type != distance_u::Type::Type8);
}
inline bool operator==(const distance_u& lhs, const ossia::foot_u& rhs)
{
  return (lhs.m_type == distance_u::Type::Type9);
}
inline bool operator==(const ossia::foot_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type == distance_u::Type::Type9);
}
inline bool operator!=(const distance_u& lhs, const ossia::foot_u& rhs)
{
  return (lhs.m_type != distance_u::Type::Type9);
}
inline bool operator!=(const ossia::foot_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type != distance_u::Type::Type9);
}
inline bool operator==(const distance_u& lhs, const ossia::mile_u& rhs)
{
  return (lhs.m_type == distance_u::Type::Type10);
}
inline bool operator==(const ossia::mile_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type == distance_u::Type::Type10);
}
inline bool operator!=(const distance_u& lhs, const ossia::mile_u& rhs)
{
  return (lhs.m_type != distance_u::Type::Type10);
}
inline bool operator!=(const ossia::mile_u& lhs, const distance_u& rhs)
{
  return (rhs.m_type != distance_u::Type::Type10);
}
struct gain_u
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::linear_u m_value0;

    ossia::midigain_u m_value1;

    ossia::decibel_u m_value2;

    ossia::decibel_raw_u m_value3;

    dummy_t m_dummy;
    Impl() : m_dummy{}
    {
    }
    ~Impl()
    {
    }
  };

  enum Type : int8_t
  {
    Type0,
    Type1,
    Type2,
    Type3,
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
      default:
        break;
    }
  }
  Impl m_impl;
  Type m_type;

public:
  static const constexpr auto npos = Npos;
  int which() const
  {
    return m_type;
  }

  operator bool() const
  {
    return m_type != npos;
  }
  template <typename T>
  const T* target() const;
  template <typename T>
  T* target();
  template <typename T>
  const T& get() const;
  template <typename T>
  T& get();

  template <typename T>
  static Type matching_type();
  gain_u() : m_type{Npos}
  {
  }
  ~gain_u()
  {
    destruct_impl();
  }
  gain_u(ossia::linear_u v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::linear_u{v};
  }
  gain_u(ossia::midigain_u v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::midigain_u{v};
  }
  gain_u(ossia::decibel_u v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::decibel_u{v};
  }
  gain_u(ossia::decibel_raw_u v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::decibel_raw_u{v};
  }
  gain_u(const gain_u& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::linear_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::midigain_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::decibel_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::decibel_raw_u{other.m_impl.m_value3};
        break;
      default:
        break;
    }
  }
  gain_u(gain_u&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::linear_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::midigain_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::decibel_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::decibel_raw_u{std::move(other.m_impl.m_value3)};
        break;
      default:
        break;
    }
  }
  gain_u& operator=(const gain_u& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::linear_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::midigain_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::decibel_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::decibel_raw_u{other.m_impl.m_value3};
        break;
      default:
        break;
    }
    return *this;
  }
  gain_u& operator=(gain_u&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::linear_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::midigain_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::decibel_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::decibel_raw_u{std::move(other.m_impl.m_value3)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::linear_u* gain_u::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::midigain_u* gain_u::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::decibel_u* gain_u::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::decibel_raw_u* gain_u::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::linear_u* gain_u::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::midigain_u* gain_u::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::decibel_u* gain_u::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::decibel_raw_u* gain_u::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::linear_u& gain_u::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::midigain_u& gain_u::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::decibel_u& gain_u::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::decibel_raw_u& gain_u::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::linear_u& gain_u::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::midigain_u& gain_u::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::decibel_u& gain_u::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::decibel_raw_u& gain_u::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const gain_u& var)
{
  switch (var.m_type)
  {
    case gain_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case gain_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case gain_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case gain_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, gain_u& var)
{
  switch (var.m_type)
  {
    case gain_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case gain_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case gain_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case gain_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, gain_u&& var)
{
  switch (var.m_type)
  {
    case gain_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case gain_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case gain_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case gain_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const gain_u& var)
{
  switch (var.m_type)
  {
    case gain_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case gain_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case gain_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case gain_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, gain_u& var)
{
  switch (var.m_type)
  {
    case gain_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case gain_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case gain_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case gain_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, gain_u&& var)
{
  switch (var.m_type)
  {
    case gain_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case gain_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case gain_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case gain_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    default:
      return functor();
  }
}
inline bool operator==(const gain_u& lhs, const gain_u& rhs)
{
  return (lhs.m_type == rhs.m_type);
}
inline bool operator!=(const gain_u& lhs, const gain_u& rhs)
{
  return (lhs.m_type != rhs.m_type);
}
inline bool operator==(const gain_u& lhs, const ossia::linear_u& rhs)
{
  return (lhs.m_type == gain_u::Type::Type0);
}
inline bool operator==(const ossia::linear_u& lhs, const gain_u& rhs)
{
  return (rhs.m_type == gain_u::Type::Type0);
}
inline bool operator!=(const gain_u& lhs, const ossia::linear_u& rhs)
{
  return (lhs.m_type != gain_u::Type::Type0);
}
inline bool operator!=(const ossia::linear_u& lhs, const gain_u& rhs)
{
  return (rhs.m_type != gain_u::Type::Type0);
}
inline bool operator==(const gain_u& lhs, const ossia::midigain_u& rhs)
{
  return (lhs.m_type == gain_u::Type::Type1);
}
inline bool operator==(const ossia::midigain_u& lhs, const gain_u& rhs)
{
  return (rhs.m_type == gain_u::Type::Type1);
}
inline bool operator!=(const gain_u& lhs, const ossia::midigain_u& rhs)
{
  return (lhs.m_type != gain_u::Type::Type1);
}
inline bool operator!=(const ossia::midigain_u& lhs, const gain_u& rhs)
{
  return (rhs.m_type != gain_u::Type::Type1);
}
inline bool operator==(const gain_u& lhs, const ossia::decibel_u& rhs)
{
  return (lhs.m_type == gain_u::Type::Type2);
}
inline bool operator==(const ossia::decibel_u& lhs, const gain_u& rhs)
{
  return (rhs.m_type == gain_u::Type::Type2);
}
inline bool operator!=(const gain_u& lhs, const ossia::decibel_u& rhs)
{
  return (lhs.m_type != gain_u::Type::Type2);
}
inline bool operator!=(const ossia::decibel_u& lhs, const gain_u& rhs)
{
  return (rhs.m_type != gain_u::Type::Type2);
}
inline bool operator==(const gain_u& lhs, const ossia::decibel_raw_u& rhs)
{
  return (lhs.m_type == gain_u::Type::Type3);
}
inline bool operator==(const ossia::decibel_raw_u& lhs, const gain_u& rhs)
{
  return (rhs.m_type == gain_u::Type::Type3);
}
inline bool operator!=(const gain_u& lhs, const ossia::decibel_raw_u& rhs)
{
  return (lhs.m_type != gain_u::Type::Type3);
}
inline bool operator!=(const ossia::decibel_raw_u& lhs, const gain_u& rhs)
{
  return (rhs.m_type != gain_u::Type::Type3);
}
struct orientation_u
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::quaternion_u m_value0;

    ossia::euler_u m_value1;

    ossia::axis_u m_value2;

    dummy_t m_dummy;
    Impl() : m_dummy{}
    {
    }
    ~Impl()
    {
    }
  };

  enum Type : int8_t
  {
    Type0,
    Type1,
    Type2,
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
      default:
        break;
    }
  }
  Impl m_impl;
  Type m_type;

public:
  static const constexpr auto npos = Npos;
  int which() const
  {
    return m_type;
  }

  operator bool() const
  {
    return m_type != npos;
  }
  template <typename T>
  const T* target() const;
  template <typename T>
  T* target();
  template <typename T>
  const T& get() const;
  template <typename T>
  T& get();

  template <typename T>
  static Type matching_type();
  orientation_u() : m_type{Npos}
  {
  }
  ~orientation_u()
  {
    destruct_impl();
  }
  orientation_u(ossia::quaternion_u v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::quaternion_u{v};
  }
  orientation_u(ossia::euler_u v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::euler_u{v};
  }
  orientation_u(ossia::axis_u v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::axis_u{v};
  }
  orientation_u(const orientation_u& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::quaternion_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::euler_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::axis_u{other.m_impl.m_value2};
        break;
      default:
        break;
    }
  }
  orientation_u(orientation_u&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::quaternion_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::euler_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::axis_u{std::move(other.m_impl.m_value2)};
        break;
      default:
        break;
    }
  }
  orientation_u& operator=(const orientation_u& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::quaternion_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::euler_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::axis_u{other.m_impl.m_value2};
        break;
      default:
        break;
    }
    return *this;
  }
  orientation_u& operator=(orientation_u&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::quaternion_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::euler_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::axis_u{std::move(other.m_impl.m_value2)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::quaternion_u* orientation_u::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::euler_u* orientation_u::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::axis_u* orientation_u::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::quaternion_u* orientation_u::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::euler_u* orientation_u::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::axis_u* orientation_u::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::quaternion_u& orientation_u::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::euler_u& orientation_u::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::axis_u& orientation_u::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::quaternion_u& orientation_u::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::euler_u& orientation_u::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::axis_u& orientation_u::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const orientation_u& var)
{
  switch (var.m_type)
  {
    case orientation_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case orientation_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case orientation_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, orientation_u& var)
{
  switch (var.m_type)
  {
    case orientation_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case orientation_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case orientation_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, orientation_u&& var)
{
  switch (var.m_type)
  {
    case orientation_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case orientation_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case orientation_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const orientation_u& var)
{
  switch (var.m_type)
  {
    case orientation_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case orientation_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case orientation_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, orientation_u& var)
{
  switch (var.m_type)
  {
    case orientation_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case orientation_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case orientation_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, orientation_u&& var)
{
  switch (var.m_type)
  {
    case orientation_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case orientation_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case orientation_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    default:
      return functor();
  }
}
inline bool operator==(const orientation_u& lhs, const orientation_u& rhs)
{
  return (lhs.m_type == rhs.m_type);
}
inline bool operator!=(const orientation_u& lhs, const orientation_u& rhs)
{
  return (lhs.m_type != rhs.m_type);
}
inline bool
operator==(const orientation_u& lhs, const ossia::quaternion_u& rhs)
{
  return (lhs.m_type == orientation_u::Type::Type0);
}
inline bool
operator==(const ossia::quaternion_u& lhs, const orientation_u& rhs)
{
  return (rhs.m_type == orientation_u::Type::Type0);
}
inline bool
operator!=(const orientation_u& lhs, const ossia::quaternion_u& rhs)
{
  return (lhs.m_type != orientation_u::Type::Type0);
}
inline bool
operator!=(const ossia::quaternion_u& lhs, const orientation_u& rhs)
{
  return (rhs.m_type != orientation_u::Type::Type0);
}
inline bool operator==(const orientation_u& lhs, const ossia::euler_u& rhs)
{
  return (lhs.m_type == orientation_u::Type::Type1);
}
inline bool operator==(const ossia::euler_u& lhs, const orientation_u& rhs)
{
  return (rhs.m_type == orientation_u::Type::Type1);
}
inline bool operator!=(const orientation_u& lhs, const ossia::euler_u& rhs)
{
  return (lhs.m_type != orientation_u::Type::Type1);
}
inline bool operator!=(const ossia::euler_u& lhs, const orientation_u& rhs)
{
  return (rhs.m_type != orientation_u::Type::Type1);
}
inline bool operator==(const orientation_u& lhs, const ossia::axis_u& rhs)
{
  return (lhs.m_type == orientation_u::Type::Type2);
}
inline bool operator==(const ossia::axis_u& lhs, const orientation_u& rhs)
{
  return (rhs.m_type == orientation_u::Type::Type2);
}
inline bool operator!=(const orientation_u& lhs, const ossia::axis_u& rhs)
{
  return (lhs.m_type != orientation_u::Type::Type2);
}
inline bool operator!=(const ossia::axis_u& lhs, const orientation_u& rhs)
{
  return (rhs.m_type != orientation_u::Type::Type2);
}
struct position_u
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::cartesian_3d_u m_value0;

    ossia::cartesian_2d_u m_value1;

    ossia::spherical_u m_value2;

    ossia::polar_u m_value3;

    ossia::opengl_u m_value4;

    ossia::cylindrical_u m_value5;

    dummy_t m_dummy;
    Impl() : m_dummy{}
    {
    }
    ~Impl()
    {
    }
  };

  enum Type : int8_t
  {
    Type0,
    Type1,
    Type2,
    Type3,
    Type4,
    Type5,
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
      default:
        break;
    }
  }
  Impl m_impl;
  Type m_type;

public:
  static const constexpr auto npos = Npos;
  int which() const
  {
    return m_type;
  }

  operator bool() const
  {
    return m_type != npos;
  }
  template <typename T>
  const T* target() const;
  template <typename T>
  T* target();
  template <typename T>
  const T& get() const;
  template <typename T>
  T& get();

  template <typename T>
  static Type matching_type();
  position_u() : m_type{Npos}
  {
  }
  ~position_u()
  {
    destruct_impl();
  }
  position_u(ossia::cartesian_3d_u v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::cartesian_3d_u{v};
  }
  position_u(ossia::cartesian_2d_u v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::cartesian_2d_u{v};
  }
  position_u(ossia::spherical_u v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::spherical_u{v};
  }
  position_u(ossia::polar_u v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::polar_u{v};
  }
  position_u(ossia::opengl_u v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::opengl_u{v};
  }
  position_u(ossia::cylindrical_u v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::cylindrical_u{v};
  }
  position_u(const position_u& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::cartesian_3d_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::cartesian_2d_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::spherical_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::polar_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::opengl_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::cylindrical_u{other.m_impl.m_value5};
        break;
      default:
        break;
    }
  }
  position_u(position_u&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::cartesian_3d_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::cartesian_2d_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::spherical_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::polar_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::opengl_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::cylindrical_u{std::move(other.m_impl.m_value5)};
        break;
      default:
        break;
    }
  }
  position_u& operator=(const position_u& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::cartesian_3d_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::cartesian_2d_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::spherical_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::polar_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::opengl_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::cylindrical_u{other.m_impl.m_value5};
        break;
      default:
        break;
    }
    return *this;
  }
  position_u& operator=(position_u&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::cartesian_3d_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::cartesian_2d_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::spherical_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::polar_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::opengl_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::cylindrical_u{std::move(other.m_impl.m_value5)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::cartesian_3d_u* position_u::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::cartesian_2d_u* position_u::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::spherical_u* position_u::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::polar_u* position_u::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::opengl_u* position_u::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::cylindrical_u* position_u::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::cartesian_3d_u* position_u::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::cartesian_2d_u* position_u::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::spherical_u* position_u::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::polar_u* position_u::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::opengl_u* position_u::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::cylindrical_u* position_u::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::cartesian_3d_u& position_u::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::cartesian_2d_u& position_u::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::spherical_u& position_u::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::polar_u& position_u::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::opengl_u& position_u::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::cylindrical_u& position_u::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::cartesian_3d_u& position_u::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::cartesian_2d_u& position_u::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::spherical_u& position_u::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::polar_u& position_u::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::opengl_u& position_u::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::cylindrical_u& position_u::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const position_u& var)
{
  switch (var.m_type)
  {
    case position_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case position_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case position_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case position_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case position_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case position_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, position_u& var)
{
  switch (var.m_type)
  {
    case position_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case position_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case position_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case position_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case position_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case position_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, position_u&& var)
{
  switch (var.m_type)
  {
    case position_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case position_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case position_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case position_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case position_u::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case position_u::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const position_u& var)
{
  switch (var.m_type)
  {
    case position_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case position_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case position_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case position_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case position_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case position_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, position_u& var)
{
  switch (var.m_type)
  {
    case position_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case position_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case position_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case position_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case position_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case position_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, position_u&& var)
{
  switch (var.m_type)
  {
    case position_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case position_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case position_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case position_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case position_u::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case position_u::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    default:
      return functor();
  }
}
inline bool operator==(const position_u& lhs, const position_u& rhs)
{
  return (lhs.m_type == rhs.m_type);
}
inline bool operator!=(const position_u& lhs, const position_u& rhs)
{
  return (lhs.m_type != rhs.m_type);
}
inline bool operator==(const position_u& lhs, const ossia::cartesian_3d_u& rhs)
{
  return (lhs.m_type == position_u::Type::Type0);
}
inline bool operator==(const ossia::cartesian_3d_u& lhs, const position_u& rhs)
{
  return (rhs.m_type == position_u::Type::Type0);
}
inline bool operator!=(const position_u& lhs, const ossia::cartesian_3d_u& rhs)
{
  return (lhs.m_type != position_u::Type::Type0);
}
inline bool operator!=(const ossia::cartesian_3d_u& lhs, const position_u& rhs)
{
  return (rhs.m_type != position_u::Type::Type0);
}
inline bool operator==(const position_u& lhs, const ossia::cartesian_2d_u& rhs)
{
  return (lhs.m_type == position_u::Type::Type1);
}
inline bool operator==(const ossia::cartesian_2d_u& lhs, const position_u& rhs)
{
  return (rhs.m_type == position_u::Type::Type1);
}
inline bool operator!=(const position_u& lhs, const ossia::cartesian_2d_u& rhs)
{
  return (lhs.m_type != position_u::Type::Type1);
}
inline bool operator!=(const ossia::cartesian_2d_u& lhs, const position_u& rhs)
{
  return (rhs.m_type != position_u::Type::Type1);
}
inline bool operator==(const position_u& lhs, const ossia::spherical_u& rhs)
{
  return (lhs.m_type == position_u::Type::Type2);
}
inline bool operator==(const ossia::spherical_u& lhs, const position_u& rhs)
{
  return (rhs.m_type == position_u::Type::Type2);
}
inline bool operator!=(const position_u& lhs, const ossia::spherical_u& rhs)
{
  return (lhs.m_type != position_u::Type::Type2);
}
inline bool operator!=(const ossia::spherical_u& lhs, const position_u& rhs)
{
  return (rhs.m_type != position_u::Type::Type2);
}
inline bool operator==(const position_u& lhs, const ossia::polar_u& rhs)
{
  return (lhs.m_type == position_u::Type::Type3);
}
inline bool operator==(const ossia::polar_u& lhs, const position_u& rhs)
{
  return (rhs.m_type == position_u::Type::Type3);
}
inline bool operator!=(const position_u& lhs, const ossia::polar_u& rhs)
{
  return (lhs.m_type != position_u::Type::Type3);
}
inline bool operator!=(const ossia::polar_u& lhs, const position_u& rhs)
{
  return (rhs.m_type != position_u::Type::Type3);
}
inline bool operator==(const position_u& lhs, const ossia::opengl_u& rhs)
{
  return (lhs.m_type == position_u::Type::Type4);
}
inline bool operator==(const ossia::opengl_u& lhs, const position_u& rhs)
{
  return (rhs.m_type == position_u::Type::Type4);
}
inline bool operator!=(const position_u& lhs, const ossia::opengl_u& rhs)
{
  return (lhs.m_type != position_u::Type::Type4);
}
inline bool operator!=(const ossia::opengl_u& lhs, const position_u& rhs)
{
  return (rhs.m_type != position_u::Type::Type4);
}
inline bool operator==(const position_u& lhs, const ossia::cylindrical_u& rhs)
{
  return (lhs.m_type == position_u::Type::Type5);
}
inline bool operator==(const ossia::cylindrical_u& lhs, const position_u& rhs)
{
  return (rhs.m_type == position_u::Type::Type5);
}
inline bool operator!=(const position_u& lhs, const ossia::cylindrical_u& rhs)
{
  return (lhs.m_type != position_u::Type::Type5);
}
inline bool operator!=(const ossia::cylindrical_u& lhs, const position_u& rhs)
{
  return (rhs.m_type != position_u::Type::Type5);
}
struct speed_u
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::meter_per_second_u m_value0;

    ossia::miles_per_hour_u m_value1;

    ossia::kilometer_per_hour_u m_value2;

    ossia::knot_u m_value3;

    ossia::foot_per_second_u m_value4;

    ossia::foot_per_hour_u m_value5;

    dummy_t m_dummy;
    Impl() : m_dummy{}
    {
    }
    ~Impl()
    {
    }
  };

  enum Type : int8_t
  {
    Type0,
    Type1,
    Type2,
    Type3,
    Type4,
    Type5,
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
      default:
        break;
    }
  }
  Impl m_impl;
  Type m_type;

public:
  static const constexpr auto npos = Npos;
  int which() const
  {
    return m_type;
  }

  operator bool() const
  {
    return m_type != npos;
  }
  template <typename T>
  const T* target() const;
  template <typename T>
  T* target();
  template <typename T>
  const T& get() const;
  template <typename T>
  T& get();

  template <typename T>
  static Type matching_type();
  speed_u() : m_type{Npos}
  {
  }
  ~speed_u()
  {
    destruct_impl();
  }
  speed_u(ossia::meter_per_second_u v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::meter_per_second_u{v};
  }
  speed_u(ossia::miles_per_hour_u v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::miles_per_hour_u{v};
  }
  speed_u(ossia::kilometer_per_hour_u v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::kilometer_per_hour_u{v};
  }
  speed_u(ossia::knot_u v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::knot_u{v};
  }
  speed_u(ossia::foot_per_second_u v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::foot_per_second_u{v};
  }
  speed_u(ossia::foot_per_hour_u v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::foot_per_hour_u{v};
  }
  speed_u(const speed_u& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::meter_per_second_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::miles_per_hour_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::kilometer_per_hour_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::knot_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::foot_per_second_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::foot_per_hour_u{other.m_impl.m_value5};
        break;
      default:
        break;
    }
  }
  speed_u(speed_u&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::meter_per_second_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::miles_per_hour_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::kilometer_per_hour_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::knot_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::foot_per_second_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::foot_per_hour_u{std::move(other.m_impl.m_value5)};
        break;
      default:
        break;
    }
  }
  speed_u& operator=(const speed_u& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::meter_per_second_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::miles_per_hour_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::kilometer_per_hour_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::knot_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::foot_per_second_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::foot_per_hour_u{other.m_impl.m_value5};
        break;
      default:
        break;
    }
    return *this;
  }
  speed_u& operator=(speed_u&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::meter_per_second_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::miles_per_hour_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::kilometer_per_hour_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::knot_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::foot_per_second_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::foot_per_hour_u{std::move(other.m_impl.m_value5)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::meter_per_second_u* speed_u::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::miles_per_hour_u* speed_u::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::kilometer_per_hour_u* speed_u::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::knot_u* speed_u::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::foot_per_second_u* speed_u::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::foot_per_hour_u* speed_u::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::meter_per_second_u* speed_u::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::miles_per_hour_u* speed_u::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::kilometer_per_hour_u* speed_u::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::knot_u* speed_u::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::foot_per_second_u* speed_u::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::foot_per_hour_u* speed_u::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::meter_per_second_u& speed_u::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::miles_per_hour_u& speed_u::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::kilometer_per_hour_u& speed_u::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::knot_u& speed_u::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::foot_per_second_u& speed_u::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::foot_per_hour_u& speed_u::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::meter_per_second_u& speed_u::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::miles_per_hour_u& speed_u::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::kilometer_per_hour_u& speed_u::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::knot_u& speed_u::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::foot_per_second_u& speed_u::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::foot_per_hour_u& speed_u::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const speed_u& var)
{
  switch (var.m_type)
  {
    case speed_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case speed_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case speed_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case speed_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case speed_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case speed_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, speed_u& var)
{
  switch (var.m_type)
  {
    case speed_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case speed_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case speed_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case speed_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case speed_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case speed_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, speed_u&& var)
{
  switch (var.m_type)
  {
    case speed_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case speed_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case speed_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case speed_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case speed_u::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case speed_u::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const speed_u& var)
{
  switch (var.m_type)
  {
    case speed_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case speed_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case speed_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case speed_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case speed_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case speed_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, speed_u& var)
{
  switch (var.m_type)
  {
    case speed_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case speed_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case speed_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case speed_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case speed_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case speed_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, speed_u&& var)
{
  switch (var.m_type)
  {
    case speed_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case speed_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case speed_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case speed_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case speed_u::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case speed_u::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    default:
      return functor();
  }
}
inline bool operator==(const speed_u& lhs, const speed_u& rhs)
{
  return (lhs.m_type == rhs.m_type);
}
inline bool operator!=(const speed_u& lhs, const speed_u& rhs)
{
  return (lhs.m_type != rhs.m_type);
}
inline bool
operator==(const speed_u& lhs, const ossia::meter_per_second_u& rhs)
{
  return (lhs.m_type == speed_u::Type::Type0);
}
inline bool
operator==(const ossia::meter_per_second_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type == speed_u::Type::Type0);
}
inline bool
operator!=(const speed_u& lhs, const ossia::meter_per_second_u& rhs)
{
  return (lhs.m_type != speed_u::Type::Type0);
}
inline bool
operator!=(const ossia::meter_per_second_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type != speed_u::Type::Type0);
}
inline bool operator==(const speed_u& lhs, const ossia::miles_per_hour_u& rhs)
{
  return (lhs.m_type == speed_u::Type::Type1);
}
inline bool operator==(const ossia::miles_per_hour_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type == speed_u::Type::Type1);
}
inline bool operator!=(const speed_u& lhs, const ossia::miles_per_hour_u& rhs)
{
  return (lhs.m_type != speed_u::Type::Type1);
}
inline bool operator!=(const ossia::miles_per_hour_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type != speed_u::Type::Type1);
}
inline bool
operator==(const speed_u& lhs, const ossia::kilometer_per_hour_u& rhs)
{
  return (lhs.m_type == speed_u::Type::Type2);
}
inline bool
operator==(const ossia::kilometer_per_hour_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type == speed_u::Type::Type2);
}
inline bool
operator!=(const speed_u& lhs, const ossia::kilometer_per_hour_u& rhs)
{
  return (lhs.m_type != speed_u::Type::Type2);
}
inline bool
operator!=(const ossia::kilometer_per_hour_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type != speed_u::Type::Type2);
}
inline bool operator==(const speed_u& lhs, const ossia::knot_u& rhs)
{
  return (lhs.m_type == speed_u::Type::Type3);
}
inline bool operator==(const ossia::knot_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type == speed_u::Type::Type3);
}
inline bool operator!=(const speed_u& lhs, const ossia::knot_u& rhs)
{
  return (lhs.m_type != speed_u::Type::Type3);
}
inline bool operator!=(const ossia::knot_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type != speed_u::Type::Type3);
}
inline bool operator==(const speed_u& lhs, const ossia::foot_per_second_u& rhs)
{
  return (lhs.m_type == speed_u::Type::Type4);
}
inline bool operator==(const ossia::foot_per_second_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type == speed_u::Type::Type4);
}
inline bool operator!=(const speed_u& lhs, const ossia::foot_per_second_u& rhs)
{
  return (lhs.m_type != speed_u::Type::Type4);
}
inline bool operator!=(const ossia::foot_per_second_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type != speed_u::Type::Type4);
}
inline bool operator==(const speed_u& lhs, const ossia::foot_per_hour_u& rhs)
{
  return (lhs.m_type == speed_u::Type::Type5);
}
inline bool operator==(const ossia::foot_per_hour_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type == speed_u::Type::Type5);
}
inline bool operator!=(const speed_u& lhs, const ossia::foot_per_hour_u& rhs)
{
  return (lhs.m_type != speed_u::Type::Type5);
}
inline bool operator!=(const ossia::foot_per_hour_u& lhs, const speed_u& rhs)
{
  return (rhs.m_type != speed_u::Type::Type5);
}
struct timing_u
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::second_u m_value0;

    ossia::bark_u m_value1;

    ossia::bpm_u m_value2;

    ossia::cent_u m_value3;

    ossia::frequency_u m_value4;

    ossia::mel_u m_value5;

    ossia::midi_pitch_u m_value6;

    ossia::millisecond_u m_value7;

    ossia::playback_speed_u m_value8;

    dummy_t m_dummy;
    Impl() : m_dummy{}
    {
    }
    ~Impl()
    {
    }
  };

  enum Type : int8_t
  {
    Type0,
    Type1,
    Type2,
    Type3,
    Type4,
    Type5,
    Type6,
    Type7,
    Type8,
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
      default:
        break;
    }
  }
  Impl m_impl;
  Type m_type;

public:
  static const constexpr auto npos = Npos;
  int which() const
  {
    return m_type;
  }

  operator bool() const
  {
    return m_type != npos;
  }
  template <typename T>
  const T* target() const;
  template <typename T>
  T* target();
  template <typename T>
  const T& get() const;
  template <typename T>
  T& get();

  template <typename T>
  static Type matching_type();
  timing_u() : m_type{Npos}
  {
  }
  ~timing_u()
  {
    destruct_impl();
  }
  timing_u(ossia::second_u v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::second_u{v};
  }
  timing_u(ossia::bark_u v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::bark_u{v};
  }
  timing_u(ossia::bpm_u v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::bpm_u{v};
  }
  timing_u(ossia::cent_u v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::cent_u{v};
  }
  timing_u(ossia::frequency_u v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::frequency_u{v};
  }
  timing_u(ossia::mel_u v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::mel_u{v};
  }
  timing_u(ossia::midi_pitch_u v) : m_type{Type6}
  {
    new (&m_impl.m_value6) ossia::midi_pitch_u{v};
  }
  timing_u(ossia::millisecond_u v) : m_type{Type7}
  {
    new (&m_impl.m_value7) ossia::millisecond_u{v};
  }
  timing_u(ossia::playback_speed_u v) : m_type{Type8}
  {
    new (&m_impl.m_value8) ossia::playback_speed_u{v};
  }
  timing_u(const timing_u& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::second_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::bark_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::bpm_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::cent_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::frequency_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::mel_u{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::midi_pitch_u{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::millisecond_u{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::playback_speed_u{other.m_impl.m_value8};
        break;
      default:
        break;
    }
  }
  timing_u(timing_u&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::second_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::bark_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::bpm_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::cent_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::frequency_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::mel_u{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6)
            ossia::midi_pitch_u{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::millisecond_u{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8)
            ossia::playback_speed_u{std::move(other.m_impl.m_value8)};
        break;
      default:
        break;
    }
  }
  timing_u& operator=(const timing_u& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::second_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::bark_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::bpm_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::cent_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::frequency_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::mel_u{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::midi_pitch_u{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::millisecond_u{other.m_impl.m_value7};
        break;
      case Type::Type8:
        new (&m_impl.m_value8) ossia::playback_speed_u{other.m_impl.m_value8};
        break;
      default:
        break;
    }
    return *this;
  }
  timing_u& operator=(timing_u&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::second_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::bark_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::bpm_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::cent_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::frequency_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::mel_u{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6)
            ossia::midi_pitch_u{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::millisecond_u{std::move(other.m_impl.m_value7)};
        break;
      case Type::Type8:
        new (&m_impl.m_value8)
            ossia::playback_speed_u{std::move(other.m_impl.m_value8)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::second_u* timing_u::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::bark_u* timing_u::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::bpm_u* timing_u::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::cent_u* timing_u::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::frequency_u* timing_u::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::mel_u* timing_u::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::midi_pitch_u* timing_u::target() const
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline const ossia::millisecond_u* timing_u::target() const
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline const ossia::playback_speed_u* timing_u::target() const
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline ossia::second_u* timing_u::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::bark_u* timing_u::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::bpm_u* timing_u::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::cent_u* timing_u::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::frequency_u* timing_u::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::mel_u* timing_u::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::midi_pitch_u* timing_u::target()
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline ossia::millisecond_u* timing_u::target()
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline ossia::playback_speed_u* timing_u::target()
{
  if (m_type == Type8)
    return &m_impl.m_value8;
  return nullptr;
}
template <>
inline const ossia::second_u& timing_u::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::bark_u& timing_u::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::bpm_u& timing_u::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::cent_u& timing_u::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::frequency_u& timing_u::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::mel_u& timing_u::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::midi_pitch_u& timing_u::get() const
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::millisecond_u& timing_u::get() const
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::playback_speed_u& timing_u::get() const
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::second_u& timing_u::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::bark_u& timing_u::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::bpm_u& timing_u::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::cent_u& timing_u::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::frequency_u& timing_u::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::mel_u& timing_u::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::midi_pitch_u& timing_u::get()
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::millisecond_u& timing_u::get()
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::playback_speed_u& timing_u::get()
{
  if (m_type == Type8)
    return m_impl.m_value8;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const timing_u& var)
{
  switch (var.m_type)
  {
    case timing_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case timing_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case timing_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case timing_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case timing_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case timing_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case timing_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case timing_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    case timing_u::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, timing_u& var)
{
  switch (var.m_type)
  {
    case timing_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case timing_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case timing_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case timing_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case timing_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case timing_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case timing_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case timing_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    case timing_u::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, timing_u&& var)
{
  switch (var.m_type)
  {
    case timing_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case timing_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case timing_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case timing_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case timing_u::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case timing_u::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case timing_u::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case timing_u::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case timing_u::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const timing_u& var)
{
  switch (var.m_type)
  {
    case timing_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case timing_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case timing_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case timing_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case timing_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case timing_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case timing_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case timing_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    case timing_u::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, timing_u& var)
{
  switch (var.m_type)
  {
    case timing_u::Type::Type0:
      return functor(var.m_impl.m_value0);
    case timing_u::Type::Type1:
      return functor(var.m_impl.m_value1);
    case timing_u::Type::Type2:
      return functor(var.m_impl.m_value2);
    case timing_u::Type::Type3:
      return functor(var.m_impl.m_value3);
    case timing_u::Type::Type4:
      return functor(var.m_impl.m_value4);
    case timing_u::Type::Type5:
      return functor(var.m_impl.m_value5);
    case timing_u::Type::Type6:
      return functor(var.m_impl.m_value6);
    case timing_u::Type::Type7:
      return functor(var.m_impl.m_value7);
    case timing_u::Type::Type8:
      return functor(var.m_impl.m_value8);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, timing_u&& var)
{
  switch (var.m_type)
  {
    case timing_u::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case timing_u::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case timing_u::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case timing_u::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case timing_u::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case timing_u::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case timing_u::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case timing_u::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    case timing_u::Type::Type8:
      return functor(std::move(var.m_impl.m_value8));
    default:
      return functor();
  }
}
inline bool operator==(const timing_u& lhs, const timing_u& rhs)
{
  return (lhs.m_type == rhs.m_type);
}
inline bool operator!=(const timing_u& lhs, const timing_u& rhs)
{
  return (lhs.m_type != rhs.m_type);
}
inline bool operator==(const timing_u& lhs, const ossia::second_u& rhs)
{
  return (lhs.m_type == timing_u::Type::Type0);
}
inline bool operator==(const ossia::second_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type == timing_u::Type::Type0);
}
inline bool operator!=(const timing_u& lhs, const ossia::second_u& rhs)
{
  return (lhs.m_type != timing_u::Type::Type0);
}
inline bool operator!=(const ossia::second_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type != timing_u::Type::Type0);
}
inline bool operator==(const timing_u& lhs, const ossia::bark_u& rhs)
{
  return (lhs.m_type == timing_u::Type::Type1);
}
inline bool operator==(const ossia::bark_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type == timing_u::Type::Type1);
}
inline bool operator!=(const timing_u& lhs, const ossia::bark_u& rhs)
{
  return (lhs.m_type != timing_u::Type::Type1);
}
inline bool operator!=(const ossia::bark_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type != timing_u::Type::Type1);
}
inline bool operator==(const timing_u& lhs, const ossia::bpm_u& rhs)
{
  return (lhs.m_type == timing_u::Type::Type2);
}
inline bool operator==(const ossia::bpm_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type == timing_u::Type::Type2);
}
inline bool operator!=(const timing_u& lhs, const ossia::bpm_u& rhs)
{
  return (lhs.m_type != timing_u::Type::Type2);
}
inline bool operator!=(const ossia::bpm_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type != timing_u::Type::Type2);
}
inline bool operator==(const timing_u& lhs, const ossia::cent_u& rhs)
{
  return (lhs.m_type == timing_u::Type::Type3);
}
inline bool operator==(const ossia::cent_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type == timing_u::Type::Type3);
}
inline bool operator!=(const timing_u& lhs, const ossia::cent_u& rhs)
{
  return (lhs.m_type != timing_u::Type::Type3);
}
inline bool operator!=(const ossia::cent_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type != timing_u::Type::Type3);
}
inline bool operator==(const timing_u& lhs, const ossia::frequency_u& rhs)
{
  return (lhs.m_type == timing_u::Type::Type4);
}
inline bool operator==(const ossia::frequency_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type == timing_u::Type::Type4);
}
inline bool operator!=(const timing_u& lhs, const ossia::frequency_u& rhs)
{
  return (lhs.m_type != timing_u::Type::Type4);
}
inline bool operator!=(const ossia::frequency_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type != timing_u::Type::Type4);
}
inline bool operator==(const timing_u& lhs, const ossia::mel_u& rhs)
{
  return (lhs.m_type == timing_u::Type::Type5);
}
inline bool operator==(const ossia::mel_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type == timing_u::Type::Type5);
}
inline bool operator!=(const timing_u& lhs, const ossia::mel_u& rhs)
{
  return (lhs.m_type != timing_u::Type::Type5);
}
inline bool operator!=(const ossia::mel_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type != timing_u::Type::Type5);
}
inline bool operator==(const timing_u& lhs, const ossia::midi_pitch_u& rhs)
{
  return (lhs.m_type == timing_u::Type::Type6);
}
inline bool operator==(const ossia::midi_pitch_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type == timing_u::Type::Type6);
}
inline bool operator!=(const timing_u& lhs, const ossia::midi_pitch_u& rhs)
{
  return (lhs.m_type != timing_u::Type::Type6);
}
inline bool operator!=(const ossia::midi_pitch_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type != timing_u::Type::Type6);
}
inline bool operator==(const timing_u& lhs, const ossia::millisecond_u& rhs)
{
  return (lhs.m_type == timing_u::Type::Type7);
}
inline bool operator==(const ossia::millisecond_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type == timing_u::Type::Type7);
}
inline bool operator!=(const timing_u& lhs, const ossia::millisecond_u& rhs)
{
  return (lhs.m_type != timing_u::Type::Type7);
}
inline bool operator!=(const ossia::millisecond_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type != timing_u::Type::Type7);
}
inline bool operator==(const timing_u& lhs, const ossia::playback_speed_u& rhs)
{
  return (lhs.m_type == timing_u::Type::Type8);
}
inline bool operator==(const ossia::playback_speed_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type == timing_u::Type::Type8);
}
inline bool operator!=(const timing_u& lhs, const ossia::playback_speed_u& rhs)
{
  return (lhs.m_type != timing_u::Type::Type8);
}
inline bool operator!=(const ossia::playback_speed_u& lhs, const timing_u& rhs)
{
  return (rhs.m_type != timing_u::Type::Type8);
}
struct unit_variant
{
public:
  struct dummy_t
  {
  };
  union Impl {
    ossia::distance_u m_value0;

    ossia::position_u m_value1;

    ossia::speed_u m_value2;

    ossia::orientation_u m_value3;

    ossia::angle_u m_value4;

    ossia::color_u m_value5;

    ossia::gain_u m_value6;

    ossia::timing_u m_value7;

    dummy_t m_dummy;
    Impl() : m_dummy{}
    {
    }
    ~Impl()
    {
    }
  };

  enum Type : int8_t
  {
    Type0,
    Type1,
    Type2,
    Type3,
    Type4,
    Type5,
    Type6,
    Type7,
    Npos
  };

  void destruct_impl()
  {
    switch (m_type)
    {
      default:
        break;
    }
  }
  Impl m_impl;
  Type m_type;

public:
  static const constexpr auto npos = Npos;
  int which() const
  {
    return m_type;
  }

  operator bool() const
  {
    return m_type != npos;
  }
  template <typename T>
  const T* target() const;
  template <typename T>
  T* target();
  template <typename T>
  const T& get() const;
  template <typename T>
  T& get();

  template <typename T>
  static Type matching_type();
  unit_variant() : m_type{Npos}
  {
  }
  ~unit_variant()
  {
    destruct_impl();
  }
  unit_variant(ossia::distance_u v) : m_type{Type0}
  {
    new (&m_impl.m_value0) ossia::distance_u{v};
  }
  unit_variant(ossia::position_u v) : m_type{Type1}
  {
    new (&m_impl.m_value1) ossia::position_u{v};
  }
  unit_variant(ossia::speed_u v) : m_type{Type2}
  {
    new (&m_impl.m_value2) ossia::speed_u{v};
  }
  unit_variant(ossia::orientation_u v) : m_type{Type3}
  {
    new (&m_impl.m_value3) ossia::orientation_u{v};
  }
  unit_variant(ossia::angle_u v) : m_type{Type4}
  {
    new (&m_impl.m_value4) ossia::angle_u{v};
  }
  unit_variant(ossia::color_u v) : m_type{Type5}
  {
    new (&m_impl.m_value5) ossia::color_u{v};
  }
  unit_variant(ossia::gain_u v) : m_type{Type6}
  {
    new (&m_impl.m_value6) ossia::gain_u{v};
  }
  unit_variant(ossia::timing_u v) : m_type{Type7}
  {
    new (&m_impl.m_value7) ossia::timing_u{v};
  }
  unit_variant(const unit_variant& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::distance_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::position_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::speed_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::orientation_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::angle_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::color_u{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::gain_u{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::timing_u{other.m_impl.m_value7};
        break;
      default:
        break;
    }
  }
  unit_variant(unit_variant&& other) : m_type{other.m_type}
  {
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::distance_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::position_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::speed_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::orientation_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::angle_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::color_u{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::gain_u{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::timing_u{std::move(other.m_impl.m_value7)};
        break;
      default:
        break;
    }
  }
  unit_variant& operator=(const unit_variant& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0) ossia::distance_u{other.m_impl.m_value0};
        break;
      case Type::Type1:
        new (&m_impl.m_value1) ossia::position_u{other.m_impl.m_value1};
        break;
      case Type::Type2:
        new (&m_impl.m_value2) ossia::speed_u{other.m_impl.m_value2};
        break;
      case Type::Type3:
        new (&m_impl.m_value3) ossia::orientation_u{other.m_impl.m_value3};
        break;
      case Type::Type4:
        new (&m_impl.m_value4) ossia::angle_u{other.m_impl.m_value4};
        break;
      case Type::Type5:
        new (&m_impl.m_value5) ossia::color_u{other.m_impl.m_value5};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::gain_u{other.m_impl.m_value6};
        break;
      case Type::Type7:
        new (&m_impl.m_value7) ossia::timing_u{other.m_impl.m_value7};
        break;
      default:
        break;
    }
    return *this;
  }
  unit_variant& operator=(unit_variant&& other)
  {
    destruct_impl();
    m_type = other.m_type;
    switch (m_type)
    {
      case Type::Type0:
        new (&m_impl.m_value0)
            ossia::distance_u{std::move(other.m_impl.m_value0)};
        break;
      case Type::Type1:
        new (&m_impl.m_value1)
            ossia::position_u{std::move(other.m_impl.m_value1)};
        break;
      case Type::Type2:
        new (&m_impl.m_value2)
            ossia::speed_u{std::move(other.m_impl.m_value2)};
        break;
      case Type::Type3:
        new (&m_impl.m_value3)
            ossia::orientation_u{std::move(other.m_impl.m_value3)};
        break;
      case Type::Type4:
        new (&m_impl.m_value4)
            ossia::angle_u{std::move(other.m_impl.m_value4)};
        break;
      case Type::Type5:
        new (&m_impl.m_value5)
            ossia::color_u{std::move(other.m_impl.m_value5)};
        break;
      case Type::Type6:
        new (&m_impl.m_value6) ossia::gain_u{std::move(other.m_impl.m_value6)};
        break;
      case Type::Type7:
        new (&m_impl.m_value7)
            ossia::timing_u{std::move(other.m_impl.m_value7)};
        break;
      default:
        break;
    }
    return *this;
  }
};
template <>
inline const ossia::distance_u* unit_variant::target() const
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline const ossia::position_u* unit_variant::target() const
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline const ossia::speed_u* unit_variant::target() const
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline const ossia::orientation_u* unit_variant::target() const
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline const ossia::angle_u* unit_variant::target() const
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline const ossia::color_u* unit_variant::target() const
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline const ossia::gain_u* unit_variant::target() const
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline const ossia::timing_u* unit_variant::target() const
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline ossia::distance_u* unit_variant::target()
{
  if (m_type == Type0)
    return &m_impl.m_value0;
  return nullptr;
}
template <>
inline ossia::position_u* unit_variant::target()
{
  if (m_type == Type1)
    return &m_impl.m_value1;
  return nullptr;
}
template <>
inline ossia::speed_u* unit_variant::target()
{
  if (m_type == Type2)
    return &m_impl.m_value2;
  return nullptr;
}
template <>
inline ossia::orientation_u* unit_variant::target()
{
  if (m_type == Type3)
    return &m_impl.m_value3;
  return nullptr;
}
template <>
inline ossia::angle_u* unit_variant::target()
{
  if (m_type == Type4)
    return &m_impl.m_value4;
  return nullptr;
}
template <>
inline ossia::color_u* unit_variant::target()
{
  if (m_type == Type5)
    return &m_impl.m_value5;
  return nullptr;
}
template <>
inline ossia::gain_u* unit_variant::target()
{
  if (m_type == Type6)
    return &m_impl.m_value6;
  return nullptr;
}
template <>
inline ossia::timing_u* unit_variant::target()
{
  if (m_type == Type7)
    return &m_impl.m_value7;
  return nullptr;
}
template <>
inline const ossia::distance_u& unit_variant::get() const
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::position_u& unit_variant::get() const
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::speed_u& unit_variant::get() const
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::orientation_u& unit_variant::get() const
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::angle_u& unit_variant::get() const
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::color_u& unit_variant::get() const
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::gain_u& unit_variant::get() const
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline const ossia::timing_u& unit_variant::get() const
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::distance_u& unit_variant::get()
{
  if (m_type == Type0)
    return m_impl.m_value0;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::position_u& unit_variant::get()
{
  if (m_type == Type1)
    return m_impl.m_value1;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::speed_u& unit_variant::get()
{
  if (m_type == Type2)
    return m_impl.m_value2;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::orientation_u& unit_variant::get()
{
  if (m_type == Type3)
    return m_impl.m_value3;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::angle_u& unit_variant::get()
{
  if (m_type == Type4)
    return m_impl.m_value4;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::color_u& unit_variant::get()
{
  if (m_type == Type5)
    return m_impl.m_value5;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::gain_u& unit_variant::get()
{
  if (m_type == Type6)
    return m_impl.m_value6;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <>
inline ossia::timing_u& unit_variant::get()
{
  if (m_type == Type7)
    return m_impl.m_value7;
  throw std::runtime_error("dataspace_base_variant: bad type");
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, const unit_variant& var)
{
  switch (var.m_type)
  {
    case unit_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case unit_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case unit_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case unit_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case unit_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case unit_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case unit_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case unit_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, unit_variant& var)
{
  switch (var.m_type)
  {
    case unit_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case unit_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case unit_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case unit_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case unit_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case unit_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case unit_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case unit_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply_nonnull(Visitor&& functor, unit_variant&& var)
{
  switch (var.m_type)
  {
    case unit_variant::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case unit_variant::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case unit_variant::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case unit_variant::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case unit_variant::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case unit_variant::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case unit_variant::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case unit_variant::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    default:
      throw std::runtime_error("dataspace_base_variant: bad type");
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, const unit_variant& var)
{
  switch (var.m_type)
  {
    case unit_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case unit_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case unit_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case unit_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case unit_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case unit_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case unit_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case unit_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, unit_variant& var)
{
  switch (var.m_type)
  {
    case unit_variant::Type::Type0:
      return functor(var.m_impl.m_value0);
    case unit_variant::Type::Type1:
      return functor(var.m_impl.m_value1);
    case unit_variant::Type::Type2:
      return functor(var.m_impl.m_value2);
    case unit_variant::Type::Type3:
      return functor(var.m_impl.m_value3);
    case unit_variant::Type::Type4:
      return functor(var.m_impl.m_value4);
    case unit_variant::Type::Type5:
      return functor(var.m_impl.m_value5);
    case unit_variant::Type::Type6:
      return functor(var.m_impl.m_value6);
    case unit_variant::Type::Type7:
      return functor(var.m_impl.m_value7);
    default:
      return functor();
  }
}
template <typename Visitor>
auto apply(Visitor&& functor, unit_variant&& var)
{
  switch (var.m_type)
  {
    case unit_variant::Type::Type0:
      return functor(std::move(var.m_impl.m_value0));
    case unit_variant::Type::Type1:
      return functor(std::move(var.m_impl.m_value1));
    case unit_variant::Type::Type2:
      return functor(std::move(var.m_impl.m_value2));
    case unit_variant::Type::Type3:
      return functor(std::move(var.m_impl.m_value3));
    case unit_variant::Type::Type4:
      return functor(std::move(var.m_impl.m_value4));
    case unit_variant::Type::Type5:
      return functor(std::move(var.m_impl.m_value5));
    case unit_variant::Type::Type6:
      return functor(std::move(var.m_impl.m_value6));
    case unit_variant::Type::Type7:
      return functor(std::move(var.m_impl.m_value7));
    default:
      return functor();
  }
}
inline bool operator==(const unit_variant& lhs, const unit_variant& rhs)
{
  if (lhs.m_type == rhs.m_type)
  {
    switch (lhs.m_type)
    {
      case unit_variant::Type::Type0:
        return lhs.m_impl.m_value0 == rhs.m_impl.m_value0;
      case unit_variant::Type::Type1:
        return lhs.m_impl.m_value1 == rhs.m_impl.m_value1;
      case unit_variant::Type::Type2:
        return lhs.m_impl.m_value2 == rhs.m_impl.m_value2;
      case unit_variant::Type::Type3:
        return lhs.m_impl.m_value3 == rhs.m_impl.m_value3;
      case unit_variant::Type::Type4:
        return lhs.m_impl.m_value4 == rhs.m_impl.m_value4;
      case unit_variant::Type::Type5:
        return lhs.m_impl.m_value5 == rhs.m_impl.m_value5;
      case unit_variant::Type::Type6:
        return lhs.m_impl.m_value6 == rhs.m_impl.m_value6;
      case unit_variant::Type::Type7:
        return lhs.m_impl.m_value7 == rhs.m_impl.m_value7;
      default:
        return true;
    }
  }
  return false;
}
inline bool operator!=(const unit_variant& lhs, const unit_variant& rhs)
{
  if (lhs.m_type != rhs.m_type)
    return true;
  switch (lhs.m_type)
  {
    case unit_variant::Type::Type0:
      return lhs.m_impl.m_value0 != rhs.m_impl.m_value0;
    case unit_variant::Type::Type1:
      return lhs.m_impl.m_value1 != rhs.m_impl.m_value1;
    case unit_variant::Type::Type2:
      return lhs.m_impl.m_value2 != rhs.m_impl.m_value2;
    case unit_variant::Type::Type3:
      return lhs.m_impl.m_value3 != rhs.m_impl.m_value3;
    case unit_variant::Type::Type4:
      return lhs.m_impl.m_value4 != rhs.m_impl.m_value4;
    case unit_variant::Type::Type5:
      return lhs.m_impl.m_value5 != rhs.m_impl.m_value5;
    case unit_variant::Type::Type6:
      return lhs.m_impl.m_value6 != rhs.m_impl.m_value6;
    case unit_variant::Type::Type7:
      return lhs.m_impl.m_value7 != rhs.m_impl.m_value7;
    default:
      return false;
  }
  return true;
}
inline bool operator==(const unit_variant& lhs, const ossia::distance_u& rhs)
{
  return (lhs.m_type == unit_variant::Type::Type0)
         && (lhs.m_impl.m_value0 == rhs);
}
inline bool operator==(const ossia::distance_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type == unit_variant::Type::Type0)
         && (rhs.m_impl.m_value0 == lhs);
}
inline bool operator!=(const unit_variant& lhs, const ossia::distance_u& rhs)
{
  return (lhs.m_type != unit_variant::Type::Type0)
         || (lhs.m_impl.m_value0 != rhs);
}
inline bool operator!=(const ossia::distance_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type != unit_variant::Type::Type0)
         || (rhs.m_impl.m_value0 != lhs);
}
inline bool operator==(const unit_variant& lhs, const ossia::position_u& rhs)
{
  return (lhs.m_type == unit_variant::Type::Type1)
         && (lhs.m_impl.m_value1 == rhs);
}
inline bool operator==(const ossia::position_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type == unit_variant::Type::Type1)
         && (rhs.m_impl.m_value1 == lhs);
}
inline bool operator!=(const unit_variant& lhs, const ossia::position_u& rhs)
{
  return (lhs.m_type != unit_variant::Type::Type1)
         || (lhs.m_impl.m_value1 != rhs);
}
inline bool operator!=(const ossia::position_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type != unit_variant::Type::Type1)
         || (rhs.m_impl.m_value1 != lhs);
}
inline bool operator==(const unit_variant& lhs, const ossia::speed_u& rhs)
{
  return (lhs.m_type == unit_variant::Type::Type2)
         && (lhs.m_impl.m_value2 == rhs);
}
inline bool operator==(const ossia::speed_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type == unit_variant::Type::Type2)
         && (rhs.m_impl.m_value2 == lhs);
}
inline bool operator!=(const unit_variant& lhs, const ossia::speed_u& rhs)
{
  return (lhs.m_type != unit_variant::Type::Type2)
         || (lhs.m_impl.m_value2 != rhs);
}
inline bool operator!=(const ossia::speed_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type != unit_variant::Type::Type2)
         || (rhs.m_impl.m_value2 != lhs);
}
inline bool
operator==(const unit_variant& lhs, const ossia::orientation_u& rhs)
{
  return (lhs.m_type == unit_variant::Type::Type3)
         && (lhs.m_impl.m_value3 == rhs);
}
inline bool
operator==(const ossia::orientation_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type == unit_variant::Type::Type3)
         && (rhs.m_impl.m_value3 == lhs);
}
inline bool
operator!=(const unit_variant& lhs, const ossia::orientation_u& rhs)
{
  return (lhs.m_type != unit_variant::Type::Type3)
         || (lhs.m_impl.m_value3 != rhs);
}
inline bool
operator!=(const ossia::orientation_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type != unit_variant::Type::Type3)
         || (rhs.m_impl.m_value3 != lhs);
}
inline bool operator==(const unit_variant& lhs, const ossia::angle_u& rhs)
{
  return (lhs.m_type == unit_variant::Type::Type4)
         && (lhs.m_impl.m_value4 == rhs);
}
inline bool operator==(const ossia::angle_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type == unit_variant::Type::Type4)
         && (rhs.m_impl.m_value4 == lhs);
}
inline bool operator!=(const unit_variant& lhs, const ossia::angle_u& rhs)
{
  return (lhs.m_type != unit_variant::Type::Type4)
         || (lhs.m_impl.m_value4 != rhs);
}
inline bool operator!=(const ossia::angle_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type != unit_variant::Type::Type4)
         || (rhs.m_impl.m_value4 != lhs);
}
inline bool operator==(const unit_variant& lhs, const ossia::color_u& rhs)
{
  return (lhs.m_type == unit_variant::Type::Type5)
         && (lhs.m_impl.m_value5 == rhs);
}
inline bool operator==(const ossia::color_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type == unit_variant::Type::Type5)
         && (rhs.m_impl.m_value5 == lhs);
}
inline bool operator!=(const unit_variant& lhs, const ossia::color_u& rhs)
{
  return (lhs.m_type != unit_variant::Type::Type5)
         || (lhs.m_impl.m_value5 != rhs);
}
inline bool operator!=(const ossia::color_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type != unit_variant::Type::Type5)
         || (rhs.m_impl.m_value5 != lhs);
}
inline bool operator==(const unit_variant& lhs, const ossia::gain_u& rhs)
{
  return (lhs.m_type == unit_variant::Type::Type6)
         && (lhs.m_impl.m_value6 == rhs);
}
inline bool operator==(const ossia::gain_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type == unit_variant::Type::Type6)
         && (rhs.m_impl.m_value6 == lhs);
}
inline bool operator!=(const unit_variant& lhs, const ossia::gain_u& rhs)
{
  return (lhs.m_type != unit_variant::Type::Type6)
         || (lhs.m_impl.m_value6 != rhs);
}
inline bool operator!=(const ossia::gain_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type != unit_variant::Type::Type6)
         || (rhs.m_impl.m_value6 != lhs);
}
inline bool operator==(const unit_variant& lhs, const ossia::timing_u& rhs)
{
  return (lhs.m_type == unit_variant::Type::Type7)
         && (lhs.m_impl.m_value7 == rhs);
}
inline bool operator==(const ossia::timing_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type == unit_variant::Type::Type7)
         && (rhs.m_impl.m_value7 == lhs);
}
inline bool operator!=(const unit_variant& lhs, const ossia::timing_u& rhs)
{
  return (lhs.m_type != unit_variant::Type::Type7)
         || (lhs.m_impl.m_value7 != rhs);
}
inline bool operator!=(const ossia::timing_u& lhs, const unit_variant& rhs)
{
  return (rhs.m_type != unit_variant::Type::Type7)
         || (rhs.m_impl.m_value7 != lhs);
}
