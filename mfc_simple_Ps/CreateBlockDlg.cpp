// CreateBlockDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CreateBlockDlg.h"
#include "afxdialogex.h"


// CreateBlockDlg ダイアログ

IMPLEMENT_DYNAMIC(CreateBlockDlg, CDialogEx)

CreateBlockDlg::CreateBlockDlg(const double in_XL, const double in_YL, const double in_ZL, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CREATE_BLOCK_DIALOG, pParent)
	, m_dEditXL(in_XL)
	, m_dEditYL(in_YL)
	, m_dEditZL(in_ZL)
	, m_dEditOX(0)
	, m_dEditOY(0)
	, m_dEditOZ(0)
{

}

CreateBlockDlg::~CreateBlockDlg()
{
}

void CreateBlockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_XL, m_dEditXL);
	DDX_Text(pDX, IDC_EDIT_YL, m_dEditYL);
	DDX_Text(pDX, IDC_EDIT_ZL, m_dEditZL);
	DDX_Text(pDX, IDC_EDIT_OX, m_dEditOX);
	DDX_Text(pDX, IDC_EDIT_OY, m_dEditOY);
	DDX_Text(pDX, IDC_EDIT_OZ, m_dEditOZ);
}


BEGIN_MESSAGE_MAP(CreateBlockDlg, CDialogEx)
END_MESSAGE_MAP()


// CreateBlockDlg メッセージ ハンドラー
