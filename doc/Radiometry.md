- energy: in J
- power: energy per unit time

- Irradiance(E): power per unit area
$$
E=\frac{d\Phi}{dA}
$$

- Radiance(L): Irradiance per unit solid angle

$$
R=\frac{dE}{d\omega}
$$


詳細推導：
將目標表面附近空間看成有兩個光場，一個是入射場Li，一個是出射場Lo，設位置固定，所以只取決於立體角，在不考慮發光的情況下，出射場只取決於入射場，但是某一個方向的出射場取決於所有方向入射場的共同貢獻，將入射場乘以某一個權重后對立體角進行積分即可得到出射場，而該權重就是BRDF：
$$
L_o(\omega_o)=\int f(\omega_i,\omega_o)L_i(\omega_i)cos\theta_id\omega_i
$$
進而有
$$
dL_o(\omega_o)=f(\omega_i,\omega_o)L_i(\omega_i)cos\theta_id\omega_i \\
f(\omega_i,\omega_o)=\frac{dL_o(\omega_o)}{dE_i(\omega_i)}
$$
其中
$$
dE_i(\omega_i)=L_i(\omega_i)cos\theta_id\omega_i
$$

又有
$$
\frac{dL_o(\omega_o)}{L_i(\omega_i)}=f(\omega_i,\omega_o)cos\theta_id\omega_i
$$
固定Li，若對兩邊做積分，則可以得到所有Lo方向貢獻之和佔Li的比例，類似於能量保留率。
$$
Ri=\int f(\omega_i,\omega_o)cos\theta_id\omega_i
$$
當然，Ri不會大於1.
若假設f爲常數，則有$f=R/\pi$

## 蒙特卡洛積分
先考慮離散化積分計算：
$$
\int_a^b f(x)dx=\frac{1}{N}\sum f(x_i)*(b-a)=\frac{1}{N}\sum \frac{f(x_i)}{1/(b-a)}
$$
推廣到非均勻採樣：
$$
\int_a^b f(x)dx=\frac{1}{N}\sum \frac{f(x_i)}{p(x)}
$$
## 光源採樣
