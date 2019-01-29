// Copyright (c) 2015, Beijing Tiertime 
// All rights reserved.
//
// Filename£∫filename.h
// Descriptions£∫This is the file to parsing json 
//----------------------------------------------------
// Version£∫1.1.1
// Author£∫LIBO
// Finish Date£∫2015/04/20 (yyyy/mm/dd)
// Descriptions: 
//------------------------------------------------- 

#ifndef cJSONx__h
#define cJSONx__h

#include "cJSON.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define cJSON_ForEach(it, array)	for(cJSON *it=array->child; it; it=it->next)
#define cJSON_DeletePtr(object)		{ cJSON_Delete(object),object = NULL; }

extern int		cJSON_NameToInt(cJSON * object);
extern double	cJSON_ValueToDouble(cJSON * object);

#define  cJSON_GetSimpleType(object)		(object->type&(~cJSON_IsReference))

extern bool		cJSON_TypeIsInt(cJSON * object);


#ifdef __cplusplus
}

#include <string>

#ifdef WIN32
#else
#include <iostream>
#endif
using namespace std;
#include <cstdlib>

extern std::string	cJSON_ValueToString(cJSON * object);

class cJSONPrintStringPtr
{
public:
	cJSONPrintStringPtr(cJSONPrintStringPtr && robj)
		:	string_(std::move(robj)),
			json_char_array_(std::move(robj.json_char_array_))
	{
		robj.json_char_array_ = NULL;
	}
	cJSONPrintStringPtr(cJSON * object)
	{
		json_char_array_ = object ? cJSON_Print(object) : NULL;
		if (json_char_array_)
			string_ = json_char_array_;
	}
	explicit cJSONPrintStringPtr(char *&& json_char_array)
		:	string_(json_char_array), 
			json_char_array_(std::move(json_char_array))
	{}
	~cJSONPrintStringPtr(){
		if (json_char_array_)
			free(json_char_array_);
	}
	cJSONPrintStringPtr& operator =(cJSON * object)
	{
		if (reinterpret_cast<cJSONPrintStringPtr*>(object) == this)
			return *this;
		json_char_array_ = object ? cJSON_Print(object) : NULL;
		if (json_char_array_)
			string_ = json_char_array_;
		return *this;
	}


	std::string &	ToString() { return string_; }
	operator		std::string() { return string_; }

public:
	std::string		string_;
	char *			json_char_array_;
};

#endif

#endif
