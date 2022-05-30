# 7. Sampling and reconstruction
sampling theory—the theory of taking discrete sample values from functions defined over continuous domains and then using those samples to reconstruct new functions that are similar to the original.

Filter is used to determine how multiple samples near each pixel are blended together to compute the final pixel value, and the Film class accumulates image sample contributions into pixels of images.

## 7.1 Sampling Theory

區分image pixels(特定採樣位置的函數值)和display pixels(物理物體，發射某種分佈的光)

### 7.1.1 The Frequency Domain and the Fourier Transform
### 7.1.2 Ideal Sampling and Reconstruction
採樣過程：函數與等脈衝火車(impulse train)函數相乘，等間距無窮多個delta函數之和，scaled by 週期T
$$
T\sum_i\delta(x-iT)f(iT)
$$

函數乘積的傅立葉變換等於函數傅立葉變換的卷積。
函數與delta函數的卷積等於函數自身。
函數與shah函數的卷積等於函數週期性的複製。(實際上似乎是加和，需要後續分析)

重建信號需要discard原點之外的函數複製，與1/T週期的box函數相乘。
也就是說，先對原信號和shah函數做頻域卷積，然後乘以box函數(頻域)，就得到了原信號在頻域的表示，做到這一點我們只是取了一些等距離採樣。
因爲在時空域的重建信號爲原信號與shah函數乘積與sinc函數做卷積(週期爲T)
因而有
$$
\tilde{f}(x)=\sum_{i=-\infty}^{\infty}sinc(x-Ti)f(Ti)
$$
但由於sinc函數無窮延伸，因此需要採樣所有f(Ti)來計算，實際應用會採取有限extent的filter，雖然不能完美重建。
常用box函數來代替sinc，有效地對x附近某區域採樣值做平均，但是這其實是一個很差的選擇，因爲不僅僅不能夠完全摘取中心副本，還會包含其他副本的高頻貢獻。

### 7.1.3 Aliasing
若時空域採樣率低，說明shah函數週期T大，則頻域的採樣間隔減小，將F(w)的副本Push地更緊，導致重疊。
採樣頻率大於兩倍信號的最高頻分量頻率時，可以完美重構信號，成爲Nyquist frequency.

但計算機圖興學中大多數有趣信號都是無限帶寬的，不連續性可能發生在任何兩個採樣點之間，因此無法保證還原。

### 7.1.4 Antialiasing Techniques
prealiasing: due to sampling
postaliasing: due to reconstruction
- Nonuniform Sampling
雖然不能性消除走樣，但能夠將一致性、規律性走樣轉換爲隨機誤差或噪音，視覺效果上好一點。
- Adaptive Sampling
因地制宜
- Prefiltering
過濾掉或模糊掉(blur)原函數的以當前採樣率無法捕捉到的高頻部分。

### 7.1.5 todo

## 7.2 Sampling Interface
### 7.2.1 Evaluating sample patterns: Discrepancy  todo

### 7.2.2 Basic sampler interface
### 7.2.3 Sampler Implementation
### 7.2.4 Pixel sampler