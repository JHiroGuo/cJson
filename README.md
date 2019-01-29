# cJSON 的使用

######  简介

cJSON 是一个基于C语言的，可跨平台的解析字典、字符串组的数据结构。

###### 下载地址



在cJSON中，json对象可以是json、字符串、数组等。

######  结构 

```C
/* cJSON Types: */
#define cJSON_False 0
#define cJSON_True 1
#define cJSON_NULL 2
#define cJSON_Number 3
#define cJSON_String 4
#define cJSON_Array 5
#define cJSON_Object 6
	
#define cJSON_IsReference 256

/* The cJSON structure: */
typedef struct cJSON {
	struct cJSON *next,*prev;	/* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
	struct cJSON *child;		/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

	int type;					/* The type of the item, as above. */

	char *valuestring;			/* The item's string, if type==cJSON_String */
	int valueint;				/* The item's number, if type==cJSON_Number */
	double valuedouble;			/* The item's number, if type==cJSON_Number */

	char *string;				/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
} cJSON;
```



###### 解析Json

```C

/* Supply a block of JSON, and this returns a cJSON object you can interrogate. Call cJSON_Delete when finished. */
extern cJSON *cJSON_Parse(const char *value);
/* Render a cJSON entity to text for transfer/storage. Free the char* when finished. */
extern char  *cJSON_Print(cJSON *item);
/* Render a cJSON entity to text for transfer/storage without any formatting. Free the char* when finished. */
extern char  *cJSON_PrintUnformatted(cJSON *item);
/* Delete a cJSON entity and all subentities. */
extern void   cJSON_Delete(cJSON *c);

/* Returns the number of items in an array (or object). */
extern int	  cJSON_GetArraySize(cJSON *array);
/* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
extern cJSON *cJSON_GetArrayItem(cJSON *array,int item);
/* Get item "string" from object. Case insensitive. */
extern cJSON *cJSON_GetObjectItem(cJSON *object,const char *string);

/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds. */
extern const char *cJSON_GetErrorPtr(void);
```



###### 解析Demo

```c++
{
    "semantic": {
        "slots":    {
            "name": "张三"
        }
    },
    "rc":   0,
    "operation":    "CALL",
    "service":  "telephone",
    "text": "打电话给张三"
}
```

```c
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
int main()
{
    char * jsonStr = "{\"semantic\":{\"slots\":{\"name\":\"张三\"}}, \"rc\":0, \"operation\":\"CALL\", \"service\":\"telephone\", \"text\":\"打电话给张三\"}";
    cJSON * root = NULL;
    cJSON * item = NULL;//cjson对象

    root = cJSON_Parse(jsonStr);     
    if (!root) 
    {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
    else
    {
        printf("%s\n", "有格式的方式打印Json:");           
        printf("%s\n\n", cJSON_Print(root));
        printf("%s\n", "无格式方式打印json：");
        printf("%s\n\n", cJSON_PrintUnformatted(root));

        printf("%s\n", "一步一步的获取name 键值对:");
        printf("%s\n", "获取semantic下的cjson对象:");
        item = cJSON_GetObjectItem(root, "semantic");
        printf("%s\n", cJSON_Print(item));
        printf("%s\n", "获取slots下的cjson对象");
        item = cJSON_GetObjectItem(item, "slots");
        printf("%s\n", cJSON_Print(item));
        printf("%s\n", "获取name下的cjson对象");
        item = cJSON_GetObjectItem(item, "name");
        printf("%s\n", cJSON_Print(item));

        printf("%s:", item->string);   //打印Key
        printf("%s\n", item->valuestring);//打印Value     
    }
    return 0;    
}
```



###### 构造函数

```C
/* These calls create a cJSON item of the appropriate type. */
extern cJSON *cJSON_CreateNull(void);
extern cJSON *cJSON_CreateTrue(void);
extern cJSON *cJSON_CreateFalse(void);
extern cJSON *cJSON_CreateBool(int b);
extern cJSON *cJSON_CreateNumber(double num);
extern cJSON *cJSON_CreateString(const char *string);
extern cJSON *cJSON_CreateArray(void);
extern cJSON *cJSON_CreateObject(void);

/* These utilities create an Array of count items. */
extern cJSON *cJSON_CreateIntArray(const int *numbers,int count);
extern cJSON *cJSON_CreateFloatArray(const float *numbers,int count);
extern cJSON *cJSON_CreateDoubleArray(const double *numbers,int count);
extern cJSON *cJSON_CreateStringArray(const char **strings,int count);

```

###### 解析Demo

```C
"semantic": {
        "slots":    {
            "name": "张三"
        }
    },
    "rc":   0,
    "operation":    "CALL",
    "service":  "telephone",
    "text": "打电话给张三"
}


#include <stdio.h>
#include "cJSON.h"

int main()
{
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();
    cJSON * next =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "rc", cJSON_CreateNumber(0));//根节点下添加
    cJSON_AddItemToObject(root, "operation", cJSON_CreateString("CALL"));
    cJSON_AddItemToObject(root, "service", cJSON_CreateString("telephone"));
    cJSON_AddItemToObject(root, "text", cJSON_CreateString("打电话给张三"));
    cJSON_AddItemToObject(root, "semantic", item);//root节点下添加semantic节点
    cJSON_AddItemToObject(item, "slots", next);//semantic节点下添加item节点
    cJSON_AddItemToObject(next, "name", cJSON_CreateString("张三"));//添加name节点

    printf("%s\n", cJSON_Print(root));

    return 0;
}
```



###### 其他常用函数

```c
/* Update array items. */
extern void cJSON_ReplaceItemInArray(cJSON *array,int which,cJSON *newitem);
extern void cJSON_ReplaceItemInObject(cJSON *object,const char *string,cJSON *newitem);

/* Duplicate a cJSON item */
extern cJSON *cJSON_Duplicate(cJSON *item,int recurse);
/* Duplicate will create a new, identical cJSON item to the one you pass, in new memory that will
need to be released. With recurse!=0, it will duplicate any children connected to the item.
The item->next and ->prev pointers are always zero on return from Duplicate. */

/* Delete a cJSON entity and all subentities. */
extern void   cJSON_Delete(cJSON *c);
```

PS:

cJSON 对象是指针类型 ，所以在创建之后一定要调用 cJSON_Delete 删除函数。