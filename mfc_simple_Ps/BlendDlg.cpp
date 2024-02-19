// BlendDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "BlendDlg.h"
#include "afxdialogex.h"


// BlendDlg ダイアログ

IMPLEMENT_DYNAMIC(BlendDlg, CDialogEx)

BlendDlg::BlendDlg(const double in_R, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BLEND_DIALOG, pParent)
	, m_dBlendR(in_R)
{

}

BlendDlg::~BlendDlg()
{
}

void BlendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_R, m_dBlendR);
}


BEGIN_MESSAGE_MAP(BlendDlg, CDialogEx)
END_MESSAGE_MAP()


// BlendDlg メッセージ ハンドラー
