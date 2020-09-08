/*******************************************************************************
 * Project:  neb
 * @file     CJsonObject.hpp
 * @brief    Json
 * @author   bwarliao
 * @date:    2014-7-16
 * @note
 * Modify history:
 ******************************************************************************/

#ifndef CJSONOBJECT_HPP_
#define CJSONOBJECT_HPP_

#include <stdio.h>
#include <stddef.h>
#include <malloc.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <string>
#include <map>
#include <list>
#ifdef __cplusplus
extern "C" {
#endif
#include "cJSON.h"
#ifdef __cplusplus
}
#endif


namespace neb
{

	class CJsonObject
	{
	public:     // method of ordinary json object or json array
		CJsonObject();
		CJsonObject(const std::string& strJson);
		CJsonObject(const CJsonObject* pJsonObject);
		CJsonObject(const CJsonObject& oJsonObject);
		virtual ~CJsonObject();

		CJsonObject& operator=(const CJsonObject& oJsonObject);
		bool operator==(const CJsonObject& oJsonObject) const;
		bool Parse(const std::string& strJson);
		void Clear();
		bool IsEmpty() const;
		bool IsArray() const;
		std::string ToString() const;
		std::string ToFormattedString() const;
		const std::string& GetErrMsg() const
		{
			return(m_strErrMsg);
		}

	public:     // method of ordinary json object
		bool GetKey(std::string& strKey);
		bool HasKey(const std::string &strKey);
		void ResetTraversing();
		CJsonObject& operator[](const std::string& strKey);
		std::string operator()(const std::string& strKey) const;
		bool Get(const std::string& strKey, CJsonObject& oJsonObject) const;
		bool Get(const std::string& strKey, std::string& strValue) const;
		bool Get(const std::string& strKey, int32& iValue) const;
		bool Get(const std::string& strKey, uint32& uiValue) const;
		bool Get(const std::string& strKey, int64& llValue) const;
		bool Get(const std::string& strKey, uint64& ullValue) const;
		bool Get(const std::string& strKey, bool& bValue) const;
		bool Get(const std::string& strKey, float& fValue) const;
		bool Get(const std::string& strKey, double& dValue) const;

		template<typename T>
		T Get(const std::string &strKey) {
			T value;
			Get(strKey, value);
			return value;
		}

	public:     // method of json array
		int GetArraySize();
		CJsonObject& operator[](unsigned int uiWhich);
		std::string operator()(unsigned int uiWhich) const;
		bool Get(int iWhich, CJsonObject& oJsonObject) const;
		bool Get(int iWhich, std::string& strValue) const;
		bool Get(int iWhich, int32& iValue) const;
		bool Get(int iWhich, uint32& uiValue) const;
		bool Get(int iWhich, int64& llValue) const;
		bool Get(int iWhich, uint64& ullValue) const;
		bool Get(int iWhich, bool& bValue) const;
		bool Get(int iWhich, float& fValue) const;
		bool Get(int iWhich, double& dValue) const;
	private:
		CJsonObject(cJSON* pJsonData);

	private:
		cJSON* m_pJsonData;
		cJSON* m_pExternJsonDataRef;
		cJSON* m_pKeyTravers;
		std::string m_strErrMsg;
		std::map<unsigned int, CJsonObject*> m_mapJsonArrayRef;
		std::map<std::string, CJsonObject*> m_mapJsonObjectRef;
	};

}

#endif /* CJSONHELPER_HPP_ */
