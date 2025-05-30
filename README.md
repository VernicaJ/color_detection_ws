# ROS 2 Webcam Driver & Color Filter

## Packages

### `apple`

ROS 2 Python node that captures video from the webcam and publishes it as a `sensor_msgs/msg/Image`.

### `blue`

ROS 2 C++ node that subscribes to webcam image, replaces a sepecified color and publishes the resulting image.

### `color_detection`

ROS2 bring-up package to launch both webcam driver and color filter packages.

## Usage

1. **Build the workspace**

```bash
cd color_detection_ws
colcon build --symlink-install
source install/setup.bash

```

2. **Launch both nodes**

```bash
ros2 launch color_detection color_detection.launch.py

```

## Result

![Filtered Image Output](color_detection_result.png)