#include <sstream>

#include "CustomerDBRec.h"

CustomerDBRec::CustomerDBRec(const CDRRecord& rec) : CustomerDBRec()
{
	switch (rec.getType())
	{
	case CDRRecord::RecType::MOC:
		// Outgoing call
		if (rec.isInternal())
		{
			// Internal outgoing
			m_internalOutgoingCalls = rec.getCallDuration();
		}
		else
		{
			// External outgoing
			m_externalOutgoingCalls = rec.getCallDuration();
		}
		break;
	case CDRRecord::RecType::MTC:
		// Incoming call
		if (rec.isInternal())
		{
			// Internal incoming
			m_internalIncomingCalls = rec.getCallDuration();
		}
		else
		{
			// External incoming
			m_externalIncomingCalls = rec.getCallDuration();
		}
		break;
	case CDRRecord::RecType::SMS_MO:
		// Outgoing SMS
		if (rec.isInternal())
		{
			// Internal
			m_internalOutgoingSMS = 1;
		}
		else
		{
			// External
			m_externalOutgoingSMS = 1;
		}
		break;
	case CDRRecord::RecType::SMS_MT:
		// Incoming SMS
		if (rec.isInternal())
		{
			// Internal
			m_internalIncomingSMS = 1;
		}
		else
		{
			// External
			m_externalIncomingSMS = 1;
		}
		break;
	case CDRRecord::RecType::GPRS:
		// GPRS
		m_upload = rec.getUpload();
		m_download = rec.getDownload();
		break;
	}
}

CustomerDBRec& CustomerDBRec::operator=(const CustomerDBRec& rhs)
{
	if (this == &rhs)
		return *this;

	m_internalOutgoingCalls = rhs.m_internalOutgoingCalls.load();
	m_internalIncomingCalls = rhs.m_internalIncomingCalls.load();

	m_externalOutgoingCalls = rhs.m_externalOutgoingCalls.load();
	m_externalIncomingCalls = rhs.m_externalIncomingCalls.load();

	m_internalIncomingSMS = rhs.m_internalIncomingSMS.load();
	m_internalOutgoingSMS = rhs.m_internalOutgoingSMS.load();

	m_externalIncomingSMS = rhs.m_externalIncomingSMS.load();
	m_externalOutgoingSMS = rhs.m_externalOutgoingSMS.load();

	m_upload = rhs.m_upload.load();
	m_download = rhs.m_download.load();
	
	return *this;
}

void CustomerDBRec::add(const CustomerDBRec& rec)
{
	m_internalOutgoingCalls.fetch_add(rec.m_internalOutgoingCalls);
	m_internalIncomingCalls.fetch_add(rec.m_internalIncomingCalls);

	m_externalOutgoingCalls.fetch_add(rec.m_externalOutgoingCalls);
	m_externalIncomingCalls.fetch_add(rec.m_externalIncomingCalls);

	m_internalIncomingSMS.fetch_add(rec.m_internalIncomingSMS);
	m_internalOutgoingSMS.fetch_add(rec.m_internalOutgoingSMS);

	m_externalIncomingSMS.fetch_add(rec.m_externalIncomingSMS);
	m_externalOutgoingSMS.fetch_add(rec.m_externalOutgoingSMS);

	m_upload.fetch_add(rec.m_upload);
	m_download.fetch_add(rec.m_download);
}

std::ostream& operator<<(std::ostream& os, const CustomerDBRec& rec)
{
	os << "In-network incoming calls (Minutes): " << rec.m_internalIncomingCalls.load() << std::endl <<
		"In-network outgoing calls (Minutes): " << rec.m_internalOutgoingCalls.load() << std::endl <<
		"In-network incoming SMS: " << rec.m_internalIncomingSMS.load() << std::endl <<
		"In-network outgoing calls: " << rec.m_internalOutgoingSMS.load() << std::endl <<
		"Between-network incoming calls (Minutes): " << rec.m_externalIncomingCalls.load() << std::endl <<
		"Between-network outgoing calls (Minutes): " << rec.m_externalOutgoingCalls.load() << std::endl <<
		"Between-network incoming SMS: " << rec.m_externalIncomingSMS.load() << std::endl <<
		"Between-network outgoing calls: " << rec.m_externalOutgoingSMS.load() << std::endl <<
		"Upload (MB): " << rec.m_upload.load() << std::endl <<
		"Download (MB): " << rec.m_download.load() << std::endl;
	
	return os;
}

CustomerDBRec::operator std::string() const
{
	std::stringstream ss;
	ss << *this;

	return ss.str();
}
