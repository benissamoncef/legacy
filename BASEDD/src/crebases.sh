#!/bin/ksh
# Fichier : $Id: crebases.sh,v 1.1 1994/09/15 17:54:26 torregrossa Exp $        Release : $Revision: 1.1 $        Date : $Date: 1994/09/15 17:54:26 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT crebases.sh
##################################################################################
# DESCRIPTION DU SCRIPT :
#
# Creation des bases pour un district ou le centre d'information
#
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.	09/08/94	: Creation
##################################################################################

# Creation des bases

if ((($#!=1)) || (test $1 != "CI" && test $1 != "DN" && test $1 != "DM"))
then    echo "Syntaxe : crebases.sh NomServeur"
	echo "          NomServeur : CI ou DN ou DM"
else
        # nom du serveur
	NomServeur=SQL_$1

	# base CFG pour CI ou PC
	crecfg.bas $NomServeur CFG cfg

	if (($?==1))
	then
		# base EXP pour CI ou PC
	        creexp.bas $NomServeur EXP exp
	fi

	if (($?==1))
	then
		# base PRC pour CI ou PC
	        creprc.bas $NomServeur PRC prc
	fi

	# bases pour CI
	if ((($?==1)) && (test $1 = "CI"))
	then
		# base HIS pour CI uniquement
		crehis.bas $NomServeur HIS his

		#base SVG pour CI uniquement
		if (($?==1))
		then    crehis.bas $NomServeur SVG svg
		fi

		# base CFT pour CI uniquement
		if (($?==1))
	        then    crecfg.bas $NomServeur CFT cft
		fi
	fi

	# compte-rendu du script
	if (($?==1))
	then echo "creation des tables realisees"
	else echo "erreur a la creation des tables"
	fi
fi
