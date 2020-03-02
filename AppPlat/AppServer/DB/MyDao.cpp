#include "MyDao.h"
#include <wchar.h>
CMyDao::CMyDao()
{
	m_binitComFlag = FALSE;
	InitCom();
}
CMyDao::~CMyDao()
{
	releaseCom();
}
BOOL CMyDao::InitCom()
{
	#if _WIN32_WINNT > 0x500
		//�����֣�����Ϊnull�� �����ط�ʽ COINIT_MULTITHREADED���̵߳ķ�ʽ����
		// �Զ��̷߳�ʽ��comͨ��
		::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	#else
		::CoInitialize(NULL);
	#endif
		m_binitComFlag = TRUE;
	return TRUE;
}
void CMyDao::releaseCom()
{
	if (m_binitComFlag)
	{
		::CoUninitialize();
	}
}
////////////////////////////////////////////////////////////////////
//��ʼ�����ݿ�
BOOL CMyDao::OpenDateBase(TCHAR* strDateBaseName,int nDataType,TCHAR* strUser,TCHAR* strPassWord,TCHAR* strIP)
{
	//"colin",DATEBASE_TYPE_SQL2005,"sa","sa","127.0.0.1,2433"
	if (nDataType == DATEBASE_TYPE_ACCESS)
	{
		//CFileFind filefind;
		//BOOL bFind = filefind.FindFile(strDateBaseName);

		//if (!bFind)
		//{
		//	m_strErrorMsg = _T("�Ҳ������ݿ�");
		//	return FALSE;
		//}
		//else
		//{
		//	m_strConnect.Format(_T("provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s"),strDateBaseName);
		//}
	}
	else if (nDataType == DATEBASE_TYPE_SQL2000 || nDataType == DATEBASE_TYPE_SQL2008)
	{
		_stprintf(m_strConnect, _T("Driver={SQL Server};Server=%s;Uid=%s;Pwd=%s;Database=%s"), strIP,strUser,strPassWord,strDateBaseName);
		//m_strConnect.Format(_T("Driver={SQL Server};Server=%s;Uid=%s;Pwd=%s;Database=%s"),strIP,strUser,strPassWord,strDateBaseName);
	}
	else if (nDataType == DATEBASE_TYPE_SQL2005)
	{
		_stprintf(m_strConnect, _T("Driver={SQL Native Client};Server=%s;Uid=%s;Pwd=%s;Database=%s"), strIP,strUser,strPassWord,strDateBaseName);
		//m_strConnect.Format(_T("Driver={SQL Native Client};Server=%s;Uid=%s;Pwd=%s;Database=%s"),strIP,strUser,strPassWord,strDateBaseName);
	}
	else
	{
		_tcscpy(m_strErrorMsg, _T("û�к��ʵ����⴮"));
		return FALSE;
	}

	//ʵ����ָ�����
	HRESULT hRes;
	//��CStringת����com�������ַ�������
	_bstr_t bstrConnection(m_strConnect);
	_bstr_t bstrUser(strUser);
	_bstr_t bstrPassword(strPassWord);

	try
	{
		//�������Ӷ��� ������
		hRes = m_pConnection.CreateInstance(__uuidof(Connection));
		if (!SUCCEEDED(hRes))
		{
			_tcscpy(m_strErrorMsg, _T("ʵ��������ָ�����ʧ��"));
			//m_strErrorMsg = _T("ʵ��������ָ�����ʧ��");
			return FALSE;
		}

		//�������ݿ�(���⴮���û���������,�򿪷�ʽ)
		//�ַ���Ҫ�õ���com������ַ��� adModeShareDenyNone���û���ͬ���Է���һ�����ݿ�
		hRes = m_pConnection->Open(bstrConnection,bstrUser,bstrPassword,adModeShareDenyNone);
		if (!SUCCEEDED(hRes))
		{
			_tcscpy(m_strErrorMsg, _T("�����ݿ�ʧ��"));
			//m_strErrorMsg = _T("�����ݿ�ʧ��");
			return FALSE;
		}

		//ʵ�������ݼ�ָ�����
		hRes = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if (!SUCCEEDED(hRes))
		{
			_tcscpy(m_strErrorMsg, _T("ʵ�������ݼ�ָ�����ʧ��"));
			//m_strErrorMsg = _T("ʵ�������ݼ�ָ�����ʧ��");
			return FALSE;
		}
		//ʵ�������ָ�����
		hRes = m_pCommandmsg.CreateInstance(__uuidof(Command));
		if (!SUCCEEDED(hRes))
		{
			_tcscpy(m_strErrorMsg, _T("ʵ�������ָ�����ʧ��"));
			//m_strErrorMsg = _T("ʵ�������ָ�����ʧ��");
			return FALSE;
		}

	}catch(_com_error *e)
	{
		//AfxMessageBox(e->Description());
		//m_strErrorMsg.Format(_T("%s"),e->Description());
		_stprintf(m_strErrorMsg, _T("%s"), e->Description());
		return FALSE;
	}

	return TRUE;
}

