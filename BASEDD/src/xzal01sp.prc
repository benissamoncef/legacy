/*E*/
/*Fichier : $Id: xzal01sp.prc,v 1.6 2020/12/14 15:08:15 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2020/12/14 15:08:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAL * FICHIER xzal01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Appel >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	03 Nov 1994	: Creation
* Niepceron	11 Oct 1996	: lecture dans LIB_PRE (dem/1235) v1.2
* Niepceron     11 Oct 1996     : lecture dans LIB_PRE ordonnee (dem/1235) v1.3
* LCL		04/07/17	: Modif PRA
* JPL		27/06/18	: Syntaxe  1.5
* LCL / RGR 	14/12/20	: retrait test CI et retour KO si pas CI
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Retourne la liste des motifs d'appel
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAL;01
   as 

/*
*
* SP	XZAL01SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* XZALT_Motif	ListeMotifs
*
* VALEUR RENDUE :
*
* Retourne la liste des motifs d'appel
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* Poste Operateur CI
*
* FONCTION :
*
* select * from LIBELLE_PREDEFINIS
*
* MECANISMES :
*
------------------------------------------------------*/
declare	@vl_Site tinyint

      /*A
      ** Le site est celui du CI
      */

	select  @vl_Site = numero
	from CFG..RES_DIS
	where serveur = @@servername

      /* PRA
      if @@servername = XDC_SQLCI
      */
     /* if @vl_Site = XDC_CI
      begin*/
      
         /*A
         ** Recherche de la liste des motifs d'appel
         */
      
         select code,libelle from CFG..LIB_PRE where notyp=XDC_LIB_MOTIF_APPEL and
							valide=XDC_OUI
	order by position
      /* end */
      
      /*else*/
      
      /*A
      ** Le site n'est pas celui du CI
      ** return XDC_NOK
      */
      
       /*  return XDC_NOK */
      
      
      return ( XDC_OK )
         
go  
