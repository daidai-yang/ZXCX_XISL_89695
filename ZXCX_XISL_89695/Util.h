#pragma once


class CUtil
{
public:
	CUtil() {};
	virtual ~CUtil() {};

};

class CErrorInfo
{
public:
	CErrorInfo() {};
	virtual ~CErrorInfo() {};

	CHAR	m_szMTC[7];
	CHAR	m_szMTCDesc[1024];

	void	SetMTC(BYTE bCode);
	CString MTCodeToData(int errCode);
};