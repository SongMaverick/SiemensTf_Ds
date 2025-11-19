#include "pch.h"
#include "SiemensTf_Ds.h"
/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include <ProUtil.h>
#include <ProWindows.h>

extern CString Path_SettingDir;

ProError UsrActivateCurrentWindow() {
	int window_id;
	ProError status;
	status = ProWindowCurrentGet(&window_id);
	status = ProWindowRepaint(window_id);
	status = ProWindowActivate(window_id);
	return status;
}

ProError UserRefreshCurrentWindow() {
	ProError status;
	int window_id;

	status = ProWindowCurrentGet(&window_id);
	status = ProWindowRefresh(window_id);

	return status;
}

bool FolderExist(CString strPath) {
	WIN32_FIND_DATA wfd;
	bool rValue = false;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
		rValue = true;
	}
	FindClose(hFind);
	return rValue;
}

// CStringתProPath (PRO_PATH_SIZE = 256)
bool ConvertCStringToProPath(const CString& src, ProPath dest) {
	if (src.IsEmpty()) {
		dest[0] = L'\0';
		return true;
	}

	// ��ȡԴ�ַ�������(��������ֹ��)
	int srcLen = src.GetLength();

	// ����Ƿ񳬹�ProPath��󳤶�
	if (srcLen >= PRO_PATH_SIZE) {
		// �ضϴ����򷵻ش��󣬸�������ѡ��
		wcscpy_s(dest, PRO_PATH_SIZE, src.Left(PRO_PATH_SIZE - 1));
		return false;
	}

	// �����ַ���
	wcscpy_s(dest, PRO_PATH_SIZE, src);
	return true;
}

// CStringתProName (PRO_NAME_SIZE = 32)
bool ConvertCStringToProName(const CString& src, ProName dest) {
	if (src.IsEmpty()) {
		dest[0] = L'\0';
		return true;
	}

	// ��ȡԴ�ַ�������(��������ֹ��)
	int srcLen = src.GetLength();

	// ����Ƿ񳬹�ProName��󳤶�
	if (srcLen >= PRO_NAME_SIZE) {
		// �ضϴ����򷵻ش���
		wcscpy_s(dest, PRO_NAME_SIZE, src.Left(PRO_NAME_SIZE - 1));
		return false;
	}

	// �����ַ���
	wcscpy_s(dest, PRO_NAME_SIZE, src);
	return true;
}


// CStringתProLine (PRO_LINE_SIZE = 81)
bool ConvertCStringToProLine(const CString& src, ProLine dest) {
	if (src.IsEmpty()) {
		dest[0] = L'\0';
		return true;
	}

	// ��ȡԴ�ַ�������(��������ֹ��)
	int srcLen = src.GetLength();

	// ����Ƿ񳬹�ProLine��󳤶�
	if (srcLen >= PRO_LINE_SIZE) {
		// �ضϴ����򷵻ش���
		wcscpy_s(dest, PRO_LINE_SIZE, src.Left(PRO_LINE_SIZE - 1));
		return false;
	}

	// �����ַ���
	wcscpy_s(dest, PRO_LINE_SIZE, src);
	return true;
}

CString UserFromProPathToCString(ProPath Path) {
	CString Str;

	Str.Format(L"%s", Path);

	return Str;
}

CString UserGetCurrentDirectory() {
	CString CurrentDir;
	ProError status;
	ProPath cur_path;
	status = ProDirectoryCurrentGet(cur_path);
	if (status == PRO_TK_NO_ERROR) {
		CurrentDir = UserFromProPathToCString(cur_path);
	} else {
		CurrentDir.Empty();
	}
	return CurrentDir;
}

void UserRecordCurrentDirectory() {
	bool Is_Change = false;
	CString FileData, ItemData, CurrentDir;

	CurrentDir = UserGetCurrentDirectory();
	//FileData = UserGetDataFromTxtFile(Path_SettingDir + _T("WorkingDirectory.txt"));

	//if (!FileData.IsEmpty()) {
		//ItemData = UserReadDataFromString(FileData, 1, 1);
		//if (ItemData != CurrentDir)
		//{
	Is_Change = true;
	//}
//}
//else {
	//Is_Change = true;
//}
//FileData.Empty();

	if (Is_Change) {
		CString Data_WriteFile;

		Data_WriteFile = CurrentDir + _T("   Last_Working_Directory");
		Data_WriteFile += _T("\r\n");

		CFile Setting_File;
		if (Setting_File.Open(Path_SettingDir + L"WorkingDirectory.txt", CFile::modeCreate | CFile::modeWrite)) {
			Setting_File.Seek(0, CFile::begin);

			//д���ļ����ı�������ANSI
			CStringA utf8String = (CStringA)Data_WriteFile;
			Setting_File.Write((LPCSTR)utf8String, utf8String.GetLength());
		}
		Setting_File.Close();
	}
}
