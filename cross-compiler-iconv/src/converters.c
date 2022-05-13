#include <string.h>
#include "converters.h"


//	OLD Version:
bool LoopReset(conv_t cd, char** outbuf, size_t* outbytesleft)
{
	/*  ��������״̬  */
	bool result = false;
	if (outbuf == NULL || *outbuf == '\0')
	{
		//	�ṹ������
		memset(cd, '\0', sizeof(struct conv_struct));
		return result = true;
	}
	else
	{
		//if (cd->ifuncs.xxx_flushwc)
		//{
		//	//	�������������뻺����������ִ�����뻺����������
		//	//	��Ҫ���ͷŶ�̬������ڴ�ռ�
		//	if (cd->ifuncs.xxx_flushwc(cd, *outbuf) == 0)
		//		cd->istate = 0;
		//	else
		//	{
		//		cd->istate = -1;
		//		return result = 1;
		//	}
		//}
		//if (cd->ofuncs.xxx_reset)
		//{
		//	//	�������������������������ִ�����������������
		//	//	��Ҫ���ͷŶ�̬������ڴ�ռ�
		//	if (cd->ofuncs.xxx_reset(cd, *outbuf, *outbytesleft) == 0)
		//		cd->ostate = 0;
		//	else
		//	{
		//		cd->ostate = -1;
		//		return result = 1;
		//	}
		//}
		//return result = 0;
		return result;
	}
}

////	NEW Version
//bool LoopReset(conv_t cd)
//{
//	/*  ��������״̬  */
//	bool result = false;
//	if (&cd->outbuf == NULL || cd->outbuf == '\0')
//	{
//		//	�ṹ������
//		memset(cd, '\0', sizeof(struct conv_struct));
//		return result = true;
//	}
//	else
//	{
//		//if (cd->ifuncs.xxx_flushwc)
//		//{
//		//	//	�������������뻺����������ִ�����뻺����������
//		//	//	��Ҫ���ͷŶ�̬������ڴ�ռ�
//		//	if (cd->ifuncs.xxx_flushwc(cd, *outbuf) == 0)
//		//		cd->istate = 0;
//		//	else
//		//	{
//		//		cd->istate = -1;
//		//		return result = 1;
//		//	}
//		//}
//		//if (cd->ofuncs.xxx_reset)
//		//{
//		//	//	�������������������������ִ�����������������
//		//	//	��Ҫ���ͷŶ�̬������ڴ�ռ�
//		//	if (cd->ofuncs.xxx_reset(cd, *outbuf, *outbytesleft) == 0)
//		//		cd->ostate = 0;
//		//	else
//		//	{
//		//		cd->ostate = -1;
//		//		return result = 1;
//		//	}
//		//}
//		//return result = 0;
//		return result;
//	}
//}


bool LoopConvert(conv_t cd, const char** inbuf, size_t* inbytesleft, char** outbuf, size_t* outbytesleft)
{
	/*
	*   ����������ݡ�����Ϊ�����漰����ָ����ƶ�
	*
	*   cd      ����  ת���ṹ��Ŀ��ƾ��
	*   result  ����  ��������״̬��0--no error��1--error
	*   inptr   ����  inbuf
	*   inleft  ����  inbytesleft
	*   outptr  ����  outbuf
	*   ouleft  ����  outbuf
	*/
	bool result = true;
	const unsigned char* inptr = (const unsigned char*)*inbuf;
	size_t inleft = *inbytesleft;
	unsigned char* outptr = (unsigned char*)*outbuf;
	size_t outleft = *outbytesleft;

	if (cd->istate == 1 && cd->ostate == 1)
	{
		//	ת������׼������,һֱִ��ת��������ֱ��û����Ҫת�����ַ����򴥷����쳣
		while (inleft > 0)
		{
			/*  do utf8_mbtowc  */
			ucs4_t wc;					//	temporary store wide char(unicode)
			int incount;				//	multibytes read count
			int outcount;				//	multibytes write count; ascii return 1��gbk return 2

			incount = cd->ifuncs.xxx_mbtowc(cd, &wc, inptr, inleft);
			if (incount < 0)
			{
				//	����ת��ʧ��
				cd->error_info = incount;
				cd->istate = -1;
				result = false;
				return result;
			}
			else
			{
				//	����ת���ɹ�
				cd->istate = 0;
				if (outleft <= 0)
				{
					//	������治��
					cd->error_info = RET_NOENOUGH;
					cd->ostate = -1;
					result = false;
					return result;
				}
				else
				{
					//	����������
					outcount = cd->ofuncs.xxx_wctomb(cd, outptr, wc, outleft);
					if (outcount <= 0)
					{
						cd->error_info = outcount;
						cd->ostate = -1;
						result = false;
						return result;
					}
					else
					{
						if (!(outcount <= (int)outleft))
						{
							//	�������̫С
							cd->error_info = RET_TOOSMALL;
							cd->ostate = -1;
							result = false;
							return result;
						}
						else
						{
							//	һ����������������
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
		*outptr = '\0';
		/**inbuf = (const char*)inptr;
		*inbytesleft = inleft;
		*outbuf = (char*)outptr;*/
		*outbytesleft = outleft;
		return result;
	}
	else
	{
		result = false;
		return result;
	}
}