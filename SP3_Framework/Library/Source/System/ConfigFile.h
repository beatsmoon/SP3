/***********************************************************************************/
/*!
\file   ConfigFile.h
\author Lim Hon Wai
\par    email: honwai0817@gmail.com
\brief
Class to define and store information from a text file
*/
/***********************************************************************************/

#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <string>
#include <vector>
#include <map>
#include <fstream>

#define GET_VARIABLE_NAME(Variable) (#Variable)

#include <iostream>

/***********************************************************************************/
/*!
		Class ConfigFile
\brief  Defines ConfigFile parameters and methods
*/
/***********************************************************************************/

class ConfigFile {

	std::vector<std::string> comments;
	std::map<std::string, std::vector<std::string>> content;

	int index;

	bool bOverwrite;

	std::string ConfigFileName;

public:
	ConfigFile();
	ConfigFile(std::string const& ConfigFile);
	~ConfigFile();

	template <typename T>
	void ParseToFile(std::string const& systemName, std::string const& parseName, T const& parseValue);

	std::string ParseFromFile(std::string const& systemName, std::string const& parseName) const;

	template <typename T>
	void ParseFromFile(std::string const& systemName, T& variable, std::string const& variableName);

	void Push(std::string systemName, std::string line, bool autoCreateNew = false);

	std::string trim(std::string const& source, char const* delims = " \t\r\n") const;

	std::map<std::string, std::vector<std::string>> GetMap();

	void GetPreviousMapID(std::string& currentID);
	void GetNextMapID(std::string& currentID);

	void SetToOverwrite();

	template <typename T>
	static void Round(T& var);

};

/***********************************************************************************/
/*!
\brief Parse information to text file by temporarily storing into map

\param systemName
	Name of subsystem
\param parseName
	Name of variable to be passed into subsystem
\param parseValue
	Value of variable to be passed into subsystem
*/
/***********************************************************************************/

template<typename T>
inline void ConfigFile::ParseToFile(std::string const& systemName, std::string const& parseName, T const& parseValue)
{
	Push(systemName, parseName + " = " + std::to_string(parseValue));
	bOverwrite = true;
}

/***********************************************************************************/
/*!
\brief Parses information from a file, edits the variable passed in directly

\param systemName
	Name of subsystem
\param
	variable needed to be changed from the subsystem
\param variableName
	Name of variable needed from the subsystem
*/
/***********************************************************************************/
template<typename T>
inline void ConfigFile::ParseFromFile(std::string const& systemName, T& variable, std::string const& variableName)
{
	if (content.empty())
	{
		std::cout << "Config file empty." << std::endl;
	}

	for (std::map<std::string, std::vector<std::string>>::const_iterator it = content.begin(); it != content.end(); ++it)
	{
		// Search for the index
		if (it->first == systemName)
		{
			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				// Search for the variable name needed
				if (trim(it->second.at(i).substr(0, it->second.at(i).find('=') - 1)) == variableName)
				{
					std::string temp = "";
					bool negative = false;
					for (unsigned int j = it->second.at(i).find('=') + 2; j < it->second.at(i).length(); ++j)
					{
						if (it->second.at(i).at(j) == '-')
						{
							negative = true;
							continue;
						}
						temp += it->second.at(i).at(j);
					}
					double temp2 = std::stod(temp);
					if (negative)
					{
						temp2 = -temp2;
					}
					variable = static_cast<T>(temp2);
					return;
				}
			}
		}
	}

	variable = static_cast<T>(0);
}

template<typename T>
inline void ConfigFile::Round(T& var)
{
	var = static_cast<int>(var * 100 + 0.5);
	var = static_cast<T>(var / 100);
}

#endif