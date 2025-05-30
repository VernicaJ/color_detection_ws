from launch import LaunchDescription
from launch_ros.actions import Node

'''
Node to filter a specific color and replace it with a set color.
- Input: /webcam/image
- Output: /webcam/image_filtered

Default behaviour:
- Filters Blue color (HSV approx. 100-130, 50-255)
- Replaces Blue with Red (HSV approx. 0-255)

'''

def generate_launch_description():
    color_filter_node = Node(
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
    return LaunchDescription([
        color_filter_node
    ])