#pragma once
#include <atomic>
#include <ostream>

#include "CDRRecord.h"

class OperatorDBRec
{
public:
	explicit OperatorDBRec(std::string networkName = "",
		size_t incomingCalls = 0, 
		size_t outgoingCalls = 0,
		size_t incomingSMS = 0,
		size_t outgoingSMS = 0,
		size_t upload = 0,
		size_t download = 0)
		:
		m_networkName(networkName),
		m_incomingCalls(incomingCalls),
		m_outgoingCalls(outgoingCalls),
		m_incomingSMS(incomingSMS),
		m_outgoingSMS(outgoingSMS),
		m_upload(upload),
		m_download(download)
	{}

	OperatorDBRec(const OperatorDBRec& rhs) 
		:
		m_networkName(rhs.m_networkName),
		m_incomingCalls(rhs.m_incomingCalls.load()),
		m_outgoingCalls(rhs.m_outgoingCalls.load()),
		m_incomingSMS(rhs.m_incomingSMS.load()),
		m_outgoingSMS(rhs.m_outgoingSMS.load()),
		m_upload(rhs.m_upload.load()),
		m_download(rhs.m_download.load())
	{}

	OperatorDBRec(const CDRRecord& rec);
	OperatorDBRec& operator=(const OperatorDBRec& rhs);

	operator std::string() const;
	friend std::ostream& operator<<(std::ostream& os, const OperatorDBRec& rec);
	void add(const OperatorDBRec& rec);

private:
	std::string m_networkName;
	std::atomic<size_t> m_incomingCalls;
	std::atomic<size_t> m_outgoingCalls;

	std::atomic<size_t> m_incomingSMS;
	std::atomic<size_t> m_outgoingSMS;

	std::atomic<size_t> m_upload;
	std::atomic<size_t> m_download;
};

