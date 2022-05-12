#ifndef __ICONV_H__
#define __ICONV_H__

typedef void* iconv_t;
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif
	/*
	*	����tocode��fromcode����������ת��ṹ��
	* 
	*	������
	*		tocode			Ŀ����뷽ʽ
	*		fromcode		��ת���ı����ʽ
	* 
	*	����ֵ��
	*		ת�����ṹ�����óɹ�	��Чָ��
	*		ʧ��					NULLָ��
	*/
	iconv_t CreateConverter(const char* tocode, const char* fromcode);


	/*
	*	��fromcode����������ַ���strת��Ϊtocode������ַ���
	* 
	*	������
	*		icd				ת�����ṹ��
	*		str				fromcode������ַ���
	* 
	*	����ֵ��
	*		true			run function success
	*		false			run function failure
	*/
	bool RunConverter(iconv_t icd, const unsigned char* str);

	/*
	*	�ú������ڻ�ȡRunConverter����ִ�к�Ľ��
	* 
	*	������
	*		icd			ת����
	*		
	*	����ֵ��
	*		ת�������������ָ�룬ת���ɹ��ַ�����Ч������Ϊ���ַ�����
	* 
	*/
	const unsigned char* ReadOutPutINFO(iconv_t icd);

	/*
	*	��ǰ�������ڹ���iconv_t�Ķ�̬�ڴ�(heap)
	*/
	bool DestroyConverter(iconv_t icd);


#ifdef __cplusplus
}
#endif

#endif // !__ICONV_H__

