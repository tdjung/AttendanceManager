
#include "attendance.h"

using std::string;
using std::cout;

AttendanceManager::AttendanceManager(ScoreDayPolicy* score_day_policy, GradeRequirePolicy* grade_require_policy, BonusRequirePolicy* bonus_require_policy, RemovedRequirePolicy* remove_require_policy)
	: score_day_policy(score_day_policy), grade_require_policy(grade_require_policy), bonus_require_policy(bonus_require_policy), remove_require_policy(remove_require_policy) {
	score_day_policy->setDayScore(1, "monday");
	score_day_policy->setDayScore(1, "tuesday");
	score_day_policy->setDayScore(3, "wednesday");
	score_day_policy->setDayScore(1, "thursday");
	score_day_policy->setDayScore(1, "friday");
	score_day_policy->setDayScore(2, "saturday");
	score_day_policy->setDayScore(2, "sunday");

	grade_require_policy->newGrade(50, "GOLD");
	grade_require_policy->newGrade(30, "SILVER");
	
	bonus_require_policy->newBonusDay(10, "wednesday");
	bonus_require_policy->newBonusDay(10, "weekend");

}
int ScoreDayPolicy::getDayScore(string day) {
	if (score_of_days.find(day) == score_of_days.end())
		return -1;
	 
	return score_of_days[day];
}
void ScoreDayPolicy::setDayScore(int score, string day) {
	score_of_days[day] = score;
}
int GradeRequirePolicy::getGradeRequireScore(string grade) {
	if (grade_require_point.find(grade) == grade_require_point.end())
		return -1;

	return grade_require_point[grade];
}
void GradeRequirePolicy::setGradeRequireScore(int score, string grade) {
	if (grade_require_point.find(grade) == grade_require_point.end())
		return;

	grade_require_point[grade] = score;
}
void GradeRequirePolicy::newGrade(int score, string grade) {
	if (grade_require_point.find(grade) != grade_require_point.end())
		return;

	grade_require_point[grade] = score;
}

string GradeRequirePolicy::getGrade(int score) {
	int cur_score = 0;
	string cur_grade = "NORMAL";
	for (auto& [grade, require] : grade_require_point) {
		if ((require > cur_score) && (score >= require)) {
			cur_grade = grade;
			cur_score = require;
		}
	}
	
	return cur_grade;
}


void BonusRequirePolicy::setBonusRequirements(int score, string day) {
	if (bonus_day.find(day) == bonus_day.end())
		return;

	bonus_day[day] = score;

}
int BonusRequirePolicy::getBonusScoreOfMember(string name, BaseballMemberInfo member_info) {
	int bonusScore = 0;
	bool is_find_weekend = false;
	for (auto& [day, require] : bonus_day) {
		if (member_info.attend_day[day] >= require)
			bonusScore += 10;

		if (is_find_weekend == false && day == "weekend") {
			if (member_info.attend_day["saturday"] >= require || member_info.attend_day["sunday"] >= require) {
				bonusScore += 10;
				is_find_weekend = true;
			}
		}
	}

	return bonusScore;
}
void BonusRequirePolicy::newBonusDay(int score, string day) {
	if (bonus_day.find(day) != bonus_day.end())
		return;

	bonus_day[day] = score;
}

void RemovedRequirePolicy::setRemovedGrade(string grade) {
	removedGrade = grade;
}

bool RemovedRequirePolicy::isRemovedPlayer(BaseballMemberInfo member_info){ 
	return (member_info.grade == "NORMAL" && member_info.attend_day["wednesday"] ==0 && member_info.attend_day["saturday"] && member_info.attend_day["sunday"] == 0);
}

void AttendanceManager::newAttendanceInfo(string name, string day) {
	baseball_member[name].point += score_day_policy->getDayScore(day);
	baseball_member[name].attend_day[day] += 1;
	
	int cur_point = 0;
	baseball_member[name].grade = grade_require_policy->getGrade(baseball_member[name].point);
}

string AttendanceManager::getMemberInfo(string name) {
	if(baseball_member.find(name) == baseball_member.end())
		return "";

	baseball_member[name].grade = grade_require_policy->getGrade(baseball_member[name].point);
	int bonus = bonus_require_policy->getBonusScoreOfMember(name, baseball_member[name]);
	string member_info = "NAME : " + name + ", POINT : " + std::to_string(baseball_member[name].point+ bonus) + ", GRADE : "  + baseball_member[name].grade + "\n";
	return member_info;
}
string AttendanceManager::getRemovedPlayer() {
	string removedPlayer = "Removed Player\n==============\n";
	for (auto& [name, memberInfo] : baseball_member) {
		string grade = memberInfo.grade;
		
		if (memberInfo.attend_day.find("wednesday") != memberInfo.attend_day.end())
			continue;

		if (memberInfo.attend_day.find("sunday") != memberInfo.attend_day.end())
			continue;

		if (memberInfo.attend_day.find("saturday") != memberInfo.attend_day.end())
			continue;

		if (grade == "NORMAL"){
			removedPlayer += name + "\n";
		}
	}

	return removedPlayer;
}


void AttendanceManager::curMemberStatePrint() {

	for (auto& [name, info] : baseball_member) {
		cout << getMemberInfo(name);
	}

	getRemovedPlayer();
}

