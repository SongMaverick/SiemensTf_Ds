#pragma once
#include "afxdialogex.h"


// IDD_DIALOG2 对话框

class IDD_DIALOG2 : public CDialogEx
{
	DECLARE_DYNAMIC(IDD_DIALOG2)

public:
	IDD_DIALOG2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~IDD_DIALOG2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	IDD_DIALOG2() = default;
};
