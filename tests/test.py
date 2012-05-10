#! /usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import pypuzzle

class TestPyPuzzle(unittest.TestCase):
    def setUp(self):
       self.puzzle = pypuzzle.Puzzle()

    def test_get_distance_from_file(self):
        distance = self.puzzle.get_distance_from_file('1.jpg', '2.jpg')
        self.assertAlmostEqual(distance, 0.0286299)

    def test_get_distance_from_cvec(self):
        cvec_1 = self.puzzle.get_cvec_from_file('1.jpg')
        cvec_2 = self.puzzle.get_cvec_from_file('2.jpg')
        distance = self.puzzle.get_distance_from_cvec(cvec_1, cvec_2)
        self.assertAlmostEqual(distance, 0.0286299)

    def test_compress_cvec(self):
        cvec = self.puzzle.get_cvec_from_file('1.jpg')
        compressed_cvec = self.puzzle.compress_cvec(cvec)
        uncompressed_cvec = self.puzzle.uncompress_cvec(compressed_cvec)
        self.assertEqual(cvec, uncompressed_cvec)

if __name__ == '__main__':
    unittest.main()
