
// postdataDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "postdata.h"
#include "postdataDlg.h"
#include <afxinet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CpostdataDlg �Ի���




CpostdataDlg::CpostdataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CpostdataDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpostdataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CpostdataDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CpostdataDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDOK2, &CpostdataDlg::OnBnClickedOk2)
END_MESSAGE_MAP()


// CpostdataDlg ��Ϣ�������

BOOL CpostdataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CpostdataDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CpostdataDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CpostdataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CpostdataDlg::OnBnClickedOk()
{
    // http://192.168.0.163:8081/pay?email=4545@qq.com&pwd=3##9-+&save_login=2 ������POST��ʽ���͸�������
    CString strRet;
    CString strServer = "192.168.0.163";
    int nPort = 8081;
    CString strUri = "/pay?";
    CString strPara = "email=4545@qq.com&pwd=3##9-+&save_login=2";
    if(HttpPost(strServer,nPort,strUri,strPara,strRet))
        MessageBox(strRet.GetBuffer());
    else
        MessageBox("error ");

}

void CpostdataDlg::OnBnClickedOk2()
{
    // http://192.168.0.163:8081/pay?email=4545@qq.com&pwd=3##9-+&save_login=2 ������GET��ʽ���͸�������
    CString strRet;
    CString strUrl = "http://192.168.0.187:8088/callback/logon.php?userId=1234567890&key=012345678901234567890123&channelId=xx&region=0&cpparam=xx";
    if(HttpGet(strUrl,strRet))
        MessageBox(strRet.GetBuffer());
    else
        MessageBox("error ");
}


bool CpostdataDlg::HttpPost( CString strServer,int nPort,CString strUri,CString strPara,CString &strRet )
{
    bool bRet = false;
    DWORD dwBeginMs = GetTickCount();
    do 
    {
        //��������
        CInternetSession objSession;
        objSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 500); 
        objSession.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500);          
        objSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 500); 
        objSession.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 500);
        objSession.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 500); 
        objSession.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);

        CHttpFile *pHttpFile = NULL;    
        CHttpConnection* pConn = NULL;
        CString strHeader = "";        
        try
        {/*
            pHttpFile = (CHttpFile*)objSession.OpenURL(strUrl);
            */            
            CHttpConnection* pConn = objSession.GetHttpConnection(strServer,(INTERNET_PORT)nPort);
            pHttpFile = pConn->OpenRequest(CHttpConnection::HTTP_VERB_POST,strUri);
            pHttpFile->SendRequest(strHeader,strPara.GetBuffer(),strPara.GetLength());            
        }
        catch(CInternetException * pException)
        {            
            if(ERROR_INTERNET_TIMEOUT == pException->m_dwError)
            {
                strRet = "���������ύ�ɹ�,�����ĵȴ�";
            }
            else
            {
                strRet = "����ʧ��,���Ժ�����";
            }
            delete pHttpFile;
            pException->Delete();
            objSession.Close();
            break;
        }
        catch(...)
        {            
            delete pHttpFile;
            delete pConn;
            objSession.Close();
            strRet = "����ʧ��,���Ժ�����";            
            break;
        }

        //ȡ�÷���ֵ
        strRet = "";       
        do 
        {
            if (!pHttpFile)
                break;
            CString strLine;
            DWORD dwStatusCode;
            pHttpFile->QueryInfoStatusCode(dwStatusCode);
            if (dwStatusCode != 200)               
            {               
                break;
            }

            pHttpFile->ReadString(strRet);
            bRet = true;          

        } while (0);

        if (pHttpFile)
            pHttpFile->Close();
        delete pHttpFile;

    } while (0);   

    return bRet;
}

bool CpostdataDlg::HttpGet( CString strUrl,CString &strRet )
{
    bool bRet = false;
    DWORD dwBeginMs = GetTickCount();
    do 
    {
        //��������
        CInternetSession objSession;
        objSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 500); 
        objSession.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 500);          
        objSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 500); 
        objSession.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 500);
        objSession.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 500); 
        objSession.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);

        CHttpFile *pHttpFile = NULL;    
        try
        {
            pHttpFile = (CHttpFile*)objSession.OpenURL(strUrl);       
        }
        catch(CInternetException * pException)
        {            
            if(ERROR_INTERNET_TIMEOUT == pException->m_dwError)
            {
                strRet = "���������ύ�ɹ�,�����ĵȴ�";
            }
            else
            {
                strRet = "����ʧ��,���Ժ�����";
            }
            delete pHttpFile;
            pException->Delete();
            objSession.Close();
            break;
        }
        catch(...)
        {            
            delete pHttpFile;
            objSession.Close();
            strRet = "����ʧ��,���Ժ�����";            
            break;
        }

        //ȡ�÷���ֵ
        strRet = "";       
        do 
        {
            if (!pHttpFile)
                break;
            CString strLine;
            DWORD dwStatusCode;
            pHttpFile->QueryInfoStatusCode(dwStatusCode);
            if (dwStatusCode != 200)               
            {               
                break;
            }

            pHttpFile->ReadString(strRet);
            bRet = true;          

        } while (0);

        if (pHttpFile)
            pHttpFile->Close();
        delete pHttpFile;

    } while (0);   

    return bRet;
}
