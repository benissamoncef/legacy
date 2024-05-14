# -------------------------------------------------------------
# -- Fichier permettant d'imprimer une fenetre 
#     
#     usage: tivue.ksh [nom_fichier]
# si le nom_fichier n'est pas renseigner alors on imprime la fenetre en noir&blanc
# sur l'imprimante par defaut, sinon nom_fichier est le fichier pcl genere
# version : @(#)tivue.ksh	1.6 01/19/96
# Niepceron : Modification
# et remplacement de if [[ $# = 0 ]] par if [ $# = 0 ] pour applix v1.1
# Niepceron 01 Juin 1995 : coreection des parametre pcltrans v1.2
# Niepceron 24 Oct  1995 : ajout pocc v1.5
# Niepceron 19 Jan  1996 : ajout podr v1.6
# Niepceron 19 Jan  1996 : ajout d'un 2eme arg optionnel pour COULEUR ou NB v1.7
# Niepceron 15 Mar 1999  : Ajout PODEX 1.8
# Cluchague 15 Sep 2003  : Ajout POAT 1.9
# Hilmarcher 11/05/2004  : MAJ imprimante sur tout le parc v1.10
# Cluchague 07 Mai 2009  : Peyruis aussi doit pouvoir imprimer en couleur
# JMG	8 juin 2009 : DP devient VD 1.11
# ???	xx/xx/2009 : Sectorisation 1.12
# Niepceron 05/01/2010	: Sauvegarde du fichier de capture en .gif pour un export vers MANTIS v1.13
# JPL	22/09/2010	: Expression du format de date evitant sa modification par SCCS 1.14
# JPL	22/09/2010	: Impression sur PODFA2 comme sur les autres postes 1.15
# XXX	13/11/2017	: Mise a jour Linux 1.16
# --------------------------------------------------------------

 
FIC_XWD=/tmp/imp_ecran.xwd
FIC_SB=/tmp/imp_ecran.sb

OS=`uname`
#dump ecran et conversion xwd -> starbase
#xwd | xwd2sb > $FIC_SB
xwd > $FIC_XWD
if [ $OS = HP-UX ]
then
xwd2sb < $FIC_XWD > $FIC_SB
fi

format_date="%y%m%d.""%H""%M""%S"
ladate=`date +"$format_date"`


# suppression des precedent fichiers svg
rm -f /produits/migrazur/appliSD/ecrans/_impression_ecran*
#mv $FIC_XWD /produits/migrazur/appliSD/ecrans/_impression_ecran.$ladate.xwd
# Svg du fichier de capture et conversion en .gif
mv $FIC_XWD /tmp/_impression_ecran.xwd

if [ "$OS" = "HP-UX" ]
then
/opt/ImageMagick/bin/convert /tmp/_impression_ecran.xwd /produits/migrazur/appliSD/ecrans/_impression_ecran.$ladate.gif
else
  convert  /tmp/_impression_ecran.xwd /produits/migrazur/appliSD/ecrans/_impression_ecran.$ladate.gif
  convert  /tmp/_impression_ecran.xwd /tmp/_impression_ecran.tiff
fi


#si pas de parametre, fait l'impression directement en nb sur l'imp par defaut
if [ $# = 0 ]
then 
   if [ "$OS" = "HP-UX" ]
   then
     pcltrans -e2 -r250 -w7.6 -l11 $FIC_SB | lp -c -oraw
   else
     tiff2ps -h11 -w8.5  /tmp/_impression_ecran.tiff > $FIC_SB
     lp $FIC_SB
   fi
else
  if [ $# = 1 ]
  then
    type=`hostname`
  else
    type=$2
  fi
  case $type in
    POCI4 | POCA1 | POCA2 | POCA3 | POCDCA1 | POCDCA3 | PODEX | PODR | POGT | POAT | POCC | PODFA | PODFA2 | POVE1 | POVE2 | POPR1 | POPR2 | POVD1 | POVD2 | PODMA1 | PODMA2 | PODMA3 | POCI1 | POCI2 | POCI3 | COULEUR )
       if [ "$OS" = "HP-UX" ]
       then
         pcltrans -C -e2 -q -w7.6 -l11 $FIC_SB > $1 
       else
         xpr /tmp/_impression_ecran.xwd > $1 
     tiff2ps -h11 -w8.5  /tmp/_impression_ecran.tiff > $1
       fi
	;;

    *) if [ "$OS" = "HP-UX" ]
       then
         pcltrans -e2 -r250 -w7.6 -l11 $FIC_SB > $1 
       else
         xpr /tmp/_impression_ecran.xwd > $1 
     tiff2ps -h11 -w8.5  /tmp/_impression_ecran.tiff > $1
       fi
	;;
  esac
fi


# Destruction des fichiers temporaires
#rm $FIC_SB # $FIC_XWD

exit 0  	

