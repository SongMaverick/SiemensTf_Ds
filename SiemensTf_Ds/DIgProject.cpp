// DIgProject.cpp: 实现文件
//

#include "pch.h"
#include "SiemensTf_Ds.h"
#include "afxdialogex.h"
#include "DIgProject.h"


// CDIgProject 对话框

IMPLEMENT_DYNAMIC(CDIgProject, CDialog)

CDIgProject::CDIgProject(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIgProject, pParent), m_Edit_Value(_T("")), m_Edit_DimensionName(_T("")) {

}

CDIgProject::~CDIgProject() {}

void CDIgProject::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List_Para);

	DDX_Control(pDX, IDC_LIST3, m_List_Dimension);
	DDX_Control(pDX, IDC_LIST_DimensionValue, m_List_DimensionValue);

	DDX_Text(pDX, IDC_EDIT2, m_Edit_Value);
	DDX_Control(pDX, IDC_EDIT2, m_Edit);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_DimensionName);
}


BEGIN_MESSAGE_MAP(CDIgProject, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDOK, &CDIgProject::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDIgProject::OnBnClickedCancel)
	ON_WM_CLOSE()

	ON_LBN_SELCHANGE(IDC_LIST3, &CDIgProject::OnLbnSelchangeList3)
END_MESSAGE_MAP()

//声明函数
extern CString Path_TK;
ProError UserCollectParameters(
	ProModelitem* p_modelitem,   /* In:  The model item */
	ProParameter** p_parameters  /* Out: ProArray with collected parameters. */
);
CString UserDoubleToCString(double value, int n);
ProError UserUpdateDimByName(ProDimension* dimension, CString Dim_name, double Dim_value);
CString UserFromProNameToCString(ProName name);
ProError UserGetDimensionOfSolid(ProSolid solid, ProDimension** p_data);


// CDIgProject 消息处理程序

void CDIgProject::OnMove(int x, int y) {
	CDialog::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
}

