

TEMPLATE = subdirs

CONFIG += ordered
CONFIG += c++11

SUBDIRS = \
    src \
    tests

tests.depends = src

