#pragma once

#ifndef SOMESTRUCT
#define SOMESTRUCT

#include <vector>
#include <string>
#include "stdafx.h"
#include "ImageRenderer.h"
#include <map>
#include <queue>
#include <time.h>
#include <opencv\cv.h>
#include "resource.h"

//advertisement structure
typedef struct Advertisement
{
	//id
	int id;
	//ad path
	std::string filePath;
	//ad tag
	std::vector<std::string> labels;
	//ad play times
	int contratual_show_count;
	//ad played times
	int showed_count;
	//ad played time
	long long int showed_time;
}Adevertisement;

//用户性别
const int SEX_MALE = 0;
const int SEX_FEMALE = 1;
const int SEX_UNKNOWN = 2;

//状态
const int STATE_NO = 0;
const int STATE_YES = 1;
const int STATE_UNKNOWN = 2;

//策略选择
//无策略自由模式
const int STRATEGY_NONE = 0;
//最小展示次数合约
const int STRATEGY_MIN_COUNT = 1;
//最小展示时间合约
const int STRATEGY_MIN_TIME = 2;

//关系模式
//情侣关系
const int RELATION_COUPLE = 0;
//好兄弟关系
const int RELATION_BROTHERS = 1;
//好姐妹关系
const int RELATION_SISTERS = 2;
//兄妹
const int RELATION_BROTHER_SISTER = 3;
//姐弟
const int RELATION_SISTER_BROTHER = 4;
//年轻母子关系
const int RELATION_MOTHER_SON_YOUNG = 5;
//年轻母女关系
const int RELATION_MOTHER_DAUGHTER_YOUNG = 6;
//年轻父子关系
const int RELATION_FATHER_SON_YOUNG = 7;
//年轻父女关系
const int RELATION_FATHER_DAUGHTER_YOUNG = 8;
//年老母子关系
const int RELATION_MOTHER_SON_OLD = 9;
//年老母女关系 
const int RELATION_MOTHER_DAUGHTER_OLD = 10;
//年老父子关系
const int RELATION_FATHER_SON_OLD = 11;
//年老父女关系
const int RELATION_FATHER_DAUGHTER_OLD = 12;
//家庭
const int RELATION_FAMILY = 13;
//一群人
const int RELATION_GROUP = 14;
//一个人
const int RELATION_SIMPLE = 15;

typedef struct MomentPersonState
{
	//id
	int id;
	//interval
	double intervalTime;
	//bool queue
	bool inSet;
	//state
	int expression;
	bool engaged;
	bool leftEyeClosed;
	bool rightEyeClosed;
	bool lookAway;
	bool mouthMoved;
	bool mouthOpen;
	bool wearingGlasse;
	//性别
	int sex;
	//年龄
	int age;
	//当前看的广告的编号
	int ad_id;
	//深度
	double depth;
	//相对横坐标
	double x;
	cv::Mat mat;
}PersonMomentState;

//天气信息
typedef struct Weather
{
	//城市名
	std::string city;
	//城市id
	std::string city_id;
	//温度
	double temperature;
	//风向
	std::string windy_direction;
	//风力
	double windy_strength;
	//湿度
	double humidity;
	//更新时间
	std::string time;
	//气压
	double pressure;
	//是否下雨
	bool rain;
} Weather;

//人物的统计信息
typedef struct PersonInformation
{
	//人物ID
	int id;
	//出现的帧数
	long long int frameCount;
	//停留总时长
	double sumTime;
	//上一帧时间
	__int64 preTime;
	//出现的时间
	time_t startTime;
	long long int addTime;
	//人物已存在时间
	long long int stay_time;
	//微笑的时长
	double happyTime;
	//engaged的时长
	double engagedTime;
	//看广告牌的时长
	double watchTime;
	//左眼闭的时长
	double leftEyeCloseTime;
	//右眼闭的时长
	double rightEyeCloseTime;
	//嘴巴动的时长
	double mouthMovedTime;
	//嘴巴张开的时长
	double mouthOpenTime;
	//年龄
	int age;
	//是否戴眼镜的计数
	int wearGT, NwearGT;
	//性别
	int sex;
	//性别计数
	int sex_male, sex_female;
	//是否戴眼镜
	bool wearGlasses;
	//人物是否微笑
	int expression;
	//人物是否在远望
	bool lookaway;
	//是否注视
	bool engaged;
	//是否左眼睁开
	bool leftEyeOpen;
	//是否右眼睁开
	bool rightEyeOpen;
	//是否嘴巴动
	bool mouthMoved;
	//是否嘴巴张开
	bool mouthOpen;
	//是否这一帧出现
	bool checked;
	//深度
	double depth;
	//相对横坐标
	double x;
	//所属组群id
	int group;
	//广告编号和时长
	std::map<int, long long int> ad_list;
	//当前天气
	double weight;
	Weather nowWeather;
	PersonInformation()
	{
		id = 0;
		frameCount = 0;
		preTime = sumTime = startTime = watchTime = happyTime = engagedTime = leftEyeCloseTime = rightEyeCloseTime = mouthMovedTime = mouthOpenTime = 0;
		age = 0;
		sex = SEX_UNKNOWN;
		wearGlasses = STATE_UNKNOWN;
		wearGT = NwearGT = 0;
		addTime = 0;
		sex_male = sex_female = 0;
		x = depth = 0;
		weight = 1;
		engaged = lookaway = leftEyeOpen = rightEyeOpen = mouthMoved = mouthOpen = expression = STATE_UNKNOWN;
	}
} PersonInformation;

//人物动态信息
typedef struct PersonMoment {
	//group信息
	int group;
	//当前帧是否出现
	bool appear;
	//id
	int id;
	//出现时间
	long long int start_time;
	// 性别
	int gender;
	// 年龄
	int age;
	// 表情
	int expression;
	// 专注
	bool engaged;
	// 眼镜
	bool wearingGlasses;
	//镜头距离
	int z;
	//距离变近
	bool dis_change;
	//人脸框
	RectI faceBox;
	//未出现的帧数
	int disapper_time;
	//图片
	cv::Mat faceImg;
}PersonMoment;

//任务训练样本
typedef struct PersonInfoTraining
{
	//关注时间
	long long int watch_time;
	//性别
	int gender;
	// 年龄
	int age;
	// 表情
	int expression;
	// 专注
	bool engaged;
	// 眼镜
	bool wearingGlasses;
};

//组群信息
typedef struct Group
{
	//人物队列
	std::vector<PersonInformation*> persons;
	//综合权值
	double weight;
	//人物关系
	int relation;
	//组群id
	int id;
} Group;

//广告的标签字符串与编号的对应关系
typedef std::map<std::string, int> map_ad_label;
//广告列表
typedef std::vector<Advertisement> vector_ad_list;
//人次信息队列
typedef std::queue<MomentPersonState> queue_moment_person_state;
//人物信息列表
typedef std::vector<PersonInformation> vector_person_information;
//人物组群列表
typedef std::vector<Group> vector_group;
//已离开人物列表
typedef std::vector<PersonInformation> vector_person_left;

#endif

