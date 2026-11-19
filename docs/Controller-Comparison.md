
# Magnetic Levitation – Controller Comparison

## Project Information

**Course:** Microelectronic Control Systems  
**University:** Rhine-Waal University of Applied Sciences  
**Date:** November 2025  
**Programming Language:** C

## Objective

Implement and experimentally evaluate P, PD, and PID
feedback controllers using a magnetic levitation
laboratory apparatus.

The system uses a Hall-effect sensor for magnetic
field feedback and an electromagnet controlled
through PWM.

## Controller Parameters

| Parameter | P | PD | PID |
|---|---|---|---|
| Kp | 85 | 75 | 75 |
| Ki | — | — | 1 |
| Kd | — | 10 | 7 |
| Gain scale | 20 | 20 | 20 |
| Base power | 220 | 220 | 255 |
| Setpoint | 136 (0x88) | 136 (0x88) | 202 |

These parameters were selected through
experimental tuning for the respective setups.

## Experimental Results

### P Controller

- Successfully achieved levitation.
- Visible oscillations occurred around the target position.
- Controller performance was highly sensitive to Kp.
- Insufficient gain caused the object to fall, while
  excessive gain could pull it toward the upper magnet.

### PD Controller

- Tested using the same levitating object as the
  P controller.
- Derivative action improved damping.
- Reduced oscillations compared with P-only control.
- Faster recovery was observed after small disturbances.
- Excessive derivative gain increased jitter.

### PID Controller

- Tested with a heavier block-shaped magnet.
- Integral action helped correct persistent position error.
- Integral accumulation was limited to reduce windup.
- Small derivative error changes were ignored to
  reduce the effects of sensor noise.
- Stable levitation was achieved with small remaining
  oscillations.

## Important Comparison Note

The P and PD controllers were tested with the same
levitating object, allowing a direct qualitative
comparison of their behavior.

The PID controller was tested with a different,
heavier magnet. Its results therefore demonstrate
PID performance under a different load rather
than a controlled comparison against P and PD.

## Key Engineering Lessons

- Feedback calibration is essential for accurate
  position sensing.
- Proportional gain affects both responsiveness
  and stability.
- Derivative action can reduce oscillations but
  is sensitive to measurement noise.
- Integral action can reduce steady-state error,
  but requires appropriate limiting.
- Controller gains must be tuned for the specific
  physical system and load.

## Project Scope

The control algorithms were implemented and
tested on university laboratory hardware.

The university supplied the underlying hardware
drivers and initial firmware framework. The
controller logic and tuning were developed
during the laboratory exercises.
