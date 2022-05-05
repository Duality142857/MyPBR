## Intro:
### 4 key quantities:
- flux
- intensity
- irradiance
- radiance

由spectral power distribution(SPD)描述

## 5.1 Spectral Representation
finding good basis functions
找到好的基函數來表示SPD：將無窮維空間映射到低維基函數係數空間。


## 5.2 The SampledSpectrum Class

### 5.2.1 XYZ Color
tristimulus theory of color perception: 所有可見SPD均可被三個數準確表示，對於人來說。
通過spectral matching curves積分計算。
這些曲線由CIE標準決定，通過實驗獲取。不同分佈，相同顏色的spectra被稱爲metamers.

XYZ表示人眼感知顏色很合適，但是不適合作爲spectral computation的基函數。

### 5.2.2 RGB Color
不同顯示設備的spectral response curve不同，分爲紅綠藍三個獨立的曲線，和SPD相乘後積分。
rgb和xyz相互轉換通過矩陣乘法。每個矩陣元爲一個分量積分$\int R(\lambda)X(\lambda)d\lambda$
