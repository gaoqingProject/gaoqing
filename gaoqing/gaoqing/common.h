#pragma once

typedef struct STRU_PLAY
{

	CString *patha;
	CString *pathb;
	CString *pathc;
	CString *pathd;
	int time;
}CAR_PLAY;

typedef struct
{
    BYTE brightness;		//年
    BYTE contrast;		//月
    BYTE sharpness;		//日
    BYTE saturation;		//时
    int exposuremode;		//分
    int exposuretime;
	int daynight;		//秒
}PARAM_STRU, *LPPARAM_STRU;