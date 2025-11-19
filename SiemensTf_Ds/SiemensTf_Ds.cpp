// SiemensTf_Ds.cpp: 定义 DLL 的初始化例程。
//
#include "pch.h"
#include "framework.h"
#include "SiemensTf_Ds.h"

#include <stdio.h>

#include "io.h"
#include "fcntl.h"

#include <wincon.h>
#include <stdlib.h>
#include <time.h>
#include <Nb30.h>
#pragma comment(lib,"netapi32.lib") 

/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include "ProMenu.h"
#include "ProUtil.h"
#include "ProMenubar.h"
#include <ProWindows.h>
#include <ProSolid.h>
#include <comdef.h>
#include <ProMessage.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*--------------------------------------------------------------------*\
Functions declaration  函数声明
\*--------------------------------------------------------------------*/
static uiCmdAccessState AccessAvailable(uiCmdAccessMode);
static uiCmdAccessState AccessUNAvailable(uiCmdAccessMode);
static uiCmdAccessState AccessDisAllow(uiCmdAccessMode);
static uiCmdAccessState AccessAvailableBySolid(uiCmdAccessMode);
static uiCmdAccessState AccessAvailableByAsm(uiCmdAccessMode);
static uiCmdAccessState AccessAvailableByPart(uiCmdAccessMode);
static uiCmdAccessState AccessAvailableByMfgOrAsm(uiCmdAccessMode);
static uiCmdAccessState AccessAvailableByMfgOrSolid(uiCmdAccessMode access_mode);
static uiCmdAccessState AccessAvailableByDrawing(uiCmdAccessMode access_mode);

int ProTestCheckButtonValue(uiCmdCmdId command, uiCmdValue* p_value);
int ProTestCheckButton(uiCmdCmdId command, uiCmdValue* p_value);
int ProTestSetRadioButtonValue(uiCmdCmdId command, uiCmdValue* p_value);
int ProTestRadioButton(uiCmdCmdId command, uiCmdValue* p_value);

void NewProJectActFn();
bool FolderExist(CString strPath);
void UserRecordCurrentDirectory();

/*--------------------------------------------------------------------*\
Application global variables
\*--------------------------------------------------------------------*/
CString Path_TK, Path_SettingDir;

// CSiemensTfDsApp

BEGIN_MESSAGE_MAP(CSiemensTfDsApp, CWinApp)
END_MESSAGE_MAP()


// CSiemensTfDsApp 构造

CSiemensTfDsApp::CSiemensTfDsApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CSiemensTfDsApp 对象

CSiemensTfDsApp theApp;

/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include "ProMenu.h"
#include "ProMenubar.h"
#include "ProUtil.h"
#include <ProMessage.h>


typedef struct user_check_but {
	uiCmdCmdId command;
	ProBoolean state;
} UserCheckBut;

static UserCheckBut check_but[2];
static ProMenuItemName RadioItemName;
/*--------------------------------------------------------------------*\
Application global variables
\*--------------------------------------------------------------------*/
//CString Path_TK;
// CSiemensTfDsApp 初始化
/*--------------------------------------------------------------------*\
FUNCTION: user_initialize()
\*--------------------------------------------------------------------*/
//extern "C" int user_initialize()
//{
//	ProPath w_path;
//	ProCharLine pathtext;
//
//	//获取程序text文件夹所在的路径
//	ProToolkitApplTextPathGet(w_path);
//	ProWstringToString(pathtext, w_path);
//	Path_TK = CString(pathtext) + CString("\\");
//
//	ProError status = PRO_TK_NO_ERROR;
//	ProFileName  MsgFile;
//	ProStringToWstring(MsgFile, "SiemensTf.txt");//设置菜单信息文件名
//	ProMenuItemIcon  Creda_icon;             //设置图标文件名
//
//
//	return status;
//}

