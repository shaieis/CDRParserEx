#pragma once
#include <atomic>
#include "CDRRecord.h"

class CustomerDBRec
{
public:
	explicit CustomerDBRec(size_t internalOutgoingCalls = 0,
		size_t internalIncomingCalls = 0,
		size_t externalOutgoingCalls = 0,
		size_t externalIncomingCalls = 0,
		size_t internalIncomingSMS = 0,
		size_t internalOutgoingSMS = 0,
		size_t externalIncomingSMS = 0,
		size_t externalOutgoingSMS = 0,
		size_t upload = 0,
		size_t download = 0)
		:
		m_internalOutgoingCalls(internalOutgoingCalls),
		m_internalIncomingCalls(internalIncomingCalls),
		m_externalOutgoingCalls(externalOutgoingCalls),
		m_externalIncomingCalls(externalIncomingCalls),
		m_internalIncomingSMS(internalIncomingSMS),
		m_internalOutgoingSMS(internalOutgoingSMS),
		m_externalIncomingSMS(externalIncomingSMS),
		m_externalOutgoingSMS(externalOutgoingSMS),
		m_upload(upload),
		m_download(download)
	{}

	CustomerDBRec(const CustomerDBRec& rhs)
		:
		m_internalOutgoingCalls(rhs.m_internalOutgoingCalls.load()),
		m_internalIncomingCalls(rhs.m_internalIncomingCalls.load()),
		m_externalOutgoingCalls(rhs.m_externalOutgoingCalls.load()),
		m_externalIncomingCalls(rhs.m_externalIncomingCalls.load()),
		m_internalIncomingSMS(rhs.m_internalIncomingSMS.load()),
		m_internalOutgoingSMS(rhs.m_internalOutgoingSMS.load()),
		m_externalIncomingSMS(rhs.m_externalIncomingSMS.load()),
		m_externalOutgoingSMS(rhs.m_externalOutgoingSMS.load()),
		m_upload(rhs.m_upload.load()),
		m_download(rhs.m_download.load())
	{}

	CustomerDBRec(const CDRRecord& rec);
	CustomerDBRec& operator=(const CustomerDBRec& rhs);

	friend std::ostream& operator<<(std::ostream& os, const CustomerDBRec& rec);
	operator std::string() const;
	void add(const CustomerDBRec& rec);

private:
	std::atomic<size_t> m_internalOutgoingCalls;
	std::atomic<size_t> m_internalIncomingCalls;

	std::atomic<size_t> m_externalOutgoingCalls;
	std::atomic<size_t> m_externalIncomingCalls;

	std::atomic<size_t> m_internalIncomingSMS;
	std::atomic<size_t> m_internalOutgoingSMS;

	std::atomic<size_t> m_externalIncomingSMS;
	std::atomic<size_t> m_externalOutgoingSMS;

	std::atomic<size_t> m_upload;
	std::atomic<size_t> m_download;
};
