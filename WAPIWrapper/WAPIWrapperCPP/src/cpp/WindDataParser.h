#pragma once

#include <string>
using namespace std;

class WindData;

class WindDataParser
{
public:
	WindDataParser(void);
	~WindDataParser(void);

	static bool IsEmpty(const VARIANT& var);
	static bool IsArray(const VARIANT& var);

	static long GetCountOfSafeArray(const VARIANT& safeArray);

	static int GetStrItemIndexOfSafeArray(const VARIANT& safeArray, const wstring& itemNameStr);
	static wstring GetStrItemByIndex(const VARIANT& safeArray, int index);

	static int GetDoubleItemIndexOfSafeArray(const VARIANT& safeArray, DOUBLE dbl);
	static DOUBLE GetDoubeItemByIndex(const VARIANT& safeArray, int index);

	static LPCWSTR GetInternalStrByIndex(const VARIANT& safeArray, int index);

	static void SetDataToWindData(int time, int codes, int fields, const VARIANT& data, WindData& wd);
	static VARIANT GetVarOfDataFromWindData(int time, int codes, int fields, const WindData& wd);
	static VARIANT GetVarFromArray(const VARIANT& safeArray, int pos);
	static void PutItemToVariant(VARIANT& var, const VARIANT& data, long pos);
};

