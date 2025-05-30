from launch import LaunchDescription
from launch_ros.actions import Node

"""
Launch file to launch both nodes:
- webcam_driver: Publishes video frames to /webcam/image
- color_filter: Subscribes to /webcam/image and publishes /webcam/image_filtered

Default parameters:
- Camera Index 0
- 30 FPS
- Blue filter values (HSV approx. 100-130, 50-255)
- Red replacement values (HSV approx. 0-255)

"""

def generate_launch_description():

    return LaunchDescription([
        Node(
        package="apple",
        executable="webcam_driver.py",
        name='webcam_driver',
        output='screen',
        parameters=[{
                'camera_index': 0,
                'fps': 30
        }]
    ),
        Node(
            package="blue",
            executable="color_filter_node",
            name='color_filter',
            output='screen',
            parameters=[{
                'image_sub_topic': '/webcam/image',
                'image_pub_topic': '/webcam/image_filtered',
                'lower_h': 100,
                'lower_s': 50,
                'lower_v': 50,
                'upper_h': 130,
                'upper_s': 255,
                'upper_v': 255,
                'set_h': 0,
                'set_s': 0,
                'set_v': 255,
                'show_images': True
            }]
        )

    ])

