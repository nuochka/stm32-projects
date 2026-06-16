# STM32 Hardware Interrupts and Debouncing

STM32 project focusing on external hardware interrupts (EXTI), asynchronous event handling, and software contact debouncing algorithms.

## Features

- **Asynchronous Interrupt Handling**: Offloading button state monitoring from the main loop to the hardware External Interrupt (EXTI) controller.
- **Software Debouncing Logic**: Eliminating mechanical contact noise (chatter) using non-blocking millisecond counters.
- **Low-Power & Event-Driven Architecture**: The main loop remains completely empty, conserving CPU cycles while waiting for physical events.
- **State Toggle Control**: Seamlessly changing operational states (e.g., toggling an LED) exactly once per physical button press.

## Hardware

- STM32L476RG (Nucleo-L476RG board)
- Onboard User Button (PC13)
- Onboard LED (PA5)

## Controls

- **Press USER_BUTTON (PC13)** → Instantly triggers a hardware interrupt, handles debouncing, and cleanly toggles the state of the onboard LED (PA5).
- **Main Loop** → Empty; no CPU resources are spent polling the button state.

## CubeMX Configuration

![CubeMX Interrupts Configuration](docs/04_cubemx.png)

- **GPIO (PC13)**: Configured as `GPIO_EXTI13` with Falling Edge trigger detection. No internal pull-up/pull-down resistors are needed since the Nucleo board includes an external hardware pull-up resistor.
- **NVIC**: `EXTI interrupts` explicitly enabled in the Nested Vectored Interrupt Controller settings, with priority assignment.

## Code Logic

- **EXTI Callback Integration**: Instead of polling the button state inside the main loop, the application utilizes the hardware interrupt callback triggered instantly by a voltage drop on PC13.
- **Debouncing Implementation**: Inside the callback, a time-check mechanism measures the duration between consecutive triggers. If a new edge is detected within a 50ms window, it is classified as mechanical bounce and ignored, preventing accidental double-toggles.

## How to run

To verify the event-driven LED toggling behavior, build and flash the project onto the board. The system will react instantaneously to button inputs without missing presses or double-triggering due to contact bounce.