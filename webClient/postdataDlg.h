
// postdataDlg.h : ͷ�ļ�
//

#pragma once


// CpostdataDlg �Ի���
class CpostdataDlg : public CDialog
{
// ����
public:
	CpostdataDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_POSTDATA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    bool HttpPost( CString strServer,int nPort,CString strUri,CString strPara,CString &strRet );
    bool HttpGet( CString strUrl,CString &strRet );
    afx_msg void OnBnClickedOk2();
};
