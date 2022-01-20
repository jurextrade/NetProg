(setq vmesa (opencom "vmesa" 3022 "jureidini" "jurex"))
(remote vmesa (system "acc sfstvf1:invent. w"))
(remote vmesa (system "flist * * w"))
(cd "c:\temp")
(get "AAAAM invent w" "invent"  'b' 'r')



(remote vmesa
(defun tel (pcom name)
  (share pcom name)
  (share pcom line)
  (system "dropbuf")
  (setq name1 (add "PIPE CMS tel " name " ! STACK ! COUNT LINES ! STACK LIFO"))
  (print name1)
  (system name1)
  (setq nbrstr (getline))
  (print nbrstr)
  (setq i (atoi nbrstr)) (setq j 0)
  (while (not (equal j i))(eval (setq j (add j 1))
                                (setq line (getline))
                                (remote pcom line)))
)
)
line
(remote vmesa (tel this "j"))
