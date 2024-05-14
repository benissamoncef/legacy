#!/bin/ksh
#################################################################################
# PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#  Compile les procedures stockees de xzae sur le serveur d'un site donne.
#
#############################################################################
#
# Service rendu
#################################################################################
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# AMA	03/09/20	: 	Creation
# ZCH   29/03/21    : 	Modification(Ajout des procedures manquantes)
# JPC	15/03/23	: 	Ajout XZAE145
# GGY	31/05/23	:	Ajout des procs chapeau - Correction PA Consignation Bouchon (DEM-475)
# GGY   15/09/2023  : 	Adaptation pour CNA (DEM-483)
# ABK	02/10/23	:	Suppression de la création des procs XZAE204,XZAE152 et XZAE552
# GGY	18/01/24	:	Ajout XZAE861_Basculement_Tunnel (DEM-483)
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
liv_ref="/share_podf1_home/transfert_DEV/livraisons"

# repertoire des includes
includes="/users/gesconf/migrazur"
typeset -u Site_Compil=$1
Fichier_Compil="xzae${Site_Compil}"
cible=$2       # DEV ou numero de jira
Nom_Fic_Source="XZAE"
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
usage ()
{
    echo " "
    echo -e '\E[47;31m'"\033[1mla commande xzae_make.sh a ete  modifiee.\033[0m"
    echo " xzae_make.sh appelle  le script ${Nom_Prog} qui prend en compte les specificites de l'environement de developpement et celles de production."
    echo " " 
    echo " Usage    : $Nom_Prog  <site>  <cible>"
    echo " les parametres:" 
    echo " <site> : CI | CA | VC| DP| HIS"  
    echo " <cible> : DEV| <numero_jira>"  
    echo " <numero_jira> : corespond au numero de la jira de reference correspondant a la livraison"  
    echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE DEVELOPPEMENT \033[0m"
    echo " xzae_make.sh appelle  le script (${Nom_Prog} ) pour prendre en compte les specificites de l'environement de developpement.(directive DEV)"
    echo " La procedure presente en base cible est sauvegardee avant le chargement d'une nouvelle procedure sur la plateforme de dev."
    echo " Cette sauvegarde se trouve sous: $proc_ref "
    echo -e '\E[47;34m'"\033[1m Exemple: procedure pour le site CI plateforme de DEV  (via xzae_make.sh): $0 CI DEV \033[0m"

	echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE PRODUCTION \033[0m" 
    echo " la procedure stockee pour l'environement de production est compilee depuis l'espace de DEV."
    echo " le livrable d'une procedure stockee devient le fichier .sql  ( anciennement .i)"
    echo " ${Nom_Prog} genere le fichier .sql a livrer en production via le script sae_liv <num_jira> -p xzae_make.sh  <site> "
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


# Directives de compilation selon le site

# preparation des commandes sql 
# efface les procedures du module
echo "use PRC" >  ${fic_tmp_c}
echo "go" >> ${fic_tmp_c}
echo "drop procedure $Nom_Fic_Source" >> ${fic_tmp_c}
echo "go" >> ${fic_tmp_c}


echo "#include \"$includes/XDMICG/inc/xdc.h\"" >>  ${fic_tmp_c}
echo "#include \"$includes/XDMICG/inc/xzaec.h\"" >> ${fic_tmp_c}



sae_dev_make_prc "xzae11sp.prc"   $Site_Compil $cible
sae_dev_make_prc "xzae40sp.prc"   $Site_Compil $cible 
cat_proc "xzae43sp.prc"  
sae_dev_make_prc "xzae49sp.prc"   $Site_Compil $cible
cat_proc "xzae68sp.prc"   
cat_proc "xzae70sp.prc"   
cat_proc "xzae85sp.prc"  
sae_dev_make_prc "xzae88sp.prc"   $Site_Compil $cible
cat_proc "xzae91sp.prc" 
cat_proc "xzae92sp.prc" 
cat_proc "xzae93sp.prc" 
cat_proc "xzae94sp.prc" 
cat_proc "xzae102sp.prc" 
cat_proc "xzae105sp.prc" 
cat_proc "xzae106sp.prc" 
cat_proc "xzae107sp.prc" 
cat_proc "xzae108sp.prc" 
sae_dev_make_prc "xzae111sp.prc"   $Site_Compil $cible
sae_dev_make_prc "xzae80sp.prc"    $Site_Compil $cible
sae_dev_make_prc "xzae81sp.prc"    $Site_Compil $cible
sae_dev_make_prc "xzae82sp.prc"    $Site_Compil $cible
sae_dev_make_prc "xzae02sp.prc"    $Site_Compil $cible
sae_dev_make_prc "xzae83sp.prc"     $Site_Compil $cible
sae_dev_make_prc "xzae41sp.prc"     $Site_Compil $cible 
cat_proc "xzae44sp.prc"  
cat_proc "xzae45sp.prc"  
cat_proc "xzae46sp.prc"  
cat_proc "xzae47sp.prc"  
sae_dev_make_prc "xzae48sp.prc"    $Site_Compil $cible
cat_proc "xzae50sp.prc"  
cat_proc "xzae51sp.prc"  
cat_proc "xzae52sp.prc"  
cat_proc "xzae69sp.prc"  
sae_dev_make_prc "xzae71sp.prc"     $Site_Compil $cible
sae_dev_make_prc "xzae72sp.prc"     $Site_Compil $cible
sae_dev_make_prc "xzae95sp.prc"     $Site_Compil $cible
cat_proc "xzae97sp.prc"     
sae_dev_make_prc "xzae98sp.prc"     $Site_Compil $cible
cat_proc "xzae101sp.prc"   
sae_dev_make_prc "xzae01sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae03sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae66sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae82sp.prc"  $Site_Compil $cible
sae_dev_make_prc "xzae90sp.prc"  $Site_Compil $cible
sae_dev_make_prc "xzae203sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae154sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae205sp.prc" $Site_Compil $cible
#sae_dev_make_prc "xzae204sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae206sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae207sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae89sp.prc"  $Site_Compil $cible
sae_dev_make_prc "xzae76sp.prc"  $Site_Compil $cible
sae_dev_make_prc "xzae211sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae565sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae562sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae563sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae155sp.prc" $Site_Compil $cible
sae_dev_make_prc "xzae34sp.prc" $Site_Compil $cible


# procedures que sur secteur et CI et CA ( donc pas HIS)
if [[ "$Site_Compil" != "HIS" ]]
then
	
	sae_dev_make_prc "xzae158sp.prc"     $Site_Compil $cible
	sae_dev_make_prc "xzae159sp.prc"     $Site_Compil $cible
	sae_dev_make_prc "xzae181sp.prc"     $Site_Compil $cible

	# Procs Chapeau (DEM-475)
	sae_dev_make_prc "xzae008sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae011sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae015sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae060sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae0210sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae040sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae051sp.prc"  $Site_Compil $cible

	#Nouvelles procedures manquantes
        sae_dev_make_prc "xzae156sp.prc"  $Site_Compil $cible
        sae_dev_make_prc "xzae157sp.prc"  $Site_Compil $cible
        sae_dev_make_prc "xzae19sp.prc"  $Site_Compil $cible
        sae_dev_make_prc "xzae202sp.prc"  $Site_Compil $cible
        sae_dev_make_prc "xzae213sp.prc"  $Site_Compil $cible
        sae_dev_make_prc "xzae920sp.prc"  $Site_Compil $cible
        sae_dev_make_prc "xzae980sp.prc"  $Site_Compil $cible
        sae_dev_make_prc "xzae982sp.prc"  $Site_Compil $cible
        cat_proc "xzae42sp.prc" 
        sae_dev_make_prc "xzae200sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae560sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae564sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae566sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae567sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae21sp.prc"     $Site_Compil $cible
        ##				

        sae_dev_make_prc "xzae08sp.prc"  $Site_Compil $cible
	cat_proc "xzae10sp.prc"  
	cat_proc "xzae30sp.prc"  
	cat_proc "xzae16sp.prc"  
	sae_dev_make_prc "xzae13sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae14sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae61sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae64sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae87sp.prc"  $Site_Compil $cible
	cat_proc "xzae104sp.prc" 
	cat_proc "xzae109sp.prc" 
	sae_dev_make_prc "xzae110sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae112sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae67sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae18sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae15sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae17sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae53sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae54sp.prc"  $Site_Compil $cible
	cat_proc "xzae55sp.prc" 
	cat_proc "xzae56sp.prc"  
	cat_proc "xzae57sp.prc" 
	cat_proc "xzae58sp.prc" 
	cat_proc "xzae60sp.prc" 
	sae_dev_make_prc "xzae62sp.prc"  $Site_Compil $cible
	cat_proc "xzae63sp.prc"  
	sae_dev_make_prc "xzae65sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae86sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae96sp.prc"  $Site_Compil $cible
	sae_dev_make_prc "xzae147sp.prc" $Site_Compil $cible	
	sae_dev_make_prc "xzae148sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae209sp.prc" $Site_Compil $cible

	sae_dev_make_prc "xzae113sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae208sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae77sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae212sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae209sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae210sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae200sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae161sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae556sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae860sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae861sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae570sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae571sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae99sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae145sp.prc" $Site_Compil $cible
else  # procedures que sur HIS
        #nouvelle procedure
        sae_dev_make_prc "xzae300sp.prc" $Site_Compil $cible
        ##
fi

# procedures que sur  CI et CA
if [[ "$Site_Compil" = "CI" ]] || [[ "$Site_Compil" = "CA" ]]
then
        # Nouvelles procedures manquantes
        sae_dev_make_prc "xzae1720sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae195sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae196sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae197sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae198sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae199sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae910sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae930sp.prc" $Site_Compil $cible
        sae_dev_make_prc "xzae940sp.prc" $Site_Compil $cible
        ###

	sae_dev_make_prc "xzae75sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae160sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae162sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae163sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae164sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae165sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae166sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae167sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae168sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae170sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae171sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae172sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae175sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae178sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae185sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae201sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae220sp.prc" $Site_Compil $cible
	
	#proc STRADA
	sae_dev_make_prc "xzae120sp.prc" $Site_Compil $cible
	cat_proc "xzae121sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae122sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae123sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae124sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae125sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae126sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae127sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae128sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae129sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae130sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae131sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae132sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae133sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae134sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae135sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae136sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae137sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae138sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae139sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae140sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae141sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae142sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae143sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae144sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae146sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae149sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae150sp.prc" $Site_Compil $cible
	#sae_dev_make_prc "xzae152sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae153sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae550sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae551sp.prc" $Site_Compil $cible
	#sae_dev_make_prc "xzae552sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae553sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae554sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae555sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae641sp.prc" $Site_Compil $cible
	sae_dev_make_prc "xzae1270sp.prc" $Site_Compil $cible

fi



if [[ "$Site_Compil" = "DP" ]]
then
       sae_dev_make_prc "xzae568sp.prc" $Site_Compil $cible
       

fi


if [[ "$Site_Compil" = "VC" ]]
then
       
       sae_dev_make_prc "xzae199sp.prc" $Site_Compil $cible

fi

if [[ "$Site_Compil" = "HIS" ]]
then
sae_dev_make_prc "xzae000sp.prc" $Site_Compil $cible
fi


#
#  traitement de la procedure groupe : precompil 
#

# precompil de la proc groupe
cpp -E -P -C  ${directive_compil_Plateforme}  ${fic_tmp_c}  > ${fic_tmp_sql}

chmod 775 ${fic_tmp_c}  ${fic_tmp_sql}  # on donne les droits au groupe

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
    isql -UMIGRAZUR -PMIGRAZUR  -SSQL_${Site_Compil}  -i ${fic_tmp_sql}  2>&1  | tee -a $Fichier_Trace

    # Supprimer les fichiers intermediaires
     rm  $proc_ref/${Site_Compil}_${Nom_Fic_Source}.svg 
     
else 

    # Pour la prod
    # en prod c'est une copie dans le repertoire de transfert dedie
    cp -p ${fic_tmp_sql}  ${liv_ref_complet}/${Site_Compil}_${Nom_Fic_Source}.sql
   
fi

# fichier de log des operations 
echo "`date +'%d/%m/%Y %H:%M:%S'`$USER ${Nom_Prog} cible SAE_$cible : compilation sur ${Site_Compil} de ${Nom_Fic_Source}"  >> $Fichier_log

# suppressions des fichiers temporaires
rm $fic_tmp_c $fic_tmp_sql
