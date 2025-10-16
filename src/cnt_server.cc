#include "action_learning/cntAction.h"
#include <actionlib/server/simple_action_server.h>
#include <ros/ros.h>

class CountToNumberAction {
protected:
  ros::NodeHandle nh_; // ROS�ڵ�����������ROSϵͳ����
  actionlib::SimpleActionServer<action_learning::cntAction>
      as_; // Action �����������������Զ���� `CountToNumberAction`
  std::string action_name_;               // Action������
  action_learning::cntFeedback feedback_; // �洢�����Ķ���
  action_learning::cntResult result_;     // �洢����Ķ���

public:
  CountToNumberAction(std::string name)
      : as_(nh_, name, boost::bind(&CountToNumberAction::executeCB, this, _1),
            false),
        action_name_(name) {
    as_.start();
  }

  void executeCB(const action_learning::cntGoalConstPtr &goal) {
    int target = goal->target_number;   // ��ȡĿ������
    ROS_INFO("Counting to %d", target); // ��ӡĿ������

    // �� 0 ��ʼ�������ṩ����
    for (int i = 0; i <= target; ++i) {
      if (as_.isPreemptRequested()) {
        ROS_INFO("%s: Preempted", action_name_.c_str());
        as_.setPreempted(); // ����������жϣ�������Ϊ���ж�
        return;
      }

      feedback_.curr_cnt = i;         // ���µ�ǰ����
      as_.publishFeedback(feedback_); // ��������
      ros::Duration(1.0).sleep();     // ÿ�����һ�η���
    }

    // ���ؽ��
    result_.success = true;    // ���ò����ɹ�
    as_.setSucceeded(result_); // ���� Action �ɹ������ؽ��
  }
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "count_to_number_action_server"); // ��ʼ��ROS�ڵ�
  CountToNumberAction count_to_number_action(
      "count_to_number"); // ���� Action ������ʵ��
  ros::spin();            // ���� ROS �¼�ѭ�����ȴ�����
  return 0;
}