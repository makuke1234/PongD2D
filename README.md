# PongD2D

![Release version](https://img.shields.io/badge/release-v1.3.0-green.svg)
![C version](https://img.shields.io/badge/version-C20-blue.svg)
![C++ version](https://img.shields.io/badge/version-C++20-blue.svg)

A pong fork written in C utilising Direct2D API (similar to my other project [SnakeD2D](https://github.com/makuke1234/SnakeD2D). Wrappers for Direct2D are written in C++.


# Obtaining

Windows binaries can be downloaded [here](https://github.com/makuke1234/PongD2D/releases).


# Controls

| Key    | Function             |
|--------|----------------------|
| W      | Move left wall up    |
| S      | Move left wall down  |
| Up     | Move right wall up   |
| Down   | Move right wall down |
| Escape | Pause/unpause        |
| Enter  | Restart game         |


# Changelog

* 1.3.0
	* Make scoring more engaging, points are given for whole game instead of the number
	of bounces, game ends at 10 points

* 1.2.0
	* Made ball movement more random/realistic
	* Added random number generator, mt19937-like engine
	*Wiki: [Mersenne Twister](https://en.wikipedia.org/wiki/Mersenne_Twister)*

* 1.1.1
	* Paddle movement speed is no longer affected by the frame rate

* 1.1.0
	* Move to a more responsive graphics approach.<br>
	*Kudos to [Creel](https://www.youtube.com/channel/UCq7dxy_qYNEBcHqQVCbc20w), his [video](https://www.youtube.com/watch?v=Rv0esscpAcQ) on D2D real-time game loops.*

	* Keyboard response is better too, raised new issue, keyboard response is tied to framerate

* 1.0.1
	* Smoother paddle operation / better keyboard response

* 0.9.1
	* Fix bug when compiling for release mode, add `volatile` to an important variable


# License

This project uses the MIT License.


# Credits

<div>Icons made by <a href="https://www.flaticon.com/authors/smashicons" title="Smashicons">Smashicons</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a></div>

