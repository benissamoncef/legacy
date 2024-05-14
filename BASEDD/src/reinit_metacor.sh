#!/bin/ksh

###############################################################################
###############################################################################
## DESCRIPTION DU MODULE :
##
##   AprIs une interruption prolongEe de l'envoie de fichier EvEnements par les passerelles, due H une rupture des connexions rEseaux ou un arrAt d'un des serveurs de passerelle, la reprise du service doit Atre amorcEe par une rEinitialisation des EvEnements en mEmoire dans PX-Metacor Prr l'ensemble des EvEnements en cours, afin que PX-Metacor Prototype 2006 ai une connaissance de l'Etat actuel du rEseau.
##
##  Copie un fichier vide reinit_evnt_ESCOTA dans /produits/migrazur/appliSD/fichiers/stra/evtout_M
##  Met a zero le champ diffusion datex de la table EXP..FMC_HIS_DER pour rediffusion des evt ` metacor
###############################################################################
## HISTORIQUE :
## PNI 23/01/07 :  Creation
###############################################################################


if [[ $# != 1 ]]
then
        echo "syntaxe : reinit_metacor.sh <date> ou <date> est la date a partir de laquelle les evt doivent etre renvoyer (date de coupure)"
	exit 1
fi
exit

#Copie un fichier vide reinit_evnt_ESCOTA dans /produits/migrazur/appliSD/fichiers/stra/evtout_M

touch /produits/migrazur/appliSD/fichiers/stra/evtout_M/reinit_evnt_ESCOTA
touch /produits/migrazur/appliSD/fichiers/stra/evtout_M_ASF/reinit_evnt_ESCOTA
/produits/migrazur/appliSD/fichiers/stra/ftp/sit_script_put_M_ASF
/produits/migrazur/appliSD/fichiers/stra/ftp/sit_script_put_M

# Met a zero le champ diffusion datex de la table EXP..FMC_HIS_DER pour rediffusion des evt ` metacor

echo "update EXP..FMC_HIS_DER set diffusion_datex=0 where  horodate_validation>= '$1' \n\
go\n" \
| isql -SSQL_CI -Usa -Pzigramur 

