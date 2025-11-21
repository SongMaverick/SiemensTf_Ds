#include "pch.h"
#include "SiemensTf_Ds.h"
/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/

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

ProError GeomitemVisitAction(ProGeomitem* p_handle, ProError status, ProAppData app_data) {
	ProArray* p_array;
	p_array = (ProArray*)((ProGeomitem**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_handle);

	return (status);
}

ProError UserCollectFeaturePoints(ProFeature* p_feature, ProGeomitem** p_geomitems) {
	ProError status;

	if (p_geomitems != NULL) {
		status = ProArrayAlloc(0, sizeof(ProGeomitem), 1, (ProArray*)p_geomitems);
		if (status == PRO_TK_NO_ERROR) {
			status = ProFeatureGeomitemVisit(p_feature, PRO_POINT,
				(ProGeomitemAction)GeomitemVisitAction, NULL, //
				(ProAppData)&p_geomitems);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_geomitems);
				*p_geomitems = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError UserCollectFeatureCurves(ProFeature* p_feature, ProGeomitem** p_geomitems) {
	ProError status;

	if (p_geomitems != NULL) {
		status = ProArrayAlloc(0, sizeof(ProGeomitem), 1, (ProArray*)p_geomitems);
		if (status == PRO_TK_NO_ERROR) {
			status = ProFeatureGeomitemVisit(p_feature, PRO_CURVE,
				(ProGeomitemAction)GeomitemVisitAction, NULL,
				(ProAppData)&p_geomitems);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_geomitems);
				*p_geomitems = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError UserCollectFeatureQuilt(ProFeature* Q_feature, ProGeomitem** Q_geomitems) {
	ProError status;

	if (Q_geomitems != NULL) {
		status = ProArrayAlloc(0, sizeof(ProGeomitem), 1, (ProArray*)Q_geomitems);
		if (status == PRO_TK_NO_ERROR) {
			status = ProFeatureGeomitemVisit(Q_feature, PRO_QUILT,
				(ProGeomitemAction)GeomitemVisitAction, NULL, (ProAppData)&Q_geomitems);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)Q_geomitems);
				*Q_geomitems = NULL;
				return (PRO_TK_E_NOT_FOUND);
			}
		}
	} else {
		return (PRO_TK_BAD_INPUTS);
	}

	return (PRO_TK_NO_ERROR);
}

ProError UserCollectFeatureCsys(ProFeature* p_feature, ProGeomitem** p_geomitems) {
	ProError status;

	if (p_geomitems != NULL) {
		status = ProArrayAlloc(0, sizeof(ProGeomitem), 1, (ProArray*)p_geomitems);
		if (status == PRO_TK_NO_ERROR) {
			status = ProFeatureGeomitemVisit(p_feature, PRO_CSYS,
				(ProGeomitemAction)GeomitemVisitAction, NULL,
				(ProAppData)&p_geomitems);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_geomitems);
				*p_geomitems = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError UserCollectFeatureAxis(ProFeature* p_feature, ProGeomitem** p_geomitems) {
	ProError status;

	if (p_geomitems != NULL) {
		status = ProArrayAlloc(0, sizeof(ProGeomitem), 1, (ProArray*)p_geomitems);
		if (status == PRO_TK_NO_ERROR) {
			status = ProFeatureGeomitemVisit(p_feature, PRO_AXIS,
				(ProGeomitemAction)GeomitemVisitAction, NULL,
				(ProAppData)&p_geomitems);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_geomitems);
				*p_geomitems = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError UserCollectFeatureSurface(ProFeature* p_feature, ProGeomitem** p_geomitems) {
	ProError status;

	if (p_geomitems != NULL) {
		status = ProArrayAlloc(0, sizeof(ProGeomitem), 1, (ProArray*)p_geomitems);
		if (status == PRO_TK_NO_ERROR) {
			status = ProFeatureGeomitemVisit(p_feature, PRO_SURFACE,
				(ProGeomitemAction)GeomitemVisitAction, NULL,
				(ProAppData)&p_geomitems);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_geomitems);
				*p_geomitems = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError PlaneSurfaceGeomFilterAction(ProGeomitem* p_geomitem, ProAppData app_data) {
	ProBoolean inactive;
	ProError status;
	ProSurface Surface;
	ProSrftype surface_type;
	ProSolid Solid;
	ProQuilt p_quilt;

	status = ProGeomitemIsInactive(p_geomitem, &inactive);
	if (inactive == PRO_B_TRUE)
		return PRO_TK_CONTINUE;

	status = ProModelitemMdlGet(p_geomitem, (ProMdl*)&Solid);
	status = ProGeomitemToSurface(p_geomitem, &Surface);
	if (status == PRO_TK_NO_ERROR) {
		status = ProSurfaceTypeGet(Surface, &surface_type);
		if (status == PRO_TK_NO_ERROR && surface_type == PRO_SRF_PLANE) {
			status = ProSurfaceQuiltGet(Solid, Surface, &p_quilt);
			if (status == PRO_TK_NO_ERROR) // �ų�����
			{
				return PRO_TK_CONTINUE;
			} else {
				return PRO_TK_NO_ERROR;
			}
		}
	}

	return PRO_TK_CONTINUE;
}

ProError UserCollectFeaturePlaneSurfaces(ProFeature* p_feature, ProGeomitem** p_geomitems) {
	ProError status;

	if (p_geomitems != NULL) {
		status = ProArrayAlloc(0, sizeof(ProGeomitem), 1, (ProArray*)p_geomitems);
		if (status == PRO_TK_NO_ERROR) {
			status = ProFeatureGeomitemVisit(p_feature, PRO_SURFACE,
				(ProGeomitemAction)GeomitemVisitAction, (ProGeomitemFilter)PlaneSurfaceGeomFilterAction,
				(ProAppData)&p_geomitems);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_geomitems);
				*p_geomitems = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError PointVisitAction(ProFeature* p_feature, ProError status, ProAppData app_data) {
	ProArray* p_array;
	p_array = (ProArray*)((ProGeomitem**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_feature);
	return (status);
}

ProError QuiltVisitAction(ProFeature* p_feature, ProError status, ProAppData app_data) {
	ProArray* p_array;
	p_array = (ProArray*)((ProGeomitem**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_feature);
	return (status);
}

ProError CsysVisitAction(ProFeature* p_feature, ProError status, ProAppData app_data) {
	ProArray* p_array;
	p_array = (ProArray*)((ProGeomitem**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_feature);
	return (status);
}

ProError AxisVisitAction(ProFeature* p_feature, ProError status, ProAppData app_data) {
	ProArray* p_array;
	p_array = (ProArray*)((ProGeomitem**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_feature);
	return (status);
}

ProError SurfaceVisitAction(ProSurface* p_surface, ProError status, ProAppData app_data) {
	ProArray* p_array;
	p_array = (ProArray*)((ProSurface**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_surface);
	return (status);
}

ProError ProUtilCollect2ParamDBVisitAction(void* p_object, ProAppData app_data) {
	ProError status;
	ProArray* p_array;

	p_array = (ProArray*)((void**)app_data)[0];

	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_object);
	return (status);
}

ProError UserCollectObjectsVisitAction(void* p_object, ProError status, ProAppData app_data) {
	return (ProUtilCollect2ParamDBVisitAction((void*)&p_object, app_data));
}

ProError UserCollectSolidAxis(ProSolid p_solid, ProAxis** p_axis) {
	ProError status;

	if (p_axis != NULL) {
		status = ProArrayAlloc(0, sizeof(ProAxis), 1, (ProArray*)p_axis);

		if (status == PRO_TK_NO_ERROR) {
			status = ProSolidAxisVisit(p_solid,
				(ProAxisVisitAction)UserCollectObjectsVisitAction, NULL, (ProAppData)&p_axis);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_axis);
				*p_axis = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError UserCollectQuiltSurfaces(ProQuilt p_quilt, ProSurface** p_surfaces) {
	ProError status;

	if (p_surfaces != NULL) {
		status = ProArrayAlloc(0, sizeof(ProSurface), 1, (ProArray*)p_surfaces);
		if (status == PRO_TK_NO_ERROR) {
			status = ProQuiltSurfaceVisit(p_quilt,
				(ProQuiltSurfaceVisitAction)UserCollectObjectsVisitAction, NULL,
				(ProAppData)&p_surfaces);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_surfaces);
				*p_surfaces = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError UserCollectSolidQuilts(ProSolid p_solid, ProQuilt** p_quilts) {
	ProError status;

	if (p_quilts != NULL) {
		status = ProArrayAlloc(0, sizeof(ProQuilt), 1, (ProArray*)p_quilts);

		if (status == PRO_TK_NO_ERROR) {
			status = ProSolidQuiltVisit(p_solid,
				(ProQuiltVisitAction)UserCollectObjectsVisitAction, NULL,
				(ProAppData)&p_quilts);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_quilts);
				*p_quilts = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError ProCollectSolidSurfaces(ProSolid p_solid, ProSurface** p_surfaces) {
	ProError status;

	if (p_surfaces != NULL) {
		status = ProArrayAlloc(0, sizeof(ProSurface), 1, (ProArray*)p_surfaces);

		if (status == PRO_TK_NO_ERROR) {
			status = ProSolidSurfaceVisit(p_solid,
				(ProSurfaceVisitAction)UserCollectObjectsVisitAction, NULL,
				(ProAppData)&p_surfaces);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_surfaces);
				*p_surfaces = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError PlaneSurfaceFilter(ProSurface p_object, ProAppData app_data) {
	ProError status;
	ProSrftype surface_type;
	double SurfaceArea = 0.0;

	status = ProSurfaceTypeGet(p_object, &surface_type);
	if (status == PRO_TK_NO_ERROR) {
		if (surface_type != PRO_SRF_PLANE)
			return (PRO_TK_CONTINUE);
		else {
			status = ProSurfaceAreaEval(p_object, &SurfaceArea);
			if (status == PRO_TK_NO_ERROR) {
				if (SurfaceArea > 0.000001)
					return (PRO_TK_NO_ERROR);
				else
					return (PRO_TK_CONTINUE);
			} else
				return (PRO_TK_CONTINUE);
		}
	} else
		return (PRO_TK_CONTINUE);
}

ProError UserCollectSolidPlaneSurfaces(ProSolid p_solid, ProSurface** p_surfaces) {
	ProError status;

	if (p_surfaces != NULL) {
		status = ProArrayAlloc(0, sizeof(ProSurface), 1, (ProArray*)p_surfaces);
		if (status == PRO_TK_NO_ERROR) {
			status = ProSolidSurfaceVisit(p_solid,
				(ProSurfaceVisitAction)UserCollectObjectsVisitAction,
				(ProSurfaceFilterAction)PlaneSurfaceFilter,
				(ProAppData)&p_surfaces);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)p_surfaces);
				*p_surfaces = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError ComponentFilterAction(ProFeature* p_feature, ProAppData app_data) {
	ProError status;
	ProFeattype feat_type;
	ProFeatStatus feat_status;

	status = ProFeatureTypeGet(p_feature, &feat_type);
	status = ProFeatureStatusGet(p_feature, &feat_status);

	if (feat_type != PRO_FEAT_COMPONENT || feat_status == PRO_FEAT_SUPPRESSED //|| feat_status != PRO_FEAT_ACTIVE
		|| feat_status == PRO_FEAT_FAMTAB_SUPPRESSED || feat_status == PRO_FEAT_PROG_SUPPRESSED)
		return (PRO_TK_CONTINUE);

	return (PRO_TK_NO_ERROR);
}
ProError PartVisitAction(ProFeature* p_feature, ProError err, ProAppData app_data) {
	ProError status;
	ProMdlfileType mdl_type;
	ProFamilyMdlName mdl_name;
	ProSolid mdl;
	ProSolid** Components = (ProSolid**)app_data;

	status = ProAsmcompMdlMdlnameGet(p_feature, &mdl_type, mdl_name);
	if (status == PRO_TK_NO_ERROR) {
		status = ProAsmcompMdlGet(p_feature, (ProMdl*)&mdl);
		if (mdl_type == PRO_MDLFILE_ASSEMBLY) {
			status = ProSolidFeatVisit(mdl, PartVisitAction, ComponentFilterAction, app_data);
		} else if (mdl_type == PRO_MDLFILE_PART) {
			status = ProArrayObjectAdd((ProArray*)Components, PRO_VALUE_UNUSED, 1, &mdl);
		}
	}
	return (PRO_TK_NO_ERROR);
}

ProError AsmVisitAction(ProFeature* p_feature, ProError err, ProAppData app_data) {
	ProError status;
	ProMdlfileType mdl_type;
	ProFamilyMdlName mdl_name;
	ProSolid mdl;
	ProSolid** Components = (ProSolid**)app_data;

	status = ProAsmcompMdlMdlnameGet(p_feature, &mdl_type, mdl_name);
	if (status == PRO_TK_NO_ERROR) {
		status = ProAsmcompMdlGet(p_feature, (ProMdl*)&mdl);
		if (mdl_type == PRO_MDLFILE_ASSEMBLY) {
			status = ProArrayObjectAdd((ProArray*)Components, PRO_VALUE_UNUSED, 1, &mdl);
			status = ProSolidFeatVisit(mdl, AsmVisitAction, ComponentFilterAction, app_data);
		}
	}
	return (PRO_TK_NO_ERROR);
}

ProError UserAsmAndPartVisitAction(ProFeature* p_feature, ProError err, ProAppData app_data) {
	ProError status;
	ProMdlfileType mdl_type;
	ProFamilyMdlName mdl_name;
	ProSolid mdl;
	ProSolid** Components = (ProSolid**)app_data;

	status = ProAsmcompMdlMdlnameGet(p_feature, &mdl_type, mdl_name);
	if (status == PRO_TK_NO_ERROR) {
		status = ProAsmcompMdlGet(p_feature, (ProMdl*)&mdl);
		if (mdl_type == PRO_MDLFILE_ASSEMBLY) {
			status = ProArrayObjectAdd((ProArray*)Components, PRO_VALUE_UNUSED, 1, &mdl);
			status = ProSolidFeatVisit(mdl, UserAsmAndPartVisitAction, ComponentFilterAction, app_data);
		} else if (mdl_type == PRO_MDLFILE_PART) {
			status = ProArrayObjectAdd((ProArray*)Components, PRO_VALUE_UNUSED, 1, &mdl);
		}
	}
	return (PRO_TK_NO_ERROR);
}

ProError FeatureVisitAction(ProFeature* p_feature, ProError err, ProAppData app_data) {
	ProError status;

	status = PRO_TK_CONTINUE;

	ProFeature** Components = (ProFeature**)app_data;

	status = ProArrayObjectAdd((ProArray*)Components, PRO_VALUE_UNUSED, 1, p_feature);

	return (PRO_TK_NO_ERROR);
}

ProError PartFilterAction(ProFeature* p_feature, ProAppData app_data) {
	ProError status;
	ProFeattype feat_type;
	ProFeatStatus feat_status;

	status = ProFeatureTypeGet(p_feature, &feat_type);
	status = ProFeatureStatusGet(p_feature, &feat_status);

	if (feat_type != PRO_FEAT_COMPONENT || feat_status == PRO_FEAT_SUPPRESSED //|| feat_status != PRO_FEAT_ACTIVE
		|| feat_status == PRO_FEAT_FAMTAB_SUPPRESSED || feat_status == PRO_FEAT_PROG_SUPPRESSED)
		return (PRO_TK_CONTINUE);

	return (PRO_TK_NO_ERROR);
}

ProError FeatureFilterAction(ProFeature* p_feature, ProAppData app_data) {
	ProError status;
	ProFeattype feat_type;
	ProFeatStatus feat_status;

	status = ProFeatureTypeGet(p_feature, &feat_type);
	status = ProFeatureStatusGet(p_feature, &feat_status);

	if (feat_type == PRO_FEAT_COMPONENT || feat_status == PRO_FEAT_SUPPRESSED //|| feat_status != PRO_FEAT_ACTIVE
		|| feat_status == PRO_FEAT_FAMTAB_SUPPRESSED || feat_status == PRO_FEAT_PROG_SUPPRESSED)
		return (PRO_TK_CONTINUE);
	else
		return (PRO_TK_NO_ERROR);
}

ProError FeatureVisitFilterAction(ProFeature* p_feature, ProAppData app_data) {
	ProError status;
	ProFeatStatus feat_status;

	status = ProFeatureStatusGet(p_feature, &feat_status);
	if (feat_status == PRO_FEAT_SUPPRESSED || feat_status == PRO_FEAT_FAMTAB_SUPPRESSED || feat_status == PRO_FEAT_PROG_SUPPRESSED)
		return (PRO_TK_CONTINUE);
	else
		return (PRO_TK_NO_ERROR);
}

ProError CsysFeatureFilterAction(ProFeature* p_feature, ProAppData app_data) {
	ProError status;
	ProFeattype feat_type;
	ProFeatStatus feat_status;

	status = ProFeatureTypeGet(p_feature, &feat_type);
	status = ProFeatureStatusGet(p_feature, &feat_status);

	if (feat_type != PRO_FEAT_CSYS || feat_status == PRO_FEAT_SUPPRESSED //|| feat_status != PRO_FEAT_ACTIVE
		|| feat_status == PRO_FEAT_FAMTAB_SUPPRESSED || feat_status == PRO_FEAT_PROG_SUPPRESSED)
		return (PRO_TK_CONTINUE);
	else
		return (PRO_TK_NO_ERROR);
}

ProError PointFeatureFilterAction(ProFeature* p_feature, ProAppData app_data) {
	ProError status;
	ProFeattype feat_type;
	ProFeatStatus feat_status;

	status = ProFeatureTypeGet(p_feature, &feat_type);
	status = ProFeatureStatusGet(p_feature, &feat_status);

	if ((feat_type != PRO_FEAT_DATUM_POINT && feat_type != PRO_FEAT_CURVE) || feat_status == PRO_FEAT_SUPPRESSED || feat_status == PRO_FEAT_FAMTAB_SUPPRESSED || feat_status == PRO_FEAT_PROG_SUPPRESSED)
		return (PRO_TK_CONTINUE);
	else
		return (PRO_TK_NO_ERROR);
}

ProError UserCollectSolidInAssembly(ProSolid p_solid, ProSolid** Comp_Solid) {
	ProError status;

	if (Comp_Solid != NULL) {
		status = ProArrayAlloc(0, sizeof(ProSolid), 1, (ProArray*)Comp_Solid);
		if (status == PRO_TK_NO_ERROR) {
			// ���װ���Ԫ��ʵ��
			status = ProSolidFeatVisit(p_solid, UserAsmAndPartVisitAction,
				ComponentFilterAction, Comp_Solid);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)Comp_Solid);
				*Comp_Solid = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError UserCollectPartInAssembly(ProSolid p_solid, ProSolid** Comp_Solid) {
	ProError status;

	if (Comp_Solid != NULL) {
		status = ProArrayAlloc(0, sizeof(ProSolid), 1, (ProArray*)Comp_Solid);
		if (status == PRO_TK_NO_ERROR) {
			// ���װ�����ʵ��
			status = ProSolidFeatVisit(p_solid, PartVisitAction,
				ComponentFilterAction, Comp_Solid);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)Comp_Solid);
				*Comp_Solid = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError UserCollectAsmInAssembly(ProSolid p_solid, ProSolid** Comp_Solid) {
	ProError status;

	if (Comp_Solid != NULL) {
		status = ProArrayAlloc(0, sizeof(ProSolid), 1, (ProArray*)Comp_Solid);
		if (status == PRO_TK_NO_ERROR) {
			// ���װ�����ʵ��
			status = ProSolidFeatVisit(p_solid, AsmVisitAction,
				ComponentFilterAction, Comp_Solid);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)Comp_Solid);
				*Comp_Solid = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

ProError UserFeatureDelete(ProSolid model, int id_feature) {
	ProError status = PRO_TK_GENERAL_ERROR;
	ProFeature feature;
	ProFeatStatus feat_status;

	if (id_feature < 1) return status;

	ProFeatureDeleteOptions* deleteOptions;
	status = ProArrayAlloc(1, sizeof(ProFeatureDeleteOptions), 1, (ProArray*)&deleteOptions);
	deleteOptions[0] = PRO_FEAT_DELETE_CLIP;

	int* feat_ids = NULL;
	status = ProArrayAlloc(1, sizeof(int), 1, (ProArray*)&feat_ids);
	feat_ids[0] = id_feature;

	status = ProFeatureInit(model, id_feature, &feature);
	if (status == PRO_TK_NO_ERROR) {
		status = ProFeatureWithoptionsDelete(model, feat_ids, deleteOptions, PRO_REGEN_NO_FLAGS);
	}

	ProArrayFree((ProArray*)&deleteOptions);
	ProArrayFree((ProArray*)&feat_ids);

	ProWindowRepaint(PRO_VALUE_UNUSED);

	return status;
}

ProError UserFeatureSuppress(ProSolid model, int id_feature) {
	ProError status = PRO_TK_GENERAL_ERROR;
	ProFeature feature;
	ProFeatStatus feat_status;

	if (id_feature < 1) return status;

	ProFeatureDeleteOptions* deleteOptions;
	status = ProArrayAlloc(1, sizeof(ProFeatureDeleteOptions), 1, (ProArray*)&deleteOptions);
	deleteOptions[0] = PRO_FEAT_DELETE_CLIP;

	int* feat_ids = NULL;
	status = ProArrayAlloc(1, sizeof(int), 1, (ProArray*)&feat_ids);
	feat_ids[0] = id_feature;

	status = ProFeatureInit(model, id_feature, &feature);
	if (status == PRO_TK_NO_ERROR) {
		status = ProFeatureStatusGet(&feature, &feat_status);
		if (feat_status != PRO_FEAT_SUPPRESSED && feat_status != PRO_FEAT_PROG_SUPPRESSED &&
			feat_status != PRO_FEAT_FAMTAB_SUPPRESSED) {
			status = ProFeatureWithoptionsSuppress(model, feat_ids, deleteOptions, PRO_REGEN_NO_FLAGS);
		}
	}

	ProArrayFree((ProArray*)&deleteOptions);
	ProArrayFree((ProArray*)&feat_ids);

	ProWindowRepaint(PRO_VALUE_UNUSED);

	return status;
}

ProError UserFeatureResume(ProSolid model, int id_feature, bool Resume_children) {
	ProError status = PRO_TK_GENERAL_ERROR;
	ProFeature feature;
	ProFeatStatus feat_status;
	ProBoolean  is_incomplete = PRO_B_FALSE;

	if (id_feature < 1) return status;

	ProFeatureResumeOptions* resume_options;
	status = ProArrayAlloc(1, sizeof(ProFeatureResumeOptions),
		1, (ProArray*)&resume_options);
	resume_options[0] = PRO_FEAT_RESUME_INCLUDE_PARENTS;

	int* feat_ids = NULL;
	status = ProArrayAlloc(1, sizeof(int), 1, (ProArray*)&feat_ids);
	feat_ids[0] = id_feature;

	status = ProFeatureInit(model, id_feature, &feature);
	if (status == PRO_TK_NO_ERROR) {
		status = ProFeatureIsIncomplete(&feature, &is_incomplete);
		status = ProFeatureStatusGet(&feature, &feat_status);
		if ((feat_status != PRO_FEAT_SUPPRESSED && feat_status != PRO_FEAT_PROG_SUPPRESSED
			&& feat_status != PRO_FEAT_FAMTAB_SUPPRESSED) || is_incomplete == PRO_B_TRUE) {

			ProArrayFree((ProArray*)&feat_ids);
			ProArrayFree((ProArray*)&resume_options);
			return PRO_TK_GENERAL_ERROR;
		}

		status = ProFeatureWithoptionsResume(model, feat_ids, resume_options, PRO_REGEN_NO_FLAGS);
		if (Resume_children) {
			int* children_ids, n_children;
			status = ProFeatureChildrenGet(&feature, &children_ids, &n_children);
			if (n_children > 0) {
				status = ProFeatureWithoptionsResume(model, children_ids, resume_options, PRO_REGEN_NO_FLAGS);
			}
			status = ProArrayFree((ProArray*)&children_ids);
		}
	}
	ProArrayFree((ProArray*)&feat_ids);
	ProArrayFree((ProArray*)&resume_options);

	ProWindowRepaint(PRO_VALUE_UNUSED);
	return status;
}

ProError UserCollectSolidFeature(ProSolid p_solid, ProFeature** feature) {
	ProError  status;

	if (feature != NULL) {
		status = ProArrayAlloc(0, sizeof(ProFeature), 1, (ProArray*)feature);
		if (status == PRO_TK_NO_ERROR) {
			//���ʵ������������
			status = ProSolidFeatVisit(p_solid, FeatureVisitAction,
				FeatureVisitFilterAction, feature);
			if (status != PRO_TK_NO_ERROR) {
				ProArrayFree((ProArray*)feature);
				*feature = NULL;
			}
		}
	} else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}


