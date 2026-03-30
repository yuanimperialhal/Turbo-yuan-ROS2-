from setuptools import find_packages, setup

package_name = 'demo_python_service'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/resource', ['resource/ceshi.jpg', 'resource/ceshi2.jpg']),
        
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='yuan',
    maintainer_email='yuan@todo.todo',
    description='TODO: Package description',
    license='Apache-2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'learn_face_detect = demo_python_service.learn_face_detect:main',
            'face_detect_node = demo_python_service.face_detect_node:main',
            'face_detect_client_node = demo_python_service.face_detect_client_node:main',
            #在entry_points中定义了两个控制台脚本,
            #分别为learn_face_detect和face_detect_node,
            #它们分别对应于demo_python_service.learn_face_detect模块中的main函数
            #和demo_python_service.face_detect_node模块中的main函数。
            #这些脚本可以通过命令行直接运行,例如可以使用ros2 run demo_python_service learn_face_detect来运行learn_face_detect脚本
            #,使用ros2 run demo_python_service face_detect_node来运行face_detect_node脚本。
            #,使用ros2 run demo_python_service face_detect_client来运行face_detect_client脚本。
        ],
    },
)
