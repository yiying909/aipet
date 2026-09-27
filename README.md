# AI Pet
AI Pet is an embedded AI companion developed as a course project for CS 256. The system combines an ESP32-based embedded platform, a TFT display, physical button input, and AI interaction to provide users with a compact interactive assistant.
The project was designed to provide hands-on experience with embedded systems, AI APIs, speech-to-text, text-to-speech, and hardware/software integration.

## Overview
The original project concept was to develop an interactive companion capable of responding to user speech and physical interactions. The planned system included voice interaction, visual feedback, animations, and physical movement using servo motors.
For the final implementation, the project focused on the core interaction and display functionality. Users interact with the device through three physical buttons and a connected computer.
The implemented functionality includes:

* **Weather and Date:** Displays the current weather and date on the TFT display.
* **To-Do List:** Displays the current to-do list.
* **AI Interaction:** Allows users to enter a question through the connected computer and displays the AI-generated response on the TFT display.
* **TFT Display:** Provides the primary visual interface for system output.
* **Physical Controls:** Three buttons provide access to the device's primary functions.

## System Architecture
The system uses an ESP32-based maker board as the primary embedded platform. The device receives input through physical buttons and communicates with a connected computer for AI interaction.

The general interaction flow is: user input in text --> physical button/computer --> ESP32 --> +Weather/Data +To-Do List +AI Interaction -> AI Response --> TFT Display

## Hardware
The project was developed using the following hardware:
* ESP32-based maker board
* 1.5" TFT display
* Three physical buttons
* Computer for AI interaction and text input
* Audio hardware
* Power supply
The original design also considered a microphone, battery, touch sensor, and servo motors for additional interaction and physical movement.

## AI and Speech Processing
Speech processing was a major component of the original project goals. The team initially intended to implement a speech-to-text pipeline that would allow users to communicate with the AI through voice. The project proposal identified speech-to-text as a key component for handling AI responses.
During development, the speech-to-text implementation presented several hardware and software challenges.
The initial audio setup using the hardware connected to the ESP32 did not consistently capture speech clearly. After transitioning to computer-based audio input, background noise continued to interfere with speech recognition. Even after reducing some of the noise, transcription accuracy varied depending on pronunciation, accent, grammar, and slang.
Because reliable speech recognition could not be achieved within the project timeline, the final implementation used text input for the AI interaction. This allowed the team to demonstrate the AI communication pipeline while maintaining reliable system behavior.

## Development Challenges

### Audio Input and Speech Recognition
The primary technical challenge was establishing a reliable speech-to-text pipeline.
Several factors affected the system:
* Inconsistent audio capture from the original hardware configuration
* Background noise in the recorded audio
* Inaccurate speech transcription
* Variation in recognition accuracy based on pronunciation, accent, grammar, and slang
Addressing these issues required testing different audio configurations and adapting the system to the limitations of the available hardware and software.

### Scope and Feature Prioritization
The initial project design included several additional interaction features, including:
* Servo-controlled movement
* Touch or "head-pat" interaction
* Physical reactions to user input
* Animated visual feedback
* Voice-based AI interaction
The original proposal included micro-movements such as nodding and shaking, along with voice interaction and speech-to-text functionality.
Due to the challenges encountered during speech recognition development, the team prioritized the functionality that could be implemented and demonstrated reliably within the available development time.

## What We Learned
This project provided practical experience in:
* Embedded systems development
* ESP32 programming
* TFT display integration
* Hardware input and output
* AI API integration
* Speech-to-text and text-to-speech technologies
* Hardware and software debugging
* System integration
* Feature prioritization and scope management
A significant aspect of the project was learning how hardware constraints and unreliable input systems can affect the design of an integrated application. The development process required evaluating the reliability of individual components and adapting the system architecture accordingly.

## Future Development
Potential future improvements include:
* Reliable microphone-based speech recognition
* Improved audio filtering and noise reduction
* Voice-based AI interaction
* Text-to-speech responses
* Servo-controlled physical movement
* Touch-based interaction
* Animated TFT expressions
* Battery-powered operation
* A 3D-printed enclosure
These additions would move the project closer to the original goal of creating a standalone interactive companion with both digital and physical feedback.

## Team
* Yiying Zhong
* Manasi Kale
* Sunny Xie

**Course:** CS 256
**Project:** AI Pet
**Status:** Completed — Final Course Project
