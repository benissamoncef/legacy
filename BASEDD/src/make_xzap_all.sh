#!/bin/sh
#################################################################################
# PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#  Compile les procedures stockées de XZAP sur le serveur d'un site donné.
#
#############################################################################
#
# Service rendu
#################################################################################
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# AMA	03/09/20	: Creation
# ZCH   29/03/2021  : Modification
# ZCH   08/02/2023  : fix compilation xzap85,86 et 36 
# JPC   15/03/2023  : Ajout de XZAP35
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
# On purge le fichier 
rm $Nom_Prog.err

Fichier_log=${proc_ref}/build_prc.log
directive_compil_Plateforme=""
liv_ref="/share_podf1_home/transfert_DEV/livraisons"

# repertoire des includes
includes="/users/gesconf/migrazur"
typeset -u Site_Compil=$1
Fichier_Compil="xzap${Site_Compil}"
cible=$2       # DEV ou numero de jira
Nom_Fic_Source="XZAP"
fic_tmp_c="/tmp/make_${Nom_Fic_Source}.c"
fic_tmp_sql="/tmp/make_${Nom_Fic_Source}.sql"
# repertoire des includes
includes="/users/gesconf/migrazur"

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
	# permet d'afficher le nom pendant la compil
	echo "select nomproc=\"$1\"" >> ${fic_tmp_c}
	echo "go" >> ${fic_tmp_c}
   else
       echo_err "cette procedure $nom_procedure_reel  est degroupee elle ne peut etre compilee avec $0 "
   fi
   cat $1 >> ${fic_tmp_c}

}

# ------------------------------------------------ 
#  genere  les proc stockees DEgroupees de facon unitaire

cat_proc_unitaire() {
    sae_dev_make_prc $1         $Site_Compil $cible |  tee -a  $Fichier_Trace
}

