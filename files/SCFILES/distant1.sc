;EXAMPLE RECUPERATION DE FICHIER SFS ET AFFICHAGE
;================================================


(defun getfile (name port filename)
 (setq pcom (opencom name port 1))
 (rcd pcom "/sfstvf1/soc0025/msg" 'r')
 (cd "c:\temp")
 (eval 
     (get pcom  filename 't' 'd' 'r')  
     (setq file (open filename "r" 't'))
     (while (setq a (getline file)) (print a))
     (close file)
 ) 
 (closecom pcom)
)

(getfile "vmesa" 3022 "AAV0GR07.04576233")

