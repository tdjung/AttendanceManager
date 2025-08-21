#include "attendance.h"

#if _DEBUG

#include <gmock/gmock.h>

class AttendanceManagerTestFixture : public ::testing::Test {
public:
    AttendanceManagerTestFixture() {
        //
    }

    ~AttendanceManagerTestFixture() {
       //
    }
protected:
    ScoreDayPolicy score_day_policy;
    GradeRequirePolicy grade_require_policy;
    BonusRequirePolicy bonus_require_policy;
    RemovedRequirePolicy remove_require_policy;
   
    virtual void SetUp() override {
      //
    }

    virtual void TearDown() override {
       //
    }
};


TEST_F(AttendanceManagerTestFixture, DayScoreTest) { 
    AttendanceManager manager_program(&score_day_policy, &grade_require_policy, &bonus_require_policy, &remove_require_policy);

    EXPECT_EQ(-1, manager_program.score_day_policy->getDayScore("errorday"));
  
    int wendesday_score = 2;
    manager_program.score_day_policy->setDayScore(wendesday_score, "wednesday");
    EXPECT_EQ(wendesday_score, manager_program.score_day_policy->getDayScore("wednesday"));

    int monday_default_score = 1;
    EXPECT_EQ(monday_default_score, manager_program.score_day_policy->getDayScore("monday"));
    
    int monday_score = 3;
    manager_program.score_day_policy->setDayScore(monday_score, "monday");
    EXPECT_EQ(monday_score, manager_program.score_day_policy->getDayScore("monday"));
}

TEST_F(AttendanceManagerTestFixture, GradeTest) {
    AttendanceManager manager_program(&score_day_policy, &grade_require_policy, &bonus_require_policy, &remove_require_policy);

    EXPECT_EQ(-1, manager_program.grade_require_policy->getGradeRequireScore("ABB"));
    EXPECT_EQ(50, manager_program.grade_require_policy->getGradeRequireScore("GOLD"));
    EXPECT_EQ(30, manager_program.grade_require_policy->getGradeRequireScore("SILVER"));

    manager_program.grade_require_policy->setGradeRequireScore(20, "ABC");
    manager_program.grade_require_policy->setGradeRequireScore(20, "SILVER");
    EXPECT_EQ(20, manager_program.grade_require_policy->getGradeRequireScore("SILVER"));

    manager_program.grade_require_policy->newGrade(15, "BRONZE");
    manager_program.grade_require_policy->newGrade(15, "BRONZE");
    EXPECT_EQ(15, manager_program.grade_require_policy->getGradeRequireScore("BRONZE"));
}

TEST_F(AttendanceManagerTestFixture, BonusTest) {
    AttendanceManager manager_program(&score_day_policy, &grade_require_policy, &bonus_require_policy, &remove_require_policy);

    int defaultBonusRequirements = 10;
    for (int i = 0; i < defaultBonusRequirements; i++) {
        manager_program.newAttendanceInfo("testName", "wednesday");
    }
    manager_program.bonus_require_policy->newBonusDay(10, "weekend");

    

    EXPECT_EQ(10, manager_program.bonus_require_policy->getBonusScoreOfMember("testName", manager_program.baseball_member["testName"]));

    int newDefaultBonusRequirements = 20;
    manager_program.bonus_require_policy->setBonusRequirements(newDefaultBonusRequirements, "ABC");
    manager_program.bonus_require_policy->setBonusRequirements(newDefaultBonusRequirements, "wednesday");
    EXPECT_EQ(0, manager_program.bonus_require_policy->getBonusScoreOfMember("testName", manager_program.baseball_member["testName"]));

    for (int i = 0; i < newDefaultBonusRequirements - defaultBonusRequirements; i++) {
        manager_program.newAttendanceInfo("testName", "wednesday");
    }
    EXPECT_EQ(10, manager_program.bonus_require_policy->getBonusScoreOfMember("testName", manager_program.baseball_member["testName"]));

    int newBonusDayRequirments = 5;
    for (int i = 0; i < newBonusDayRequirments; i++) {
        manager_program.newAttendanceInfo("testName", "monday");
    }
    EXPECT_EQ(10, manager_program.bonus_require_policy->getBonusScoreOfMember("testName", manager_program.baseball_member["testName"]));
    manager_program.bonus_require_policy->newBonusDay(newBonusDayRequirments, "monday");
    manager_program.bonus_require_policy->newBonusDay(newBonusDayRequirments, "monday");
    EXPECT_EQ(20, manager_program.bonus_require_policy->getBonusScoreOfMember("testName", manager_program.baseball_member["testName"]));
    for (int i = 0; i < defaultBonusRequirements; i++) {
        manager_program.newAttendanceInfo("testName", "sunday");
    }
    EXPECT_EQ(30, manager_program.bonus_require_policy->getBonusScoreOfMember("testName", manager_program.baseball_member["testName"]));
}