//�ر����ݿ�
void CMyDao::CloseDataBase()
{
	m_pConnection->Close();
	//m_pConnection.DestroyInstance();
}

//�������
BOOL CMyDao::AddData(TCHAR* strTableName,list<TCHAR*>& strCloumValue,int nCloumNum)
{
	TCHAR strQuery[100];
	_stprintf(m_strErrorMsg, _T("select * from [%s]"),strTableName);
	//strQuery.Format(_T("select * from [%s]"),strTableName);

	//�Դ����ַ�������
	_bstr_t bstrQuery(strQuery);
	_bstr_t bstrCource(m_strConnect);

	HRESULT hres;
	Fields *pFiles = NULL;
	//ado�����е��������Ͷ�д��VARIANT������
	VARIANT varIndex;
	varIndex.vt = VT_I4; //R����float CY==Ǯ��  VT_����Ķ���comADO�������������

	Field  *pFile = NULL;
	DataTypeEnum dataType;

	VARIANT varValue;
	try
	{
		//�򿪱�
		hres = m_pRecordset->Open(bstrQuery,bstrCource,adOpenDynamic,adLockBatchOptimistic,adCmdText);
		if (!SUCCEEDED(hres))
		{
			//m_strErrorMsg = _T("�򿪱�ʧ��");
			_stprintf(m_strErrorMsg, _T("�򿪱�ʧ��"));
			return FALSE;
		}
		else
		{
			//������Ӽ�¼������
			int nRosNum = (int)(strCloumValue.size()/nCloumNum);
			if(nRosNum <= 0 )
			{
				//m_strErrorMsg = _T("������������");
				//AfxMessageBox(_T("���������������������"));
				_stprintf(m_strErrorMsg, _T("������������"));
				return FALSE;
			}
			//��Ӽ�¼ ѭ����
			for (int i=0; i<nRosNum; i++)
			{
				//��¼������Ҫ��Ӽ�¼
				m_pRecordset->AddNew();
				//Ҫ�Ѽ�¼һ��һ�е�д��ȥ
				for (int ncloum=0;ncloum<nCloumNum;ncloum++)
				{
					//����Ķ����ַ��� ��Ҫ�ȵõ��е��������� Ȼ��������Ӧ��ת�� �ٰ�������ӵ����ݿ���
					//�����ݿ����ж����Լ�����ʽ���ڵģ�������Ҫ�õ����е��У��ڴ��еõ�ÿһ����
					//Fields *pFiles �Ǹ�˫ָ��
					//1.�õ����е���
					m_pRecordset->get_Fields(&pFiles);

					//2.�õ��������
					//ADO��COM�����ݿ��� ���˶������ַ���������û�о��嶨�� ado�����е��������Ͷ�д��VARIANT������
					varIndex.intVal = ncloum;
					pFiles->get_Item(varIndex,&pFile);

					//3.�õ���(�ֶ�)��Ӧ����������
					pFile->get_Type(&dataType);

					//4.ת�� CString--����Ӧ������ 0 1 2 3 4 5   3
					//if (StringToDataType(strCloumValue.GetAt(ncloum+i*nCloumNum),dataType,&varValue))
					if (StringToDataType(GetPos(strCloumValue, ncloum+i*nCloumNum),dataType,&varValue))
					{
						//5.��Ӽ�¼
						pFile->put_Value(varValue);
						if (varValue.vt == (VT_UI1|VT_ARRAY))
						{
							SafeArrayDestroy(varValue.parray);
						}
					}
					
				}
				//��������
				m_pRecordset->UpdateBatch(adAffectCurrent);

			}
			//�رռ�¼�� ������رվͻᱻһֱռ�ã��ʹ򲻿���
			//if (pStrReturnID)
			//{
			//	m_pRecordset->MoveLast();
			//	m_pRecordset->get_Fields(&pFiles);

			//	varIndex.intVal = 0;
			//	pFiles->get_Item(varIndex,&pFile);
			//	VARIANT varKey;
			//	pFile->get_Value(&varKey);

			//	pStrReturnID->Format(_T("%d"),varKey.intVal);
			//}

			m_pRecordset->Close();
		}
	}catch(_com_error *e)
	{
		//AfxMessageBox(e->Description());
		//m_strErrorMsg.Format(_T("%s"),e->Description());
		_stprintf(m_strErrorMsg, _T("%s"),e->Description());
		return FALSE;
	}

	return TRUE;
}

