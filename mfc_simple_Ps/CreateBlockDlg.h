#pragma once
#include "Resource.h"

// CreateBlockDlg ダイアログ

class CreateBlockDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateBlockDlg)

public:
	CreateBlockDlg(const double in_XL, const double in_YL, const double in_ZL, CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CreateBlockDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATE_BLOCK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	double m_dEditXL;
	double m_dEditYL;
	double m_dEditZL;
	double m_dEditOX;
	double m_dEditOY;
	double m_dEditOZ;
};
