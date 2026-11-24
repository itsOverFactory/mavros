/**
 * @brief Track Point plugin
 * @file track_point.cpp
 * @author Burak Ozter <43870713+bcanozter@users.noreply.github.com>
 *
 * @addtogroup plugin
 * @{
 */

#include "mavros/mavros_uas.hpp"
#include "mavros/plugin.hpp"
#include "mavros/plugin_filter.hpp"

#include "mavros_msgs/msg/track_point.hpp"

namespace mavros
{
namespace extra_plugins
{
using namespace std::placeholders;

static constexpr uint16_t MAV_CMD_TRACK_POINT = 31999;

class TrackPointPlugin : public plugin::Plugin
{
public:
  explicit TrackPointPlugin(plugin::UASPtr uas_)
  : Plugin(uas_, "track_point")
  {
    track_point_pub = node->create_publisher<mavros_msgs::msg::TrackPoint>("~/point", 10);
  }

  Subscriptions get_subscriptions() override
  {
    return {
      make_handler(&TrackPointPlugin::handle_command_long)
    };
  }

private:
  rclcpp::Publisher<mavros_msgs::msg::TrackPoint>::SharedPtr track_point_pub;


  void handle_command_long(
    const mavlink::mavlink_message_t * msg [[maybe_unused]],
    mavlink::common::msg::COMMAND_LONG & cmd,
    plugin::filter::AnyOk filter [[maybe_unused]])
  {
    
    if (cmd.command != MAV_CMD_TRACK_POINT) {
      return;
    }

    if (!uas->is_addressed_to_us(cmd.target_system, cmd.target_component)) {
      return;
    }

    auto tp = mavros_msgs::msg::TrackPoint();

    tp.header.stamp = node->now();
    tp.x = cmd.param1;
    tp.y = cmd.param2;
    tp.radius = cmd.param3;
    tp.camera_id = static_cast<uint8_t>(cmd.param4);

    track_point_pub->publish(tp);

    mavlink::common::msg::COMMAND_ACK ack{};
    ack.command = cmd.command;
    ack.result = utils::enum_value(mavlink::common::MAV_RESULT::ACCEPTED);
    ack.target_system = msg->sysid;
    ack.target_component = msg->compid;

    uas->send_message(ack);
  }
};

}       // namespace extra_plugins
}       // namespace mavros

#include <mavros/mavros_plugin_register_macro.hpp>  // NOLINT
MAVROS_PLUGIN_REGISTER(mavros::extra_plugins::TrackPointPlugin)
