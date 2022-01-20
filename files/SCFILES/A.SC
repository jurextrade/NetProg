(addport 1999)
(loadresource "a.dg")
(onrecvmessage "A" "essai" 'essai')


(defun essai (pcom a b)  (print a "1 parameter")
  (print b "2 parmeter")
  ;(sendmessage pcom "A" "essai1"  1 3 "JUREIDINI")
)