TEST_F(AttendanceManagerTestFixture, MemberInfoTest) {
    AttendanceManager manager_program(&score_day_policy, &grade_require_policy, &bonus_require_policy, &remove_require_policy);

    EXPECT_STREQ("", manager_program.getMemberInfo("testName").c_str());

    manager_program.newAttendanceInfo("testName", "monday");
    EXPECT_STREQ("NAME : testName, POINT : 1, GRADE : NORMAL\n", manager_program.getMemberInfo("testName").c_str());

    manager_program.newAttendanceInfo("testName", "monday");
    EXPECT_STREQ("NAME : testName, POINT : 2, GRADE : NORMAL\n", manager_program.getMemberInfo("testName").c_str());

    for(int i=0; i<28; i++)
        manager_program.newAttendanceInfo("testName", "monday");
    EXPECT_STREQ("NAME : testName, POINT : 30, GRADE : SILVER\n", manager_program.getMemberInfo("testName").c_str());
}

TEST_F(AttendanceManagerTestFixture, RemovedMemberTest) {
    AttendanceManager manager_program(&score_day_policy, &grade_require_policy, &bonus_require_policy, &remove_require_policy);

    manager_program.remove_require_policy->setRemovedGrade("NORMAL");
    EXPECT_STREQ("Removed Player\n==============\n", manager_program.getRemovedPlayer().c_str());
    manager_program.newAttendanceInfo("testName", "monday");
    EXPECT_STREQ("Removed Player\n==============\ntestName\n", manager_program.getRemovedPlayer().c_str());
    manager_program.newAttendanceInfo("testName2", "monday");
    EXPECT_STREQ("Removed Player\n==============\ntestName\ntestName2\n", manager_program.getRemovedPlayer().c_str());
    manager_program.newAttendanceInfo("testName", "wednesday");
    EXPECT_STREQ("Removed Player\n==============\ntestName2\n", manager_program.getRemovedPlayer().c_str());

    for(int i=0; i<28; i++)
        manager_program.newAttendanceInfo("testName2", "monday");

    EXPECT_STREQ("Removed Player\n==============\ntestName2\n", manager_program.getRemovedPlayer().c_str());
    manager_program.newAttendanceInfo("testName2", "monday");
    EXPECT_STREQ("Removed Player\n==============\n", manager_program.getRemovedPlayer().c_str());
    EXPECT_EQ(false, manager_program.remove_require_policy->isRemovedPlayer(manager_program.baseball_member["testName"]));
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

#else

int main() {
    string file;
    cout << "BaseBall Member Attendance Data File Path: ";
    std::cin >> file;

    ScoreDayPolicy score_day_policy;
    GradeRequirePolicy grade_require_policy;
    BonusRequirePolicy bonus_require_policy;
    RemovedRequirePolicy remove_require_policy;
    AttendanceManager manager_program(&score_day_policy, &grade_require_policy, &bonus_require_policy, &remove_require_policy);

    std::ifstream attendanceData{ file };
    string info, memberName, attendDay;

    if (attendanceData.is_open()) {
        while (getline(attendanceData, info)) {
            std::stringstream ss(info);
            if (ss >> memberName >> attendDay) {
                manager_program.newAttendanceInfo(memberName, attendDay);
            }
            else {
                return 0;
            }
        }
        attendanceData.close();
    }
    else {
        return 0;
    }

    manager_program.curMemberStatePrint();
    
    return 0;
}

#endif
