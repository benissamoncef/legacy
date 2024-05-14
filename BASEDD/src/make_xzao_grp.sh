/#!/bin/sh
#################################################################################
# PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#  Compile les procedures stock�es de XZAO sur le serveur d'un site donn�.
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
# ZCH   31/03/2021      : Ajout proc manquantes
# ZCH   08/02/2023  : fix compilation xzao1012 et xzao1014
# ZCH   14/02/2023  : inhiber la compilation de xzao20 sur HIS
# GGY   15/09/2023  : Adaptation pour CNA (DEM-483)
# JPC   21/11/2023  : Ajout de xzao1166
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
includes=$HOME
typeset -u Site_Compil=$1
Fichier_Compil="xzao${Site_Compil}"
cible=$2       # DEV ou numero de jira
Nom_Fic_Source="XZAO"
fic_tmp_c="/tmp/make_${Nom_Fic_Source}.c"
fic_tmp_sql="/tmp/make_${Nom_Fic_Source}.sql"
# repertoire des includes
includes=$HOME

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
    echo -e '\E[47;31m'"\033[1mla commande xzao_make.sh a ete  modifiee.\033[0m"
    echo " xzao_make.sh appelle  le script ${Nom_Prog} qui prend en compte les specificites de l'environement de developpement et celles de production."
    echo " " 
    echo " Usage    : $Nom_Prog  <site>  <cible>"
    echo " les parametres:" 
    echo " <site> : Escota : CI | VC | DP | HIS"
	echo "        : CNA    : CA | HIS"  
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
    echo " pour information, la procedure pr�sente en base cible de production est sauvegardee par le script de deployement en production."
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


echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" >>  ${fic_tmp_c}
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${fic_tmp_c}
echo "#include \"$includes/migrazur/XDMICG/inc/xzaac.h\"" >> ${fic_tmp_c}

cat_proc "xzao05sp.prc"                                                                       
cat_proc "xzao11sp.prc"                                                                       
cat_proc "xzao12sp.prc"                                                                       
cat_proc "xzao15sp.prc"                                                                       
cat_proc "xzao16sp.prc"                                                                       
cat_proc "xzao19sp.prc"                                                                                                                                              
cat_proc "xzao21sp.prc"                                                                       
cat_proc "xzao22sp.prc"                                                                       
cat_proc "xzao75sp.prc"                                                                                                                          
cat_proc "xzao77sp.prc"                                                                       
cat_proc "xzao82sp.prc"                                                                       
cat_proc "xzao116sp.prc"                                                                      
cat_proc "xzao117sp.prc"                                                                      
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
cat_proc "xzao74sp.prc"                                                                       
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
cat_proc "xzao1166sp.prc"
                                                            
if [[ "$Site_Compil" != "HIS" ]]                                                                 
then  
cat_proc "xzao304sp.prc" 
cat_proc "xzao20sp.prc"                                                           
fi     
                                                                                    
# precompil de la proc groupe
cpp -E -P -C  ${directive_compil_Plateforme}  ${fic_tmp_c}  > ${fic_tmp_sql}

chmod 775 ${fic_tmp_c}  ${fic_tmp_sql}  # on donne les droits au groupe

# phase pour la livraison 
# -----------------------

# Pour le DEV
if [ "$cible" = "DEV" ]  
then
    # cas de la plateforme de dev, on effectue  les m�mes op�ration qu'avant
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

# fichier de log des op�rations 
echo "`date +'%d/%m/%Y %H:%M:%S'`$USER ${Nom_Prog} cible SAE_$cible : compilation sur ${Site_Compil} de ${Nom_Fic_Source}"  >> $Fichier_log

# suppressions des fichiers temporaires
rm $fic_tmp_c $fic_tmp_sql
