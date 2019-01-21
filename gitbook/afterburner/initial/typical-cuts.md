# Typical cuts

In papers from the BESIII Collaboration, you will usually encounter the following cuts. As such, they have been implemented in the base algorithm \(see [next section](base.md)\).

## Vertex geometry

* Interaction point cuts?
* Multiplicity cuts?

## Charged tracks

* Distance of the vertex of the track from the interaction \(IP\) in $$xy$$ plane: $$\left|\text{d}r\right| < 1\text{ cm}$$.
* Distance of the vertex of the track from the IP in $$z$$ plane: $$\left|\text{d}z\right| < 10\text{ cm}$$.
* Polar angle: $$\left|\cos\theta\right| < 0.93$$.
* PID: usually making use of [MDC](../../boss/besiii.md#main-drift-chamber-mdc) and [TOF](../../boss/besiii.md#time-of-flight-system-tof) and using a probability of at least $$0.001$$.

## Neutral tracks

Neutral tracks are reconstructed from electromagnetic showers in the [EMC](../../boss/besiii.md#electromagnetic-calorimeter-emc), which consists of a barrel and an end cap.

|  |  |  |
| :--- | :--- | :--- |
| Energy for **barrel** showers | $$\left|\cos\theta\right| < 0.8$$ | $$E > 25\text{ MeV}$$ |
| Energy for **end cap** showers | $$0.83 < \left|\cos\theta\right| < 0.93$$ | $$E > 50\text{ MeV}$$ |

If there are more than one charged tracks, there is a time requirement of $$0 \leq T \leq 14 (50\text{ ns})$$.

## Kinematic fits

* $$\chi^2$$ of the kinematic fit \(no matter how many constraints\): $$\chi^2 < 200$$ \(one may of course choose to divert from this\).

