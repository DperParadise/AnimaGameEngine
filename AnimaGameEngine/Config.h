#ifndef __CONGIG_H__
#define __CONFIG_H__

#include "parson.h"
#include <string>

class Config
{
public:
	
	~Config();

	static Config* LoadConfig(const std::string &file);
	const bool GetBool(const char *module, const char *property) const;
	const char* GetString(const char *module, const char *property) const;
	const int GetInt(const char *module, const char *property) const;
	const double GetDouble(const char *module, const char *property) const;
	const float GetFloat(const char *module, const char *property) const;

private:

	Config(JSON_Value *value, JSON_Object *object);
	JSON_Value *root_value = nullptr;
	JSON_Object *root_object = nullptr;
	const char *path = nullptr;
};
#endif