extern "C" int user_initialize() {
	ProPath w_path;
	ProCharLine pathtext;

	//获取程序text文件夹所在的路径
	ProToolkitApplTextPathGet(w_path);
	ProWstringToString(pathtext, w_path);
	Path_TK = CString(pathtext) + CString("\\");

	/*-----------------------------------------------------------------------------------------*\
		检查Creda保存设置文件的目录是否存在，若不存在则创建目录
	\*-----------------------------------------------------------------------------------------*/
	Path_SettingDir = "C:\\ProgramData";
	if (!FolderExist(Path_SettingDir))
		CreateDirectory(Path_SettingDir, NULL);

	Path_SettingDir = "C:\\ProgramData\\SiemensTf";
	if (!FolderExist(Path_SettingDir))
		CreateDirectory(Path_SettingDir, NULL);

	Path_SettingDir = "C:\\ProgramData\\SiemensTf\\";

	ProError status = PRO_TK_NO_ERROR;
	ProFileName  MsgFile;
	ProStringToWstring(MsgFile, "SiemensTf.txt");//设置菜单信息文件名
	//ProCmdItemIcon  Siemens_icon;             //设置图标文件名

	uiCmdCmdId  PB_project, Radio_act;
	/*--------------------------------------------------------------------*\
		添加菜单
	\*--------------------------------------------------------------------*/
	status = ProMenubarMenuAdd("SiemensTf", "SiemensTf", "Windows", PRO_B_TRUE, MsgFile);


	/*--------------------------------------------------------------------*\
		添加项目子菜单项
	\*--------------------------------------------------------------------*/
	ProMenubarmenuMenuAdd("SiemensTf", "ProJect", "CProJect",NULL, PRO_B_TRUE, MsgFile);

	//设置新项目菜单按钮的动作函数
	ProCmdActionAdd("PushButtonAct1", (uiCmdCmdActFn)NewProJectActFn,
		uiCmdPrioDefault, AccessAvailable,
		PRO_B_TRUE, PRO_B_TRUE, &PB_project);

	//添加新项目菜单按钮
	ProMenubarmenuPushbuttonAdd("ProJect", "NewProJect", "CNewProJect",
		"New ProJect", NULL,
		PRO_B_TRUE, PB_project, MsgFile);

	//sprintf_s(Siemens_icon, Path_TK + L"text\\resource\\project_new.gif");
	status = ProCmdIconSet(PB_project, "project_new.gif");

	//添加复选按钮和单选按钮子菜单项
	/*status = ProMenubarmenuMenuAdd((char*)"SiemensTf", (char*)"CheckAndRadioButton", (char*)"Check_Radio_Button",
		NULL, PRO_B_TRUE, MsgFile);*/
	/*--------------------------------------------------------------------*\
		添加复选菜单按钮
	\*--------------------------------------------------------------------*/
	//添加复选按钮和单选按钮子菜单项
	/*status = ProMenubarmenuMenuAdd((char*)"MSiemensTf", (char*)"CheckAndRadioButton", (char*)"Check_Radio_Button",
		NULL, PRO_B_TRUE, MsgFile);*/
	/*--------------------------------------------------------------------*\
		添加复选菜单按钮
	\*--------------------------------------------------------------------*/
	//设置复选菜单按钮的动作函数
	//status = ProCmdOptionAdd((char*)"TestCheckButtonOpt1", //选项命令名，必须唯一
	//	(uiCmdCmdActFn)ProTestCheckButton,                     //调用的动作函数
	//	PRO_B_TRUE,                                            //设置菜单项的类型，PRO_B_TRUE为复选菜单，PRO_B_FALSE为单选菜单
	//	(uiCmdCmdValFn)ProTestCheckButtonValue,                //设置菜单复选按钮之值的回调函数
	//	AccessAvailable,                                       //确定该菜单命令是否可以访问的回调函数
	//	PRO_B_TRUE,                                            //确定是否在非激活窗口显示该菜单项
	//	PRO_B_TRUE,                                            //确定是否在附属窗口显示该菜单项
	//	&check_but[0].command);                                //调用动作函数的命令标识号(ID号)

	//status = ProCmdOptionAdd((char*)"TestCheckButtonOpt2",
	//	(uiCmdCmdActFn)ProTestCheckButton, PRO_B_TRUE,
	//	(uiCmdCmdValFn)ProTestCheckButtonValue,
	//	AccessAvailable, PRO_B_TRUE, PRO_B_TRUE, &check_but[1].command);

	//复选菜单按钮
	//status = ProMenubarmenuChkbuttonAdd((char*)"CheckAndRadioButton",//父菜单的名称
	//	(char*)"TestChkBut1",                                            //复选按钮的名称
	//	(char*)"Check button 1",                                         //复选按钮的文本标签，必须与
	//	//信息资源文本一组信息(四行)中的第一行内容一致
	//	(char*)"Test check button",                                      //在线提示信息，必须与信息资源文本
	//	//一组信息中的第一行内容一致
	//	NULL,                                                            //邻近菜单按钮的名称， 取NULL指放在最后 
	//	PRO_B_TRUE,                                                      //指定菜单按钮是在邻近菜单按钮的上面还是下面，
	//	//PRO_B_TRUE指放在邻近菜单按钮的下面
	//	check_but[0].command,                                            //动作函数的标识
	//	MsgFile);                                                        //菜单信息文件名

	//status = ProMenubarmenuChkbuttonAdd((char*)"CheckAndRadioButton", (char*)"TestChkBut2", (char*)"Check button 2",
	//	(char*)"Test check button", NULL, PRO_B_TRUE, check_but[1].command, MsgFile);


	/*--------------------------------------------------------------------*\
		添加单选菜单按钮
	\*--------------------------------------------------------------------*/
	//设置单选按钮组的动作函数
	/*status = ProCmdOptionAdd((char*)"RadioGroupOpt",
		(uiCmdCmdActFn)ProTestRadioButton, PRO_B_FALSE,
		(uiCmdCmdValFn)ProTestSetRadioButtonValue,
		AccessAvailable, PRO_B_TRUE, PRO_B_TRUE, &Radio_act);*/

	//设置单选按钮组各单选按钮标识名
	//ProMenuItemName Radio_group_items[] = { "RadioButton1", "RadioButton2", "RadioButton3", "" };

	//设置单选按钮组各单选按钮标签名
	//static ProMenuItemLabel Radio_group_lables[] = { "RadioButton1", "RadioButton2", "RadioButton3", "" };

	//设置单选按钮组各单选按钮提示信息的关键词
	/*static ProMenuLineHelp Radio_group_help[] = { "Select radio button 1", "Select radio button 2",
		"Select radio button 3", "" };*/

	//添加单选菜单按钮
	/*status = ProMenubarmenuRadiogrpAdd((char*)"CheckAndRadioButton", (char*)"RadioGroup",
		3, Radio_group_items, Radio_group_lables, Radio_group_help, NULL, PRO_B_TRUE, Radio_act, MsgFile);*/

	return status;
}

