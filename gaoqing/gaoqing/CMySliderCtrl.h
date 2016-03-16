class CMySliderCtrl : public CSliderCtrl 
{     
	DECLARE_DYNAMIC(CMySliderCtrl)   
	public:     
		CMySliderCtrl();     
		virtual ~CMySliderCtrl();  
		bool *sign;
	protected:     
		DECLARE_MESSAGE_MAP() 
	public:     
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point); 
		
};  

