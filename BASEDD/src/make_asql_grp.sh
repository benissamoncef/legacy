#!/bin/sh
#################################################################################
# PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#  Compile les procedures stockoes de ASQL sur le serveur d'un site donne.
#
#############################################################################
#
# Service rendu
#################################################################################
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# AMA	03/09/20	: Creation
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
liv_ref="/share_podf1_home/transfert_DEV/livraisons"

# repertoire des includes
includes=$HOME
typeset -u Site_Compil=$1
Fichier_Compil="asql${Site_Compil}"
cible=$2       # DEV ou numero de jira
Nom_Fic_Source="ASQL"
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
cat_proc_unitaire() {
    sae_dev_make_prc $1         $Site_Compil $cible
}

#TODO l'envoit des triggers est different
cat_proc_trigger() {
    echo sae_dev_make_trg $1         $Site_Compil $cible
   sae_dev_make_trg $1         $Site_Compil $cible
}
# ---
# ------------------------------------------------ 
usage ()
{
    echo " "
    echo -e '\E[47;31m'"\033[1mla commande asql_make.sh a ete  modifiee.\033[0m"
    echo " asql_make.sh appelle  le script ${Nom_Prog} qui prend en compte les specificites de l'environement de developpement et celles de production."
    echo " " 
    echo " Usage    : $Nom_Prog  <site>  <cible>"
    echo " les parametres:" 
    echo " <site> : CI | VC | DP | HIS | CA"  
    echo " <cible> : DEV| <numero_jira>"  
    echo " <numero_jira> : corespond au numero de la jira de reference correspondant a la livraison"  
    echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE DEVELOPPEMENT \033[0m"
    echo " asql_make.sh appelle  le script (${Nom_Prog} ) pour prendre en compte les specificites de l'environement de developpement.(directive DEV)"
    echo " La procedure presente en base cible est sauvegardee avant le chargement d'une nouvelle procedure sur la plateforme de dev."
    echo " Cette sauvegarde se trouve sous: $proc_ref "
    echo -e '\E[47;34m'"\033[1m Exemple: procedure pour le site CI plateforme de DEV  (via asql_make.sh): $0 CI DEV \033[0m"

	echo " "
    echo -e '\E[47;31m'"\033[1mPOUR ENVIRONEMENT DE PRODUCTION \033[0m" 
    echo " la procedure stockee pour l'environement de production est compilee depuis l'espace de DEV."
    echo " le livrable d'une procedure stockee devient le fichier .sql  ( anciennement .i)"
    echo " ${Nom_Prog} genere le fichier .sql a livrer en production via le script sae_liv <num_jira> -p asql_make.sh  <site> "
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
echo "drop procedure ASQL" >> ${fic_tmp_c}
echo "go" >> ${fic_tmp_c}

# ========================================================================================================================

echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" >> ${fic_tmp_c}
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${fic_tmp_c}                        
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${fic_tmp_c}                        

cat_proc_trigger "asql_t01_alm_eqt.trg"
cat_proc_trigger "asql_t04_eqt_gen.trg"
cat_proc_trigger "asql_t05_fmc_gen.trg"
cat_proc_trigger "asql_t12_fmc_trf.trg"
cat_proc_trigger "asql_t02_fmc_his.trg"
cat_proc_trigger "asql_t03_fmc_rev.trg"
cat_proc_trigger "asql_t06_fmc_acc.trg"
cat_proc_trigger "asql_t07_fmc_bas.trg"
cat_proc_trigger "asql_t08_fmc_cnc.trg"
cat_proc_trigger "asql_t09_fmc_del.trg"
cat_proc_trigger "asql_t10_fmc_ech.trg"
cat_proc_trigger "asql_t11_fmc_mnf.trg"
cat_proc_trigger "asql_t13_fmc_veh.trg"
cat_proc_trigger "asql_t14_act_gen.trg"
cat_proc_trigger "asql_t15_act_int.trg"
cat_proc_trigger "asql_t16_act_sis.trg"
cat_proc_trigger "asql_t17_act_dep.trg"
cat_proc_trigger "asql_t18_act_rmq.trg"
cat_proc_trigger "asql_t19_act_frg.trg"
cat_proc_trigger "asql_t20_act_gar.trg"
if [[ $1 != "CI" ]] && [[ $1 != "CA" ]]
then
cat_proc_trigger "asql_t22_fmc_trs.trg"
fi
if [[ $1 = "CI" ]] && [[ $1 != "CA" ]]
then
cat_proc_trigger "asql_t23_alt_exp.trg"
cat_proc_trigger "asql_t24_mes_niv.trg"
fi
if [[ $1 = "CI" ]] && [[ $1 != "CA" ]]
then
cat_proc_trigger "asql_c01_res_air.trg"
cat_proc_trigger "asql_c02_res_aut.trg"
cat_proc_trigger "asql_c03_res_cha.trg"
cat_proc_trigger "asql_c04_res_dis.trg"
cat_proc_trigger "asql_c05_res_ech.trg"
cat_proc_trigger "asql_c06_res_inf.trg"
cat_proc_trigger "asql_c07_res_itp.trg"
cat_proc_trigger "asql_c08_res_neu.trg"
cat_proc_trigger "asql_c09_res_pea.trg"
cat_proc_trigger "asql_c10_res_pnt.trg"
cat_proc_trigger "asql_c11_res_pol.trg"
cat_proc_trigger "asql_c12_res_por.trg"
cat_proc_trigger "asql_c13_res_ptl.trg"
cat_proc_trigger "asql_c14_res_seg.trg"
cat_proc_trigger "asql_c15_res_sup.trg"
cat_proc_trigger "asql_c16_res_trc.trg"
cat_proc_trigger "asql_c17_res_trn.trg"
cat_proc_trigger "asql_c18_res_vil.trg"
fi

# ========================================================================================================================

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
