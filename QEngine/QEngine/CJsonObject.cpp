/*******************************************************************************
 * Project:  neb
 * @file     CJsonObject.cpp
 * @brief
 * @author   bwarliao
 * @date:    2014-7-16
 * @note
 * Modify history:
 ******************************************************************************/

#include "CJsonObject.hpp"

#ifdef _WIN32
#define snprintf _snprintf_s
#endif

namespace neb
{

	CJsonObject::CJsonObject()
		: m_pJsonData(NULL), m_pExternJsonDataRef(NULL), m_pKeyTravers(NULL)
	{
		// m_pJsonData = cJSON_CreateObject();  
	}

	CJsonObject::CJsonObject(const std::string& strJson)
		: m_pJsonData(NULL), m_pExternJsonDataRef(NULL), m_pKeyTravers(NULL)
	{
		Parse(strJson);
	}

	CJsonObject::CJsonObject(const CJsonObject* pJsonObject)
		: m_pJsonData(NULL), m_pExternJsonDataRef(NULL), m_pKeyTravers(NULL)
	{
		if (pJsonObject)
		{
			Parse(pJsonObject->ToString());
		}
	}

	CJsonObject::CJsonObject(const CJsonObject& oJsonObject)
		: m_pJsonData(NULL), m_pExternJsonDataRef(NULL), m_pKeyTravers(NULL)
	{
		Parse(oJsonObject.ToString());
	}

	CJsonObject::~CJsonObject()
	{
		Clear();
	}

	CJsonObject& CJsonObject::operator=(const CJsonObject& oJsonObject)
	{
		Parse(oJsonObject.ToString().c_str());
		return(*this);
	}

	bool CJsonObject::operator==(const CJsonObject& oJsonObject) const
	{
		return(this->ToString() == oJsonObject.ToString());
	}

	bool CJsonObject::GetKey(std::string& strKey)
	{
		if (IsArray())
		{
			return(false);
		}
		if (m_pKeyTravers == NULL)
		{
			if (m_pJsonData != NULL)
			{
				m_pKeyTravers = m_pJsonData;
			}
			else if (m_pExternJsonDataRef != NULL)
			{
				m_pKeyTravers = m_pExternJsonDataRef;
			}
			return(false);
		}
		else if (m_pKeyTravers == m_pJsonData || m_pKeyTravers == m_pExternJsonDataRef)
		{
			cJSON *c = m_pKeyTravers->child;
			if (c)
			{
				strKey = c->string;
				m_pKeyTravers = c->next;
				return(true);
			}
			else
			{
				return(false);
			}
		}
		else
		{
			strKey = m_pKeyTravers->string;
			m_pKeyTravers = m_pKeyTravers->next;
			return(true);
		}
	}

	bool CJsonObject::HasKey(const std::string & strKey)
	{
		if (IsArray()) {
			return(false);
		}
		if (m_pKeyTravers == NULL) {
			if (m_pJsonData != NULL)
			{
				m_pKeyTravers = m_pJsonData;
			}
			else if (m_pExternJsonDataRef != NULL)
			{
				m_pKeyTravers = m_pExternJsonDataRef;
			}
		}

		if (m_pKeyTravers == m_pJsonData || m_pKeyTravers == m_pExternJsonDataRef) {
			cJSON *c = m_pKeyTravers->child;
			while (c) {
				if (std::string(c->string) == strKey) {
					return true;
				}
				c = c->next;
			}
			return false;
		}
		
		return false;
	}

	void CJsonObject::ResetTraversing()
	{
		if (m_pJsonData != NULL)
		{
			m_pKeyTravers = m_pJsonData;
		}
		else
		{
			m_pKeyTravers = m_pExternJsonDataRef;
		}
	}

