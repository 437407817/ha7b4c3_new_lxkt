#include "./DataConvert/cvt_time.h"

#include "./sys/sysio.h"





time_t CalculateTimestamp(uint16_t t_year,uint16_t t_month,uint16_t t_day,uint16_t t_hour,uint16_t t_minute,uint16_t t_second){
//GV_SD_485_time_ITEM
struct tm timeinfo = {0};
timeinfo.tm_year= t_year-1900;
timeinfo.tm_mon= t_month-1;	
timeinfo.tm_mday= t_day;
timeinfo.tm_hour= t_hour;	
timeinfo.tm_min= t_minute;	
timeinfo.tm_sec= t_second;
	
time_t timestamp = mktime(&timeinfo);	
	
	return timestamp;
	
}


time_t CalculateTimestampDifference(time_t start_timestamp,uint16_t t_year,uint16_t t_month,uint16_t t_day,uint16_t t_hour,uint16_t t_minute,uint16_t t_second){
//GV_SD_485_time_ITEM
struct tm timeinfo = {0};
timeinfo.tm_year= t_year-1900;
timeinfo.tm_mon= t_month-1;	
timeinfo.tm_mday= t_day;
timeinfo.tm_hour= t_hour;	
timeinfo.tm_min= t_minute;	
timeinfo.tm_sec= t_second;
	
time_t end_timestamp = mktime(&timeinfo);	
	
	return difftime(end_timestamp, start_timestamp);
	
}



void CalculateTimestampToTime(time_t seconds,uint16_t* t_year,uint16_t* t_month,uint16_t* t_day,uint16_t* t_hour,uint16_t* t_minute,uint16_t* t_second){
//GV_SD_485_time_ITEM
	struct tm *timeinfo;
	//timeinfo = malloc(sizeof(struct tm));
	//timeinfo = gmtime(&seconds);  // 使用 gmtime 获取 UTC 时间
	//time_t tempTime = seconds;
	//timeinfo = localtime(&tempTime);
	
	timeinfo = localtime(&seconds);
	if (timeinfo == NULL) {
        SYSTEM_INFO("Error converting time\n");
		//free(timeinfo);
        return;
    }
// 提取日期和时间组件
    *t_year = (uint16_t)(timeinfo->tm_year + 1900);  // 从 1900 年开始的年数
    *t_month = (uint16_t)(timeinfo->tm_mon + 1);     // 从 0 开始的月份
    *t_day = (uint16_t)(timeinfo->tm_mday);
    *t_hour = (uint16_t)(timeinfo->tm_hour);
    *t_minute = (uint16_t)(timeinfo->tm_min);
    *t_second = (uint16_t)(timeinfo->tm_sec);
		timeinfo=NULL;
		
		
//		char buffer[80];
//	strftime(buffer, 80, "%Y%m%d %H:%M:%S", timeinfo);
//		SYSTEM_DEBUG(" CAL++s %s\n",buffer);
//		
//SYSTEM_DEBUG(" CAL++ %d %u-%u-%u,%u-%u-%u\n",seconds,timeinfo->tm_year ,timeinfo->tm_mon ,timeinfo->tm_mday,
//timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
//	//free(timeinfo);
}





















