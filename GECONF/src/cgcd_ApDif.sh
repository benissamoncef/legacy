#!/bin/ksh
# Fichier : $Id: cgcd_ApDif.sh,v 1.9 1995/10/03 11:20:10 gesconf Exp $        Release : $Revision: 1.9 $        Date : $Date: 1995/10/03 11:20:10 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME GECONF
##################################################################################
# MODULE MTRF # FICHIER cgcd_mtrf.sh
##################################################################################
# DESCRIPTION DU MODULE :
#	Effectue le transfert de tous les fichiers macros Applix d un repertoire donne vers un
#       autre repertoire d une machine distante.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# MERCIER	29 Jan 1995	: Creation
# MERCIER	?? fev 1995	: 
# MERCIER	06 Avr 1995	: Ajout du chmod 644 sur le repertoire de dep
# 	et dans le repertoire destinataire.				V 1.4
# MERCIER (GECONF) 8 Juin 1995	: Ajout du cd avant le chmod		V 1.5
# NIEPCERON     16 Juin 1995	: Suppression de la copie des fichiers  V 1.6
# NIEPCERON     28 Jun 1995	: Suppression du chmod ds rep dep 	V1.7
# NIEPCERON     29 Sep 1995	: Copie globale des fichiers		V1.8
##################################################################################
# la fonction ne renvoie pas de chaine si tout s est bien passe sinon
# elle renvoie l erreur qu a detecte le systeme.
 if [[ $# != 3 ]]
        then
        print "Le programme $0 permet de copier le contenu d un repertoire vers un autre repertoire d une machine distante"
        print "de la base sccs suivant: $(sccs_dir)"
        print " "
        print "syntaxe : $0 rep_source machine_distante rep_destination"
       
fi

cd $1        
vl_fichiers=`ls *.elo`
integer vl_compterendu=0
       
if [ "$vl_fichiers" = "" ]
then
       	print "Pas de fichiers .elo trouves dans ce repertoire..."
 	return 1
else
	remsh $2 -n "cd $3;chmod 644 *.elo" 2>&1

	#for vl_fichier_a_copier in $vl_fichiers
        #do
        #	vl_erreur=`rcp $1/$vl_fichier_a_copier $2:$3 2>&1`
		vl_erreur=`rcp $1/*.elo $2:$3 2>&1`
		vl_compterendu=vl_compterendu+$?
		if ((vl_compterendu >0)) 	
			then print $vl_erreur
		fi
        #done
fi

vl_fichiers=`ls *.as`
       
if [ "$vl_fichiers" = "" ]
then
       	print "Pas de fichiers .as trouves dans ce repertoire..."
 	
else
	remsh $2 -n "cd $3;chmod 644 *.as" 2>&1

	#for vl_fichier_a_copier in $vl_fichiers
        #do
                #vl_erreur=`rcp $1/$vl_fichier_a_copier $2:$3 2>&1`
		vl_erreur=`rcp $1/*.as $2:$3 2>&1`
		vl_compterendu=vl_compterendu+$?
		if ((vl_compterendu >0)) 	
			then print $vl_erreur
		fi
        #done
	
fi


vl_fichiers=`ls *.d`
       
if [ "$vl_fichiers" = "" ]
then
       	print "Pas de fichiers .elo trouves dans ce repertoire..."
 	return 1
else
	remsh $2 -n "cd $3;chmod 644 *.d" 2>&1
	#for vl_fichier_a_copier in $vl_fichiers
        #do
        	#vl_erreur=`rcp $1/$vl_fichier_a_copier $2:$3 2>&1`
		vl_erreur=`rcp $1/*.d $2:$3 2>&1`
		vl_compterendu=vl_compterendu+$?
		if ((vl_compterendu >0)) 	
			then print $vl_erreur
		fi
        #done
	return $vl_compterendu
fi
