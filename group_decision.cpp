#include "group_decision.h"

int group_decision::tree_predict(vvs tableInfo, string default_class, node* root)
{
	vector<string>info;
	string num_str = to_string(num);
	string age_str = to_string(age_diff);
	string gender_str = to_string(gender_diff);

	info.insert(info.end(), num_str);
	info.insert(info.end(), age_str);
	info.insert(info.end(), gender_str);

	string result = testDataOnDecisionTree(info, root, tableInfo, default_class);
	return atoi(result.c_str());
}


