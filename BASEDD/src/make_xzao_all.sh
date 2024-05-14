#!/bin/sh
#################################################################################
# PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#  Compile les procedures stockées de XZAO sur le serveur d'un site donné.
#
#############################################################################
#
# Service rendu
#################################################################################
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# AMA	03/09/20	: Creation
# LCL	27/01/21	: Ajout XZAO9999
# ZCH   31/03/2021  : Ajout proc manquantes
# ZCH   08/02/2023  : fix compilation xzao1012 et xzao1014
# ZCH   14/02/2023  : fix compil xzao20 sur HIS
# JPC	08/03/2023	: Conversion XZAO;22 en XZAO22 pour compatibilite CreationBouchon
# JPC   15/03/2023  : Ajout XZAO1240, XZAO508 et XZAO913. Degroupage XZA0;74 en XZAO74
# JPC   22/03/2023  : Ajout de XZAO892 
# GGY   15/09/2023  : Adaptation pour CNA (DEM483)
# GGY   14/11/2023  : Ajout procedures PIC (DEM483)
# JPL   24/01/2024  : Ajout de XZAO963
# JPL   20/03/2024  : XZAO963 sur HIS; Ajout XZAO964/965 (non utilisees, XZAS15 modifiee)
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
includes="/users/gesconf/migrazur/"
typeset -u Site_Compil=$1
Fichier_Compil="xzao${Site_Compil}"
cible=$2       # DEV ou numero de jira
Nom_Fic_Source="XZAO"
fic_tmp_c="/tmp/make_${Nom_Fic_Source}.c"
fic_tmp_sql="/tmp/make_${Nom_Fic_Source}.sql"
# repertoire des includes
includes="/users/gesconf/migrazur/"

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
cat_proc_unitaire() {
    sae_dev_make_prc $1         $Site_Compil $cible
}
# ------------------------------------------------ 
usage ()
{
    echo " "
    echo -e '\E[47;31m'"\033[1mla commande xzao_make.sh a ete  modifiee.\033[0m"
    echo " xzao_make.sh appelle  le script ${Nom_Prog} qui prend en compte les specificites de l'environement de developpement et celles de production."
    echo " " 
    echo " Usage    : $Nom_Prog  <site>  <cible>"
    echo " les parametres:" 
    echo " <site> : CI | CA | VC | DP | HIS"  
    echo " <cible> : DEV| <numero_jira>"  
    echo " <numero_jira> : corespond au numero de la jira de reference correspondant a la livraison"  
    echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE DEVELOPPEMENT \033[0m"
    echo " xzao_make.sh appelle  le script (${Nom_Prog} ) pour prendre en compte les specificites de l'environement de developpement.(directive DEV)"
    echo " La procedure presente en base cible est sauvegardee avant le chargement d'une nouvelle procedure sur la plateforme de dev."
    echo " Cette sauvegarde se trouve sous: $proc_ref "
    echo -e '\E[47;34m'"\033[1m Exemple: procedure pour le site CI plateforme de DEV  (via xzao_make.sh): $0 CI DEV \033[0m"

	echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE PRODUCTION \033[0m" 
    echo " la procedure stockee pour l'environement de production est compilee depuis l'espace de DEV."
    echo " le livrable d'une procedure stockee devient le fichier .sql  ( anciennement .i)"
    echo " ${Nom_Prog} genere le fichier .sql a livrer en production via le script sae_liv <num_jira> -p xzao_make.sh  <site> "
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
echo "drop procedure XZAO" >> ${fic_tmp_c}
echo "go" >> ${fic_tmp_c}


echo "#include \"$includes/XDMICG/inc/xdc.h\"" >>  ${fic_tmp_c}
echo "#include \"$includes/XDMICG/inc/xzaec.h\"" >> ${fic_tmp_c}
echo "#include \"$includes/XDMICG/inc/xzaac.h\"" >> ${fic_tmp_c}

# Ajout des procedures PIC
cat_proc_unitaire "xzao2571sp.prc"
cat_proc_unitaire "xzao8701sp.prc"

#ajout des procédures de l'imu
cat_proc_unitaire "xzao490sp.prc" 
cat_proc_unitaire "xzao491sp.prc" 
cat_proc_unitaire "xzao492sp.prc" 
cat_proc_unitaire "xzao493sp.prc" 
cat_proc_unitaire "xzao494sp.prc" 
cat_proc_unitaire "xzao495sp.prc" 
cat_proc_unitaire "xzao497sp.prc" 
cat_proc_unitaire "xzao498sp.prc" 
cat_proc_unitaire "xzao505sp.prc" 
#Ajout des procédures SONO
cat_proc_unitaire "xzao802sp.prc"


cat_proc_unitaire "xzao00sp.prc"
cat_proc "xzao05sp.prc"                                                                       
cat_proc "xzao11sp.prc"                                                                       
cat_proc "xzao12sp.prc"                                                                       
cat_proc "xzao15sp.prc"                                                                       
cat_proc "xzao16sp.prc"                                                                       
cat_proc "xzao19sp.prc"                                                                                                                                              
cat_proc "xzao21sp.prc"                                                                       
cat_proc_unitaire "xzao22bsp.prc"                                                                       
cat_proc "xzao22sp.prc"                                                                       
cat_proc "xzao75sp.prc"                                                                       
cat_proc_unitaire "xzao76sp.prc"                                                              
cat_proc "xzao77sp.prc"                                                                       
cat_proc_unitaire "xzao79sp.prc"                                                                       
cat_proc "xzao82sp.prc"                                                                       
cat_proc "xzao116sp.prc"                                                                      
cat_proc "xzao122sp.prc"                                                                      
cat_proc "xzao124sp.prc"                                                                      
cat_proc "xzao125sp.prc"                                                                      
cat_proc "xzao126sp.prc"                                                                      
cat_proc "xzao127sp.prc"                                                                      
cat_proc "xzao128sp.prc"                                                                      
cat_proc "xzao129sp.prc"                                                                      
cat_proc "xzao130sp.prc"                                                                      
cat_proc "xzao131sp.prc"                                                                      
cat_proc "xzao132sp.prc"                                                                      
cat_proc "xzao133sp.prc"                                                                      
cat_proc "xzao134sp.prc"                                                                      
cat_proc "xzao135sp.prc"                                                                      
cat_proc "xzao147sp.prc"                                                                      
cat_proc "xzao148sp.prc"                                                                      
cat_proc "xzao149sp.prc"                                                                      
cat_proc "xzao152sp.prc"                                                                      
cat_proc "xzao162sp.prc"                                                                      
cat_proc "xzao163sp.prc"                                                                      
cat_proc "xzao164sp.prc"                                                                      
cat_proc "xzao185sp.prc"                                                                      
cat_proc "xzao186sp.prc"                                                                      
cat_proc "xzao199sp.prc"                                                                      
cat_proc "xzao200sp.prc"                                                                      
cat_proc "xzao201sp.prc"                                                                      
cat_proc "xzao202sp.prc"                                                                      
cat_proc "xzao203sp.prc"                                                                      
cat_proc "xzao204sp.prc"                                                                      
cat_proc "xzao205sp.prc"                                                                      
cat_proc "xzao206sp.prc"                                                                      
cat_proc "xzao208sp.prc"                                                                      
cat_proc "xzao212sp.prc"                                                                      
cat_proc "xzao214sp.prc"                                                                      
cat_proc "xzao302sp.prc"                                                                      
cat_proc "xzao303sp.prc"
cat_proc_unitaire "xzao305sp.prc"
cat_proc "xzao307sp.prc"                                                                      
cat_proc "xzao308sp.prc"                                                                      
cat_proc "xzao309sp.prc"                                                                      
cat_proc "xzao317sp.prc"                                                                      
cat_proc "xzao320sp.prc"                                                                      
cat_proc "xzao321sp.prc"                                                                      
cat_proc "xzao322sp.prc"                                                                      
cat_proc "xzao410sp.prc"                                                                      
cat_proc "xzao411sp.prc"                                                                      
cat_proc "xzao415sp.prc"                                                                      
cat_proc "xzao418sp.prc"                                                                      
cat_proc "xzao422sp.prc"                                                                      
cat_proc_unitaire "xzao963sp.prc"                                                             
#cat_proc_unitaire "xzao964sp.prc"
#cat_proc_unitaire "xzao965sp.prc"
cat_proc_unitaire "xzao9999sp.prc"                                                             
cat_proc_unitaire "xzao02sp.prc"                                                              
cat_proc_unitaire "xzao01sp.prc"                                                              
cat_proc_unitaire "xzao10sp.prc"                                                                                       
cat_proc_unitaire "xzao17sp.prc"                                                              
cat_proc_unitaire "xzao18sp.prc"                                                              
cat_proc_unitaire "xzao30sp.prc"                                                              
cat_proc "xzao74sp.prc"                                                                       
cat_proc_unitaire "xzao78sp.prc"                                                                                      
cat_proc_unitaire "xzao115sp.prc"                                                             
cat_proc_unitaire "xzao123sp.prc"                                                             
cat_proc_unitaire "xzao136sp.prc"                                                             
cat_proc_unitaire "xzao501sp.prc" 
cat_proc_unitaire "xzao452sp.prc"                                                             
cat_proc_unitaire "xzao453sp.prc"                                                             
cat_proc_unitaire "xzao454sp.prc"                                                             
cat_proc_unitaire "xzao2031sp.prc" 
cat_proc_unitaire "xzao220sp.prc"                                                             
cat_proc_unitaire "xzao223sp.prc"                                                             
cat_proc_unitaire "xzao224sp.prc"                                                             
cat_proc_unitaire "xzao225sp.prc"                                                             
cat_proc_unitaire "xzao250sp.prc"                                                             
cat_proc_unitaire "xzao251sp.prc"                                                             
cat_proc_unitaire "xzao301sp.prc"                                                             
cat_proc_unitaire "xzao306sp.prc"                                                             
cat_proc_unitaire "xzao310sp.prc"                                                             
cat_proc_unitaire "xzao316sp.prc"                                                             
cat_proc_unitaire "xzao318sp.prc"                                                             
cat_proc_unitaire "xzao350sp.prc"                                                             
cat_proc_unitaire "xzao4001sp.prc"                                                            
cat_proc_unitaire "xzao401sp.prc"                                                             
cat_proc_unitaire "xzao402sp.prc"   
cat_proc_unitaire "xzao412sp.prc"                                                             
cat_proc_unitaire "xzao413sp.prc"                                                             
cat_proc_unitaire "xzao414sp.prc"                                                             
cat_proc_unitaire "xzao419sp.prc" 
cat_proc_unitaire "xzao423sp.prc"                                                             
cat_proc_unitaire "xzao424sp.prc"                                                             
cat_proc_unitaire "xzao425sp.prc"                                                             
cat_proc_unitaire "xzao426sp.prc"                                                             
cat_proc_unitaire "xzao427sp.prc"                                                             
cat_proc_unitaire "xzao428sp.prc"                                                             
cat_proc_unitaire "xzao429sp.prc"                                                             
cat_proc_unitaire "xzao430sp.prc"                                                             
cat_proc_unitaire "xzao431sp.prc"                                                             
cat_proc_unitaire "xzao432sp.prc" 
cat_proc_unitaire "xzao440sp.prc"                                                             
cat_proc_unitaire "xzao441sp.prc"                                                             
cat_proc_unitaire "xzao442sp.prc"                                                             
cat_proc_unitaire "xzao443sp.prc"                                                             
cat_proc_unitaire "xzao444sp.prc"                                                             
cat_proc_unitaire "xzao464sp.prc"                                                             
cat_proc_unitaire "xzao465sp.prc"                                                             
cat_proc_unitaire "xzao444sp.prc"                                                             
cat_proc_unitaire "xzao500sp.prc"                                                             
cat_proc_unitaire "xzao508sp.prc"
cat_proc_unitaire "xzao510sp.prc"                                                             
cat_proc_unitaire "xzao511sp.prc"                                                             
cat_proc_unitaire "xzao550sp.prc"                                                             
cat_proc_unitaire "xzao551sp.prc"                                                             
cat_proc_unitaire "xzao555sp.prc"                                                             
cat_proc_unitaire "xzao557sp.prc"                                                             
cat_proc_unitaire "xzao558sp.prc"                                                             
cat_proc_unitaire "xzao561sp.prc" 
cat_proc_unitaire "xzao563sp.prc" 
cat_proc_unitaire "xzao570sp.prc"                                                             
cat_proc_unitaire "xzao573sp.prc"                                                             
cat_proc_unitaire "xzao575sp.prc"                                                             
cat_proc_unitaire "xzao611sp.prc"                                                             
cat_proc_unitaire "xzao651sp.prc"                                                             
cat_proc_unitaire "xzao780sp.prc"                                                             
cat_proc_unitaire "xzao801sp.prc" 
cat_proc_unitaire "xzao807sp.prc" 
cat_proc_unitaire "xzao870sp.prc"                                                             
# Compiler les procédures de configuration du SAE                                             
cat_proc "xzao51sp.prc"                                                                       
cat_proc "xzao53sp.prc"                                                                       
cat_proc "xzao55sp.prc"                                                                       
cat_proc "xzao57sp.prc"                                                                       
cat_proc "xzao59sp.prc"                                                                       
cat_proc "xzao61sp.prc"                                                                       
cat_proc "xzao63sp.prc"                                                                       
cat_proc "xzao64sp.prc"                                                                       
cat_proc "xzao65sp.prc"                                                                       
cat_proc "xzao66sp.prc"                                                                       
cat_proc "xzao68sp.prc"                                                                       
cat_proc "xzao69sp.prc"                                                                       
cat_proc "xzao71sp.prc"                                                                       
cat_proc "xzao86sp.prc"                                                                       
cat_proc "xzao88sp.prc"                                                                       
cat_proc "xzao90sp.prc"                                                                       
cat_proc "xzao92sp.prc"                                                                       
cat_proc "xzao94sp.prc"                                                                       
cat_proc "xzao119sp.prc"                                                                      
cat_proc "xzao121sp.prc"                                                                      
cat_proc "xzao137sp.prc"                                                                      
cat_proc "xzao138sp.prc"                                                                      
cat_proc "xzao140sp.prc"                                                                      
cat_proc "xzao141sp.prc"                                                                      
cat_proc "xzao142sp.prc"                                                                      
cat_proc "xzao146sp.prc"                                                                      
cat_proc "xzao151sp.prc"                                                                      
cat_proc "xzao155sp.prc"                                                                      
cat_proc "xzao157sp.prc"                                                                      
cat_proc "xzao159sp.prc"                                                                      
cat_proc "xzao168sp.prc"                                                                      
cat_proc "xzao170sp.prc"                                                                      
cat_proc "xzao172sp.prc"                                                                      
cat_proc "xzao174sp.prc"                                                                      
cat_proc "xzao176sp.prc"                                                                      
cat_proc "xzao178sp.prc"                                                                      
cat_proc "xzao188sp.prc"                                                                      
cat_proc "xzao189sp.prc"                                                                      
cat_proc "xzao190sp.prc"                                                                      
cat_proc "xzao405sp.prc"                                                                      
cat_proc "xzao600sp.prc"                                                                      
cat_proc "xzao601sp.prc"                                                                      
cat_proc "xzao602sp.prc"                                                                      
cat_proc "xzao603sp.prc"                                                                      
cat_proc "xzao604sp.prc"                                                                      
cat_proc "xzao605sp.prc"                                                                      
cat_proc "xzao606sp.prc"                                                                      
cat_proc "xzao607sp.prc"                                                                      
cat_proc "xzao620sp.prc"                                                                      
cat_proc "xzao621sp.prc"                                                                      
cat_proc "xzao622sp.prc"                                                                      
cat_proc "xzao623sp.prc"                                                                      
cat_proc "xzao624sp.prc"                                                                      
cat_proc "xzao166sp.prc"                                                                      
cat_proc "xzao0166sp.prc"                                                                     
cat_proc "xzao978sp.prc"                                                                      
cat_proc "xzao980sp.prc"                                                                      
cat_proc "xzao994sp.prc"                                                                      
cat_proc "xzao996sp.prc"                                                                      
cat_proc "xzao998sp.prc"                                                                      
cat_proc "xzao1000sp.prc"                                                                     
cat_proc "xzao1002sp.prc"                                                                     
cat_proc "xzao1004sp.prc"                                                                     
cat_proc "xzao1006sp.prc"                                                                     
cat_proc "xzao1008sp.prc"                                                                     
cat_proc "xzao1010sp.prc"                                                                     
cat_proc "xzao982sp.prc"                                                                      
cat_proc "xzao984sp.prc"                                                                      
cat_proc "xzao986sp.prc"                                                                      
cat_proc "xzao988sp.prc"                                                                      
cat_proc "xzao990sp.prc"                                                                      
#cat_proc "xzao1012sp.prc"                                                                     
#cat_proc "xzao1014sp.prc"                                                                     
cat_proc_unitaire "xzao67sp.prc"                                                              
cat_proc_unitaire "xzao70sp.prc"                                                              
cat_proc_unitaire "xzao72sp.prc"                                                              
cat_proc_unitaire "xzao73sp.prc"                                                              
cat_proc_unitaire "xzao80sp.prc"                                                              
cat_proc_unitaire "xzao81sp.prc"                                                              
cat_proc_unitaire "xzao84sp.prc"                                                              
cat_proc_unitaire "xzao139sp.prc"                                                             
cat_proc_unitaire "xzao144sp.prc"                                                             
cat_proc_unitaire "xzao145sp.prc"                                                             
cat_proc_unitaire "xzao161sp.prc"                                                             
cat_proc_unitaire "xzao180sp.prc"                                                             
cat_proc_unitaire "xzao182sp.prc"                                                             
cat_proc_unitaire "xzao184sp.prc"                                                             
cat_proc_unitaire "xzao187sp.prc"                                                             
cat_proc_unitaire "xzao450sp.prc"
cat_proc_unitaire "xzao451sp.prc"
cat_proc_unitaire "xzao992sp.prc"
cat_proc_unitaire "xzao226sp.prc"
cat_proc_unitaire "xzao257sp.prc"
cat_proc_unitaire "xzao351sp.prc"
cat_proc_unitaire "xzao407sp.prc"
cat_proc_unitaire "xzao416sp.prc"
cat_proc_unitaire "xzao421sp.prc"
cat_proc_unitaire "xzao4290sp.prc"
cat_proc_unitaire "xzao455sp.prc"
cat_proc_unitaire "xzao456sp.prc"


                                                            
if [[ "$Site_Compil" != "HIS" ]]                                                                 
then  
cat_proc_unitaire "xzao960sp.prc"                                                             
cat_proc_unitaire "xzao961sp.prc"                                                             
cat_proc_unitaire "xzao962sp.prc"                                                             
cat_proc_unitaire "xzao556sp.prc"                                                             
cat_proc_unitaire "xzao574sp.prc"                                                             
cat_proc_unitaire "xzao576sp.prc"
cat_proc_unitaire "xzao5761sp.prc"                                                             
cat_proc_unitaire "xzao577sp.prc"
cat_proc_unitaire "xzao5771sp.prc"
cat_proc_unitaire "xzao910sp.prc"
cat_proc_unitaire "xzao911sp.prc"
cat_proc_unitaire "xzao912sp.prc"
cat_proc_unitaire "xzao143sp.prc"
cat_proc "xzao117sp.prc"                                                                      
cat_proc "xzao304sp.prc" 
cat_proc "xzao20sp.prc"
cat_proc_unitaire "xzao907sp.prc"
#nouvelles proc ajoutés
cat_proc_unitaire "xzao319sp.prc" 
cat_proc_unitaire "xzao259sp.prc"  
cat_proc_unitaire "xzao400sp.prc" 
cat_proc_unitaire "xzao445sp.prc" 
cat_proc_unitaire "xzao469sp.prc" 
cat_proc_unitaire "xzao470sp.prc" 
cat_proc_unitaire "xzao471sp.prc" 
cat_proc_unitaire "xzao781sp.prc" 
cat_proc_unitaire "xzao790sp.prc"                                                                                        
cat_proc_unitaire "xzao791sp.prc" 
cat_proc_unitaire "xzao819sp.prc" 
cat_proc_unitaire "xzao825sp.prc"
cat_proc_unitaire "xzao840sp.prc"
cat_proc_unitaire "xzao841sp.prc"
cat_proc_unitaire "xzao843sp.prc"
cat_proc_unitaire "xzao861sp.prc"
cat_proc_unitaire "xzao871sp.prc"

cat_proc_unitaire "xzao960sp.prc"
cat_proc_unitaire "xzao961sp.prc"
cat_proc_unitaire "xzao962sp.prc"
##
cat_proc_unitaire "xzao13sp.prc"
cat_proc_unitaire "xzao111sp.prc"  
cat_proc_unitaire "xzao150sp.prc"                                                              
cat_proc_unitaire "xzao209sp.prc"                                                             
cat_proc_unitaire "xzao210sp.prc"                                                             
cat_proc_unitaire "xzao231sp.prc" 
cat_proc_unitaire "xzao409sp.prc" 
cat_proc_unitaire "xzao420sp.prc" 
cat_proc_unitaire "xzao437sp.prc" 
cat_proc_unitaire "xzao811sp.prc"                                                            
cat_proc_unitaire "xzao1240sp.prc"
cat_proc_unitaire "xzao913sp.prc"
cat_proc_unitaire "xzao74bsp.prc"
fi     

                                                                                       
if [[ "$Site_Compil" = "CI" ]] || [[ "$Site_Compil" = "CA" ]]                                                                   
then      
#nouvelles proc#                                                                                   
cat_proc_unitaire "xzao215sp.prc"
cat_proc_unitaire "xzao1250sp.prc"
cat_proc_unitaire "xzao1270sp.prc"
cat_proc_unitaire "xzao1280sp.prc"
cat_proc_unitaire "xzao1480sp.prc"
cat_proc_unitaire "xzao1481sp.prc"
cat_proc_unitaire "xzao1490sp.prc"
cat_proc_unitaire "xzao232sp.prc"
cat_proc_unitaire "xzao233sp.prc"
cat_proc_unitaire "xzao467sp.prc"
cat_proc_unitaire "xzao590sp.prc"
cat_proc_unitaire "xzao591sp.prc"
cat_proc_unitaire "xzao592sp.prc"
cat_proc_unitaire "xzao816sp.prc"
cat_proc_unitaire "xzao820sp.prc"
cat_proc_unitaire "xzao826sp.prc"
cat_proc_unitaire "xzao830sp.prc"
cat_proc_unitaire "xzao85sp.prc"
cat_proc_unitaire "xzao89sp.prc"
cat_proc_unitaire "xzao953sp.prc"
cat_proc_unitaire "xzao153sp.prc"
cat_proc_unitaire "xzao258sp.prc"
cat_proc_unitaire "xzao571sp.prc"
cat_proc_unitaire "xzao579sp.prc"
cat_proc_unitaire "xzao580sp.prc"
cat_proc_unitaire "xzao824sp.prc"
cat_proc_unitaire "xzao900sp.prc"
cat_proc_unitaire "xzao265sp.prc"
cat_proc_unitaire "xzao468sp.prc"
cat_proc_unitaire "xzao460sp.prc"
cat_proc_unitaire "xzao433sp.prc"                                                             
cat_proc_unitaire "xzao434sp.prc"                                                             
cat_proc_unitaire "xzao435sp.prc"                                                             
cat_proc_unitaire "xzao4350sp.prc" 
cat_proc_unitaire "xzao562sp.prc"
cat_proc_unitaire "xzao565sp.prc"                                                             
cat_proc_unitaire "xzao566sp.prc" 
cat_proc_unitaire "xzao193sp.prc" 
cat_proc_unitaire "xzao262sp.prc"
cat_proc_unitaire "xzao403sp.prc"
cat_proc_unitaire "xzao461sp.prc"
cat_proc_unitaire "xzao466sp.prc"
cat_proc_unitaire "xzao552sp.prc"
cat_proc_unitaire "xzao553sp.prc"
cat_proc_unitaire "xzao559sp.prc"
cat_proc_unitaire "xzao564sp.prc"
cat_proc_unitaire "xzao578sp.prc"
cat_proc_unitaire "xzao650sp.prc"
cat_proc_unitaire "xzao720sp.prc"
cat_proc_unitaire "xzao730sp.prc"
cat_proc_unitaire "xzao1012sp.prc"
cat_proc_unitaire "xzao1014sp.prc"
cat_proc_unitaire "xzao892sp.prc"
fi            


if [[ "$Site_Compil" = "DP" ]]                                                                   
then         
cat_proc_unitaire "xzao153sp.prc"
cat_proc_unitaire "xzao258sp.prc"
cat_proc_unitaire "xzao571sp.prc"
cat_proc_unitaire "xzao579sp.prc"
cat_proc_unitaire "xzao580sp.prc"
cat_proc_unitaire "xzao824sp.prc"
cat_proc_unitaire "xzao900sp.prc"

fi


if [[ "$Site_Compil" = "VC" ]]                                                                   
then         
cat_proc_unitaire "xzao265sp.prc"
cat_proc_unitaire "xzao468sp.prc"
cat_proc_unitaire "xzao460sp.prc"
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
