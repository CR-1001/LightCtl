#!/bin/bash

# generates the help output (i.e. cout << "<line>" << endl)
# might be simplyfied.

awk '{print "    << \""$0"\" << endl"}' ../../README.md | sed s/"\"\" << endl"/endl/