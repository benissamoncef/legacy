#!/bin/ksh
# Fichier : @(#)cgcd_mtrf.sh	1.4        Release : 1.4        Date : 09/29/95
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME GECONF
##################################################################################
# MODULE MTRF # FICHIER cgcd_mtrf.sh
##################################################################################
# DESCRIPTION DU MODULE :
#	Effectue le transfert de tous les fichiers d un repertoire donne vers un
#       autre repertoire d une machine distante.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# MERCIER	08 Dec 1994	: Creation
#NIEPCERON	23 Jun 1995	: Modif de rcp en rcp -r v1.2
#NIEPCERON	28 Jun 1995	: Modif si rep source diff de rep dest v1.3
#NIEPCERON      27 Jun 1996	: deprotege les fic avant copie, et protege si fmod  v1.4
##################################################################################
# la fonction renvoie 0 (via le print) si tout s est bien passe
# 2 s il manque des arguments
# 1 si le repertoire a copier est vide
 if [[ $# != 3 ]]
        then
        #print "Le programme $0 permet de copier le contenu d un repertoire vers un autre repertoire d une machine distante"
        #print "de la base sccs suivant: $(sccs_dir)"
        #print " "
        #print "syntaxe : $0 rep_source machine_distante rep_destination"
       	vl_cr=2
        print $vl_cr
        #return $vl_cr
fi
        
vl_fichiers=`ls $1`
integer vl_compterendu=0
       
if [ "$vl_fichiers" = "" ]
then
       	print "Pas de fichiers trouves dans ce repertoire..."
 	return 1
else
  if [[ "$3" = "/produits/migrazur/appliSD/exec" || "$2" = "PODFA"  || "$2" = "podfa" ]]
  then
	for vl_fichier_a_copier in $vl_fichiers
        do
                vl_erreur=`chmod +w $3/*`
                vl_compterendu=vl_compterendu+$?
                if ((vl_compterendu >0))
                        then print $vl_erreur
                fi

        	vl_erreur=`rcp $1/$vl_fichier_a_copier $2:$3 2>&1`
		vl_compterendu=vl_compterendu+$?
		if ((vl_compterendu >0)) 	
			then print $vl_erreur
		fi
		if [[ "$3" = "/produits/migrazur/appliSD/fichiers/mod" ]]
                then
                   vl_erreur=`chmod -w $3/*`
                   vl_compterendu=vl_compterendu+$?
                   if ((vl_compterendu >0))
                        then print $vl_erreur
                   fi
               fi
        done
	return $vl_compterendu
  else
       vl_erreur=`remsh $2 chmod +w $3/*`
       vl_compterendu=vl_compterendu+$?
       if ((vl_compterendu >0))
              then print $vl_erreur
       fi
       vl_erreur=`rcp -r $1 $2:$3/.. 2>&1`
       vl_compterendu=vl_compterendu+$?
       if ((vl_compterendu >0)) 
	   then print $vl_erreur
        fi
        if [[ "$3" = "/produits/migrazur/appliSD/fichiers/mod" ]]
        then
             vl_erreur=`remsh $2 chmod -w $3/*`
             vl_compterendu=vl_compterendu+$?
             if ((vl_compterendu >0))
                   then print $vl_erreur
             fi
        fi
	return $vl_compterendu
  fi  
fi
