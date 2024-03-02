# LISTE DES LIBRAIRES AVEC LEUR DEFINE ET LEURS ACCES
# VERSION : 19/07/96  
# AUTEUR : Jerome JEAN-CHARLES
# FONCTION : 
#  Les numeros de version ne sont pas definies, l'utilisateur
#  est donc oblige de le definir pour acceder a l'OBJ et L'INC
#  qui correspondent sinon il recoit un message 
#  "invalid patch ( null component)"
#  ce messsage n'est recu que s'il y a appel des macros de paths predefinies
#  par H_SERACHES ou BIND_SEARCHES
#  MODJER100197 : correct bug done at 080197 on TERA LIB 's version missing
#  MODJER110998 : 1 lib IF_API 
# la logique de nommage est :
#  COMPOSANT : XXX
#  VERSION   : VER_XXX (seule a mettre a jour par ultisateur)
#  INCLUDE   : INC_XXX ( a referer dans H_SEARCHES )
#  OBJET     : OBJ_XXX ( pour BIND_SEARCHES) 
#
# REMARQUE : seule VER_SYB peut etre utilise sans initprealable 
########## end of doc ############## 

######  LISTE DES VERSIONS A METTRE A JOUR PAR LES APPELANTS (IF USED) #####

#VER_TERA=V4  # version avec longmap explicite 
#VER_TCP= V1.0 # should not change normally 
#VER_SYB = PAS DE VERSION 

#VER_VOSV3FIC    = 1.3  # ex lib de roissy utilise partout (91)

#VER_VOSFILE    = 1.2
#VER_LIB   = V1.3
#VER_SQL      = V2.2

#VER_INC_LIBGEN  = E_1.0
#VER_INC_GEN  = V1.3 #link 

#VER_OPC_BIN = V2.0  # msg_trace , nosq , serv_help 
################ LIBRAIRIE SYSTEMES #################

########  librairie d'acces a la teradata
#VER_TERA=V4  # version avec longmap explicite 
INC_TERA     = %devlp\#m1_d01>NCR>TERACONF>include>$(VER_TERA)
OBJ_TERA     = %devlp\#m1_d01>NCR>TERACONF>lib>$(VER_TERA)

######## structure d 'acces a TCPIP ( avec liens) 
#VER_TCP= V1.0 # should not change normally 
INC_TCP      = >PRJ>LIBGEN>TCP>$(VER_TCP)>INC
OBJ_TCP      = >PRJ>LIBGEN>TCP>$(VER_TCP)>OBJ

####### librairie SYBASE ( sybdef.h etc ..) 
# VER_SYB = PAS DE VERSION 
INC_SYB  =%devlp\#m1_d01>system>sybase>include
OBJ_SYB  =%devlp\#m1_d01>system>sybase>lib>libsybdb

################ LIBRAIRIES "CHRONO" #################

# VER_VOSV3FIC    = 1.3 # ex lib de Eric roissy 
INC_VOSV3FIC  = >PRJ>LIBGEN>FICHIER>$(VER_VOSV3FIC)>INC
OBJ_VOSV3FIC  = >PRJ>LIBGEN>FICHIER>$(VER_VOSV3FIC)>OBJ

##### Vos.c librairie d'acces fichiers VOS de Poupon
#VER_VOSFILE    = V1.4
INC_VOSFILE  = >PRJ>LIBGEN>VOSFILE>$(VER_VOSFILE)>INC
OBJ_VOSFILE  = >PRJ>LIBGEN>VOSFILE>$(VER_VOSFILE)>OBJ

##### LIB generale chrono ( mini fonc+ DateTime et strdatetime)
#VER_LIB   = V1.3
INC_LIB   = >PRJ>LIB>$(VER_LIB)>INC
OBJ_LIB   = >PRJ>LIB>$(VER_LIB)>OBJ

##### librairie SQL de Poupon ( TERA ET SYBASE) 
#VER_SQL      = V2.2
INC_SQL      = >PRJ>LIBGEN>SQL>$(VER_SQL)>INC
OBJ_SQL      = >PRJ>LIBGEN>SQL>$(VER_SQL)>OBJ

#########  INCLUDES PARTAGES ##################

##### include partage des LIBGEN ( appli.h , macdep uniflex beurk) 
#VER_INC_LIBGEN  = E_1.0
INC_LIBGEN       = >PRJ>LIBGEN>INC>$(VER_INC_LIBGEN)

##### include techniques : cc.h vos_call.h etc..
#VER_INC_GEN  = V1.3 #link 
INC_GEN       = >PRJ>INC>$(VER_INC_GEN)


##### utilitaires  (dev  sous OPENCLI) MAIS GENERAUX 
####  nosq (read,write queue) ,  msg_trace : service de trace journalieres
####  serv_help : dispatch de services par help , distrib_call !!
###   str_rand :genere divers string aleatoire 
#VER_OPC_BIN  = V2.0 #link 
INC_OPC_BIN       = >PRJ>OPENCLI>BIN>$(VER_OPC_BIN)>INC
OBJ_OPC_BIN       = >PRJ>OPENCLI>BIN>$(VER_OPC_BIN)>OBJ


### acces compress et expand et if_base_info_lt.h 1022
##  par Jerome JEAN-CHARLES le 110998 +1 BLOC 
#VER_IF_API  = V1.0 #link 
INC_IF_API       = >PRJ>LIBGEN>IF_API>$(VER_IF_API)>INC
OBJ_IF_API       = >PRJ>LIBGEN>IF_API>$(VER_IF_API)>OBJ


# eof acces.mak