void CDIgProject::OnBnClickedOk() {
	// TODO: 在此添加控件通知处理程序代码

	//DestroyWindow();  // 非模态的关闭方式
	// CDialog::OnOK(); 模态的关闭方式

	UpdateData(true);

	ProError   status;
	ProSolid   solid;
	ProMdlType mdl_type;

	//获得当前模型
	status = ProMdlCurrentGet((ProMdl*)&solid);
	if (status != PRO_TK_NO_ERROR) {
		return;
	}

	//获得模型的类型
	status = ProMdlTypeGet(solid, &mdl_type);
	if (status == PRO_TK_NO_ERROR) {
		if (mdl_type == PRO_MDL_ASSEMBLY || mdl_type == PRO_MDL_PART) {
			ProDimension* dim;

			status = UserGetDimensionOfSolid(solid, &dim);
			if (status == PRO_TK_NO_ERROR) {
				UserUpdateDimByName(dim, m_Edit_DimensionName, _ttof(m_Edit_Value));

				ProArrayFree((ProArray*)&dim);
				dim = NULL;
			}

			status = ProSolidRegenerate(solid, PRO_REGEN_NO_FLAGS);

			//status = ProWindowRefit();
			status = ProWindowRepaint(PRO_VALUE_UNUSED);

		}
	}
	//m_List_Mat.GetText(m_List_Mat.GetCurSel(), m_Mat);
	//m_List_DimValue.GetText(m_List_Mat.GetCurSel(), m_Width);

	//DestroyWindow();
	//CDialog::OnOK();

	UpdateData(false);
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


BOOL CDIgProject::OnInitDialog() {

	CDialog::OnInitDialog();

	ProError   status;
	ProName    part_name;      // ProName  32字节                            
	ProMdl     Model;
	ProSolid  solid;

	//获取模型的名称
	status = ProMdlCurrentGet(&Model);
	//status = ProMdlCurrentGet((ProMdl*) &solid);
	if (status != PRO_TK_NO_ERROR) {
		AfxMessageBox(_T("Can not find the model"));
		EndDialog(1);
		return false;
	}

	ProMdlType mdl_type;
	//获得模型的类型
	status = ProMdlTypeGet(Model, &mdl_type);
	if (status == PRO_TK_NO_ERROR) {
		//根据模型类型的不同执行对于的命令
		switch (mdl_type) {
		case PRO_MDL_ASSEMBLY:
			AfxMessageBox(_T("the model type is: PRO_MDL_ASSEMBLY"));
			break;
		case PRO_MDL_PART:
			AfxMessageBox(_T("the model type is: PRO_MDL_PART"));
			break;
		case PRO_MDL_DRAWING:
			AfxMessageBox(_T("the model type is: PRO_MDL_DRAWING"));
			break;
		case PRO_MDL_MFG:
			AfxMessageBox(_T("the model type is: PRO_MDL_MFG"));
			break;
		case PRO_MDL_LAYOUT:
			AfxMessageBox(_T("the model type is: PRO_MDL_LAYOUT"));
			break;
		case PRO_MDL_DWGFORM:
			AfxMessageBox(_T("the model type is: PRO_MDL_DWGFORM"));
			break;
		}
	}

	//模型用名称和类型初始化模型，获取内存中的模型
	//ProError   status;
	//ProName    part_name;      // ProName  32字节                            
	ProSolid   Solid;
	//ProMdlName    part_name;

	//ProStringToWstring(part_name, "4_ES_METAL_TUB_BEND_V_CJC000000.prt");
	//初始化模型
	//status = ProMdlnameInit(part_name, PRO_MDLFILE_PART, (ProMdl*)&Solid);
	//if (status == PRO_TK_NO_ERROR) {
		//AfxMessageBox(_T("Find the part -- 4_ES_METAL_TUB_BEND_V_CJC000000.prt"));

		//获取模型的名称
		/*status = ProMdlNameGet((ProMdl)Solid, part_name);
		if (status == PRO_TK_NO_ERROR) {
			m_TestData = UserFromProNameToCString(part_name);
		}*/

		////在内存中删除模型,若模型在窗口中打开，则自动关闭窗口
		//status = ProMdlErase((ProMdl)Solid);
		//if (status == PRO_TK_NO_ERROR) {
		//	AfxMessageBox(_T("The model has been erased in memory"));
		//}

		//在内存和硬盘中删除模型
		//status = ProMdlDelete ( (ProMdl)Solid );
		//ProError status;
		//CString File_Name;
		//ProPath Path;
		//ProSolid Solid;
		////模型的全路径名称
		//File_Name = Path_TK + _T("Components\\counter.prt");
		////UserFromCStringToProName(File_Name, Path);
		////检索模型
		//status = ProMdlRetrieve(Path, PRO_MDL_PART, (ProMdl*)&Solid);
		//if (status == PRO_TK_NO_ERROR) {
		//
		//	//create a window

		//};

	//获得模型的类型
	status = ProMdlTypeGet(Model, &mdl_type);
	if (status == PRO_TK_NO_ERROR) {

		if (mdl_type == PRO_MDL_ASSEMBLY || mdl_type == PRO_MDL_PART) {

			ProDimension* dim;
			int count = 0;
			CString name_dim, dimvalue;
			ProName dimName;
			double Dim_value = 0.0;

			status = UserGetDimensionOfSolid((ProSolid)Model, &dim);
			if (status == PRO_TK_NO_ERROR) {

				status = ProArraySizeGet(dim, &count);
				if (count > 0) {

					for (int i = 0; i < count; i++) {

						status = ProDimensionSymbolGet(dim + i, dimName);
						if (status == PRO_TK_NO_ERROR) {

							m_List_Dimension.AddString(UserFromProNameToCString(dimName));

							status = ProDimensionValueGet(dim + i, &Dim_value);
							if (status == PRO_TK_NO_ERROR) {
								//dimvalue.Format(_T("%.5f"), Dim_value);
								dimvalue = UserDoubleToCString(Dim_value, 5);
								m_List_DimensionValue.AddString(dimvalue);
							} else {
								m_List_DimensionValue.AddString(L"0.0");
							}
						}
					}
				}
				//释放
				ProArrayFree((ProArray*)&dim);
				dim = NULL;

				ProModelitem  modelitem;
				status = ProMdlToModelitem(Model, &modelitem);
				if (status == PRO_TK_NO_ERROR) {
					ProParameter* p_parameters;
					int count = 0;
					CString name_arameter;

					status = UserCollectParameters(&modelitem, &p_parameters);
					if (status == PRO_TK_NO_ERROR) {
						status = ProArraySizeGet(p_parameters, &count);
						if (count > 0) {
							for (int i = 0; i < count; i++) {
								name_arameter = UserFromProNameToCString(p_parameters[i].id);
								//AfxMessageBox(name_arameter);
								m_List_Para.AddString(name_arameter);
							}
						}

						ProArrayFree((ProArray*)&p_parameters);
						p_parameters = NULL;
					}
				}
			}
		} else {

			AfxMessageBox(_T("Can not Find the part -- 4_ES_ADDITION_INS_P_CJC000000.prt"));
		}

		UpdateData(false);
		return TRUE;
	}
}


void CDIgProject::OnLbnSelchangeList3() {
	// TODO: 在此添加控件通知处理程序代码
	m_List_Dimension.GetText(m_List_Dimension.GetCurSel(), m_Edit_DimensionName);
	m_List_DimensionValue.GetText(m_List_Dimension.GetCurSel(), m_Edit_Value);

	UpdateData(false);
}
