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
	//人数
	int num;
	//年龄差别
	int age_diff;
	//性别差别
	int gender_diff;

	int tree_predict(vvs tableInfo, string default_class, node* root);
	
};
#endif // !GROUP_DECISION_H
