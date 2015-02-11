#ifndef _WINDDATACOMSINK_H
#define _WINDDATACOMSINK_H

#include "stdafx.h"

#include <map>
using namespace std;

#include "..\header\WAPIWrapperCpp.h"

struct WsqReq{
	WindData			windData;
	WsqCallBack		callback;
	BOOL				updateAllFields;
};

class WindDataComSink : public WindDataCOMLib::_DWindDataCOMEvents
{
public:
	WindDataComSink();
	virtual ~WindDataComSink();

public:

	void insertToMap(ULONGLONG reqId, WsqReq* pWsqReq);
	void deleteFromMap(ULONGLONG reqId);
	void clearMap();

	STDMETHODIMP Fire_StateChanged (long state, unsigned __int64 requestid, long errorCode);

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppvObject);
	
	ULONG STDMETHODCALLTYPE AddRef();

	ULONG STDMETHODCALLTYPE Release();

	HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
		/* [out] */ __RPC__out UINT *pctinfo);

	HRESULT STDMETHODCALLTYPE GetTypeInfo( 
		/* [in] */ UINT iTInfo,
		/* [in] */ LCID lcid,
		/* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);

	HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
		/* [in] */ __RPC__in REFIID riid,
		/* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
		/* [range][in] */ UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);

	/* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS *pDispParams,
		/* [out] */ VARIANT *pVarResult,
		/* [out] */ EXCEPINFO *pExcepInfo,
		/* [out] */ UINT *puArgErr);

private:
	void updateWsqAllFields(WindData &src, WindData &dest);

private:
	DWORD						dwRefCount;	
	map<ULONGLONG, WsqReq*>		m_wsqMap;
};

#endif