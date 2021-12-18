#include <memory>

#include "CDRRecord.h"

CDRRecord::CDRRecord(size_t MSISDN,
	const std::string& networkName,
	size_t MMC_MNC,
	const RecType& callType,
	size_t duration,
	size_t download,
	size_t upload,
	size_t thdPartyMSISDN,
	size_t thdPartyMMC_MNC)
	:
	m_MSISDN(MSISDN),
	m_MMC_MNC(MMC_MNC),
	m_callType(callType),
	m_duration(duration),
	m_upload(upload),
	m_download(download),
	m_thdPartyMSISDN(thdPartyMSISDN),
	m_thdPartyMMC_MNC(thdPartyMMC_MNC),
	m_networkName("")
{
	memcpy(m_networkName, networkName.c_str(), MAX_NETWORK_NAME_LEN);
	m_networkName[MAX_NETWORK_NAME_LEN] = '\0';
}

CDRRecord CDRRecord::makeNonGPRSRecord(size_t MSISDN,
	const std::string& networkName,
	size_t MMC_MNC,
	size_t duration,
	const RecType& callType,
	size_t thdPartyMSISDN,
	size_t thdPartyMMC_MNC)
{
	return CDRRecord(MSISDN, networkName, MMC_MNC, callType, duration, 0, 0, thdPartyMSISDN, thdPartyMMC_MNC);
}

CDRRecord CDRRecord::makeGPRSRecord(size_t MSISDN,
	const std::string& networkName, 
	size_t MMC_MNC, 
	size_t duration, 
	size_t download, 
	size_t upload, 
	size_t thdPartyMMC_MNC)
{
	return CDRRecord(MSISDN, networkName,MMC_MNC, RecType::GPRS, duration, download, upload, 0, thdPartyMMC_MNC);
}

CDRRecord::RecType CDRRecord::stringToRecType(const std::string& strType)
{
	for (size_t i = 0; i < static_cast<size_t>(RecType::Count); ++i)
	{
		if (!strType.compare(typeStrs[i]))
			return static_cast<RecType>(i);
	}

	return RecType::INVALID;
}
