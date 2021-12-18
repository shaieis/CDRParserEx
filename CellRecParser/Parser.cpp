#include <string>
#include <sstream>
#include <istream>

#include "Parser.h"

void Parser::parseIntoQueue(Queue<CDRRecord>& queue) 
{
	std::string line;
	while (std::getline(m_inputStream, line))
	{
		CDRRecord cdrRec = parseLine(line);

		queue.push(cdrRec);
	}
}

CDRRecord Parser::parseLine(const std::string& line) const
{
	std::stringstream lineStream(line);
	std::string seg;
	
	// MSISDN
	std::getline(lineStream, seg, '|');
	size_t MSISDN = std::stoi(seg);

	// BRAND
	std::string brand;
	std::getline(lineStream, brand, '|');

	// MMC/MNC
	std::getline(lineStream, seg, '|');
	size_t MMC_MNC = std::stoi(seg);

	// Call type
	std::getline(lineStream, seg, '|');
	CDRRecord::RecType callType = CDRRecord::stringToRecType(seg);

	// Duration
	std::getline(lineStream, seg, '|');
	size_t duration = std::stoi(seg);

	// Download (Only GPRS)
	std::getline(lineStream, seg, '|');

	size_t download = 0;
	if (callType == CDRRecord::RecType::GPRS)
	{
		download = std::stoi(seg);
	}

	// Upload (Only GPRS)
	std::getline(lineStream, seg, '|');

	size_t upload = 0;
	if (callType == CDRRecord::RecType::GPRS)
	{
		upload = std::stoi(seg);
	}
	
	// Third party MSISDN (Empty for GPRS)
	std::getline(lineStream, seg, '|');
	size_t thdPartyMSISDN = 0;
	if (callType != CDRRecord::RecType::GPRS)
	{
		thdPartyMSISDN = std::stoi(seg);
	}

	// Third party MMC MNC
	std::getline(lineStream, seg, '|');
	size_t thdPartyMMC_MNC = 0;
	thdPartyMMC_MNC = std::stoi(seg);
	
	// To ensure RVO, CDRRecord is initialized on the return value
	if (callType == CDRRecord::RecType::GPRS)
	{
		return CDRRecord::makeGPRSRecord(MSISDN,
			brand,
			MMC_MNC,
			duration,
			download,
			upload,
			thdPartyMMC_MNC);
	}
	else
	{
		return CDRRecord::makeNonGPRSRecord(MSISDN,
			brand,
			MMC_MNC,
			duration,
			callType,
			thdPartyMMC_MNC,
			thdPartyMSISDN);
	}
}
