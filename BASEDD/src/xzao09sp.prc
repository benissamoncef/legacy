/*E*/
/*  Fichier : $Id: xzao09sp.prc,v 1.7 2018/10/11 15:51:38 devgfi Exp $      Release : $Revision: 1.7 $        Date : $Date: 2018/10/11 15:51:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao09sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO09SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute		Autoroute_In
* XDY_PR		PR1_In
* XDY_PR		PR2_In
* XDY_Sens		Sens_In
* XDY_District		District_In
*
* PARAMETRES EN SORTIE :
*
* XDY_Nom		ListeTunnels	
*
* VALEUR RENDUE :
*
* Recherche les tunnels (physiques) entre 2 PR et/ou
* dans un district.
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Select dans les tables RES_POR (PORTION) et RES_SEG 
* (SEGMENT_HOMOGENE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	24 Aug 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao09sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzao09sp.prc; */

/* declaration de fonctions internes */

/* definition de fonctions externes */
 
use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO09' and type = 'P')
	drop procedure XZAO09
go


create procedure XZAO09
   @Autoroute_In	tinyint,
   @PR1_In		int,
   @PR2_In		int,
   @Sens_In		tinyint,
   @District_In		smallint
   as
   
   /* Definition des variables locales */
   
   /* Test de validite des parametres d'entree */

   if @Autoroute_In = null or @Autoroute_In < 0 or ((@PR1_In = null or @PR2_In = null) and @District_In = null)
      return XDC_ARG_INV


      /* Verifier l'existence de l'autoroute */
      if not exists (select 1 from CFG..RES_AUT where numero = @Autoroute_In)
         return XDC_NOK


         /* Recherche les tunnels dans 1 District                                 */
         
         if (@PR1_In = null or @PR2_In = null) /* --Recherche les tunnels dans 1 District */
            begin
            select RS.nom_tunnel
               from CFG..RES_POR RP, CFG..RES_SEG RS
               where RP.autoroute = @Autoroute_In
                 and RP.district  = @District_In
                 and RS.portion   = RP.numero
                 and RS.sens	  = @Sens_In
                 and RS.tunnel    = XDC_OUI
            end
         else
            
            /* Recherche les tunnels entre 2 PR                           */
            
            if @District_In = null --Recherche les tunnels entre 2 PR
               begin
               select RS.nom_tunnel
                  from CFG..RES_POR RP, CFG..RES_SEG RS
                  where RP.autoroute = @Autoroute_In
                    and RS.portion   = RP.numero
                    and RS.PR_debut >= @PR1_In
                    and RS.PR_debut <= @PR2_In
                    and RS.sens	     = @Sens_In
                    and RS.tunnel    = XDC_OUI
               end
            else --Recherche les tunnels entre 2 PR et dans 1 District
               begin
               
               /* Recherche les tunnels entre 2 PR et dans 1 District */
               
               select RS.nom_tunnel
                  from CFG..RES_POR RP, CFG..RES_SEG RS
                  where RP.autoroute = @Autoroute_In
                    and RP.district  = @District_In
                    and RS.portion   = RP.numero
                    and RS.PR_debut >= @PR1_In
                    and RS.PR_debut <= @PR2_In
                    and RS.sens	     = @Sens_In
                    and RS.tunnel    = XDC_OUI
               end        

         return XDC_OK
go            


