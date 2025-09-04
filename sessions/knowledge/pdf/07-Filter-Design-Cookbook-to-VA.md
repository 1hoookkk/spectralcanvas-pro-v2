# 07 — Filter Design: Cookbook → VA

- Cookbook (RBJ): quick biquad prototypes (LP/HP/BP/peak/shelf); great for static EQ.
- Bilinear transform: prewarp Fc/Q; map analog to digital with stability.
- TPT (VA) filters: topology‑preserving transforms for robust modulation and stability.
- Nonlinear stages: place saturation in the integrator path; use implicit (trapezoidal) forms.
- Parameter mapping: freq in Hz → normalized g = tan(pi*Fc/Fs); use fast tan approx cautiously.
- Stability: clamp Q/g; handle extreme Fc near Nyquist; denormal‑safe states.

