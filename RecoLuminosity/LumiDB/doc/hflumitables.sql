/*=========================LUMIDATA=======================*/
CREATE TABLE "LUMIDATA"
(	"DATA_ID" NUMBER(20,0),
	"ENTRY_ID" NUMBER(20,0),
	"ENTRY_NAME" VARCHAR2(56),
	"SOURCE" VARCHAR2(128),
	"RUNNUM" NUMBER(10,0),
	"NOMINALEGEV" BINARY_FLOAT,
	 CONSTRAINT "LUMIDATA_PK" PRIMARY KEY ("DATA_ID")
)
PARTITION BY RANGE(RUNNUM)
(
PARTITION YEAR2010 VALUES LESS THAN(153368),
PARTITION YEAR2011 VALUES LESS THAN(183338),
PARTITION THEREST VALUES LESS THAN(MAXVALUE)
)
;
GRANT SELECT ON "LUMIDATA" TO PUBLIC;
GRANT SELECT,INSERT,DELETE,UPDATE ON "LUMIDATA" TO CMS_LUMI_WRITER;
CREATE INDEX LUMIDATA_RUNNUM_IDX ON LUMIDATA(RUNNUM) LOCAL;
/*=========================LUMIDATA_ID=======================*/
CREATE TABLE "LUMIDATA_ID"
(	"NEXTID" NUMBER(20,0),
	 CONSTRAINT "LUMIDATA_ID_PK" PRIMARY KEY ("NEXTID")
);
INSERT INTO "LUMIDATA_ID"("NEXTID") VALUES(0);
GRANT SELECT ON "LUMIDATA_ID" TO PUBLIC;
GRANT SELECT,INSERT,DELETE,UPDATE ON "LUMIDATA_ID" TO CMS_LUMI_WRITER;
/*=========================LUMIDATA_ENTRIES=======================*/
CREATE TABLE "LUMIDATA_ENTRIES"
(	"ENTRY_ID" NUMBER(20,0),
	"REVISION_ID" NUMBER(20,0),
	"NAME" VARCHAR2(56),
	 CONSTRAINT "LUMIDATA_ENTRIES_PK" PRIMARY KEY ("ENTRY_ID")
);
GRANT SELECT ON "LUMIDATA_ENTRIES" TO PUBLIC;
GRANT SELECT,INSERT,DELETE,UPDATE ON "LUMIDATA_ENTRIES" TO CMS_LUMI_WRITER;
/*=========================LUMIDATA_ENTRIES_ID=======================*/
CREATE TABLE "LUMIDATA_ENTRIES_ID"
(	"NEXTID" NUMBER(20,0),
	 CONSTRAINT "LUMIDATA_ENTRIES_ID_PK" PRIMARY KEY ("NEXTID")
);
INSERT INTO "LUMIDATA_ENTRIES_ID"("NEXTID") VALUES(0);
GRANT SELECT ON "LUMIDATA_ENTRIES_ID" TO PUBLIC;
GRANT SELECT,INSERT,DELETE,UPDATE ON "LUMIDATA_ENTRIES_ID" TO CMS_LUMI_WRITER;
/*=========================LUMIDATA_REV=======================*/
CREATE TABLE "LUMIDATA_REV"
(	"DATA_ID" NUMBER(20,0),
	"REVISION_ID" NUMBER(20,0),
	 CONSTRAINT "LUMIDATA_REV_PK" PRIMARY KEY("DATA_ID","REVISION_ID")
);
GRANT SELECT ON "LUMIDATA_REV" TO PUBLIC;
GRANT SELECT,INSERT,DELETE,UPDATE ON "LUMIDATA_REV" TO CMS_LUMI_WRITER;
/*=========================LUMISUMMARYV2=======================*/
CREATE TABLE "LUMISUMMARYV2"
("DATA_ID" NUMBER(20,0),
"RUNNUM" NUMBER(10,0),
"LUMILSNUM" NUMBER(10,0),
"CMSLSNUM" NUMBER(10,0),
"INSTLUMI" BINARY_FLOAT,
"INSTLUMIERROR" BINARY_FLOAT,
"INSTLUMIQUALITY" NUMBER(5,0),
"BEAMSTATUS" VARCHAR2(28),
"BEAMENERGY" BINARY_FLOAT,
"NUMORBIT" NUMBER(10,0),
"STARTORBIT" NUMBER(10,0),
"CMSBXINDEXBLOB" BLOB,
"BEAMINTENSITYBLOB_1" BLOB,
"BEAMINTENSITYBLOB_2" BLOB,
"BXLUMIVALUE_OCC1" BLOB,
"BXLUMIVALUE_OCC2" BLOB,
"BXLUMIVALUE_ET" BLOB,
"BXLUMIERROR_OCC1" BLOB,
"BXLUMIERROR_OCC2" BLOB,
"BXLUMIERROR_ET" BLOB,
"BXLUMIQUALITY_OCC1" BLOB,
"BXLUMIQUALITY_OCC2" BLOB,
"BXLUMIQUALITY_ET" BLOB,
 CONSTRAINT "LUMISUMMARYV2_PK" PRIMARY KEY ("DATA_ID", "LUMILSNUM"))
PARTITION BY RANGE(DATA_ID)(
PARTITION LUMISUMMARYV2_P1 VALUES LESS THAN(500),
PARTITION LUMISUMMARYV2_P2 VALUES LESS THAN(1000),
PARTITION LUMISUMMARYV2_P3 VALUES LESS THAN(1500),
PARTITION LUMISUMMARYV2_P4 VALUES LESS THAN(2000),
PARTITION THEREST VALUES LESS THAN(MAXVALUE)
);
GRANT SELECT ON "LUMISUMMARYV2" TO PUBLIC;
GRANT SELECT,INSERT,DELETE,UPDATE ON "LUMISUMMARYV2" TO CMS_LUMI_WRITER;
