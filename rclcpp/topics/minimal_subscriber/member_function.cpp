```jsx
// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <functional>
#include <memory>

//사용된 개념
//상속, 생성자, 콜백함수, 바인드

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;
//Placeholder와 std::bind를 사용할 수 있다.
// _1을 플래이스홀더로 정의한다.

class MinimalSubscriber : public rclcpp::Node // MinimalSubscriber inherits from Node
{
public:
//구현 부분이다.
  MinimalSubscriber()
  : Node("minimal_subscriber")
  {
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "topic", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
    // MinimalSubscriber 클래스가 rclcpp::Node를 상속받기 때문에, 
    //MinimalSubscriber의 인스턴스(객체)는 
    //rclcpp::Node 클래스의 메서드와 멤버 변수에 접근할 수 있기 때문에
    //rclcpp::Node에 정의된 create_subscription 메서드를 호출하고
    //create_subscription의 템플릿 인수인 
    //<std_msgs::msg::String> 타입의 메시지를 구독할 수 있다.

    //MinimalSubscriber의 인스턴스가 rclcpp::node상속
    // -> create_subscription 메서드 호출
    // -> <std_msgs::msg::String> 타입의 메시지를 구독하는 구독 객체를 반환
    // subscription_에 반환
    //("토픽 이름", 메시지 갯수, bind함수())

    //this->create_subscription<std_msgs::msg::String>()
    //의 세 번째 인자는 구독한 토픽에서 메시지를 받을 때 호줄되는 콜백 함수이다.

    //std::bind(&MinimalSubscriber::topic_callback, this, _1)는
    //MinimalSubscriber::topic_callback을 호출할 수 있는 함수 객체를 생성하고
    //이 객체는 구독 객체의 콜백 함수로 사용된다.

    //_1은 플레이스홀더로 함수 객체를 호출할 때의 첫 번째 인수를 대체한다.
    //새 메시지가 수신될 때마다 _1은 메시지로 대체되어 topic_callback으로 전달된다.
    //(여기에서는, topic_callback함수가 메시지를 출력한다.)
  }

private:
//정의 부분이다.
  void topic_callback(const std_msgs::msg::String & msg) const
  //topic_Callback을 정의한다
  {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
    //topic_callback은 메시지를 출력한다.
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  //subscription_을 정의한다.
  //subscription은 SharedPtr로 정의되어 있고, <std_msg::String>형식의 메시지를 갖는다.
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  //MinimalSubscriber 객체를 생성하고
  //std::shared_ptr<MinimalSubscriber>을 반환한다.
  // 반환된 객체는 rclcpp::spin()에 인자로 전달된다.

  //rclcpp::spin()함수는 인자인 노드가 실행되는동안 노드의 콜백 함수를 처리한다.
  // (topic_callback함수)
  rclcpp::shutdown();
  return 0;
}

//MinimalSubscriber 객체는 각각의 구문에서 지역 변수로 사용되었다.
```