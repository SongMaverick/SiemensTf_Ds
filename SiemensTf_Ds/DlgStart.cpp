#include "pch.h"
#include "SiemensTf_Ds.h"
#include <ProWindows.h>
#include "DIgProject.h"
#include "Dialog1.h"

/*--------------------------------------------------------------------*\
Functions declaration
\*--------------------------------------------------------------------*/
ProError UsrActivateCurrentWindow();

//变量声明  全局变量
CDIgProject DlgProject; //项目对话框指针
//CDialog1 Dialog1; //项目对话框指针

void NewProJectActFn() {

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int status;

	//创建对话框
	status = DlgProject.Create(IDD_DIgProject, NULL);

	//显示对话框
	DlgProject.ShowWindow(SW_SHOW);

	//激活当前窗口
	UsrActivateCurrentWindow();

	return;
}
