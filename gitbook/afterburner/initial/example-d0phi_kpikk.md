# Example: D0phi\_KpiKK

## Physics

Analyses the decay:

$$
J/\psi \rightarrow D^0\phi \rightarrow K^-\pi^+ K^-K^+
$$

This is a rare decay \(note the violation of conservation of strangeness\). In addition, this decay is of the form $$J/\psi \rightarrow \text{PV}$$, where $$\text{P}$$ stands for _pseudoscalar meson_ and $$\text{V}$$ stands for _vector meson_. Decays of this type can serve as a test for different beyond the Standard Model models, because these predict certain branching ratios for this type of decay.

Inspiration for this decay channel comes from [arXiv:1402.4025](https://arxiv.org/abs/1402.4025). In this package, however, the $$D^0$$ and $$\phi$$ mesons are, however, reconstructed through purely hadronic decay channels. Whether this works is not yet clear \(hadronic backgrounds are so large that they probably overshadow the signal of this rare decay mode\), but the advantage is that that there are only charged tracks to be considered. In addition, while there are many decay modes of the $$D^0$$ meson, the decay mode  $$D^0 \rightarrow K^-\pi^+$$ is still a relatively dominant decay mode: $$\Gamma_{32} = (3.89 ± 0.04) \%$$. For $$\phi$$, on the other hand, we have $$\Gamma_1 = (49.2 \pm 0.5)\%$$ for  $$D^0 \rightarrow K^-\pi^+$$, meaning that this is the dominant decay channel.

Still, there is another problem with this purely hadronic decay mode: there are two negative kaons $$K^-$$, so we have to decide which one decayed from $$D^0$$ and which one decayed from $$\phi$$. Luckily, the masses of $$D^0$$ and $$\phi$$ differ quite significantly — $$m_{_{D^0}} = 1864.84 \pm 0.04\text{ MeV}/c$$ versus $$m_{_\phi} = 1019.461 \pm 0.016\text{ MeV}/c$$\) — which means that we can select the best combination based on which combination candidate has an invariant mass closest to these two expected values.

## Class description

See [Doxygen pages here](https://redeboer.github.io/BOSS_Afterburner/classD0phi__KpiKK.html).

