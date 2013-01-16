#pragma once
//#include "CommInc.h"
#include "snmp_pp/snmp_pp.h"

using namespace boost;

#define MAX_POSTFIX_OID_LEN 8

struct oid_postfix
{
	unsigned long len;

	unsigned long data[MAX_POSTFIX_OID_LEN];	//MAX_OID_LEN defined in asn1.h,value assigned to 128

	oid_postfix () { memset (this, 0, sizeof oid_postfix); };
};

class SnmpConstraint
{
public:
	string			m_ip;

	string			m_read_community;

	int				m_retry;

	int				m_time_out;

	snmp_version	m_version;

public:
	SnmpConstraint () 
		: m_retry (2), m_time_out (500), m_version (version1) {};
};

class SnmpSource
{
public:
	SnmpSource(void);

public:
	~SnmpSource(void);

public:
	typedef enum { fmt_ascii, fmt_hex, fmt_binary, fmt_ip2physaddress, fmt_ip2ifindex, fmt_mac2port, fmt_ifindex, fmt_ifdescr} format_type;
	
public:
	void set_constraint (SnmpConstraint &constraint);

	bool get_next (const string & oid, map<string, pair<NnmUInt64, DWORD> > & port2val) const;

	bool get_next (const string & oid, hash_map<string, pair<NnmUInt64, DWORD> > & port2val) const;

	bool get_next (const string & oid, map<string, NnmUInt64> & port2val) const;

	bool get_next (const string & oid, hash_map<string, NnmUInt64> & port2val) const;

	bool get_next (const string & oid, map<string, string> & oid2val) const;

	bool get_next (const string & oid, hash_map<string, string> & oid2val) const;

	bool get_next (const string & oid, list<pair<oid_postfix, string> > & oid2val, format_type type = fmt_ascii) const;

	bool get_next (const string & oid, list<NnmUInt64> & value) const;

	bool get_next (const string & oid, list<string> & value, format_type type = fmt_ascii) const;

	bool get (const string & oid, map<string, pair<NnmUInt64, DWORD> > & oid2val) const;

	bool get (const string & oid, hash_map<string, pair<NnmUInt64, DWORD> > & oid2val) const;

	bool get (const string & oid, map<string, NnmUInt64> & oid2val);

	bool get (const string & oid, hash_map<string, NnmUInt64> & oid2val);

	bool get_bulk (const string & oid, map<string, pair<NnmUInt64, DWORD> > & port2val) const;

	bool get_bulk (const string & oid, map<string, NnmUInt64> & oid2val) const;

	bool get_bulk (const string & oid, map<string, string>	& oid2val) const;

	bool get_bulk (const string & oid, list<pair<oid_postfix, string> > & oid2val, format_type type = fmt_ascii) const;

	bool get_bulk (const string & oid, list<NnmUInt64> & value) const;

	bool get_bulk (const string & oid, list<string> & value, format_type type = fmt_ascii) const;

	bool get_version (const string & oid) const;

private:
	string get_printable_oid (const char * oid, format_type type) const;

	void get_printable_oid (Oid & oid, format_type type, oid_postfix & postfix) const;

	string get_printable_value (OctetStr & os, format_type type) const;

private:
	const int				m_BULK_MAX;

	const size_t			m_MAX_ITEM;

	pair<string, string>	m_community;

	shared_ptr<Snmp>		m_snmp;

	CTarget					m_target;

	bool					m_init;

	mutable int				m_error_code;

	mutable string			m_error_msg;
};
