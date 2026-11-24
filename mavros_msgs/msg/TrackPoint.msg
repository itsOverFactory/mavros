# MAV_CMD_TRACK_POINT (31999)

std_msgs/Header header
float32 x           # Point to track x value (normalized 0..1, 0 is left, 1 is right).
float32 y           # Point to track y value (normalized 0..1, 0 is top, 1 is bottom).
float32 radius      # Point radius (normalized 0..1, 0 is one pixel, 1 is full image width).
uint8 camera_id     # Target camera ID. 7 to 255: MAVLink camera component id. 1 to 6 for cameras attached to the autopilot, 0: all cameras.
