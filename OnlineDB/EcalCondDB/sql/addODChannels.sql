/*
 Add channels to CHANNELVIEW to describe CCUs not yet in the DB, in particular:

 CCU 69 and 70: serves PIN diodes
 CCU 71       : virtual CCU containign the global status of a CCS board
 CCU 127      : serves token rings in TRLB boards
*/
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 644, 69, NULL, 'EB_readout_tower', 1236440069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 625, 69, NULL, 'EB_readout_tower', 1236250069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 636, 69, NULL, 'EB_readout_tower', 1236360069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 639, 69, NULL, 'EB_readout_tower', 1236390069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 615, 69, NULL, 'EB_readout_tower', 1236150069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 632, 69, NULL, 'EB_readout_tower', 1236320069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 635, 69, NULL, 'EB_readout_tower', 1236350069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 618, 69, NULL, 'EB_readout_tower', 1236180069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 624, 69, NULL, 'EB_readout_tower', 1236240069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 628, 69, NULL, 'EB_readout_tower', 1236280069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 638, 69, NULL, 'EB_readout_tower', 1236380069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 617, 69, NULL, 'EB_readout_tower', 1236170069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 629, 69, NULL, 'EB_readout_tower', 1236290069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 630, 69, NULL, 'EB_readout_tower', 1236300069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 610, 69, NULL, 'EB_readout_tower', 1236100069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 614, 69, NULL, 'EB_readout_tower', 1236140069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 643, 69, NULL, 'EB_readout_tower', 1236430069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 645, 69, NULL, 'EB_readout_tower', 1236450069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 633, 69, NULL, 'EB_readout_tower', 1236330069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 611, 69, NULL, 'EB_readout_tower', 1236110069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 612, 69, NULL, 'EB_readout_tower', 1236120069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 619, 69, NULL, 'EB_readout_tower', 1236190069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 626, 69, NULL, 'EB_readout_tower', 1236260069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 627, 69, NULL, 'EB_readout_tower', 1236270069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 640, 69, NULL, 'EB_readout_tower', 1236400069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 616, 69, NULL, 'EB_readout_tower', 1236160069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 620, 69, NULL, 'EB_readout_tower', 1236200069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 621, 69, NULL, 'EB_readout_tower', 1236210069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 631, 69, NULL, 'EB_readout_tower', 1236310069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 634, 69, NULL, 'EB_readout_tower', 1236340069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 637, 69, NULL, 'EB_readout_tower', 1236370069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 641, 69, NULL, 'EB_readout_tower', 1236410069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 613, 69, NULL, 'EB_readout_tower', 1236130069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 642, 69, NULL, 'EB_readout_tower', 1236420069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 622, 69, NULL, 'EB_readout_tower', 1236220069);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 623, 69, NULL, 'EB_readout_tower', 1236230069);

INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 615, 70, NULL, 'EB_readout_tower', 1236150070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 618, 70, NULL, 'EB_readout_tower', 1236180070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 633, 70, NULL, 'EB_readout_tower', 1236330070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 613, 70, NULL, 'EB_readout_tower', 1236130070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 628, 70, NULL, 'EB_readout_tower', 1236280070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 638, 70, NULL, 'EB_readout_tower', 1236380070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 640, 70, NULL, 'EB_readout_tower', 1236400070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 642, 70, NULL, 'EB_readout_tower', 1236420070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 623, 70, NULL, 'EB_readout_tower', 1236230070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 625, 70, NULL, 'EB_readout_tower', 1236250070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 632, 70, NULL, 'EB_readout_tower', 1236320070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 634, 70, NULL, 'EB_readout_tower', 1236340070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 636, 70, NULL, 'EB_readout_tower', 1236360070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 641, 70, NULL, 'EB_readout_tower', 1236410070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 614, 70, NULL, 'EB_readout_tower', 1236140070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 621, 70, NULL, 'EB_readout_tower', 1236210070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 624, 70, NULL, 'EB_readout_tower', 1236240070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 629, 70, NULL, 'EB_readout_tower', 1236290070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 631, 70, NULL, 'EB_readout_tower', 1236310070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 635, 70, NULL, 'EB_readout_tower', 1236350070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 639, 70, NULL, 'EB_readout_tower', 1236390070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 612, 70, NULL, 'EB_readout_tower', 1236120070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 627, 70, NULL, 'EB_readout_tower', 1236270070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 637, 70, NULL, 'EB_readout_tower', 1236370070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 643, 70, NULL, 'EB_readout_tower', 1236430070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 616, 70, NULL, 'EB_readout_tower', 1236160070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 644, 70, NULL, 'EB_readout_tower', 1236440070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 645, 70, NULL, 'EB_readout_tower', 1236450070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 617, 70, NULL, 'EB_readout_tower', 1236170070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 610, 70, NULL, 'EB_readout_tower', 1236100070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 619, 70, NULL, 'EB_readout_tower', 1236190070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 620, 70, NULL, 'EB_readout_tower', 1236200070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 622, 70, NULL, 'EB_readout_tower', 1236220070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 626, 70, NULL, 'EB_readout_tower', 1236260070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 630, 70, NULL, 'EB_readout_tower', 1236300070);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 611, 70, NULL, 'EB_readout_tower', 1236110070);