/*--------------------------------------------------------------------*\
FUNCTION: user_terminate()
\*--------------------------------------------------------------------*/
extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//记录退出之前的当前路径
	//UserRecordCurrentDirectory();
}

BOOL CSiemensTfDsApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


/*================================================================*\
FUNCTION: AccessAvailable （菜单项的访问权限设置：可选）
\*================================================================*/
static uiCmdAccessState AccessAvailable(uiCmdAccessMode access_mode) {
	return (ACCESS_AVAILABLE);
}

/*================================================================*\
FUNCTION: AccessUNAvailable （菜单项的访问权限设置：可见不可选）
\*================================================================*/
static uiCmdAccessState AccessUNAvailable(uiCmdAccessMode access_mode) {
	return (ACCESS_UNAVAILABLE);
}

/*===========================================================================*\
FUNCTION: AccessAvailableByAsm （菜单项的访问权限设置：在当前模型是组装时可选）
\*===========================================================================*/
static uiCmdAccessState AccessAvailableByAsm(uiCmdAccessMode access_mode) {
	ProError status;
	ProMdl mdl;
	ProMdlType  type;

	status = ProMdlCurrentGet(&mdl);
	if (status == PRO_TK_NO_ERROR) {
		status = ProMdlTypeGet(mdl, &type);
		if (type == PRO_MDL_ASSEMBLY) {  //在当前模型是组装时可选
			//PRO_MDL_PART   PRO_MDL_MFG  PRO_MDL_DRAWING
			return (ACCESS_AVAILABLE);
		} else {
			return (ACCESS_UNAVAILABLE);
		}
	} else {
		return (ACCESS_UNAVAILABLE);
	}
}

