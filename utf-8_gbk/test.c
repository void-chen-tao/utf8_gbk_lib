#include <stdio.h>
#include <string.h>
#include "iconv.h"


int main(void)
{
	const char* source_code = u8"ÖÐ¹ú1314";
	iconv_t cd = CreateConverter("gbk", "utf-8");
	size_t retval = RunConverter(cd, source_code);
	DestroyConverter(cd);


	return 0;
}