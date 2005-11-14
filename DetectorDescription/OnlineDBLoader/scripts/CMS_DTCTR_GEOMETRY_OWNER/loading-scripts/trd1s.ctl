LOAD DATA
INFILE './data/TRD1S.dat' 
BADFILE './data/TRD1S.bad'
DISCARDFILE './data/TRD1S.dsc'
INSERT INTO TABLE CMSINTEGRATION.TRAPEZOIDS
FIELDS TERMINATED by ","
(
 SOL_NAME CHAR, 
 DX2 INTEGER EXTERNAL,
 DX1 INTEGER EXTERNAL,
 DY1 INTEGER EXTERNAL,
 DZ  INTEGER EXTERNAL,
 DY2 INTEGER EXTERNAL
)
