/*E*/
/*Fichier : $Id: itre_son.c,v 1.7 2012/11/07 14:51:18 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2012/11/07 14:51:18 $
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
* FICHIER : itre_son.c				*
*************************************************
* DESCRIPTION :
* module generateur de sons pour les alertes;
* a partir de 

(c)Copyright 1992 Hewlett-Packard Co.,  All Rights Reserved.
 * player - command-line audio file player
 *   Aug. 28 1991
 *    by three unknown, unsung audio programmers
 *     (well, only two are unsung)


*************************************************
* HISTORIQUE :
* Niepceron 06 Jul 1995 : Creation
* Niepceron 07 Jul 1995 : Modification de exit(1) en return XDC_NOK
* Niepceron 07 Jul 1995 : Ajout de sons v1.3
* JMG	12/11/04	: on appelle un script (plus de link avec des librairies son 1.6
* PNI	02/10/12	: appelle du player sans passer par tscmd 1.7
*************************************************
*/

#include "itre_son.h"


static char *version = "$Id: itre_son.c,v 1.7 2012/11/07 14:51:18 pc2dpdy Exp $ : itre_son" ;

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  	Lance le type de son pass≈ en arg vers les enceintes externes
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 int itr_LanceSon( int   va_num_son )

/*
* ARGUMENTS EN ENTREE :
*   va_num_son : numero du type de son a lancer
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/

{
  char afile[100];
  char vl_cmd[150];


  switch( va_num_son )
    {
    case ITRE_SON_BASE : sprintf( afile, "%s/%s", XDC_PATH_INF, ITRE_FIC_SONBASE );
    	     break;
    case ITRE_SON_TADA : sprintf( afile, "%s/%s", XDC_PATH_INF, ITRE_FIC_SONTADA );
    	     break;
    case ITRE_SON_DRING : sprintf( afile, "%s/%s", XDC_PATH_INF, ITRE_FIC_SONDRING );
    	     break;
    case ITRE_SON_CARILLON : sprintf( afile, "%s/%s", XDC_PATH_INF, ITRE_FIC_SONCARILLON );
    	     break;
    case ITRE_SON_3DRINGS : sprintf( afile, "%s/%s", XDC_PATH_INF, ITRE_FIC_SON3DRINGS );
    	     break;
    default : sprintf( afile, "%s/%s", XDC_PATH_INF, ITRE_FIC_SONBASE );
    }

  if (XZSS_20PidTache("Aserver")==0)
    /*relance du Aserver*/
    system("tscmd.x \"/produits/migrazur/appliSD/exec/tiAserver.ksh\"");

  sprintf(vl_cmd,"tscmd.x \"/produits/migrazur/appliSD/exec/tiAserver_Play.ksh %s\"",afile);
  system(vl_cmd);
     
  sprintf(vl_cmd,"/produits/migrazur/appliSD/exec/tiAserver_Play.ksh %s",afile);
  system(vl_cmd);
  return XDC_OK;
}

