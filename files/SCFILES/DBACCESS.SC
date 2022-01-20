(setq pcom (opencom "oemcomputer" 2001 "jureidini" "jurex"))
(setq pclient (dbconnect pcom "plvsjm" "" ""))
(dbprocess pclient "select * from PRODUIT where cip < 10000")