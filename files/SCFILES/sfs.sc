;EXAMPLE RECUPERATION DE FICHIER SFS ET AFFICHAGE
;================================================


(defun getfile (name port filename)
 (setq pcom (opencom name port "jureidini" "jurex"))
 (rcd pcom "/sfstvf1/soc0075/msg" 'r')
 (cd "c:\temp")
 (eval 
     (get pcom  filename 't' 'd' 'r')  
     (setq file (open filename "r" 't'))
     (while (setq a (getline file)) (print a))
     (if file (close file))
 ) 
 (closecom pcom)
)


;EXAMPLE RECUPERATION DE FICHIER CMS ET AFFICHAGE
;================================================


(defun getcmsfile (name port filename)
 (setq pcom (opencom name port "jureidini" "jurex"))
 (cd "c:\temp")
 (eval 
     (get pcom  filename 't' 'd' 'd')  
     (setq file (open filename "r" 't'))
     (while (setq a (getline file)) (print a))
     (if file (close file))
 ) 
 (closecom pcom)
)

;(getfile "vmesa" 3022 "profile.exec")


(getcmsfile "vmesa" 3022 "profile.exec")

;(java "led")

