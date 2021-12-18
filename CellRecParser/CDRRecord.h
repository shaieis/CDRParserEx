#pragma once
#include <string>

class CDRRecord
{
public:
	static constexpr size_t MAX_NETWORK_NAME_LEN = 64;
	
	enum class RecType
	{
		INVALID = -1,
		MOC,
		MTC,
		SMS_MO,
		SMS_MT,
		GPRS,
		Count
	};
	static constexpr const char* typeStrs[(size_t)RecType::Count] = 
	{ "MOC", "MTC", "SMS-MO", "SMS-MT", "GPRS" };

	explicit CDRRecord(size_t MSISDN = 0,
		const std::string& networkName = "",
		size_t MMC_MNC = 0,
		const RecType& callType = RecType::INVALID,
		size_t duration = 0,
		size_t download = 0,
		size_t upload = 0,
		size_t thdPartyMSISDN = 0,
		size_t thdPartyMMC_MNC = 0);

	static CDRRecord makeGPRSRecord(size_t MSISDN,
		const std::string& networkName,
		size_t MMC_MNC,
		size_t duration,
		size_t download,
		size_t upload,
		size_t thdPartyMMC_MNC);

	static CDRRecord makeNonGPRSRecord(size_t MSISDN,
		const std::string& networkName,
		size_t MMC_MNC,
		size_t duration,
		const RecType& callType,
		size_t thdPartyMSISDN,
		size_t thdPartyMMC_MNC);

public:
	// getters
	inline size_t getMSISDN() const { return m_MSISDN; };
	inline RecType getType() const { return m_callType; };

	inline size_t getCallDuration() const { return m_duration; };

	inline size_t getUpload() const { return m_upload; };
	inline size_t getDownload() const { return m_download; };

	inline const char* getNetworkName() const { return m_networkName; };
	
	inline size_t getMMC_MNC() const { return m_MMC_MNC; };

	// returns true if record is inter-network 
	inline bool isInternal() const { return m_MMC_MNC == m_thdPartyMMC_MNC; };
	
	// get Type by string
	static RecType stringToRecType(const std::string& strType);

private:
	size_t m_MSISDN;
	char m_networkName[MAX_NETWORK_NAME_LEN +1]; // can't use std::stirng because 
	                                             // boost queue requires trivial destructors
	size_t m_MMC_MNC;
	RecType m_callType;
	size_t m_duration;
	size_t m_download;
	size_t m_upload;
	size_t m_thdPartyMSISDN;
	size_t m_thdPartyMMC_MNC;
};
