# PyPuzzle [![Build Status](https://travis-ci.org/ArchangelSDY/PyPuzzle.svg?branch=master)](https://travis-ci.org/ArchangelSDY/PyPuzzle)

PyPuzzle provides python bindings for libpuzzle. Libpuzzle is designed to quickly find visually similar images (gif, png, jpg), even if they have been resized, recompressed, recolored or slightly modified. Since libpuzzle is a C library, PyPuzzle connects it to python.

For more information about libpuzzle, consult its [home page](http://libpuzzle.pureftpd.org).

## Installation

At present, PyPuzzle has only been tested under Linux and OS X 10.9.

Before compiling, make sure you have header files of libpuzzle and libgd2 installed. See 'COMPILATION' section in [the doc of libpuzzle](http://download.pureftpd.org/pub/pure-ftpd/misc/libpuzzle/doc/README).

For Ubuntu users, you can just install them from source:

	apt-get install libgd2-xpm-dev
	apt-get install libpuzzle-dev

Note: In case of missing some image formats support, make sure you have corresponding image libraries installed.
For example, libjpeg/libpng, etc.

Then you can install PyPuzzle with pip:

```shell
$ pip install pypuzzle
```

or easy_install:

```shell
$ esay_install pypuzzle
```

In addition, you can compile it from source:

```shell
$ python setup.py install
```

## Usage

```python
import pypuzzle

puzzle = pypuzzle.Puzzle()

# Get distance between two image files
distance = puzzle.get_distance_from_file("file_1.jpg", "file_2.jpg")

# Get vector from image file
vec_1 = puzzle.get_cvec_from_file("file_1.jpg")
vec_2 = puzzle.get_cvec_from_file("file_2.jpg")

# Get distance between two vectors
distance = puzzle.get_distance_from_cvec(vec_1, vec_2)

# Compress vector
cmp_vec_1 = puzzle.compress_cvec(vec_1)

# Uncompress vector
uncmp_vec_1 = puzzle.uncompress_cvec(cmp_vec_1)
```

PyPuzzle allows you to change related parameters of the algorithm with set_*() methods:

```python
Puzzle.set_max_width(max_width)
Puzzle.set_max_height(max_height)
Puzzle.set_lambdas(lambdas)
Puzzle.set_p_ratio(p_ratio)
Puzzle.set_noise_cutoff(noise_cutoff)
Puzzle.set_contrast_barrier_for_cropping(barrier)
Puzzle.set_max_cropping_ratio(ratio)
Puzzle.set_autocrop(enable)
```

Consult [original page](http://download.pureftpd.org/pub/pure-ftpd/misc/libpuzzle/doc/man3/puzzle_set.3.html) of libpuzzle for more details.

## Changes

### v1.3

* Add Python 3 support

### v1.2

* Raise IOError in `get_cvec_from_file` and `get_distance_from_file` if error
occurred when reading from file.
