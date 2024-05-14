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
# script de rollback des proc_stoc
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
#   
#
#################################################################################

# ------------------------------------------------ 
# Variables et constantes
liv_ref=${PWD%/*/*}
liv_ref2=${PWD%/*/*/*}
nom_livraison="SAE_`basename $0 | awk -F"_SAE_" '{ print $2 }' |  awk -F"."  '{ print $1 }'`"
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
on_continue ()
{
  echo " Voulez-vous continuer (o/n) ?"
  read rep
  if [ "$rep" != "o" ]
  then
  	exit 0 ;
  fi
  rep=""
}

# ------------------------------------------------ 
usage ()
{
    echo ""
    echo -e "Service rendu de \033[1m `basename $0` \033[0m "
    echo " Fait un retour arriere par rapport aux procedures stockees livrees pour ce changement SAE (jira)."
    echo -e " Les procedures sauvegardees avant ce changement sont sous : \033[1m$path_sauvegarde_proc \033[0m"
    echo -e " Un fichier de log trace les operations effectuees :\033[1m $Fichier_log \033[0m "
    echo " Le traitement concerne la livraison ${nom_livraison}"    
    echo -e '\E[47;34m'"\033[1mAttention cette procedure peut entrainer des regressions si elle est executee aprés d'autres livraisons  \033[0m"
    echo " "

}
# =================================================== #
# main : Fichier .

# controle que le repertoire des sources a charger existe
if ! [ -e "${path_sauvegarde_proc}" ]
then
    echo_err "Le repertoire de sauvegarde n'existe pas. Vous devez etre sur PODFA"
fi   
# controle que l'operation est possible
cd $path_sauvegarde_proc
cpt=`ls ./*.${nom_livraison} 2>/dev/null | wc -l`
if [ "$cpt" = "0" ]
then
    cpt1=`ls ./*.${nom_livraison}.rollback 2>/dev/null | wc -l`
    if [ "$cpt1" = "0" ]
    then    
        echo_err "Il n'y a pas de retour arriere a faire"
    else
        echo_err "Retour arriere deja fait.\n Consultez le fichier de log pour plus de precision :  $Fichier_log"
    
    fi    
fi

# affichage info
usage

echo -e "\033[1mListe des procedures qui vont etre rechargees en base\033[0m"
ls -l ./*.${nom_livraison}

on_continue

# Traitement pour reprise livraison
# ----------------------------------------------------------------------
for i in *.${nom_livraison} ; 
do
    echo " Fichier proc : $i  "
    date_op=`date +'%d%m%Y_%H%M%S'`
    if [ -r ./$i  ] ; then   # Ceinture et bretelles
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
           
        echo "Chargement et compilation en base de procedure $i sur le site ${Site} en cours ..." 
        echo "$date_op (`basename $0`) isql -UMIGRAZUR -PMIGRAZUR  -SSQL_${Site}  -i ${i}  2>&1 " >> $Fichier_log
        isql -UMIGRAZUR -PMIGRAZUR  -SSQL_${Site}  -i ${i}  2>&1  | tee -a $Fichier_log
        mv ./$i ./$i.rollback
    fi
done;

exit 0                                                                                      
 

