#    Copyright 2012 Walker Mills
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#    Authored by Walker Mills

"""Fix incompatibilities between Python 2 and Python 3."""

from __future__ import print_function
import sys
import itertools
import unittest
if sys.version_info.major == 2:
    import __builtin__
elif sys.version_info.major == 3:
    import builtins as __builtin__


class _TestSuite(unittest.TestCase):

    def test_basestring(self):
        """Test the basestring property.

        Tests whether or not the method returns the correct type of value,
        as its type depends on what version of Python is run.

        Returns:
            This method returns no values, but it does modify the 
            unittest.TestCase instance

        Usage:
            The _TestSuite.unit_test method will run this and other tests

        """

        if sys.version_info.major == 2:
            self.assertEqual(basestring, __builtin__.basestring)
        elif sys.version_info.major == 3:
            self.assertEqual(basestring, __builtin__.str)

    def test_long(self):
        """Test the long property.

        Tests whether or not the method returns the correct type of value,
        as its type depends on what version of Python is run.

        Returns:
            This method returns no values, but it does modify the 
            unittest.TestCase instance

        Usage:
            The _TestSuite.unit_test method will run this and other tests

        """

        if sys.version_info.major == 2:
            self.assertEqual(basestring, __builtin__.long)
        elif sys.version_info.major == 3:
            self.assertEqual(basestring, __builtin__.int)

    def test_range(self):
        """Test the range method.

        Tests whether or not the method returns the correct type of value,
        as its type depends on what version of Python is run.

        Returns:
            This method returns no values, but it does modify the 
            unittest.TestCase instance

        Usage:
            The _TestSuite.unit_test method will run this and other tests

        """

        iterator = range(0)
        if sys.version_info.major == 2:
            self.assertIsInstance(iterator, __builtin__.xrange)
        elif sys.version_info.major == 3:
            self.assertIsInstance(iterator, __builtin__.range)

    def test_two_or_three(self):
        """Test the two_or_three method.

        Tests whether or not the method returns the correct type of value,
        as its type depends on what version of Python is run.

        Returns:
            This method returns no values, but it does modify the 
            unittest.TestCase instance

        Usage:
            The _TestSuite.unit_test method will run this and other tests

        """

        output = two_or_three("2", "3")
        if sys.version_info.major == 2:
            self.assertTrue(output == 2)
        elif sys.version_info.major == 3:
            self.assertTrue(output == 3)

    def test_unicode(self):
        """Test the unicode method.

        Tests whether or not the method returns the correct type of value,
        as its type depends on what version of Python is run.

        Returns:
            This method returns no values, but it does modify the 
            unittest.TestCase instance

        Usage:
            The _TestSuite.unit_test method will run this and other tests

        """

        string = unicode("a string")
        if sys.version_info.major == 2:
            self.assertIsInstance(string, __builtin__.unicode)
        elif sys.version_info.major == 3:
            self.assertIsInstance(string, __builtin__.str)

    def test_zip(self):
        """Test the zip method.

        Tests whether or not the method returns the correct type of value,
        as its type depends on what version of Python is run.

        Returns:
            This method returns no values, but it does modify the 
            unittest.TestCase instance

        Usage:
            The _TestSuite.unit_test method will run this and other tests

        """

        zipped = zip((), ())
        if sys.version_info.major == 2:
            self.assertIsInstance(zipped, itertools.izip)
        elif sys.version_info.major == 3:
            self.assertIsInstance(zipped, __builtin__.zip)

    @staticmethod
    def unit_test(test_suite, *args, **kwargs):
        """Test that other methods are functioning properly.

        This method uses the unittest module to execute a test suite.

        Positional Arguments:
            test_suite -- a class which contains tests to be loaded by the
                          unittest module
            args -- positional arguments which are passed to
                    unittest.TextTestRunner (see the documentation for
                    unittest.TextTestRunner for more detail)

        Optional Arguments:
            kwargs -- optional arguments which are passed to 
                      unittest.TextTestRunner (see the documentation for
                      unittest.TextTestRunner for more detail)

        Returns:
            This method returns no values; depending on the verbosity,
            it outputs varying amounts of detail concerning the tests
            being run to stderr

        Usage:
            >>> import unittest
            >>> class test_suite(unittest.TestCase)
            ...
            ...     def test_foo(self):
            ...         pass
            ...
            ...     def test_foo(self):
            ...         pass
            ...
            >>> _TestSuite.unit_test(test_suite)
            test_foo (__main__.test_suite) ... ok
            test_bar (__main__.test_suite) ... ok

            ----------------------------------------------------------------------
            Ran 2 tests in 0.000s

            OK

        """

        if 'verbosity' not in kwargs.keys():
            kwargs['verbosity'] = 2
        if 'buffer' not in kwargs.keys():
            kwargs['buffer'] = True
        if 'descriptions' not in kwargs.keys():
            kwargs['descriptions'] = False
        suite = unittest.TestLoader().loadTestsFromTestCase(test_suite)
        unittest.TextTestRunner(*args, **kwargs).run(suite)

def two_or_three(python2, python3):
    if sys.version_info.major == 2:
        return eval(python2)
    elif sys.version_info.major == 3:
        return eval(python3)

basestring = two_or_three("__builtin__.basestring", "__builtin__.str")
long = two_or_three("__builtin__.long", "__builtin__.int")
range = two_or_three("__builtin__.xrange", "__builtin__.range")
unicode = two_or_three("__builtin__.unicode", "__builtin__.str")
zip = two_or_three("itertools.izip", "__builtin__.zip")
