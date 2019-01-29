#include "XcJSON.h"
#include "math.h"
#include "float.h"
#include "limits.h"

/*extern*/ int		cJSON_NameToInt(cJSON * object)
{
	//if(NULL == object->string || '\0' == object->string)
	if (!object->string)
		return 0;
	return atoi(object->string);
}

/*extern*/ double	cJSON_ValueToDouble(cJSON * object)
{
	switch (cJSON_GetSimpleType(object))
	{
	case cJSON_Number:
	case cJSON_True:
	case cJSON_False:
		return cJSON_Number != object->type ? (cJSON_True == object->type ? 1 : 0) : object->valuedouble;
		break;
	case cJSON_String:
		return  strtod(object->valuestring, NULL);
		break;
	case cJSON_Array:
	case cJSON_Object:
	case cJSON_NULL:
	default:
		return 0;
	}
}



/*extern*/ bool		cJSON_TypeIsInt(cJSON * object)
{
	double d = object->valuedouble;
	return cJSON_Number == cJSON_GetSimpleType(object) 
		&& (fabs(((double)object->valueint) - d) <= DBL_EPSILON && d <= INT_MAX && d >= INT_MIN);
}

#include <sstream>
/*extern*/ std::string	cJSON_ValueToString(cJSON * object)
{
    if (NULL == object)
		return std::string();

	switch (cJSON_GetSimpleType(object))
	{
	case cJSON_Number:
		if (cJSON_TypeIsInt(object))
        {
            std::string s;
            std::stringstream ss(s);
            ss << object->valueint;
            return ss.str();
            //return std::to_string(object->valueint);
        }
		else
        {
            std::string s;
            std::stringstream ss(s);
            ss << object->valuedouble;
            return ss.str();
            //return std::to_string(object->valuedouble);
        }
	case cJSON_True:
	case cJSON_False:
    {
        std::string s;
        std::stringstream ss(s);
        ss << object->valueint;
        return ss.str();
        //return std::to_string(object->valueint);
    }
	case cJSON_String:
		return std::string(object->valuestring);
	case cJSON_Array:
	case cJSON_Object:
		return std::string(cJSONPrintStringPtr(object));
	case cJSON_NULL:
	default:
		break;
	}
	return std::string();
}
