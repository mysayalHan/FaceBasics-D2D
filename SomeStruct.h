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

//�û��Ա�
const int SEX_MALE = 0;
const int SEX_FEMALE = 1;
const int SEX_UNKNOWN = 2;

//״̬
const int STATE_NO = 0;
const int STATE_YES = 1;
const int STATE_UNKNOWN = 2;

//����ѡ��
//�޲�������ģʽ
const int STRATEGY_NONE = 0;
//��Сչʾ������Լ
const int STRATEGY_MIN_COUNT = 1;
//��Сչʾʱ���Լ
const int STRATEGY_MIN_TIME = 2;

//��ϵģʽ
//���¹�ϵ
const int RELATION_COUPLE = 0;
//���ֵܹ�ϵ
const int RELATION_BROTHERS = 1;
//�ý��ù�ϵ
const int RELATION_SISTERS = 2;
//����
const int RELATION_BROTHER_SISTER = 3;
//���
const int RELATION_SISTER_BROTHER = 4;
//����ĸ�ӹ�ϵ
const int RELATION_MOTHER_SON_YOUNG = 5;
//����ĸŮ��ϵ
const int RELATION_MOTHER_DAUGHTER_YOUNG = 6;
//���Ḹ�ӹ�ϵ
const int RELATION_FATHER_SON_YOUNG = 7;
//���ḸŮ��ϵ
const int RELATION_FATHER_DAUGHTER_YOUNG = 8;
//����ĸ�ӹ�ϵ
const int RELATION_MOTHER_SON_OLD = 9;
//����ĸŮ��ϵ 
const int RELATION_MOTHER_DAUGHTER_OLD = 10;
//���ϸ��ӹ�ϵ
const int RELATION_FATHER_SON_OLD = 11;
//���ϸ�Ů��ϵ
const int RELATION_FATHER_DAUGHTER_OLD = 12;
//��ͥ
const int RELATION_FAMILY = 13;
//һȺ��
const int RELATION_GROUP = 14;
//һ����
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
	//�Ա�
	int sex;
	//����
	int age;
	//��ǰ���Ĺ��ı��
	int ad_id;
	//���
	double depth;
	//��Ժ�����
	double x;
	cv::Mat mat;
}PersonMomentState;

//������Ϣ
typedef struct Weather
{
	//������
	std::string city;
	//����id
	std::string city_id;
	//�¶�
	double temperature;
	//����
	std::string windy_direction;
	//����
	double windy_strength;
	//ʪ��
	double humidity;
	//����ʱ��
	std::string time;
	//��ѹ
	double pressure;
	//�Ƿ�����
	bool rain;
} Weather;

//�����ͳ����Ϣ
typedef struct PersonInformation
{
	//����ID
	int id;
	//���ֵ�֡��
	long long int frameCount;
	//ͣ����ʱ��
	double sumTime;
	//��һ֡ʱ��
	__int64 preTime;
	//���ֵ�ʱ��
	time_t startTime;
	long long int addTime;
	//�����Ѵ���ʱ��
	long long int stay_time;
	//΢Ц��ʱ��
	double happyTime;
	//engaged��ʱ��
	double engagedTime;
	//������Ƶ�ʱ��
	double watchTime;
	//���۱յ�ʱ��
	double leftEyeCloseTime;
	//���۱յ�ʱ��
	double rightEyeCloseTime;
	//��Ͷ���ʱ��
	double mouthMovedTime;
	//����ſ���ʱ��
	double mouthOpenTime;
	//����
	int age;
	//�Ƿ���۾��ļ���
	int wearGT, NwearGT;
	//�Ա�
	int sex;
	//�Ա����
	int sex_male, sex_female;
	//�Ƿ���۾�
	bool wearGlasses;
	//�����Ƿ�΢Ц
	int expression;
	//�����Ƿ���Զ��
	bool lookaway;
	//�Ƿ�ע��
	bool engaged;
	//�Ƿ���������
	bool leftEyeOpen;
	//�Ƿ���������
	bool rightEyeOpen;
	//�Ƿ���Ͷ�
	bool mouthMoved;
	//�Ƿ�����ſ�
	bool mouthOpen;
	//�Ƿ���һ֡����
	bool checked;
	//���
	double depth;
	//��Ժ�����
	double x;
	//������Ⱥid
	int group;
	//����ź�ʱ��
	std::map<int, long long int> ad_list;
	//��ǰ����
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

//���ﶯ̬��Ϣ
typedef struct PersonMoment {
	//group��Ϣ
	int group;
	//��ǰ֡�Ƿ����
	bool appear;
	//id
	int id;
	//����ʱ��
	long long int start_time;
	// �Ա�
	int gender;
	// ����
	int age;
	// ����
	int expression;
	// רע
	bool engaged;
	// �۾�
	bool wearingGlasses;
	//��ͷ����
	int z;
	//������
	bool dis_change;
	//������
	RectI faceBox;
	//δ���ֵ�֡��
	int disapper_time;
	//ͼƬ
	cv::Mat faceImg;
}PersonMoment;

//����ѵ������
typedef struct PersonInfoTraining
{
	//��עʱ��
	long long int watch_time;
	//�Ա�
	int gender;
	// ����
	int age;
	// ����
	int expression;
	// רע
	bool engaged;
	// �۾�
	bool wearingGlasses;
};

//��Ⱥ��Ϣ
typedef struct Group
{
	//�������
	std::vector<PersonInformation*> persons;
	//�ۺ�Ȩֵ
	double weight;
	//�����ϵ
	int relation;
	//��Ⱥid
	int id;
} Group;

//���ı�ǩ�ַ������ŵĶ�Ӧ��ϵ
typedef std::map<std::string, int> map_ad_label;
//����б�
typedef std::vector<Advertisement> vector_ad_list;
//�˴���Ϣ����
typedef std::queue<MomentPersonState> queue_moment_person_state;
//������Ϣ�б�
typedef std::vector<PersonInformation> vector_person_information;
//������Ⱥ�б�
typedef std::vector<Group> vector_group;
//���뿪�����б�
typedef std::vector<PersonInformation> vector_person_left;

#endif

