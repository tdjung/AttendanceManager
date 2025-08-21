#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

using std::cout;
using std::string;

enum class ERROR_CODE { SUCCESS, INVALID_FORMAT, INVALID_FILE };
enum class GRADE { NORMAL, SILVER, GOLD };

constexpr int WEDNESDAY_POINT = 3;
constexpr int WEEKEND_POINT = 2;
constexpr int NORMAL_DAYS_POINT = 1;

constexpr int BONUS_REQUIRE_DAYS = 10;
constexpr int BONUS_POINT = 10;

constexpr int GOLD_GRADE_REQUIRE_POINT = 50;
constexpr int SILVER_GRADE_REQUIRE_POINT = 30;

class BaseballMemberInfo {
public:
	int point;
	std::unordered_map<string, int> attend_day;
	string grade;

	BaseballMemberInfo() : point(0), grade("NORMAL") {}
};

class IScoreDayPolicy {
public:
	virtual int getDayScore(string day) = 0;
	virtual void setDayScore(int score, string day) = 0;
	virtual ~IScoreDayPolicy() = default;
};
class IGradeRequirePolicy {
public:
	virtual int getGradeRequireScore(string grade) = 0;
	virtual void setGradeRequireScore(int score, string grade) = 0;
	virtual void newGrade(int score, string grade) = 0;
	virtual string getGrade(int score) = 0;
	virtual ~IGradeRequirePolicy() = default;
};
class IBonusRequirePolicy {
public:
	virtual int getBonusScoreOfMember(string name, BaseballMemberInfo member_info) = 0;
	virtual void setBonusRequirements(int score, string day) = 0;
	virtual void newBonusDay(int score, string day) = 0;
	virtual ~IBonusRequirePolicy() = default;
};
class IRemovedRequirePolicy {
public:
	virtual void setRemovedGrade(string Grade) = 0;
	virtual bool isRemovedPlayer(BaseballMemberInfo member_info) = 0;
	virtual ~IRemovedRequirePolicy() = default;
};

class ScoreDayPolicy : public IScoreDayPolicy {
public:
	int getDayScore(string day);
	void setDayScore(int score, string day);
private:
	std::unordered_map<string, int> score_of_days;
};

class GradeRequirePolicy : public IGradeRequirePolicy {
public:
	int getGradeRequireScore(string grade);
	void setGradeRequireScore(int score, string grade);
	void newGrade(int score, string grade);
	string getGrade(int score);
private:
	std::unordered_map<string, int> grade_require_point;
};

class BonusRequirePolicy : public IBonusRequirePolicy {
public:
	int getBonusScoreOfMember(string name, BaseballMemberInfo member_info);
	void setBonusRequirements(int score, string day);
	void newBonusDay(int score, string day);
private:
	std::unordered_map<string, int> bonus_day;
};

class RemovedRequirePolicy : public IRemovedRequirePolicy {
public:
	void setRemovedGrade(string grade);
	bool isRemovedPlayer(BaseballMemberInfo member_info);
private:
	string removedGrade;
};



class AttendanceManager {
public:
	AttendanceManager(ScoreDayPolicy* score_day_policy, GradeRequirePolicy* grade_require_policy, BonusRequirePolicy* bonus_require_policy, RemovedRequirePolicy* remove_require_policy);
	void newAttendanceInfo(string name, string day);
	string getMemberInfo(string name);
	string getRemovedPlayer();
	void curMemberStatePrint();
	IScoreDayPolicy* score_day_policy;
	IGradeRequirePolicy* grade_require_policy;
	IBonusRequirePolicy* bonus_require_policy;
	IRemovedRequirePolicy* remove_require_policy;

	std::unordered_map<string, BaseballMemberInfo> baseball_member;

};




