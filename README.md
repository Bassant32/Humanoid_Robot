# Humanoid_Robot
# Overview

This project aims to design and develop a humanoid robot capable of playing football (soccer).
The robot is programmed to detect, approach, and kick the ball toward the goal using intelligent movement control and vision-based decision-making.

The project combines mechanical design, control systems, computer vision, and embedded programming to simulate a real football-playing humanoid.

# Objectives

Design a humanoid robot capable of dynamic movement and balance.

Implement motor control (servo motors) for walking, turning, and kicking actions.

Integrate image processing to detect the ball and the goal.

Develop a control algorithm for movement decisions based on camera input.

Test the robot’s performance in real-time football scenarios.

# System Components

Motors: Servo motors for leg and arm control

Sensors: ESP32_cam 

Software: Python / OpenCV for vision

Power Source: Rechargeable Li-ion battery pack

# Vision-Based Detection Model
Model Overview

To enable autonomous behavior and intelligent decision-making, a deep learning–based object detection model was integrated into the humanoid robot system.
The model is responsible for detecting the football in real time and guiding the robot’s movement accordingly.

A YOLO (You Only Look Once) architecture was selected due to its:

Real-time inference capability

High detection accuracy

Suitability for robotics and embedded systems

📁Best Model :
🔗 [[Best_model.to download]](https://drive.google.com/file/d/1UYAT8nxeGcZ84a0SpvINdR0bbdYw65qB/view?usp=drive_link)
Dataset

Custom dataset collected from the robot’s point of view.

Images include variations in:

1. Lighting conditions

2. Ball positions and distances

3. Background complexity

4. The dataset is manually annotated for the football class.

📁 Dataset & Training Assets:
🔗[ [DataSetand all outputs]](https://drive.google.com/drive/folders/1FfqXchAo3R5Nu-eGQ3MDiFsXkTvR2cm7?usp=drive_link)


Training Configuration

*Transfer learning was used to leverage pretrained YOLO weights.

*Input image size: 640 × 640.

*Training performed on a GPU-enabled environment.

*Data augmentation applied to improve robustness in real-world scenarios.

*Training Stability & Optimization

*Initial training experiments showed high loss oscillations, indicating unstable convergence.

To improve stability:

The learning rate was adjusted.

Mosaic augmentation was reduced.

These changes resulted in smoother loss curves and more stable training behavior.

System Integration

The trained model runs within a Python-based vision pipeline.

Real-time frames are captured using a ESP32_cam.

Detection outputs (ball position and confidence) are passed to the movement control logic to guide robot actions.

Future Work

Extend detection to include:

Goal posts

Field boundaries

Obstacles
