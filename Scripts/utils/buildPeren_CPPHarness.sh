#!/bin/sh
# ====================================================================
#
# Copyright (C) 2011, Hewlett-Packard Development Company, L.P.
# All Rights Reserved.
#
# Open64 is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# Open64 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301, USA.
#
# ====================================================================
#
# Pre-run hook for building Cperen harness.
#
# Description:
# ------------
# All CPeren tests are required to be linked with
# scafold file, "scafold.o and stub.o", containing some support functions
# for running the test. This harness file has to be compiled with
# the same flavor of compiler that we use for the test itself (e.g. 
# same architecture, same data mode, etc). Rather than rely on 
# precompiled copies of the scafold file, we build the
# harness scafold on the fly as part of this hook.
#
# Notes:
# ------
# Ideally for a single cperen run (which includes many units)
# we would do a single compile of the harness source files, then 
# use the resulting scafold  file for all remaining tests. With
# distributed tests execution, this is hard to arrange, however, 
# since we can potentially have all units in the run launched
# in parallel. We deal with this by compiling the harness
# for every unit. This is wasteful in that we incur multiple
# harness compiles, but it does have certain advantages-- if
# there are process problems with machine X, those problems
# will only be seen by units launched to machine X.
#
#
# Arguments:
# $1 - unit name (ex: Regression/bbopt)
# $2 - unit work dir (full path)
# $3 - file to append output to
#
ME=$0
UNIT=$1
WORKDIR=$2
OUT=$3
#
if (test "$SHOW_SCRIPT_TRACE" = "true") then
  echo $ME: params: U=$1 W=$2 O=$3
fi
#
error() { 
  typeset args="$*"
  if (test "$OUT" != "") then
    echo "# $ME: fatal error: $args" >> $OUT
  else
    echo "$ME: fatal error: $args"
  fi
  exit 1
}
warning() { 
  typeset args="$*"
  if (test "$OUT" != "") then
    echo "# $ME: $args" >> $OUT
  else
    echo "$ME: $args"
  fi
}
#
if (test ! -d "$WORKDIR") then
  error "can't access unit work dir $WORK_DIR"
fi
if (test "x$UNIT" = "x") then
  error "bad command line options: no UNIT specified"
fi
if (test "x$OUT" = "x") then
  error "bad command line options: no output file specified"
fi

#
# Change to unit work directory
#
cd $TEST_WORK_DIR/$UNIT
if (test $? != 0) then
  error "can't cd to test work dir $TEST_WORK_DIR/$UNIT"
fi

#
# Build harness using current compiler settings. 
#
mkdir scaffolddir 
if (test $? != 0) then
  error "can't create scafolddir subdir"
fi

PERENDIR=/path/to/TESTC++/Perennial/peren_CTI/CCVS/testsrc

ln -s $PERENDIR/driver.cfg $PERENDIR/err_data $PERENDIR/err_msg .

cd scaffolddir
ln -s $PERENDIR/../scaffold/scaffold.C . 
ln -s $PERENDIR/../scaffold/stub.C . 
ln -s $PERENDIR/include .

BCMD="$CXX $CXX_OPTIONS $DATA_MODE_FLAG ${CTI_OPT_SIGN}O${OPT_LEVEL} scaffold.C stub.C -c  -Iinclude"

$BCMD  1> PreRunHookErr 2>&1

ST=$?
if (test $ST != 0) then
  warning "error: build failed (exit status $ST)"
  warning "build cmd: $BCMD"
  HERE=`pwd`
  error "build output is in file $HERE/err"
fi
if (test ! -f scaffold.o) then
  error "harness build failed (no object file)"
fi
if (test ! -f stub.o) then
  error "harness build failed (no object file)"
fi

cd ..

#
# Set up object file link for stub and scaffold
#
ln -s scaffolddir/scaffold.o .
if (test $? != 0) then
  error "link failed (exit status $?)"
fi

ln -s scaffolddir/stub.o .
if (test $? != 0) then
  error "link failed (exit status $?)"
fi
if (test "$SHOW_SCRIPT_TRACE" = "true") then
  echo $ME: successfully built $TEST_WORK_DIR/$UNIT/scaffolddir/scaffold.o and stub.o
fi

exit 0


