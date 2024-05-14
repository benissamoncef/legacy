#!/bin/sh
#################################################################################
# PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#  Compile les procedures stockees de XZAC sur le serveur d'un site donne.
#
#############################################################################
#
# Service rendu
#################################################################################
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# AMA	03/09/20	: Creation
# RGR	14/10/20	: Ajout xzac900 et 901 Controleur de Feux
# RGR   26/11/20    : Ajout des procedures xzac manquantes
# ZCH   30/03/21    : Ajout des procedures xzac manquantes
# RGR   20/10/2021  : Correction pour compilation XZAC190
# JPC   15/03/2023  : Conversion XZAC;11 en XZAC11
# GGY   15/09/2023  : Adaptation pour CNA (DEM483)
# GGY   14/11/2023  : Ajout procedures PIC (DEM483)
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
includes="/users/gesconf/migrazur"
typeset -u Site_Compil=$1
Fichier_Compil="xzac${Site_Compil}"
cible=$2       # DEV ou numero de jira
Nom_Fic_Source="xzac"
fic_tmp_c_xzac="/tmp/make_${Nom_Fic_Source}.c"
fic_tmp_sql_xzac="/tmp/make_${Nom_Fic_Source}.sql"

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
       echo "select nomproc=\"$1\"" >> ${fic_tmp_c_xzac}   
       echo "go" >> ${fic_tmp_c_xzac}
   else
       echo_err "cette procedure $nom_procedure_reel  est degroupee elle ne peut etre compilee avec $0 "
   fi
   cat $1 >> ${fic_tmp_c_xzac}
 
}
# ------------------------------------------------ 
usage ()
{
    echo " "
    echo -e '\E[47;31m'"\033[1mla commande xzac_make.sh a ete  modifiee.\033[0m"
    echo " xzac_make.sh appelle  le script ${Nom_Prog} qui prend en compte les specificites de l'environement de developpement et celles de production."
    echo " " 
    echo " Usage    : $Nom_Prog  <site>  <cible>"
    echo " les parametres:" 
    echo " <site> : CI | CA | VC| DP| HIS"  
    echo " <cible> : DEV | <numero_jira>"  
    echo " <numero_jira> : corespond au numero de la jira de reference correspondant a la livraison"  
    echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE DEVELOPPEMENT \033[0m"
    echo " xzac_make.sh appelle  le script (${Nom_Prog} ) pour prendre en compte les specificites de l'environement de developpement.(directive DEV)"
    echo " La procedure presente en base cible est sauvegardee avant le chargement d'une nouvelle procedure sur la plateforme de dev."
    echo " Cette sauvegarde se trouve sous: $proc_ref "
    echo -e '\E[47;34m'"\033[1m Exemple: procedure pour le site CI plateforme de DEV  (via xzac_make.sh): $0 CI DEV \033[0m"

	echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE PRODUCTION \033[0m" 
    echo " la procedure stockee pour l'environement de production est compilee depuis l'espace de DEV."
    echo " le livrable d'une procedure stockee devient le fichier .sql  ( anciennement .i)"
    echo " ${Nom_Prog} genere le fichier .sql a livrer en production via le script sae_liv <num_jira> -p xzac_make.sh  <site> "
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

# CETTE DIRECTIVE N'EXISTE PLUS : $type" = "PC3"

# preparation des commandes sql 
# efface les procedures du module
echo "use PRC" >  ${fic_tmp_c_xzac}
echo "go" >> ${fic_tmp_c_xzac}
echo "drop procedure XZAC " >> ${fic_tmp_c_xzac} 

# C'est fait par sae_dev_make_prc

# if [[ "${Site_Compil}" != "CI" ]] && [[ "$Site_Compil" != "CA" ]]
# then
# # en attente de pouvoir faire le drop dans la procedure.

# echo " 
# use PRC
# go
# drop procedure XZAC90
# drop procedure XZAC91
# drop procedure XZAC93
# drop procedure XZAC98
# drop procedure XZAC99
# drop procedure XZAC200
# go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Site_Compil} 
# else
# echo " 
# use PRC
# go
# drop procedure XZAC90
# drop procedure XZAC91
# drop procedure XZAC93
# drop procedure XZAC98
# drop procedure XZAC99
# drop procedure XZAC200
# drop procedure XZAC190
# go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Site_Compil} 

# fi


echo "go" >> ${fic_tmp_c_xzac}

# integration des includes utilises
echo "#include \"$includes/XDMICG/inc/xdc.h\"" >> ${fic_tmp_c_xzac}    
echo "#include \"$includes/XDMICG/inc/xzaec.h\"" >> ${fic_tmp_c_xzac}  
echo "#include \"$includes/XDMICG/inc/xzacc.h\"" >> ${fic_tmp_c_xzac}  

# les groupees

cat_proc "xzac22sp.prc"
cat_proc "xzac23sp.prc"
cat_proc "xzac25sp.prc"
cat_proc "xzac26sp.prc"
cat_proc "xzac27sp.prc"

cat_proc "xzac30sp.prc"
cat_proc "xzac31sp.prc"
cat_proc "xzac32sp.prc"
cat_proc "xzac33sp.prc"
cat_proc "xzac34sp.prc"
cat_proc "xzac35sp.prc"
cat_proc "xzac36sp.prc"
cat_proc "xzac37sp.prc"

cat_proc "xzac40sp.prc"
cat_proc "xzac41sp.prc"
cat_proc "xzac42sp.prc"
cat_proc "xzac43sp.prc"
cat_proc "xzac44sp.prc"
cat_proc "xzac60sp.prc"
cat_proc "xzac68sp.prc"

# les degroupees

# Ajout des procedures PIC
sae_dev_make_prc "xzac3031sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac5501sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac8051sp.prc" $Site_Compil $cible

#Ajout des procedures IMU
sae_dev_make_prc "xzac890sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac891sp.prc" $Site_Compil $cible

#Ajout des procedures SONO
sae_dev_make_prc "xzac892sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac893sp.prc" $Site_Compil $cible

# procedures que sur secteur et CI et CA ( donc pas HIS)
if [[ "$Site_Compil" != "HIS" ]]
then
#Ajout des procedures IMU
sae_dev_make_prc "xzac903sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac904sp.prc" $Site_Compil $cible 
#Ajout des procedures SONO
sae_dev_make_prc "xzac905sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac906sp.prc" $Site_Compil $cible
fi


sae_dev_make_prc "xzac85sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac86sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac87sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac88sp.prc" $Site_Compil $cible


sae_dev_make_prc "xzac94sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac95sp.prc" $Site_Compil $cible


sae_dev_make_prc "xzac120sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac121sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac122sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac123sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac124sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac125sp.prc" $Site_Compil $cible


sae_dev_make_prc "xzac185sp.prc" $Site_Compil $cible



sae_dev_make_prc "xzac999sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzac1000sp.prc" $Site_Compil $cible

# procedures que sur CI, CA et HIS
if [[ "$Site_Compil" = "CI" ]]  || [[ "$Site_Compil" = "CA" ]]  || [[ "$Site_Compil" = "HIS" ]]
then
    sae_dev_make_prc "xzac45sp.prc" $Site_Compil $cible
fi

# procedures que sur secteur et CI et CA ( donc pas HIS)
if [[ "$Site_Compil" != "HIS" ]]
then

    sae_dev_make_prc "xzac21sp.prc" $Site_Compil $cible 
    sae_dev_make_prc "xzac74sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac82sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac83sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac84sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac90sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac91sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac92sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac93sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac96sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac98sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac99sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac126sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac682sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac840sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac303sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac550sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac900sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac901sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac902sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac1001sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac640sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac841sp.prc" $Site_Compil $cible
# les groupees
    cat_proc "xzac01sp.prc"
    cat_proc "xzac02sp.prc"
    cat_proc "xzac03sp.prc"
    cat_proc "xzac05sp.prc"
    cat_proc "xzac06sp.prc"
    cat_proc "xzac07sp.prc"
    cat_proc "xzac08sp.prc"
    
    cat_proc "xzac10sp.prc"
    cat_proc "xzac11sp.prc"
    cat_proc "xzac12sp.prc"
    cat_proc "xzac13sp.prc"
    cat_proc "xzac14sp.prc"
    cat_proc "xzac15sp.prc"
    cat_proc "xzac16sp.prc"
    cat_proc "xzac17sp.prc"
#    cat_proc "xzac18sp.prc"
#    cat_proc "xzac20sp.prc"
     
    cat_proc "xzac50sp.prc"
    cat_proc "xzac51sp.prc"
    cat_proc "xzac61sp.prc"
    cat_proc "xzac62sp.prc"
    cat_proc "xzac65sp.prc"
    cat_proc "xzac66sp.prc"
    cat_proc "xzac67sp.prc"
    # ex  exclu des PC3
    cat_proc "xzac52sp.prc"   
    cat_proc "xzac53sp.prc"
    cat_proc "xzac54sp.prc"
    cat_proc "xzac63sp.prc"
    cat_proc "xzac64sp.prc"
    cat_proc "xzac100sp.prc"
    cat_proc "xzac101sp.prc"
    cat_proc "xzac102sp.prc"
    cat_proc "xzac103sp.prc"
    cat_proc "xzac104sp.prc"
    cat_proc "xzac105sp.prc"
    cat_proc "xzac110sp.prc"
    cat_proc "xzac111sp.prc"
    cat_proc "xzac112sp.prc"
    cat_proc "xzac113sp.prc"
    cat_proc "xzac114sp.prc"
    cat_proc "xzac115sp.prc"

    
    sae_dev_make_prc "xzac09sp.prc" $Site_Compil $cible 
    sae_dev_make_prc "xzac18sp.prc" $Site_Compil $cible 
    sae_dev_make_prc "xzac20sp.prc" $Site_Compil $cible    
    sae_dev_make_prc "xzac73sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac75sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac76sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac77sp.prc" $Site_Compil $cible

   
    sae_dev_make_prc "xzac182sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac190sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac200sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac610sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac611sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac631sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac632sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac633sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac634sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac635sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac636sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac683sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac802sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac8021sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac805sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac11bsp.prc" $Site_Compil $cible

fi

# procedures que sur  CI ou CA
if [[ "$Site_Compil" = "CI" ]] || [[ "$Site_Compil" = "CA" ]]
then
    # celles qui etaient exclu des PC3
    sae_dev_make_prc "xzac19sp.prc" $Site_Compil $cible												
    
    sae_dev_make_prc "xzac70sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac71sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac72sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac78sp.prc" $Site_Compil $cible
    sae_dev_make_prc "xzac79sp.prc" $Site_Compil $cible
        
    sae_dev_make_prc "xzac80sp.prc" $Site_Compil $cible     # ajout SAGA
    sae_dev_make_prc "xzac81sp.prc" $Site_Compil $cible     # ajout SAGA
    
    sae_dev_make_prc "xzac636sp.prc" $Site_Compil $cible

fi


# precompil de la				 proc groupe
cpp -E -P -C  ${directive_compil_Plateforme}  ${fic_tmp_c_xzac}  > ${fic_tmp_sql_xzac}

chmod 775 ${fic_tmp_c_xzac}  ${fic_tmp_sql_xzac}  # on donne les droits au groupe

# phase pour la livraison 
# -----------------------

# Pour le DEV
if [ "$cible" = "DEV" ]  
then
    # cas de la plateforme de dev, on effectue  les memes operation qu'avant
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
    isql -UMIGRAZUR -PMIGRAZUR  -SSQL_${Site_Compil}  -i ${fic_tmp_sql_xzac}  2>&1  | tee -a $Fichier_Trace

    # Supprimer les fichiers intermediaires
     rm  $proc_ref/${Site_Compil}_${Nom_Fic_Source}.svg 
     
     
else 

    # Pour la prod
    # en prod c'est une copie dans le repertoire de transfert dedie
    cp -p ${fic_tmp_sql_xzac}  ${liv_ref_complet}/${Site_Compil}_${Nom_Fic_Source}.sql
    
   
fi

# fichier de log des operations 
echo "`date +'%d/%m/%Y %H:%M:%S'`$USER ${Nom_Prog} cible SAE_$cible : compilation sur ${Site_Compil} de ${Nom_Fic_Source}"  >> $Fichier_log




