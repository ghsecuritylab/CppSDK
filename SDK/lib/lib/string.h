/*
 * lib/string.h
 */

#ifndef LIB_LIB_STRING_H_
#define LIB_LIB_STRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include <include/global.h>

namespace GI
{
	class String
	{
	public:
		String();
		String(char *String);
		String(GI::String *String);
		~String();
		bool equal(char *string);
		bool equal(GI::String *string);
		void toUper();
		void toLower();
		void removeNewLine(GI::String *string);
		void set(char* string);
		void set(GI::String *string);
		void setF(const char *pcString, ...);
		void setF(GI::String *string, ...);
		void append(GI::String *string);
		void append(char* string);
		void append(char Char);
		void appendF(const char *pcString, ...);
		void appendF(GI::String *string, ...);
		void insert(GI::String* string, unsigned int location);
		void insert(char *string, unsigned int location);
		void clone(GI::String* string);
		void clear();
		char *subString(unsigned int position, unsigned int len);
		char *subString(unsigned int position);

		char *buff;
		unsigned int length;
		/*
		 * Private variables.
		 */
		unsigned int modifyed;
		int error;
		//bool initialized;
	};
}

namespace GI
{
	class StringArray
	{
	public:
		StringArray();
		~StringArray();
		SysErr add(GI::String *str);
		SysErr add(char *str);
		SysErr insert(GI::String *str, unsigned int position);
		SysErr insert(char *str, unsigned int position);
		SysErr remove(unsigned int position);
		SysErr toCharStrArray(char ***charArray, unsigned int *itemsCount);
		SysErr freeCharStrArray(char **charArray, unsigned int itemsCount);
		GI::String ** array;
		unsigned int itemsCount;
	};
}

namespace GI
{
	class StringCharArray
	{
	public:
		StringCharArray();
		~StringCharArray();
		StringCharArray(GI::StringArray *strArray);
		StringCharArray(GI::StringArray *strArray, unsigned int offset);
		SysErr convert(GI::StringArray *strArray);
		SysErr convert(GI::StringArray *strArray, unsigned int offset);
		char ** array;
		unsigned int itemsCount;
	};
}
/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "string.cpp"
#endif
/*#####################################################*/

#endif /* LIB_LIB_STRING_H_ */
