# -------------------------------------------------------------
# -- Fichier permettant d'imprimer une fenetre 
#     
#     usage: tivue.x [nom_fichier]
# si le nom_fichier n'est pas renseigner alors on imprime la fenetre en noir&blanc
# sur l'imprimante par defaut, sinon nom_fichier est le fichier pcl genere
# version : $Date: 1995/05/31 12:06:50 $ $Id: tivue.x,v 1.2 1995/05/31 12:06:50 niepceron Exp $
# Niepceron : Modification
# Niepceron : remplacement de if [[ $# = 0 ]] par if [ $# = 0 ] pour applix v1.2
#
# --------------------------------------------------------------

 
FIC_XWD=/tmp/imp_ecran.xwd
FIC_SB=/tmp/imp_ecran.sb

#dump ecran
xwd > $FIC_XWD

#conversion xwd -> starbase
xwd2sb < $FIC_XWD > $FIC_SB

#si pas de parametre, fait l'impression directement en nb sur l'imp par defaut
if [ $# = 0 ]
then 
   pcltrans -e2 -r250 -w7.6 -l11 $FIC_SB | lp -oraw
else
  case `hostname` in
    PODFA | POGT | POCMA | POCDDN | POCDDM ) 
       pcltrans -C -P -d8. -h8.5 -a1 -T -q $FIC_SB > $1 ;;

    *) pcltrans -e2 -r250 -w7.6 -l11 $FIC_SB > $1 ;;
  esac
fi


# Destruction des fichiers temporaires
rm $FIC_XWD $FIC_SB

exit 0  	

