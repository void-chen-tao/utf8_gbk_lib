#include <stdio.h>
#include <string.h>
#include "iconv.h"


#define OUTPUTBUFFERSIZE	1024
static char OutPutBuffer[OUTPUTBUFFERSIZE];
static char SourceBuffer[OUTPUTBUFFERSIZE];

int main(void)
{
	const char* source_code = u8"ÖÐ¹ú1314";
	size_t srclen = strlen(source_code);
	size_t outlen = sizeof(OutPutBuffer);
	char* src_copy = source_code;
	char* outbuf_copy = OutPutBuffer;

	iconv_t cd = CreateConverter("gbk", "utf-8");
	size_t retval = RunConverter(cd, &src_copy, &srclen, &outbuf_copy, &outlen);
	DestroyConverter(cd);


	return 0;
}