#pragma once

class ITabFile
{
public:
    virtual void    Clear() = 0;
    virtual void    Release() = 0;

    virtual void    SetErrorLog(int bEnable) = 0;

    //do not use "FindRow"
    virtual int     FindRow(const char* szRow) = 0;         //返回以1为起点的值
    virtual int     FindColumn(const char* szColumn) = 0;   //返回以1为起点的值
    virtual int     GetWidth() = 0;
    virtual int     GetHeight() = 0;

    //--读取数据---
    virtual int     GetString(int nRow, const char* szColumn, const char* lpDefault,
        char* lpRString, unsigned int dwSize, int bColumnLab = true) = 0;
    virtual int     GetString(int nRow, int nColumn, const char* lpDefault,
        char* lpRString, unsigned int dwSize) = 0;
    virtual int     GetString(const char* szRow, const char* szColumn, const char* lpDefault,
        char* lpRString, unsigned int dwSize) = 0;

    virtual int     GetInteger(int nRow, const char* szColumn, int nDefault,
        int *pnValue, int bColumnLab = true) = 0;
    template<class T>
    inline  int     GetIntegerToType(int nRow, const char* szColumn, int nDefault,
        T *pnValue, int bColumnLab = true)
    {
        if (pnValue == nullptr)
            return 0;
        int nValue = 0;
        int nRet = GetInteger(nRow, szColumn, nDefault, &nValue, bColumnLab);
        *pnValue = nValue;
        return nRet;
    }

    virtual int     GetInteger(int nRow, int nColumn, int nDefault, int *pnValue) = 0;
    template<class T>
    inline  int     GetIntegerToType(int nRow, int nColumn, int nDefault, T *pnValue)
    {
        if (pnValue == nullptr)
            return 0;
        int nValue = 0;
        int nRet = GetInteger(nRow, nColumn, nDefault, &nValue);
        *pnValue = nValue;
        return nRet;
    }

    virtual int     GetInteger(const char* szRow, const char* szColumn,
        int nDefault, int *pnValue) = 0;
    template<class T>
    inline  int     GetIntegerToType(const char* szRow, const char* szColumn,
        int nDefault, T *pnValue)
    {
        if (pnValue == nullptr)
            return 0;
        int nValue = 0;
        int nRet = GetInteger(szRow, szColumn, nDefault, &nValue);
        *pnValue = nValue;
        return nRet;
    }

    virtual int     GetBoolean(int nRow, const char* szColumn, bool bDefault,
        bool *pbValue, int bColumnLab = true) = 0;
    virtual int     GetBoolean(int nRow, int nColumn, bool bDefault, bool *pbValue) = 0;
    virtual int     GetBoolean(const char* szRow, const char* szColumn,
        bool bDefault, bool *pbValue) = 0;

    virtual int     GetFloat(int nRow, const char* szColumn, float fDefault,
        float *pfValue, int bColumnLab = true) = 0;
    virtual int     GetFloat(int nRow, int nColumn, float fDefault, float *pfValue) = 0;
    virtual int     GetFloat(const char* szRow, const char* szColumn,
        float fDefault, float *pfValue) = 0;

    virtual const char* GetRowName(int nRow) = 0;
    virtual const char* GetColName(int nCol) = 0;

    virtual ~ITabFile() {};
};
