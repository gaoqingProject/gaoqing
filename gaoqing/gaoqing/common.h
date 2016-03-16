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
    BYTE brightness;		//��
    BYTE contrast;		//��
    BYTE sharpness;		//��
    BYTE saturation;		//ʱ
    int exposuremode;		//��
    int exposuretime;
	int daynight;		//��
}PARAM_STRU, *LPPARAM_STRU;