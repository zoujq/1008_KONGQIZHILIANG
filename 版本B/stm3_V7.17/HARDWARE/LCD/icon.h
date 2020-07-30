#ifndef __ICON_H
#define __ICON_H	   

extern const unsigned char gImage_RF4[2700]; 
//extern const unsigned char gImage_RF3[2700]; 
//extern const unsigned char gImage_RF2[2700]; 
extern const unsigned char gImage_RF1[2700]; 


extern const unsigned char gImage_low_03[16368];


//extern const unsigned char gImage_MOLA_03[3000];

//extern const unsigned char gImage_RHT_03[3000];

extern const unsigned char gImage_NB_031_03[4582];
//extern const unsigned char gImage_NB_032_03[4582];
extern const unsigned char gImage_NB_033_03[4582];

extern const unsigned char gImage_battery_00[2548];

//extern const unsigned char gImage_battery_00[2548];
//extern const unsigned char gImage_battery_01[2548];
//extern const unsigned char gImage_battery_02[2548];
//extern const unsigned char gImage_battery_03[2548];

extern const unsigned char gImage_CO2_03[1888]; 
extern const unsigned char gImage_CO2_03EG[1888]; 
extern const unsigned char gImage_PM25_03[1408]; 
extern const unsigned char gImage_HCHO_03[2080];
extern const unsigned char gImage_HCHO_03EG[2080];
extern const unsigned char gImage_TVOC_03[1800];
extern const unsigned char gImage_humy_03[3774];
extern const unsigned char gImage_temp_03[3078];
extern const unsigned char gImage_tempc_03[576]; 	
extern const unsigned char gImage_humyc_03[544];
extern const unsigned char gImage_ppm_03[1050]; 
extern const unsigned char gImage_mgm3_03[1872];
extern const unsigned char gImage_ugm3_03[1656]; 


extern const unsigned char gImage_ppmb_03[3920];
extern const unsigned char gImage_mgm3b_03[7704]; 
extern const unsigned char gImage_ugm3b_03[4898];

//extern const unsigned char gImage_co2b_03[8680];
//extern const unsigned char gImage_co2b_03EG[8680];

extern const unsigned char gImage_co2b_03[7440] ;
extern const unsigned char gImage_co2b_03EG[3016] ;

//extern const unsigned char gImage_HCHOb_03[5376];
//extern const unsigned char gImage_HCHOb_03EG[5376];

extern const unsigned char gImage_HCHOb_03[3658] ;
extern const unsigned char gImage_HCHOb_03EG[4200]  ;

extern const unsigned char gImage_pm25b_03[6204]; 
extern const unsigned char gImage_tvocb_03[6758];


extern const unsigned char gImage_20_03[630];
extern const unsigned char gImage_21_03[630];
extern const unsigned char gImage_22_03[630];
extern const unsigned char gImage_23_03[630];
extern const unsigned char gImage_24_03[630];
extern const unsigned char gImage_25_03[630];
extern const unsigned char gImage_26_03[630];
extern const unsigned char gImage_27_03[630];
extern const unsigned char gImage_28_03[630];
extern const unsigned char gImage_29_03[630];


extern const unsigned char gImage_10_03[2790];
extern const unsigned char gImage_11_03[2790];
extern const unsigned char gImage_12_03[2790];
extern const unsigned char gImage_13_03[2790];
extern const unsigned char gImage_14_03[2790];
extern const unsigned char gImage_15_03[2790];
extern const unsigned char gImage_16_03[2790];
extern const unsigned char gImage_17_03[2790];
extern const unsigned char gImage_18_03[2790];
extern const unsigned char gImage_19_03[2790];



extern const unsigned char gImage_00_03[11700];
extern const unsigned char gImage_01_03[11700];
extern const unsigned char gImage_02_03[11700];
extern const unsigned char gImage_03_03[11700];
extern const unsigned char gImage_04_03[11700];
extern const unsigned char gImage_05_03[11700];
extern const unsigned char gImage_06_03[11700];
extern const unsigned char gImage_07_03[11700];
extern const unsigned char gImage_08_03[11700];
extern const unsigned char gImage_09_03[11700];


extern const unsigned char gImage_Week_03[2704]; 
extern const unsigned char gImage_Mon_03[1404];
extern const unsigned char gImage_Tues_03[1404];
extern const unsigned char gImage_Wed_03[1404];
extern const unsigned char gImage_Thur_03[1404]; 
extern const unsigned char gImage_Fri_03[1404];
extern const unsigned char gImage_Sat_03[1404];
extern const unsigned char gImage_Sun_03[1404];
extern const unsigned char gImage_Month_03[1300]; 
extern const unsigned char gImage_Day_03[1300];
	


extern const unsigned char gImage_eMon_03[2604]; 
extern const unsigned char gImage_eTue_03[2604];
extern const unsigned char gImage_eWed_03[2604]	;
extern const unsigned char gImage_eThu_03[2604] ;
extern const unsigned char gImage_eFri_03[2604];
extern const unsigned char gImage_eSat_03[2604];
extern const unsigned char gImage_eSun_03[2604] ;
	


extern const unsigned char gImage_Jan_03[3016] ;
extern const unsigned char gImage_Feb_03[3016];
extern const unsigned char gImage_Mar_03[3016];
extern const unsigned char gImage_Apr_03[3016] ;
extern const unsigned char gImage_May_03[3016]; 
extern const unsigned char gImage_Jun_03[3016]	;
extern const unsigned char gImage_Jul_03[3016] ;
extern const unsigned char gImage_Aug_03[3016];
extern const unsigned char gImage_Sep_03[3016] ;
extern const unsigned char gImage_Oct_03[3016] ;
extern const unsigned char gImage_Nov_03[3016];
extern const unsigned char gImage_Dec_03[3016] ;


extern const unsigned char gImage_excellent_03[2624]; 
extern const unsigned char gImage_excellent_03EG[2624] ;	
extern const unsigned char gImage_good_03[2624] ;
extern const unsigned char gImage_good_03EG[2624];
extern const unsigned char gImage_moderate_03[2624] ;
extern const unsigned char gImage_moderate_03EG[2624]	;
extern const unsigned char gImage_poor_03[2624];
extern const unsigned char gImage_poor_03EG[2624] ;

extern const unsigned char gImage_excellentb_03[9240]; 
extern const unsigned char gImage_excellentbEG_03[9240];
extern const unsigned char gImage_goodb_03[9240];
extern const unsigned char gImage_goodbEG_03[9240] ;	
extern const unsigned char gImage_moderateb_03[9240] ;
extern const unsigned char gImage_moderatebEG_03[9240];
extern const unsigned char gImage_poorb_03[9240];
extern const unsigned char gImage_poorbEG_03[9240] 	;

extern const unsigned char gImage_PiaoAi[3016];
extern const unsigned char gImage_AIR[2024] ;

void icon_show(unsigned short  x, unsigned short  y, unsigned short  width, unsigned short  height, const unsigned char* pstr,unsigned char ishow);//显示
void icon_show_enlarge(unsigned short  x, unsigned short  y, unsigned short  width, unsigned short  height, const unsigned char* pstr,unsigned char ishow,unsigned char ienlarge);//放大显示	
void icon_show_narrow(unsigned short  x, unsigned short  y, unsigned short  width, unsigned short  height, const unsigned char* pstr,unsigned char ishow,unsigned char inarrow);//缩小显示
	



#endif
