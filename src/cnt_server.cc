#include "action_learning/cntAction.h"
#include <actionlib/server/simple_action_server.h>
#include <ros/ros.h>

class CountToNumberAction {
protected:
  ros::NodeHandle nh_; // ROS节点句柄，用于与ROS系统交互
  actionlib::SimpleActionServer<action_learning::cntAction>
      as_; // Action 服务器对象，类型是自定义的 `CountToNumberAction`
  std::string action_name_;               // Action的名称
  action_learning::cntFeedback feedback_; // 存储反馈的对象
  action_learning::cntResult result_;     // 存储结果的对象

public:
  CountToNumberAction(std::string name)
      : as_(nh_, name, boost::bind(&CountToNumberAction::executeCB, this, _1),
            false),
        action_name_(name) {
    as_.start();
  }

  void executeCB(const action_learning::cntGoalConstPtr &goal) {
    int target = goal->target_number;   // 获取目标数字
    ROS_INFO("Counting to %d", target); // 打印目标数字

    // 从 0 开始计数并提供反馈
    for (int i = 0; i <= target; ++i) {
      if (as_.isPreemptRequested()) {
        ROS_INFO("%s: Preempted", action_name_.c_str());
        as_.setPreempted(); // 如果请求了中断，则设置为已中断
        return;
      }

      feedback_.curr_cnt = i;         // 更新当前计数
      as_.publishFeedback(feedback_); // 发布反馈
      ros::Duration(1.0).sleep();     // 每秒更新一次反馈
    }

    // 返回结果
    result_.success = true;    // 设置操作成功
    as_.setSucceeded(result_); // 设置 Action 成功并返回结果
  }
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "count_to_number_action_server"); // 初始化ROS节点
  CountToNumberAction count_to_number_action(
      "count_to_number"); // 创建 Action 服务器实例
  ros::spin();            // 进入 ROS 事件循环，等待请求
  return 0;
}