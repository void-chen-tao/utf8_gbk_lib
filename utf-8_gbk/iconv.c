#include <ctype.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include "converters.h"
#include "iconv.h"

#define gbk_mbtowc			(void*)0
#define utf8_wctomb			(void*)0
#define CODESCHEMELIMIT		20


static const char* TableOfCodeScheme[] = {
	"UTF-8",
	"GBK",
};

static const void* TableOfMBToWC[] = {
	utf8_mbtowc,
	gbk_mbtowc,
};

static const void* TableOfWCToMB[] = {
	utf8_wctomb,
	gbk_wctomb,
};

static  bool UpperCodeScheme(const char* inbuf, char* outbuf)
{
	if (!inbuf || !outbuf)
		return false;

	int i;
	for (i = 0; inbuf[i] != '\0' && i < CODESCHEMELIMIT - 1; ++i)
	{
		if (isalpha(inbuf[i]))
			outbuf[i] = toupper(inbuf[i]);
		else
			outbuf[i] = inbuf[i];
	}
	outbuf[i] = '\0';

	return true;
}

static int GetCodeSchemeIndex(const char* inbuf)
{
	int index = -1;
	for (int i = 0; i < sizeof(TableOfCodeScheme) / sizeof(char*); ++i)
	{
		if (0 == strcmp(inbuf, TableOfCodeScheme[i]))
			return index = i;
	}
	return index;
}

static int GetCharactersNumber(iconv_t cd, const unsigned char* str)
{
	return utf8_mbRead(cd, str);
}

iconv_t CreateConverter(const char* tocode, const char* fromcode)
{
	if (!tocode || !fromcode)
		return NULL;

	char tcbuf[CODESCHEMELIMIT];
	char fcbuf[CODESCHEMELIMIT];
	if (!UpperCodeScheme(fromcode, fcbuf) || !UpperCodeScheme(tocode, tcbuf))
		return NULL;

	int fc_index, tc_index;
	tc_index = GetCodeSchemeIndex(tcbuf);
	fc_index = GetCodeSchemeIndex(fcbuf);
	if (tc_index == -1 || fc_index == -1)
		return NULL;

	conv_t cd = malloc(sizeof(struct conv_struct));
	if (!cd)
		return NULL;

	memset(cd, '\0', sizeof(struct conv_struct));
	cd->lfuncs.loop_convert = &LoopConvert;
	cd->lfuncs.loop_reset = &LoopReset;
	cd->lfstate = 1;	//	ready and waitting be calling
	cd->ifuncs.xxx_mbtowc = (int (*) (conv_t conv, ucs4_t * pwc, unsigned char const* s, size_t n))TableOfMBToWC[fc_index];
	cd->ifuncs.xxx_flushwc = NULL;
	cd->istate = 1;		//	ready and waitting be calling
	cd->ofuncs.xxx_wctomb = (int (*) (conv_t conv, unsigned char* r, ucs4_t wc, size_t n))TableOfWCToMB[tc_index];
	cd->ofuncs.xxx_reset = NULL;
	cd->ostate = 1;

	return (iconv_t)cd;
}


bool RunConverter(iconv_t icd, const unsigned char* inbuf)
{
	if (!icd)
		return false;

	conv_t cd = (conv_t)icd;
	if (inbuf == NULL)
		return cd->lfuncs.loop_reset(cd, &cd->outbuf, &cd->outbytesleft);
	else
	{
		int num = GetCharactersNumber(cd, inbuf);
		if (num <= 0)
			return cd->lfuncs.loop_reset(cd, &cd->outbuf, &cd->outbytesleft);
		cd->outbytesleft = (size_t)num * (size_t)2;
		cd->outbuf = (char*)malloc((size_t)cd->outbytesleft * sizeof(char));

		size_t inbuflen = strlen(inbuf);
		return cd->lfuncs.loop_convert(cd, &inbuf, &inbuflen, &cd->outbuf, &cd->outbytesleft);
	}
}

bool DestroyConverter(iconv_t icd)
{
	if (!icd)
		return false;

	conv_t cd = (conv_t)icd;
	free(cd->outbuf);
	free(cd);

	return true;
}