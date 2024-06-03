# Triton C++ Example
This is an example of an AI inference server that can scale out using Triton. The blockchain payment system is a bonus.   

## 0. Introduction
Nvidia Triton is a very attractive project.   
It supports various AI models while boasting fast speeds.   
However, it is not ideal from a scale-out perspective.   

In this project, we implement a very simple example that addresses the scale-out issue using Nvidia Triton.   
The configuration is as follows:   
| Service | Description |
|:---|:---|
| Frontend + Backend Server | Provides server code to verify actual operation. |
| Gateway | The entry point for user requests and responses. |
| Scheduler | Schedules which Triton node should handle the request. <br>In this project, a simple round-robin method is used. |
| Triton Node | Consists of two parts: <br>  * Triton Server: Performs inference using AI models. <br>  * Manager: Manages the Triton server. It announces itself with health check messages and forwards requests to the Triton server. |
| Health Checker | Monitors Triton nodes. <br>Continuously creates a list of nodes and provides it to the scheduler. |
| Blockchain-Based Payment System | Implements a payment system. |