	CJsonObject& CJsonObject::operator[](const std::string& strKey)
	{
		std::map<std::string, CJsonObject*>::iterator iter;
		iter = m_mapJsonObjectRef.find(strKey);
		if (iter == m_mapJsonObjectRef.end())
		{
			cJSON* pJsonStruct = NULL;
			if (m_pJsonData != NULL)
			{
				if (m_pJsonData->type == cJSON_Object)
				{
					pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
				}
			}
			else if (m_pExternJsonDataRef != NULL)
			{
				if (m_pExternJsonDataRef->type == cJSON_Object)
				{
					pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
				}
			}
			if (pJsonStruct == NULL)
			{
				CJsonObject* pJsonObject = new CJsonObject();
				m_mapJsonObjectRef.insert(std::pair<std::string, CJsonObject*>(strKey, pJsonObject));
				return(*pJsonObject);
			}
			else
			{
				CJsonObject* pJsonObject = new CJsonObject(pJsonStruct);
				m_mapJsonObjectRef.insert(std::pair<std::string, CJsonObject*>(strKey, pJsonObject));
				return(*pJsonObject);
			}
		}
		else
		{
			return(*(iter->second));
		}
	}

	CJsonObject& CJsonObject::operator[](unsigned int uiWhich)
	{
		std::map<unsigned int, CJsonObject*>::iterator iter;
		iter = m_mapJsonArrayRef.find(uiWhich);
		if (iter == m_mapJsonArrayRef.end())
		{
			cJSON* pJsonStruct = NULL;
			if (m_pJsonData != NULL)
			{
				if (m_pJsonData->type == cJSON_Array)
				{
					pJsonStruct = cJSON_GetArrayItem(m_pJsonData, uiWhich);
				}
			}
			else if (m_pExternJsonDataRef != NULL)
			{
				if (m_pExternJsonDataRef->type == cJSON_Array)
				{
					pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, uiWhich);
				}
			}
			if (pJsonStruct == NULL)
			{
				CJsonObject* pJsonObject = new CJsonObject();
				m_mapJsonArrayRef.insert(std::pair<unsigned int, CJsonObject*>(uiWhich, pJsonObject));
				return(*pJsonObject);
			}
			else
			{
				CJsonObject* pJsonObject = new CJsonObject(pJsonStruct);
				m_mapJsonArrayRef.insert(std::pair<unsigned int, CJsonObject*>(uiWhich, pJsonObject));
				return(*pJsonObject);
			}
		}
		else
		{
			return(*(iter->second));
		}
	}

	std::string CJsonObject::operator()(const std::string& strKey) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
			}
		}
		if (pJsonStruct == NULL)
		{
			return(std::string(""));
		}
		if (pJsonStruct->type == cJSON_String)
		{
			return(pJsonStruct->valuestring);
		}
		else if (pJsonStruct->type == cJSON_Int)
		{
			char szNumber[128] = { 0 };
			if (pJsonStruct->sign == -1)
			{
				if (pJsonStruct->valueint <= (int64)INT_MAX && (int64)pJsonStruct->valueint >= (int64)INT_MIN)
				{
					snprintf(szNumber, sizeof(szNumber), "%d", (int32)pJsonStruct->valueint);
				}
				else
				{
					snprintf(szNumber, sizeof(szNumber), "%ld", (int64)pJsonStruct->valueint);
				}
			}
			else
			{
				if ((uint64)pJsonStruct->valueint <= (uint64)UINT_MAX)
				{
					snprintf(szNumber, sizeof(szNumber), "%u", (uint32)pJsonStruct->valueint);
				}
				else
				{
					snprintf(szNumber, sizeof(szNumber), "%lu", pJsonStruct->valueint);
				}
			}
			return(std::string(szNumber));
		}
		else if (pJsonStruct->type == cJSON_Double)
		{
			char szNumber[128] = { 0 };
			if (fabs(pJsonStruct->valuedouble) < 1.0e-6 || fabs(pJsonStruct->valuedouble) > 1.0e9)
			{
				snprintf(szNumber, sizeof(szNumber), "%e", pJsonStruct->valuedouble);
			}
			else
			{
				snprintf(szNumber, sizeof(szNumber), "%f", pJsonStruct->valuedouble);
			}
			return(std::string(szNumber));
		}
		else if (pJsonStruct->type == cJSON_False)
		{
			return(std::string("false"));
		}
		else if (pJsonStruct->type == cJSON_True)
		{
			return(std::string("true"));
		}
		return(std::string(""));
	}

	std::string CJsonObject::operator()(unsigned int uiWhich) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pJsonData, uiWhich);
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, uiWhich);
			}
		}
		if (pJsonStruct == NULL)
		{
			return(std::string(""));
		}
		if (pJsonStruct->type == cJSON_String)
		{
			return(pJsonStruct->valuestring);
		}
		else if (pJsonStruct->type == cJSON_Int)
		{
			char szNumber[128] = { 0 };
			if (pJsonStruct->sign == -1)
			{
				if (pJsonStruct->valueint <= (int64)INT_MAX && (int64)pJsonStruct->valueint >= (int64)INT_MIN)
				{
					snprintf(szNumber, sizeof(szNumber), "%d", (int32)pJsonStruct->valueint);
				}
				else
				{
					snprintf(szNumber, sizeof(szNumber), "%ld", (int64)pJsonStruct->valueint);
				}
			}
			else
			{
				if ((uint64)pJsonStruct->valueint <= (uint64)UINT_MAX)
				{
					snprintf(szNumber, sizeof(szNumber), "%u", (uint32)pJsonStruct->valueint);
				}
				else
				{
					snprintf(szNumber, sizeof(szNumber), "%lu", pJsonStruct->valueint);
				}
			}
			return(std::string(szNumber));
		}
		else if (pJsonStruct->type == cJSON_Double)
		{
			char szNumber[128] = { 0 };
			if (fabs(pJsonStruct->valuedouble) < 1.0e-6 || fabs(pJsonStruct->valuedouble) > 1.0e9)
			{
				snprintf(szNumber, sizeof(szNumber), "%e", pJsonStruct->valuedouble);
			}
			else
			{
				snprintf(szNumber, sizeof(szNumber), "%f", pJsonStruct->valuedouble);
			}
			return(std::string(szNumber));
		}
		else if (pJsonStruct->type == cJSON_False)
		{
			return(std::string("false"));
		}
		else if (pJsonStruct->type == cJSON_True)
		{
			return(std::string("true"));
		}
		return(std::string(""));
	}

	bool CJsonObject::Parse(const std::string& strJson)
	{
		Clear();
		m_pJsonData = cJSON_Parse(strJson.c_str());
		m_pKeyTravers = m_pJsonData;
		if (m_pJsonData == NULL)
		{
			m_strErrMsg = std::string("prase json string error at ") + cJSON_GetErrorPtr();
			return(false);
		}
		return(true);
	}

	void CJsonObject::Clear()
	{
		m_pExternJsonDataRef = NULL;
		m_pKeyTravers = NULL;
		if (m_pJsonData != NULL)
		{
			cJSON_Delete(m_pJsonData);
			m_pJsonData = NULL;
		}
		for (std::map<unsigned int, CJsonObject*>::iterator iter = m_mapJsonArrayRef.begin();
			iter != m_mapJsonArrayRef.end(); ++iter)
		{
			if (iter->second != NULL)
			{
				delete (iter->second);
				iter->second = NULL;
			}
		}
		m_mapJsonArrayRef.clear();
		for (std::map<std::string, CJsonObject*>::iterator iter = m_mapJsonObjectRef.begin();
			iter != m_mapJsonObjectRef.end(); ++iter)
		{
			if (iter->second != NULL)
			{
				delete (iter->second);
				iter->second = NULL;
			}
		}
		m_mapJsonObjectRef.clear();
	}

	bool CJsonObject::IsEmpty() const
	{
		if (m_pJsonData != NULL)
		{
			return(false);
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			return(false);
		}
		return(true);
	}

	bool CJsonObject::IsArray() const
	{
		cJSON* pFocusData = NULL;
		if (m_pJsonData != NULL)
		{
			pFocusData = m_pJsonData;
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			pFocusData = m_pExternJsonDataRef;
		}

		if (pFocusData == NULL)
		{
			return(false);
		}

		if (pFocusData->type == cJSON_Array)
		{
			return(true);
		}
		else
		{
			return(false);
		}
	}

	std::string CJsonObject::ToString() const
	{
		char* pJsonString = NULL;
		std::string strJsonData = "";
		if (m_pJsonData != NULL)
		{
			pJsonString = cJSON_PrintUnformatted(m_pJsonData);
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			pJsonString = cJSON_PrintUnformatted(m_pExternJsonDataRef);
		}
		if (pJsonString != NULL)
		{
			strJsonData = pJsonString;
			free(pJsonString);
		}
		return(strJsonData);
	}

	std::string CJsonObject::ToFormattedString() const
	{
		char* pJsonString = NULL;
		std::string strJsonData = "";
		if (m_pJsonData != NULL)
		{
			pJsonString = cJSON_Print(m_pJsonData);
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			pJsonString = cJSON_Print(m_pExternJsonDataRef);
		}
		if (pJsonString != NULL)
		{
			strJsonData = pJsonString;
			free(pJsonString);
		}
		return(strJsonData);
	}


	bool CJsonObject::Get(const std::string& strKey, CJsonObject& oJsonObject) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		char* pJsonString = cJSON_Print(pJsonStruct);
		std::string strJsonData = pJsonString;
		free(pJsonString);
		if (oJsonObject.Parse(strJsonData))
		{
			return(true);
		}
		else
		{
			return(false);
		}
	}

	bool CJsonObject::Get(const std::string& strKey, std::string& strValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type != cJSON_String)
		{
			return(false);
		}
		strValue = pJsonStruct->valuestring;
		return(true);
	}

	bool CJsonObject::Get(const std::string& strKey, int32& iValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Int)
		{
			iValue = (int32)(pJsonStruct->valueint);
			return(true);
		}
		else if (pJsonStruct->type == cJSON_Double)
		{
			iValue = (int32)(pJsonStruct->valuedouble);
			return(true);
		}
		return(false);
	}

	bool CJsonObject::Get(const std::string& strKey, uint32& uiValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Int)
		{
			uiValue = (uint32)(pJsonStruct->valueint);
			return(true);
		}
		else if (pJsonStruct->type == cJSON_Double)
		{
			uiValue = (uint32)(pJsonStruct->valuedouble);
			return(true);
		}
		return(false);
	}

	bool CJsonObject::Get(const std::string& strKey, int64& llValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Int)
		{
			llValue = (int64)(pJsonStruct->valueint);
			return(true);
		}
		else if (pJsonStruct->type == cJSON_Double)
		{
			llValue = (int64)(pJsonStruct->valuedouble);
			return(true);
		}
		return(false);
	}

	bool CJsonObject::Get(const std::string& strKey, uint64& ullValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Int)
		{
			ullValue = (uint64)(pJsonStruct->valueint);
			return(true);
		}
		else if (pJsonStruct->type == cJSON_Double)
		{
			ullValue = (uint64)(pJsonStruct->valuedouble);
			return(true);
		}
		return(false);
	}

	bool CJsonObject::Get(const std::string& strKey, bool& bValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type > cJSON_True)
		{
			return(false);
		}
		bValue = pJsonStruct->type;
		return(true);
	}

	bool CJsonObject::Get(const std::string& strKey, float& fValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Double || pJsonStruct->type == cJSON_Int)
		{
			fValue = (float)(pJsonStruct->valuedouble);
			return(true);
		}
		return(false);
	}

	bool CJsonObject::Get(const std::string& strKey, double& dValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pJsonData, strKey.c_str());
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Object)
			{
				pJsonStruct = cJSON_GetObjectItem(m_pExternJsonDataRef, strKey.c_str());
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Double || pJsonStruct->type == cJSON_Int)
		{
			dValue = pJsonStruct->valuedouble;
			return(true);
		}
		return(false);
	}

	int CJsonObject::GetArraySize()
	{
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Array)
			{
				return(cJSON_GetArraySize(m_pJsonData));
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Array)
			{
				return(cJSON_GetArraySize(m_pExternJsonDataRef));
			}
		}
		return(0);
	}

	bool CJsonObject::Get(int iWhich, CJsonObject& oJsonObject) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		char* pJsonString = cJSON_Print(pJsonStruct);
		std::string strJsonData = pJsonString;
		free(pJsonString);
		if (oJsonObject.Parse(strJsonData))
		{
			return(true);
		}
		else
		{
			return(false);
		}
	}

	bool CJsonObject::Get(int iWhich, std::string& strValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type != cJSON_String)
		{
			return(false);
		}
		strValue = pJsonStruct->valuestring;
		return(true);
	}

	bool CJsonObject::Get(int iWhich, int32& iValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Int)
		{
			iValue = (int32)(pJsonStruct->valueint);
			return(true);
		}
		else if (pJsonStruct->type == cJSON_Double)
		{
			iValue = (int32)(pJsonStruct->valuedouble);
			return(true);
		}
		return(false);
	}

	bool CJsonObject::Get(int iWhich, uint32& uiValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Int)
		{
			uiValue = (uint32)(pJsonStruct->valueint);
			return(true);
		}
		else if (pJsonStruct->type == cJSON_Double)
		{
			uiValue = (uint32)(pJsonStruct->valuedouble);
			return(true);
		}
		return(false);
	}

	bool CJsonObject::Get(int iWhich, int64& llValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Int)
		{
			llValue = (int64)(pJsonStruct->valueint);
			return(true);
		}
		else if (pJsonStruct->type == cJSON_Double)
		{
			llValue = (int64)(pJsonStruct->valuedouble);
			return(true);
		}
		return(false);
	}

	bool CJsonObject::Get(int iWhich, uint64& ullValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Int)
		{
			ullValue = (uint64)(pJsonStruct->valueint);
			return(true);
		}
		else if (pJsonStruct->type == cJSON_Double)
		{
			ullValue = (uint64)(pJsonStruct->valuedouble);
			return(true);
		}
		return(false);
	}

	bool CJsonObject::Get(int iWhich, bool& bValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type > cJSON_True)
		{
			return(false);
		}
		bValue = pJsonStruct->type;
		return(true);
	}

	bool CJsonObject::Get(int iWhich, float& fValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Double || pJsonStruct->type == cJSON_Int)
		{
			fValue = (float)(pJsonStruct->valuedouble);
			return(true);
		}
		return(false);
	}

	bool CJsonObject::Get(int iWhich, double& dValue) const
	{
		cJSON* pJsonStruct = NULL;
		if (m_pJsonData != NULL)
		{
			if (m_pJsonData->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pJsonData, iWhich);
			}
		}
		else if (m_pExternJsonDataRef != NULL)
		{
			if (m_pExternJsonDataRef->type == cJSON_Array)
			{
				pJsonStruct = cJSON_GetArrayItem(m_pExternJsonDataRef, iWhich);
			}
		}
		if (pJsonStruct == NULL)
		{
			return(false);
		}
		if (pJsonStruct->type == cJSON_Double || pJsonStruct->type == cJSON_Int)
		{
			dValue = pJsonStruct->valuedouble;
			return(true);
		}
		return(false);
	}

	CJsonObject::CJsonObject(cJSON* pJsonData)
		: m_pJsonData(NULL), m_pExternJsonDataRef(pJsonData), m_pKeyTravers(pJsonData)
	{
	}

}


