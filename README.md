# cJSON 的使用

######  简介

cJSON 是一个基于C语言的，可跨平台的解析字典、字符串组的数据结构。

###### 下载地址
      https://github.com/JHiroGuo/cJson



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


```
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"


static void print_cjson_obj_value(const cJSON * obj)
{
    if (!obj) {
        printf("NULL object!\n");
    }

    switch (obj->type) {
        case cJSON_False:
            printf("%s: false\n", obj->string);
            break;
        case cJSON_True:
            printf("%s: true\n", obj->string);
            break;
        case cJSON_NULL:
            printf("%s: cJSON_NULL\n", obj->string);
            break;
        case cJSON_Number:
            printf("%s: %d, %f\n", 
                    obj->string, obj->valueint, obj->valuedouble);
            break;
        case cJSON_String:
            printf("%s: %s\n", 
                    obj->string, obj->valuestring);
            break;
        case cJSON_Array:
            printf("%s: cJSON_Array\n", obj->string);
            break;
        case cJSON_Object:
            printf("%s: cJSON_Object\n", obj->string);
            break;
        default:
            printf("unknown type\n");
    }
}

/* *
 * 字符串转换成json对象
 */
int test_parse_json_string_to_object()
{
#define MY_JSON_STRING    \
    "{\"name\": \"Jack (\\\"Bee\\\") Nimble\", \
        \"format\": {                     \
            \"type\":      \"rect\", \
            \"width\":     1920,     \
            \"interlace\": true, \
            \"height\": 123.1 \
        },                                 \
        \"brother\": [ \
            \"Lea\",                     \
            \"Boo\"                         \
        ]                                 \
    }"

    /* 1. 解析，打印 
     (1) cJSON_Parse解析字符串为json对象
     (2) cJSON_Print将json对象打包成字符串，使用之后需要释放字符串空间
    */
    printf("(1) parse, print\n");
    cJSON * cj_root = cJSON_Parse(MY_JSON_STRING);
    const char * cj_str = cJSON_Print(cj_root);
    printf("%s\n\n", cj_str);
    free(cj_str);

    /* 2. 获取对象 
          (1) 如果获取的对象不存在，cJSON_GetObjectItem返回为空
          (2) cJSON_GetObjectItem只能获得当前对象的元素，不能获取前对象的元素中的对象
              例如 
                   cJSON_GetObjectItem(cj_root,"format");不为空
                   cJSON_GetObjectItem(cj_root,"type");为空
          (3) 判断对象是否存在不使用cJSON_HasObjectItem()，直接判断cJSON_GetObjectItem()返回的指针是否为空
    */
    printf("(2) get object\n");
    cJSON * format = cJSON_GetObjectItem(cj_root, "format");
    cJSON * brother = cJSON_GetObjectItem(cj_root, "brother");
    cJSON * type = cJSON_GetObjectItem(cj_root, "type");
    printf("format [%p]\n", format);
    printf("brother [%p]\n", brother);
    printf("type [%p]\n", type);
    
    /* 3. 获取对象的值 
          (1) 每个对象是由cJSON结构体组成 
     (2) cJSON_Number包括整形和浮点型
     (3) 打印数组对象的值先判断数组长度，再打印。或者使用cJSON_ArrayForEach宏，后一个遍历速度更快
     (4) json使用完之后需要释放其空间
    */

    printf("\n(3) get object value\n");
    print_cjson_obj_value(cJSON_GetObjectItem(cj_root, "name"));
    print_cjson_obj_value(cJSON_GetObjectItem(format, "type"));
    print_cjson_obj_value(cJSON_GetObjectItem(format, "width"));
    print_cjson_obj_value(cJSON_GetObjectItem(format, "interlace"));
    print_cjson_obj_value(cJSON_GetObjectItem(format, "height"));

#if 0
    int size = cJSON_GetArraySize(brother);
    for (int i = 0; i < size; i++) {
        print_cjson_obj_value(cJSON_GetArrayItem(brother, i));
    }
#else
    cJSON * pos = NULL;
    cJSON_ArrayForEach(pos, brother) {
        print_cjson_obj_value(pos);
    }
#endif

    /* *
     * cjson使用完成之后需要释放空间
     */
    cJSON_Delete(cj_root);
    
#undef MY_JSON_STRING
}

/* *
 * json对象转化成字符串
 */
int test_pack_json_object_to_string()
{

    /* 
     (1) 创建json对像，要先使用cJSON_CreateObject()创建一个根对象
     (2) 在使用cJSON_AddItemToObject()添加各种对象到根对象中。也可以直接使用头文件提供的宏来添加对象和值，如下
         cJSON_AddNullToObject(object,name)
        cJSON_AddTrueToObject(object,name) 
        cJSON_AddFalseToObject(object,name)
        cJSON_AddBoolToObject(object,name,b)
        cJSON_AddNumberToObject(object,name,n)
        cJSON_AddStringToObject(object,name,s)
    */

    cJSON * root = NULL;
    cJSON * format = NULL;
    cJSON * brother = NULL;
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "Jack (\"Bee\") Nimble");
    cJSON_AddItemToObject(root, "format", format=cJSON_CreateObject());
    cJSON_AddItemToObject(root, "brother", brother=cJSON_CreateArray());

    /* 普通对象中插入对象和值 */
    {
        cJSON_AddStringToObject(format, "type", "rect");
        cJSON_AddFalseToObject(format, "interlace");
        cJSON_AddNumberToObject(format, "width", 1920);    
        cJSON_AddNumberToObject(format, "height", 1920.11);
        cJSON_AddBoolToObject(format, "flag", 1);
    }

    /* 数组中插入对象和值 */
    {    
        cJSON_AddItemToArray(brother,cJSON_CreateString("Lea"));
        cJSON_AddItemToArray(brother,cJSON_CreateString("Boo"));
    }

    const char * cj_str = cJSON_Print(root);
    printf("%s\n\n", cj_str);
    free(cj_str);

    /* *
     * cjson使用完成之后需要释放空间
     */
    cJSON_Delete(root);
}

void print_cjson_string(const char * describe, const char *cj_str)
{
    printf("%s\n%s\n\n", describe, cj_str);
    free(cj_str);
}

/* *
 * 使用cjson提供的其它接口操作，都在cJSON.h中
 */
int test_cjson_operation()
{
#define MY_JSON_STRING    \
        "{\"name\": \"Lee\"}"

    /* *
     (1) cJSON_PrintUnformatted打印出的json字符串是紧凑的不用换行，适合传输json字符串时使用
     (2) cJSON_PrintBuffered可以事先指定分配一个缓冲区，如果足够大，则可以节省不断重新分配空间的时间；
         如果指定缓冲区过小，则与cJSON_Print和cJSON_PrintUnformatted性能能相同。
     (3) cJSON_PrintPreallocated可以将json字符串填入给定的缓冲区，并指定缓冲区大小。
         成功返回1，失败返回0。除了这个打印json字符串的函数，以上的函数在生成json字符串时若空间不足，
         都会重新分配一次空间。     
     */
    cJSON * root = cJSON_Parse(MY_JSON_STRING);
    print_cjson_string("(1) cJSON_PrintUnformatted",
                 cJSON_PrintUnformatted(root));
    print_cjson_string("(2) cJSON_PrintBuffered",
                 cJSON_PrintBuffered(root, 1, 1));
    print_cjson_string("(3) cJSON_PrintBuffered",
                 cJSON_PrintBuffered(root, 1, 0));
#define BUF_LEN 1024
    char buf[BUF_LEN] = {0};
    if (0 == cJSON_PrintPreallocated(root, buf, BUF_LEN, 0)) {
        printf("(4) cJSON_PrintPreallocated\nfail\n\n");
    } else {
        printf("(4) cJSON_PrintPreallocated\n%s\n\n", buf);
    }
#undef BUF_LEN

    /* *
     (4) cJSON_GetErrorPtr可以获取字符串解析函数cJSON_Parse出错时的字符位置。
     如果解析没有错误，则返回NULL。
     */

    printf("(5) cJSON_GetErrorPtr\n");
    const cJSON * cj_tmp = cJSON_Parse("{\"haha\":12,, \"xixi\":34}");
    printf("[%s]\n\n", cJSON_GetErrorPtr());

    /* *
     (5) cJSON_CreateIntArray, cJSON_CreateFloatArray, cJSON_CreateDoubleArray和cJSON_CreateStringArray
     这4个函数可以直接将数组转换成json数组对象
     */
    int intarray[4] = {0, 1, 2, 3};
    cJSON_AddItemToObject(root, "intarray",cJSON_CreateIntArray(intarray, 4));
    print_cjson_string("(6) cJSON_CreateIntArray",cJSON_Print(root));

    /* *
     (6) cJSON_AddItemToObjectCS中的string参数是一个字符串常量，不需要为string分配空间。
         感觉没什么用。
     */
    const char * con_str = "haha";
    cJSON_AddItemToObjectCS(root, con_str, cJSON_CreateString("go"));
    print_cjson_string("(7) cJSON_AddItemToObjectCS",cJSON_Print(root));

    /* *
     (7) cJSON_AddItemReferenceToObject和cJSON_AddItemReferenceToArray
         将一个json对象引用到另一个json对象上，只拷贝了被引用对象的根节点,并不是复制所有节点到另一个json对象。
     */
     
    cJSON * n_root = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(n_root, "n_intarray", 
                                 cJSON_GetObjectItem(root,"intarray"));
    print_cjson_string("(8) cJSON_AddItemReferenceToObject",cJSON_Print(n_root));

    /* *
     (8) cJSON_DetachItemFromArray, cJSON_DeleteItemFromArray, cJSON_DetachItemFromObject, cJSON_DeleteItemFromObject
         cJSON_DetachItemFromArray是从json数组对象中分离出来第n个节点，cJSON_DeleteItemFromArray是删除json数组对象的第n个节点
         cJSON_DetachItemFromObject是分离任意节点，cJSON_DeleteItemFromObject是删除任意节点
     */
    cJSON_DeleteItemFromArray(cJSON_GetObjectItem(root,"intarray"), 2);
    print_cjson_string("(9) cJSON_DeleteItemFromArray",cJSON_Print(root));    

    /* *
     (9) cJSON_InsertItemInArray 插入节点到数组中
         cJSON_ReplaceItemInArray 替换数组中的第n个节点
         cJSON_ReplaceItemInObject 替换任意节点
     */
    cJSON_InsertItemInArray(cJSON_GetObjectItem(root,"intarray"), 
                             2, cJSON_CreateNumber(2));
      cJSON_ReplaceItemInObject(root, "name", cJSON_CreateString("Bluce"));
       print_cjson_string("(10) cJSON_DeleteItemFromArray",cJSON_Print(root));

       /* *
     (10) cJSON_InsertItemInArray 插入节点到数组中
         cJSON_ReplaceItemInArray 替换数组中的第n个节点
         cJSON_ReplaceItemInObject 替换任意节点
     */
    cJSON_InsertItemInArray(cJSON_GetObjectItem(root,"intarray"), 
                             2, cJSON_CreateNumber(2));
      cJSON_ReplaceItemInObject(root, "name", cJSON_CreateString("Bluce"));
       print_cjson_string("(11) cJSON_DeleteItemFromArray",cJSON_Print(root));    

       /* *
     (11) cJSON_Duplicate recurse为1表示递归复制一个json对象。
     */
       cJSON * cj_duplicate = cJSON_Duplicate(root, 1);
    print_cjson_string("(12) cJSON_Duplicate",cJSON_Print(cj_duplicate));     

       /* *
     (12) cJSON_ParseWithOpts
         该函数是cJSON_Parse的实现，但是它比cJSON_Parse更灵活。
         1.如果cJSON_Parse解析错误，必须调用cJSON_GetErrorPtr找到解析错误位置
         2.cJSON_Parse解析不严谨，如"{\"name\": \"Lee\"} }"这个字符串，后边多了一个大括号，但还是解析成功了。
         3.cJSON_ParseWithOpts的return_parse_end参数可以获取解析失败的位置，
          require_null_terminated参数为1会检查剩余未解析字符串是否是空字符串，
          若不为空字符串，则会释放内存，返回空。
          @所以因该使用cJSON_ParseWithOpts解析json对象
     */
    {
        printf("(13) cJSON_ParseWithOpts\n");
        char * end = NULL;
     cJSON * cj_ParseWithOpts = cJSON_ParseWithOpts("{\"name\": \"Lee\"} }", &end, 1);
     if (cj_ParseWithOpts) {
            printf("%s\n\n", cJSON_Print(cj_ParseWithOpts));
     } else {
            printf("[%s]\n\n", end);
     }
    }
    
    {
        printf("(14) cJSON_Parse\n");
        char * end = NULL;
     cJSON * cj_ParseWithOpts = cJSON_Parse("{\"name\": \"Lee\"} }");
     if (cj_ParseWithOpts) {
            printf("%s\n\n", cJSON_Print(cj_ParseWithOpts));
     } else {
            printf("[%s]\n\n", end);
     }
    }

    /* *
     (12) cJSON_SetIntValue和cJSON_SetNumberValue相同，
          既可以设置整形值，也可以设置double类型值。
     */
    printf("(15) cJSON_SetIntValue\n");
    cJSON * int_cj = cJSON_CreateNumber(1);
    printf("%s\n", cJSON_Print(int_cj));
    cJSON_SetIntValue(int_cj, 1.1);
    printf("%s\n\n", cJSON_Print(int_cj));

    cJSON_Delete(root);
}


int main()
{
    //将json字符串解析成json对象
    printf("<< test_parse_json_string_to_object >>\n");
    test_parse_json_string_to_object();

    //将生成json对象，并将json对象组成字符串
    printf("\n<< test_pack_json_object_to_string >>\n");
    test_pack_json_object_to_string();

    //其它一些json接口测试
    printf("<< test_cjson_operation >>\n");
    test_cjson_operation();
    return 0;
}
```
