;EXAMPLE DE VARIABLE PARTAGEE
;============================

(setq vmesa (opencom "vmesa" 3022 "jureidini" "jurex"))
(setq a b)
(remote vmesa (when (equal b 3) (print "ok")))
(share vmesa b)
(setq b 3)
(remote vmesa a)
(remote vmesa f)

(setq h 4)
(closecom vmesa)

(when (equal g 3) (print "dfldflk"))
(set g (add 1 2))
(setq g 3)
(setq a 3)

(setq vmesa (opencom "aixdev" 2000 1))
(share vmesa h)

(print h)
(remote vmesa h)

(remote vmesa (when (equal h 3) (print "ok")))
(setq h 3)
(setq h 4)
