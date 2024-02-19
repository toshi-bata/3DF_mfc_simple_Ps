#pragma once
#include "Resource.h"

// BlendDlg ダイアログ

class BlendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BlendDlg)

public:
	BlendDlg(const double in_R, CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~BlendDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLEND_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	double m_dBlendR;

};
