# DSSIM

Implementation of a [structural similarity] metric in C++ using [OpenCV]. Code from [Philipp Klaus Krause updates] to [Rabah Mehdi original].

Resurrected to be a handy comparison with newer metrics particularly the [Kornel Lesiński DSSIM tool].

# Building

```
mkdir build
cd build
cmake ..
make
```

# Example results

```
1a.jpg 1a.jpg : 0.000
1a.jpg 1b.jpg : 6.679
1a.jpg 1c.jpg : 5.049
1b.jpg 1c.jpg : 5.732
2a.jpg 2b.jpg : 1.474
2a.jpg 2c.jpg : 1.500
2b.jpg 2c.jpg : 0.195
```

# References

[structural similarity]: https://en.wikipedia.org/wiki/Structural_similarity
[OpenCV]: https://opencv.org/
[Philipp Klaus Krause updates]: http://www.colecovision.eu/graphics/DSSIM/
[Rabah Mehdi original]: http://mehdi.rabah.free.fr/SSIM/
[Kornel Lesiński DSSIM tool]: https://kornel.ski/dssim
