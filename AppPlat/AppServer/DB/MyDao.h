#pragma once

#ifndef __INCLUDE_MY_DAO_H__
#define __INCLUDE_MY_DAO_H__
#include <windows.h>
#include <list>
#include <stdio.h>
#include <tchar.h>
using namespace std;

#pragma warning(disable:4018 4996 4172)

#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" no_namespace rename("EOF","ADOEOF") rename("BOF","ADOBOF")
#define DATEBASE_TYPE_ACCESS	0
#define DATEBASE_TYPE_SQL2000   1
#define DATEBASE_TYPE_SQL2005   2
#define DATEBASE_TYPE_OTHER		3
#define DATEBASE_TYPE_SQL2008   4

#define DEF_MAX_STR_LEN			(100)

class CMyDao
{
public:
	CMyDao();
	~CMyDao();
	BOOL InitCom();        //��ʼ��com���
	void releaseCom();     //�ͷ�com���

public:
	//��ʼ�����ݿ�
	BOOL OpenDateBase(TCHAR* strDateBaseName,int nDataType,TCHAR* strUser,TCHAR* strPassWord,TCHAR* strIP);

	//�ر����ݿ�
	void CloseDataBase();

	//�������
	BOOL AddData(TCHAR* strTableName,list<TCHAR*>& strCloumValue,int nCloumNum);
	//�ַ���ת��Ϊָ������
	BOOL StringToDataType(TCHAR* strValue,int nDataType, VARIANT *pVar);

	//��ȡ����
	BOOL GetData(TCHAR* strSql,list<TCHAR*>& strQueryCloum,int nCloumNum,list<TCHAR*>& strDataValue);

	//����ת�ַ���
	TCHAR* DataToStringType(VARIANT Var);

	//�޸�����
	BOOL EditData(TCHAR* strSQL/*, int nColumnNum, list<TCHAR*>& pStrFieldName, list<TCHAR*> &saValue*/);

	//ɾ������
	BOOL DeleteData(TCHAR* strSQL);

private:
	//��ȡָ��λ�õ�ֵ
	TCHAR* GetPos(list<TCHAR*>& str, int iPos)
	{
		if (str.size() <= iPos)
		{
			return NULL;
		}
		list<TCHAR*>::iterator it = str.begin();
		int iCount = 0;
		while (it != str.end())
		{
			if (++iCount == iPos)
			{
				break;
			}
			it++;
		}

		return *it;
	}

private:
	_ConnectionPtr m_pConnection;  //�������ݿ��ָ�����
	_RecordsetPtr  m_pRecordset;   //������¼����ָ�����
	_CommandPtr    m_pCommandmsg;  //SQL�����ָ�����

	BOOL m_binitComFlag;
	TCHAR m_strErrorMsg[DEF_MAX_STR_LEN];         //������Ϣ
	TCHAR m_strConnect[DEF_MAX_STR_LEN];          //���Ӵ�
};

#endif //__INCLUDE_MY_DAO_H__

