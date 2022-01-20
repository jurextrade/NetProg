(loadresource "a.dg")
(setq pcom (opencom "dev21" 1999 "jureidini" "jurex"))

(onsendmessage "A" "essai" 'essai')
(onrecvmessage "A" "essai1" 'essai1')

(defun essai (pcom a b)
  (print a "1 parameter")
  (print b "2 parameter")
)

(defun essai1 ( pcom a b c) 
  (print a "1 parmeter")
  (print b "2 parameter")
  (print c "3 parmeter")
  (putmessage pcom "A" "essai")
)



(sendmessage pcom "A" "essai" "jureidini" "gabriel")
(system "rlogin dev36")


