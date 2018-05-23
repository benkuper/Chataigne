#pragma once
#include <ossia/network/dataspace/dataspace_base.hpp>

namespace ossia
{
struct linear_u;
struct gain_dataspace;
template <typename Impl>
struct gain_unit
{
  using is_unit = std::true_type;
  using neutral_unit = linear_u;
  using value_type = float;
  using concrete_type = Impl;
  using dataspace_type = gain_dataspace;
};

/** Utility functions taken from Jamoma TTBase.h **/
namespace detail
{
const constexpr double DecibelHeadroom = 96.;
const double GainMidiPower
    = std::log(ossia::log1p(12. / DecibelHeadroom) / std::log(127. / 100.))
      / std::log(2.);
const double GainMidiPowPow2 = ossia::exp2(GainMidiPower);

template <typename T>
T LinearGainToDecibels(const T value)
{
  return value >= 0. ? 20.0 * std::log10(value) : 0.;
}

template <typename T>
T LinearGainToDecibelsClipped(const T value)
{
  return value <= 0. ? -DecibelHeadroom
                     : clamp_min(20.0 * std::log10(value), -DecibelHeadroom);
}

template <typename T>
T DecibelsToLinearGain(const T value)
{
  return std::pow(10., value * 0.05);
}

template <typename T>
T DecibelsToLinearGainClipped(const T value)
{
  return value <= -DecibelHeadroom ? 0. : DecibelsToLinearGain(value);
}

template <typename T>
T MidiToLinearGain(const T value)
{
  return value <= 0.
             ? 0.
             : DecibelsToLinearGainClipped(
                   DecibelHeadroom
                   * (std::pow(value / 100., ossia::exp2(GainMidiPower))
                      - 1.));
}

template <typename T>
T DecibelsToMidi(const T value)
{
  return value <= -DecibelHeadroom
             ? 0.
             : 100. * std::exp(
                          ossia::log1p(value / DecibelHeadroom)
                          / GainMidiPowPow2);
}

template <typename T>
T LinearGainToMidi(const T value)
{
  return DecibelsToMidi(LinearGainToDecibels(value));
}
}

struct OSSIA_EXPORT linear_u : public gain_unit<linear_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("linear");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return self;
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR value_type
  from_neutral(strong_value<neutral_unit> self)
  {
    return self.dataspace_value;
  }
};

struct OSSIA_EXPORT midigain_u : public gain_unit<midigain_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("midigain");
  }

  static strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return detail::MidiToLinearGain(self.dataspace_value);
  }

  static value_type from_neutral(strong_value<neutral_unit> self)
  {
    return detail::LinearGainToMidi(self.dataspace_value);
  }
};

struct OSSIA_EXPORT decibel_u : public gain_unit<decibel_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("db", "dB");
  }

  static strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return detail::DecibelsToLinearGainClipped(self.dataspace_value);
  }

  static value_type from_neutral(strong_value<neutral_unit> self)
  {
    return detail::LinearGainToDecibelsClipped(self.dataspace_value);
  }
};

struct OSSIA_EXPORT decibel_raw_u : public gain_unit<decibel_raw_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("db-raw", "dB-raw");
  }

  static strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return detail::DecibelsToLinearGain(self.dataspace_value);
  }

  static value_type from_neutral(strong_value<neutral_unit> self)
  {
    return 20.0 * (std::log10(self.dataspace_value));
  }
};
}
