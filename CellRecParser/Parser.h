#pragma once

#include <boost/core/noncopyable.hpp>
#include <istream>

#include "Queue.h"
#include "CDRRecord.h"

class Parser : private boost::noncopyable
{
public:
	Parser(std::istream& stream) : m_inputStream(stream) {};
	void parseIntoQueue(Queue<CDRRecord>& queue);
	
private:
	std::istream& m_inputStream;
	CDRRecord parseLine(const std::string& line) const;
};


