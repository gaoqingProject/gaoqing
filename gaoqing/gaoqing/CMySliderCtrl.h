/*
File name:   CMySliderCtrl.h
File Author: Li Weichen
Date:        2016.3.1
*/
class CMySliderCtrl : public CSliderCtrl 
{     
	DECLARE_DYNAMIC(CMySliderCtrl)   
	public:     
		CMySliderCtrl();     
		virtual ~CMySliderCtrl();  
		bool *sign; //is dragging
	protected:     
		DECLARE_MESSAGE_MAP() 
	public:     
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point); 
		
};  

