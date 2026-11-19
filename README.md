Magnetic Levitation Control — P, PD & PID
Embedded C | Feedback Control | Hall-Effect Sensing | PWM Actuation
Overview
This project documents a hands-on magnetic levitation experiment completed in the Microelectronic Control Systems course at Rhine-Waal University of Applied Sciences during the winter semester 2025/26 (Lab 4, November 2025).

The objective was to implement, tune, and compare proportional (P), proportional–derivative (PD), and proportional–integral–derivative (PID) controllers on a physical levitation apparatus. The controllers used Hall-effect sensor feedback to adjust the power supplied to an electromagnet and maintain the levitating object near a selected reference position.

This was a university laboratory control-programming project, not a custom PCB or mechanical design project.
Experimental Setup
The laboratory apparatus consisted of:
- A fixed permanent magnet above the levitation area
- A controlled electromagnet beneath it
- A Hall-effect sensor for magnetic-field-based position feedback
- A microcontroller board running C firmware
- PWM-controlled electromagnet power and indicator LEDs
Feedback path: Hall sensor → position error → P / PD / PID controller → PWM command → electromagnet → levitating object.

The supplied laboratory framework provided the device drivers for the Hall sensor, electromagnet, LEDs, and UART. My implementation work focused on the control algorithms, their tuning, and experimental testing.
Sensor Calibration and Control Loop
Before entering the control loop, the program swept electromagnet power through its 0–255 command range and recorded Hall-sensor offsets. This calibration compensated for the electromagnet's own contribution to the sensor reading.

For each control-loop iteration:
1. Read the Hall-effect sensor using hall_get().
2. Calculate the error: error = setPoint - input.
3. Calculate the P, PD, or PID correction.
4. Limit the command to the valid 0–255 PWM range.
5. Apply the command using magnet_set() and update sensor compensation using hall_setmagpower().
The control-law structure in my implementation was:
output = basePower - P - I - D;
if (output > 255) output = 255;
if (output < 0)   output = 0;
magnet_set((uint8_t)output);
hall_setmagpower((uint8_t)output);
For the P and PD versions, unused terms were omitted. The sign convention was chosen for the apparatus: greater positive position error reduced the electromagnet command.
Controller Implementations
Controller	Test object	Setpoint	Kp	Ki	Kd	Gain scale	Base power
P	Tape-roll test object	0x88 (136)	85	—	—	20	220
PD	Same tape-roll test object	0x88 (136)	75	—	10	20	220
PID	Heavier block-shaped magnet	202	75	1	7	20	255


These were experimental parameter values for the specific laboratory setups, not universal tuning recommendations.
P controller
I implemented proportional feedback in main.c, calculated a scaled position-error correction, and adjusted the electromagnet's PWM output. I also used red/green status LEDs to indicate whether the object was near or far from the reference position.

Observed result: The tape-roll test object levitated, but small oscillations around the reference position remained. Tuning Kp and the baseline power was important: insufficient correction caused the object to fall, while excessive correction could pull it toward the upper magnet.
PD controller
I extended the same control loop with a derivative term based on the change in position error between consecutive iterations:
D = ((int16_t)Kd * (error - previousError)) / gainScale;
Observed result: With the same tape-roll object, PD control reduced oscillations and improved recovery after small disturbances compared with P-only control. Excessive derivative gain caused jitter, illustrating sensitivity to measurement changes.
PID controller
For the heavier block-shaped magnet, I added integral correction to address persistent position error and modified the tuning parameters and reference position.

The implementation also included:
- Integral limiting: accumulation only when abs_error < 20, with the integral clamped between −300 and +300; otherwise it was reset
- Derivative deadband: very small error changes (−2 < dErr < 2) were treated as zero to reduce jitter
- PWM saturation: output constrained to the valid 8-bit command range
Observed result: The heavier object levitated with smoother motion and smaller persistent position error when integral action was enabled, although some oscillation remained.

Comparison limitation: P and PD were tested on the same tape-roll object, so their observed behavior can be compared more directly. The PID experiment used a different, heavier magnetic object; therefore, its result should not be treated as a controlled head-to-head performance ranking against the earlier two tests.
My Contributions
- Implemented P, PD, and PID controller logic in embedded C using the provided laboratory code framework
- Defined position error, scaled controller gains, baseline power, and PWM command limits
- Tuned gains experimentally while observing levitation stability and oscillation behavior
- Added the derivative term and stored previous error for PD/PID control
- Implemented limited integral accumulation and a derivative deadband in the PID version
- Programmed the microcontroller and tested each controller on the physical apparatus
- Documented qualitative results, challenges, and control-behavior differences
Results and Engineering Lessons
The experiments demonstrated how controller structure and parameter selection affect the behavior of a feedback system:
- P: capable of levitation, but prone to visible oscillation
- PD: improved damping on the same test object
- PID: helped compensate persistent position error on the heavier test object
- Calibration: essential because electromagnet power affects the Hall sensor measurement
- Practical constraints: PWM saturation, sensor noise, and different object properties influence control performance
The observations were primarily qualitative; this repository does not claim measured rise times, settling times, or overshoot percentages that were not recorded in the report.
Technologies and Skills
Embedded C · AVR Programming · Feedback Control · P / PD / PID · Hall-Effect Sensors · PWM · Electromagnetic Actuation · Controller Tuning · Hardware Testing
Project Files
Supporting photographs, controller source files, and a privacy-checked version of the laboratory report can be added to this repository as they become available. The README describes the completed experiment; any future reconstructed code or diagrams will be identified as reconstructions rather than original laboratory files.
Potential Improvements
- Log sensor readings and control outputs for quantitative step-response comparisons
- Test all three controller types on the same object under identical conditions
- Explore filtering and systematic gain-tuning methods
- Evaluate disturbances and actuator saturation more quantitatively
Author: Hasti Chiniforoush
Course: Microelectronic Control Systems — Lab 4: Levitating Magnet
Institution: Rhine-Waal University of Applied Sciences, Germany
Completed: November 2025
