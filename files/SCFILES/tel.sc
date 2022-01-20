;EXAMPLE RECUPERATION DE NUMERO DE TELEPHONE CMS
;================================================


(defun tel (machine port name)
  (setq pcom (opencom machine port "jureidini" "jurex"))
  (setq telname name) 
  (share pcom telname) 
  (remote pcom (system "dropbuf"))
  (remote pcom (setq name1 (add "PIPE CMS tel " telname " ! STACK ! COUNT LINES ! STACK LIFO")))
  (remote pcom  (system name1))
  (setq nbrstr (remote pcom (getline)))
  (setq i (atoi nbrstr)) (setq j 0)
  (eval (while (not (equal j i))(eval (setq j (add j 1))
                                (remote pcom (getline))))
  )
  (closecom pcom)
)

(tel "vmesa" 3022 "guba")


