#pragma once


// CUploadDlg �Ի���

class CUploadDlg : public CDialog
{
	DECLARE_DYNAMIC(CUploadDlg)

public:
	CUploadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUploadDlg();

// �Ի�������
	enum { IDD = IDD_UPLOADDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
