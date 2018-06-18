#!/bin/sh
BIN=
SBIN=
OSTEST=
BOOTPARAM="-c 1-7 -m 10G@0"

if [ -f ../../../config.h ]; then
	str=`grep "^#define BINDIR " ../../../config.h | head -1 | sed 's/^#define BINDIR /BINDIR=/'`
	eval $str
fi
if [ "x$BINDIR" = x ];then
	BINDIR="$BIN"
fi

if [ -f ../../../Makefile ]; then
	str=`grep ^SBINDIR ../../../Makefile | head -1 | sed 's/ //g'`
	eval $str
fi
if [ "x$SBINDIR" = x ];then
	SBINDIR="$SBIN"
fi

if [ -f $HOME/ostest/bin/test_mck ]; then
	OSTESTDIR="$HOME/ostest"
fi
if [ "x$OSTESTDIR" = x ]; then
	OSTESTDIR="$OSTEST"
fi

if ! lsmod | grep mcctrl > /dev/null 2>&1; then
	if [ ! -x $SBINDIR/mcreboot.sh ]; then
		echo no mcreboot found >&2
		exit 1
	fi
	sudo $SBINDIR/mcreboot.sh $BOOTPARAM
fi

if [ ! -x $SBINDIR/ihkosctl ]; then
	echo no ihkosctl found >&2
	exit 1
fi

maxmem=`$SBINDIR/ihkosctl 0 query mem | cut -d '@' -f 1`
mem95p=`expr $maxmem \* 95 / 100`
mem110p=`expr $maxmem \* 110 / 100`

if [ ! -x $BINDIR/mcexec ]; then
	echo no mcexec found >&2
	exit 1
fi

for i in 10240:9961472:01 2097152:2040109466:02 unlimited:$mem95p:03; do
	ul=`echo $i|sed 's/:.*//'`
	st=`echo $i|sed -e 's/^[^:]*://' -e 's/:[^:]*$//'`
	id=`echo $i|sed 's/.*://'`

	sudo sh -c "ulimit -s $ul; $BINDIR/mcexec $OSTESTDIR/bin/test_mck -s mem_stack_limits -n 0 -- -s $st" 2>&1 | tee C1109T$id.txt
	if grep "RESULT: ok" C1109T$id.txt > /dev/null 2>&1; then
		echo "*** C1109T$id: OK"
	else
		echo "*** C1109T$id: NG"
	fi
done

for i in 10M:9961472:04 2G:2040109466:05 100000G:$mem95p:06; do
	ul=`echo $i|sed 's/:.*//'`
	st=`echo $i|sed -e 's/^[^:]*://' -e 's/:[^:]*$//'`
	id=`echo $i|sed 's/.*://'`

	$BINDIR/mcexec -s 2M,$ul $OSTESTDIR/bin/test_mck -s mem_stack_limits -n 0 -- -s $st 2>&1 | tee C1109T$id.txt
	if grep "RESULT: ok" C1109T$id.txt > /dev/null 2>&1; then
		echo "*** C1109T$id: OK"
	else
		echo "*** C1109T$id: NG"
	fi
done
