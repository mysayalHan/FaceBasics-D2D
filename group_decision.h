#pragma once
#ifndef GROUP_DECISION_H
#define GROUP_DECISION_H
#include <string>
#include <iostream>
#include "decision_tree.h"
#include<stdlib.h>

using namespace std;

class group_decision 
{
public:
	//����
	int num;
	//������
	int age_diff;
	//�Ա���
	int gender_diff;

	int tree_predict(vvs tableInfo, string default_class, node* root);
	
};
#endif // !GROUP_DECISION_H
