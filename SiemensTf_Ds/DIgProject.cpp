// DIgProject.cpp: 实现文件
//

#include "pch.h"
#include "SiemensTf_Ds.h"
#include "afxdialogex.h"
#include "DIgProject.h"


// CDIgProject 对话框

IMPLEMENT_DYNAMIC(CDIgProject, CDialog)

CDIgProject::CDIgProject(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIgProject, pParent), m_Edit_Value(_T("")), m_Edit_DimensionName(_T("")), m_Edit_Para_Value(_T("")), m_Edit_Para_name(_T("")) {

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
	DDX_Control(pDX, IDC_LIST2, m_List_name);
	DDX_Text(pDX, IDC_EDIT3, m_Edit_Para_Value);
	DDX_Control(pDX, IDC_EDIT3, m_Edit_Para);
	DDX_Text(pDX, IDC_EDIT4, m_Edit_Para_name);
}


BEGIN_MESSAGE_MAP(CDIgProject, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDOK, &CDIgProject::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDIgProject::OnBnClickedCancel)
	ON_WM_CLOSE()

	ON_LBN_SELCHANGE(IDC_LIST3, &CDIgProject::OnLbnSelchangeList3)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDIgProject::OnLbnSelchangeList1)
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
ProError UserCollectSolidAxis(ProSolid p_solid, ProAxis** p_axis);
ProError UserCollectSolidFeature(ProSolid p_solid, ProFeature** feature);
ProError UserFeatureDelete(ProSolid model, int id_feature);
ProError UserCollectSolidInAssembly(ProSolid p_solid, ProSolid** Comp_Solid);
CString GetParamValueByName(CString param_name, ProModelitem* modelitem);
bool UpdateParamByName(CString param_name, CString newParValue, ProModelitem* modelitem);


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

	//修改尺寸
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

	//修改参数
	ProMdl Model;
	status = ProMdlCurrentGet(&Model);
	if (status == PRO_TK_NO_ERROR) {

		ProModelitem  modelitem;
		status = ProMdlToModelitem(Model, &modelitem);

		bool result = UpdateParamByName(m_Edit_Para_name, m_Edit_Para_Value, &modelitem);
		
		status = ProSolidRegenerate(solid, PRO_REGEN_NO_FLAGS);

		status = ProWindowRepaint(PRO_VALUE_UNUSED);
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

	//获得模型的类型
	status = ProMdlTypeGet(Model, &mdl_type);
	if (status == PRO_TK_NO_ERROR) {

		if (mdl_type == PRO_MDL_ASSEMBLY || mdl_type == PRO_MDL_PART) {

			ProDimension* dim;
			int count = 0;
			CString name_dim, dimvalue;
			ProName dimName;
			double Dim_value = 0.0;

			//拿到尺寸
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


				//拿到参数
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
	}

	//删除特征代码测试
	status = ProMdlTypeGet(Model, &mdl_type);
	if (status == PRO_TK_NO_ERROR) {
		if (mdl_type == PRO_MDL_ASSEMBLY || mdl_type == PRO_MDL_PART) {
			ProFeature* feature;
			int count = 0;
			ProName name;

			status = UserCollectSolidFeature((ProSolid)Model, &feature);
			if (status == PRO_TK_NO_ERROR) {
				status = ProArraySizeGet(feature, &count);
				if (count > 0) {
					for (int i = 0; i < count; i++) {
						status = ProModelitemNameGet(&feature[i], name);
						if (status == PRO_TK_NO_ERROR) {
							if (UserFromProNameToCString(name) == "FEATURE_TEST") {
								status = UserFeatureDelete((ProSolid)Model, feature[i].id);
							}
						}
					}
					ProArrayFree((ProArray*)&feature);
					feature = NULL;
				}
			}
		}
	}

	//读取装配体中的装配体和零件到列表框中
	status = ProMdlTypeGet(Model, &mdl_type);
	if (status == PRO_TK_NO_ERROR) {
		if (mdl_type == PRO_MDL_ASSEMBLY /*|| mdl_type == PRO_MDL_PART*/) {
			ProSolid* comp_Solid;
			int count = 0;
			ProName name;
			ProMdlType comp_Solid_type;
			status = UserCollectSolidInAssembly((ProSolid)Model, &comp_Solid);
			//status = UserCollectSolidFeature((ProSolid)Model, &feature);
			CString name_asm_prt;
			if (status == PRO_TK_NO_ERROR) {
				status = ProArraySizeGet(comp_Solid, &count);
				if (count > 0) {
					for (int i = 0; i < count; i++) {

						status = ProMdlNameGet((ProMdl)comp_Solid[i], name);
						status = ProMdlTypeGet(comp_Solid[i], &comp_Solid_type);
						if (status == PRO_TK_NO_ERROR) {

							if (comp_Solid_type == PRO_MDL_ASSEMBLY) {

								name_asm_prt = UserFromProNameToCString(name) += _T(".asm");
							} else if (PRO_MDL_PART) {
								name_asm_prt = UserFromProNameToCString(name) += _T(".prt");
							}
							//AfxMessageBox(name_asm_prt);
							m_List_name.AddString(name_asm_prt);
							//name_asm_prt = UserFromProNameToCString(p_parameters[i].id);
							//AfxMessageBox(name_arameter);
							//m_List_Para.AddString(name_arameter);
						}
					}
					ProArrayFree((ProArray*)&comp_Solid);
					comp_Solid = NULL;
				}
			}
		}
	}


	//尝试拿到骨架模型
	ProMdlName name;
	ProMdl skeleton;
	//ProMdl Model;
	//ProError err;
	status = ProMdlCurrentGet(&Model);
	status = ProMdlTypeGet(Model, &mdl_type);
	if (status == PRO_TK_NO_ERROR) {
		if (mdl_type == PRO_MDL_ASSEMBLY) {

			status = ProAsmSkeletonGet((ProAssembly)Model, &skeleton);
			status = ProMdlNameGet(skeleton, name);
			AfxMessageBox(UserFromProNameToCString(name));
			//status = ProSolidMdlnameInit(name, (ProMdlfileType)model_type, &solid);
		}
	}

	UpdateData(false);
	return TRUE;
}


void CDIgProject::OnLbnSelchangeList3() {
	// TODO: 在此添加控件通知处理程序代码
	m_List_Dimension.GetText(m_List_Dimension.GetCurSel(), m_Edit_DimensionName);
	m_List_DimensionValue.GetText(m_List_Dimension.GetCurSel(), m_Edit_Value);

	UpdateData(false);
}

void CDIgProject::OnLbnSelchangeList1() {
	// TODO: 在此添加控件通知处理程序代码

	m_List_Para.GetText(m_List_Para.GetCurSel(), m_Edit_Para_name);

	ProMdl Model;
	ProModelitem  modelitem;
	ProError  status;
	status = ProMdlCurrentGet(&Model);
	status = ProMdlToModelitem(Model, &modelitem);
	if (status == PRO_TK_NO_ERROR) {
		CString paraName = GetParamValueByName(m_Edit_Para_name, &modelitem);
		m_Edit_Para_Value = paraName;
	}
	//m_List_DimensionValue.GetText(m_List_Dimension.GetCurSel(), m_Edit_Value);
	UpdateData(false);
}
