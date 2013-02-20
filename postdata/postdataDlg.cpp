
// postdataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "postdata.h"
#include "postdataDlg.h"
#include <afxinet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CpostdataDlg 对话框




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


// CpostdataDlg 消息处理程序

BOOL CpostdataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CpostdataDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CpostdataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CpostdataDlg::OnBnClickedOk()
{
    // http://192.168.0.163:8081/pay?email=4545@qq.com&pwd=3##9-+&save_login=2 参数以POST方式发送给服务器
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
    // http://192.168.0.163:8081/pay?email=4545@qq.com&pwd=3##9-+&save_login=2 参数以GET方式发送给服务器
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
        //发起请求
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
                strRet = "操作请求提交成功,请耐心等待";
            }
            else
            {
                strRet = "访问失败,请稍后重试";
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
            strRet = "访问失败,请稍后重试";            
            break;
        }

        //取得返回值
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
        //发起请求
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
                strRet = "操作请求提交成功,请耐心等待";
            }
            else
            {
                strRet = "访问失败,请稍后重试";
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
            strRet = "访问失败,请稍后重试";            
            break;
        }

        //取得返回值
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
