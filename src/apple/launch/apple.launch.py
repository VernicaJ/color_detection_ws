from launch import LaunchDescription
from launch_ros.actions import Node

'''
Webcam Driver Node to display current time and publish frames
- Output: /webcam/image

Default parameters:
- Camera Index 0
- 30 FPS

'''

def generate_launch_description():
    webcam_driver_node = Node(
        package="apple",
        executable="webcam_driver.py",
        name='webcam_driver',
        output='screen',
        parameters=[{
                'camera_index': 0,
                'fps': 30
        }]
    )
    return LaunchDescription([
        webcam_driver_node,
    ])