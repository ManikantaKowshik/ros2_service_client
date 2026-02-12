#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using namespace std::chrono_literals;

class AddTwoIntsClient : public rclcpp::Node {
public:
    AddTwoIntsClient() : Node("add_two_ints_client") {
        client_ = this->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints");
    }

    // Explicitly using the SharedFuture type to avoid deduction errors
    using ServiceResponseFuture = rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedFuture;

    ServiceResponseFuture send_request(int64_t a, int64_t b) {
        while (!client_->wait_for_service(1s)) {
            if (!rclcpp::ok()) {
                RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for service.");
                return ServiceResponseFuture(); 
            }
            RCLCPP_INFO(this->get_logger(), "Waiting for service to become available...");
        }

        auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
        request->a = a;
        request->b = b;

        RCLCPP_INFO(this->get_logger(), "Service available, sending request...");
        
        // In Jazzy, we call .share() to get a SharedFuture from the RequestID object
        return client_->async_send_request(request).share();
    }

private:
    rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<AddTwoIntsClient>();

    auto result_future = node->send_request(41, 1);

    if (rclcpp::spin_until_future_complete(node, result_future) == rclcpp::FutureReturnCode::SUCCESS) {
        auto response = result_future.get();
        RCLCPP_INFO(node->get_logger(), "Result: 41 + 1 = %ld", response->sum);
    } else {
        RCLCPP_ERROR(node->get_logger(), "Failed to call service add_two_ints");
    }

    rclcpp::shutdown();
    return 0;
}
