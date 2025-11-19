// DIgProject.cpp: 实现文件
//

#include "pch.h"
#include "SiemensTf_Ds.h"
#include "afxdialogex.h"
#include "DIgProject.h"


// CDIgProject 对话框

IMPLEMENT_DYNAMIC(CDIgProject, CDialog)

CDIgProject::CDIgProject(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIgProject, pParent)
{

}

CDIgProject::~CDIgProject()
{
}

void CDIgProject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDIgProject, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDOK, &CDIgProject::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDIgProject::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDIgProject 消息处理程序

void CDIgProject::OnMove(int x, int y) {
	CDialog::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
}

void CDIgProject::OnBnClickedOk() {
	// TODO: 在此添加控件通知处理程序代码
	
	DestroyWindow();  // 非模态的关闭方式
	// CDialog::OnOK(); 模态的关闭方式
}

void CDIgProject::OnBnClickedCancel() {
	// TODO: 在此添加控件通知处理程序代码

	DestroyWindow();  // 非模态的关闭方式
	// CDialog::OnCancel(); 模态的关闭方式
}

void CDIgProject::OnClose() {
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	DestroyWindow();  // 非模态的关闭方式
	// CDialog::OnClose(); 模态的关闭方式
}

BOOL CDIgProject::OnInitDialog() 
{
	/*-----------------------------------------------------------------------------------------*\
		检查程序是否执行到这里
	\*-----------------------------------------------------------------------------------------*/
	/*CString message;
	message.Format(_T("DLG row = 73！"));
	AfxMessageBox(message);*/

	return TRUE;
}
