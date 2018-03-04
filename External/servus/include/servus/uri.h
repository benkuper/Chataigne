/* Copyright (c) 2013-2014, ahmet.bilgili@epfl.ch
 *                    2014, Stefan.Eilemann@epfl.ch
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

#ifndef SERVUS_URI_H
#define SERVUS_URI_H

#include <servus/api.h>
#include <servus/types.h>

#include <map>
#include <sstream>

namespace servus
{
namespace detail
{
class URI;
}

/**
 * The URI class parses the given uri using the generic syntax from RFC3986 and
 * RFC6570
 * @verbatim
 * http://bob@www.example.com:8080/path/?key=value&foo=bar#fragment
 * ^   ^  ^  ^               ^    ^     ^                 ^
 * a   b  c  d               e    f     g                 h
 *
 * URI part	Range	String
 * scheme	[a, b)	"http"
 * userinfo [c, d) bob
 * host	[d, e)	"www.example.com"
 * port (e, f) 8080
 * path	[f, g)	"/path/"
 * query (g, h)	"key=value&foo=bar"
 * fragment	(h,-) "fragment"
 * @endverbatim
 *
 * Queries are parsed into key-value pairs and can be accessed using
 * findQuery(), queryBegin() and queryEnd().
 *
 * We enforce schemas to have the separator "://", not only ":" which is enough
 * for the RFC specification.
 *
 * Example: @include tests/uri.cpp
 */
class URI
{
public:
    typedef std::map<std::string, std::string> KVMap;
    typedef KVMap::const_iterator ConstKVIter;

    /** Construct an empty URI. */
    SERVUS_API URI();

    /**
     * @param uri URI string to parse.
     * @throw std::exception for incomplete URIs, and std::invalid_argument
     *        if the port is not a number.
     */
    SERVUS_API explicit URI(const std::string& uri);

    /** @overload URI::URI( const std::string& ) */
    SERVUS_API explicit URI(const char* uri);

    /** Copy-construct an URI. */
    SERVUS_API URI(const URI& from);

    SERVUS_API ~URI();

    /** Assign the data from another URI. */
    SERVUS_API URI& operator=(const URI& rhs);

    /** Equals operator */
    SERVUS_API bool operator==(const URI& rhs) const;

    /** Not equals operator */
    SERVUS_API bool operator!=(const URI& rhs) const;

    /** @name Getters for uri data */
    //@{
    SERVUS_API const std::string& getScheme() const;
    SERVUS_API const std::string& getUserinfo() const;
    SERVUS_API uint16_t getPort() const;
    SERVUS_API const std::string& getHost() const;
    /** Return the compound authority part of the URI.

        User info added only if not empty, port number added only if it's
        different from 0. */
    SERVUS_API std::string getAuthority() const;
    SERVUS_API const std::string& getPath() const;
    SERVUS_API const std::string& getQuery() const;
    SERVUS_API const std::string& getFragment() const;
    //@}

    /** @name Setters for uri data. */
    //@{
    SERVUS_API void setScheme(const std::string& scheme);
    SERVUS_API void setUserInfo(const std::string& userinfo);
    SERVUS_API void setHost(const std::string& host);
    SERVUS_API void setPort(uint16_t port);
    SERVUS_API void setPath(const std::string& path);
    SERVUS_API void setQuery(const std::string& query);
    SERVUS_API void setFragment(const std::string& fragment);
    //@}

    /** @name Access to key-value data in query. */
    //@{
    /**
     * @return a const iterator to the beginning of the query map.
     */
    SERVUS_API ConstKVIter queryBegin() const;

    /**
     * @return a const iterator to end beginning of the query map.
     */
    SERVUS_API ConstKVIter queryEnd() const;

    /**
     * @return a const iterator to the given key, or queryEnd().
     */
    SERVUS_API ConstKVIter findQuery(const std::string& key) const;

    /** Add a key-value pair to the query. */
    SERVUS_API void addQuery(const std::string& key, const std::string& value);
    //@}

private:
    detail::URI* const _impl;
};

inline std::ostream& operator<<(std::ostream& os, const URI& uri)
{
    if (!uri.getScheme().empty())
        os << uri.getScheme() << "://";
    // A valid URI can't contain the user info or port number alone, so if
    // the host name is empty the other two field are simply ignored.
    if (!uri.getHost().empty())
    {
        if (!uri.getUserinfo().empty())
            os << uri.getUserinfo() << "@";
        os << uri.getHost();
        if (uri.getPort())
            os << ':' << uri.getPort();
    }
    os << uri.getPath();
    if (!uri.getQuery().empty())
        os << '?' << uri.getQuery();
    if (!uri.getFragment().empty())
        os << '#' << uri.getFragment();
    return os;
}
}

namespace std
{
inline std::string to_string(const servus::URI& uri)
{
    ostringstream os;
    os << uri;
    return os.str();
}
}
#endif // SERVUS_URI_H
