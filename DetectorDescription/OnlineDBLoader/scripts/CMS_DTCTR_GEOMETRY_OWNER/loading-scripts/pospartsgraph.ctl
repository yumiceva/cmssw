LOAD DATA
INFILE './data/POSPARTS.dat'
BADFILE './data/POSPARTSGRAPH.bad'
DISCARDFILE './data/POSPARTSGRAPH.dsc'
INSERT INTO TABLE CMSINTEGRATION.POSPARTSGRAPH
FIELDS TERMINATED by ","
(
 POSPARTID POSITION(1) INTEGER EXTERNAL,
 COPYNUMBER INTEGER EXTERNAL,
 skip_field3 FILLER,
 skip_field4 FILLER,
 skip_field5 FILLER,
 skip_field6 FILLER,
 LOGPARENT_NAME CHAR,
 LOGCHILD_NAME CHAR
)
INTO TABLE CMSINTEGRATION.RELPLACEMENTS
FIELDS TERMINATED by ","
(
 POSPARTID POSITION(1) INTEGER EXTERNAL,
 skip_field2 FILLER,
 X INTEGER EXTERNAL NULLIF X='',
 Y INTEGER EXTERNAL NULLIF Y='',
 Z INTEGER EXTERNAL NULLIF Z='',
 ROT_NAME CHAR
)
