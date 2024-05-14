/*E*/
/*Fichier : $Id: xzao123sp.prc,v 1.5 2010/10/01 17:54:23 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/10/01 17:54:23 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao123sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* FV	07/11/94 : Creation 1.1
* ???	26/02/99 : 1.4
* JPL	01/10/10 : Retour des PR debut et fin autoroute et concession; degroupee (DEM 930) 1.5
-----------------------------------------------------------------------------*/



/*X----------------------------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Autoroutes
*
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO123' and type = 'P')
	drop procedure XZAO123
go


create procedure XZAO123
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO123SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des Autoroutes
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
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* B.G.	24/10/95	: gestion du sens inverse A57 (1.3)
* P.N.	02/12/98	: ajout colonne escota 1.4 dem/1724
* JPL	01/10/10	: Ajout PR limites en concession et globales; degroupee (DEM 930) 1.5
------------------------------------------------------*/

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
    
      return XDC_ARG_INV
   
   else   
   
   /*A
   **  Les parametres d'entree ne sont pas nuls
   */
   
   begin

	/*A Creer une table temporaire des donnees a retourner */
	create table #TMP_AUTOROUTES
	(

		numero			tinyint,
		nom			char(4),
		inversee		smallint,
		escota			tinyint,
		debut			int		NULL,
		fin			int		NULL,
		debut_concession	int		NULL,
		fin_concession		int		NULL
	)
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche des autoroutes
         */
         
         insert #TMP_AUTOROUTES
		 (numero, nom, inversee, escota)

         select	A.numero,
         	A.nom,
         	I.inversee,
         	I.escota

         from CFG..RES_AUT A, CFG..RES_AUT_INV I

         where A.numero = I.numero

         order by A.numero


         /*A
         ** Ajout des Pr des limites sur concession et globales
         */

         update #TMP_AUTOROUTES

         set debut = (select min (PR_debut) from CFG..RES_POR
                         where autoroute = #TMP_AUTOROUTES.numero),

             fin = (select max (PR_fin) from CFG..RES_POR
                        where autoroute = #TMP_AUTOROUTES.numero)


         update #TMP_AUTOROUTES

         set debut_concession	= (select min (PR_debut) from CFG..RES_POR
                                      where autoroute = #TMP_AUTOROUTES.numero
                                      and district in (select numero from CFG..RES_DIS
                                                          where type = XDC_TYPEM_PC2)
                                  ),

             fin_concession = (select max (PR_fin) from CFG..RES_POR
                                  where autoroute = #TMP_AUTOROUTES.numero
                                    and district in (select numero from CFG..RES_DIS
                                                        where type = XDC_TYPEM_PC2)
                              )
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche des autoroutes
         */
         
         insert #TMP_AUTOROUTES
		 (numero, nom, inversee, escota)

         select	A.numero,
         	A.nom,
         	I.inversee,
         	I.escota

         from CFT..RES_AUT A, CFT..RES_AUT_INV I

         where A.numero = I.numero

         order by A.numero


         /*A
         ** Ajout des Pr des limites sur concession et globales
         */

         update #TMP_AUTOROUTES

         set debut = (select min (PR_debut) from CFT..RES_POR
                         where autoroute = #TMP_AUTOROUTES.numero),

             fin = (select max (PR_fin) from CFT..RES_POR
                        where autoroute = #TMP_AUTOROUTES.numero)


         update #TMP_AUTOROUTES

         set debut_concession	= (select min (PR_debut) from CFT..RES_POR
                                      where autoroute = #TMP_AUTOROUTES.numero
                                      and district in (select numero from CFT..RES_DIS
                                                          where type = XDC_TYPEM_PC2)
                                  ),

             fin_concession = (select max (PR_fin) from CFT..RES_POR
                                  where autoroute = #TMP_AUTOROUTES.numero
                                    and district in (select numero from CFT..RES_DIS
                                                        where type = XDC_TYPEM_PC2)
                              )
      end      

#endif

   end


   /* Retourner les autoroutes dans l'ordre de selection */
   select * from #TMP_AUTOROUTES

   return XDC_OK
go  
