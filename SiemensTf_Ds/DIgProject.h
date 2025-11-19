#pragma once
#include "afxdialogex.h"


// CDIgProject 对话框

class CDIgProject : public CDialog
{
	DECLARE_DYNAMIC(CDIgProject)

public:
	CDIgProject(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDIgProject();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIgProject };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
