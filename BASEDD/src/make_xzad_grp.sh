#!/bin/ksh
#################################################################################
# PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#  Compile les procedures stockees de xzad sur le serveur d'un site donne.
#
#############################################################################
#
# Service rendu
#################################################################################
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# RGR	04/11/20	: Creation
# GGY   15/09/2023  : Adaptation pour CNA (DEM-483)
##################################################################################

# ------------------------------------------------ 
# Variables et constantes
# ------------------------------------------------ 
proc_ref="/share_podf1_home/references_dev/proc_sybase"
Nom_Prog=`basename $0`
# Nom des fichiers temporaires utilises durant la compilation de la procedure
# Repertoire des fichiers d'en-tete communs inclus
Fichier_Trace=$Nom_Prog.err
Fichier_log=${proc_ref}/build_prc.log
directive_compil_Plateforme=""
option_directive_compil_Plateforme=""
liv_ref="/share_podf1_home/transfert_DEV/livraisons"

# repertoire des includes
includes=$HOME
typeset -u Site_Compil=$1
Fichier_Compil="xzad${Site_Compil}"
cible=$2       # DEV ou numero de jira
Nom_Fic_Source="xzad"
fic_tmp_c="/tmp/make_${Nom_Fic_Source}.c"
fic_tmp_sql="/tmp/make_${Nom_Fic_Source}.sql"

# ------------------------------------------------ 
# fonctions
# ------------------------------------------------ 
# affiche un message erreur en rouge
echo_err ()
{
	echo ""
	echo -e '\E[47;31m'"\033[1m(`basename $0`) Erreur: $1  \033[0m"
	exit 1;
}
# ------------------------------------------------ 
#  concatene les proc stockees groupees avec le nom de la proc affichee a chaque chargement
cat_proc ()
{
   # recuperation du nom de la procedure dans le fichier
   nom_procedure_reel=`cat $1| grep "create procedure" | awk  '{ print $3 }'`
   if ( grep -sq ";" <<< $nom_procedure_reel ) 
   then 
       echo "select nomproc=\"$1\"" >> ${fic_tmp_c}   
       echo "go" >> ${fic_tmp_c}
   else
       echo_err "cette procedure $nom_procedure_reel  est degroupee elle ne peut etre compilee avec $0 "
   fi
   cat $1 >> ${fic_tmp_c}
 
}
# ------------------------------------------------ 
usage ()
{
    echo " "
    echo -e '\E[47;31m'"\033[1mla commande xzad_make.sh a ete  modifiee.\033[0m"
    echo " xzad_make.sh appelle  le script ${Nom_Prog} qui prend en compte les specificites de l'environnement de developpement et celles de production."
    echo " " 
    echo " Usage    : $Nom_Prog  <site>  <cible>"
    echo " les parametres:" 
    echo " <site> : CI | VC | DP | HIS | CA"  
    echo " <cible> : DEV | <numero_jira>"  
    echo " <numero_jira> : corespond au numero de la jira de reference correspondant a la livraison"  
    echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE DEVELOPPEMENT \033[0m"
    echo " xzad_make.sh appelle  le script (${Nom_Prog} ) pour prendre en compte les specificites de l'environement de developpement.(directive DEV)"
    echo " La procedure presente en base cible est sauvegardee avant le chargement d'une nouvelle procedure sur la plateforme de dev."
    echo " Cette sauvegarde se trouve sous: $proc_ref "
    echo -e '\E[47;34m'"\033[1m Exemple: procedure pour le site CI plateforme de DEV  (via xzad_make.sh): $0 CI DEV \033[0m"

	echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE PRODUCTION \033[0m" 
    echo " la procedure stockee pour l'environement de production est compilee depuis l'espace de DEV."
    echo " le livrable d'une procedure stockee devient le fichier .sql  ( anciennement .i)"
    echo " ${Nom_Prog} genere le fichier .sql a livrer en production via le script sae_liv <num_jira> -p xzad_make.sh  <site> "
    echo " pour information, la procedure presente en base cible de production est sauvegardee par le script de deployement en production."
    echo -e '\E[47;34m'"\033[1m Exemple: procedure pour le site CI en production (via sae_liv) : $0 CI 113 \033[0m"
    echo " "

}

# ------------------------------------------------ 
# main
# ------------------------------------------------ 
# Verifier la presence et la validite des arguments obligatoires
if [[ $# -ne 2 ]]; then
	usage
	exit 1
fi

# Determiner les options de compilation Si DEV ou PROD
case $cible in
    "DEV" ) # cette directive permet de bouchonner les equipements
    directive_compil_Plateforme="-DDEV"
    ;;

    *)  # ces controles on deja ete fait en amont     
		if ! [[ $cible =~ ^[0-9]+$ ]]
		then
		   echo_err  "le parametre $cible doit corespondre au numero de la Jira qui porte la demande de modification (DEM)" 
 		fi
		liv_ref_complet="$liv_ref/SAE_$cible/proc"
    ;;
esac ;

# Determiner les options de compilation en fonction du serveur
# Directives de compilation selon le site
case $Site_Compil in
    "HIS" )
        directive_compil_Plateforme+=" -DHIST"
    ;;
    "CI" )
        directive_compil_Plateforme+=" -DCI"
    ;;
    "CA" )
        directive_compil_Plateforme+=" -DCA -DCI"
    ;;

    *)
    ;;
esac ;


# integration des includes utilises
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" >> ${fic_tmp_c}    
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${fic_tmp_c}  
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${fic_tmp_c}  

# Pour tous les sites
sae_dev_make_prc "xzad04sp.prc" $Site_Compil $cible

# procedures sur HIS
if [[ "$Site_Compil" = "HIS" ]]
then
  
    sae_dev_make_prc "xzad03sp.prc" $Site_Compil $cible 
    sae_dev_make_prc "xzad07sp.prc" $Site_Compil $cible    
    sae_dev_make_prc "xzad18sp.prc" $Site_Compil $cible
    
else

    sae_dev_make_prc "xzad01sp.prc" $Site_Compil $cible 
    sae_dev_make_prc "xzad02sp.prc" $Site_Compil $cible 
    sae_dev_make_prc "xzad05sp.prc" $Site_Compil $cible    
    sae_dev_make_prc "xzad06sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzad07sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzad08sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzad09sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzad10sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzad11sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzad17sp.prc" $Site_Compil $cible

fi
