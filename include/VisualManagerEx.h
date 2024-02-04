#pragma once

// CVisualManagerEx command target

class CVisualManagerEx : public CMFCVisualManagerOffice2007
{
	DECLARE_DYNCREATE(CVisualManagerEx)
public:
	CVisualManagerEx();
	virtual ~CVisualManagerEx();

	virtual void GetTabFrameColors(const CMFCBaseTabCtrl* pTabWnd, COLORREF& clrDark, COLORREF& clrBlack, COLORREF& clrHighlight, COLORREF& clrFace, COLORREF& clrDarkShadow, COLORREF& clrLight, CBrush*& pbrFace, CBrush*& pbrBlack);

	//virtual void OnDrawTab(CDC* pDC, CRect rectTab, int iTab, BOOL bIsActive, const CMFCBaseTabCtrl* pTabWnd);
	virtual void OnEraseTabsArea(CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd);
	virtual BOOL OnEraseTabsFrame(CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd);
	virtual COLORREF OnDrawRibbonStatusBarPane(CDC* pDC, CMFCRibbonStatusBar* pBar, CMFCRibbonStatusBarPane* pPane);
	/*virtual void OnDrawTabCloseButton(CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled) override;
	virtual void OnEraseTabsButton(CDC* pDC, CRect rect, CMFCButton* pButton, CMFCBaseTabCtrl* pWndTab) override;*/
	virtual void OnDrawTabsButtonBorder(CDC* pDC, CRect& rect, CMFCButton* pButton, UINT uiState, CMFCBaseTabCtrl* pWndTab);
	virtual COLORREF GetTabTextColor(const CMFCBaseTabCtrl* pTabWnd, int iTab, BOOL bIsActive);
	//virtual void AdjustTabTextRect(CRect& rectTabText) { rectTabText.OffsetRect(-3, 0); }
	

	//Zoom slider
	virtual void OnDrawRibbonSliderZoomButton(CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect, BOOL bIsZoomOut, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled);
	virtual void OnDrawRibbonSliderChannel(CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect);
	virtual void OnDrawRibbonSliderThumb(CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled);

	virtual void OnFillBarBackground(CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea = FALSE);
	virtual void OnDrawMenuBorder(CDC* pDC, CMFCPopupMenu* pMenu, CRect rect);
	
	virtual void OnDrawMenuCheck(CDC* pDC, CMFCToolBarMenuButton* pButton, CRect rect, BOOL bHighlight, BOOL bIsRadio);

	//virtual void OnDrawMenuItemButton(CDC* pDC, CMFCToolBarMenuButton* pButton, CRect rectButton, BOOL bHighlight, BOOL bDisabled);
	virtual void OnDrawSeparator(CDC* pDC, CBasePane* pBar, CRect rect, BOOL bIsHoriz);

	

	void OnFillButtonInterior(CDC * pDC, CMFCToolBarButton * pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state);

	virtual COLORREF GetStatusBarPaneTextColor(CMFCStatusBar* pStatusBar, CMFCStatusBarPaneInfo* pPane);
	virtual COLORREF GetToolbarButtonTextColor(CMFCToolBarButton* pButton, CMFCVisualManager::AFX_BUTTON_STATE state);
	virtual COLORREF GetMenuItemTextColor(CMFCToolBarMenuButton* pButton, BOOL bHighlighted, BOOL bDisabled);
	virtual COLORREF GetHighlightedMenuItemTextColor(CMFCToolBarMenuButton* pButton);
	virtual void OnHighlightMenuItem(CDC *pDC, CMFCToolBarMenuButton* pButton, CRect rect, COLORREF& clrText);
	
	virtual COLORREF GetCaptionBarTextColor(CMFCCaptionBar* pBar);
	//virtual void OnFillOutlookPageButton(CDC* pDC, const CRect& rect, BOOL bIsHighlighted, BOOL bIsPressed, COLORREF& clrText);
	//virtual void OnFillOutlookBarCaption(CDC* pDC, CRect rectCaption, COLORREF& clrText);
	virtual COLORREF GetToolbarDisabledTextColor();
	//virtual void OnDrawRibbonCaption(CDC* pDC, CMFCRibbonBar* pBar, CRect rectCaption, CRect rectText);
	virtual COLORREF GetRibbonQuickAccessToolBarTextColor(BOOL bDisabled = FALSE) { return bDisabled ? m_clrRibbonCategoryTextDisabled : m_clrRibbonCategoryText; }
	virtual COLORREF OnDrawRibbonCategoryCaption(CDC* pDC, CMFCRibbonContextCaption* pContextCaption);
	virtual COLORREF GetRibbonHyperlinkTextColor(CMFCRibbonLinkCtrl* pHyperLink);
	virtual COLORREF GetRibbonStatusBarTextColor(CMFCRibbonStatusBar* pStatusBar);


	virtual int GetRibbonPopupBorderSize(const CMFCRibbonPanelMenu* pPopup) const;

	virtual void OnDrawMiniFrameBorder(CDC* pDC, CPaneFrameWnd* pFrameWnd, CRect rectBorder, CRect rectBorderSize);
	virtual void OnDrawFloatingToolbarBorder(CDC* pDC, CMFCBaseToolBar* pToolBar, CRect rectBorder, CRect rectBorderSize);
	

	virtual COLORREF OnDrawPaneCaption(CDC* pDC, CDockablePane* pBar, BOOL bActive, CRect rectCaption, CRect rectButtons);
	virtual void OnDrawButtonBorder(CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state);

	virtual void OnDrawHeaderCtrlBorder(CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted);
	virtual void OnDrawRibbonLabel(CDC* pDC, CMFCRibbonLabel* pLabel, CRect rect);


	virtual BOOL GetToolTipInfo(CMFCToolTipInfo& params, UINT nType = (UINT)(-1)) override;
	
	
	// Auto-hide buttons:
	virtual void OnFillAutoHideButtonBackground(CDC* pDC, CRect rect, CMFCAutoHideButton* pButton);
	virtual void OnDrawAutoHideButtonBorder(CDC* pDC, CRect rectBounds, CRect rectBorderSize, CMFCAutoHideButton* pButton) {}
	



protected:
	CBrush *m_pBackBrush;
	CBrush *m_pFaceBrush;
};


