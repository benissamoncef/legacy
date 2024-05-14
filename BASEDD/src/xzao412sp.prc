/*E*/
/*Fichier : $Id: xzao412sp.prc,v 1.2 2018/01/09 15:57:51 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date :$Date: 2018/01/09 15:57:51 $ 
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao412sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao401sp.h" */

/* static char *version = "@(#)xzao401.c	1.1 23/18/94 : xzao401" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des destinations Temps De Parcours 
* pour tdpac (temporaire)
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO412
go

create procedure XZAO412
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO401SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des destinations Temps De Parcours 
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
* Retourne les parametres numero de destination temps De Parcours, libellees, autoroute, PR, Sens, Type
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* mismer	05 Fev 1996	: Creation (DEM/1395)
* JMG		07/06/16 : ajout des destinations ASF 1.2
------------------------------------------------------*/
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV 
   */
      
   if @va_Basedd_in = null 
      return XDC_ARG_INV
   
   else   
   
   begin

      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche de la liste des destination Temps De Parcours 
         */
         
           select distinct DTP.numero,
        	   	   rtrim(DTP.libelle_TFM),
        	   	   DTP.libelle_PMV,
        		   DTP.autoroute, 
        	   	   DTP.PR, 
        	   	   DTP.sens,
           		   DTP.type_localisation,
			   POR.district
        
           from    CFG..EQT_GEN GEN,
           	   CFG..EQT_ZDP ZDP,
           	   CFG..RES_DTP DTP,
		   CFG..RES_POR POR
          
             where GEN.type  	    =  XDC_EQT_ZDP
               and ZDP.numero	    =  GEN.numero
               and ZDP.inhibition   =  0
               and GEN.autoroute    =  DTP.autoroute
               and GEN.PR 	    <= DTP.PR 
               and DTP.PR 	    <= ZDP.PR_fin
               and DTP.sens	    =  GEN.sens
               and DTP.inhibition   =  0
               and POR.autoroute    =  DTP.autoroute
	       and POR.PR_debut     <= DTP.PR
	       and POR.PR_fin       >= DTP.PR 
           /*  order by DTP.sens, DTP.autoroute, DTP.PR, DTP.numero    */
           
	   union

	   select distinct DTP.numero,
		      rtrim(DTP.libelle_TFM),
		 DTP.libelle_PMV,
		    DTP.autoroute,
	       DTP.PR,
		  DTP.sens,
		     DTP.type_localisation,
		     201
           from    CFG..EQT_GEN GEN,
                   CFG..EQT_ZDP ZDP,
                   CFG..RES_DTP DTP

             where GEN.type         =  XDC_EQT_ZDP
               and ZDP.numero       =  GEN.numero
               and ZDP.inhibition   =  0
               and GEN.autoroute    =  DTP.autoroute
               and GEN.PR           <= DTP.PR
               and DTP.PR           <= ZDP.PR_fin
               and DTP.sens         =  GEN.sens
               and DTP.inhibition   =  0
	and DTP.autoroute not in (select autoroute from CFG..RES_POR)
	order by DTP.sens, DTP.autoroute, DTP.PR, DTP.numero	
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche de la liste des destination Temps De Parcours 
         */
         
           select distinct DTP.numero,
        	   	   DTP.libelle_TFM,
        	   	   DTP.libelle_PMV,
        		   DTP.autoroute, 
        	   	   DTP.PR, 
        	   	   DTP.sens,
           		   DTP.type_localisation,
			   POR.district
        
           from    CFT..EQT_GEN GEN,
           	   CFT..EQT_ZDP ZDP,
		   CFT..RES_DTP DTP,
		   CFT..RES_POR POR
          
             where GEN.type  	    =  XDC_EQT_ZDP
               and ZDP.numero	    =  GEN.numero
               and ZDP.inhibition   =  0
               and GEN.autoroute    =  DTP.autoroute
               and GEN.PR 	    <= DTP.PR 
               and DTP.PR 	    <= ZDP.PR_fin
               and DTP.sens	    =  GEN.sens
               and DTP.inhibition   =  0
               and POR.autoroute    =  DTP.autoroute
	       and POR.PR_debut     <= DTP.PR
	       and POR.PR_fin       >= DTP.PR
            
	   union

	   select distinct DTP.numero,
		      rtrim(DTP.libelle_TFM),
		 DTP.libelle_PMV,
		    DTP.autoroute,
	       DTP.PR,
		  DTP.sens,
		     DTP.type_localisation,
		     201
           from    CFT..EQT_GEN GEN,
                   CFT..EQT_ZDP ZDP,
                   CFT..RES_DTP DTP

             where GEN.type         =  XDC_EQT_ZDP
               and ZDP.numero       =  GEN.numero
               and ZDP.inhibition   =  0
               and GEN.autoroute    =  DTP.autoroute
               and GEN.PR           <= DTP.PR
               and DTP.PR           <= ZDP.PR_fin
               and DTP.sens         =  GEN.sens
               and DTP.inhibition   =  0
	and DTP.autoroute not in (select autoroute from CFG..RES_POR)
	order by DTP.sens, DTP.autoroute, DTP.PR, DTP.numero	
      
            
      end

#endif      
   end
      
   return XDC_OK
         
go  
