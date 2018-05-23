#pragma once
#include <ossia/detail/string_view.hpp>
#include <ossia/network/minuit/detail/minuit_common.hpp>
#include <array>
#include <string>
namespace ossia
{
namespace minuit
{
class name_table
{
public:
  void set_device_name(const std::string& name)
  {
    m_actions[(int)minuit_action::NamespaceRequest] = name + "?namespace";
    m_actions[(int)minuit_action::NamespaceReply] = name + ":namespace";
    m_actions[(int)minuit_action::NamespaceError] = name + "!namespace";

    m_actions[(int)minuit_action::GetRequest] = name + "?get";
    m_actions[(int)minuit_action::GetReply] = name + ":get";
    m_actions[(int)minuit_action::GetError] = name + "!get";

    m_actions[(int)minuit_action::ListenRequest] = name + "?listen";
    m_actions[(int)minuit_action::ListenReply] = name + ":listen";
    m_actions[(int)minuit_action::ListenError] = name + "!listen";
  }

  ossia::string_view get_action(minuit_action c) const
  {
    return m_actions[static_cast<int>(c)];
  }

private:
  std::array<std::string, 9> m_actions;
};
}
}