//�ַ���ת��Ϊָ������
BOOL CMyDao::StringToDataType(TCHAR* strValue,int nDataType, VARIANT *pVar)
{
	switch(nDataType)
	{
	case adInteger: //����
		pVar->vt = VT_I2;
		pVar->intVal = _ttoi(strValue);
		break;
	case adBoolean: //BOOL����
		pVar->vt = VT_BOOL;
		pVar->bVal = _ttoi(strValue);
		break;
	case adSingle:   //������
		pVar->vt = VT_R4;
		pVar->fltVal = (float)_tstof(strValue);
		break;
	case adDouble:  //˫����
		pVar->vt = VT_R8;
		pVar->dblVal = _tstof(strValue);
		break;
	case adBSTR:   //�ַ���
	case adChar:
	case adVarChar:
	case adVarWChar:	
	case adWChar:
	case adLongVarWChar:
	case adLongVarChar:
		pVar->vt = VT_BSTR;
		pVar->bstrVal = (bstr_t)strValue;
		break;
	default:
		pVar->vt = VT_EMPTY;
		break;
	}
	return TRUE;
}

//��ȡ����
BOOL CMyDao::GetData(TCHAR* strSql,list<TCHAR*>& strQueryCloum,int nCloumNum,list<TCHAR*>& strDataValue)
{
	HRESULT het;
	//_bstr_t    �ַ���    		_variant_t �����������ͼ��� vt����   ..valueֵ								//com
	_bstr_t bstrSql = strSql;				//ִ��sql
	_bstr_t bstrConnect = m_strConnect;		//���⴮

	TCHAR strValue[100];

	try
	{
		het = m_pRecordset->Open(bstrSql,bstrConnect,adOpenDynamic,adLockOptimistic,adCmdText);
		if (!SUCCEEDED(het))
		{
			//AfxMessageBox(_T("�򿪱�ʧ��"));
			//m_strErrorMsg = _T("��ѯ��ʧ��");
			_stprintf(m_strErrorMsg, _T("��ѯ��ʧ��"));
			return FALSE;
		}
		//ѭ�����
		list<TCHAR*>::iterator it = strQueryCloum.begin();
		
		while(!m_pRecordset->ADOEOF)
		{
			it = strQueryCloum.begin();
			while (it != strQueryCloum.end())
			{
				_variant_t varBLOB;
				Fields *fields;
				FieldPtr fieldPtr;
				_bstr_t bstrQueryCloum;					//����

				bstrQueryCloum = *it;
				fields = m_pRecordset->GetFields();

				fieldPtr = fields->GetItem(bstrQueryCloum);

				varBLOB = fieldPtr->GetValue();
				if (202 == fieldPtr->GetType())
				{
					//ת������Ϊ���ֽڴ洢
					char szBuf[100];
					::WideCharToMultiByte(CP_ACP, 0, (WCHAR*)varBLOB.bstrVal, 
						wcslen((WCHAR*)varBLOB.bstrVal) + 1, szBuf, 100, NULL, FALSE);
					TCHAR* pCopy = new TCHAR[_tcslen(szBuf)+1];
					_tcscpy(pCopy, szBuf);
					strDataValue.push_back(pCopy);

				}
				else
				{
					varBLOB.vt = fieldPtr->GetType();
					_stprintf(strValue, _T("%s"), DataToStringType(varBLOB));
					TCHAR* pCopy = new TCHAR[_tcslen(strValue)+1];
					_tcscpy(pCopy, strValue);
					strDataValue.push_back(pCopy);
				}

				it++;
			}
			m_pRecordset->MoveNext();
		}

		m_pRecordset->Close();
	}
	catch(_com_error *e)
	{
		//m_strErrorMsg = e->ErrorMessage();
		//AfxMessageBox(e->ErrorMessage());
		_stprintf(m_strErrorMsg, _T("%s"), e->ErrorMessage());
	}

	return TRUE;
}