/*================================================================*\
FUNCTION: AccessDisAllow （菜单项的访问权限设置：不可选）
\*================================================================*/
static uiCmdAccessState AccessDisAllow(uiCmdAccessMode access_mode) {
	return (ACCESS_DISALLOW);
}

/*================================================================*\
FUNCTION: AccessAvailableBySolid （菜单项的访问权限设置：在当前模型是实体时可选）
\*================================================================*/
static uiCmdAccessState AccessAvailableBySolid(uiCmdAccessMode access_mode) {
	ProError status;
	ProMdl mdl;
	ProMdlType  type;

	status = ProMdlCurrentGet(&mdl);
	if (status == PRO_TK_NO_ERROR) {
		status = ProMdlTypeGet(mdl, &type);
		if (type == PRO_MDL_PART || type == PRO_MDL_ASSEMBLY) {
			return (ACCESS_AVAILABLE);
		} else {
			return (ACCESS_UNAVAILABLE);
		}
	} else {
		return (ACCESS_UNAVAILABLE);
	}
}

/*================================================================*\
FUNCTION: AccessAvailableByPart （菜单项的访问权限设置：在当前模型是零件时可选）
\*================================================================*/
static uiCmdAccessState AccessAvailableByPart(uiCmdAccessMode access_mode) {
	ProError status;
	ProMdl mdl;
	ProMdlType  type;

	status = ProMdlCurrentGet(&mdl);
	if (status == PRO_TK_NO_ERROR) {
		status = ProMdlTypeGet(mdl, &type);
		if (type == PRO_MDL_PART) {
			return (ACCESS_AVAILABLE);
		} else {
			return (ACCESS_UNAVAILABLE);
		}
	} else {
		return (ACCESS_UNAVAILABLE);
	}
}

/*================================================================*\
FUNCTION: AccessAvailableByMfgOrAsm （菜单项的访问权限设置：在当前模型是Mfg或组装时可选）
\*================================================================*/
static uiCmdAccessState AccessAvailableByMfgOrAsm(uiCmdAccessMode access_mode) {
	ProError status;
	ProMdl mdl;
	ProMdlType  type;

	status = ProMdlCurrentGet(&mdl);
	if (status == PRO_TK_NO_ERROR) {
		status = ProMdlTypeGet(mdl, &type);
		if (type == PRO_MDL_MFG || type == PRO_MDL_ASSEMBLY) {
			return (ACCESS_AVAILABLE);
		} else {
			return (ACCESS_UNAVAILABLE);
		}
	} else {
		return (ACCESS_UNAVAILABLE);
	}
}

/*================================================================*\
FUNCTION: AccessAvailableByMfgOrSolid （菜单项的访问权限设置：在当前模型是Mfg或实体时可选）
\*================================================================*/
static uiCmdAccessState AccessAvailableByMfgOrSolid(uiCmdAccessMode access_mode) {
	ProError status;
	ProMdl mdl;
	ProMdlType  type;

	status = ProMdlCurrentGet(&mdl);
	if (status == PRO_TK_NO_ERROR) {
		status = ProMdlTypeGet(mdl, &type);
		if (type == PRO_MDL_PART || type == PRO_MDL_ASSEMBLY || type == PRO_MFG) {
			return (ACCESS_AVAILABLE);
		} else {
			return (ACCESS_UNAVAILABLE);
		}
	} else {
		return (ACCESS_UNAVAILABLE);
	}
}

/*================================================================*\
FUNCTION: AccessAvailableByDrawing （菜单项的访问权限设置：在当前模型是绘图时可选）
\*================================================================*/
static uiCmdAccessState AccessAvailableByDrawing(uiCmdAccessMode access_mode) {
	ProError status;
	ProMdl mdl;
	ProMdlType  type;

	status = ProMdlCurrentGet(&mdl);
	if (status == PRO_TK_NO_ERROR) {
		status = ProMdlTypeGet(mdl, &type);
		if (type == PRO_MDL_DRAWING) {
			return (ACCESS_AVAILABLE);
		} else {
			return (ACCESS_UNAVAILABLE);
		}
	} else {
		return (ACCESS_UNAVAILABLE);
	}
}

