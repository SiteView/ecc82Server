#include "SnmpSource.h"

SnmpSource::SnmpSource(void) : m_BULK_MAX (30), m_MAX_ITEM (100000)
{
	int status;
	m_snmp = shared_ptr<Snmp>(new Snmp(status));
	if (status != SNMP_CLASS_SUCCESS) {
		string err = string ("SnmpSource object construction error: ") + 
			string (m_snmp->error_msg (status));
		Log::report(err);
		m_init = false;
	}
}

SnmpSource::~SnmpSource(void)
{
}

void SnmpSource::set_constraint (SnmpConstraint &constraint)
{
	snmp_version ver;
	if (SnmpVersionProcessor::get_snmp_version (constraint.m_ip, ver))
		constraint.m_version = ver;

	m_target.set_version (constraint.m_version);
	m_target.set_address (static_cast<IpAddress> (constraint.m_ip.c_str ()));
	m_target.set_readcommunity (constraint.m_read_community.c_str ());
	m_target.set_timeout (constraint.m_time_out);
	m_target.set_retry (constraint.m_retry);
}

bool SnmpSource::get_next (const string & oid, list<NnmUInt64> & value) const
{
	assert (m_init);

	int status;
	string err, port;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	while ((status = m_snmp->get_next(pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb(vb, 0);
		vb.get_oid(item_oid);
		if (table_oid.nCompare(table_oid.len(), item_oid) != 0) {
			break;
		}

		switch (vb.get_syntax ()) {
		case sNMP_SYNTAX_UINT32:
		case sNMP_SYNTAX_CNTR32:
		case sNMP_SYNTAX_TIMETICKS:
			{
				unsigned long v = 0;
				vb.get_value (v);
				value.push_back (v);
			}
			break;
		case sNMP_SYNTAX_INT32:
			{
				int v = 0;
				vb.get_value (v);
				value.push_back (v);
			}
			break;
		}

		if (value.size () > m_MAX_ITEM) 
			break;
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_next () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_next (const string & oid, list<string> & value, format_type type/* = fmt_ascii*/) const
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	while ((status = m_snmp->get_next(pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb (vb, 0);
		vb.get_oid (item_oid);
		if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
			break;
		}

		if (vb.get_syntax () == sNMP_SYNTAX_OCTETS) {
			OctetStr octet;
			vb.get_value (octet);
			value.push_back (get_printable_value (octet, type));
		} else {
			value.push_back (vb.get_printable_value ());
		}

		if (value.size () > m_MAX_ITEM) 
			break;
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_next () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_next (const string & oid, /*hash_*/map<string, pair<NnmUInt64, DWORD> > &port2val) const
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	while ((status = m_snmp->get_next(pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb (vb, 0);
		vb.get_oid (item_oid);
		if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
			break;
		}

		oid_tmp = vb.get_printable_oid ();
		pos = oid_tmp.find_last_of (OID_DELIMITER);
		port = oid_tmp.substr (pos + 1, oid_tmp.size () - pos - 1);

		switch (vb.get_syntax ()) {
		case sNMP_SYNTAX_UINT32:
		case sNMP_SYNTAX_CNTR32:
		case sNMP_SYNTAX_TIMETICKS:
			{
				unsigned long v = 0;
				vb.get_value (v);
				port2val.insert (make_pair (port, make_pair (v, 0)));
			}
			break;
		case sNMP_SYNTAX_INT32:
			{
				int v = 0;
				vb.get_value (v);
				port2val.insert (make_pair (port, make_pair (v, 0)));
			}
			break;
		}

		if (port2val.size () > m_MAX_ITEM) 
			break;
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_next () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_next (const string & oid, hash_map<string, pair<NnmUInt64, DWORD> > & port2val) const
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	while ((status = m_snmp->get_next(pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb (vb, 0);
		vb.get_oid (item_oid);
		if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
			break;
		}

		oid_tmp = vb.get_printable_oid ();
		pos = oid_tmp.find_last_of (OID_DELIMITER);
		port = oid_tmp.substr (pos + 1, oid_tmp.size () - pos - 1);

		switch (vb.get_syntax ()) {
		case sNMP_SYNTAX_UINT32:
		case sNMP_SYNTAX_CNTR32:
		case sNMP_SYNTAX_TIMETICKS:
			{
				unsigned long v = 0;
				vb.get_value (v);
				port2val.insert (make_pair (port, make_pair (v, 0)));
			}
			break;
		case sNMP_SYNTAX_INT32:
			{
				int v = 0;
				vb.get_value (v);
				port2val.insert (make_pair (port, make_pair (v, 0)));
			}
			break;
		}

		if (port2val.size () > m_MAX_ITEM) 
			break;
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_next () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_next (const string & oid, /*hash_*/map<string, NnmUInt64> & port2val) const
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	while ((status = m_snmp->get_next(pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb (vb, 0);
		vb.get_oid (item_oid);
		if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
			break;
		}

		oid_tmp = vb.get_printable_oid ();
		pos = oid_tmp.find_last_of (OID_DELIMITER);
		port = oid_tmp.substr (pos + 1, oid_tmp.size () - pos - 1);

		switch (vb.get_syntax ()) {
		case sNMP_SYNTAX_UINT32:
		case sNMP_SYNTAX_CNTR32:
		case sNMP_SYNTAX_TIMETICKS:
			{
				unsigned long v = 0;
				vb.get_value (v);
				port2val.insert (make_pair (port, v));
			}
			break;
		case sNMP_SYNTAX_INT32:
			{
				int v = 0;
				vb.get_value (v);
				port2val.insert (make_pair (port, v));
			}
			break;
		}

		if (port2val.size () > m_MAX_ITEM) 
			break;
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_next () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_next (const string & oid, hash_map<string, NnmUInt64> & port2val) const
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	while ((status = m_snmp->get_next(pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb (vb, 0);
		vb.get_oid (item_oid);
		if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
			break;
		}

		oid_tmp = vb.get_printable_oid ();
		pos = oid_tmp.find_last_of (OID_DELIMITER);
		port = oid_tmp.substr (pos + 1, oid_tmp.size () - pos - 1);

		switch (vb.get_syntax ()) {
		case sNMP_SYNTAX_UINT32:
		case sNMP_SYNTAX_CNTR32:
		case sNMP_SYNTAX_TIMETICKS:
			{
				unsigned long v = 0;
				vb.get_value (v);
				port2val.insert (make_pair (port, v));
			}
			break;
		case sNMP_SYNTAX_INT32:
			{
				int v = 0;
				vb.get_value (v);
				port2val.insert (make_pair (port, v));
			}
			break;
		}

		if (port2val.size () > m_MAX_ITEM) 
			break;
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_next () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_bulk (const string & oid, map<string, NnmUInt64> &oid2val) const
{
	assert(m_init);

	string err;	
	int status;
	Vb vb(oid.c_str());
	Pdu pdu;
	Oid table_oid = oid.c_str();
	Oid item_oid;
	
	pdu += vb;
	while ((status = m_snmp->get_bulk (pdu, m_target, 0, m_BULK_MAX)) == SNMP_CLASS_SUCCESS) {
		for (int i = 0; i < pdu.get_vb_count(); ++i) {
			pdu.get_vb( vb,i);

			vb.get_oid(item_oid);

			if (table_oid.nCompare(table_oid.len(), item_oid) != 0) {
				return true;
			}

			switch (vb.get_syntax ()) {
			case sNMP_SYNTAX_UINT32:
			case sNMP_SYNTAX_CNTR32:
			case sNMP_SYNTAX_TIMETICKS:
				{
					unsigned long v = 0;
					vb.get_value (v);
					oid2val.insert (make_pair (vb.get_printable_oid (), v));
				}
				break;
			case sNMP_SYNTAX_INT32:
				{
					int v = 0;
					vb.get_value (v);
					oid2val.insert (make_pair (vb.get_printable_oid (), v));
				}
				break;
			}
		}
		pdu.set_vblist(&vb, 1);
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_bulk () error: ")
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_bulk (const string & oid, map<string, pair<NnmUInt64, DWORD> > & port2val) const
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	while ((status = m_snmp->get_bulk (pdu, m_target, 0, m_BULK_MAX)) == SNMP_CLASS_SUCCESS) {
		for (int i = 0; i < pdu.get_vb_count(); ++i) {
			pdu.get_vb( vb,i);

			vb.get_oid(item_oid);

			if (table_oid.nCompare(table_oid.len(), item_oid) != 0) {
				return true;
			}

			oid_tmp = vb.get_printable_oid ();
			pos = oid_tmp.find_last_of (OID_DELIMITER);
			port = oid_tmp.substr (pos + 1, oid_tmp.size () - pos - 1);

			switch (vb.get_syntax ()) {
			case sNMP_SYNTAX_UINT32:
			case sNMP_SYNTAX_CNTR32:
			case sNMP_SYNTAX_TIMETICKS:
				{
					unsigned long v = 0;
					vb.get_value (v);
					port2val.insert (make_pair (port, make_pair (v, 0)));
				}
				break;
			case sNMP_SYNTAX_INT32:
				{
					int v = 0;
					vb.get_value (v);
					port2val.insert (make_pair (port, make_pair (v, 0)));
				}
				break;
			}
		}
		pdu.set_vblist(&vb, 1);
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_bulk () error: ")
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_bulk (const string & oid, list<pair<oid_postfix, string> > & oid2val, format_type type/* = fmt_ascii*/) const
{
	assert(m_init);

	string err;
	int status;
	Vb vb(oid.c_str());
	Pdu pdu;
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	while ((status = m_snmp->get_next (pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		for (int i = 0; i < pdu.get_vb_count(); ++i) {
			pdu.get_vb( vb,i);

			vb.get_oid (item_oid);

			if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
				break;
			}

			if (vb.get_syntax () == sNMP_SYNTAX_OCTETS) {
				OctetStr octet;
				vb.get_value (octet);
				string value = get_printable_value (octet, type);

				oid_postfix postfix;
				get_printable_oid (item_oid, type, postfix);

				oid2val.push_back (make_pair (postfix, value));
			} else {
				oid_postfix postfix;
				get_printable_oid (item_oid, type, postfix);
				oid2val.push_back (make_pair (postfix, vb.get_printable_value ()));
			}
		}
		pdu.set_vblist(&vb, 1);
	}
	
	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_bulk () error: ")
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_bulk (const string & oid, list<NnmUInt64> & value) const
{
	assert (m_init);

	int status;
	string err, port;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	while ((status = m_snmp->get_next(pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		for (int i = 0; i < pdu.get_vb_count(); ++i) {
			pdu.get_vb(vb, i);
			vb.get_oid(item_oid);
			if (table_oid.nCompare(table_oid.len(), item_oid) != 0) {
				break;
			}

			switch (vb.get_syntax ()) {
			case sNMP_SYNTAX_UINT32:
			case sNMP_SYNTAX_CNTR32:
			case sNMP_SYNTAX_TIMETICKS:
				{
					unsigned long v = 0;
					vb.get_value (v);
					value.push_back (v);
				}
				break;
			case sNMP_SYNTAX_INT32:
				{
					int v = 0;
					vb.get_value (v);
					value.push_back (v);
				}
				break;
			}
		}
		pdu.set_vblist(&vb, 1);
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_bulk () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_bulk (const string & oid, list<string> & value, format_type type/* = fmt_ascii*/) const
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	while ((status = m_snmp->get_next(pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		for (int i = 0; i < pdu.get_vb_count(); ++i) {
			pdu.get_vb(vb, i);
			pdu.get_vb (vb, 0);
			vb.get_oid (item_oid);
			if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
				break;
			}

			if (vb.get_syntax () == sNMP_SYNTAX_OCTETS) {
				OctetStr octet;
				vb.get_value (octet);
				value.push_back (get_printable_value (octet, type));
			} else {
				value.push_back (vb.get_printable_value ());
			}
		}
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_bulk () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_next (const string & oid, map<string, string> &oid2val) const
{
	assert(m_init);

	string err;
	int status;
	Vb vb(oid.c_str());
	Pdu pdu;
	Oid table_oid = oid.c_str();
	Oid item_oid;
	
	pdu += vb;
	while ( (status = m_snmp->get_next(pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb(vb, 0);
		vb.get_oid(item_oid);

		if (table_oid.nCompare(table_oid.len(), item_oid) != 0) {
			break;
		}

		oid2val.insert (make_pair (vb.get_printable_oid (), vb.get_printable_value ()));

		if (oid2val.size () > m_MAX_ITEM) 
			break;
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_next () error: ")
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_next (const string & oid, hash_map<string, string> &oid2val) const
{
	assert(m_init);

	string err;
	int status;
	Vb vb(oid.c_str());
	Pdu pdu;
	Oid table_oid = oid.c_str();
	Oid item_oid;
	
	pdu += vb;
	while ( (status = m_snmp->get_next(pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb(vb, 0);
		vb.get_oid(item_oid);

		if (table_oid.nCompare(table_oid.len(), item_oid) != 0) {
			break;
		}

		oid2val.insert (make_pair (vb.get_printable_oid (), vb.get_printable_value ()));

		if (oid2val.size () > m_MAX_ITEM) 
			break;
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_next () error: ")
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_next (const string & oid, list<pair<oid_postfix, string> > & oid2val, format_type type/* = fmt_ascii*/) const
{
	assert(m_init);

	string err;
	int status;
	Vb vb(oid.c_str());
	Pdu pdu;
	Oid table_oid = oid.c_str();
	Oid item_oid;
	
	pdu += vb;
	while ((status = m_snmp->get_next (pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb (vb, 0);
		vb.get_oid (item_oid);

		if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
			break;
		}

		if (vb.get_syntax () == sNMP_SYNTAX_OCTETS) {
			OctetStr octet;
			vb.get_value (octet);
			string value = get_printable_value (octet, type);

			oid_postfix postfix;
			get_printable_oid (item_oid, type, postfix);

			oid2val.push_back (make_pair (postfix, value));
		} else {
			oid_postfix postfix;
			get_printable_oid (item_oid, type, postfix);
			oid2val.push_back (make_pair (postfix, vb.get_printable_value ()));
		}

		if (oid2val.size () > m_MAX_ITEM) 
			break;
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_next () error: ")
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}

bool SnmpSource::get_bulk (const string & oid, map<string, string>	&oid2val) const
{
	assert(m_init);

	int status;
	Vb vb(oid.c_str());
	Pdu pdu;
	Oid table_oid = oid.c_str();
	Oid item_oid;
	
	pdu += vb;
	while ( (status = m_snmp->get_bulk (pdu, m_target, 0, m_BULK_MAX)) == SNMP_CLASS_SUCCESS) {
		for ( int i = 0; i < pdu.get_vb_count(); i++) {
			pdu.get_vb( vb,i);

			vb.get_oid(item_oid);

			cout << table_oid.get_printable () << "\n";
			cout << item_oid.get_printable () << "\n";

			if (table_oid.nCompare(table_oid.len(), item_oid) != 0) {
				return true;
			}

			switch (vb.get_syntax ())
			{
			case sNMP_SYNTAX_OCTETS:
				{
					//OctetStr os;
					//vb.get_value (os);
					unsigned char mac[2050];
					memset ((char*)mac, 0, sizeof mac);
					vb.get_value ((char*)mac);

					oid2val.insert (make_pair (vb.get_printable_oid (),
						vb.get_printable_value ()));
				}
				break;
			}
		}
		pdu.set_vblist(&vb, 1);
	}

	if (status != SNMP_CLASS_SUCCESS) {
		//debug_report("Snmp get_next error: ", m_snmp->error_msg(status));
		//SetLastError(status);
		return false;
	}

	return true;
}

bool SnmpSource::get (const string & oid, map<string, pair<NnmUInt64, DWORD> > & oid2val) const
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	if ((status = m_snmp->get (pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb (vb, 0);
		vb.get_oid (item_oid);
		if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
			return false;
		}

		oid_tmp = vb.get_printable_oid ();
		pos = oid_tmp.find_last_of (OID_DELIMITER);
		port = oid_tmp.substr (pos + 1, oid_tmp.size () - pos - 1);

		switch (vb.get_syntax ()) {
		case sNMP_SYNTAX_UINT32:
		case sNMP_SYNTAX_CNTR32:
		case sNMP_SYNTAX_TIMETICKS:
			{
				unsigned long v = 0;
				vb.get_value (v);
				oid2val.insert (make_pair (port, make_pair (v, 0)));
			}
			break;
		case sNMP_SYNTAX_INT32:
			{
				int v = 0;
				vb.get_value (v);
				oid2val.insert (make_pair (port, make_pair (v, 0)));
			}
			break;
		}
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}
	
	DWORD tick = GetTickCount ();
	map<string, pair<NnmUInt64, DWORD> >::iterator i = oid2val.begin ();
	map<string, pair<NnmUInt64, DWORD> >::iterator end = oid2val.end();
	for (; i != end; ++i) {
		i->second.second = tick;
	}

	return true;
}

bool SnmpSource::get (const string & oid, hash_map<string, pair<NnmUInt64, DWORD> > & oid2val) const
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	if ((status = m_snmp->get (pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb (vb, 0);
		vb.get_oid (item_oid);
		if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
			return false;
		}

		oid_tmp = vb.get_printable_oid ();
		pos = oid_tmp.find_last_of (OID_DELIMITER);
		port = oid_tmp.substr (pos + 1, oid_tmp.size () - pos - 1);

		switch (vb.get_syntax ()) {
		case sNMP_SYNTAX_UINT32:
		case sNMP_SYNTAX_CNTR32:
		case sNMP_SYNTAX_TIMETICKS:
			{
				unsigned long v = 0;
				vb.get_value (v);
				oid2val.insert (make_pair (port, make_pair (v, 0)));
			}
			break;
		case sNMP_SYNTAX_INT32:
			{
				int v = 0;
				vb.get_value (v);
				oid2val.insert (make_pair (port, make_pair (v, 0)));
			}
			break;
		}
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}
	
	DWORD tick = GetTickCount ();
	hash_map<string, pair<NnmUInt64, DWORD> >::iterator i = oid2val.begin ();
	hash_map<string, pair<NnmUInt64, DWORD> >::iterator end = oid2val.end();
	for (; i != end; ++i) {
		i->second.second = tick;
	}

	return true;
}

bool SnmpSource::get (const string & oid, map<string, NnmUInt64> & oid2val)
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	if ((status = m_snmp->get (pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb (vb, 0);
		vb.get_oid (item_oid);
		if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
			return false;
		}

		oid_tmp = vb.get_printable_oid ();
		pos = oid_tmp.find_last_of (OID_DELIMITER);
		port = oid_tmp.substr (pos + 1, oid_tmp.size () - pos - 1);

		switch (vb.get_syntax ()) {
		case sNMP_SYNTAX_UINT32:
		case sNMP_SYNTAX_CNTR32:
		case sNMP_SYNTAX_TIMETICKS:
			{
				unsigned long v = 0;
				vb.get_value (v);
				oid2val.insert (make_pair (port, v));
			}
			break;
		case sNMP_SYNTAX_INT32:
			{
				int v = 0;
				vb.get_value (v);
				oid2val.insert (make_pair (port, v));
			}
			break;
		}
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}
	
	return true;
}

bool SnmpSource::get (const string & oid, hash_map<string, NnmUInt64> & oid2val)
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	if ((status = m_snmp->get (pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb (vb, 0);
		vb.get_oid (item_oid);
		if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
			return false;
		}

		oid_tmp = vb.get_printable_oid ();
		pos = oid_tmp.find_last_of (OID_DELIMITER);
		port = oid_tmp.substr (pos + 1, oid_tmp.size () - pos - 1);

		switch (vb.get_syntax ()) {
		case sNMP_SYNTAX_UINT32:
		case sNMP_SYNTAX_CNTR32:
		case sNMP_SYNTAX_TIMETICKS:
			{
				unsigned long v = 0;
				vb.get_value (v);
				oid2val.insert (make_pair (port, v));
			}
			break;
		case sNMP_SYNTAX_INT32:
			{
				int v = 0;
				vb.get_value (v);
				oid2val.insert (make_pair (port, v));
			}
			break;
		}
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}
	
	return true;
}

string SnmpSource::get_printable_value (OctetStr & os, format_type type) const
{
	string res;
	char octet_buf[4096];
	memset (octet_buf, 0, sizeof octet_buf);

	switch (type) {
		case fmt_ascii:
		case fmt_ifdescr:
			res = os.get_printable_clear ();
			break;

		case fmt_hex:
		case fmt_ip2physaddress:
			{
				assert (os.len () == 6);
				if (os.len () != 6) {
					return res;
				}

				unsigned char * bin_ptr = os.data ();
				sprintf_s (octet_buf, sizeof octet_buf, "%2.2X %2.2X %2.2X %2.2X %2.2X %2.2X", bin_ptr[0], bin_ptr[1], bin_ptr[2], bin_ptr[3], bin_ptr[4], bin_ptr[5]);
				res = octet_buf;
			}
			break;

		case fmt_binary:
			memcpy_s(octet_buf, sizeof (octet_buf), os.data (), os.len ());
			if (sizeof (octet_buf) > os.len ()) {
				octet_buf[os.len ()] = '\0';
			}
			res = octet_buf;
			break;
	}

	return res;
}

string SnmpSource::get_printable_oid (const char * oid, format_type type/*= fmt_mac*/) const
{
	string s = oid;
	switch (type) {
		case fmt_ip2ifindex:
		case fmt_ip2physaddress:
			{
				int cnt = 4;
				string::size_type prev_pos = 0, pos = 0;

				pos = s.rfind (OID_DELIMITER);
				while (pos != string::npos && cnt-- != 0) {
					prev_pos = pos;
					pos = s.rfind (OID_DELIMITER, prev_pos - 1);
				}

				if (cnt != 0) {
					s = s.substr (prev_pos + 1, s.size () - prev_pos + 1);
				}		
			}
			break;
	}

	return s;
}

void SnmpSource::get_printable_oid (Oid & oid, format_type type, oid_postfix & postfix) const
{
	string s;
	unsigned long len = 0, oid_len = 0;

	switch (type) {
		case fmt_mac2port:
			len = 6;
			break;

		case fmt_ip2ifindex:
		case fmt_ip2physaddress:
			len = 4;
			break;

		case fmt_ifindex:
		case fmt_ifdescr:
			len = 1;
			break;
	}

	oid_len = oid.oidval ()->len;
	if (len == 0 || oid_len < len) {
		return;
	}

	unsigned long i = oid_len - len;
	for (; i < oid_len; i++) {
		if (i - oid_len + len == MAX_OID_LEN) {
			break;
		}
		postfix.data[i - oid_len + len] = oid.oidval()->ptr[i];
	}
	postfix.len = len;
}

bool SnmpSource::get_version (const string & oid) const
{
	assert(m_init);

	int status;
	string err, port, oid_tmp;
	string::size_type pos = 0;
	Pdu pdu;
	Vb vb(oid.c_str());
	Oid table_oid = oid.c_str();
	Oid item_oid;

	pdu += vb;
	while ((status = m_snmp->get_next(pdu, m_target)) == SNMP_CLASS_SUCCESS) {
		break;
		//pdu.get_vb (vb, 0);
		//vb.get_oid (item_oid);
		//if (table_oid.nCompare (table_oid.len (), item_oid) != 0) {
		//	break;
		//}

		//oid_tmp = vb.get_printable_oid ();
		//pos = oid_tmp.find_last_of (OID_DELIMITER);
		//port = oid_tmp.substr (pos + 1, oid_tmp.size () - pos - 1);

		//switch (vb.get_syntax ()) {
		//case sNMP_SYNTAX_UINT32:
		//case sNMP_SYNTAX_CNTR32:
		//case sNMP_SYNTAX_TIMETICKS:
		//	{
		//		unsigned long v = 0;
		//		vb.get_value (v);
		//		port2val.insert (make_pair (port, v));
		//	}
		//	break;
		//case sNMP_SYNTAX_INT32:
		//	{
		//		int v = 0;
		//		vb.get_value (v);
		//		port2val.insert (make_pair (port, v));
		//	}
		//	break;
		//}

		//if (port2val.size () > m_MAX_ITEM) 
		//	break;
	}

	if (status != SNMP_CLASS_SUCCESS) {
		err = string ("SnmpSource::get_version () error: ") 
			+ string (m_snmp->error_msg (status));
		Log::report (err, false);
		return false;
	}

	return true;
}