/*
** Purpose: Define enumeration string library
**
** Notes:
**   1. The enumeration macro design is from 
**      https://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c/202511 
**
** References:
**   1. OpenSatKit Object-based Application Developer's Guide.
**   2. cFS Application Developer's Guide.
**
**   Written by David McComas, licensed under the Apache License, Version 2.0
**   (the "License"); you may not use this file except in compliance with the
**   License. You may obtain a copy of the License at
**
**      http://www.apache.org/licenses/LICENSE-2.0
**
**   Unless required by applicable law or agreed to in writing, software
**   distributed under the License is distributed on an "AS IS" BASIS,
**   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**   See the License for the specific language governing permissions and
**   limitations under the License.
*/
#ifndef _ini_lib_
#define _ini_lib_



#define INILIB_TYPE_INT  "uint32"
#define INILIB_TYPE_STR  "char*"


/******************************************************************************
** Initialization Library 
**
*/

typedef const char * (*INILIB_GetConfigFuncPtr_t) (int ConfigParam);

typedef struct
{

   int Start;
   int End;
   
   INILIB_GetConfigFuncPtr_t  GetStr;
   INILIB_GetConfigFuncPtr_t  GetType;

} INILIB_CfgEnum_t;


/******************************************************************************
** Enumeration Macros
**
** These maintain consistency init file enumerations, string names and 
** structure names 
*/


/* expansion macro for enum value definition */
#define STRUCT_VALUE(name,type) type name;

/* expansion macro for enum value definition */
#define ENUM_VALUE(name,type) name,

/* expansion macro for enum to string conversion */
#define ENUM_CASE(name,type) case name: return #name;

/* expansion macro for enum to type string conversion */
#define ENUM_TYPE_CASE(name,type) case name: return #type;

/* expansion macro for string to enum conversion */
#define ENUM_STRCMP(name,type) if (!strcmp(str,#name)) return name;

/* declare the access function and define enum values */
#define DECLARE_ENUM(TypeName,ENUM_DEF) \
  typedef enum { \
    CFG_ENUM_START = 0, \
    ENUM_DEF(ENUM_VALUE) \
    CFG_ENUM_END \
  } INITBL_##TypeName##Enum; \
  typedef struct { \
    ENUM_DEF(STRUCT_VALUE) \
  } INITBL_##TypeName##Struct; \
  
/* 
** Define the access function names */
/* - Get##TypeName##Str() & Get##TypeName##Type use int parameters instead of
**   INITBL_##TypeName##Enum to prevent compiler warnings with callback functions 
*/
#define DEFINE_ENUM(TypeName,ENUM_DEF) \
  static const char *Get##TypeName##Str(int value); \
  static const char *Get##TypeName##Type(int value); \
  static const char *Get##TypeName##Str(int value) \
  { \
    switch(value) \
    { \
      ENUM_DEF(ENUM_CASE) \
      default: return ""; /* handle input error */ \
    } \
  } \
  static const char *Get##TypeName##Type(int value) \
  { \
    switch(value) \
    { \
      ENUM_DEF(ENUM_TYPE_CASE) \
      default: return ""; /* handle input error */ \
    } \
  } \
  static INILIB_CfgEnum_t IniCfgEnum = { CFG_ENUM_START, CFG_ENUM_END, Get##TypeName##Str, Get##TypeName##Type }; \
  
/*
** Remove this unused function to avoid compiler warnings
**
**  static INITBL_##TypeName##Enum Get##TypeName##Val(const char *string); \
**  static INITBL_##TypeName##Enum Get##TypeName##Val(const char *str) \
**  { \
**    ENUM_DEF(ENUM_STRCMP) \
**    return (INITBL_##TypeName##Enum)0;  Comment: handle input error  \
**  } \
*/

#endif /* _ini_lib_ */
