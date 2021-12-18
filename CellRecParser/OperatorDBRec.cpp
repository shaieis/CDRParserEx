#include <sstream>

#include "OperatorDBRec.h"

OperatorDBRec::OperatorDBRec(const CDRRecord& rec) : 
	OperatorDBRec(rec.getNetworkName())
{
	switch (rec.getType())
	{
	case CDRRecord::RecType::MOC:
		// Outgoing call
		m_outgoingCalls = rec.getCallDuration();
		break;
	case CDRRecord::RecType::MTC:
		// Incoming call
		m_incomingCalls = rec.getCallDuration();
		break;
	case CDRRecord::RecType::SMS_MO:
		// Outgoing SMS
		m_outgoingSMS = 1;
		break;
	case CDRRecord::RecType::SMS_MT:
		// Incoming SMS
		m_incomingSMS = 1;
		break;
	case CDRRecord::RecType::GPRS:
		// GPRS
		m_upload = rec.getUpload(), 
		m_download = rec.getDownload();
		break;
	}
}

OperatorDBRec& OperatorDBRec::operator=(const OperatorDBRec& rhs)
{
	if (this == &rhs)
		return *this;

	m_networkName = rhs.m_networkName;
	m_incomingCalls = rhs.m_incomingCalls.load();
	m_outgoingCalls = rhs.m_outgoingCalls.load();

	m_incomingSMS = rhs.m_incomingSMS.load();
	m_outgoingSMS = rhs.m_outgoingSMS.load();

	m_upload = rhs.m_upload.load();
	m_download = rhs.m_download.load();

	return *this;
}

void OperatorDBRec::add(const OperatorDBRec& rec)
{
	m_incomingCalls.fetch_add(rec.m_incomingCalls);
	m_outgoingCalls.fetch_add(rec.m_outgoingCalls);
	
	m_incomingSMS.fetch_add(rec.m_incomingSMS);
	m_outgoingSMS.fetch_add(rec.m_outgoingSMS);
	
	m_upload.fetch_add(rec.m_upload);
	m_download.fetch_add(rec.m_download);
}
std::ostream& operator<<(std::ostream& os, const OperatorDBRec& rec)
{
	os << "Operator name: " << rec.m_networkName << std::endl <<
		"Incoming calls (Minutes): " << rec.m_incomingCalls.load() << std::endl <<
		"Outgoing calls (Minutes): " << rec.m_outgoingCalls.load() << std::endl <<
		"Incoming SMS: " << rec.m_incomingSMS.load() << std::endl <<
		"Outgoing SMS: " << rec.m_outgoingSMS.load() << std::endl <<
		"Upload (MB): " << rec.m_upload.load() << std::endl <<
		"Download (MB): " << rec.m_download.load() << std::endl;

	return os;
}


OperatorDBRec::operator std::string() const
{
	std::stringstream ss;
	ss << *this;

	return ss.str();
}
