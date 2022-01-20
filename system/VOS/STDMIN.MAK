# Makefile : std    min.mak !!
# AUTEUR   : Jerome JEAN-CHARLES 

.INCLUDE %devlp#m1_d02>PRJ>MAK>std_base.mak
_CHECK_UNINIT_ =  -check_uninitialized
COPTS = $(COPTS) $(FULL_PROTO) # ajout du 20/02 pour avoir 1 prototypage fort 
COPTS = $(COPTS) $(_CHECK_UNINIT_) # ajout du 25/09/97  
# eof stdmin.mak

