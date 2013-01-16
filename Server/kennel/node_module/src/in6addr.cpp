// Copyright (C) 1999-2005 Open Source Telecom Corporation.
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// 
// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however    
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.    
//
// This exception applies only to the code released under the name GNU
// Common C++.  If you copy code from other releases into a copy of GNU
// Common C++, as the General Public License permits, the exception does
// not apply to the code that you add in this way.  To avoid misleading
// anyone as to the status of such modified files, you must delete
// this exception notice from them.
//
// If you write modifications of your own for GNU Common C++, it is your choice
// whether to permit this exception to apply to your modifications.
// If you do not wish that, delete this exception notice.
//

#include <cc++/config.h>
#include <cc++/export.h>
#include <cc++/address.h>
#include "private.h"
#include <cstdlib>

#ifdef	CCXX_IPV6

#ifdef	CCXX_NAMESPACES
namespace ost {
#endif

#ifndef WIN32
Mutex IPV6Address::mutex;
#endif

const IPV6MulticastValidator IPV6Multicast::validator;

void IPV6MulticastValidator::operator()(const in6_addr address) const
{
#ifdef	CCXX_EXCEPTIONS
	// "0000:" is always accepted, as it is an "empty" address.
	if ( (address.s6_addr[0] != 0 || address.s6_addr[1] != 0) &&
	     (address.s6_addr[0] != 0xff || address.s6_addr[1] < 0x1f)) {
		throw "Multicast address not in the valid prefix ff00-ff1f:";
	}
#endif
}

IPV6Address::IPV6Address(const IPV6Validator *_validator) 
	: validator(_validator), hostname(NULL)
{
	addr_count = 1;
	ipaddr = new struct in6_addr[1];
	memcpy(ipaddr, &in6addr_any, sizeof(in6_addr));
}

IPV6Address::IPV6Address(const char *address, const IPV6Validator *_validator) : 
	validator(_validator), addr_count(0), ipaddr(NULL), hostname(NULL)
{
	if ( this->validator )
		this->validator = validator;
	if(address == 0 || !strcmp(address, "*"))
		setAddress(NULL);
	else
		setAddress(address);
}

IPV6Address::IPV6Address(struct in6_addr addr, const IPV6Validator *_validator) : 
	validator(_validator), ipaddr(NULL), hostname(NULL)
{
	if ( this->validator ){
		this->validator = validator;
		(*validator)(addr);
	}
	addr_count = 1;
	ipaddr = new struct in6_addr[1];
	memcpy(&ipaddr, &addr, sizeof(struct in6_addr));
}

IPV6Address::IPV6Address(const IPV6Address &rhs) : 
	validator(rhs.validator), addr_count(rhs.addr_count), hostname(NULL)
{
	ipaddr = new struct in6_addr[addr_count];
	memcpy(ipaddr, rhs.ipaddr, sizeof(struct in6_addr) * addr_count);
}

IPV6Address::~IPV6Address()
{
	if(ipaddr)
	{
		delete[] ipaddr;
		ipaddr = NULL;
	}
	if(hostname)
	{
		delString(hostname);
		hostname = NULL;
	}
}

struct in6_addr IPV6Address::getAddress(void) const
{
	return ipaddr[0];
}

struct in6_addr IPV6Address::getAddress(size_t i) const
{
	return (i < addr_count ? ipaddr[i] : ipaddr[0]);
}

bool IPV6Address::isInetAddress(void) const
{
	struct in6_addr addr;
	memset(&addr, 0, sizeof(addr));
	if(!ipaddr)
		return false;
	if(memcmp(&addr, &ipaddr[0], sizeof(addr)))
		return true;
	return false;
}

IPV6Address &IPV6Address::operator=(const char *str)
{
	if(str == 0 || !strcmp(str, "*"))
		str = "::";

	setAddress(str);

	return *this;
}

IPV6Address &IPV6Address::operator=(struct in6_addr addr)
{
	if(ipaddr)
		delete[] ipaddr;
	if ( validator )
		(*validator)(addr);
	addr_count = 1;
	ipaddr = new struct in6_addr[1];
	ipaddr[0] = addr;
	if(hostname)
		delString(hostname);
	hostname = NULL;
	return *this;
}

IPV6Address &IPV6Address::operator=(const IPV6Address &rhs)
{
	if(this == &rhs) return *this;

	addr_count = rhs.addr_count;
	if(ipaddr)
		delete[] ipaddr;
	ipaddr = new struct in6_addr[addr_count];
	memcpy(ipaddr, rhs.ipaddr, sizeof(struct in6_addr) * addr_count);
	validator = rhs.validator;
	if(hostname)
		delString(hostname);
	hostname = NULL;

	return *this;
}

bool IPV6Address::operator==(const IPV6Address &a) const
{
	const IPV6Address *smaller, *larger;
	size_t s, l;
	
	if(addr_count > a.addr_count)
	{
		smaller = &a;
		larger  = this;
	}
	else
	{
		smaller = this;
		larger  = &a;
	}

	// Loop through all addr's in the smaller and make sure
	// that they are all in the larger
	for(s = 0; s < smaller->addr_count; s++)
	{
		// bool found = false;
		for(l = 0; l < larger->addr_count && 
			memcmp((char *)&ipaddr[s], (char *)&a.ipaddr[l], sizeof(struct in6_addr)); l++);
		if(l == larger->addr_count) return false;
	}
	return true;
}

bool IPV6Address::operator!=(const IPV6Address &a) const
{
	// Impliment in terms of operator==
	return (*this == a ? false : true);
}

IPV6Host &IPV6Host::operator&=(const IPV6Mask &ma)
{
	for(size_t i = 0; i < addr_count; i++)
	{		
		struct in6_addr mask = ma.getAddress();
		unsigned char *a = (unsigned char *)&ipaddr[i];
		unsigned char *m = (unsigned char *)&mask;
		
		for(size_t j = 0; j < sizeof(struct in6_addr); ++j)
			*(a++) &= *(m++);
	}
	if(hostname)
		delString(hostname);
	hostname = NULL;

	return *this;
}
	
IPV6Host::IPV6Host(struct in6_addr addr) :
IPV6Address(addr) {}

IPV6Host::IPV6Host(const char *host) : 
IPV6Address(host)
{
	char namebuf[256];

	if(!host)
	{
		gethostname(namebuf, 256);
		setAddress(namebuf);
	}
}

bool IPV6Address::setIPAddress(const char *host)
{
	if(!host)
		return false;

	struct in6_addr l_addr;

#ifdef	WIN32
	struct sockaddr saddr;
	int slen = sizeof(saddr);
	struct sockaddr_in6 *paddr = (struct sockaddr_in6 *)&saddr;
	int ok = WSAStringToAddress((LPSTR)host, AF_INET6, NULL, &saddr, &slen);
	l_addr = paddr->sin6_addr;
#else
	int ok = inet_pton(AF_INET6, host, &l_addr);
#endif
	if ( validator )
		(*validator)(l_addr);
	if ( !ok )
		return false;
	*this = l_addr;
	return true;
}

#if defined(HAVE_GETADDRINFO) && !defined(HAVE_GETHOSTBYNAME2)

void IPV6Address::setAddress(const char *host)
{	
	if(hostname)
		delString(hostname);
	hostname = NULL;

	if(!host)  // The way this is currently used, this can never happen
		host = "::";

#ifdef	WIN32
	if(!stricmp(host, "localhost"))
		host = "::1";
#endif

	if(!setIPAddress(host))
	{
		struct addrinfo hint, *list = NULL, *first;
		memset(&hint, 0, sizeof(hint));
		hint.ai_family = AF_INET6;
		struct in6_addr *addr;
		struct sockaddr_in6 *ip6addr;

		if(getaddrinfo(host, NULL, &hint, &list) || !list)
		{
			if(ipaddr)
				delete[] ipaddr;
			ipaddr = new struct in6_addr[1];
			memset((void *)&ipaddr[0], 0, sizeof(ipaddr));
			return;
		}
		
		// Count the number of IP addresses returned
		addr_count = 0;
		first = list;
		while(list)
		{
			++addr_count;
			list = list->ai_next;
		}

		// Allocate enough memory
		if(ipaddr)
			delete[] ipaddr;	// Cause this was allocated in base
		ipaddr = new struct in6_addr[addr_count];

		// Now go through the list again assigning to 
		// the member ipaddr;
		list = first;
		int i = 0;
		while(list)
		{
			ip6addr = (struct sockaddr_in6 *)list->ai_addr;
			addr = &ip6addr->sin6_addr;
			if(validator)
				(*validator)(*addr);
			ipaddr[i++] = *addr;
			list = list->ai_next;
		}
		freeaddrinfo(first);
	}
}

#else

void IPV6Address::setAddress(const char *host)
{	
	if(hostname)
		delString(hostname);
	hostname = NULL;

	if(!host)  // The way this is currently used, this can never happen
		host = "::";

#ifdef	WIN32
	if(!stricmp(host, "localhost"))
		host = "::1";
#endif

	if(!setIPAddress(host))
	{
		struct hostent *hp;
		struct in6_addr **bptr;
#if defined(__GLIBC__)
		char   hbuf[8192];
		struct hostent hb;
		int    rtn;

		if(gethostbyname2_r(host, AF_INET6, &hb, hbuf, sizeof(hbuf), &hp, &rtn))
			hp = NULL;
#elif defined(sun)
		char   hbuf[8192];
		struct hostent hb;
		int    rtn;

		hp = gethostbyname2_r(host, AF_INET6, &hb, hbuf, sizeof(hbuf), &rtn);
#elif (defined(__osf__) || defined(_OSF_SOURCE) || defined(__hpux))
		hp = gethostbyname(host);
#elif defined(WIN32) && (!defined(_MSC_VER) || _MSC_VER < 1300)
		hp = gethostbyname(host);
#elif defined(WIN32)
		hp = gethostbyname2(host, AF_INET6);
#else
		mutex.enterMutex();
		hp = gethostbyname2(host, AF_INET6);
		mutex.leaveMutex();
#endif
		if(!hp)
		{
			if(ipaddr)
				delete[] ipaddr;
			ipaddr = new struct in6_addr[1];
			memset((void *)&ipaddr[0], 0, sizeof(ipaddr));
			return;
		}
		
		// Count the number of IP addresses returned
		addr_count = 0;
		for(bptr = (struct in6_addr **)hp->h_addr_list; *bptr != NULL; bptr++)
		{
			addr_count++;
		}

		// Allocate enough memory
		if(ipaddr)
			delete[] ipaddr;	// Cause this was allocated in base
		ipaddr = new struct in6_addr[addr_count];

		// Now go through the list again assigning to 
		// the member ipaddr;
		bptr = (struct in6_addr **)hp->h_addr_list;
		for(unsigned int i = 0; i < addr_count; i++)
		{
			if ( validator )
				(*validator)(*bptr[i]);
			ipaddr[i] = *bptr[i];
		}
	}
}

#endif

IPV6Broadcast::IPV6Broadcast(const char *net) : 
IPV6Address(net)
{
}

IPV6Mask::IPV6Mask(const char *mask) :
IPV6Address(mask)
{
}

const char *IPV6Address::getHostname(void) const
{
	struct hostent *hp = NULL;
	struct in6_addr addr0;
	static char strbuf[64];

	memset(&addr0, 0, sizeof(addr0));
	if(!memcmp(&addr0, &ipaddr[0], sizeof(addr0)))
		return NULL;

	if(!memcmp(&in6addr_loopback, &ipaddr[0], sizeof(addr0)))
		return "localhost";

#if defined(__GLIBC__)
	char   hbuf[8192];
	struct hostent hb;
	int    rtn;
	if(gethostbyaddr_r((char *)&ipaddr[0], sizeof(addr0), AF_INET6, &hb, hbuf, sizeof(hbuf), &hp, &rtn))
		hp = NULL;
#elif defined(sun)
	char   hbuf[8192];
	struct hostent hb;
	int    rtn;
	hp = gethostbyaddr_r((char *)&ipaddr[0], sizeof(addr0), AF_INET6, &hb, hbuf, (int)sizeof(hbuf), &rtn);
#elif defined(__osf__) || defined(WIN32)
	hp = gethostbyaddr((char *)&ipaddr[0], sizeof(addr0), AF_INET6);
#else
	mutex.enterMutex();
	hp = gethostbyaddr((char *)&ipaddr[0], sizeof(addr0), AF_INET6);
	mutex.leaveMutex();
#endif
	if(hp) {
		if(hostname)
			delString(hostname);
		hostname = newString(hp->h_name);
		return hostname;
	} else {
#ifdef	WIN32
		struct sockaddr saddr;
		struct sockaddr_in6 *paddr = (struct sockaddr_in6 *)&saddr;
		DWORD slen = sizeof(strbuf);
		memset(&saddr, 0, sizeof(saddr));
		paddr->sin6_family = AF_INET6;
		paddr->sin6_addr = ipaddr[0];
		WSAAddressToString(&saddr, sizeof(saddr), NULL, strbuf, &slen);
		return strbuf;
#else
		return inet_ntop(AF_INET6, &ipaddr[0], strbuf, sizeof(strbuf));
#endif
	}
}

IPV6Host operator&(const IPV6Host &addr, const IPV6Mask &mask)
{
	IPV6Host temp = addr;
	temp &= mask;
	return temp;
}

IPV6Multicast::IPV6Multicast() : 
IPV6Address(&validator)
{ }

IPV6Multicast::IPV6Multicast(const char *address) :
IPV6Address(address,&validator)
{}

#ifdef	CCXX_NAMESPACES
}
#endif

#endif

/** EMACS **
 * Local variables:
 * mode: c++
 * c-basic-offset: 8
 * End:
 */
