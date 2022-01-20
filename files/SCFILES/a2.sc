(setq aixdev (opencom "aixdev" 2000 "jureidini" "jurex"))
(remote aixdev (setq name3 3))
(remote aixdev (share this name3))

(remote aixdev
(defun tel (pcom name)
  (setq name3 name) 
  (setq i 3) (setq j 0)
  (while (not (equal j i))(eval (setq j (add j 1)) (remote pcom name3))) 
)
)

(remote aixdev (tel this "haine"))
NULL
(share aixdev h)
(remote aixdev (when (equal h 3) (print "ok")))

(setq h 3)


