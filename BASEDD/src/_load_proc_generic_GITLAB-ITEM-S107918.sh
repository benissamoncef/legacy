#!/bin/bash
#################################################################################
# PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME outils
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Gere la  livraison des procedures stockees et fait une sauvegarde pour  rollback
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
#
#################################################################################
#
# Service rendu
# #
# Sequence d'appel
#   load_proc_generic_SAE_<numero_issue_jira>.sh 
#
#
#.
#################################################################################

# ------------------------------------------------ 
# Variables et constantes
liv_ref=${PWD%/*/*}
liv_ref2=${PWD%/*/*/*}
patern_fichier=$1
nom_livraison="SAE_`basename $0 | awk -F"_SAE_" '{ print $2 }' |  awk -F"."  '{ print $1 }'`"
path_repertoire_livraison="$liv_ref/${nom_livraison}"
path_sauvegarde_proc="$liv_ref2/proc_dev_gitlab"
Fichier_log=${liv_ref2}/proc_dev_gitlab.log
# ------------------------------------------------ 
# fonctions

# ------------------------------------------------ 
echo_err ()
{
	usage
	echo ""
	echo -e '\E[47;31m'"\033[1m(`basename $0`) Erreur: $1  \033[0m"
	echo ""
	exit 1;
}

# ------------------------------------------------



# ------------------------------------------------ 
usage ()
{
    echo " "
    echo "Service rendu par `basename $0` <patern_fichier>  "
    echo " les parametres:" 
    echo " <patern_fichier> : l'ensemble des fichiers repondant au patern seront traites "  
    echo " "
    echo -e '\E[47;34m'"\033[1m Exemple1 chargement de toutes les procedures stockees livrees : `basename $0` all  \033[0m"
    echo -e '\E[47;34m'"\033[1m Exemple2 chargement des procedures stockees livrees pour le site DP : `basename $0` DP  \033[0m"
    echo -e '\E[47;34m'"\033[1m Exemple3 chargement des procedures stockees livrees qui comencent par xzae pour le site DP : `basename $0` DP_xzae  \033[0m"
    echo -e '\E[47;34m'"\033[1m Exemple4 chargement d'une procedures stockee livree   : `basename $0` CI_xzae49sp.prc  \033[0m"

    echo " "

}
# =================================================== #
# main : Fichier .

# gestion param d'entrée
# pas de param : liste les proc a livrer et affiche l'aide
if [ -z "$1" ]
then
    if [ -e "${path_repertoire_livraison}/proc" ]
    then
        echo "liste des procedures disponibles a livrer pour $nom_livraison "
        ls $path_repertoire_livraison/proc
    fi
    echo_err "Un patern est necessaire pour la selection des procedure(s) stockee(s) a livrer"
fi
  
# verif parametre
if [ $# -ne 1 ]
then
    echo "nombre de parametre incorecte :$*"
    usage
    exit ;
fi



# controle que le repertoire des sources a charger existe
if ! [ -e "${path_repertoire_livraison}" ]
then
    echo "Le repertoire de livraison n'existe pas rien a générer"
    exit ;
fi    

if [ "${patern_fichier}" = "all" ] 
then
    patern_fichier=""
fi

# Traitement pour load  => $fichier_deployer
# ----------------------------------------------------------------------
# Repertoire "proc" 
if [ -e "${path_repertoire_livraison}/proc" ]
then
  cd "${path_repertoire_livraison}/proc"
    for i in ${patern_fichier}* ; 
    do
        echo " Fichier proc : $i  "
        date_op=`date +'%d%m%Y_%H%M%S'`
        if [ -r ./$i  ] ; then 
            # recuperation du site dans le nom du .sql
            Site=`echo $i | awk -F"_" '{ print $1 }'`
            case $Site in
                "VC" | "DM" | "DP" | "DS" | "CI" | "HIS" )
                # c'est bon le fichier est bien forme
                ;;    
                
                *)
                echo_err "le nom du fichier la procedure ne comtient pas le site cible $i"
                ;;
            esac 
            
            # recuperation du nom de la procedure dans le fichier
            nom_procedure_reel=`cat $i | grep "create procedure" | awk  '{ print $3 }'`
            if ( grep -sq "!" <<< $nom_procedure_reel ) 
            then 
                echo_err "cette procedure est groupe elle ne peut etre compilee avec $0 "
            fi 
 
            # Sauvegarde  la proc en base
            fichier_sauvegarde="${path_sauvegarde_proc}/${i}.${nom_livraison}" # premiere livraison (la initiale) celle qui sera prise pour un rollback
            if [ -r "${fichier_sauvegarde}" ] ; then # les suivantes (utile si plusieurs liv en //)
                fichier_sauvegarde="${path_sauvegarde_proc}/${i}.${nom_livraison}.$date_op" 
            fi
            
            echo "Sauvegarde de la procedure ${nom_procedure_reel} en base ${Site} ..."
            echo "dans le fichier ${fichier_sauvegarde}"
            {
                echo "use PRC"
                echo "go"
                echo "sp_showtext $nom_procedure_reel "
#                echo "sp_helptext $nom_procedure_reel "
                echo "go"
            } | isql -Usa -Pzigramur -w2000 -SSQL_${Site}  -Jiso_1   >  ${fichier_sauvegarde}.tmp
            
            # on retire la derniere ligne du fichier car rajout sybase que je comprend pas
            sed '$d' -i ${fichier_sauvegarde}.tmp

            # on rajoute le code pour pouvoir recharcher la procedure si besoin
            echo "use PRC" >  ${fichier_sauvegarde}
            echo "go" >> ${fichier_sauvegarde}
            echo "if exists (select 1 from sysobjects where name = '${nom_procedure_reel}' and type = 'P')" >> ${fichier_sauvegarde}
            echo "  drop procedure ${nom_procedure_reel}" >> ${fichier_sauvegarde}
            echo "go" >> ${fichier_sauvegarde}
            cat "${fichier_sauvegarde}.tmp" >> ${fichier_sauvegarde}
            echo "go" >> ${fichier_sauvegarde}
            chmod 775 ${fichier_sauvegarde}   # on donne les droits au groupe
            rm ${fichier_sauvegarde}.tmp
            
            
            echo "Chargement et compilation en base de procedure $i sur le site ${Site} en cours ..." 
            echo "$date_op (`basename $0`) isql -UMIGRAZUR -PMIGRAZUR  -SSQL_${Site}  -i ${i}  2>&1 " >> $Fichier_log
            isql -UMIGRAZUR -PMIGRAZUR  -SSQL_${Site}  -i ${i}  2>&1  | tee -a $Fichier_log

        fi
    done;
else
    echo "pas de procedure stockees a livrer pour cette livraison"
fi

exit 0                                                                                      
 

