#pragma once 
#include <Windows.h>
/* access to build control class to realize the single mode of c++ */
/* ����ͳһ��control��ȥʵ��ʱgetinstance �� destoryinstance �ǳ�ʼ��������
��Դ�Ĺ��̣����಻֪��������Ҫ��ʲô��Ա��Ҫ��ʼ�������٣������ṩ����ĳ�ʼ��
�����ٺ���,�Լ���Ӧ��ģ��*/

//�����˵�����ģ��ģʽ
//��̬��Ϊ��̬��̬�Ͷ�̬��̬����̬��̬����ָ����麯���������ڴ棬 
//��ģ��ģʽ���þ�̬��̬����ģ�����class<T> a; a->getinstance()��Ӧ����Ĳ��� 
#ifndef __CONTROL_H__
#define __CONTROL_H__

template<typename T>
class CControl{
protected://��ֹ���ֱ�Ӵ������
	CControl();
	~CControl();
	CControl& operator = (CControl& control);
	CControl(CControl& control);
private:
	static CControl *m_pSingleton;
public:
	static CControl* getInstance();
	static void destoryInsstance();
	//setConsoleCtrlHandler �¼��������
	//��װ����̨�¼�������
	static BOOL WINAPI HandlerRoutine(DWORD wdCtrlType);
	BOOL OpenControl();
	void CloseControl();
	//�ṩ������ʵ����Դ�ĳ�ʼ��������
	BOOL Init();
	void UnInit();
	 
	

public:
	// the index of whether console quit to notify main thread
	static BOOL m_bQuit;


};

//use the class template
//static number to init
template<typename T>
CControl<T>* CControl<T>::m_pSingleton = NULL;
template<typename T>
BOOL CControl<T>::m_bQuit = FALSE;
template<typename T>
CControl<T>::CControl(){

}
template<typename T>
CControl<T>::~CControl(){

}
template<typename T>
CControl<T>& CControl<T>::operator = (CControl<T>& control){
	return *CControl<T>::m_pSingleton;
}
template<typename T>
CControl<T>::CControl(CControl<T>& control){

}
template<typename T>
CControl<T>* CControl<T>::getInstance(){
	if( CControl<T>::m_pSingleton == NULL){
		//notice here
		CControl<T>::m_pSingleton = new T;
	}
	return CControl<T>::m_pSingleton;
}
template<typename T>
void CControl<T>::destoryInsstance(){
	if( CControl<T>::m_pSingleton != NULL){
		//ǿ�Ƶ������������
		delete (T*)CControl<T>::m_pSingleton;
		CControl::m_pSingleton = NULL;
	}
	return ;
}
template<typename T>
BOOL WINAPI CControl<T>::HandlerRoutine(DWORD wdCtrlType){
	switch(wdCtrlType){
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		{
			CControl<T>::m_bQuit = TRUE;
			while(1){
				Sleep(10);
			}
		}
		break;
	}
	return true;
}
template<typename T>
BOOL CControl<T>::OpenControl(){
	SetConsoleCtrlHandler(CControl<T>::HandlerRoutine,TRUE);

	((T*)CControl<T>::m_pSingleton)->Init();
	return TRUE;
}
template<typename T>
void CControl<T>::CloseControl(){
	((T*)CControl<T>::m_pSingleton)->UnInit();
}


#endif 



