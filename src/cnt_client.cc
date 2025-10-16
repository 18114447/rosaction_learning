#include "action_learning/cntAction.h"
#include <actionlib/client/simple_action_client.h>
#include <ros/ros.h>

typedef actionlib::SimpleActionClient<action_learning::cntAction> Client;

// doneCb - 任务完成后的回调
void doneCb(const actionlib::SimpleClientGoalState &state,
            const action_learning::cntResultConstPtr &result) {
  ROS_INFO("Finished in state: %s", state.toString().c_str());
  if (result->success)
    ROS_INFO("Counting completed successfully!");
  else
    ROS_WARN("Counting failed.");
}

/**
 * activeCb：当目标开始被服务器处理时触发。此回调表示目标已经被接受并正在处理。
   它简单地打印一条信息，表示目标正在被处理。
 */
void activeCb() { ROS_INFO("Goal is being processed..."); }

// feedbackCb - 反馈更新的回调
void feedbackCb(const action_learning::cntFeedbackConstPtr &feedback) {
  ROS_INFO("Current count: %d", feedback->curr_cnt);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "count_to_number_action_client");

  // 创建一个客户端，连接到服务器
  Client ac("count_to_number", true);
  ROS_INFO("Waiting for action server to start...");
  ac.waitForServer(); // 等待服务器启动

  action_learning::cntGoal goal;
  goal.target_number = 10; // 设置目标数字

  // 发送目标并设置回调函数
  ac.sendGoal(goal, &doneCb, &activeCb, &feedbackCb);

  ros::spin();
  return 0;
}