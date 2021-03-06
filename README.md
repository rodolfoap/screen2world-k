# Screen-to-World-K

Screen to world coordinates conversion (z=0) using perspective-n-points strategy with OpenCV4 solvePnP(). To find screen-to-world correspondences (which point in the space in the camera image corresponds to a point in the screen), an _extrinsics matrix_ is required. See the full calculation here: https://github.com/rodolfoap/screen2world-k. In such example, the _extrinsics matrix_ is calculated using a set of world-to-screen point correspondences.

* To find screen-to-world correspondences, another approach is proposed: See the full calculation here: https://github.com/rodolfoap/screen2world-d. In such example, the _extrinsics matrix_ is calculated by using a set of person detection boxes to calculate the distances to the camera. Knowing the camera bearing, world-to-screen points correspondences are automatically found, and the _extrinsics matrix_ is calculated in some steps.
* The previous applications require an _intrinsics matrix_ (also called _calibration matrix_, _camera matrix_ or _K matrix_) which can be calculated using https://github.com/rodolfoap/cameracalibration.
* To find screen-to-world correspondences in a simple manner, another approach is proposed: See the full calculation here: https://github.com/rodolfoap/screen2world-h. In such example, In such example, an _homography matrix_ is calculated just by using a set of world-to-screen point correspondences. Therefore, the _intrinsics_ matrix is not needed.

## Usage

1. Build the points correspondence matrix using https://github.com/rodolfoap/points-picker.
2. Generate the camera matrix using https://github.com/rodolfoap/cameracalibration.
3. Take a screen point and test it.

## Build

```
mkdir -p build && pushd $_;
cmake ..
make
popd
```

## Sample output

```
Camera Matrix:
[986.5826792241824, 0, 656.4406302926737;
 0, 921.4350263186225, 356.2173553778945;
 0, 0, 1]

Distortion Coefficients:
[0.2602946051208809;
 -1.81010098557185;
 -0.0003817657625064795;
 0.01448355587452157;
 5.978379488602926]

Image Points:
[81, 639;
 377, 437;
 356, 328;
 636, 146;
 693, 142;
 892, 185;
 936, 219;
 1024, 335;
 748, 573;
 90, 279;
 67, 265]

World Points:
[43.60545, 1.448632, 0;
 43.605026, 1.448012, 0;
 43.60479, 1.448023, 0;
 43.604374, 1.447412, 0;
 43.604362, 1.4472769, 0;
 43.604469, 1.446902, 0;
 43.604534, 1.446805, 0;
 43.604824, 1.446698, 0;
 43.60532, 1.447323, 0;
 43.604691, 1.4485461, 0;
 43.60466, 1.448594, 0]

RVec:
[0.06634425906241274;
 0.06145742401836587;
 1.493216220033204]

RMat:
[0.0765939613778385, -0.9936908925718362, 0.08192542401622219;
 0.9970623676531358, 0.07633574932511147, -0.006283978429072239;
 -9.506496968840483e-06, 0.0821660720416971, 0.9966186514985886]

TVec:
[-1.901595961898777;
 -43.58726706166781;
 -0.1164105524075865]

Camera Position:
[43.60487335704141;
 1.447233101331502;
 -0.002095462750983557] // 168 meters, seems fine

Camera Coordinates:
[500;
 482;
 1]

World Coordinates:
[43.60513764639308;
 1.447766480278994;
 1.163133494188979e-19]

[500, 482] > [43.6051, 1.44777, 0]

Expected: (500, 482) > [43.605129, 1.447795, 0.00]
```
