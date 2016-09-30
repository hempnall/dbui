# DBUI

## Synopsis

DBUI is a very basic ORM comprising a set of QML elements

## Installation

DBUI must be installed as a QML module.

```
$ export PATH=$PATH:~/Qt54//5.4/clang_64/bin
$ git clone <this-repo>
$ cd <this-repo>
$ qmake
$ make
$ make install
... installs ...
$ ../build-dbui-Desktop_Qt_5_4_1_clang_64bit-Debug/tests/tst_example
********* Start testing of example *********
Config: Using QtTest library 5.4.1, Qt 5.4.1 (x86_64-little_endian-lp64 shared (dynamic) release build; by Clang 6.0 (clang-600.0.54) (Apple))
PASS   : example::Create and Destroy DB::initTestCase()
PASS   : example::Create and Destroy DB::test_create_and_destroy()
PASS   : example::Create and Destroy DB::cleanupTestCase()
PASS   : example::Create and Destroy Data (addnew)::initTestCase()
PASS   : example::Create and Destroy Data (addnew)::test_normal()
PASS   : example::Create and Destroy Data (addnew)::cleanupTestCase()
PASS   : example::Create and Destroy Data::initTestCase()
PASS   : example::Create and Destroy Data::test_a_addnew()
PASS   : example::Create and Destroy Data::test_b_addnew_record()
PASS   : example::Create and Destroy Data::test_c_filtering()
PASS   : example::Create and Destroy Data::cleanupTestCase()
Totals: 11 passed, 0 failed, 0 skipped, 0 blacklisted
********* Finished testing of example *********
$ echo "phew that was lucky!!"
```

## Samples

```

```
