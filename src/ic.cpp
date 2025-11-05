
#include "../head/ic.h"

std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (std::string::npos == first) return "";
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

void Ic::load(const std::string& filename)
{
	std::string fname = filename;

	if (fname.find(FORMAT) == std::string::npos)
		fname += FORMAT;

	std::ifstream in(fname);

	if (!in.is_open())
		throw FileNotExistence();

	std::string line;

	while (getline(in, line))
		parse(line);

	in.close();
	
}

void Ic::parse(const std::string& line)
{

	std::string cline = trim(line.substr(0, line.find(COMMENT_PREFIX)));

	if (cline.empty())
		return;

	if (cline == MAIN_SECTION)
	{
		section = MAIN_SECTION_TITLE;
		return;
	}

	if (cline[0] == OPEN_SECTION && cline[cline.length() - 1] == CLOSE_SECTION)
	{
		section = trim(cline.substr(1, cline.length() - 2));
		return;
	}

	size_t del_pos = cline.find(DELIMITER);

	if (del_pos == std::string::npos)
		return;

	std::string	key = trim(cline.substr(0, del_pos));
	std::string val = trim(cline.substr(del_pos + 1));
	val = val.find(' ') != std::string::npos ? val.substr(0, val.find(' ')) : val;

	if (!key.empty())
		data[section][key] = val;
	
}

bool Ic::has(const std::string& target_section) const
{
	return data.find(target_section) != data.end();
}

bool Ic::has(const std::string& target_section, const std::string& key) const
{
	auto it = data.find(target_section);
	return it != data.end() && it->second.find(key) != it->second.end();
}

std::string Ic::getVal(const std::string& target, const std::string& key) const
{
	auto it = data.find(target);
	return it != data.end() ? ( it->second.find(key) != it->second.end() ? it->second.at(key) : "" ) : "";
}

std::string Ic::getVal(const std::string& key) const
{
	return getVal(MAIN_SECTION_TITLE, key);
}


int Ic::as_int(const std::string& target, const std::string& key) const
{
	return has(target, key) ? std::stoi(getVal(target, key)) : throw KeyError();
}

int Ic::as_int(const std::string& key) const
{
	return as_int(MAIN_SECTION_TITLE, key);
}


double Ic::as_double(const std::string& target, const std::string& key) const
{
	return has(target, key) ? std::stod(getVal(target, key)) : throw KeyError();
}

double Ic::as_double(const std::string& key) const
{
	return as_double(MAIN_SECTION_TITLE, key);
}

bool Ic::as_bool(const std::string& target, const std::string& key) const
{
	if (!has(target, key))
		throw KeyError();
		
	const std::string val = getVal(target, key);

	return val == "true" ? true : 
		val == "false" ? false : throw BoolError();
}

bool Ic::as_bool(const std::string& key) const
{
	return as_bool(MAIN_SECTION_TITLE, key);
}
