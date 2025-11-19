// Dialog1.cpp: 实现文件
//

#include "pch.h"
#include "SiemensTf_Ds.h"
#include "afxdialogex.h"
#include "Dialog1.h"


// CDialog1 对话框

IMPLEMENT_DYNAMIC(CDialog1, CDialog)

CDialog1::CDialog1(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

CDialog1::~CDialog1()
{
}

void CDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog1, CDialog)
END_MESSAGE_MAP()


// CDialog1 消息处理程序
