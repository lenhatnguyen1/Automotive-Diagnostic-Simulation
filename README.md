# UDS Service 0x29 - Authentication (Personal Research Project)

## 📌 Overview

This project is a **personal implementation** of the UDS Diagnostic Service **0x29 – Authentication**, defined in **ISO 14229-2**. It simulates an embedded security flow where a client and an ECU engage in an authentication handshake using challenge-response mechanisms and cryptographic validation (e.g., HMAC-SHA256).

The goal is to:
- Understand and design the internal logic of Service 0x29
- Simulate FSM behavior for secure communication
- Apply embedded-friendly cryptographic techniques
- Build a clean, scalable software architecture for diagnostic services

---

## 📋 General Requirements

- Support authentication process defined in ISO 14229-2:
  - Authentication Request
  - Challenge
  - Authentication Message
  - Result Evaluation
- Maintain proper **state handling** and **NRC (Negative Response Code)** management
- Implement basic **crypto backend** using software-based HMAC (no HSM required)
- Ensure code readability and extensibility (clean architecture)

> ⚠️ This is a software-level simulation, not deployed on real ECU. However, the design aims to be **portable** and **adaptable** to embedded systems (e.g., STM32) in the future.

---

## 📁 Project Structure (Planned)

```plaintext
Source/
├── main.c                        // Entry point
├── App 
    ├── diag/
    │   ├── DiagRouter.c/.h          // Service dispatcher
    │   ├── Service27/               // Optional: Seed/Key for Security Access
    │   └── Service29/               // Main FSM logic for Authentication
    ├── crypto/
    │   ├── HMAC.c/.h                // HMAC SHA256 engine
    │   └── SHA256.c/.h              // Internal hash functions
    ├── test/
    │   └── Test.c             // Simulate input frames & verify flow
    ├── utils/
    │   └── Logger.c/.h              // Lightweight debugging logger