/* virtual CCU 71: global CCS board status */
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 621, 71, NULL, 'EB_readout_tower', 1236210071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 623, 71, NULL, 'EB_readout_tower', 1236230071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 624, 71, NULL, 'EB_readout_tower', 1236240071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 639, 71, NULL, 'EB_readout_tower', 1236390071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 617, 71, NULL, 'EB_readout_tower', 1236170071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 629, 71, NULL, 'EB_readout_tower', 1236290071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 632, 71, NULL, 'EB_readout_tower', 1236320071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 634, 71, NULL, 'EB_readout_tower', 1236340071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 637, 71, NULL, 'EB_readout_tower', 1236370071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 631, 71, NULL, 'EB_readout_tower', 1236310071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 635, 71, NULL, 'EB_readout_tower', 1236350071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 644, 71, NULL, 'EB_readout_tower', 1236440071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 619, 71, NULL, 'EB_readout_tower', 1236190071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 628, 71, NULL, 'EB_readout_tower', 1236280071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 640, 71, NULL, 'EB_readout_tower', 1236400071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 614, 71, NULL, 'EB_readout_tower', 1236140071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 620, 71, NULL, 'EB_readout_tower', 1236200071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 625, 71, NULL, 'EB_readout_tower', 1236250071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 626, 71, NULL, 'EB_readout_tower', 1236260071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 636, 71, NULL, 'EB_readout_tower', 1236360071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 611, 71, NULL, 'EB_readout_tower', 1236110071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 615, 71, NULL, 'EB_readout_tower', 1236150071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 645, 71, NULL, 'EB_readout_tower', 1236450071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 622, 71, NULL, 'EB_readout_tower', 1236220071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 630, 71, NULL, 'EB_readout_tower', 1236300071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 633, 71, NULL, 'EB_readout_tower', 1236330071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 638, 71, NULL, 'EB_readout_tower', 1236380071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 616, 71, NULL, 'EB_readout_tower', 1236160071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 610, 71, NULL, 'EB_readout_tower', 1236100071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 612, 71, NULL, 'EB_readout_tower', 1236120071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 641, 71, NULL, 'EB_readout_tower', 1236410071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 613, 71, NULL, 'EB_readout_tower', 1236130071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 642, 71, NULL, 'EB_readout_tower', 1236420071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 643, 71, NULL, 'EB_readout_tower', 1236430071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 618, 71, NULL, 'EB_readout_tower', 1236180071);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 627, 71, NULL, 'EB_readout_tower', 1236270071);

/* CCU 127 serving token rings in TRLB boards */
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 627, 127, NULL, 'EB_readout_tower', 1236270127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 628, 127, NULL, 'EB_readout_tower', 1236280127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 631, 127, NULL, 'EB_readout_tower', 1236310127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 634, 127, NULL, 'EB_readout_tower', 1236340127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 637, 127, NULL, 'EB_readout_tower', 1236370127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 643, 127, NULL, 'EB_readout_tower', 1236430127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 644, 127, NULL, 'EB_readout_tower', 1236440127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 620, 127, NULL, 'EB_readout_tower', 1236200127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 639, 127, NULL, 'EB_readout_tower', 1236390127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 612, 127, NULL, 'EB_readout_tower', 1236120127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 613, 127, NULL, 'EB_readout_tower', 1236130127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 618, 127, NULL, 'EB_readout_tower', 1236180127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 633, 127, NULL, 'EB_readout_tower', 1236330127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 624, 127, NULL, 'EB_readout_tower', 1236240127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 626, 127, NULL, 'EB_readout_tower', 1236260127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 635, 127, NULL, 'EB_readout_tower', 1236350127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 641, 127, NULL, 'EB_readout_tower', 1236410127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 622, 127, NULL, 'EB_readout_tower', 1236220127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 623, 127, NULL, 'EB_readout_tower', 1236230127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 625, 127, NULL, 'EB_readout_tower', 1236250127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 638, 127, NULL, 'EB_readout_tower', 1236380127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 640, 127, NULL, 'EB_readout_tower', 1236400127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 615, 127, NULL, 'EB_readout_tower', 1236150127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 616, 127, NULL, 'EB_readout_tower', 1236160127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 645, 127, NULL, 'EB_readout_tower', 1236450127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 617, 127, NULL, 'EB_readout_tower', 1236170127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 619, 127, NULL, 'EB_readout_tower', 1236190127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 610, 127, NULL, 'EB_readout_tower', 1236100127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 621, 127, NULL, 'EB_readout_tower', 1236210127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 629, 127, NULL, 'EB_readout_tower', 1236290127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 636, 127, NULL, 'EB_readout_tower', 1236360127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 611, 127, NULL, 'EB_readout_tower', 1236110127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 614, 127, NULL, 'EB_readout_tower', 1236140127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 642, 127, NULL, 'EB_readout_tower', 1236420127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 630, 127, NULL, 'EB_readout_tower', 1236300127);
INSERT INTO CHANNELVIEW VALUES ('EB_readout_tower', 632, 127, NULL, 'EB_readout_tower', 1236320127);
