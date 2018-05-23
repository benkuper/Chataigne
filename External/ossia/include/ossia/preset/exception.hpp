#pragma once
#include <ossia/detail/config.hpp>

#include <exception>
#include <string>

namespace ossia
{

class ossiaException : public std::exception
{
protected:
  const std::string m_message;
  ossiaException(
      int lineno, const std::string& filename, const std::string& details,
      const std::string& name);

public:
  ossiaException(
      int lineno, const std::string& filename,
      const std::string& details = {});
  const char* what() const noexcept override;
};

class ossiaException_InvalidJSON : public ossiaException
{
public:
  ossiaException_InvalidJSON(
      int line, const std::string& filename, const std::string& details = {});
};

class ossiaException_InvalidXML : public ossiaException
{
public:
  ossiaException_InvalidXML(
      int line, const std::string& filename, const std::string& details = {});
};

class ossiaException_InvalidAddress : public ossiaException
{
public:
  ossiaException_InvalidAddress(
      int line, const std::string& filename, const std::string& details = {});
};
}
