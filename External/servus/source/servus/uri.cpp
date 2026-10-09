/* Copyright (c) 2013-2016, Human Brain Project
 *                          Ahmet.Bilgili@epfl.ch
 *                          Juan Hernando <jhernando@fi.upm.es>
 *
 * This file is part of Servus <https://github.com/HBPVIS/Servus>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "uri.h"

#include <algorithm>
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace servus
{
namespace
{
struct URIData
{
    URIData()
        : port(0)
    {
    }

    bool operator==(const URIData& rhs) const
    {
        return (userinfo == rhs.userinfo && host == rhs.host &&
                port == rhs.port && path == rhs.path && query == rhs.query &&
                fragment == rhs.fragment && queryMap == rhs.queryMap);
    }

    std::string scheme;
    std::string userinfo;
    std::string host;
    uint16_t port;
    std::string path;
    std::string query;
    std::string fragment;
    URI::KVMap queryMap;
};
}

namespace detail
{
class uri_parse : public std::exception
{
public:
    explicit uri_parse(const std::string& uri)
    {
        _error = std::string("Error parsing URI string: ") + uri;
    }

    uri_parse(const uri_parse& excep) { _error = excep._error; }
    virtual ~uri_parse() throw() {}
    virtual const char* what() const throw() { return _error.c_str(); }
private:
    std::string _error;
};

enum URIPart
{
    SCHEME = 0,
    AUTHORITY,
    PATH,
    QUERY,
    FRAGMENT
};

bool _parseURIPart(std::string& input, const URIPart& part, std::string& output)
{
#ifndef NDEBUG
    const char requireFirst[] = {0, 0, 0, '?', '#'};
#endif
    const char* const separators[] = {"://", "/?#", "?#", "#", ""};
    const char* const disallowed[] = {"/?#", 0, 0, 0, 0};
    const bool fullSeparator[] = {true, false, false, false, false};
    const bool needsSeparator[] = {true, false, false, false, false};
    const size_t skip[] = {0, 0, 0, 1, 1};
    const size_t postSkip[] = {3, 0, 0, 0, 0};
    const size_t pos = fullSeparator[part]
                           ? input.find(separators[part])
                           : input.find_first_of(separators[part]);

    if (pos == std::string::npos)
    {
        if (needsSeparator[part])
        {
            output = "";
            return true;
        }
    }
    else if (pos == 0 ||
             (disallowed[part] && input.find_first_of(disallowed[part]) < pos))
    {
        output = "";
        return true;
    }
    // If the separator is not the first character, assert that parts requiring
    // an initial character find it.
    assert(!requireFirst[part] || pos == 0 || input[0] == requireFirst[part]);

    // If the separator was found at pos == 0, the returned string is empty
    assert(input.size() >= skip[part]);
    output = input.substr(skip[part], pos - skip[part]);
    input = pos == std::string::npos ? "" : input.substr(pos + postSkip[part]);
    return true;
}

void _parseAuthority(URIData& data, const std::string& auth)
{
    const size_t atPos = auth.find_first_of('@');
    if (atPos != std::string::npos)
        data.userinfo = auth.substr(0, atPos);
    const size_t hostPos = atPos == std::string::npos ? 0 : atPos + 1;
    const size_t colonPos = auth.find_first_of(':', hostPos);
    if (colonPos != std::string::npos)
    {
        const std::string port = auth.substr(colonPos + 1);
        char* end = 0;
        data.port = ::strtol(port.c_str(), &end, 10);
        if (port.empty() || end != port.c_str() + port.length())
            throw std::runtime_error(port + " is not a valid port number");
    }
    // Works regardless of colonPos == npos
    data.host = auth.substr(hostPos, colonPos - hostPos);
    if (data.host.empty())
        throw std::invalid_argument("");
}

void _parseQueryMap(URIData& data)
{
    // parse query data into key-value pairs
    std::string query = data.query;

    data.queryMap.clear();
    while (!query.empty())
    {
        const size_t nextPair = query.find('&');
        if (nextPair == 0)
        {
            query = query.substr(1);
            continue;
        }

        const std::string pair = query.substr(0, nextPair);
        if (nextPair == std::string::npos)
            query.clear();
        else
            query = query.substr(nextPair + 1);

        const size_t eq = pair.find('=');
        if (eq == 0) // empty key
            continue;
        if (eq == std::string::npos) // empty value
            data.queryMap[pair] = std::string();
        else
            data.queryMap[pair.substr(0, eq)] = pair.substr(eq + 1);
    }
}

void _toLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

class URI
{
public:
    explicit URI(const std::string& uri)
    {
        if (uri.empty())
            return;

        try
        {
            _parseURI(uri);
        }
        catch (...)
        {
            throw uri_parse(uri);
        }
    }

    URIData& getData() { return _uriData; }
    const URIData& getData() const { return _uriData; }
private:
    URIData _uriData;

    void _parseURI(std::string input)
    {
        URIPart part = SCHEME;
        while (!input.empty())
        {
            switch (part)
            {
            case SCHEME:
                _parseURIPart(input, part, _uriData.scheme);
                _toLower(_uriData.scheme);
                if (!_uriData.scheme.empty() &&
                    (!isalpha(_uriData.scheme[0]) ||
                     _uriData.scheme.find_first_not_of(
                         "abcdefghijlkmnopqrstuvwxyz0123456789+-.", 1) !=
                         std::string::npos))
                {
                    throw std::invalid_argument("");
                }
                part = _uriData.scheme == "file" || _uriData.scheme.empty()
                           ? PATH
                           : AUTHORITY;
                // from http://en.wikipedia.org/wiki/File_URI_scheme:
                //  "file:///foo.txt" is okay, while "file://foo.txt"
                // is not, although some interpreters manage to handle
                // the latter. We are "some".
                break;
            case AUTHORITY:
            {
                std::string authority;
                _parseURIPart(input, part, authority);
                if (!authority.empty())
                    _parseAuthority(_uriData, authority);
                part = PATH;
                break;
            }
            case PATH:
                _parseURIPart(input, part, _uriData.path);
                part = QUERY;
                break;
            case QUERY:
                _parseURIPart(input, part, _uriData.query);
                _parseQueryMap(_uriData);
                part = FRAGMENT;
                break;
            case FRAGMENT:
                _parseURIPart(input, part, _uriData.fragment);
                break;
            }
        }
    }
};
}

URI::URI()
    : _impl(new detail::URI(std::string()))
{
}

URI::URI(const std::string& uri)
    : _impl(new detail::URI(uri))
{
}

URI::URI(const char* uri)
    : _impl(new detail::URI(std::string(uri)))
{
}

URI::URI(const URI& from)
    : _impl(new detail::URI(*from._impl))
{
}

servus::URI::~URI()
{
    delete _impl;
}

URI& URI::operator=(const URI& rhs)
{
    if (this != &rhs)
        *_impl = *rhs._impl;
    return *this;
}

bool URI::operator==(const URI& rhs) const
{
    return this == &rhs || (_impl->getData() == rhs._impl->getData());
}

bool URI::operator!=(const URI& rhs) const
{
    return !(*this == rhs);
}

const std::string& URI::getScheme() const
{
    return _impl->getData().scheme;
}

const std::string& URI::getHost() const
{
    return _impl->getData().host;
}

std::string URI::getAuthority() const
{
    std::stringstream authority;
    if (!_impl->getData().userinfo.empty())
        authority << _impl->getData().userinfo << "@";
    // IPv6 IPs are not considered.
    authority << _impl->getData().host;
    if (_impl->getData().port)
        authority << ":" << _impl->getData().port;
    return authority.str();
}

uint16_t URI::getPort() const
{
    return _impl->getData().port;
}

const std::string& URI::getUserinfo() const
{
    return _impl->getData().userinfo;
}

const std::string& URI::getPath() const
{
    return _impl->getData().path;
}

const std::string& URI::getQuery() const
{
    return _impl->getData().query;
}

const std::string& URI::getFragment() const
{
    return _impl->getData().fragment;
}

void URI::setScheme(const std::string& scheme)
{
    _impl->getData().scheme = scheme;
}

void URI::setUserInfo(const std::string& userinfo)
{
    _impl->getData().userinfo = userinfo;
}

void URI::setHost(const std::string& host)
{
    _impl->getData().host = host;
}

void URI::setPort(const uint16_t port)
{
    _impl->getData().port = port;
}

void URI::setPath(const std::string& path)
{
    _impl->getData().path = path;
}

void URI::setQuery(const std::string& query)
{
    URIData& data = _impl->getData();
    data.query = query;
    detail::_parseQueryMap(data);
}

void URI::setFragment(const std::string& fragment)
{
    _impl->getData().fragment = fragment;
}

URI::ConstKVIter URI::queryBegin() const
{
    return _impl->getData().queryMap.begin();
}

URI::ConstKVIter URI::queryEnd() const
{
    return _impl->getData().queryMap.end();
}

URI::ConstKVIter URI::findQuery(const std::string& key) const
{
    return _impl->getData().queryMap.find(key);
}

void URI::addQuery(const std::string& key, const std::string& value)
{
    URIData& data = _impl->getData();

    data.queryMap[key] = value;
    data.fragment.clear();

    // Rebuild fragment string
    data.query.clear();
    for (URI::ConstKVIter i = queryBegin(); i != queryEnd(); ++i)
    {
        if (data.query.empty())
            data.query = i->first + "=" + i->second;
        else
            data.query += std::string("&") + i->first + "=" + i->second;
    }
}
}
