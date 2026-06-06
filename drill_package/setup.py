from setuptools import find_packages, setup

package_name = 'drill_package'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='cwilhelm',
    maintainer_email='cwilhelm@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
    'console_scripts': [
        'drill_node = drill_package.drill_node:main',
        'listener = drill_package.listener:main',
        'drill_esp = drill_package.drill_esp:main',
        'sample_esp = drill_package.sample_esp:main',
        'publisher = drill_package.publisher:main',
        'follower = drill_package.subscriber:main',
    ],
},
)
