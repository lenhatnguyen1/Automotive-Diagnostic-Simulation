# UDS Service 0x29 - Authentication (Personal Research Project)

## Overview

This project simulates the security functionality of UDS as defined in ISO 14229-1:2020 in small-scale, with a focus on:

- Diagnostic mechanism

- Service 0x27 – SecurityAccess (Seed/Key)

- Service 0x29 – Authentication (Introduced in ISO 14229-1:2020,)

The goal is not to implement a full AUTOSAR-compliant ECU, but rather to design, manage, and deliver a learning-oriented software component that reflects the structure, flow, and behavior of real automotive software.

Although the implementation does not rely on the AUTOSAR stack, it follows AUTOSAR-inspired design principles, such as:

- Modular architecture

- Layered separation

- State machine

---

## Project Objectives

- Develop a lightweight C-based simulation of UDS authentication services

- Understand the diagnostic flow and security algorithms

- Practice software design and clean architecture in an embedded-like context

- Use GitHub to manage the codebase as a real deliverable

- Implement basic functional verification

## Self-goal

- Enhance C programming skills

- Improve software architecture mindset

- Deepen understanding Service 0x27

- Know the basic concept of Service 0x29

> This is a software-level simulation, not deployed on real ECU. However, the design aims to be **portable** and **adaptable** to embedded systems (e.g., STM32) in the future.

---

## Project Structure (Planned)

```plaintext
Source/
├── main.c                        // Entry point
├── App 
    ├── diag/
    │   ├── Diag.c/.h          // Service dispatcher
    │   ├── S27_SecurityAccess/               // Optional: Seed/Key for Security Access
    │   └── S29_Authentication/               // Main FSM logic for Authentication
    ├── crypto/
    │   ├── Crypto.c/.h                // HMAC SHA256 engine
    |__ test/
        |__ test.c
