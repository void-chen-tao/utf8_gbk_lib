#include <string.h>
#include "converters.h"


size_t LoopReset(conv_t cd, char** outbuf, size_t* outbytesleft)
{
	/*  函数运行状态  */
	state_t result = 1;
	if (outbuf == NULL || *outbuf == NULL)
	{
		//	表明没有输入也没有输出，所以将输入输出函数的标志位设置为空
		memset(&cd->istate, '\0', sizeof(state_t));
		memset(&cd->ostate, '\0', sizeof(state_t));
		return result = 0;
	}
	else
	{
		if (cd->ifuncs.xxx_flushwc)
		{
			//	如果存在清除输入缓冲区函数，执行输入缓冲区清理函数
			//	主要是释放动态申请的内存空间
			if (cd->ifuncs.xxx_flushwc(cd, *outbuf) == 0)
				cd->istate = 0;
			else
			{
				cd->istate = -1;
				return result = 1;
			}
		}
		if (cd->ofuncs.xxx_reset)
		{
			//	如果存在清除输出缓冲区函数，执行输出缓冲区清理函数
			//	主要是释放动态申请的内存空间
			if (cd->ofuncs.xxx_reset(cd, *outbuf, *outbytesleft) == 0)
				cd->ostate = 0;
			else
			{
				cd->ostate = -1;
				return result = 1;
			}
		}
		return result = 0;
	}
}


size_t LoopConvert(conv_t cd, const char** inbuf, size_t* inbytesleft, char** outbuf, size_t* outbytesleft)
{
	/*
	*   输入参数备份——因为其中涉及到了指针的移动
	*
	*   cd      ——  转换结构体的控制句柄
	*   result  ——  函数运行状态；0--no error、1--error
	*   inptr   ——  inbuf
	*   inleft  ——  inbytesleft
	*   outptr  ——  outbuf
	*   ouleft  ——  outbuf
	*/
	size_t result = 0;
	const unsigned char* inptr = (const unsigned char*)*inbuf;
	size_t inleft = *inbytesleft;
	unsigned char* outptr = (unsigned char*)*outbuf;
	size_t outleft = *outbytesleft;

	if (cd->istate == 1 && cd->ostate == 1)
	{
		//	转换函数准备就绪,一直执行转换函数，直到没有需要转换的字符串或触发了异常
		while (inleft > 0)
		{
			/*  do utf8_mbtowc  */
			ucs4_t wc;					//	temporary store wide char(unicode)
			int incount;				//	multibytes read count
			int outcount;				//	multibytes write count; ascii return 1、gbk return 2

			incount = cd->ifuncs.xxx_mbtowc(cd, &wc, inptr, inleft);
			if (incount < 0)
			{
				//	解码转换失败
				cd->error_info = incount;
				cd->istate = -1;
				return result = -1;
			}
			else
			{
				//	解码转换成功
				cd->istate = 0;
				if (outleft <= 0)
				{
					//	输出缓存不足
					cd->error_info = RET_NOENOUGH;
					cd->ostate = -1;
					return result = -1;
				}
				else
				{
					//	输出缓存充足
					outcount = cd->ofuncs.xxx_wctomb(cd, outptr, wc, outleft);
					if (outcount <= 0)
					{
						cd->error_info = outcount;
						cd->ostate = -1;
						return result = -1;
					}
					else
					{
						if (!(outcount <= (int)outleft))
						{
							//	输出缓存太小
							cd->error_info = RET_TOOSMALL;
							cd->ostate = -1;
							return result = -1;
						}
						else
						{
							//	一切正常，更新数据
							outptr += outcount;
							outleft -= outcount;
							if (!(incount <= (int)inleft))
								return RET_ILUNI;
							inptr += incount;
							inleft -= incount;
						}
					}
				}
			}
		}
		*inbuf = (const char*)inptr;
		*inbytesleft = inleft;
		*outbuf = (char*)outptr;
		*outbytesleft = outleft;
		return result;
	}
	else
		return result = 1;
}