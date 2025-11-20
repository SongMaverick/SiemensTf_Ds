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

ProError ParameterFilterAction(ProParameter* p_object,
	ProAppData app_data) {
	ProError status;
	ProParamvalue value;
	ProUnititem units;

	status = ProParameterValueWithUnitsGet(p_object, &value, &units);
	if (status == PRO_TK_NO_ERROR) {
		//if (value.type == PRO_PARAM_BOOLEAN)
		//{
			//return (PRO_TK_CONTINUE);
		//}
		//else {
		return (PRO_TK_NO_ERROR);
		//}
	} else
		return (PRO_TK_CONTINUE);
}

ProError ParameterVisitAction(ProParameter* p_object, ProError status,
	ProAppData app_data) {
	ProArray* p_array;
	p_array = (ProArray*)((ProParameter**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_object);
	return (status);
}

ProError UserCollectParameters(
	ProModelitem* p_modelitem,   /* In:  The model item */
	ProParameter** p_parameters  /* Out: ProArray with collected parameters. */
) {
	ProError	    status;

	if (p_parameters != NULL) {
		status = ProArrayAlloc(0, sizeof(ProParameter), 1, (ProArray*)p_parameters);
		if (status == PRO_TK_NO_ERROR) {
			status = ProParameterVisit(p_modelitem, (ProParameterFilter)ParameterFilterAction,
				(ProParameterAction)ParameterVisitAction, (ProAppData)&p_parameters);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_parameters);
				*p_parameters = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

CString UserFromProNameToCString(ProName name) {
	CString Str;

	Str.Format(L"%s", name);

	return Str;
}
ProError DimensionVisitAction(ProDimension* p_object, ProError status,
	ProAppData app_data) {
	ProArray* p_array;
	p_array = (ProArray*)((ProDimension**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_object);
	return (status);
}

ProError DimensionFilterAction(ProDimension* p_object,
	ProAppData app_data) {
	ProBoolean rel_driven;
	ProDimensionIsReldriven(p_object, &rel_driven);
	if (rel_driven)
		return (PRO_TK_CONTINUE);
	else
		return (PRO_TK_NO_ERROR);
}

ProError  UserGetDimensionOfSolid(ProSolid solid, ProDimension** p_data) {
	ProError status;
	if (p_data != NULL) {
		status = ProArrayAlloc(0, sizeof(ProDimension), 1, (ProArray*)p_data);
		if (status == PRO_TK_NO_ERROR) {
			status = ProSolidDimensionVisit(solid, PRO_B_FALSE,
				(ProDimensionVisitAction)DimensionVisitAction,
				(ProDimensionFilterAction)DimensionFilterAction, (ProAppData)&p_data);
		} else {
			ProArrayFree((ProArray*)p_data);
			*p_data = NULL;
		}

	} else
		status = PRO_TK_BAD_INPUTS;
	return (status);

}

CString UserDoubleToCString(double value, int n) {
	if (n < 0) n = 1;   
	const bool isNegative = value < 0; 
	double absValue = fabs(value);     

	
	const double factor = pow(10.0, n);
	const double scaledValue = absValue * factor;
	double integerPart = floor(scaledValue);
	const double fractionalPart = scaledValue - integerPart;

	if (fractionalPart >= 0.1) {
		integerPart += 1;
	}

	double roundedValue = integerPart / factor;
	if (isNegative) roundedValue = -roundedValue;

	CString result;
	result.Format(_T("%.*f"), n, roundedValue);

	int dotPos = result.Find('.');
	if (dotPos != -1) {
		int lastNonZero = result.GetLength() - 1;
		while (lastNonZero > dotPos && result[lastNonZero] == '0') {
			lastNonZero--;
		}
		result = result.Left(lastNonZero + 1);

		
		if (result[lastNonZero] == '.') {
			result = result.Left(dotPos);
		}
	}

	return result;
}

ProError UserUpdateDimByName(ProDimension* dimension, CString Dim_name, double Dim_value) {
	ProError status;
	ProName DimName;
	int n;
	bool find = false;

	if (Dim_name.IsEmpty()) {
		return PRO_TK_E_NOT_FOUND;
	}

	ProArraySizeGet((ProArray)dimension, &n);
	for (int i = 0; i < n && !find; i++) {
		status = ProDimensionSymbolGet(dimension + i, DimName);
		if (status == PRO_TK_NO_ERROR) {
			if (UserFromProNameToCString(DimName) == Dim_name) {
				find = true;
				status = ProDimensionValueSet(dimension + i, Dim_value);
			}
		}
	}

	if (!find) {
		return PRO_TK_E_NOT_FOUND;
	}
	return status;
}