//����ת�ַ���
TCHAR* CMyDao::DataToStringType(VARIANT Var)
{
	TCHAR strValue[100];
	switch(Var.vt)
	{
	case VT_BOOL:
		//strValue.Format(_T("%d"), Var.boolVal);
		_stprintf(strValue, _T("%d"), Var.boolVal);
		break;
	case VT_I4:
		//strValue.Format(_T("%d"), Var.intVal);
		_stprintf(strValue, _T("%d"), Var.intVal);
		break;
	case VT_I8:
		{
			sprintf(strValue, "%I64d", Var.llVal);
		}
		break;
	case VT_R8:
	case VT_DECIMAL:
		//strValue.Format(_T("%0.2f"), Var.dblVal);
		_stprintf(strValue, _T("%0.2f"), Var.dblVal);
		break;
	case VT_BSTR:
		_stprintf(strValue, _T("%s"), Var.bstrVal);
		//strValue= Var.bstrVal;
		break;
	default:
		_stprintf(strValue, _T(""));
		break;
	}

	return strValue;
}
//�޸�����
BOOL CMyDao::EditData(TCHAR* strSQL/*, int nColumnNum, list<TCHAR*>& pStrFieldName, list<TCHAR*> &saValue*/)
{
	int nIndex = 0;		//�е�����
	int nCount = 0;		//�޸ĵ�����
	//int nTotal = (int)saValue.size();  //һ���޸ĵĸ���

	HRESULT hRet;
	_variant_t varFieldName;
	_variant_t varValue;

	_bstr_t bstrQuery = strSQL;
	_bstr_t bstrConnent = m_strConnect;
	try
	{
		hRet = m_pRecordset->Open(bstrQuery, bstrConnent, adOpenDynamic, adLockOptimistic, adCmdText);
		if(!SUCCEEDED(hRet))
		{
			_tcscpy(m_strErrorMsg, _T("�򿪱�ʧ��"));
			//m_strErrorMsg = _T("�򿪱�ʧ�ܣ�");
			return FALSE;
		}
		//if (m_pRecordset->ADOEOF)
		//{
		//	_tcscpy(m_strErrorMsg, _T("û��ѡ���޸ĵ���"));
		//	//m_strErrorMsg = _T("û��ѡ���޸ĵ���");
		//	m_pRecordset->Close();
		//	return FALSE;
		//}
		//Fields *pFiles = NULL;
		////ado�����е��������Ͷ�д��VARIANT������
		//VARIANT varIndex;
		//varIndex.vt = VT_I4; //R����float CY==Ǯ��  VT_����Ķ���comADO�������������

		//Field  *pFile = NULL;
		//list<TCHAR*>::iterator it = pStrFieldName.begin();
		//list<TCHAR*>::iterator itValue = saValue.begin();
		//while(!m_pRecordset->ADOEOF)
		//{
		//	////�޸���һ��  s#  sname    1 'ss'  2 'ss2'   stringarr  4    2
		//	//varFieldName.SetString((bstr_t)pStrFieldName[nIndex]);
		//	varFieldName.SetString((bstr_t)(*it));

		//	//varValue.SetString((bstr_t)saValue.GetAt(nCount));
		//	varValue.SetString((bstr_t)(*itValue));

		//	m_pRecordset->Update(varFieldName,varValue);
		//	nIndex++;
		//	it++;
		//	nCount++;
		//	itValue++;
		//	if(nIndex>=nColumnNum)
		//	{
		//		nIndex = 0;
		//		it = pStrFieldName.begin();
		//		m_pRecordset->MoveNext();
		//	}

		//	if (nColumnNum == 1)
		//	{
		//		if(nCount == nTotal)
		//		{
		//			break;
		//		}
		//	}
		//	else
		//	{
		//		if(nCount > nTotal)
		//		{
		//			break;
		//		}
		//	}
		//}
		try
		{
			m_pRecordset->Close();
		}
		catch(...)
		{
			
		}
	}
	catch(_com_error *e)
	{
		//m_strErrorMsg = e->ErrorMessage();
		//AfxMessageBox(e->Description());
		_stprintf(m_strErrorMsg, _T("%s"), e->Description());
		return FALSE;
	}

	return TRUE;
}

//ɾ������
BOOL CMyDao::DeleteData(TCHAR* strSQL)
{
	HRESULT hRet;

	_bstr_t bstrQuery = strSQL;
	_bstr_t bstrConnent = m_strConnect;

	try
	{
		hRet = m_pRecordset->Open(bstrQuery, bstrConnent, adOpenDynamic, adLockOptimistic, adCmdText);
		if(!SUCCEEDED(hRet))
		{
			_tcscpy(m_strErrorMsg, _T("�򿪱�ʧ��"));
			//m_strErrorMsg = _T("�򿪱�ʧ�ܣ�");
			return FALSE;
		}
		while(!m_pRecordset->ADOEOF)
		{
			m_pRecordset->Delete(adAffectCurrent);
			m_pRecordset->MoveNext();
		}

		m_pRecordset->Close();
	}
	catch(_com_error *e)
	{
		//AfxMessageBox(e->Description());
		_stprintf(m_strErrorMsg, _T("%s"), e->Description());
		return FALSE;
	}
	
	return TRUE;
}