# Line Follower Robot with Arduino and PID Control

This project is a simple autonomous vehicle that follows a black line on a white surface using infrared sensors and a PID (Proportional–Integral–Derivative) controller. It was developed as part of a final project for an automation course.

## 🎯 Objective

Build a line-following robot capable of staying on track by adjusting motor speeds based on sensor input and real-time error correction using a PID controller.

## 🔧 Technologies and Components

- Arduino UNO R3
- Infrared sensors (TCRT5000)
- L298N motor driver
- DC motors with wheels
- PID controller
- Circuit design simulated with Tinkercad

## ⚙️ How It Works

Three infrared sensors detect the position of the line. Based on the sensor readings, an error value is calculated and fed into the PID controller, which adjusts the speed of each motor independently to keep the robot centered on the line.

## 📝 Features

- Real-time line tracking
- Adjustable PID constants (Kp, Ki, Kd)
- Smooth handling of curves and path recovery
- Cost-effective and easy to assemble
