(loadclass "d:/msdev/projects/admin/admin.dg")
(when (recvmessage "ADMIN" "Envoi_Erreur") (print "erreur"))

(setq aixdev (opencom "aixdev" 4000 "jureidini" "jurex"))

(remote aixdev (when (recvmessage "ADMIN" "Envoi_Erreur") (putmessage this "ADMIN" "Envoi_Erreur" 2 "1" "2" "3")))
(putmessage aixdev "ADMIN" "Envoi_Erreur" 2 "ldkflfkdlk" 1 "slksldkl")

(when (and (remote aixdev (equal a 2)) (equal a 2)) (print "ok"))
(remote aixdev (setq a 2))
(remote aixdev (setq a 3))
(setq a 2)
(look)
