#pragma once

#include <boost/core/noncopyable.hpp>
#include <boost/thread.hpp>
#include <unordered_map>
#include <sstream>
#include <string>
#include <mutex>

template <class REC>
class DB : private boost::noncopyable
{
public:
	DB(const std::string& identifierName) :
		m_identifierName(identifierName)
	{}

	void update(size_t key, const REC& rec);

	template<class REC>
	friend std::ostream& operator<<(std::ostream& os, const DB<REC>& rec);

	operator std::string() const;

private:
	std::unordered_map<size_t, REC> m_map;

	boost::shared_mutex m_access;
	std::string m_identifierName; // the name of the key. used for printing
};

template<class REC>
inline void DB<REC>::update(size_t key, const REC& rec)
{
	{
		// read lock	
		boost::shared_lock<boost::shared_mutex> lock(m_access);

		// check if key exists
		if (m_map.count(key))
		{
			// aggragate values in val
			REC& val = m_map[key];
			val.add(rec);
			return;
		}
	}
	{
		// key did not exists. Write lock
		boost::upgrade_lock<boost::shared_mutex> lock(m_access);
		boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

		if (m_map.count(key)) // check again, maybe the key has been created since the read lock was released
		{
			REC& val = m_map[key];
			val.add(rec);
			return;
		}
		else
		{
			// key does not exist. create new key-val in map
			m_map[key] = rec;
		}
	}
}

template<class REC>
std::ostream& operator<<(std::ostream& os, const DB<REC>& db)
{
	for (auto& elem : db.m_map)
	{
		size_t key = elem.first;
		REC rec = elem.second;

		os << db.m_identifierName << ": " << key << std::endl << "--------------------------------" << std::endl;

		os << static_cast<std::string>(rec);
		os << std::endl;
	}

	return os;
}

template<class REC>
inline DB<REC>::operator std::string() const
{
	std::stringstream ss;
	ss << *this;
	return ss.str();
}
