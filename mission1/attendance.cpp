#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
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

struct BaseballMemberInfo {
	int point;
	int attend_wed_cnt;
	int attend_weeken_cnt;
	GRADE grade;

	BaseballMemberInfo() : point(0), attend_wed_cnt(0), attend_weeken_cnt(0), grade(GRADE::NORMAL) {}
};
std::unordered_map<string, BaseballMemberInfo> baseballMember;

void AddMemberPoint(string memberName, string attendDay) {
	if (attendDay == "wednesday") {
		baseballMember[memberName].attend_wed_cnt++;
		baseballMember[memberName].point += WEDNESDAY_POINT;
	}
	else if (attendDay == "saturday" || attendDay == "sunday") {
		baseballMember[memberName].attend_weeken_cnt++;
		baseballMember[memberName].point += WEEKEND_POINT;
	}
	else {
		baseballMember[memberName].point += NORMAL_DAYS_POINT;
	}
}

void AddMemberbonusPoint() {
	for (auto& member : baseballMember) {
		if (member.second.attend_wed_cnt >= BONUS_REQUIRE_DAYS) {
			member.second.point += BONUS_POINT;
		}
		if (member.second.attend_weeken_cnt >= BONUS_REQUIRE_DAYS) {
			member.second.point += BONUS_POINT;
		}
	}
}

void MemberGradeCheck() {
	for (auto& member : baseballMember) {
		if (member.second.point >= GOLD_GRADE_REQUIRE_POINT) {
			member.second.grade = GRADE::GOLD;
		}
		else if (member.second.point >= SILVER_GRADE_REQUIRE_POINT) {
			member.second.grade = GRADE::SILVER;
		}
		else {
			member.second.grade = GRADE::NORMAL;
		}
	}
}

ERROR_CODE CalculationMemberPoint(string attendanceDataFile) {
	std::ifstream attendanceData{ attendanceDataFile };
	string info, memberName, attendDay;
	if (attendanceData.is_open()) {
		while (getline(attendanceData, info)) {
			std::stringstream ss(info);
			if (ss >> memberName >> attendDay) {
				AddMemberPoint(memberName, attendDay);
			}
			else {
				cout << "Invalid line format: " << info << "\n";
				return ERROR_CODE::INVALID_FORMAT;
			}
		}
		attendanceData.close();
	}
	else {
		std::cout << "Invalid file: " << attendanceDataFile << std::endl;
		return ERROR_CODE::INVALID_FILE;
	}

	AddMemberbonusPoint();

	return ERROR_CODE::SUCCESS;
}

bool isRemovedPlayer(BaseballMemberInfo member) {
	return (member.grade == GRADE::NORMAL && member.attend_wed_cnt == 0 && member.attend_weeken_cnt == 0);
}

void ResultPrint() {
	for (const auto& member : baseballMember) {
		cout << "NAME : " << member.first << ", ";
		cout << "POINT : " << member.second.point << ", ";
		cout << "GRADE : ";

		if (member.second.grade == GRADE::GOLD) {
			cout << "GOLD" << "\n";
		}
		else if (member.second.grade == GRADE::SILVER) {
			cout << "SILVER" << "\n";
		}
		else {
			cout << "NORMAL" << "\n";
		}
	}

	cout << "\n";
	cout << "Removed player\n";
	cout << "==============\n";
	for (const auto& member : baseballMember) {
		if (isRemovedPlayer(member.second)) {
			cout << member.first << "\n";
		}
	}

}

int main() {
	string file;
	cout << "BaseBall Member Attendance Data File Path: ";
	std::cin >> file;

	if (CalculationMemberPoint(file) == ERROR_CODE::SUCCESS) {
		MemberGradeCheck();
		ResultPrint();
	}

}