# ------------------------------------------------ 
usage ()
{
    echo " "
    echo -e '\E[47;31m'"\033[1mla commande xzap_make.sh a ete  modifiee.\033[0m"
    echo " xzap_make.sh appelle  le script ${Nom_Prog} qui prend en compte les specificites de l'environement de developpement et celles de production."
    echo " " 
    echo " Usage    : $Nom_Prog  <site>  <cible>"
    echo " les parametres:" 
    echo " <site> : CI | CA | VC| DP| HIS"  
    echo " <cible> : DEV| <numero_jira>"  
    echo " <numero_jira> : corespond au numero de la jira de reference correspondant a la livraison"  
    echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE DEVELOPPEMENT \033[0m"
    echo " xzap_make.sh appelle  le script (${Nom_Prog} ) pour prendre en compte les specificites de l'environement de developpement.(directive DEV)"
    echo " La procedure presente en base cible est sauvegardee avant le chargement d'une nouvelle procedure sur la plateforme de dev."
    echo " Cette sauvegarde se trouve sous: $proc_ref "
    echo -e '\E[47;34m'"\033[1m Exemple: procedure pour le site CI plateforme de DEV  (via xzap_make.sh): $0 CI DEV \033[0m"

	echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE PRODUCTION \033[0m" 
    echo " la procedure stockee pour l'environement de production est compilee depuis l'espace de DEV."
    echo " le livrable d'une procedure stockee devient le fichier .sql  ( anciennement .i)"
    echo " ${Nom_Prog} genere le fichier .sql a livrer en production via le script sae_liv <num_jira> -p xzap_make.sh  <site> "
    echo " pour information, la procedure présente en base cible de production est sauvegardee par le script de deployement en production."
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


# Directives de compilation selon le site

# preparation des commandes sql 
# efface les procedures du module
echo "use PRC" >  ${fic_tmp_c}
echo "go" >> ${fic_tmp_c}
echo "drop procedure XZAP" >> ${fic_tmp_c}
echo "go" >> ${fic_tmp_c}


echo "#include \"$includes/XDMICG/inc/xdc.h\""      >> ${fic_tmp_c}
echo "#include \"$includes/BASEDD/inc/xzap11sp.h\"" >> ${fic_tmp_c}
echo "#include \"$includes/BASEDD/inc/xzap12sp.h\"" >> ${fic_tmp_c}
echo "#include \"$includes/BASEDD/inc/xzap13sp.h\"" >> ${fic_tmp_c}
echo "#include \"$includes/BASEDD/inc/xzap14sp.h\"" >> ${fic_tmp_c}
echo "#include \"$includes/BASEDD/inc/xzap15sp.h\"" >> ${fic_tmp_c}
echo "#include \"$includes/BASEDD/inc/xzap16sp.h\"" >> ${fic_tmp_c}
echo "#include \"$includes/BASEDD/inc/xzap17sp.h\"" >> ${fic_tmp_c}
echo "#include \"$includes/BASEDD/inc/xzap18sp.h\"" >> ${fic_tmp_c}
echo "#include \"$includes/XDMICG/inc/xzaec.h\""    >> ${fic_tmp_c}

#Ajout des procédures IMU
cat_proc_unitaire "xzap85sp.prc" 
cat_proc_unitaire "xzap36sp.prc"
#Ajout des procédures SONO
cat_proc_unitaire "xzap86sp.prc"


cat_proc "xzap01sp.prc"
cat_proc "xzap03sp.prc"
cat_proc "xzap04sp.prc"
cat_proc "xzap05sp.prc"
cat_proc "xzap06sp.prc"
cat_proc_unitaire "xzap07sp.prc"
cat_proc_unitaire "xzap08sp.prc"
cat_proc_unitaire "xzap09sp.prc"
cat_proc "xzap10sp.prc"
cat_proc "xzap12sp.prc"
cat_proc "xzap13sp.prc"
cat_proc "xzap14sp.prc"
cat_proc "xzap15sp.prc"
cat_proc "xzap16sp.prc"
cat_proc_unitaire "xzap17sp.prc"
cat_proc_unitaire "xzap18sp.prc"
cat_proc "xzap21sp.prc"
cat_proc "xzap22sp.prc"
cat_proc_unitaire "xzap27sp.prc"
cat_proc_unitaire "xzap130sp.prc"
cat_proc_unitaire "xzap28sp.prc"
cat_proc_unitaire "xzap29sp.prc"
cat_proc_unitaire "xzap30sp.prc"
cat_proc_unitaire "xzap31sp.prc"
cat_proc_unitaire "xzap32sp.prc"
cat_proc_unitaire "xzap33sp.prc"
cat_proc_unitaire "xzap35sp.prc"
cat_proc_unitaire "xzap36sp.prc"
# procedures manquante rajoutee pour MOVIS
cat_proc_unitaire "xzap131sp.prc"
cat_proc_unitaire "xzap180sp.prc"
cat_proc_unitaire "xzap34sp.prc"
# cat_proc_unitaire "xzap41sp.prc"
# cat_proc_unitaire "xzap42sp.prc"
# cat_proc_unitaire "xzap43sp.prc"
# cat_proc_unitaire "xzap44sp.prc"
# cat_proc_unitaire "xzap61sp.prc"
cat_proc_unitaire "xzap64sp.prc"
cat_proc_unitaire "xzap70sp.prc"


if [[ "${Site_Compil}" != "HIS" ]]
then	
cat_proc_unitaire "xzap02sp.prc"
cat_proc_unitaire "xzap11sp.prc"
cat_proc_unitaire "xzap50sp.prc"
cat_proc_unitaire "xzap63sp.prc"
cat_proc_unitaire "xzap83sp.prc"																		
cat_proc_unitaire "xzap26sp.prc"
else 
cat_proc "xzap24sp.prc"
cat_proc "xzap25sp.prc"
fi 


# procedures que sur CI ou CA
if [[ "$Site_Compil" = "CI" ]] || [[ "$Site_Compil" = "CA" ]]
then
    cat_proc "xzap23sp.prc"
    cat_proc "xzap24sp.prc"
    cat_proc "xzap25sp.prc"
    cat_proc_unitaire "xzap82sp.prc"
fi 



# precompil de la proc groupe
cpp -E -P -C  ${directive_compil_Plateforme}  ${fic_tmp_c}  > ${fic_tmp_sql}

chmod 775 ${fic_tmp_c}  ${fic_tmp_sql}  # on donne les droits au groupe

# phase pour la livraison 
# -----------------------

# Pour le DEV
if [ "$cible" = "DEV" ]  
then
    # cas de la plateforme de dev, on effectue  les mêmes opération qu'avant
    # avec en plus sauvegarde la proc courrante en base
    date_backup=`date +'%d%m%Y_%H%M%S'`
    fichier_sauvegarde="$proc_ref/${Site_Compil}_${Nom_Fic_Source}.${date_backup}"
    echo "Sauvegarde de la procedure ${nom_procedure_reel} en base ${Site_Compil} ..."
    echo "dans le fichier ${fichier_sauvegarde}"
    {
        echo "use PRC"
        echo "go"
        echo "sp_showtext $Nom_Fic_Source "
        echo "go"
        } | isql -Usa -Pzigramur -w2000 -SSQL_${Site_Compil}  -Jiso_1   >  $proc_ref/${Site_Compil}_${Nom_Fic_Source}.svg


    echo "use PRC" >  ${fichier_sauvegarde}
    echo "go" >> ${fichier_sauvegarde}
    echo "if exists (select 1 from sysobjects where name = '${Nom_Fic_Source}' and type = 'P')" >> ${fichier_sauvegarde}
    echo "  drop procedure ${Nom_Fic_Source}" >> ${fichier_sauvegarde}
    echo "go" >> ${fichier_sauvegarde}
    cat "$proc_ref/${Site_Compil}_${Nom_Fic_Source}.svg" >> ${fichier_sauvegarde}
    echo "go" >> ${fichier_sauvegarde}
    chmod 775 ${fichier_sauvegarde}   # on donne les droits au groupe
    
    echo "Compilation de procedure sur le site ${Site_Compil} en cours ..."
    isql -UMIGRAZUR -PMIGRAZUR  -SSQL_${Site_Compil}  -i ${fic_tmp_sql}  2>&1  | tee -a $Fichier_Trace

    # Supprimer les fichiers intermediaires
     rm  $proc_ref/${Site_Compil}_${Nom_Fic_Source}.svg 
     
     
else 

    # Pour la prod
    # en prod c'est une copie dans le repertoire de transfert dedie
    cp -p ${fic_tmp_sql}  ${liv_ref_complet}/${Site_Compil}_${Nom_Fic_Source}.sql
    
   
fi

# fichier de log des opérations 
echo "`date +'%d/%m/%Y %H:%M:%S'`$USER ${Nom_Prog} cible SAE_$cible : compilation sur ${Site_Compil} de ${Nom_Fic_Source}"  >> $Fichier_log

# suppressions des fichiers temporaires
rm $fic_tmp_c $fic_tmp_sql
