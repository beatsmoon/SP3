/***********************************************************************************/
/*!
\file   ConfigFile.cpp
\author Lim Hon Wai
\par    email : honwai0817@gmail.com
\brief
Class function prototype definintions for ConfigFile.h
*/
/***********************************************************************************/

#include "ConfigFile.h"

/***********************************************************************************/
/*!
\brief Parses information from a file, then returns it as a string

\param systemName
	Name of subsystem
\param parseName
	Name of variable needed from the subsystem
*/
/***********************************************************************************/

std::string ConfigFile::ParseFromFile(std::string const& systemName, std::string const& parseName) const
{
	if (content.empty())
	{
		std::cout << "Config file empty." << std::endl;
		return "0";
	}

	for (std::map<std::string, std::vector<std::string>>::const_iterator it = content.begin(); it != content.end(); ++it)
	{
		// Search for the index
		if (it->first == systemName)
		{
			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				// Search for the variable name needed behind the '=' sign
				if (trim(it->second.at(i).substr(0, it->second.at(i).find('=') - 1)) == parseName)
				{
					std::string temp = "";
					for (unsigned int j = it->second.at(i).find('=') + 2; j < it->second.at(i).length(); ++j)
					{
						char t = it->second.at(i).at(j);
						temp += t;
					}
					return temp;
				}
			}
		}
	}

	return "0";
}

/***********************************************************************************/
/*!
\brief Pushes the content needed into the content map defined in .h file

\param systemName
	Name of subsystem
\param line
	String to be pushed into subsystem
*/
/***********************************************************************************/

void ConfigFile::Push(std::string systemName, std::string line, bool autoCreateNew)
{
	bool found = false;
	for (std::map<std::string, std::vector<std::string>>::iterator it = content.begin(); it != content.end(); ++it)
	{
		if (it->first == systemName)
		{
			for (unsigned int i = 0; i < it->second.size(); ++i)
			{
				if (trim(it->second.at(i).substr(0, line.find('=') - 1)) == trim(line.substr(0, line.find("=") - 1)))
				{
					it->second.at(i) = line;
					found = true;
					// Break nested for loop if we already have the text we want
					goto BreakNested;
				}
			}
			it->second.push_back(line);
			found = true;
			break;
		}
	}

BreakNested:
	// If the parser did not find any match, it will automatically make a new system with the corresponding name.
	if (found == false)
	{
		std::vector<std::string> newVector;
		content.insert(std::pair<std::string, std::vector<std::string>>(systemName, newVector));
		// If the boolean flag for creating a new section for edit is enabled, create new section for future edit.
		if (autoCreateNew == true)
		{
			content.insert(std::pair<std::string, std::vector<std::string>>("NEW SECTION", newVector));
		}
		content[systemName].push_back(line);
	}
}

/***********************************************************************************/
/*!
\brief Trims a string for removing formatting: eg [Format] to Format

\param source
	String needed for trimming : Credit http://www.adp-gmbh.ch/cpp/config_file.html
\param delims
	Character to trim out
*/
/***********************************************************************************/
std::string ConfigFile::trim(std::string const& source, char const* delims) const
{
	std::string result(source);
	std::string::size_type index = result.find_last_not_of(delims);
	if (index != std::string::npos)
		result.erase(++index);

	index = result.find_first_not_of(delims);
	if (index != std::string::npos)
		result.erase(0, index);
	else
		result.erase();
	return result;
}

/***********************************************************************************/
/*!
\brief Get content map

*/
/***********************************************************************************/

std::map<std::string, std::vector<std::string>> ConfigFile::GetMap()
{
	return content;
}

/***********************************************************************************/
/*!
\brief Get the previous ID (Map uses first as ID, and traversing map with string is not easy.)

\param currentID
	String of current index
*/
/***********************************************************************************/
void ConfigFile::GetPreviousMapID(std::string& currentID)
{
	std::string temp = "";
	for (std::map<std::string, std::vector<std::string>>::iterator it = content.begin(); it != content.end(); ++it)
	{
		if (it->first == currentID)
		{
			if (temp == "")
			{
				break;
			}
			currentID = temp;
		}
		else
		{
			temp = it->first;
		}
	}
}

/***********************************************************************************/
/*!
\brief Get the next ID (Map uses first as ID, and traversing map with string is not easy.)

\param currentID
	String of current index
*/
/***********************************************************************************/
void ConfigFile::GetNextMapID(std::string& currentID)
{
	bool found = false;
	for (std::map<std::string, std::vector<std::string>>::iterator it = content.begin(); it != content.end(); ++it)
	{
		if (found == true)
		{
			currentID = it->first;
			break;
		}
		else if (it->first == currentID)
		{
			found = true;
		}
	}
}

/***********************************************************************************/
/*!
\brief Set the overwriting status of the destructor to true so that saving can be done at the destructor
*/
/***********************************************************************************/
void ConfigFile::SetToOverwrite()
{
	bOverwrite = true;
}

ConfigFile::ConfigFile()
{
}

/***********************************************************************************/
/*!
\brief ConfigFile overloaded constructor

\param configFile
	String path for text file used for edit
*/
/***********************************************************************************/

ConfigFile::ConfigFile(std::string const& configFile)
{
	this->index = 0;
	ConfigFileName = configFile;
	bOverwrite = false;

	// Open .cfg file using ifstream
	std::ifstream file(ConfigFileName.c_str());

	// string to read line of file
	std::string line;
	// string to be pushed into content
	std::string inSection;

	bool push = false;
	std::string pushIndex = "";

	while (std::getline(file, line)) 
	{
		if (!line.length()) continue;

		if (line[0] == '/' && line[1] == '/')
		{
			comments.push_back(line);
			continue;
		}

		for (unsigned int i = 0; i < line.length(); ++i)
		{
			if (line[i] == '#')
			{
				push = false;
			}
		}

		if (push)
		{
			Push(inSection, line);
		}

			
		for (unsigned int i = 0; i < line.length(); ++i)
		{
			if (line[i] == '#')
			{
				push = false;
				inSection = "";
				continue;
			}

			if (line[i] == '[')
			{
				inSection = trim(line.substr(i + 1, line.find(']') - 1));
				pushIndex = inSection;
				push = true;
				continue;
			}
		}
	}	
}

/***********************************************************************************/
/*!
\brief ConfigFile destructor that handles writing back into the text file after destruction

*/
/***********************************************************************************/

ConfigFile::~ConfigFile()
{
	if (!bOverwrite)
		return;

	std::ofstream file(ConfigFileName);

	if (!file.is_open())
	{
		std::cout << "Failed to open " << ConfigFileName << std::endl;
	}
	else
	{
		for (auto it : comments)
		{
			file << it << std::endl;
		}

		file << "" << std::endl;

		int hash = 1;
		for (std::map<std::string, std::vector<std::string>>::iterator it = content.begin(); it != content.end(); ++it)
		{
			file << "#" + std::to_string(hash) << std::endl;
			file << "[" << it->first << "]" << std::endl;
			for (auto ot : it->second)
			{
				file << ot << std::endl;
			}

			file << "" << std::endl;
			++hash;
		}
	}

	file.close();
}
