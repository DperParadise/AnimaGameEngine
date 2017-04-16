#include "Config.h"
#include "libraries/parson/parson.h"

Config::Config(JSON_Value *value, JSON_Object *object) : root_value(value), root_object(object){}

Config::~Config()
{
	json_value_free(root_value);
}

Config* Config::LoadConfig(const std::string &file)
{
	Config *ret = nullptr;

	const char *path = file.c_str();
	static JSON_Value *root_value = json_parse_file(path);
	JSON_Object *root_object = json_value_get_object(root_value);

	if(root_value != NULL && root_object != NULL)
		ret = new Config(root_value, root_object);

	return ret;
}

const bool Config::GetBool(const char *module, const char *property) const
{
	std::string json_field = std::string(module) + "." + std::string(property);
	bool ret = (bool)json_object_dotget_boolean(root_object, json_field.c_str());
	return ret;
}

const char *Config::GetString(const char *module, const char *property) const
{
	std::string json_field = std::string(module) + "." + std::string(property);
	const char *ret = json_object_dotget_string(root_object, json_field.c_str());
	return ret;
}

const int Config::GetInt(const char *module, const char *property) const
{
	std::string json_field = std::string(module) + "." + std::string(property);
	int ret = (int)json_object_dotget_number(root_object, json_field.c_str());
	return ret;	
}

const double Config::GetDouble(const char *module, const char *property) const
{
	std::string json_field = std::string(module) + "." + std::string(property);
	int ret = json_object_dotget_number(root_object, json_field.c_str());
	return ret;
}

const float Config::GetFloat(const char *module, const char *property) const
{
	std::string json_field = std::string(module) + "." + std::string(property);
	int ret = (float)json_object_dotget_number(root_object, json_field.c_str());
	return ret;
}