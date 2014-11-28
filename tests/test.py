#! /usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import pypuzzle

class TestPyPuzzle(unittest.TestCase):
    def setUp(self):
       self.puzzle = pypuzzle.Puzzle()

    def test_get_cvec_from_file(self):
        cvec = self.puzzle.get_cvec_from_file('1.jpg')
        self.assertIsNotNone(cvec)
        self.assertTrue(len(cvec) > 0)

    def test_get_cvec_from_invalid_file(self):
        with self.assertRaises(IOError) as cm:
            self.puzzle.get_cvec_from_file('invalid.jpg')

        self.assertEqual(str(cm.exception), 'Fail to read file: invalid.jpg')

    def test_get_distance_from_file(self):
        distance = self.puzzle.get_distance_from_file('1.jpg', '2.jpg')
        self.assertTrue(distance > 0.01)

    def test_get_distance_from_invalid_file(self):
        with self.assertRaises(IOError) as cm:
            self.puzzle.get_distance_from_file('1.jpg', 'invalid.jpg')

        self.assertEqual(str(cm.exception), 'Fail to read file: invalid.jpg')

    def test_get_distance_from_cvec(self):
        cvec_1 = self.puzzle.get_cvec_from_file('1.jpg')
        cvec_2 = self.puzzle.get_cvec_from_file('2.jpg')
        distance = self.puzzle.get_distance_from_cvec(cvec_1, cvec_2)
        self.assertTrue(distance > 0.01)

    def test_get_distance_from_cvec_invalid_types(self):
        # Invalid arg type should not lead to crash
        with self.assertRaises(TypeError):
            self.puzzle.get_distance_from_cvec(1, 2)

    def test_compress_cvec(self):
        cvec = self.puzzle.get_cvec_from_file('1.jpg')
        compressed_cvec = self.puzzle.compress_cvec(cvec)
        uncompressed_cvec = self.puzzle.uncompress_cvec(compressed_cvec)
        self.assertEqual(cvec, uncompressed_cvec)

if __name__ == '__main__':
    unittest.main()
