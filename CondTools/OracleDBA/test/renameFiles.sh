#!/bin/bash

PARAM=3
PWD=`pwd`

E_ERR_ARG=65
E_NOFILE=66

DBFILE=$1
METADATAFILE=$2
TAGNAME=$3
UUID=`uuidgen -t`
if [ $# -ne "$PARAM" ]
    then
    echo
    echo "Usage: ./renameFiles.sh <sqlitefile> <metadatafile> <tagname>"
    exit $E_ERR_ARG
fi

if [ ! -e "${DBFILE}" ]
    then
    echo
    echo "$DBFILE does not exist"
    exit $E_NOFILE
fi

if [ ! -e "$METADATAFILE" ]
    then
    echo
    echo "$METADATAFILE does not exist"
    exit $E_NOFILE
fi

cp $DBFILE ${PWD}/${TAGNAME}@${UUID}.db
cp $METADATAFILE ${PWD}/${TAGNAME}@${UUID}.txt

exit 0