/*====================================================================*\
	FUNCTION :	ProTestCheckButtonValue()
	PURPOSE  :  Set new value for check buttons  设置复选按钮之值
\*====================================================================*/
int ProTestCheckButtonValue(
	uiCmdCmdId command,    //ProCmdOptionAdd()函数传给回调用函数的菜单复选按钮命令的ID号
	uiCmdValue* p_value)   //是指向菜单复选按钮的状态值的指针，通过该指针可以调用ProMenubarmenuChkbuttonValueGet()
	//函数获得菜单复选按钮的状态，选中状态值为PRO_B_TRUE, 未选中状态值为PRO_B_FALSE
{
	ProError status;
	ProBoolean value;
	int i;

	for (i = 0; i < sizeof(check_but) / sizeof(check_but[0]); i++) {

		if (check_but[i].command == command) {
			status = ProMenubarmenuChkbuttonValueGet(p_value, &value);

			if (value == check_but[i].state)
				continue;

			status = ProMenubarmenuChkbuttonValueSet(p_value, check_but[i].state);
			break;
		}
	}




	return(0);
}

/*====================================================================*\
	FUNCTION :	ProTestCheckButton()
	PURPOSE  :  Callback function for check button options  复选按钮1的动作函数
\*====================================================================*/
int ProTestCheckButton(
	uiCmdCmdId command,         //ProCmdOptionAdd()函数传给回调用函数的菜单复选按钮命令的ID号
	uiCmdValue* p_value         //是指向菜单复选按钮的状态值的指针，通过该指针可以调用ProMenubarmenuChkbuttonValueGet()
	//函数获得菜单复选按钮的状态，选中状态值为PRO_B_TRUE, 未选中状态值为PRO_B_FALSE
) //void *p_push_command_data
{
	//切换菜单复选按钮项的开关状态

	for (int i = 0; i < sizeof(check_but) / sizeof(check_but[0]); i++)
		if (check_but[i].command == command) {
			//切换菜单复选按钮项的开关状态
			if (check_but[i].state == PRO_B_FALSE)
				check_but[i].state = PRO_B_TRUE;
			else
				check_but[i].state = PRO_B_FALSE;

			//获得菜单复选按钮之值
			ProMenubarmenuChkbuttonValueGet(p_value, &check_but[i].state);

			//根据当前菜单复选按钮之值确定执行的命令
			if (check_but[i].state) {
				switch (i) {
				case 0:
					AfxMessageBox(_T("Check button 1 has been selected"));
					break;
				case 1:
					AfxMessageBox(_T("Check button 2 has been selected"));
					break;
				}
			} else {
				switch (i) {
				case 0:
					AfxMessageBox(_T("Check button 1 unselected"));
					break;
				case 1:
					AfxMessageBox(_T("Check button 2 unselected"));
					break;
				}
			}

			break;
		}

	return(0);
}

/*====================================================================*\
	FUNCTION :	ProTestRadioButton()
	PURPOSE  :  单选按钮组的动作函数
\*====================================================================*/
int ProTestRadioButton(uiCmdCmdId command, uiCmdValue* p_value) {
	ProError status;
	//获得在单选按钮组中当前所选中的单选按钮的名称
	status = ProMenubarMenuRadiogrpValueGet(p_value, RadioItemName);
	if (status == PRO_TK_NO_ERROR) {
		if (strcmp(RadioItemName, (char*)"RadioButton1") == 0) {
			AfxMessageBox(_T("Radio button 1 has been selected"));
		}

		if (strcmp(RadioItemName, (char*)"RadioButton2") == 0) {
			AfxMessageBox(_T("Radio button 2 has been selected"));
		}

		if (strcmp(RadioItemName, (char*)"RadioButton3") == 0) {
			AfxMessageBox(_T("Radio button 3 has been selected"));
		}
	}

	return 0;
}

/*====================================================================*\
	FUNCTION :	ProTestSetRadioButtonValue()
	PURPOSE  :  设置单选按钮组之值
\*====================================================================*/
int ProTestSetRadioButtonValue(uiCmdCmdId command, uiCmdValue* p_value) {
	ProMenubarMenuRadiogrpValueSet(p_value, RadioItemName);
	return 0;
}
