#include "action_learning/cntAction.h"
#include <actionlib/client/simple_action_client.h>
#include <ros/ros.h>

typedef actionlib::SimpleActionClient<action_learning::cntAction> Client;

// doneCb - ������ɺ�Ļص�
void doneCb(const actionlib::SimpleClientGoalState &state,
            const action_learning::cntResultConstPtr &result) {
  ROS_INFO("Finished in state: %s", state.toString().c_str());
  if (result->success)
    ROS_INFO("Counting completed successfully!");
  else
    ROS_WARN("Counting failed.");
}

/**
 * activeCb����Ŀ�꿪ʼ������������ʱ�������˻ص���ʾĿ���Ѿ������ܲ����ڴ���
   ���򵥵ش�ӡһ����Ϣ����ʾĿ�����ڱ�����
 */
void activeCb() { ROS_INFO("Goal is being processed..."); }

// feedbackCb - �������µĻص�
void feedbackCb(const action_learning::cntFeedbackConstPtr &feedback) {
  ROS_INFO("Current count: %d", feedback->curr_cnt);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "count_to_number_action_client");

  // ����һ���ͻ��ˣ����ӵ�������
  Client ac("count_to_number", true);
  ROS_INFO("Waiting for action server to start...");
  ac.waitForServer(); // �ȴ�����������

  action_learning::cntGoal goal;
  goal.target_number = 10; // ����Ŀ������

  // ����Ŀ�겢���ûص�����
  ac.sendGoal(goal, &doneCb, &activeCb, &feedbackCb);

  ros::spin();
  return 0;
}