#!/bin/sh
# $Id: cleanupEmu.sh,v 1.6 2008/07/23 21:36:40 loizides Exp $

if test -e "/etc/profile.d/sm_env.sh"; then 
    source /etc/profile.d/sm_env.sh
fi

inst=`ps ax | grep "/bin/sh $0" | grep -v cron | grep -v grep | wc -l`

if test "$inst" != "2"; then
    echo "Output from ps: "
    ps ax | grep "/bin/sh $0" | grep -v cron | grep -v grep
    echo "Another instance running, exiting cleanly."
    exit 0;
fi

store=/store
if test -n "$SM_STORE"; then
    store=$SM_STORE
fi

#Path of emulator directory to cleanup
EMUDIR="$store/emulator/"

if ! test -d "$EMUDIR"; then
    echo "Dir $EMUDIR not found or not a directory"
    exit 123
fi
 
# lifetime in mins / 1 day = 1440
LIFETIME90=90
LIFETIME30=360
LIFETIME20=1440
LIFETIME00=10080
 
# date
EPOCH=`date +%s`

#Loop over all the links 00,01,02... where SM writes
for CUD in $( ls $EMUDIR | grep ^[0-9][0-9]$ ); do
  
    # find mount point of dir to cleanup
    mntpoint=`ls -l $EMUDIR/$CUD | cut -f2 -d">" | cut -f3 -d"/"`

    # find how full disk is to determine how much to delete
    LIFETIME=$(df | 
        awk -v LIFETIME90="$LIFETIME90" \
            -v LIFETIME30="$LIFETIME30" \
            -v LIFETIME20="$LIFETIME20" \
            -v LIFETIME00="$LIFETIME00" \
            -v pat="$mntpoint" \
           '$0 ~ pat {if ($5 > 90) print LIFETIME90; \
                 else if ($5 > 30) print LIFETIME30; \
                 else if ($5 > 20) print LIFETIME20; \
                 else print LIFETIME00; }' )

    #clean
    CUDdir="$EMUDIR/$CUD/"
    find $CUDdir -cmin +$LIFETIME -type f -exec rm -f {} \; >& /dev/null

done

# clean up old notify 
find $EMUDIR/mbox/ -iname "*.notify" -cmin +15000 -type f -exec rm -f {} \; >& /dev/null
